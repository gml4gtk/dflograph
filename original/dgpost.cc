// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dgpost.cc
//
// 03 Aug 95 [mw]
// $Id: dgpost.cc,v 1.3 1996/08/28 21:12:47 mwolfe Exp mwolfe $


// Put the graph on a Postscript file

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
FILE* PostFile = NULL;

// X position for interference graphs
static inline Sint DGXInt( P_DDG dg, Bool edge, Bool left, Bool right ){
////    Sint C = circlesize;
////    Sint XC = C+Radius;
////    float theta = MYPI*2/graph->DG_Count_Nodes;
////    float thetan = dg->DDG_Position*theta;
////    if( left ) thetan = thetan - theta/4;
////    if( right ) thetan = thetan + theta/4;
////    float sin_thetan = sin(thetan);
////    Sint R = Radius;
////    if( edge ) R = R-C/2;
////    return (Sint)(XC+sin_thetan*R);
}

// Y position for interference graphs
static inline Sint DGYInt( P_DDG dg, Bool edge, Bool left, Bool right ){
////    Sint C = circlesize;
////    Sint YC = C+Radius;
////    float theta = MYPI*2/graph->DG_Count_Nodes;
////    float thetan = dg->DDG_Position*theta;
////    if( left ) thetan = thetan - theta/4;
////    if( right ) thetan = thetan + theta/4;
////    float cos_thetan = cos(thetan);
////    Sint R = Radius;
////    if( edge ) R = R-C/2;
////    return (Sint)(YC+cos_thetan*R);
}

static void DGDrawText( Sint x, Sint y, char* Name, char* Name2, int doname2 ){
    if( doname2 ){
	if( Name == NULL && Name2 == NULL ) {
	}else if( Name == NULL ){
	    fprintf( PostFile, "%d %d () (%s) TextTwo\n", x, y, Name2 );
	}else if( Name2 == NULL ){
	    fprintf( PostFile, "%d %d (%s) () TextTwo\n", x, y, Name );
	}else{
	    fprintf( PostFile, "%d %d (%s) (%s) TextTwo\n", x, y, Name, Name2 );
	}
    }else if( Name != NULL ){
	fprintf( PostFile, "%d %d (%s) CText\n", x, y, Name );
    }
}// DGDrawText

static void DGDrawText2( Sint x1, Sint y1, Sint x2, Sint y2,
	char* Name, char* Name2, int doname2 ){
    if( Name || (doname2 && Name2) ){
	fprintf( PostFile, "%d 0.6 mul %d 0.4 mul add %d 0.6 mul %d 0.4 mul add ",
		x1, x2, y1, y2 );
	if( doname2 ){
	    if( Name == NULL ){
		fprintf( PostFile, "() (%s) TextTwo\n", Name2 );
	    }else if( Name2 == NULL ){
		fprintf( PostFile, "(%s) () TextTwo\n", Name );
	    }else{
		fprintf( PostFile, "(%s) (%s) TextTwo\n", Name, Name2 );
	    }
	}else{
	    fprintf( PostFile, "(%s) CText\n", Name );
	}
    }
}// DGDrawText2

// Set the line attributes for this style
static DG_Styles LastStyle = dg_bold;
static Uint32 LastColor = 9999;

static void DGLineAttributes( DG_Styles style, Uint32 color, Bool destroy = 0 ){
    if( style == LastStyle ) return;
    if( destroy ){
	LastStyle = dg_blank;
    }else{
	LastStyle = style;
    }
    switch( style ){
    case dg_solid: 
	fprintf( PostFile, "[] 0 setdash solidwidth setlinewidth %% SolidLine\n" );
	break;
    case dg_bold:
	fprintf( PostFile, "[] 0 setdash boldwidth setlinewidth %% BoldLine\n" );
	break;
    case dg_dashed:
	fprintf( PostFile, "dashedpattern 0 setdash solidwidth setlinewidth %% DashedLine\n" );
	break;
    case dg_dotted:
	fprintf( PostFile, "dottedpattern 0 setdash dottedwidth setlinewidth %% DottedLine\n" );
	break;
    case dg_blank:
	// nothing to do
	break;
    }
}// DGLineAttributes

static void putname( DG_Shapes shape ){
    switch( shape ){
    case dg_circle :
	fprintf( PostFile, " circle" );
	break;
    case dg_box :
	fprintf( PostFile, " box" );
	break;
    case dg_hex :
	fprintf( PostFile, " hex" );
	break;
    case dg_diamond :
	fprintf( PostFile, " diamond" );
	break;
    case dg_oct :
	fprintf( PostFile, " oct" );
	break;
    default:
      break;
    }
}

