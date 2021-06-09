// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dgplot.cc
//
// 08 Dec 94 [mw]
// $Id: dgplot.cc,v 1.3 1996/08/28 18:58:19 mwolfe Exp mwolfe $


// make a directed graph, then try to plot it interactively
// pretty picture using Motif widgets

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/BulletinB.h>
#include <Xm/PanedW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrollBar.h>
#include <Xm/Scale.h>
#include <Xm/List.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>

#include <math.h>
#include <string.h>


#ifdef Bool
#undef Bool
#endif

#include "portable.h"
#include "dg.h"
#include "dgplot.h"
#include "standard.h"

#define DEBUG 0
#define MYPI 3.1415926535897932384626433

static Display* Global_Display;
static XtAppContext Global_App;
static Font GlobalFont;

class Draw_Window
{
public:
    Sint32 pixnum, pixden;	// (num/den) canvas pixels per unit
    Sint32 draw_x;		// X,Y position of UL corner of view canvas
    Sint32 draw_y;		// on drawing, in units
    Sint32 draw_width;		// width of drawing, in units
    Sint32 draw_height;		// height of drawing in units
    Sint32 draw_show_width;	// how much of the drawing is showing
    Sint32 draw_show_height;	// on the canvas
    Sint32 old_draw_width;
    Sint32 old_draw_height;
    Sint32 old_draw_show_width;
    Sint32 old_draw_show_height;
    Sint32 old_draw_x;
    Sint32 old_draw_y;
    Sint32 circlesize;		// size of circle, in units
    Sint32 circlerad;		// half of circlesize
    double alength;		// length of arrowhead
    double aangle;		// angle of arrowhead
    Dimension canvas_width;	// pixel width of view canvas
    Dimension canvas_height;	// pixel height of view canvas
    Widget pane; 	// paned window of X objects
    unsigned short paneindent;
    Widget buttonholder;// button pane, in the paned window
    Widget menubar;
    Widget frame; 	// frame (around canvas) in paned window
    Widget board; 	// bulletin board, in frame
    Widget canvas;	// canvas in bulletin board
    Widget xscroll;	// X scroll bar, in bulletin board
    Widget yscroll;	// Y scroll bar, in bulletin board
    Widget scale;	// scale bar, in bulletin board
    GC gc;
    Window window;
    P_DG_Graph graph;
    Draw_Window( P_DG_Graph, char*, char*, char* );
    P_char fontname;
    XFontStruct* fontstruct;
    P_char leftfontname;
    XFontStruct* leftfontstruct;
    P_char rightfontname;
    XFontStruct* rightfontstruct;
    Bool notdragging;
    Bool ShowText;
    Widget toplevel;	// top level X object
    Sint32 last_width;
    Sint Radius;
};

inline Sint min( Sint a, Sint b )
{
    return ( a < b ? a : b );
}
inline Sint max( Sint a, Sint b )
{
    return ( a > b ? a : b );
}

static Sint FontNames = 0;
#define maxfontnames 100
static P_char FontName[maxfontnames];
static XFontStruct* FontStruct[maxfontnames];

static XFontStruct* addfont( P_char fontname )
{
    Sint i;
    for( i = 0; i < FontNames; ++i ){
	if( strcmp( FontName[i], fontname ) == 0 ) return FontStruct[i];
    }
    FontStruct[FontNames] = XLoadQueryFont( Global_Display, fontname );
    FontName[FontNames] = (char*)malloc( strlen( fontname ) + 1 );
    strcpy( FontName[FontNames], fontname );
    ++FontNames;
    return FontStruct[i];
}

static void killfonts()
{
    Sint i;
    for( i = 0; i < FontNames; ++i ){
	XFreeFont( Global_Display, FontStruct[i] );
    }
}


static void Init_Draw_Window( Draw_Window* W, P_DG_Graph g, P_char initialfont,
	P_char leftfont, P_char rightfont ){
    W->pixnum = 1000;
    W->pixden = 1000;
    W->draw_x = 0;
    W->draw_y = 0;
    W->circlesize = 30;//20;
    W->circlerad = (W->circlesize+1)/2;
    W->alength = W->circlesize/2.0;
	// compute the angle in radians as pi/(180/angle)
	// where angle is in degrees between 0 and 180.
	// pi/(180/15) gives a 15-degree arrow.
    W->aangle = MYPI/(180./15.);
    if( g->Interference ){
	W->Radius = (Sint)(W->circlesize / sin(MYPI/g->DG_Count_Nodes));
	W->draw_width = 2*(W->circlesize+W->Radius);
	W->draw_height = 2*(W->circlesize+W->Radius);
    }else{
	W->draw_width = (g->DG_Width_Max + 1) * W->circlesize;
	W->draw_height = (g->DG_Level_Max + 1) * 2 * W->circlesize;
    }
    W->canvas_width = min( 400, max( 100, W->draw_width * W->pixnum/W->pixden ) );
    W->canvas_height = min( 300, max( 200, W->draw_height * W->pixnum/W->pixden ) );
    W->draw_show_width = W->canvas_width * W->pixden/W->pixnum;
    W->draw_show_height = W->canvas_height * W->pixden/W->pixnum;
    W->graph = g;
    W->fontname = initialfont;
    W->fontstruct = NULL;
    W->leftfontname = leftfont;
    W->leftfontstruct = NULL;
    W->rightfontname = rightfont;
    W->rightfontstruct = NULL;
    W->notdragging = 1;
    W->ShowText = 1;
    g->Graphics = (Uint32)(W);
    W->old_draw_width = 0;
    W->old_draw_height = 0;
    W->old_draw_show_width = 0;
    W->old_draw_show_height = 0;
    W->old_draw_x = W->draw_x;
    W->old_draw_y = W->draw_y;
    W->last_width = 0;
}// Init_Draw_Window

inline Draw_Window::Draw_Window( P_DG_Graph g, char* fontname,
	char* leftfont, char* rightfont ){
    memset( this, 0, sizeof( Draw_Window ) );
    Init_Draw_Window( this, g, fontname, leftfont, rightfont );
}// constructor for Draw_Window

void handle_resize ( Widget w, caddr_t client_data, XEvent *event, char* dunno );
void handle_input ( Widget w, XtPointer, XtPointer);
void handle_expose ( Widget w, XtPointer, XtPointer);
void handle_xscroll ( Widget w, XtPointer, XtPointer);
void handle_xscroll_drag ( Widget w, XtPointer, XtPointer);
void handle_yscroll ( Widget w, XtPointer, XtPointer);
void handle_yscroll_drag ( Widget w, XtPointer, XtPointer);
void handle_scale ( Widget w, XtPointer, XtPointer);
void handle_scale_drag ( Widget w, XtPointer, XtPointer);
void Button_Press( Widget w, XtPointer, XtPointer );
void List_Item_Press( Widget w, XtPointer, XtPointer );
void Close_Handler( Widget w, XtPointer, XtPointer );

// return 'x', 'y' position of center of node dg in drawing units
inline Sint Xcenter( Draw_Window* W, Sint position )
{
    return (position-1)*W->circlesize;
}
inline Sint Ycenter( Draw_Window* W, Sint level )
{
    return (level*2-1)*W->circlesize;
}
inline Sint upper( Draw_Window* W, Sint ycenter )
{
    return ycenter-W->circlerad;
}
inline Sint lower( Draw_Window* W, Sint ycenter )
{
    return ycenter+W->circlerad;
}
inline Sint left( Draw_Window* W, Sint xcenter )
{
    return xcenter-W->circlerad;
}
inline Sint right( Draw_Window* W, Sint xcenter ) 
{
    return xcenter+W->circlerad;
}

inline Sint DGXInt( Draw_Window* W, P_DDG dg, Bool edge, Bool left, Bool right )
{
    Sint C = W->circlesize;
    Sint XC = C+W->Radius;
    float theta = MYPI*2/W->graph->DG_Count_Nodes;
    float thetan = dg->DDG_Position*theta;
    if( left ) thetan = thetan - theta/4;
    if( right ) thetan = thetan + theta/4;
    float sin_thetan = sin(thetan);
    Sint R = W->Radius;
    if( edge ) R = R-C/2;
    return (Sint)(XC+sin_thetan*R);
}

