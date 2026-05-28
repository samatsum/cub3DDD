#ifndef ENGINE_H
# define ENGINE_H

/* ** 構造体とマクロの基盤 */
# include "types.h"

/* ** 分割された機能別レイヤー */
/* 修正: サブディレクトリへのパスを明記する */
# include "engine/raycast/engine_raycast.h"
# include "engine/render/engine_render.h"
# include "engine/texture/engine_texture.h"
# include "ui/ui.h"

#endif