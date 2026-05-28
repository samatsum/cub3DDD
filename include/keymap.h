/* keymap.h */
#ifndef KEYMAP_H
# define KEYMAP_H

# if defined(__linux__)
/* ========================================= */
/* Linux (X11) Settings                      */
/* ========================================= */

/* Key Codes */
#  define KEY_ESC       65307
#  define KEY_W         119
#  define KEY_A         97
#  define KEY_S         115
#  define KEY_D         100
#  define KEY_LEFT      65361
#  define KEY_RIGHT     65363
#  define KEY_FORWARD   65362
#  define KEY_BACKWARD  65364
#  define KEY_Q         113
#  define KEY_E         101
#  define KEY_I         105
#  define KEY_L         108
#  define KEY_O         111

/* Events */
#  define EVENT_CLOSE   33  /* X11 ClientMessage */
#  define EVENT_EXPOSE  12  /* ← 追加: 画面の再描画要求イベント */

/* Event Masks */
#  define MASK_KEY_PRESS     (1L << 0)
#  define MASK_KEY_RELEASE   (1L << 1)
#  define MASK_CLOSE         (1L << 17)
#  define MASK_EXPOSE        (1L << 15) /* ← 追加: Expose用マスク */

# elif defined(__APPLE__)
/* ========================================= */
/* macOS (Cocoa) Settings                    */
/* ========================================= */

/* Key Codes */
#  define KEY_ESC       53
#  define KEY_W         13
#  define KEY_A         0
#  define KEY_S         1
#  define KEY_D         2
#  define KEY_LEFT      123
#  define KEY_RIGHT     124
#  define KEY_FORWARD   126
#  define KEY_BACKWARD  125
#  define KEY_Q         12
#  define KEY_E         14
#  define KEY_I         34
#  define KEY_L         37
#  define KEY_O         31

/* Events */
#  define EVENT_CLOSE   17

/* Event Masks (Macはマスクを無視するため0で統一) */
#  define MASK_KEY_PRESS     0
#  define MASK_KEY_RELEASE   0
#  define MASK_CLOSE         0

# endif

/* ========================================= */
/* Common Events (OS Independent)            */
/* ========================================= */
# define EVENT_KEY_PRESS   2
# define EVENT_KEY_RELEASE 3

#endif