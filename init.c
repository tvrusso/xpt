/**
 *
 * xpt -- An X Periodic Table
 *
 * init.c -- initialize the colors needed
 *
 * Written by Joel P. Lord 03/05/93
 *
 *    This software is available for free distribution,
 * under the condition that this not be removed from the
 * source code.
 *
**/


#include "xpt.h"

unsigned long GetNumColor();

void init_colors()
{
  default_cmap = DefaultColormap(p_disp, DefaultScreen(p_disp));

  mbgpix = GetColors("NavyBlue", default_cmap, BP);
  mfgpix = GetColors("white", default_cmap, WP);
  red = GetColors("red", default_cmap, WP);
  white = mfgpix;
  slate_grey = GetColors("slategrey", default_cmap, BP);
}

unsigned long GetNumColor(num, cmap, def)
unsigned long num;
Colormap cmap;
unsigned long def;
{
  XColor color;
  unsigned long retval;

  color.pixel = num;
  color.red = ((num >> 16) * 256);
  color.green = ((num >> 8) & 0xFF) * 256;
  color.blue = (num & 0xFF) * 256;
  color.flags = DoBlue | DoGreen | DoRed;

  if (XAllocColor(p_disp, cmap, &color) == 0)
    retval = def;
  else
    retval = color.pixel;

  return retval;
}