inline Sint DGYInt( Draw_Window* W, P_DDG dg, Bool edge, Bool left, Bool right )
{
    Sint C = W->circlesize;
    Sint YC = C+W->Radius;
    float theta = MYPI*2/W->graph->DG_Count_Nodes;
    float thetan = dg->DDG_Position*theta;
    if( left ) thetan = thetan - theta/4;
    if( right ) thetan = thetan + theta/4;
    float cos_thetan = cos(thetan);
    Sint R = W->Radius;
    if( edge ) R = R-C/2;
    return (Sint)(YC+cos_thetan*R);
}

inline Sint DGX( Draw_Window* W, P_DDG dg ) 
{
    if( W->graph->Interference ){
	return DGXInt( W, dg, 0, 0, 0 );
    }
    return Xcenter(W, dg->DDG_Position);
}// DGX

inline Sint DGY( Draw_Window* W, P_DDG dg ) 
{
    if( W->graph->Interference ){
	return DGYInt( W, dg, 0, 0, 0 );
    }
    return Ycenter(W, dg->DDG_Level);
}// DGY

inline Sint First_Level( Draw_Window* W ) 
{
    // solve for smallest 'level' such that
    // Ycenter(W,level) - draw_y + circlesize >= 0
    // Ycenter is (level*2-1)*circlesize
    // solve for (level*2-1)*circlecsize - draw_y + circlesize >= 0
    // level*2-1 >= ( draw_y - circlesize )/circlesize
    // level >= draw_y/(2*circlesize)
    Sint level = (W->draw_y)/(2*W->circlesize);
    if( level <= 0 ) level = 1;
    if( level >= W->graph->DG_Level_Max ) level = W->graph->DG_Level_Max;
    return level;
}
inline Sint Last_Level( Draw_Window* W ) 
{
    // solve for largest 'level' such that
    // Ycenter(W,level) - draw_y - circlesize <= height
    // Ycenter is (level*2-1)*circlesize
    // solve for (level*2-1)*circlecsize - draw_y - circlesize <= height
    // level*2-1 <= (height+draw_y+circlesize)/circlesize
    // level <= (height+draw_y+2*circlesize)/(2*circlesize)
    Sint y = W->draw_show_height + W->circlesize;
    Sint ycenter = y + W->draw_y;
    Sint level = (W->draw_show_height + W->draw_y + 2*W->circlesize)/
		(2*W->circlesize);
    if( level <= 0 ) level = 1;
    if( level >= W->graph->DG_Level_Max ) level = W->graph->DG_Level_Max;
    return level;
}

inline Bool Level_Appears( Draw_Window* W, Sint level ) 
{
Sint y = Ycenter( W, level ) - W->draw_y;
    return( y + W->circlesize >= 0 &&
	 y - W->circlesize <= W->draw_show_height );
}

inline Bool Node_Appears( Draw_Window* W, P_DDG dg ) 
{
    if( dg->DDG_Show ){
	Sint x = Xcenter( W, dg->DDG_Position ) - W->draw_x;
	if( x + W->circlesize >= 0 &&
	     x - W->circlesize <= W->draw_show_width ) return 1;
	if( dg->DDG_Type == ddg_top_corner ||
	    dg->DDG_Type == ddg_both_corners ){
	    P_DDG side = dg->Sideways_To;
	    Sint xs = Xcenter( W, side->DDG_Position ) - W->draw_x;
	    if( xs + W->circlesize >= 0 &&
		 xs - W->circlesize <= W->draw_show_width ) return 1;
	    if( x + W->circlesize < 0 && xs - W->circlesize > W->draw_show_width )
		return 1;
	    if( xs + W->circlesize < 0 && x - W->circlesize > W->draw_show_width )
		return 1;
	}
	if( dg->DDG_Type == ddg_bottom_corner ||
	    dg->DDG_Type == ddg_both_corners ){
	    P_DDG side = dg->Sideways_From;
	    Sint xs = Xcenter( W, side->DDG_Position ) - W->draw_x;
	    if( xs + W->circlesize >= 0 &&
		 xs - W->circlesize <= W->draw_show_width ) return 1;
	    if( x + W->circlesize < 0 && xs - W->circlesize > W->draw_show_width )
		return 1;
	    if( xs + W->circlesize < 0 && x - W->circlesize > W->draw_show_width )
		return 1;
	}
    }
    return 0;
}

inline Sint xcanvas( Draw_Window* W, Sint xunit ) 
{
    return (xunit-W->draw_x)*W->pixnum/W->pixden;
}

inline Sint ycanvas( Draw_Window* W, Sint yunit ) 
{
    return (yunit-W->draw_y)*W->pixnum/W->pixden;
}

inline Sint xcanvas( Draw_Window* W, double xunit ) 
{
    return (Sint)((xunit-W->draw_x)*W->pixnum/W->pixden);
}

inline Sint ycanvas( Draw_Window* W, double yunit ) 
{
    return (Sint)((yunit-W->draw_y)*W->pixnum/W->pixden);
}

static inline setfont( Draw_Window* W, XFontStruct* fs ) 
{
    if( fs->fid != GlobalFont ){
	XSetFont( Global_Display, W->gc, fs->fid );
	GlobalFont = fs->fid;
    }
}

static void DGDrawText( Draw_Window* W, Sint x, Sint y, char* Name, char* Name2, int doname2 ) 
{
XCharStruct info, info2;
int ascent, descent, direction;
int ascent2, descent2, direction2;
Sint csize = W->circlesize * W->pixnum/W->pixden;
Sint maxheight = 3*csize/4;
Sint maxwidth = 4*csize/5;
Sint width, width2;
Sint length, length2;

    if( Name == NULL ){
	length = 0;
	ascent = 0;
	descent = 0;
	width = 0;
    }else{
	length = strlen( Name );

	// how big is the string, will it fit?
	XTextExtents( W->fontstruct, Name, length,
	    &direction, &ascent, &descent, &info );
	ascent = info.ascent;
	descent = info.descent;
	if( ascent+descent > maxheight ){
	    return; // too small to plot at all.
	}
	// Only use middle part of circle/box
	width = info.width;
	if( width > maxwidth ){
	    // figure out how many characters to plot at all
	    // fixed size fonts - use width/length/
	    length = ( maxwidth * length )/width;
	    width = XTextWidth( W->fontstruct, Name, length );
	}
    }
    Bool secondline = 0;
    if( doname2 ){
	secondline = 1;
	length2 = strlen( Name2 );
	XTextExtents( W->fontstruct, Name2, length2,
	    &direction2, &ascent2, &descent2, &info2 );
	ascent2 = info2.ascent;
	descent2 = info2.descent;
	width2 = info2.width;
	if( width2 > maxwidth ){
	    length2 = ( maxwidth * length2 )/width2;
	    width2 = XTextWidth( W->fontstruct, Name2, length2 );
	}
	if( ascent2+descent2+ascent+descent > maxheight ){
	    // don't bother to plot 2nd line
	    secondline = 0;
	}
    }
    int clear = 1;
    if( !secondline ){
	int xx = xcanvas(W,x);
	int yy = ycanvas(W,y);
	int hh = ascent+descent;
	int hh2 = hh/2;
	if( Name ){
	    if( clear ){
		XClearArea( Global_Display, W->window,
		    xx-width/2, yy-ascent+descent, width, hh, 0 );
	    }
	    setfont( W, W->fontstruct );
	    XDrawString( Global_Display, W->window, W->gc,
		xx-width/2, yy+descent, 
		Name, length );
	}
    }else{
	int xx = xcanvas(W,x);
	int yy = ycanvas(W,y);
	setfont( W, W->fontstruct );
	if( Name ){
	    int hh = ascent+descent;
	    if( clear ){
		XClearArea( Global_Display, W->window,
		    xx-width/2, yy-hh, width, hh, 0 );
	    }
	    XDrawString( Global_Display, W->window, W->gc,
		xx-width/2, yy-descent, Name, length );
	}
	int hh2 = ascent2+descent2;
	if( clear ){
	    XClearArea( Global_Display, W->window,
		xx-width2/2, yy+hh2, width2, hh2, 0 );
	}
	XDrawString( Global_Display, W->window, W->gc,
	    xx-width2/2, yy+ascent2, Name2, length2 );
    }
}// DGDrawText

