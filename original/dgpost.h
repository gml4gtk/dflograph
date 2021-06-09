// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dgpost.h
//
// 04 Aug 95 [mw]
// $Id: dgpost.h,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $

// Interactively plot the directed graph

// Call DG_Post_Init to set up the Postscript file
extern void DG_Post_Init( Sint circlesize,
	char* font, Sint fontsize, Sint spacing, Bool scalefont );

// Call DG_Post_Graph to print a graph
extern void DG_Post_Graph( P_DG_Graph GR, Bool scaledrawing );

extern FILE* PostFile;
