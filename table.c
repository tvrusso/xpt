/**
 *
 * xpt -- An X Periodic Table
 *
 * table.c - Create the periodic table, and handle data requests
 *
 * Written bu Joel P. Lord 03/05/93
 *
 *    This software is available for free distribution,
 * under the condition that this not be removed from the
 * source code.
 *
**/

#include <stdlib.h>
#include "xpt.h"
#include <X11/Xaw/AsciiText.h>
#include "table.h"

void init_table()
{
  Widget elements[104];
  Widget quit_button;
  int n = 0, i = 0, j = 0, k = 0, horiz_offset = 5;
  Arg args[20];
  char title[3];
  void element_choice(), quit();
  XEvent tevent;
  static char *table[] = {
"H                                 He",
"LiBe                    B C N O F Ne",
"NaMg                    AlSiP S ClAr",
"K CaScTiV CrMnFeCoNiCuZnGaGeAsSeBrKr",
"RbSrY ZrNbMoTcRuRhPdAgCdInSnSbTeI Xe",
"CsBaLaHfTaW ReOsIrPtAuHgTlPbBiPoAtRn",
"FrRaAc                              ",
"                                    ",
"        CePrNdPmSmEuGdTbDyHoErTmYbLu",
"        ThPaU NpPuAmCmBkCfEsFmMdNoLr"
};

  n = 0;
  XtSetArg(args[n], XtNfromHoriz, NULL);
  n++;
  XtSetArg(args[n], XtNfromVert, NULL);
  n++;
  XtSetArg(args[n], XtNhorizDistance, 5);
  n++;
  XtSetArg(args[n], XtNvertDistance, 5);
  n++;
  XtSetArg(args[n], XtNwidth, 24);
  n++;
  XtSetArg(args[n], XtNheight, 32);
  n++;
  mbgpix = GetColors("NavyBlue", default_cmap, BP);
  XtSetArg(args[n], XtNbackground, slate_grey);
  n++;
  XtSetArg(args[n], XtNforeground, mfgpix);
  n++;
  XtSetArg(args[n], XtNfont, mfontstruct);
  n++;

  title[2] = 0;
  for (j = 0; j < 10; j++)
    {
      for(i = 0; i < 36; i += 2)
	{
	  title[0] = table[j][i];
	  title[1] = table[j][i+1];
	  if (title[0] != ' ')
	    {
	      elements[k] = XtCreateWidget(title, commandWidgetClass,
					   MainW, args, n);
	      XtAddCallback(elements[k], XtNcallback, element_choice, k);
	      XtRealizeWidget(elements[k]);
	      XtManageChild(elements[k]);
	      k++;
	      horiz_offset += 26;
	      XtSetArg(args[2], XtNhorizDistance, horiz_offset);
	    }
	  else
	    {
	      horiz_offset += 26;
	      XtSetArg(args[2], XtNhorizDistance, horiz_offset);
	    }
	}
      horiz_offset = 0;
      XtSetArg(args[0], XtNfromHoriz, NULL);
      XtSetArg(args[1], XtNfromVert, elements[k-1]);
      XtSetArg(args[2], XtNhorizDistance, 5);
      XtSetArg(args[3], XtNvertDistance, 0);
    }

  XtSetArg(args[0], XtNfromHoriz, NULL);
  XtSetArg(args[1], XtNfromVert, NULL);
  XtSetArg(args[2], XtNhorizDistance, 160);
  XtSetArg(args[3], XtNvertDistance, 5);
  XtSetArg(args[4], XtNwidth, 49);
  XtSetArg(args[5], XtNheight, 32);
  XtSetArg(args[6], XtNbackground, red);
  XtSetArg(args[7], XtNforeground, white);
  XtSetArg(args[9], XtNborderColor, red);
  
  quit_button = XtCreateManagedWidget("Quit", commandWidgetClass, MainW,
				      args, n + 1);
  XtRealizeWidget(quit_button);
  XtAddCallback(quit_button, XtNcallback, quit, 0);
}

static int destroy_info;

