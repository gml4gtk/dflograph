// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dgplot.h
//
// 08 Dec 94 [mw]
// $Id: dgplot.h,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $

// Interactively plot the directed graph

// Call DG_Graph_Init to set up the screen for plotting
extern void DG_Graph_Init( void );

// Call DG_Graph to plot a graph
extern Uint DG_Graph_Plot( P_DG_Graph DGR, char *name, char* fontname, char* leftfont, char* rightfont );

// Call DG_Graph_Fini to shut down the graphics screen when done plotting
extern void DG_Graph_Fini( void );

// call this to replot the graph if something has changed
extern void DG_Draw_Graph( );

// call this as a button to quit program
// pass in the P_DG_Graph
extern Uint DG_Quit( P_DG_Graph graph, Uint g );

// call this as a button to close this window
extern Uint DG_Close( P_DG_Graph graph, Uint g );

// call this to replot screen
extern void DG_Clear( P_DG_Graph GR );

// call this to change the cursor to a pirate
extern void PirateCursor( P_DG_Graph GR );

// call this to change the cursor to a 'waiting' clock symbol
extern void WaitCursor( P_DG_Graph GR );

// call this to restore the cursor
extern void RestoreCursor( P_DG_Graph GR );
