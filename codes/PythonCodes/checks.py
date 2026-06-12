"""checks — 9 本の検査スクリプトを 1 つに統合した検査レジストリ。

旧 check_includes.py / check_pointer_notation.py / check_style.py /
check_signatures.py / check_separators.py / check_duplicates.py /
check_magic_numbers.py / check_unused_funcs.py / check_static.py を、
それぞれ「Finding を返す純粋関数」へ書き直して 1 ファイルに集約した。

各関数は @check デコレータで CHECKS に登録され、lint.py がまとめて実行する。
共有ロジック（コメント除去・ファイル走査・正規表現）は cutils に集約済み。
"""

from __future__ import annotations

import re
from collections import defaultdict
from typing import Callable, Iterable, List

from cutils import (
    FUNC_DEF_RE,
    MACRO_DEF_RE,
    STATIC_DEF_RE,
    Context,
    Finding,
    Severity,
    iter_func_defs,
    separator_status,
)

# --------------------------------------------------------------------------- #
# 検査レジストリと登録デコレータ
# --------------------------------------------------------------------------- #

CheckFn = Callable[[Context], Iterable[Finding]]


class Check:
    """検査 1 つ分のメタ情報＋実行関数。"""

    def __init__(self, name: str, summary: str, fn: CheckFn) -> None:
        self.name = name
        self.summary = summary
        self.fn = fn

    def run(self, ctx: Context) -> List[Finding]:
        return list(self.fn(ctx))


CHECKS: List[Check] = []


def check(name: str, summary: str) -> Callable[[CheckFn], CheckFn]:
    """検査関数を CHECKS に登録するデコレータ。"""

    def deco(fn: CheckFn) -> CheckFn:
        CHECKS.append(Check(name, summary, fn))
        return fn

    return deco


# 小さなヘルパ：Finding を簡潔に生成する
def _err(path, line, message, detail=None) -> Finding:
    return Finding(Severity.ERROR, path, line, message, detail)


def _warn(path, line, message, detail=None) -> Finding:
    return Finding(Severity.WARNING, path, line, message, detail)


# --------------------------------------------------------------------------- #
# include 順序（共通ルール 2）：標準ヘッダは自作ヘッダより前
# --------------------------------------------------------------------------- #


@check("includes", '標準ヘッダ <> は自作ヘッダ "" より前に置く')
def check_includes(ctx: Context) -> Iterable[Finding]:
    for sf in ctx.select(("srcs", "includes"), (".c", ".h")):
        seen_local = False
        for n, line in enumerate(sf.raw.splitlines(), 1):
            s = line.strip()
            if not s.startswith("#include"):
                continue
            if '"' in s:
                seen_local = True
            elif "<" in s and ">" in s and seen_local:
                yield _err(
                    sf.path,
                    n,
                    "自作ヘッダの後に標準ヘッダが置かれている",
                    f'{s} は自作ヘッダ("")より前に書く',
                )


# --------------------------------------------------------------------------- #
# ポインタ表記（共通ルール 4）：'*' は型名側に寄せる
# --------------------------------------------------------------------------- #

_PTR_TYPE_RE = re.compile(
    r"\b(int|char|float|double|void|long|short|unsigned|signed|size_t"
    r"|t_[a-zA-Z0-9_]+)\s+\*"
)
_PTR_STRUCT_RE = re.compile(r"\bstruct\s+s_[a-zA-Z0-9_]+\s+\*")


@check("pointer", "'*' は型名側に寄せる（int* ptr;）")
def check_pointer(ctx: Context) -> Iterable[Finding]:
    for sf in ctx.select(("srcs", "includes"), (".c", ".h")):
        for n, line in enumerate(sf.no_comments_strings.splitlines(), 1):
            if _PTR_TYPE_RE.search(line) or _PTR_STRUCT_RE.search(line):
                yield _err(
                    sf.path,
                    n,
                    "ポインタ '*' が型名から離れている",
                    f"{line.strip()} → 'int* ptr;' の形にする",
                )


# --------------------------------------------------------------------------- #
# スタイル（共通ルール 3 / 7 / 8）：制御構文の空白・else 形・終端改行
# --------------------------------------------------------------------------- #

_MISSING_SPACE_RE = re.compile(r"\b(if|for|while)\(")
_ELSE_RE = re.compile(r"\belse\b")


