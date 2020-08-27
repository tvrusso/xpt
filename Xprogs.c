/**
 *
 * xpt -- An X Periodic Table
 *
 *  Modularized X Windows functions
 *
 * GetColors(name, cmap, def)
 *  Allocates color "name", or uses color "def" if "name" is unavailable
 *  char *name		name of color to be allocated
 *  Colormap cmap	colormap to get "name" from
 *  unsigned long def	default color to use if "name" is unavailable
 *		  	  probably the output from a function like "BlackPixel"
 *
 * XFontStruct *GetFont(fontname)
 *  Load a font for use in X Windows
 *  char *fontname	name of font to load
 *
 * GC CreateGC(window, font, fgcolor, bgcolor)
 *  Create a Graphics Context for a window
 *  Window window;		the window to create the graphics context for
 *  XFontStruct font;		The default font for the window
 *  unsigned long fgcolor, bgcolor;
 *
 *  Joel P. Lord 03/05/93
 *
**/

#include <stdlib.h>
#define Xprogs
#include "Xprogs.h"

unsigned long GetColors(char *name, Colormap cmap, unsigned long def)
{
  XColor color;
  unsigned long retval;

  if (XParseColor(p_disp, cmap, name, &color) == 0 || 
      XAllocColor(p_disp, cmap, &color) == 0)
    retval = def;
  else
    retval = color.pixel;
  return retval;
}

XFontStruct *GetFont(char *fname)
{
  XFontStruct *tempfont;

  if ((tempfont = XLoadQueryFont(p_disp,fname)) == NULL)
    {
      fputs("Can't open font",stderr);
      fputs(fname,stderr);
      exit(1);
    }
  return tempfont;
}

GC CreateGC(Window wind, XFontStruct *fntstruct, unsigned long fgpix, unsigned long bgpix)
{
  XGCValues gcv;

  gcv.font = fntstruct->fid;
  gcv.foreground = fgpix;
  gcv.background = bgpix;

  return(XCreateGC(p_disp, wind, (GCFont | GCForeground | GCBackground), &gcv));
}