// Set the line attributes for this style
static LastStyle = dg_bold;
static void DGGCAttributes( Draw_Window* W, DG_Styles style ) 
{
    if( style == LastStyle ) return;
    LastStyle = style;
    int LineStyle = LineSolid;
    int CapStyle = CapButt;
    int JoinStyle = JoinRound;
    unsigned int LineWidth = 0;
    int DottedCount = 2;
    char DottedLine[] = {2,3};
    int DashOffset = 0;
    int DashedCount = 2;
    char DashedLine[] = {4,4};
    int DashCount;
    char* DashLine;
    switch( style ){
    case dg_solid: 
	LineStyle = LineSolid;
	LineWidth = 0;
	XSetLineAttributes( Global_Display, W->gc, LineWidth, LineStyle, CapStyle, JoinStyle );
	break;
    case dg_bold:
	LineStyle = LineSolid;
	LineWidth = 3;
	XSetLineAttributes( Global_Display, W->gc, LineWidth, LineStyle, CapStyle, JoinStyle );
	break;
    case dg_dashed:
	LineStyle = LineOnOffDash;
	LineWidth = 1;
	XSetDashes( Global_Display, W->gc, DashOffset, DashedLine, DashedCount );
	XSetLineAttributes( Global_Display, W->gc, LineWidth, LineStyle, CapStyle, JoinStyle );
	break;
    case dg_dotted:
	LineStyle = LineOnOffDash;
	LineWidth = 2;
	XSetDashes( Global_Display, W->gc, DashOffset, DottedLine, DottedCount );
	XSetLineAttributes( Global_Display, W->gc, LineWidth, LineStyle, CapStyle, JoinStyle );
	break;
    case dg_blank:
	// nothing to do
	break;
    }
}// DGGCAttributes


// draw arrowhead.
// We have a line from (x1,y1) to (x2,y2)
// The angle of this line is theta = atan((y1-y2)/(x1-x2))
// We have to negate the Y coordinate, since X-window coordinates
// have increasing Y going down
// The angle of the boundaries of the arrowhead are
// a1 = -theta+aangle and a2 = -theta-aangle.
// The length of the boundaries of the arrowhead are l = alength,
// so the (x,y) coordinates of the corners of the arrowhead 
// are (x2,y2), (x2+cos(a1)*l,y2+sin(a1)*l), (x2+cos(a2)*l,y2+sin(a2)*l)
void DGDrawArrow( Draw_Window* W, Sint x1, Sint y1, Sint x2, Sint y2 ) 
{
double theta = atan2(y1-y2,x2-x1);
double a1 = -theta+W->aangle;
double a2 = -theta-W->aangle;
XPoint corner[3];
double xl = x2-cos(a1)*W->alength;
double yl = y2-sin(a1)*W->alength;
double xr = x2-cos(a2)*W->alength;
double yr = y2-sin(a2)*W->alength;
    corner[0].x = xcanvas( W, x2 );
    corner[0].y = ycanvas( W, y2 );
    corner[1].x = xcanvas( W, xl );
    corner[1].y = ycanvas( W, yl );
    corner[2].x = xcanvas( W, xr );
    corner[2].y = ycanvas( W, yr );
    XFillPolygon (Global_Display, W->window, W->gc, 
	    corner, 3, Convex, CoordModeOrigin );
}// DGDrawArrow



static void DGLineAttributes( Draw_Window* W, DG_Styles style, Uint32 color ) 
{
    if( !(W->notdragging) ){
	DGGCAttributes( W, dg_solid );
    }else{
	DGGCAttributes( W, style );
    }
}// DGLineAttributes

static Sint tan225n = 6786/2;
static Sint tan225d = 16384;
static Sint tan225( Sint x ) 
{
    return (x*tan225n)/tan225d;
}


// get (x,y) of start point of line from node 'dg' in window 'W'
void DGLineStart( Draw_Window* W, P_DDG dg, Sint side, Sint& x, Sint& y ) 
{
// square root of two is 1.414, sqrt(2)/2 = 1414/2000
const Sint sqrt2 = 1414, sqrtd = 2000;

    x = DGX( W, dg );
    y = DGY( W, dg );
    if( dg->DDG_Type != ddg_clone ){
	y = y + W->circlerad;
    }else if( side == 0 ){
	// no side, just middle of bottom, same for all shapes
	y = y + W->circlerad;
    }else{
	// left or right, depending on value of "side", +1 or -1
	Sint diam;
	Sint td;
	Sint offset;
	switch( dg->DDG_Clone->DG_Shape_Bot ){
	case dg_circle :
	    x = x+side*(sqrt2*W->circlesize/(2*sqrtd));
	    y = y+sqrt2*W->circlesize/(2*sqrtd);
	    break;

	case dg_box :
	    x = x+side*W->circlerad;
	    y = y+W->circlerad;
	    break;

	case dg_hex :
	    x = x+side*W->circlesize/3;
	    y = y+W->circlerad;
	    break;

	case dg_diamond :
	    x = x+side*W->circlerad/2;
	    y = y+W->circlerad/2;
	    break;

	case dg_oct :
	    diam = W->circlesize;
	    td = tan225(diam);
	    offset = (td+W->circlerad)/2;
	    x = x+side*offset;
	    y = y+offset;
	    break;

	default:
	  break;
	}
    }
}// DGLineStart

// get (x,y) of end point of line from node 'dg' in window 'W'
void DGLineEnd( Draw_Window* W, P_DDG dg, Sint side, Sint& x, Sint& y ) 
{
// square root of two is 1.414, sqrt(2)/2 = 1414/2000
const Sint sqrt2 = 1414, sqrtd = 2000;

    x = DGX( W, dg );
    y = DGY( W, dg );
    if( dg->DDG_Type != ddg_clone ){
	y = y - W->circlerad;
    }else if( side == 0 ){
	// no side, just middle of top, same for all shapes
	y = y - W->circlerad;
    }else{
	// left or right, depending on value of "side", +1 or -1
	Sint diam;
	Sint td;
	Sint offset;
	switch( dg->DDG_Clone->DG_Shape_Top ){
	case dg_circle :
	    x = x+side*(sqrt2*W->circlesize/(2*sqrtd));
	    y = y-sqrt2*W->circlesize/(2*sqrtd);
	    break;

	case dg_box :
	    x = x+side*W->circlerad;
	    y = y-W->circlerad;
	    break;

	case dg_hex :
	    x = x+side*W->circlesize/3;
	    y = y-W->circlerad;
	    break;

	case dg_diamond :
	    x = x+side*W->circlerad/2;
	    y = y-W->circlerad/2;
	    break;

	case dg_oct :
	    diam = W->circlesize;
	    td = tan225(diam);
	    offset = (td+W->circlerad)/2;
	    x = x+side*offset;
	    y = y-offset;
	    break;

	default:
	  break;
	}
    }
}// DGLineEnd

static XPoint DGpoints[50];
static int DGnpoints;

inline startline(){ DGnpoints = 0; }
static addline( Draw_Window* W, Sint x, Sint y ) 
{
    DGpoints[DGnpoints].x = xcanvas( W, x );
    DGpoints[DGnpoints].y = ycanvas( W, y );
    ++DGnpoints;
}// addline
static showline( Draw_Window* W ){ 
    XDrawLines( Global_Display, W->window, W->gc, 
	DGpoints, DGnpoints, CoordModeOrigin );
    startline();
}// showline



//(x1,y1)*---------*(x2,y2)
//      /           \
//     *(x0,y0)      *(x3,y3)
static void DrawTopChannel( Draw_Window* W, P_DDG from, P_DDG to, Sint channel )
{
Sint x0, y0, x1, y1, x2, y2, x3, y3;
Sint beta = 5;
Sint alpha = 3;
    // Get X,Y coordinates of top of top_corner
    DGLineEnd( W, from, 0, x0, y0 );
    // Y coordinate of the channel
    y2 = y1 = y0 - beta - alpha*channel;
    // Is top corner to left or right of target node?
    Sint side;
    if( from->DDG_Position < to->DDG_Position ){
	side = -1;
    }else{
	side = 1;
    }
    // Adjust 'x1' to be 'to the side' of (x0,y0)
    x1 = x0 - side*(y0-y1);
    // Get X,Y coordinates of the 'side' of the target node
    DGLineEnd( W, to, side, x3, y3 );
    // Where to place the turn?
    x2 = x3 + side*(y3-y2);
    // Draw the arrow, if needed
    // Need a way to determine if the arrow goes here or at the bottom
    Sint arrowend = from->DDG_Ptr_Clone->DGL_Arrow == dgl_end || 
		 from->DDG_Ptr_Clone->DGL_Arrow == dgl_both;
    if( arrowend && W->notdragging ){
	DGDrawArrow( W, x2, y2, x3, y3 );
    }
    addline( W, x3, y3 );
    addline( W, x2, y2 );
    addline( W, x1, y2 );
    addline( W, x0, y0 );
}// DrawTopChannel