@check("style", "制御構文の空白 / else 形 / 終端改行（ルール 3,7,8）")
def check_style(ctx: Context) -> Iterable[Finding]:
    for sf in ctx.select(("srcs", "includes"), (".c", ".h")):
        # [ルール 8] 終端改行（生テキストで判定）
        if sf.raw and not sf.raw.endswith("\n"):
            yield _err(sf.path, None, "ファイル終端が改行で終わっていない")

        for n, line in enumerate(sf.no_comments_strings.splitlines(), 1):
            s = line.strip()
            if not s:
                continue
            # [ルール 3] if/for/while の直後に空白がない
            m = _MISSING_SPACE_RE.search(s)
            if m:
                kw = m.group(1)
                yield _err(
                    sf.path,
                    n,
                    f"'{kw}' の直後に空白がない",
                    f"'{kw} (' と書く",
                )
            # [ルール 7] else は '} else {' / '} else if' の形
            if _ELSE_RE.search(s) and "} else {" not in s and "} else if" not in s:
                yield _err(
                    sf.path,
                    n,
                    "else の形が不正",
                    "'} else {' または '} else if' にする",
                )


# --------------------------------------------------------------------------- #
# シグネチャ（ルール 6）：戻り値型と関数名を別行に、名前行は先頭タブ
# --------------------------------------------------------------------------- #

_SIG_SAME_LINE_RE = re.compile(r"^([a-zA-Z_][a-zA-Z0-9_\s\*]*?)\s+([a-zA-Z_]\w*)\s*\(")
_SIG_NO_TAB_RE = re.compile(r"^([a-zA-Z_]\w*)\s*\(")
_SIG_IGNORE = {"if", "while", "for", "switch", "return", "sizeof", "else"}


@check("signatures", "戻り値型と関数名は別行・名前行は先頭タブ（ルール 6）")
def check_signatures(ctx: Context) -> Iterable[Finding]:
    for sf in ctx.select(("srcs", "includes"), (".c", ".h")):
        for n, line in enumerate(sf.no_comments_strings.splitlines(), 1):
            if not line.strip() or line.lstrip().startswith("#"):
                continue

            m = _SIG_SAME_LINE_RE.match(line)
            if m:
                first_word = m.group(1).split()[0]
                func_name = m.group(2)
                if first_word not in _SIG_IGNORE and func_name not in _SIG_IGNORE:
                    yield _err(
                        sf.path,
                        n,
                        "戻り値型と関数名が同じ行にある",
                        f"{line.strip()} → 型と名前を別行に分ける",
                    )
                    continue

            m = _SIG_NO_TAB_RE.match(line)
            if m and m.group(1) not in _SIG_IGNORE:
                yield _err(
                    sf.path,
                    n,
                    "関数名の行に先頭タブがない",
                    f"{line.strip()} → 先頭にタブ 1 つを入れる",
                )


# --------------------------------------------------------------------------- #
# セパレータ＆コメント（.cルール 9,10）：関数定義の直前に区切りと // コメント
# --------------------------------------------------------------------------- #


@check("separators", "関数定義の直前にセパレータと // コメント（.c ルール 9,10）")
def check_separators(ctx: Context) -> Iterable[Finding]:
    for sf in ctx.select(("srcs",), (".c",)):
        lines = sf.raw.split("\n")
        for name, fidx in iter_func_defs(sf.raw):
            st = separator_status(lines, fidx)
            if st.status == "ok":
                continue
            if st.status in ("no_separator", "no_doc"):
                yield _err(
                    sf.path,
                    fidx + 1,
                    f"関数 '{name}' の前にセパレータが無い/不正",
                )
            if st.status in ("no_comment", "no_doc"):
                yield _err(
                    sf.path,
                    fidx + 1,
                    f"関数 '{name}' の直前に '//' コメントがない",
                )


# --------------------------------------------------------------------------- #
# 重複・名前衝突（マクロ二重定義 / 関数の多重定義）
# --------------------------------------------------------------------------- #