// draw node for 'dg'
static void DGdraw( P_DDG dg ){
    P_DDG frlink;
    P_DDG dt;
    P_Ptr_DG ddglink;
    P_DG dgc;
    Bool arrowstart, arrowend;
    Sint x = dg->DDG_Position;
    Sint y = dg->DDG_Level;
    switch( dg->DDG_Type ){
    case ddg_clone:
	dgc = dg->DDG_Clone;
	if( dgc->DG_Style_Top == dgc->DG_Style_Bot &&
		dgc->DG_Shape_Top == dgc->DG_Shape_Bot ){
	    DGLineAttributes( dgc->DG_Style_Top, dgc->DG_Color );
	    fprintf( PostFile, "%d %d ", x, y );
	    putname( dgc->DG_Shape_Top );
	    fprintf( PostFile, "\n" );
	}else{
	    DGLineAttributes( dgc->DG_Style_Top, dgc->DG_Color );
	    fprintf( PostFile, "%d %d ", x, y );
	    putname( dgc->DG_Shape_Top );
	    fprintf( PostFile, "top\n" );

	    DGLineAttributes( dgc->DG_Style_Bot, dgc->DG_Color );
	    fprintf( PostFile, "%d %d ", x, y );
	    putname( dgc->DG_Shape_Bot );
	    fprintf( PostFile, "bottom\n" );
	}
	DGDrawText( x, y, dgc->DG_Name, dgc->DG_Name2, dgc->DG_Second_Line );
	if( dgc->DG_Bar_Style != dg_blank ){
	    DGLineAttributes( dgc->DG_Bar_Style, dgc->DG_Bar_Color );
	    fprintf( PostFile, "newpath %d XLeft %d YCenter moveto "
				"%d XRight %d YCenter lineto stroke %% Bar\n",
		x, y, x, y );
	}
	break;

    case ddg_rway :
    case ddg_bottom_corner :
    case ddg_both_corners :
    case ddg_top_corner :
	ddglink = dg->DDG_Ptr_Clone;
	arrowstart = ddglink->DGL_Arrow == dgl_start || 
			 ddglink->DGL_Arrow == dgl_both;
	arrowend = ddglink->DGL_Arrow == dgl_end || 
			 ddglink->DGL_Arrow == dgl_both;
	if( dg->DDG_Type != ddg_top_corner && 
		dg->DDG_Type != ddg_both_corners ){
	    fprintf( PostFile, "firstrow %d eq { ", dg->DDG_Level );
	    fprintf( PostFile, "%d %d %d %d startS eline\n",
		dg->DDG_Position, dg->DDG_Level, dg->DDG_Position, dg->DDG_Level+1 );
	}else{
	    P_DDG tolink = dg->Sideways_To;
	    fprintf( PostFile, "%d %d %d %d ",
		tolink->DDG_Position, dg->DDG_Position, dg->DDG_Level, 
		dg->DDG_Top_Channel );
	    putname( tolink->DDG_Clone->DG_Shape_Top );
	    fprintf( PostFile, "offset " );
	    if( arrowend ){
		fprintf( PostFile, "startTA " );
	    }else{
		fprintf( PostFile, "startTC " );
	    }
	    fprintf( PostFile, "midTC\n" );
	}

	// find the level at which it ends
	dt = dg;
	if( dg->DDG_Type != ddg_both_corners ){
	    for( ; dt && dt->DDG_Type != ddg_bottom_corner;
			dt = link( first( dt->DDG_Succs ) ) ) ;
	}
	frlink = dt->Sideways_From;
	fprintf( PostFile, "%d %d %d %d midBC ",
	    dt->DDG_Position, frlink->DDG_Position, dt->DDG_Level, 
	    dt->DDG_Bot_Channel );
	putname( frlink->DDG_Clone->DG_Shape_Bot );
	fprintf( PostFile, "offset " );
	if( arrowstart ){
	    fprintf( PostFile, "endBA " );
	}else{
	    fprintf( PostFile, "endBC " );
	}
	if( ddglink->DGL_Text[0] ){
	    /* show text at one place (both_corners) or
	     * at both corners (top_corner) or
	     * at bottom corner (else) */
	    fprintf( PostFile, "\n" );
	    if( dg->DDG_Type == ddg_top_corner ){
		DGDrawText( dg->DDG_Position, dg->DDG_Level, ddglink->DGL_Text,
			NULL, 0 );
	    }
	    DGDrawText( dt->DDG_Position, dt->DDG_Level, ddglink->DGL_Text,
			NULL, 0 );
	}
	if( dg->DDG_Type != ddg_top_corner && 
		dg->DDG_Type != ddg_both_corners ){
	    fprintf( PostFile, "} if" );
	}
	fprintf( PostFile, "\n" );
	break;

    case ddg_holder :
    case ddg_way :
	break;

    }
}// DGdraw