static void DrawBottomChannel( Draw_Window* W, P_DDG from, P_DDG to, Sint channel ) 
{
Sint x0, y0, x1, y1, x2, y2, x3, y3;
Sint beta = 5;
Sint alpha = 3;
    // Get X,Y coordinates of bottom of the bottom_corner
    DGLineStart( W, to, 0, x0, y0 );
    // Y coordinate of the channel
    y2 = y1 = y0 + beta + alpha*channel;
    // Is bottom_corner to left or right of source node?
    Sint side;
    if( to->DDG_Position < from->DDG_Position ){
	side = -1;
    }else{
	side = 1;
    }
    // Adjust 'x1' to be 'to the side' of (x0,y0)
    x1 = x0 + side*(y0-y1);
    // Get X,Y coordinates of the 'side' of the source node
    DGLineStart( W, from, side, x3, y3 );
    // Where to place the turn?
    x2 = x3 - side*(y3-y2);
    // Draw 1st part of line, from top corner to channel
    addline( W, x0, y0 );
    addline( W, x1, y2 );
    addline( W, x2, y2 );
    addline( W, x3, y3 );
    showline( W );
    // Draw the arrow, if needed
    // Need a way to determine if the arrow goes here or at the bottom
    Sint arrowstart = to->DDG_Ptr_Clone->DGL_Arrow == dgl_start || 
		 to->DDG_Ptr_Clone->DGL_Arrow == dgl_both;
    if( arrowstart && W->notdragging ){
	DGDrawArrow( W, x2, y2, x3, y3 );
    }
}// DrawBottomChannel

