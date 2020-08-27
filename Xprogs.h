/**
 *
 *  Definitions of some useful data types for Xlib programming,
 *    as well as a few necessary variables (display name, etc)
 *
 *
 *  Joel P. Lord 2/6/93
 *
**/

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include "fontlist.h"

#define WP	WhitePixel(p_disp, DefaultScreen(p_disp))
#define BP	BlackPixel(p_disp, DefaultScreen(p_disp))

typedef struct XWIN
{
  Window	xid;
  Window	parent;
  void		*data;
  void		(*event_handler)();
  GC		button_gc;
} XWIN;

typedef struct Pix
{
  Pixmap	image;
  unsigned	width;
  unsigned	height;
} Pix;

typedef struct D_BUTTON
{
  char		*label;
  int		(*action)();
  caddr_t	action_args;
  Pix		*pix;
} D_BUTTON;

#ifdef MAIN

XFontStruct *mfontstruct;
unsigned mfontheight;
Display *p_disp;
Window Main;
GC theGC;
XEvent theEvent;
char default_geometry[80];
unsigned long mbgpix, mfgpix;
XContext xwin_context;

#else

extern XFontStruct *mfontstruct;
extern unsigned mfontheight;
extern Display *p_disp;
extern Window Main;
extern GC theGC;
extern XEvent theEvent;
extern char default_geometry[80];
extern unsigned long mbgpix, mfgpix;
extern XContext xwin_context;

#endif
unsigned long GetColors(char *name, Colormap cmap, unsigned long def);
XFontStruct *GetFont(char *fname);
GC CreateGC(Window wind, XFontStruct *fntstruct, unsigned long fgpix, unsigned long bgpix);

