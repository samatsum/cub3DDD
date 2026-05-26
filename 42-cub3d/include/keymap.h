/* keymap.h */
#ifndef KEYMAP_H
# define KEYMAP_H

# if defined(__linux__)
/* Linux (X11) Key Codes */
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
#  define EVENT_CLOSE   33  /* X11 ClientMessage */

# elif defined(__APPLE__)
/* macOS (Cocoa) Key Codes */
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
#  define EVENT_CLOSE   17

# endif

/* イベントフック用の共通コード */
# define EVENT_KEY_PRESS 2
# define EVENT_KEY_RELEASE 3

#endif