static void DGDrawLine( P_DDG dg, P_DDG ds, P_Ptr_DDG ddglink, 
	Sint fromside = 0, Sint toside = 0 ){
Sint x1, y1, x2, y2;
Bool arrowstart, arrowend;
P_DDG lastdt;
P_DDG dt;
    P_Ptr_DG clone = ddglink->DDGL_Clone;
    if( clone->DGL_Type == dgl_invis ) return;

    switch( dg->DDG_Type ){
    case ddg_clone:
    case ddg_way :
	arrowstart = clone->DGL_Arrow == dgl_start || 
		     clone->DGL_Arrow == dgl_both;
	arrowend = clone->DGL_Arrow == dgl_end || 
		     clone->DGL_Arrow == dgl_both;
	if( dg->DDG_Type != ddg_clone ){
	    // check that we haven't already done it
	    fprintf( PostFile, "firstrow %d eq {", dg->DDG_Level );
	    DGLineAttributes( clone->DGL_Style, clone->DGL_Color, 1 );
	}else{
	    DGLineAttributes( clone->DGL_Style, clone->DGL_Color );
	}
	fprintf( PostFile, "%d %d %d %d ",
	    dg->DDG_Position, dg->DDG_Level, ds->DDG_Position, ds->DDG_Level );
	if( dg->DDG_Type != ddg_clone ){
	    fprintf( PostFile, "startS " );
	}else if( arrowstart ){
	    fprintf( PostFile, "startA " );
	}else{
	    fprintf( PostFile, "startP " );
	}
	lastdt = ds;
	if( ds->DDG_Type != ddg_clone ){
	    fprintf( PostFile, "cornerT " );
	    // find the level at which it ends
	    for( dt = link( first( ds->DDG_Succs )); dt->DDG_Type != ddg_clone; 
			dt = link( first( dt->DDG_Succs ) ) ){
		lastdt = dt;
	    }
	    fprintf( PostFile, "%d %d %d %d cornerB ",
		lastdt->DDG_Position, lastdt->DDG_Level, 
		    dt->DDG_Position, dt->DDG_Level );
	}
	if( arrowend ){
	    fprintf( PostFile, "endA " );
	}else{
	    fprintf( PostFile, "endP " );
	}
	if( clone->DGL_Text[0] ){
	    /* show text in middle of line (no waypoints)
	     * or at top/bottom waypoints (start at clone)
	     * or at bottom waypoint only (else) */
	    fprintf( PostFile, "\n" );
	    if( dg->DDG_Type == ddg_clone && ds->DDG_Type == ddg_clone ){
		DGDrawText2( dg->DDG_Position, dg->DDG_Level, 
			ds->DDG_Position, ds->DDG_Level,
			clone->DGL_Text, NULL, 0 );
	    }else{
		if( dg->DDG_Type == ddg_clone ){
		    DGDrawText( ds->DDG_Position, ds->DDG_Level, 
			clone->DGL_Text, NULL, 0 );
		}
		DGDrawText( lastdt->DDG_Position, lastdt->DDG_Level, 
			clone->DGL_Text, NULL, 0 );
	    }
	}
	if( dg->DDG_Type != ddg_clone ){
	    fprintf( PostFile, "} if" );
	}
	fprintf( PostFile, "\n" );
	break;
    case ddg_rway :
    case ddg_bottom_corner :
    case ddg_both_corners :
    case ddg_top_corner :
	break;
    }
}// DGDrawLine

static void DGDrawInterfereLine( P_DDG ddg, P_DDG dds, P_Ptr_DDG link ){
////    DGLineAttributes( link->DDGL_Clone->DGL_Style, 
////		link->DDGL_Clone->DGL_Color );
////    if( ddg->DDG_Position == dds->DDG_Position ) return;
////    if( ddg->DDG_Position > dds->DDG_Position ){
////	P_DDG d = ddg;
////	ddg = dds;
////	dds = d;
////    }
////    Sint x1, x2, y1, y2;
////    if( ddg->DDG_Position+1 == dds->DDG_Position ){
////	// edge between two neighbors
////	x1 = DGXInt( W, ddg, 0, 0, 1 );
////	y1 = DGYInt( W, ddg, 0, 0, 1 );
////	x2 = DGXInt( W, dds, 0, 1, 0 );
////	y2 = DGYInt( W, dds, 0, 1, 0 );
////    }else if( ddg->DDG_Position == 0 && 
////		dds->DDG_Position == W->graph->DG_Count_Nodes-1 ){
////	// edge between two neighbors
////	x1 = DGXInt( W, ddg, 0, 1, 0 );
////	y1 = DGYInt( W, ddg, 0, 1, 0 );
////	x2 = DGXInt( W, dds, 0, 0, 1 );
////	y2 = DGYInt( W, dds, 0, 0, 1 );
////    }else{
////	// edge between two random nodes
////	x1 = DGXInt( W, ddg, 1, 0, 0 );
////	y1 = DGYInt( W, ddg, 1, 0, 0 );
////	x2 = DGXInt( W, dds, 1, 0, 0 );
////	y2 = DGYInt( W, dds, 1, 0, 0 );
////    }
////    XDrawLine (Global_Display, W->window, W->gc, 
////		xcanvas( W, x1 ), ycanvas( W, y1 ), 
////		xcanvas( W, x2 ), ycanvas( W, y2 ) );
}// DGDrawInterferenceLine


