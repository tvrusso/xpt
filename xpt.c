/**
 *
 *  xpt -- An X Periodic Table
 *
 * Written by Joel P. Lord 03/05/93
 *
 *    This software is available for free distribution,
 * under the condition that this not be removed from the
 * source code.
 *
**/

#define MAIN
#include <stdio.h>
#include "xpt.h"
#include "init.h"
#include "table.h"

int main(int argc, char **argv)
{
  Arg args[20];
  int n = 0;
  XGCValues gcv;

  toplevel = XtInitialize("xpt", "X Periodic Table",
			  NULL, 0, &argc, argv);

  p_disp = XtDisplay(toplevel);

  mfontstruct = (XFontStruct *)GetFont(HELV_MEDIUM_12);
  BigFont = (XFontStruct *)GetFont(HELV_BOLD_24);
  mfontheight = mfontstruct->max_bounds.ascent +
    mfontstruct->max_bounds.descent;

  init_colors();

  XtSetArg(args[n], XtNfont, mfontstruct);
  n++;
  XtSetArg(args[n], XtNheight, 320);
  n++;
  XtSetArg(args[n], XtNwidth, 480);
  n++;
  XtSetArg(args[n], XtNbackground, mbgpix);
  n++;
  XtSetArg(args[n], XtNforeground, mfgpix);
  n++;

  MainW = XtCreateManagedWidget("Xpt", formWidgetClass, toplevel, args, n);

  XtRealizeWidget(toplevel);

  theGC = (GC)CreateGC(XtWindow(MainW), mfontstruct, mfgpix, mbgpix);
  BigGC = (GC)CreateGC(XtWindow(MainW), BigFont, mfgpix, mbgpix);

  init_table();

  done = 0;

  while (!done)
    {
      XtNextEvent(&theEvent);
      XtDispatchEvent(&theEvent);
    }
}