// draw node for 'dg'
void DGdraw( Draw_Window* W, P_DDG dg, Sint firstlevel ) 
{
Sint x, y, csize;
static XPoint corner[10];
Sint diam = W->circlesize;
P_DG dgc;
Sint td;
Sint x1, x2, y1, y2;
P_DDG dt;

    x = DGX( W, dg );
    y = DGY( W, dg );
    csize = diam * W->pixnum/W->pixden;
    switch( dg->DDG_Type ){
    case ddg_clone:
	dgc = dg->DDG_Clone;
	if( dgc->DG_Style_Top == dgc->DG_Style_Bot &&
		dgc->DG_Shape_Top == dgc->DG_Shape_Bot ){
	    DGLineAttributes( W, dgc->DG_Style_Top, dgc->DG_Color );
	    switch( dgc->DG_Shape_Top ){
	    case dg_circle :
		XDrawArc (Global_Display, W->window, W->gc, 
			xcanvas( W, left( W, x ) ), 
			ycanvas( W, upper( W, y ) ), 
			csize, csize, 0, 64*360);
		break;

	    case dg_box :
		startline();
		addline( W, left( W, x ), lower( W, y ) );
		addline( W, left( W, x ), upper( W, y ) );
		addline( W, right( W, x ), upper( W, y ) );
		addline( W, right( W, x ), lower( W, y ) );
		addline( W, left( W, x ), lower( W, y ) );
		showline( W );
		break;

	    case dg_hex :
		startline();
		addline( W, left( W, x ), y );
		addline( W, x-diam/3, upper( W, y ) );
		addline( W, x+diam/3, upper( W, y ) );
		addline( W, right( W, x ), y );
		addline( W, x+diam/3, lower( W, y ) );
		addline( W, x-diam/3, lower( W, y ) );
		addline( W, left( W, x ), y );
		showline( W );
		break;

	    case dg_diamond :
		startline();
		addline( W, left( W, x ), y );
		addline( W, x, upper( W, y ) );
		addline( W, right( W, x ), y );
		addline( W, x, lower( W, y ) );
		addline( W, left( W, x ), y );
		showline( W );
		break;

	    case dg_oct :
		td = tan225(diam);
		startline();
		addline( W, left( W, x ), y + td );
		addline( W, left( W, x ), y - td );
		addline( W, x - td, upper( W, y ) );
		addline( W, x + td, upper( W, y ) );
		addline( W, right( W, x ), y - td );
		addline( W, right( W, x ), y + td );
		addline( W, x + td, lower( W, y ) );
		addline( W, x - td, lower( W, y ) );
		addline( W, left( W, x ), y + td );
		showline( W );
		break;

	    default:
	      break;
	    }
	}else{
	    DGLineAttributes( W, dgc->DG_Style_Top, dgc->DG_Color );
	    switch( dgc->DG_Shape_Top ){
	    case dg_circle :
		XDrawArc (Global_Display, W->window, W->gc, 
			xcanvas( W, left( W, x ) ), 
			ycanvas( W, upper( W, y ) ), 
			csize, csize, 0, 64*180);
		break;

	    case dg_box :
		startline();
		addline( W, left( W, x ), y );
		addline( W, left( W, x ), upper( W, y ) );
		addline( W, right( W, x ), upper( W, y ) );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    case dg_hex :
		startline();
		addline( W, left( W, x ), y );
		addline( W, x-diam/3, upper( W, y ) );
		addline( W, x+diam/3, upper( W, y ) );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    case dg_diamond :
		startline();
		addline( W, left( W, x ), y );
		addline( W, x, upper( W, y ) );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    case dg_oct :
		td = tan225(diam);
		startline();
		addline( W, left( W, x ), y );
		addline( W, left( W, x ), y - td );
		addline( W, x - td, upper( W, y ) );
		addline( W, x + td, upper( W, y ) );
		addline( W, right( W, x ), y - td );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    default:
	      break;
	    }
	    DGLineAttributes( W, dg->DDG_Clone->DG_Style_Bot, dg->DDG_Clone->DG_Color );
	    switch( dg->DDG_Clone->DG_Shape_Bot ){
	    case dg_circle :
		XDrawArc (Global_Display, W->window, W->gc, 
			xcanvas( W, left( W, x ) ), 
			ycanvas( W, upper( W, y ) ), 
			csize, csize, 0, - 64*180);
		break;

	    case dg_box :
		startline();
		addline( W, left( W, x ), y );
		addline( W, left( W, x ), lower( W, y ) );
		addline( W, right( W, x ), lower( W, y ) );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    case dg_hex :
		startline();
		addline( W, left( W, x ), y );
		addline( W, x-diam/3, lower( W, y ) );
		addline( W, x+diam/3, lower( W, y ) );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    case dg_diamond :
		startline();
		addline( W, left( W, x ), y );
		addline( W, x, lower( W, y ) );
		addline( W, right( W, x ), y );
		showline( W );
		break;

	    case dg_oct :
		td = tan225(diam);
		startline();
		addline( W, left( W, x ), y );
		addline( W, left( W, x ), y + td );
		addline( W, x - td, lower( W, y ) );
		addline( W, x + td, lower( W, y ) );
		addline( W, right( W, x ), y + td );
		addline( W, right( W, x ), y );
		showline( W );
		break;
	    default:
	      break;
	    }
	}
	if( W->notdragging && W->ShowText ){
	    P_DG clone = dg->DDG_Clone;
	    DGDrawText( W, x, y, clone->DG_Name, clone->DG_Name2, clone->DG_Second_Line );
	}
	if( W->notdragging && dg->DDG_Clone->DG_Bar_Style != dg_blank ){
	    DGLineAttributes( W, dg->DDG_Clone->DG_Bar_Style, 
		dg->DDG_Clone->DG_Bar_Color );
	    XDrawLine( Global_Display, W->window, W->gc,
		xcanvas( W, left( W, x ) ), ycanvas( W, y ), 
		xcanvas( W, right( W, x ) ), ycanvas( W, y ) );
	}
	break;

    case ddg_rway :
    case ddg_bottom_corner :
    case ddg_both_corners :
    case ddg_top_corner :
	if( dg->DDG_Level == firstlevel || dg->DDG_Type == ddg_top_corner ||
		dg->DDG_Type == ddg_both_corners ){
	    P_Ptr_DG clone = dg->DDG_Ptr_Clone;
	    DGLineAttributes( W, clone->DGL_Style, 
		    clone->DGL_Color );
	    startline();
	    if( dg->DDG_Type == ddg_top_corner || dg->DDG_Type == ddg_both_corners ){
		DrawTopChannel( W, dg, dg->Sideways_To, dg->DDG_Top_Channel );
	    }else{
	        DGLineEnd( W, dg, 0, x1, y1 );
		addline( W, x1, y1 );
	    }
	    dt = dg;
	    if( dg->DDG_Type != ddg_both_corners ){
		for( ; dt && dt->DDG_Type != ddg_bottom_corner;
			dt = link( first( dt->DDG_Succs ) ) ) ;
	    }
	    DrawBottomChannel( W, dt->Sideways_From, dt, dt->DDG_Bot_Channel );
	    if( clone->DGL_Text[0] ){
		// when to show text on the line?
		// solution: find top/bottom waypoints on the line.
		// case 1: no waypoints: can't happen
		// case 2: only one waypoint: show the text at that waypoint.
		// case 3: many waypoints: show the text at both first/last ones.
		P_DDG firstway, lastway;
		firstway = dg;
		lastway = dt;
		if( firstway == lastway ){
		    // case 2, show text once only
		    Sint x = DGX( W, lastway );
		    Sint y = DGY( W, lastway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}else if( dg->DDG_Type == ddg_rway ){
		    // case 3, but a continuation of a longer line
		    // show text at lastway only
		    Sint x = DGX( W, lastway );
		    Sint y = DGY( W, lastway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}else{
		    // case 3
		    // show text at firstway and lastway
		    Sint x = DGX( W, firstway );
		    Sint y = DGY( W, firstway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		    x = DGX( W, lastway );
		    y = DGY( W, lastway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}
	    }
	}
	break;

    case ddg_holder :
    case ddg_way :
	break;

    }
}// DGdraw

void DGDrawLine( Draw_Window* W, P_DDG dg, P_DDG ds, P_Ptr_DDG ddglink, 
	Sint firstlevel ){
    Sint x1, y1, x2, y2, x3, y3, x4, y4;
    Bool arrowstart, arrowend;
    P_Ptr_DG clone = ddglink->DDGL_Clone;
    if( clone->DGL_Type == dgl_invis ) return;
    switch( dg->DDG_Type ){
    case ddg_clone:
    case ddg_way:
	arrowstart = clone->DGL_Arrow == dgl_start || 
		     clone->DGL_Arrow == dgl_both;
	arrowend = clone->DGL_Arrow == dgl_end || 
		     clone->DGL_Arrow == dgl_both;
	if( dg->DDG_Type == ddg_clone || dg->DDG_Level == firstlevel ){
	    DGLineAttributes( W, clone->DGL_Style, clone->DGL_Color );
	    DGLineStart( W, dg, 0, x1, y1 );
	    DGLineEnd( W, ds, 0, x2, y2 );
	    if( dg->DDG_Type == ddg_clone && arrowstart && W->notdragging ){
		DGDrawArrow( W, x2, y2, x1, y1 );
	    }
	    P_DDG dt, lastdt, firstway, lastway;
	    startline();
	    addline( W, x1, y1 );
	    addline( W, x2, y2 );
	    if( ds->DDG_Type == ddg_clone ){
		lastdt = dg;
		dt = ds;
		x3 = x1; y3 = y1;
		x4 = x2; y4 = y2;
	    }else{
		lastdt = ds;
		for( dt = link( first( ds->DDG_Succs )); dt->DDG_Type != ddg_clone;
			dt = link( first( dt->DDG_Succs )) ){
		    lastdt = dt;
		}
		DGLineStart( W, lastdt, 0, x3, y3 );
		addline( W, x3, y3 );
		DGLineEnd( W, dt, 0, x4, y4 );
		addline( W, x4, y4 );
	    }
	    showline( W );
	    if( dt->DDG_Type == ddg_clone && arrowend && W->notdragging ){
		DGDrawArrow( W, x3, y3, x4, y4 );
	    }
	    if( clone->DGL_Text[0] ){
		// when to show text on a line?
		// solution: find first/last waypoints on the line.
		// case 1: no waypoints: show the text in the middle of the line.
		// case 2: only one waypoint: show the text at that waypoint.
		// case 3: many waypoints: show the text at both first/last ones.
		firstway = ds;
		lastway = lastdt;
		if( dg->DDG_Type == ddg_clone && ds->DDG_Type == ddg_clone ){
		    // case 1
		    Sint x = (x1+x2)/2;
		    Sint y = (y1+y2)/2;
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}else if( firstway == lastway ){
		    // case 2, show text once only
		    Sint x = DGX( W, lastway );
		    Sint y = DGY( W, lastway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}else if( dg->DDG_Type == ddg_way ){
		    // case 3, but a continuation of a longer line
		    // show text at lastway only
		    Sint x = DGX( W, lastway );
		    Sint y = DGY( W, lastway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}else{
		    // case 3
		    // show text at firstway and lastway
		    Sint x = DGX( W, firstway );
		    Sint y = DGY( W, firstway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		    x = DGX( W, lastway );
		    y = DGY( W, lastway );
		    DGDrawText( W, x, y, clone->DGL_Text, NULL, 0 );
		}
	    }
	}
    }
}// DGDrawLine

void DGDrawInterfereLine( Draw_Window* W, P_DDG ddg, 
					P_DDG dds, P_Ptr_DDG link ){
    DGLineAttributes( W, link->DDGL_Clone->DGL_Style, 
		link->DDGL_Clone->DGL_Color );
    if( ddg->DDG_Position == dds->DDG_Position ) return;
    if( ddg->DDG_Position > dds->DDG_Position ){
	P_DDG d = ddg;
	ddg = dds;
	dds = d;
    }
    Sint x1, x2, y1, y2;
    if( ddg->DDG_Position+1 == dds->DDG_Position ){
	// edge between two neighbors
	x1 = DGXInt( W, ddg, 0, 0, 1 );
	y1 = DGYInt( W, ddg, 0, 0, 1 );
	x2 = DGXInt( W, dds, 0, 1, 0 );
	y2 = DGYInt( W, dds, 0, 1, 0 );
    }else if( ddg->DDG_Position == 0 && 
		dds->DDG_Position == W->graph->DG_Count_Nodes-1 ){
	// edge between two neighbors
	x1 = DGXInt( W, ddg, 0, 1, 0 );
	y1 = DGYInt( W, ddg, 0, 1, 0 );
	x2 = DGXInt( W, dds, 0, 0, 1 );
	y2 = DGYInt( W, dds, 0, 0, 1 );
    }else{
	// edge between two random nodes
	x1 = DGXInt( W, ddg, 1, 0, 0 );
	y1 = DGYInt( W, ddg, 1, 0, 0 );
	x2 = DGXInt( W, dds, 1, 0, 0 );
	y2 = DGYInt( W, dds, 1, 0, 0 );
    }
    XDrawLine (Global_Display, W->window, W->gc, 
		xcanvas( W, x1 ), ycanvas( W, y1 ), 
		xcanvas( W, x2 ), ycanvas( W, y2 ) );
}// DGDrawInterferenceLine



// visit each graph node, and its successors
void DG_Draw_Graph( Draw_Window* W, P_DG_Graph GR ) 
{
Sint i,j;

    // plot nodes

    Sint h;
    Sint hfirst = First_Level( W );
    Sint hlast = Last_Level( W );
    for( h = hfirst; h <= hlast; ++h ){
	P_DDG_Head head = GR->headers+h;
        // plot level 'head'
	Visit_List( dg, head->Level_List, DDG ){
	    if( dg->DDG_Show ){
		// draw node 'dg'
		Bool dolinks = 0;
		if( Node_Appears( W, dg ) ){
		    DGdraw( W, dg, hfirst );
		    dolinks = 1;
		}
		Visit_Ptr_List( ds, dg->DDG_Succs, DDG ){
		    if( ds->DDG_Show ){
			if( dolinks || Node_Appears( W, ds ) ){
			    DGDrawLine( W, dg, ds, ptr_ds, hfirst );
			}
		    }
		}End_Visit;
	    }
	}End_Visit;
    }

}// DG_Draw_Graph


void DG_Draw_Interference( Draw_Window* W, P_DG_Graph GR ) 
{
Sint i,j;

    // plot nodes
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( ddg->DDG_Show ){
	    DGdraw( W, ddg, 0 );
	    Visit_Ptr_List( dds, ddg->DDG_Succs, DDG ){
		if( dds->DDG_Show ){
		    // only draw the line once!
		    DGDrawInterfereLine( W, ddg, dds, ptr_dds );
		}
	    }End_Visit;
	}
    }End_Visit;
}// DG_Draw_Interference

static Bool Continue = 1, Exit = 0;
static Uint Event_Nest = 0;
static Uint Active_Windows = 0;

static void Event_Loop() 
{
    ++Event_Nest;
    if( Event_Nest == 1 ){
	// Only dispatch events if we're not in an event already
	Continue = 1;
	while( Continue && Active_Windows > 0 ){
	    XEvent event;
	    XtAppNextEvent( Global_App, &event );
	    XtDispatchEvent( &event );
	}
    }
    --Event_Nest;
}// Event_Loop

Uint DG_Graph_Plot( P_DG_Graph GR, char *name, char* initialfont,
	char* leftfont, char* rightfont ){
Sint slider_size;
int n = 0;
Draw_Window *W;
    W = new Draw_Window( GR, initialfont, leftfont, rightfont );
    ++Active_Windows;

    W->toplevel = XtAppCreateShell( name, "Crescent", 
	applicationShellWidgetClass, Global_Display,
	NULL, 0 );

    // Add event handler to handle resizing
    XtAddEventHandler( W->toplevel, StructureNotifyMask, FALSE, 
			    handle_resize, W );

    // create a paned window
    W->pane = XtVaCreateManagedWidget( "pane",
	    xmPanedWindowWidgetClass, W->toplevel,
	    XmNallowResize, TRUE,
	    NULL );
    XtVaGetValues( W->pane, XmNmarginWidth, &(W->paneindent), NULL );

    // Add the list of menu 'buttons'
    W->menubar = XmCreateMenuBar( W->pane, "menu bar", NULL, 0 );
    XtManageChild( W->menubar );
    Uint count_buttons = 0;
    Visit_List( button, GR->all_buttons, DGbutton ){
	++count_buttons;
	// Now, add menu item
	if( button->Button_Menu ){
	    Widget pane = XmCreatePulldownMenu( W->menubar, "menu_pane", NULL, 0 );
	    Visit_List( menu, button->Button_Menu->Item_List, DG_List_Item ){
		Widget mitem = XmCreatePushButton( pane, menu->Item_Text, NULL, 0 );
		XtManageChild( mitem );
		XtAddCallback( mitem, XmNactivateCallback, List_Item_Press, menu );
	    }End_Visit;
	    int n = 0;
	    Arg args[2];
	    XtSetArg( args[n], XmNsubMenuId, pane ); ++n;
	    Widget cascade = XmCreateCascadeButton( W->menubar, 
			button->Button_Text, args, n );
	    XtManageChild( cascade );
	}else{
	    Widget menubutton = XmCreateCascadeButton( W->menubar, 
		    button->Button_Text, NULL, 0 );
	    XtManageChild( menubutton );
	    XtAddCallback( menubutton, XmNactivateCallback, Button_Press, button );
	}
    }End_Visit;
    if( count_buttons == 0 ){
	// always add a 'quit' button
	Widget menubutton = XmCreateCascadeButton( W->menubar, 
		"Close", NULL, 0 );
	XtManageChild( menubutton );
	XtAddCallback( menubutton, XmNactivateCallback, Close_Handler, W );
    }

    // Make a frame for the bulletin board
    W->frame =  XtCreateManagedWidget( "frame", xmFrameWidgetClass,
			    W->pane, NULL, 0);

    // create bulletin board widget inside to contain the rest of the stuff
    W->board =  XtCreateManagedWidget( "board", xmBulletinBoardWidgetClass,
			    W->frame, NULL, 0);

    // create a drawing canvas on which to draw the graph
    W->canvas = XtVaCreateManagedWidget ("canvas", xmDrawingAreaWidgetClass,
		    W->board, 
		    XtNx, 0,
		    XtNy, 0,
		    XtNwidth, W->canvas_width,
		    XtNheight, W->canvas_height,
		    NULL );

    // add callback handler to redraw the graph on "expose" events
    XtAddCallback( W->canvas, XmNexposeCallback, handle_expose, W );
    // add callback handler for input on the canvas
    XtAddCallback( W->canvas, XmNinputCallback, handle_input, W );

    // add an X scroll bar
    slider_size = min( W->draw_width, W->draw_show_width );
    W->xscroll = XtVaCreateManagedWidget ("xscroll", xmScrollBarWidgetClass,
		    W->board, 
		    XmNorientation, XmHORIZONTAL,
		    XmNprocessingDirection, XmMAX_ON_RIGHT,
		    XtNx, 0,
		    XtNy, 14 + W->canvas_height,
		    XtNwidth, W->canvas_width,
		    XmNminimum, 0,
		    XmNmaximum, W->draw_width,
		    XmNvalue, W->draw_x,
		    XmNincrement, 10,
		    XmNsliderSize, slider_size,
		    XmNpageIncrement, slider_size,
		    NULL );
    XtAddCallback( W->xscroll, XmNvalueChangedCallback, handle_xscroll, W );
    XtAddCallback( W->xscroll, XmNdragCallback, handle_xscroll_drag, W );

    // add a Y scroll bar
    slider_size = min( W->draw_height, W->draw_show_height );
    if( W->graph->PTree && !(W->graph->Tree) ){
	// If this is to be drawn from the bottom,
	// put the y-slider all the way down.
	W->draw_y = W->draw_height - slider_size;
    }
    W->yscroll = XtVaCreateManagedWidget ("yscroll", xmScrollBarWidgetClass,
		    W->board, 
		    XtNy, 0,
		    XtNx, 18 + W->canvas_width,
		    XtNheight, W->canvas_height,
		    XmNminimum, 0,
		    XmNmaximum, W->draw_height,
		    XmNvalue, W->draw_y,
		    XmNincrement, 10,
		    XmNsliderSize, slider_size,
		    XmNpageIncrement, slider_size,
		    NULL );
    XtAddCallback (W->yscroll, XmNvalueChangedCallback, handle_yscroll, W );
    XtAddCallback (W->yscroll, XmNdragCallback, handle_yscroll_drag, W );

    // add the scale slider
    W->scale = XtVaCreateManagedWidget ("scale", xmScaleWidgetClass,
		    W->board, 
		    XtNy, 0,
		    XtNx, 35 + W->canvas_width,
		    XmNscaleHeight, W->canvas_height,
		    XmNminimum, W->pixnum/5,
		    XmNmaximum, W->pixnum*10,
		    XmNvalue, W->pixden,
		    XmNscaleMultiple, W->pixden/2,
		    NULL );
    XtAddCallback (W->scale, XmNvalueChangedCallback, handle_scale, W );
    XtAddCallback (W->scale, XmNdragCallback, handle_scale_drag, W );

    // draw the stuff
    XtRealizeWidget(W->toplevel);

    // get the display and window, and create the graphics context
    W->window = XtWindow( W->canvas );
    W->gc = XCreateGC( Global_Display, W->window, 0, 0 );
    DGGCAttributes( W, dg_solid );
    XSetState( Global_Display, W->gc, 
		BlackPixel (Global_Display, DefaultScreen (Global_Display)),
		WhitePixel (Global_Display, DefaultScreen (Global_Display)), 
		GXcopy, 0xffffffff );
    W->fontstruct = addfont( W->fontname );
    XSetFont( Global_Display, W->gc, W->fontstruct->fid );
    GlobalFont = W->fontstruct->fid;
    W->leftfontstruct = addfont( W->leftfontname );
    W->rightfontstruct = addfont( W->rightfontname );

    // Set the name
    // XStoreName( Global_Display, W->window, "nerd" );

    // set colors for foreground, background of graphics context

    Event_Loop();
    return Exit;
}// DG_Graph_Plot

void DG_Graph_Init( )  
{
    int n = 0;
    XtToolkitInitialize();
    Global_App = XtCreateApplicationContext();
    Global_Display = XtOpenDisplay( Global_App, "", "", "", NULL, 0, &n, NULL );
}// DG_Graph_Init


void DG_Graph_Fini( )  
{
    killfonts();
    XtCloseDisplay( Global_Display );
}// DG_Graph_Fini



void handle_expose ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmDrawingAreaCallbackStruct *call_data  = (XmDrawingAreaCallbackStruct *)calld;

    if( W->graph->Interference ){
	DG_Draw_Interference( W, W->graph );
    }else{
	DG_Draw_Graph( W, W->graph );
    }
}

void handle_xscroll_drag ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmScrollBarCallbackStruct *call_data  = (XmScrollBarCallbackStruct *)calld;
    W->notdragging = 0;
    W->draw_x = call_data->value;
    XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
}

void handle_xscroll ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmScrollBarCallbackStruct *call_data  = (XmScrollBarCallbackStruct *)calld;
    W->notdragging = 1;
    W->draw_x = call_data->value;
    XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
}


void handle_yscroll_drag ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmScrollBarCallbackStruct *call_data  = (XmScrollBarCallbackStruct *)calld;
    W->notdragging = 0;
    W->draw_y = call_data->value;
    XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
}

void handle_yscroll ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmScrollBarCallbackStruct *call_data  = (XmScrollBarCallbackStruct *)calld;
    W->notdragging = 1;
    W->draw_y = call_data->value;
    XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
}

static void make_sliders( Draw_Window* W ) 
{
Sint slider_size;

    if( W->draw_show_width != W->old_draw_show_width ||
	W->draw_show_height != W->old_draw_show_height ||
	W->draw_width != W->old_draw_width ||
	W->draw_height != W->old_draw_height ){
	W->old_draw_x = -1;
	W->old_draw_y = -1;
	W->old_draw_width = W->draw_width;
	W->old_draw_height = W->draw_height;
	W->old_draw_show_width = W->draw_show_width;
	W->old_draw_show_height = W->draw_show_height;
    }

    slider_size = min( W->draw_width, W->draw_show_width );
    W->draw_x = min( W->draw_x, W->draw_width - slider_size );
    if( W->draw_x != W->old_draw_x ){
	XtVaSetValues( W->xscroll, 
	    XmNmaximum, W->draw_width,
	    XmNsliderSize, slider_size,
	    XmNpageIncrement, slider_size,
	    XmNvalue, W->draw_x,
	    NULL );
	W->old_draw_x = W->draw_x;
    }

    slider_size = min( W->draw_height, W->draw_show_height );
    W->draw_y = min( W->draw_y, W->draw_height - slider_size );
    if( W->draw_y != W->old_draw_y ){
	XtVaSetValues (W->yscroll, 
	    XmNmaximum, W->draw_height,
	    XmNvalue, W->draw_y,
	    XmNsliderSize, slider_size,
	    XmNpageIncrement, slider_size,
	    NULL );
	W->old_draw_y = W->draw_y;
    }
}// make_sliders

void scale( Widget w, Draw_Window* W, XmScaleCallbackStruct *call_data ) 
{
    if (W->pixden != call_data->value) {
	W->pixden = call_data->value;
	W->draw_show_width = W->canvas_width * W->pixden/W->pixnum;
	W->draw_show_height = W->canvas_height * W->pixden/W->pixnum;

	XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
	make_sliders( W );
    }
}

void handle_scale ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmScaleCallbackStruct *call_data  = (XmScaleCallbackStruct *)calld;

    scale( w, W, call_data );
    if( W->notdragging == 0 ){
	W->notdragging = 1;
	XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
	/// ***
	//  I don't know why this should be needed, but if it isn't here,
	// then when letting up the button when dragging the scale bar,
	// the screen is not cleared and replotted, as it should be.
	// MW 04-29-93
	// DG_Draw_Graph( W, W->graph );
    }
}