// visit each graph node, and its successors
void DG_Post_Graph( P_DG_Graph GR, Bool scaledrawing ){

Sint i,j;

    // set variables to hold size of the plot
    fprintf( PostFile, "/columns %d def /rows %d def\n",
	GR->DG_Width_Max, GR->DG_Level_Max );
    fprintf( PostFile, "/Title (%s) def\n", GR->Name );
    DGLineAttributes( dg_solid, 0 );
    // plot nodes
    for( Uint h = 1; h <= GR->DG_Level_Max; ++h ){
	fprintf( PostFile, "/ROW%4.4d {\n", h );
	P_DDG_Head head = GR->headers+h;
        // plot level 'head'
	Visit_List( dg, head->Level_List, DDG ){
	    if( dg->DDG_Show ){
		// draw node 'dg'
		DGdraw( dg );
		Visit_Ptr_List( ds, dg->DDG_Succs, DDG ){
		    if( ds->DDG_Show ){
			DGDrawLine( dg, ds, ptr_ds );
		    }
		}End_Visit;
	    }
	}End_Visit;
	fprintf( PostFile, "} def\n" );
    }
    if( scaledrawing ){
	fprintf( PostFile, "/firstrow 1 def /lastrow rows def\n" );
	fprintf( PostFile, "/firstcol 1 def\n" );
	fprintf( PostFile, "scaledrawing\n" );
    }else{
	fprintf( PostFile, "%% Go through all pages, how many rows per page\n" );
	fprintf( PostFile, "1 maxlength csize 2 mul div round rows {\n"
		"/firstrow exch def \n"
		"/lastrow maxlength csize div round firstrow add def\n" );
	fprintf( PostFile, "%% Go through all pages across columns per page\n" );
	fprintf( PostFile, "1 maxwidth csize div round columns {\n"
		"/firstcol exch def\n" );
    }
    fprintf( PostFile,	"%% Show Title\n"
			"defaultfont setfont maxwidth Title stringwidth pop sub 2 div \n"
			"height moveto Title show\n" );
    LastStyle = dg_blank;
    DGLineAttributes( dg_solid, 0 );
    fprintf( PostFile, "%% Go through all rows\n1 1 %d { showrow } for\n", GR->DG_Level_Max );
    fprintf( PostFile, "showpage\n" );
    if( scaledrawing ){
    }else{
	fprintf( PostFile, "} for } for %% loop for rows, columns\n" );
    }
    fprintf( PostFile, "%%%%Trailer\n" );
    fprintf( PostFile, "%%%%End\n" );

}// DG_Post_Graph


void DG_Post_Interference( P_DG_Graph GR ){
Sint i,j;

    // plot nodes
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( ddg->DDG_Show ){
	    DGdraw( ddg );
	    Visit_Ptr_List( dds, ddg->DDG_Succs, DDG ){
		if( dds->DDG_Show ){
		    // only draw the line once!
		    DGDrawInterfereLine( ddg, dds, ptr_dds );
		}
	    }End_Visit;
	}
    }End_Visit;
}// DG_Post_Interference

