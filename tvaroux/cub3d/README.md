_This project has been created as part of the 42 curriculum by hyamamot, oo_

[English](README.md) | [Japanese](README.ja.md)

## Table of Contents

- [Description](#description)
- [Instructions](#instructions)
- [Resources](#resources)

---

## Description

### About the Project

**cub3d** is a project from the 42 curriculum.  
Inspired by *Wolfenstein 3D*, the legendary FPS developed by id Software in 1992,
it implements a pseudo-3D first-person viewer in C using the raycasting algorithm.

### Goals

- Understand and implement the raycasting algorithm
- Learn to use **MiniLibX**, the official 42 graphics library
- Practice memory management and file parsing in C
- Write code conforming to the Norminette (42 coding standard)

### Feature Overview

| Feature | Details |
|---|---|
| 3D rendering | Pseudo-3D first-person view via raycasting |
| Textures | Individual XPM textures for North / South / East / West walls |
| Ceiling & floor color | RGB color specified in the `.cub` file |
| Player movement | `W` `A` `S` `D` for forward / backward / strafe |
| View rotation | Left / Right arrow keys |
| Map validation | Closed-map integrity check using DFS |
| Exit | `ESC` key or window close button |

### Directory Structure

```
cub3d/
├── include/        Header files
├── src/
│   ├── init/       .cub file parsing and map initialisation
│   ├── raycasting/ Main loop, raycasting, rendering
│   ├── utils/      Custom library functions (ft_*) and get_next_line
│   └── valid/      Map validation (DFS)
├── texture/        XPM texture images
└── in.cub          Sample map file
```

### `.cub` File Format

```
NO ./texture/north.xpm   # North wall texture
SO ./texture/south.xpm   # South wall texture
WE ./texture/west.xpm    # West wall texture
EA ./texture/east.xpm    # East wall texture

F 220,100,0              # Floor colour (R,G,B)
C 225,30,0               # Ceiling colour (R,G,B)

11111                    # Map (1=wall, 0=floor, N/S/E/W=start position & direction)
10N01
11111
```

---

## Instructions

### Dependencies

- `gcc`
- `make`
- X11 development libraries: `libx11-dev`, `libxext-dev`
- MiniLibX (located at `/workspace/hyamamot/minilibx-linux` inside the repo)

```bash
# Ubuntu / Debian
sudo apt-get install gcc make libx11-dev libxext-dev
```

### Compilation

```bash
cd cub3d
make
```

### Execution

```bash
./cub3d <map_file.cub>

# Example
./cub3d in.cub
```

### Cleanup

```bash
make clean    # Remove object files
make fclean   # Remove object files and the executable
make re       # Full rebuild
```

### Controls

| Key | Action |
|---|---|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Strafe left |
| `D` | Strafe right |
| `←` | Rotate left |
| `→` | Rotate right |
| `ESC` | Quit |

---

## Resources

| # | Title | Purpose |
|---|---|---|
| [1] | [Cub3D - C言語で3DFPSゲーム作ってみた](https://zenn.dev/rt3mis10/articles/3482cc49e288ea) | Implementation example and overall design reference for the same assignment |
| [2] | [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx) | MiniLibX API reference and sample code |
| [3] | [Adobe: レイキャスティングとは？](https://www.adobe.com/jp/products/substance3d/discover/what-is-ray-casting.html) | Conceptual overview of raycasting |
| [4] | [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) | Primary algorithmic reference for raycasting from theory to implementation |
| [5] | [釧路高専 レイキャスティング](https://www.kushiro-ct.ac.jp/yanagawa/cg-2015/1403/index.html) | Japanese-language explanation of the raycasting algorithm |
| [6] | [GitHub: Raycasting Prototype with DDA](https://github.com/GPMLAD/raycasting) | Implementation example of DDA-based raycasting |
| [7] | [釧路高専 線分の描画 DDA](https://www.kushiro-ct.ac.jp/yanagawa/cg/03/index.html) | Detailed explanation of the DDA algorithm for line drawing |
| [8] | [Qiita: C言語でFPSに依存しないアニメーションをする](https://qiita.com/Tanoren/items/10a4f610b9d3c5f744d0) | Frame-time-based, frame-rate-independent movement speed control |
| [9] | [DeepWiki: cub3d Flood Fill Algorithm](https://deepwiki.com/eltitoyisus/cub3D/3.2-flood-fill-algorithm) | Reference for map validation via DFS / Flood Fill |
| [10] | [C言語で3Dゲームを作った](https://jun-networks.hatenablog.com/entry/2021/03/04/130629) | Article on implementing a 3D game in C |

### Use of AI

This project used **GitHub Copilot (Claude Sonnet 4.6)** for the following tasks:

| Task | Details |
|---|---|
| Code refactoring | Batch-fixing Norminette violations (camelCase names, lines too long, variable declarations outside function scope) |
| Naming convention migration | Converting `posX/Y`, `dirX/Y`, `planeX/Y` etc. from camelCase to snake_case across all files |
| Bug fixing | Identifying and fixing an infinite loop caused by `continue` skipping the `get_next_line` call at the end of a loop |
| README authoring | Generating this document in both Japanese and English |

The core algorithmic work (raycasting, DFS validation, map parsing) was implemented
independently. AI assistance was primarily used for style conformance, debugging, and
documentation.