void handle_scale_drag ( Widget w, XtPointer client_data, XtPointer calld ) 
{
Draw_Window* W = (Draw_Window*)client_data;
XmScaleCallbackStruct *call_data  = (XmScaleCallbackStruct *)calld;
    W->notdragging = 0;
    scale( w, W, call_data );
}


static void FindNode( Draw_Window* W, Sint x, Sint y ) 
{
const Sint X = 1; // slack
    if( W->graph->Interference ){
	Visit_Ptr_List( ddg, W->graph->all_DDG, DDG ){
	    if( ddg->DDG_Show ){
		// get x,y
		Sint xcenter = DGXInt( W, ddg, 0, 0, 0 );
		Sint ycenter = DGYInt( W, ddg, 0, 0, 0 );
		if( upper( W, ycenter ) - X <= y && 
				y <= lower( W, ycenter ) + X ){
		    if( left( W, xcenter ) - X <= x && 
				x <= right( W, xcenter ) ){
			P_DG clone = ddg->DDG_Clone;
			if( clone->DG_Handler != 0 ){
			    Uint32 val = (clone->DG_Handler)( W->graph, clone, clone->DG_Param );
			    if( val != 0 ){
				Continue = 0;
				return;
			    }
			}
		    }
		}
	    }
	}End_Visit;
    }else{
	Sint h;
	Sint hfirst = First_Level( W );
	Sint hlast = Last_Level( W );
	for( h = hfirst; h <= hlast; ++h ){
	    P_DDG_Head head = W->graph->headers + h;
	    Sint ycenter = Ycenter( W, h );
	    if( upper( W, ycenter ) - X <= y && y <= lower( W, ycenter ) + X ){
		// look for a node here that matches
		Visit_List( dg, head->Level_List, DDG ){
		    if( dg->DDG_Type == ddg_clone ){
			Sint xcenter = DGX( W, dg );
			if( left( W, xcenter ) - X <= x && x <= right( W, xcenter ) ){
			    P_DG clone = dg->DDG_Clone;
			    if( clone->DG_Handler != 0 ){
				Uint32 val = (clone->DG_Handler)( W->graph, clone, clone->DG_Param );
				if( val != 0 ){
				    Continue = 0;
				    return;
				}
			    }
			}
		    }
		}End_Visit;
	    }
	}
    }
}// FindNode

