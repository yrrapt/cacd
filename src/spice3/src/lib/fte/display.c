/**********
Copyright 1990 Regents of the University of California. All rights reserved.
**********/

#include "spice.h"
#include "ftegraph.h"
#include "ftedev.h"
#include "fteinput.h"
#include "cpdefs.h"     /* for VT_STRING */
#include "ftedefs.h"        /* for mylog() */

/* forward declarations */
static int nodev(), nop(), gen_DatatoScreen(), gen_Input();

#ifdef HAS_X11
extern int  X11_Init(), X11_NewViewport(), X11_Close(), X11_Clear(),
        X11_DrawLine(), X11_Arc(), X11_Text(), X11_DefineColor(),
        X11_DefineLinestyle(), X11_SetLinestyle(), X11_SetColor(),
        X11_Update(),
        X11_Input();
#endif

extern int  Plt5_Init(), Plt5_NewViewport(), Plt5_Close(), Plt5_Clear(),
        Plt5_DrawLine(), Plt5_Arc(), Plt5_Text(),
        Plt5_DefineLinestyle(), Plt5_SetLinestyle(), Plt5_SetColor(),
        Plt5_Update();

extern int  PS_Init(), PS_NewViewport(), PS_Close(), PS_Clear(),
        PS_DrawLine(), PS_Arc(), PS_Text(),
        PS_DefineLinestyle(), PS_SetLinestyle(), PS_SetColor(),
        PS_Update();

DISPDEVICE device[] = {

    {"error", 0, 0, 0, 0, 0, 0, nop, nop,
    nop, nop,
    nop, nop, nop, nop, nop,
    nop, nop, nop,
    nop, nop, nop, gen_Input,
    nop,},

#ifdef HAS_X11
    {"X11", 0, 0, 1024, 864, 0, 0, X11_Init, X11_NewViewport,
    X11_Close, X11_Clear,
    X11_DrawLine, X11_Arc, X11_Text, X11_DefineColor, X11_DefineLinestyle,
    X11_SetLinestyle, X11_SetColor, X11_Update,
    nodev, nodev, nodev, X11_Input,
    gen_DatatoScreen,},
#endif

    {"plot5", 0, 0, 1000, 1000, 0, 0, Plt5_Init, Plt5_NewViewport,
    Plt5_Close, Plt5_Clear,
    Plt5_DrawLine, Plt5_Arc, Plt5_Text, nodev, nodev,
    Plt5_SetLinestyle, Plt5_SetColor, Plt5_Update,
    nodev, nodev, nodev, nodev,
    gen_DatatoScreen,},

    {"postscript", 0, 0, 1000, 1000, 0, 0, PS_Init, PS_NewViewport,
    PS_Close, PS_Clear,
    PS_DrawLine, PS_Arc, PS_Text, nodev, nodev,
    PS_SetLinestyle, PS_SetColor, PS_Update,
    nodev, nodev, nodev, nodev,
    gen_DatatoScreen,},

    {"printf", 0, 0, 24, 80, 0, 0, nodev, nodev,
    nodev, nodev,
    nodev, nodev, nodev, nodev, nodev,
    nodev, nodev, nodev,
    nodev, nodev, nodev, gen_Input,
    nodev,},

};

DISPDEVICE *dispdev = device + NUMELEMS(device) - 1;

#define XtNumber(arr)       (sizeof(arr) / sizeof(arr[0]))

static DISPDEVICE *FindDev (char *name)
{
    int i;

    for (i = 0; i < XtNumber(device); i++) {
      if (eq(name, device[i].name)) return(&device[i]);
    }
    sprintf(ErrorMessage, "Can't find device %s.", name);
    internalerror(ErrorMessage);
    return(&device[0]);
}

void DevInit()
{
    char buf[128];

    dispdev = NULL;
#ifdef HAS_X11
    /* determine display type */
    if (getenv("DISPLAY") || cp_getvar("display", VT_STRING, buf)) {
      dispdev = FindDev("X11");
    }
#endif

    if (!dispdev) {
	externalerror("no graphics interface; please check compiling instructions");
	dispdev = FindDev("error");
    } else if ((*(dispdev->Init))()) {
      fprintf(cp_err, "Warning: can't initialize display device for graphics.\n");
      dispdev = FindDev("error");
    }
}

/* NewViewport is responsible for filling in graph->viewport */
int NewViewport (GRAPH *pgraph)
{
    return (*(dispdev->NewViewport))(pgraph);
}

void DevClose()
{
    (*(dispdev->Close))();
}

void DevClear()
{
    (*(dispdev->Clear))();
}

