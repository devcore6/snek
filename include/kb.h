#ifndef __KB_H
# define __KB_H
# include <stdbool.h>

# define PS2_W 0x11
# define PS2_A 0x1E
# define PS2_S 0x1F
# define PS2_D 0x20
# define PS2_ENTER 0x1c
# define PS2_SPACE 0x39

# define PS2_RELEASE_W 0x91
# define PS2_RELEASE_A 0x9E
# define PS2_RELEASE_S 0x9F
# define PS2_RELEASE_D 0xA0
# define PS2_RELEASE_ENTER 0x9C

extern bool _w;
extern bool _a;
extern bool _s;
extern bool _d;
extern bool _enter;

#endif