void handle_input( Widget w, XtPointer client_data, XtPointer calld ){ 
Draw_Window* W = (Draw_Window*)client_data;
XmDrawingAreaCallbackStruct *call_data  = (XmDrawingAreaCallbackStruct *)calld;
XEvent * event;
KeySym keysym;
char c;
Sint x, y;
Sint x_unit, y_unit;
Sint x_pos, y_pos;

    event = call_data->event;
    switch (event->type) {
    case ButtonPress:
	x = event->xbutton.x;
	y = event->xbutton.y;

	if (event->xbutton.button == 1) {
	    x_unit = (x * W->pixden / W->pixnum) + W->draw_x;
	    y_unit = (y * W->pixden / W->pixnum) + W->draw_y;
	    FindNode( W, x_unit, y_unit );
	}else if (event->xbutton.button == 3){
	    Continue = 0;
	    return;
	}
	break;

    case KeyPress:
	XLookupString (&(event->xkey), &c, 1, &keysym, NULL);
	if (c == 'q' || c == 'x')
	    Continue = 0;
	    return;
	break;

    case ButtonRelease:
    case KeyRelease:
	break;

    default:
	break;
    }
}


void handle_resize( Widget w, caddr_t client_data, XEvent *event, char* dunno ) 
{
Draw_Window* W = (Draw_Window*)client_data;
Dimension new_height, new_width;
Position new_x, new_y;
Sint slider_size;

    XtVaGetValues( w, XtNwidth, &new_width, XtNheight, &new_height, NULL );

    if( W->last_width != new_width ){
	W->last_width = new_width;
	// resize the buttons
	XtWidgetGeometry Wgeom, Hgeom;
	Wgeom.request_mode = CWWidth;
	Wgeom.width = new_width - 2*W->paneindent;
	Hgeom.request_mode = CWHeight;
	XtQueryGeometry( W->menubar, &Wgeom, &Hgeom );
	XtUnmanageChild( W->menubar );
	XtVaSetValues( W->menubar,
		XmNpaneMinimum, Hgeom.height,
		XmNpaneMaximum, Hgeom.height,
		NULL );
	XtManageChild( W->menubar );
    }

    Dimension BoardWidth, BoardHeight;
    XtVaGetValues( W->board, 
		XtNwidth, &BoardWidth,
		XtNheight, &BoardHeight,
		NULL );
    if( BoardWidth - 65 != W->canvas_width || 
	BoardHeight - 40 != W->canvas_height ){
	W->canvas_height = BoardHeight - 40;
	W->canvas_width = BoardWidth - 65;
	W->draw_show_width = W->canvas_width * W->pixden/W->pixnum;
	W->draw_show_height = W->canvas_height * W->pixden/W->pixnum;

	XtVaSetValues( W->canvas, 
		XtNwidth, W->canvas_width,
		XtNheight, W->canvas_height,
		NULL );

	slider_size = min( W->draw_width, W->draw_show_width );
	W->draw_x = min(W->draw_x, W->draw_width - slider_size );
	XtVaSetValues( W->xscroll, 
		XtNx, 0,
		XtNy, 14 + W->canvas_height,
		XtNwidth, W->canvas_width,
		XmNsliderSize, slider_size,
		XmNpageIncrement, slider_size,
		XmNvalue, W->draw_x,
		NULL );

	slider_size = min( W->draw_height, W->draw_show_height );
	W->draw_y = min( W->draw_y, W->draw_height - slider_size );
	XtVaSetValues (W->yscroll, 
		XtNy, 0,
		XtNx, 18 + W->canvas_width,
		XtNheight, W->canvas_height,
		XmNsliderSize, slider_size,
		XmNpageIncrement, slider_size,
		XmNvalue, W->draw_y,
		NULL );

	XtVaSetValues (W->scale, 
		XtNy, 0,
		XtNx, 35 + W->canvas_width,
		XmNscaleHeight, W->canvas_height,
		NULL );

	make_sliders( W );

	XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
    }
}// handle_resize