void DrawLine(int x1, int y1, int x2, int y2)
{
    (*(dispdev->DrawLine))(x1, y1, x2, y2);
}

void Arc(int x0, int y0, int radius, double theta1, double theta2)
{
    (*(dispdev->Arc))(x0, y0, radius, theta1, theta2);
}

void Text(char *text, int x, int y)
{
    (*(dispdev->Text))(text, x, y);
}

void DefineColor(int colorid, double red, double green, double blue)
{
    (*(dispdev->DefineColor))(colorid, red, green, blue);
}

void DefineLinestyle(int linestyleid, int mask)
{
    (*(dispdev->DefineLinestyle))(linestyleid, mask);
}

void SetLinestyle(int linestyleid)
{
    (*(dispdev->SetLinestyle))(linestyleid);
}

void SetColor(int colorid)
{
    (*(dispdev->SetColor))(colorid);
}

void Update()
{
    if (dispdev) (*(dispdev->Update))();
}

/* note: screen coordinates are relative to window
    so need to add viewport offsets
*/
static int gen_DatatoScreen (GRAPH *graph, double x, double y, int *screenx, int *screeny)
{
    double low, high;

    /* note: may want to cache datawindowsize/viewportsize */ /* done */

    /* note: think this out---Is 1 part of the viewport? Do we handle
        this correctly? */

    /* have to handle several types of grids */

    /* note: we can't compensate for X's demented y-coordinate system here
        since the grid routines use DrawLine w/o calling this routine */
    if ((graph->grid.gridtype == GRID_LOGLOG) || (graph->grid.gridtype == GRID_YLOG)) {
	low  = mylog10(graph->datawindow.ymin);
	high = mylog10(graph->datawindow.ymax);
	y = (mylog10(y) - low) / (high - low) * graph->viewport.height;
    } else {
	y = (y - graph->datawindow.ymin) / graph->aspectratioy;
    }

    if ((graph->grid.gridtype == GRID_LOGLOG) || (graph->grid.gridtype == GRID_XLOG)) {
	low  = mylog10(graph->datawindow.xmin);
	high = mylog10(graph->datawindow.xmax);
	x = (mylog10(x) - low) / (high - low) * graph->viewport.width;
    } else {
	x = (x - graph->datawindow.xmin) / graph->aspectratiox;
    }
   *screenx = x + 0.5 + graph->viewportxoff;
   *screeny = y + 0.5 + graph->viewportyoff;
    return(0);
}

void DatatoScreen (GRAPH *graph, double x, double y, int *screenx, int *screeny)
{
    (*(dispdev->DatatoScreen))(graph, x, y, screenx, screeny);
}

void Input (REQUEST *request, RESPONSE *response)
{
    (*(dispdev->Input))(request, response);
}

static int gen_Input (REQUEST *request, RESPONSE *response)
{
    switch (request->option) {
      case char_option:
        response->reply.ch = inchar(request->fp);
        response->option = request->option;
        break;
      default:
        /* just ignore, since we don't want a million error messages */
	if (response)
	    response->option = error_option;
        break;
    }
    return 0;
}

static int nop() /* no operation, do nothing */
{
    return(1);  /* so NewViewport will fail */
}

static int nodev()
{
    sprintf(ErrorMessage, "This operation is not defined for display type %s.", dispdev->name);
    internalerror(ErrorMessage);
    return(1);
}

void SaveText (GRAPH *graph, char *text, int x, int y)
{
    struct _keyed *keyed;

    keyed = (struct _keyed *) calloc(1, sizeof(struct _keyed));

    if (!graph->keyed) {
      graph->keyed = keyed;
    } else {
      keyed->next = graph->keyed;
      graph->keyed = keyed;
    }

    keyed->text = copy(text);
    keyed->x = x;
    keyed->y = y;
    keyed->colorindex = graph->currentcolor;
}

/* if given name of a hardcopy device, finds it and switches devices
   if given NULL, switches back
*/
int DevSwitch (char *devname)
{
    static DISPDEVICE *lastdev = NULL;

    if (devname) {
      if (lastdev) {
        internalerror("DevSwitch w/o changing back");
        return (1);
      }
      lastdev = dispdev;
      dispdev = FindDev(devname);
      if (eq(dispdev->name, "error")) {
        internalerror("no hardcopy device");
        dispdev = lastdev;  /* undo */
        lastdev = NULL;
        return (1);
      }
      (*(dispdev->Init))();
    } else {
      (*(dispdev->Close))();
      dispdev = lastdev;
      lastdev = NULL;
    }
    return(0);
}
