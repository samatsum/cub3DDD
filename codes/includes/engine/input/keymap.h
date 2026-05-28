#ifndef ENGINE_INPUT_KEYMAP_H
# define ENGINE_INPUT_KEYMAP_H

/* X11 Events (types.h から移動) */
# define X_EVENT_KEY_PRESS		2
# define X_EVENT_KEY_RELEASE	3
# define X_EVENT_MOUSE_PRESS	4
# define X_EVENT_MOUSE_RELEASE	5
# define X_EVENT_MOUSE_MOVE		6
# define X_EVENT_EXIT			17

/* Mouse Buttons (cub3d.h から移動) */
# define LEFT_CLICK     1
# define RIGHT_CLICK    2
# define MIDDLE_CLICK   3
# define SCROLL_UP      4
# define SCROLL_DOWN    5

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

/* ========================================= */
/* Common Events (OS Independent)            */
/* ========================================= */
# define EVENT_KEY_PRESS   2
# define EVENT_KEY_RELEASE 3

#endif