@check("duplicates", "マクロ二重定義・関数の多重定義/名前衝突を検出")
def check_duplicates(ctx: Context) -> Iterable[Finding]:
    macro_defs: dict[str, list] = defaultdict(list)
    func_defs: dict[str, list] = defaultdict(list)

    for sf in ctx.select(("srcs", "includes"), (".c", ".h")):
        for m in MACRO_DEF_RE.finditer(sf.no_comments):
            macro_defs[m.group(1)].append(sf.path)
        if sf.suffix == ".c":
            for m in FUNC_DEF_RE.finditer(sf.no_comments):
                ret_type = m.group(1).strip()
                is_static = "static" in ret_type.split()
                func_defs[m.group(2)].append((sf.path, is_static))

    for macro, paths in macro_defs.items():
        unique = list(set(paths))
        if len(unique) > 1:
            detail = "\n        ".join(p.name for p in unique)
            yield _err(
                None,
                None,
                f"マクロ '{macro}' が複数ファイルで定義されている",
                detail,
            )
        elif len(paths) > 1:
            yield _warn(
                paths[0],
                None,
                f"マクロ '{macro}' が同一ファイル内で再定義されている",
            )

    for func, locs in func_defs.items():
        if len(locs) <= 1:
            continue
        globals_ = [p for p, is_static in locs if not is_static]
        if len(globals_) > 1:
            detail = "\n        ".join(p.name for p in globals_)
            yield _err(
                None,
                None,
                f"グローバル関数 '{func}' が多重定義されている",
                detail,
            )
        else:
            detail = "\n        ".join(
                f"{p.name} ({'static' if s else 'global'})" for p, s in locs
            )
            yield _warn(
                None,
                None,
                f"関数 '{func}' の名前が複数ファイルで衝突している",
                detail,
            )


# --------------------------------------------------------------------------- #
# マジックナンバー（可読性）：0 / 1 / -1 以外の直書き数値
# --------------------------------------------------------------------------- #

_NUMBER_RE = re.compile(r"\b(-?(?!0\b|1\b|-1\b)\d+)\b")
_STRING_RE = re.compile(r'".*?"')


@check("magic", "0/1/-1 以外の直書き数値（マジックナンバー）を検出")
def check_magic_numbers(ctx: Context) -> Iterable[Finding]:
    for sf in ctx.select(("srcs",), (".c",)):
        for n, line in enumerate(sf.no_comments.splitlines(), 1):
            s = line.strip()
            if not s or s.startswith("#"):
                continue
            code = _STRING_RE.sub('""', s)
            numbers = _NUMBER_RE.findall(code)
            if numbers:
                yield _warn(
                    sf.path,
                    n,
                    "マジックナンバーを検出",
                    f"{s}  →  {', '.join(numbers)}",
                )


# --------------------------------------------------------------------------- #
# 未使用関数（デッドコード）：どこからも呼ばれていない関数
# --------------------------------------------------------------------------- #


@check("unused", "どこからも呼ばれていない関数（デッドコード）を検出")
def check_unused_funcs(ctx: Context) -> Iterable[Finding]:
    files = ctx.select(("srcs",), (".c",))
    defined: dict[str, "object"] = {}
    blobs: list[str] = []

    for sf in files:
        blobs.append(sf.no_comments)
        for m in FUNC_DEF_RE.finditer(sf.no_comments):
            defined[m.group(2)] = sf.path

    blob = "\n".join(blobs)
    for func, path in defined.items():
        if func == "main":
            continue
        # 定義 1 回ぶんしか出現しない＝呼び出しがない
        if len(re.findall(rf"\b{func}\b", blob)) <= 1:
            yield _warn(path, None, f"未使用関数 '{func}' は一度も呼ばれていない")


# --------------------------------------------------------------------------- #
# static 漏洩（カプセル化）：.c の static 関数が .h に公開されていないか
# --------------------------------------------------------------------------- #


@check("static-leak", ".c の static 関数が .h に漏れていないか（情報隠蔽）")
def check_static_leak(ctx: Context) -> Iterable[Finding]:
    static_funcs: dict[str, "object"] = {}
    for sf in ctx.select(("srcs",), (".c",)):
        for func_name in STATIC_DEF_RE.findall(sf.no_comments):
            static_funcs[func_name] = sf.path

    for sf in ctx.select(("includes",), (".h",)):
        clean = sf.no_comments
        for func_name, src_path in static_funcs.items():
            decl_re = re.compile(rf"^[^\n]+\n\t{func_name}\s*\(", re.MULTILINE)
            if decl_re.search(clean):
                yield _err(
                    sf.path,
                    None,
                    f"static 関数 '{func_name}' がヘッダに漏れている",
                    f"定義元: {src_path.name}",
                )