void element_choice(Widget w, int element, XEvent *event)
{
  Widget element_info, ok_button, help_button;
  Arg args[20];
  char *data_buf;
  void ok(), help();

  destroy_info = 0;

  data_buf = (char *)malloc(1000);

  element_info = XtVaCreateManagedWidget("Element Info",formWidgetClass, MainW,
				       XtNfromHoriz, NULL,
				       XtNfromVert, NULL,
				       XtNwidth, 200,
				       XtNheight, 250,
				       XtNvertDistance, 60,
				       XtNhorizDistance, 240,
				       XtNbackground, mbgpix,
				       XtNforeground, mfgpix,
				       XtNfont, mfontstruct,
				       XtNborderColor, slate_grey, NULL);
  display_info(element_info, element);

  ok_button = XtVaCreateManagedWidget("Done", commandWidgetClass, element_info,
				    XtNfromHoriz, NULL,
				    XtNfromVert, NULL,
				    XtNwidth, 40,
				    XtNheight, 32,
				    XtNvertDistance, 210,
				    XtNhorizDistance, 100,
				    XtNbackground, slate_grey,
				    XtNforeground, mfgpix,
				    XtNfont, mfontstruct,
				    XtNborderColor, mbgpix, NULL);

  XtAddCallback(ok_button, XtNcallback, ok, 0);

  help_button = XtVaCreateManagedWidget("Help", commandWidgetClass,
					element_info,
					XtNfromHoriz, NULL,
					XtNfromVert, NULL,
					XtNwidth, 40,
					XtNheight, 32,
					XtNvertDistance, 210,
					XtNhorizDistance, 60,
					XtNbackground, slate_grey,
					XtNforeground, mfgpix,
					XtNfont, mfontstruct,
					XtNborderColor, mbgpix, NULL);
  XtAddCallback(help_button, XtNcallback, help, 0);

  while (!destroy_info && !done)
    {
      XtNextEvent(&theEvent);
      XtDispatchEvent(&theEvent);
      if (theEvent.type == Expose)
	display_info(element_info, element);
    }
  XtDestroyWidget(element_info);
  free(data_buf);
}

void ok(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{
  destroy_info = 1;
}

void quit(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
  done = 1;
}

static unsigned destroy_help = 1;
static Widget help_window;

void help(w, client_data, call_data)
Widget w;
XtPointer client_data, call_data;
{
  if (!destroy_help)
    {
      XtDestroyWidget(help_window);
      destroy_help = 1;
    }
  else
    {
      destroy_help = 0;

      help_window = XtVaCreateManagedWidget("Element Info",formWidgetClass,
					    MainW,
					    XtNfromHoriz, NULL,
					    XtNfromVert, NULL,
					    XtNwidth, 200,
					    XtNheight, 250,
					    XtNvertDistance, 60,
					    XtNhorizDistance, 40,
					    XtNbackground, mbgpix,
					    XtNforeground, mfgpix,
					    XtNfont, mfontstruct,
					    XtNborderColor, slate_grey, NULL);
      display_help(help_window);
      
      while (!destroy_info && !done && !destroy_help)
	{
	  XtNextEvent(&theEvent);
	  XtDispatchEvent(&theEvent);
	  if(theEvent.type == Expose)
	    display_help(help_window);
	}
      XtDestroyWidget(help_window);
      destroy_help = 1;
    }
}

void display_help(Widget w)
{
  XDrawString(p_disp, XtWindow(w), theGC, 10, 10 + mfontheight, "Number", 6);
  XDrawString(p_disp, XtWindow(w), BigGC,
	      100 - XTextWidth(BigFont, "Sy", 2) / 2, 110 + mfontheight,
	      "Sy", 2);
  XDrawString(p_disp, XtWindow(w), theGC, 
	      190 - XTextWidth(mfontstruct, "Mass", 4),
	      10 + mfontheight, "Mass", 4);
  XDrawString(p_disp, XtWindow(w), theGC, 10, 60 + mfontheight, "BP", 2);
  XDrawString(p_disp, XtWindow(w), theGC, 10, 100 + mfontheight, "MP", 2);
  XDrawString(p_disp, XtWindow(w), theGC, 10, 140 + mfontheight, "Density", 7);

  XDrawString(p_disp, XtWindow(w), theGC,
	      185 - XTextWidth(mfontstruct, "Atomic", 6),
	      55 + mfontheight, "Atomic", 6);
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, "Radius", 6),
	      55 + 2 * mfontheight, "Radius", 6);

  XDrawString(p_disp, XtWindow(w), theGC,
	      185 - XTextWidth(mfontstruct, "Covalent", 8),
	      95 + mfontheight, "Covalent", 8);
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, "Radius", 6),
	      95 + 2 * mfontheight, "Radius", 6);

  XDrawString(p_disp, XtWindow(w), theGC,
	      185 - XTextWidth(mfontstruct, "Atomic", 6),
	      135 + mfontheight, "Atomic", 6);
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, "Volume", 6),
	      135 + 2 * mfontheight, "Volume", 6);

  XDrawString(p_disp, XtWindow(w), theGC,
	      100 - XTextWidth(mfontstruct, "Name", 4) / 2,
	      190 + mfontheight, "Name", 4);
}