void DG_Clear( P_DG_Graph GR ) 
{
Draw_Window* W = (Draw_Window*)(GR->Graphics);
    XClearArea (Global_Display, W->window, 0, 0, 0, 0, TRUE);
    W->draw_width = (GR->DG_Width_Max + 1) * W->circlesize;
    W->draw_height = (GR->DG_Level_Max + 1) * 2 * W->circlesize;
    make_sliders( W );
}// DG_Clear


// Call this when a button is pressed.
// This then calls the user's button handler.
void Button_Press( Widget w, XtPointer client_data, XtPointer calld ){ 
P_DGbutton button = (P_DGbutton)client_data;
    (button->Button_Handler)(button->Button_Graph, button->Button_Param);
}// Button_Press

void List_Item_Press( Widget w, XtPointer client_data, XtPointer calld ){ 
P_DG_List_Item item = (P_DG_List_Item)client_data;
    if( item->Item_Handler )(item->Item_Handler)(item->List, item->Item_Param);
}// List_Item_Press


Uint DG_Close( P_DG_Graph graph, Uint g ) 
{
Draw_Window* W = (Draw_Window*)(graph->Graphics);
    XtDestroyWidget( W->toplevel );
    --Active_Windows;
    return 0;
}// DG_Close

void Close_Handler( Widget w, XtPointer client_data, XtPointer calld ){ 
Draw_Window* W = (Draw_Window*)(client_data);
    XtDestroyWidget( W->toplevel );
    --Active_Windows;
}// Close_Handler

Uint DG_Quit( P_DG_Graph graph, Uint g ) 
{
    Continue = 0;
    Exit = 1;
    return 0; // to eliminate warnings
}

Uint DG_Next( P_DG_Graph graph, Uint g ) 
{
Draw_Window* W = (Draw_Window*)(graph->Graphics);
    Continue = 0;
    return 0;
}

void PirateCursor( P_DG_Graph GR ) 
{
Draw_Window* W = (Draw_Window*)(GR->Graphics);
static Cursor cursor = 0;
XSetWindowAttributes attrs;
    if( !cursor ){
	cursor = XCreateFontCursor( Global_Display, XC_pirate );
    }
    attrs.cursor = cursor;
    XChangeWindowAttributes( Global_Display, XtWindow( W->toplevel ), CWCursor, &attrs );
}

void WaitCursor( P_DG_Graph GR ) 
{
Draw_Window* W = (Draw_Window*)(GR->Graphics);
static Cursor cursor = 0;
XSetWindowAttributes attrs;
    if( !cursor ){
	cursor = XCreateFontCursor( Global_Display, XC_clock );
    }
    attrs.cursor = cursor;
    XChangeWindowAttributes( Global_Display, XtWindow( W->toplevel ), CWCursor, &attrs );
}

void RestoreCursor( P_DG_Graph GR ) 
{
Draw_Window* W = (Draw_Window*)(GR->Graphics);
XSetWindowAttributes attrs;
    attrs.cursor = None;
    XChangeWindowAttributes( Global_Display, XtWindow( W->toplevel ), CWCursor, &attrs );
}

class Draw_List 
{
public:
    Widget toplevel;	// top level X object
    Widget pane; 	// paned window of X objects
    Widget menubar;	// menu pane, in the paned window
    Widget scrolledlist;// scrolled list
    Window window;
    Sint32 last_width;
    unsigned short paneindent;
    P_DG_List list;
    P_char fontname;
    XFontStruct* fontstruct;
    Draw_List( P_DG_List );
};

Draw_List::Draw_List( P_DG_List L ) 
{
    memset( this, 0, sizeof( Draw_List ) );
    list = L;
}

void Close_List_Handler( Widget w, XtPointer client_data, XtPointer calld ){ 
Draw_List* GL = (Draw_List*)(client_data);
    XtDestroyWidget( GL->toplevel );
    --Active_Windows;
}// Close_List_Handler

void Resize_List( Widget w, caddr_t client_data, XEvent *event, char* dunno ) 
{
Draw_List* GL = (Draw_List*)client_data;
Dimension new_height, new_width;
Position new_x, new_y;
Sint slider_size;

    XtVaGetValues( w, XtNwidth, &new_width, XtNheight, &new_height, NULL );

    if( GL->last_width != new_width ){
	// resize the buttons
	XtWidgetGeometry Wgeom, Hgeom;
	Wgeom.request_mode = CWWidth;
	Wgeom.width = new_width - 2*GL->paneindent;
	Hgeom.request_mode = CWHeight;
	XtQueryGeometry( GL->menubar, &Wgeom, &Hgeom );
	XtUnmanageChild( GL->menubar );
	XtVaSetValues( GL->menubar,
		XmNpaneMinimum, Hgeom.height,
		XmNpaneMaximum, Hgeom.height,
		NULL );
	XtManageChild( GL->menubar );
	GL->last_width = new_width;
    }
}// Resize_List

void SelectProc( Widget w, XtPointer client, XtPointer data ) 
{
    XmListCallbackStruct *cb = (XmListCallbackStruct*)(data);
    Draw_List* GL = (Draw_List*)client;

    if( cb->reason == XmCR_BROWSE_SELECT ){
	// find this element
	P_DG_List L = GL->list;
	P_DG_List_Item item = first( L->Item_List );
	for( Uint position = cb->item_position; position > 1; --position ){
	    item = next( item );
	}
	if( item->Item_Handler )(item->Item_Handler)(item->List, item->Item_Param); }
}// SelectProc

void DG_Display_List( P_DG_List L ) 
{
Sint slider_size;
int n = 0;
Draw_List *GL;
    GL = new Draw_List( L );
    ++Active_Windows;

    GL->toplevel = XtAppCreateShell( L->List_Name, "Crescent", 
	applicationShellWidgetClass, Global_Display,
	NULL, 0 );

    XtAddEventHandler( GL->toplevel, StructureNotifyMask, FALSE, 
			    Resize_List, GL );

    // create a paned window
    GL->pane = XtVaCreateManagedWidget( "pane",
	    xmPanedWindowWidgetClass, GL->toplevel,
	    XmNallowResize, TRUE,
	    NULL );
    XtVaGetValues( GL->pane, XmNmarginWidth, &(GL->paneindent), NULL );

    // Make a place for all the buttons, however many there be
    GL->menubar = XmCreateMenuBar( GL->pane, "menu bar", NULL, 0 );
    XtManageChild( GL->menubar );

    Uint count_buttons = 0;
    Visit_List( button, L->List_Buttons, DGbutton ){
	++count_buttons;
	Widget menubutton = XmCreateCascadeButton( GL->menubar, 
		button->Button_Text, NULL, 0 );
	XtManageChild( menubutton );
	XtAddCallback( menubutton, XmNactivateCallback, Button_Press, button );
    }End_Visit;
    if( count_buttons == 0 ){
	// always add a 'quit' button
	Widget menubutton = XmCreateCascadeButton( GL->menubar, 
		"Close", NULL, 0 );
	XtManageChild( menubutton );
	XtAddCallback( menubutton, XmNactivateCallback, Close_List_Handler, GL );
    }

    GL->scrolledlist =  XmCreateScrolledList( GL->pane, "list", NULL, 0 );
    XtManageChild( GL->scrolledlist );
    XtAddCallback( GL->scrolledlist, XmNbrowseSelectionCallback, SelectProc, GL );
    Uint itemcount = 0;
    Visit_List( item, L->Item_List, DG_List_Item ){
	XmString String = XmStringCreateSimple( item->Item_Text );
	++itemcount;
	XmListAddItem( GL->scrolledlist, String, itemcount );
    }End_Visit;
    XtVaSetValues( GL->scrolledlist, XmNvisibleItemCount, itemcount, NULL );

    // draw the stuff
    XtRealizeWidget(GL->toplevel);

    Event_Loop();
}// DG_Display_List

void DG_Hide_List( P_DG_List L )
{
}// DG_Hide_List