void DG_Post_Init( Sint circlesize,
	char* font, Sint fontsize, Sint spacing, Bool scalefont ){
    fprintf( PostFile, "%%!PS-Adobe-2.0 EPSF-2.0\n"
		"%%%%Creator: dflo\n"
		"%%%%Title: dflo flow graph diagram\n"
		"%%%%CreationDate: ??\n"
		"%%%%Pages: 1\n"
		"%%%%PageOrder: Ascent\n"
		"%%%%BoundingBox: 000 000 999 999\n"
		"%%%%EndComments\n" );

    fprintf( PostFile, "%% diameter of node\n"
		"/csize %d def\n", circlesize );
    fprintf( PostFile, "%% relative size of curve for edges, percent of csize\n"
		"/chcurve %d def\n"
		"/chradius csize chcurve 100 div mul def\n", 20 );
    fprintf( PostFile, "%% relative size of text (fontsize in percent of csize)\n"
		"/fontrelsize %d def\n", fontsize );
    fprintf( PostFile, "%% fontsize\n"
		"/fontsize fontrelsize 100 div csize mul def\n", fontsize );
    fprintf( PostFile, "%% font spacing, whether to scale font\n"
		"/relspacing %d def\n"
		"/spacing relspacing 100 div csize mul def\n"
		"/scalethefont %d def\n", spacing, scalefont );
    fprintf( PostFile, "%% maximum width of text in a node, if not scaled\n"
		"/TextMax 0.80 def\n" );
    fprintf( PostFile, "%% default font name\n"
		"/defaultfont /%s findfont fontsize scalefont def\n", font );
    fprintf( PostFile, "%% current font size\n"
		"/currentfontsize fontsize def\n" );
    fprintf( PostFile, "%% get size of descenders\n"
		"/descender defaultfont /FontBBox get 1 get neg 1000 div def\n" );
    fprintf( PostFile, "%% maximum page length, 72 pts per inch, 10 inches\n"
		"/maxlength 72 10 mul def\n" );
    fprintf( PostFile, "%% maximum page width, 72 pts per inch, 7.5 inches\n"
		"/maxwidth 72 8.0 mul def\n" );
    fprintf( PostFile, "%% size of solid line\n"
		"/solidwidth csize 66 div def\n" );
    fprintf( PostFile, "%% size of bold line\n"
		"/boldwidth solidwidth 2 mul def\n" );
    fprintf( PostFile, "%% size of dotted line\n"
		"/dottedwidth solidwidth 2 mul def\n" );
    fprintf( PostFile, "%% dotted line pattern\n"
		"/dottedpattern [1 1] def\n" );
    fprintf( PostFile, "%% dashed line pattern\n"
		"/dashedpattern [4 4] def\n" );
    fprintf( PostFile, "%% radius of node\n"
		"/crad csize 2 div def\n" );
    fprintf( PostFile, "%% 1/2 radius of node\n"
		"/crad2 csize 4 div def\n" );
    fprintf( PostFile, "%% negatives\n"
		"/ncsize csize neg def\n" );
    fprintf( PostFile, "%% radius of node\n"
		"/ncrad crad neg def\n" );
    fprintf( PostFile, "%% 1/2 radius of node\n"
		"/ncrad2 crad2 neg def\n" );
    fprintf( PostFile, "%% 1/3 of diameter, for hexagons\n"
		"/csize3 csize 3 div def\n" );
    fprintf( PostFile, "%% crad/sqrt(2)\n"
		"/craddivsqrt2 csize 2 2 sqrt mul div def\n" );
    fprintf( PostFile, "%% tan(22.5 degrees)\n"
		"/tan225 22.5 sin 22.5 cos div def\n" );
    fprintf( PostFile, "%% crad*tan(22.5), for octagons\n"
		"/cradtan225 crad tan225 mul def\n" );
    fprintf( PostFile, "%% crad - crad*tan(22.5), for octagons\n"
		"/cradmtan225 crad cradtan225 sub def\n" );
    fprintf( PostFile, "%% angle of arrowhead\n"
		"/aangle 180 15 div def\n" );
    fprintf( PostFile, "%% length of arrowhead\n"
		"/alength csize 2 div def\n" );
    fprintf( PostFile, "%% move to center of node\n"
		"/XCenter { firstcol sub 1 add csize mul } def\n" );
    fprintf( PostFile, "%% move to center of node\n"
		"/height 0.5 72 mul maxlength add def\n"
		"/YCenter { firstrow sub 2 mul 1 add csize mul height exch sub } def\n" );
    fprintf( PostFile, "%% move to left edge\n"
		"/XLeft { XCenter crad sub } def\n" );
    fprintf( PostFile, "%% move to right edge\n"
		"/XRight { XCenter crad add } def\n" );
    fprintf( PostFile, "%% move to upper edge\n"
		"/YUpper { YCenter crad add } def\n" );
    fprintf( PostFile, "%% move to lower edge\n"
		"/YLower { YCenter crad sub } def\n" );
    fprintf( PostFile, "%% cross channel first increment\n"
		"/chbeta { csize 4 div } def\n" );
    fprintf( PostFile, "%% cross channel distance increments\n"
		"/chalpha { csize 8 div } def\n" );
    fprintf( PostFile, "%% Temp Row Number\n"
		"/ROWPROC (ROW0000) def\n" );
    fprintf( PostFile, "%% plot this row\n"
		"/showthisrow { ROWPROC 3 (0000) putinterval\n"
		" ROWPROC exch 4 string cvs dup length 7 sub neg exch putinterval\n"
		" ROWPROC load exec } def\n" );
    fprintf( PostFile, "%% show this row\n"
		"/showrow { dup dup firstrow ge exch lastrow le and { showthisrow }{ pop } ifelse } def\n" );
    fprintf( PostFile, "%% shorten line if it ends in an arrow  x1 y1 x2 y2 fixarrow line x2' y2'\n"
		"/fixarrowline {4 2 roll 2 index sub exch 3 index sub atan \n"
		"3 -1 roll 1 index cos alength 2 div mul add \n"
		"3 1 roll sin alength 2 div mul add } def\n" );
    // draw an arrowhead.
    // We have a line from (x1,y1) to (x2,y2)
    // The angle of this line is theta = atan((y1-y2)/(x1-x2))
    // We have to negate the Y coordinate, since X-window coordinates
    // have increasing Y going down
    // The angle of the boundaries of the arrowhead are
    // a1 = -theta+aangle and a2 = -theta-aangle.
    // The length of the boundaries of the arrowhead are l = alength,
    // so the (x,y) coordinates of the corners of the arrowhead 
    // are (x2,y2), (x2+cos(a1)*l,y2+sin(a1)*l), (x2+cos(a2)*l,y2+sin(a2)*l)
    fprintf( PostFile, "%% draw an arrowhead\n"
	"/arrowhead { /y2 exch def /x2 exch def /y1 exch def /x1 exch def\n" 
	"	/theta y1 y2 sub x2 x1 sub atan def\n"
	"	/a1 theta neg aangle add def\n"
	"	/a2 theta neg aangle sub def\n"
	"	newpath x2 y2 moveto\n"
	"	x2 a1 cos alength mul sub y2 a1 sin alength mul sub lineto\n"
	"	x2 a2 cos alength mul sub y2 a2 sin alength mul sub lineto\n"
	"	closepath fill } def\n" );
    fprintf( PostFile, "%% a b minimum min(a,b)\n"
	"/min { 2 copy le { }{ exch }ifelse pop } def\n" );
    fprintf( PostFile, "%% scale size of circle to fit\n"
	"/scaledrawing{ maxwidth columns div csize min /csize exch def\n"
	"	maxlength rows 2 mul div csize min /csize exch def } def\n" ); 
    fprintf( PostFile, "%% draw circle\n"
	"/circle { newpath exch XCenter exch YCenter "
			"crad 0 360 arc stroke } def \n" ); 
    fprintf( PostFile, "%% draw upper half of circle\n"
	"/circletop { newpath exch XCenter exch YCenter "
			"crad 0 180 arc stroke } def \n" ); 
    fprintf( PostFile, "%% draw lower half of circle\n"
	"/circlebottom { newpath exch XCenter exch YCenter "
			"crad 180 360 arc stroke } def \n" ); 
    fprintf( PostFile, "%% draw box\n"
	"/box { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad crad rmoveto csize 0 rlineto\n"
	"	0 ncsize rlineto ncsize 0 rlineto\n"
	"	0 csize rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw upper half of box\n"
	"/boxtop { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto 0 crad rlineto\n"
	"	csize 0 rlineto 0 ncrad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw lower half of box\n"
	"/boxbottom { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto 0 ncrad rlineto\n"
	"	csize 0 rlineto 0 crad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw hex\n"
	"/hex { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto crad2 crad rlineto\n"
	"	crad 0 rlineto crad2 ncrad rlineto\n"
	"	ncrad2 ncrad rlineto ncrad 0 rlineto\n"
	"	ncrad2 crad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw upper half of hex\n"
	"/hextop { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto crad2 crad rlineto\n"
	"	crad 0 rlineto crad2 ncrad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw lower half of hex\n"
	"/hexbottom { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto crad2 ncrad rlineto\n"
	"	crad 0 rlineto crad2 crad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw diamond\n"
	"/diamond { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto crad crad rlineto\n"
	"	crad ncrad rlineto ncrad ncrad rlineto\n"
	"	ncrad crad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw upper half of diamond\n"
	"/diamondtop { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto crad crad rlineto\n"
	"	crad ncrad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw lower half of diamond\n"
	"/diamondbottom { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto crad ncrad rlineto\n"
	"	crad crad rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw octagon\n"
	"/oct { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad cradtan225 rmoveto\n"
	"	cradmtan225 cradmtan225 rlineto\n"
	"	cradtan225 2 mul 0 rlineto\n"
	"	cradmtan225 cradmtan225 neg rlineto\n"
	"	0 cradtan225 neg 2 mul rlineto\n"
	"	cradmtan225 neg cradmtan225 neg rlineto\n"
	"	cradtan225 2 mul neg 0 rlineto\n"
	"	cradmtan225 neg cradmtan225 rlineto\n"
	"	0 cradtan225 2 mul rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw upper half of octagon\n"
	"/octtop { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto\n"
	"	0 cradtan225 rlineto\n"
	"	cradmtan225 cradmtan225 rlineto\n"
	"	cradtan225 2 mul 0 rlineto\n"
	"	cradmtan225 cradmtan225 neg rlineto\n"
	"	0 cradtan225 neg rlineto stroke } def\n" );
    fprintf( PostFile, "%% draw lower half of octagon\n"
	"/octbottom { newpath exch XCenter exch YCenter moveto\n"
	"	ncrad 0 rmoveto\n"
	"	0 cradtan225 neg rlineto\n"
	"	cradmtan225 cradmtan225 neg rlineto\n"
	"	cradtan225 2 mul 0 rlineto\n"
	"	cradmtan225 cradmtan225 rlineto\n"
	"	0 cradtan225 rlineto stroke } def\n" );
    fprintf( PostFile, "/eline { pop pop pop pop } def\n" );
    fprintf( PostFile, "%% given start/end coordinates, start a straight line\n"
	"/startS { newpath 3 index XCenter 3 index YUpper moveto "
		"3 index XCenter 3 index YLower lineto } def\n" );
    fprintf( PostFile, "%% given start/end coordinates, start a plain line\n"
	"/startP { newpath 3 index XCenter 3 index YLower moveto } def\n" );
    fprintf( PostFile, "%% given start/end coordinates, start an arrow line\n"
	"/startA { newpath 3 index XCenter 3 index YLower "
		"3 index XCenter 3 index YUpper "
		"4 2 roll 4 copy arrowhead fixarrowline moveto } def\n" );
    fprintf( PostFile, "%% xpos1 ypos1 xpos2 ypos2\n"
	"/cornerT {  3 index 2 index lt /LEFT exch def\n"
	"       2 index YLower 1 index YUpper sub\n"
	"       2 index XCenter 5 index XCenter sub LEFT {}{neg} ifelse\n"
	"       atan neg 90 add\n"
	"       dup 2 div sin chradius mul 2 index YUpper add\n"
	"       3 index XCenter chradius LEFT {sub}{add}ifelse exch\n"
	"       chradius 4 3 roll LEFT{0 arcn}{neg 180 add 180 arc}ifelse eline} def\n" );
    fprintf( PostFile, "%% xpos1 ypos1 xpos2 ypos2\n"
	"/cornerB { 3 index 2 index lt /LEFT exch def\n"
	"        2 index YLower 1 index YUpper sub\n"
	"        2 index XCenter 5 index XCenter sub LEFT {} {neg} ifelse atan\n"
	"        neg 90 add dup 2 div sin chradius mul 4 index YLower exch add\n"
	"        5 index XCenter chradius LEFT {add}{sub}ifelse exch\n"
	"        chradius 4 3 roll LEFT {180 exch 180 add arc} {0 exch neg arcn}ifelse}def\n" );

    fprintf( PostFile, "/endline { stroke eline } def\n" );
    fprintf( PostFile, "%% given start/end coordinates, end a straight line\n"
	"/endS { 1 index XCenter 1 index YUpper lineto "
		"1 index XCenter 1 index YCenter lineto endline } def\n" );
    fprintf( PostFile, "%% given start/end coordinates, end a plain line\n"
	"/endP { 1 index XCenter 1 index YUpper lineto endline } def\n" );
    fprintf( PostFile, "%% given start/end coordinates, end an arrow line\n"
	"/endA { 3 index XCenter 3 index YLower "
		"3 index XCenter 3 index YUpper "
		"4 copy fixarrowline lineto stroke arrowhead eline } def\n" );

    fprintf( PostFile, "%% offset from center of shape to corner\n" );
    fprintf( PostFile, "/circleoffset{ craddivsqrt2 dup }def\n" );
    fprintf( PostFile, "/boxoffset{ crad dup }def\n" );
    fprintf( PostFile, "/hexoffset{ csize3 crad }def\n" );
    fprintf( PostFile, "/diamondoffset{ crad2 dup }def\n" );
    fprintf( PostFile, "/octoffset{ cradtan225 crad add 2 div dup }def\n" );

    fprintf( PostFile, "%% draw top channel line given xpos1 xpos2 ypos channel xoffset yoffset\n"
		// xp1 xp2 yp ch xoff yoff
	"/startTC { 5 index 5 index gt /LEFT exch def\n"
	"       3 index YCenter add\n"
		// xp1 xp2 yp ch xoff y1
	"	exch LEFT { neg } if 5 index XCenter add exch\n"
		// xp1 xp2 yp ch x1 y1
	"	3 2 roll chalpha mul chbeta add 3 index YUpper add\n"
		// xp1 xp2 yp x1 y1 y2
	"	dup 4 1 roll\n"
		// xp1 xp2 yp y2 x1 y1 y2
	"	2 copy exch sub LEFT { neg } if 3 index add exch\n"
		// xp1 xp2 yp y2 x1 y1 x2 y2
	"	4 2 roll moveto\n"
	"       chradius sub exch chradius 22.5 sin mul LEFT {sub}{add}ifelse exch \n"
	"       chradius LEFT{45 90 arc}{135 90 arcn}ifelse } def\n" );
		// xp1 xp2 yp y2 
    fprintf( PostFile, "%% draw top channel line with arrow given xp1 xp2 yp ch xoff yoff\n"
		// xp1 xp2 yp ch xoff yoff
	"/startTA { 5 index 5 index gt /LEFT exch def\n"
	"       3 index YCenter add\n"
		// xp1 xp2 yp ch xoff y1
	"	exch LEFT { neg } if 5 index XCenter add exch\n"
		// xp1 xp2 yp ch x1 y1
	"	3 2 roll chalpha mul chbeta add 3 index YUpper add\n"
		// xp1 xp2 yp x1 y1 y2
	"	dup 4 1 roll\n"
		// xp1 xp2 yp y2 x1 y1 y2
	"	2 copy exch sub LEFT { neg } if 3 index add exch\n"
		// xp1 xp2 yp y2 x1 y1 x2 y2
	"	4 2 roll 4 copy arrowhead\n"
		// xp1 xp2 yp y2 x2 y2 x1 y1
	"	3 index 3 index fixarrowline\n"
		// xp1 xp2 yp y2 x1 y1 x2 y2
	"	moveto\n"
	"       chradius sub exch chradius 22.5 sin mul LEFT{sub}{add}ifelse exch \n"
	"       chradius LEFT{45 90 arc}{135 90 arcn}ifelse } def\n" );
		// xp1 xp2 yp y2 
    fprintf( PostFile, "%% draw rest of top channel line given xpos1 xpos2 ypos ychannel\n"
		// xp1 xp2 yp y2 
	"/midTC{ chradius sub 2 index XCenter chradius LEFT{add}{sub}ifelse exch\n"
	"       chradius LEFT{90 180 arc}{90 0 arcn}ifelse\n"
		// xp1 xp2 yp 
	"	YUpper exch XCenter exch lineto pop }def\n" );
    fprintf( PostFile, "%% draw start of bottom channel line given xpos1 xpos2 ypos channel\n"
		// xp1 xp2 yp c
	"/midBC{ 3 index 3 index lt /LEFT exch def\n"
	"       chalpha mul chbeta add 1 index YLower exch sub\n"
	"       3 index XCenter chradius LEFT{add}{sub}ifelse\n"
	"       1 index chradius add chradius LEFT {180 270 arc} {360 270 arcn} ifelse}def\n" );
    fprintf( PostFile, "%% draw bottom channel line given xpos1 xpos2 ypos ychannel xoffset yoffset\n"
		// xp1 xp2 yp ych xoff yoff
	"/endBC { 3 index YCenter exch sub\n"
		// xp1 xp2 yp ych xoff y2
	"	exch LEFT { neg } if 4 index XCenter add exch\n"
		// xp1 xp2 yp ych x2 y2
	"	3 2 roll\n"
		// xp1 xp2 yp x2 y2 y1
	"	2 copy sub LEFT { neg } if 3 index add\n"
	"       chradius 22.5 sin mul LEFT{sub}{add}ifelse exch\n"
	"       chradius add chradius LEFT{270 315 arc}{270 225 arcn}ifelse\n"
	"       lineto stroke pop pop pop } def\n" );
    fprintf( PostFile, "%% draw bottom channel arrow line given xpos1 xpos2 ypos ychannel xoffset yoffset\n"
		// xp1 xp2 yp ych xoff yoff
	"/endBA { 3 index YCenter exch sub\n"
		// xp1 xp2 yp ych xoff y2
	"	exch LEFT { neg } if 4 index XCenter add exch\n"
		// xp1 xp2 yp ych x2 y2
	"	3 2 roll\n"
		// xp1 xp2 yp x2 y2 y1
	"	2 copy sub LEFT { neg } if 3 index add 2 copy\n"
	"       chradius 22.5 sin mul LEFT{sub}{add}ifelse exch\n"
	"       chradius add chradius LEFT{270 315 arc}{270 225 arcn}ifelse\n"
	"       exch 4 2 roll 4 copy fixarrowline lineto stroke arrowhead pop pop pop } def\n" );

    fprintf( PostFile, "%% text on top of stack, scale or shorten it if needed, leave width on stack\n"
	"/TextScale { defaultfont setfont /currentfontsize fontsize def\n"
	"    dup stringwidth pop\n"
	"    dup csize TextMax mul gt { scalethefont 1 eq {\n"
	"        csize TextMax mul exch div dup defaultfont exch scalefont setfont\n"
	"        fontsize mul /currentfontsize exch def\n"
	"        dup stringwidth pop } if } if } def\n" );
    fprintf( PostFile, "%% xpos ypos text, show text in box\n"
	"/Text { TextScale 2 div neg 3 index XCenter add\n"
	"   2 index YCenter currentfontsize 2 div sub descender currentfontsize mul add moveto show pop pop } def\n" );
    fprintf( PostFile, "%% xpos ypos text1 text2, show texts in box\n"
	"/TextTwo { TextScale 2 div neg 4 index XCenter add\n"
	"   3 index YCenter currentfontsize sub spacing 2 div sub descender currentfontsize mul add moveto show\n"
	"   TextScale 2 div neg 3 index XCenter add\n"
	"   2 index YCenter spacing 2 div add descender add moveto show pop pop } def\n" );
    fprintf( PostFile, "%% xpos ypos text, show text after clearing area\n"
	"/CText { TextScale 2 div \n"
	"   3 index XCenter 3 index YCenter \n"
	"   1 index 3 index sub 1 index currentfontsize 2 div sub moveto\n"
	"   1 index 3 index sub 1 index currentfontsize 2 div add lineto\n"
	"   1 index 3 index add 1 index currentfontsize 2 div add lineto\n"
	"   1 index 3 index add 1 index currentfontsize 2 div sub lineto\n"
	"   closepath 1 setgray fill 0 setgray pop pop \n"
	"   neg 3 index XCenter add\n"
	"   2 index YCenter currentfontsize 2 div sub descender currentfontsize mul add moveto show pop pop } def\n" );
}// DG_Post_Init