void display_info(Widget w, unsigned element)
{
  char *temp_buf, *temp2;

  temp_buf = (char *)malloc(256);

  sprintf(temp_buf, "%d", p_table[element].atomic_number);
  XDrawString(p_disp, XtWindow(w), theGC, 10, 10 + mfontheight, temp_buf,
	    strlen(temp_buf));

  sprintf(temp_buf, "%s", p_table[element].symbol);
  temp2 = strchr(temp_buf, ' ');
  *temp2 = 0;
  XDrawString(p_disp, XtWindow(w), BigGC,
	      100 - XTextWidth(BigFont, temp_buf, strlen(temp_buf)) / 2,
	      110 + mfontheight, temp_buf, strlen(temp_buf));

  sprintf(temp_buf, "%1.4lf", p_table[element].atomic_mass);
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, temp_buf, strlen(temp_buf)),
	      10 + mfontheight, temp_buf, strlen(temp_buf));

  if (p_table[element].boiling_pt > 0.0)
    sprintf(temp_buf, "%.2lf", p_table[element].boiling_pt);
  else
    strcpy(temp_buf, "--");
  XDrawString(p_disp, XtWindow(w), theGC, 10, 60 + mfontheight, temp_buf,
	      strlen(temp_buf));

  if (p_table[element].melting_pt > 0.0)
    sprintf(temp_buf, "%.2lf", p_table[element].melting_pt);
  else
    strcpy(temp_buf, "--");
  XDrawString(p_disp, XtWindow(w), theGC, 10, 100 + mfontheight, temp_buf,
	      strlen(temp_buf));


  if (p_table[element].density > 0.0)
    sprintf(temp_buf, "%.2lf", p_table[element].density);
  else
    strcpy(temp_buf, "--");
  XDrawString(p_disp, XtWindow(w), theGC, 10, 140 + mfontheight, temp_buf,
	      strlen(temp_buf));

  if (p_table[element].atomic_radius > 0.0)
    sprintf(temp_buf, "%.2lf", p_table[element].atomic_radius);
  else
    strcpy(temp_buf, "--");
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, temp_buf, strlen(temp_buf)),
	      60 + mfontheight, temp_buf, strlen(temp_buf));

  if (p_table[element].covalent_radius > 0.0)
    sprintf(temp_buf, "%.2lf", p_table[element].covalent_radius);
  else
    strcpy(temp_buf, "--");
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, temp_buf, strlen(temp_buf)),
	      100 + mfontheight, temp_buf, strlen(temp_buf));

  if (p_table[element].atomic_volume > 0.0)
    sprintf(temp_buf, "%.2lf", p_table[element].atomic_volume);
  else
    strcpy(temp_buf, "--");
  XDrawString(p_disp, XtWindow(w), theGC,
	      190 - XTextWidth(mfontstruct, temp_buf, strlen(temp_buf)),
	      140 + mfontheight, temp_buf, strlen(temp_buf));

  strcpy(temp_buf,p_table[element].valence);
  temp2 = strchr(temp_buf, ' ');
  *temp2 = 0;
  XDrawString(p_disp, XtWindow(w), theGC,
	      100 - XTextWidth(mfontstruct, temp_buf, strlen(temp_buf)) / 2,
	      160 + mfontheight, temp_buf, strlen(temp_buf));

  strcpy(temp_buf, p_table[element].name);
  temp2 = strchr(temp_buf, ' ');
  *temp2 = 0;
  XDrawString(p_disp, XtWindow(w), theGC,
	      100 - XTextWidth(mfontstruct, temp_buf, strlen(temp_buf)) / 2,
	      190 + mfontheight, temp_buf, strlen(temp_buf));

  free(temp_buf);
}
