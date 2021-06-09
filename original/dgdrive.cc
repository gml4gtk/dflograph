// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dgdrive.cc
//
// 08 Dec 94 [mw]
// $Id: dgdrive.cc,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $

// make a directed graph, then try to plot it interactively

#include "portable.h"
#include "standard.h"
#include "error.h"
#include "dg.h"

static void dumplist( List_Ptr_DG& l ){
    Visit_Ptr_List( d, l, DG ){
	printf( " %s", d->DG_Name );
    }End_Visit;
}
static void dumpall( P_char str, P_DG_Graph GR ){
    printf( str );
    Visit_List( dg, GR->all_nodes, DG ){
	printf( "\n%s Succs:", dg->DG_Name ); dumplist( dg->DG_Succs );
	printf( "\n%s Preds:", dg->DG_Name ); dumplist( dg->DG_Preds );
    }End_Visit;
    printf( "\n" );
}

static void Find_Size( P_DG_Graph GR ){
    Visit_List( dg, GR->all_nodes, DG ){
        dg->DG_Height = 1; // Initially, just set height for all nodes to 1!
        dg->DG_Width = 1; // Initially, just set width for all nodes to 1!
			// Real width is 2*DG_Width
    }End_Visit;
}// Find_Size


// Perform a traversal of the graph, starting at node 'dgnode'.
// Assign levels to each node in the graph, starting with level '1'
// for top-level nodes.  
static void SpanTree( P_DG_Graph GR, P_DG dgnode, Uint level ){
    if( dgnode->DG_Level <= level ){
	// We either haven't reached this node before (DG_Level == 0)
	// or else we need to reset its level to a higher value
	dgnode->DG_Level = level;
	if( level >= GR->DG_Level_Max ) GR->DG_Level_Max = level;
	dgnode->DG_Extra = -1;   // flag that incoming edges are 'back' edges

	Visit_Ptr_List( succs, dgnode->DG_Succs, DG ){
	    if( succs->DG_Show ){
		// don't look at edges that are 'back' edges
		if( succs->DG_Extra == -1 ){
		    debug( 2, ( "link from %s to %s is a back edge",
			    dgnode->DG_Name, succs->DG_Name ) );
		}else{
		    SpanTree( GR, succs, level+dgnode->DG_Height );
		}
	    }
	}End_Visit;
	dgnode->DG_Extra = 0;
    }
}// SpanTree

static void Free_Old_Display_Nodes( P_DG_Graph GR ){
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	delete ddg;
    }End_Visit;
    Free_List( GR->all_DDG, Ptr_DDG );
    Visit_List( dg, GR->all_nodes, DG ){
	dg->DG_Clone = NULL;
    }End_Visit;
}

static void Assign_Levels( P_DG_Graph GR ){
    // Now, assign levels in the graph
    // Go to every node with no predecessors, assign them at the top
    // Initialize level to zero
    GR->DG_Old_Level_Max = GR->DG_Level_Max;
    GR->DG_Level_Max = 0;
    GR->DG_Width_Max = 0;
    Visit_List( dg, GR->all_nodes, DG ){
	dg->DG_Level = 0;
	dg->DG_Extra = 0;
	Visit_Ptr_List( pred, dg->DG_Preds, DG ){
            if( pred->DG_Show ){
		++(dg->DG_Extra); // count predecessors
            }
        }End_Visit;
    }End_Visit;
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show && dg->DG_Level == 0 && dg->DG_Extra == 0 ){
	    debug( 2, ( "start spanning tree at root %s", dg->DG_Name ) );
	    SpanTree( GR, dg, 1 );
	}
    }End_Visit;
    // See if there is a node in a cycle, with some predecessor visited, but not all
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show && dg->DG_Level == 0 ){
	    Uint maxpredlevel = 0;
	    Visit_Ptr_List( pred, dg->DG_Preds, DG ){
		if( pred->DG_Show && 
			pred->DG_Level + pred->DG_Height > maxpredlevel )
		    maxpredlevel = pred->DG_Level + pred->DG_Height;
	    }End_Visit;
	    if( maxpredlevel > 0 ){
		debug( 2, ( "start spanning tree at cycle node %s", dg->DG_Name ) );
                SpanTree( GR, dg, maxpredlevel );
            }
	} 
    }End_Visit;
    // If there are any nodes that haven't been assigned a level, they must be part
    // of a cycle.  Choose some node with no invisible predecessor edges, and start
    // there.
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show && dg->DG_Level == 0 ){
	    // see if there are any invisible incoming edges
	    // If there are, we want this node to be below that predecessor, and that
	    // predecessor must not have been levelized yet, either
	    Bool invispred = 0;
	    Visit_List( dglink, dg->DG_Preds, Ptr_DG ){
		if( dglink->DGL_Type == dgl_invis ){
                    invispred = 1;
                }
	    }End_Visit;
	    if( !invispred ){
            	debug( 2, ( "start spanning tree at node in tight cycle at %s", dg->DG_Name ) );
		SpanTree( GR, dg, 1 );
            }
        }
    }End_Visit;
    // Finally, see if there is any node still not given a level
    // If there is, there must be a cycle of invisible edges, so just ignore some
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show && dg->DG_Level == 0 ){
	    Uint maxpredlevel = 0;
	    Visit_Ptr_List( pred, dg->DG_Preds, DG ){
		if( pred->DG_Show && 
			pred->DG_Level + pred->DG_Height > maxpredlevel )
		    maxpredlevel = pred->DG_Level + pred->DG_Height;
	    }End_Visit;
	    debug( 2, ( "start spanning tree at lost node %s", dg->DG_Name ) );
	    SpanTree( GR, dg, maxpredlevel );
        }
    }End_Visit;
}// Assign_Levels



// Sink node 'dgnode' down to its highest successor.
// Ignore edges that are "back" edges in the original spanning tree.
// Decrement successor count (in DG_Extra) of its predecessors, and recurse when that reaches 0.
static void Sink_Node( P_DG_Graph GR, P_DG dgnode ){
    Uint succlevel = GR->DG_Level_Max + 1;
    Visit_Ptr_List( succ, dgnode->DG_Succs, DG ){
	if( succ->DG_Show ){
	    if( succ->DG_Level > dgnode->DG_Level && succ->DG_Level < succlevel ){
		succlevel = succ->DG_Level;
	    }
	}
    }End_Visit;
    debug( 2, ( "node %s on level %ld connected below to level %ld",
			dgnode->DG_Name, dgnode->DG_Level, succlevel ) );
    if( succlevel < GR->DG_Level_Max + 1 ){
	succlevel = succlevel - dgnode->DG_Height;
	if( succlevel > dgnode->DG_Level ){// move this node down
	    debug( 2, ( "move node %s down from level %ld to level %ld",
		    dgnode->DG_Name, dgnode->DG_Level, succlevel ) );
	    dgnode->DG_Level = succlevel;
	}
    }
    // go through the predecessors
    Visit_Ptr_List( pred, dgnode->DG_Preds, DG ){
	if( pred->DG_Show ){
	    --(pred->DG_Extra);
	    if( pred->DG_Extra == 0 ){ // have visited all its successors
		Sink_Node( GR, pred );
	    }
	}
    }End_Visit;
}// Sink_Node

static void Sink_Down( P_DG_Graph GR, Bool AllTheWay = 0 ){
    Visit_List( dg, GR->all_nodes, DG ){
	dg->DG_Extra = 0;
	Visit_Ptr_List( succ, dg->DG_Succs, DG ){
	    if( succ->DG_Show && succ->DG_Level > dg->DG_Level ){
		++(dg->DG_Extra); // count successors
            }
        }End_Visit;
    }End_Visit;
    // Look for a "leaf", a node with no higher-leveled successors
    Visit_List( dg, GR->all_nodes, DG ){
	if( first( dg->DG_Succs ) == NULL && dg->DG_Show ){
	    if( AllTheWay ){
		dg->DG_Level = GR->DG_Level_Max + 1 - dg->DG_Height;
	    }
	    Sink_Node( GR, dg );
	}
    }End_Visit;
}// Sink_Down


// Float node 'dgnode' up to its lowest predecessor.
// Ignore edges that are "back" edges in the original spanning tree.
// Decrement predecessor count (in DG_Extra) of its successors, and recurse when that reaches 0.
static void Float_Node( P_DG_Graph GR, P_DG dgnode ){
    Uint predlevel = 0;
    Visit_Ptr_List( pred, dgnode->DG_Preds, DG ){
	if( pred->DG_Show ){
	    if( pred->DG_Level < dgnode->DG_Level && 
		    pred->DG_Level + pred->DG_Height > predlevel ){
		predlevel = pred->DG_Level + pred->DG_Height;
	    }
	}
    }End_Visit;
    debug( 2, ( "node %s on level %ld connected above to level %ld",
			dgnode->DG_Name, dgnode->DG_Level, predlevel ) );
    if( predlevel > 0 ){
	if( predlevel < dgnode->DG_Level ){// move this node up
	    debug( 2, ( "move node %s up from level %ld to level %ld",
		    dgnode->DG_Name, dgnode->DG_Level, predlevel ) );
	    dgnode->DG_Level = predlevel;
	}
    }
    // go through the successors
    Visit_Ptr_List( succ, dgnode->DG_Succs, DG ){
	if( succ->DG_Show ){
	    --(succ->DG_Extra);
	    if( succ->DG_Extra == 0 ){ // have visited all its predecessors
		Float_Node( GR, succ );
	    }
	}
    }End_Visit;
}// Float_Node

static void Float_Up( P_DG_Graph GR ){
    Visit_List( dg, GR->all_nodes, DG ){
	dg->DG_Extra = 0;
	Visit_Ptr_List( pred, dg->DG_Preds, DG ){
	    if( pred->DG_Show && pred->DG_Level < dg->DG_Level ){
		++(dg->DG_Extra); // count predecessors
            }
        }End_Visit;
    }End_Visit;
    // Look for a "leaf", a node with no lower-leveled predecessors
    Visit_List( dg, GR->all_nodes, DG ){
	if( first( dg->DG_Preds ) == NULL && dg->DG_Show ){
	    Float_Node( GR, dg );
	}
    }End_Visit;
}// Float_Up



// create drawn node for 'dgnode'
static void Initial_DDG( P_DG_Graph GR, P_DG dgnode ){
Uint l;		// level of node 'dgnode'
Uint ll;	// level of successors of node 'dgnode' (level+height)
Uint h;
P_DDG newddg, oldddg;

    debug( 2, ("node %s at level %ld", dgnode->DG_Name, dgnode->DG_Level ) );
    P_DDG ddg = dgnode->DG_Clone;

    // Make 'holder' nodes for the 'height' of this node
    l = dgnode->DG_Level;
    ll = l + ddg->DDG_Clone->DG_Height;
    oldddg = ddg;
    for( h = l+1; h < ll; ++h ){
	newddg = new DDG( NULL, ddg_holder, h, GR );
	DDG_Link( oldddg, newddg, NULL );
	oldddg = newddg;
    }
    // look at each successor.
    // If it is at the next level, create that node, link to it, recurse to it.
    Visit_List( succedge, dgnode->DG_Succs, Ptr_DG ){
	P_DG succ = link( succedge );
	debug( 3, ( "succ link %s --> %s", dgnode->DG_Name, succ->DG_Name ) );
        if( succ->DG_Show ){
	    P_DDG lastddg = oldddg;
	    if( succ->DG_Level >= ll ){
		debug( 2, ( "node %s is below node %s at level %ld",
			succ->DG_Name, dgnode->DG_Name, succ->DG_Level ) );
		// put way-point nodes between here and there.
		for( h = ll; h < succ->DG_Level; ++h ){
		    debug( 2, ( "add a way-point at level %ld", h ) );
		    newddg = new DDG( succedge, ddg_way, h, GR );
		    DDG_Link( lastddg, newddg, succedge );
		    lastddg = newddg;
		}
		if( succ->DG_Clone == NULL ){
		    // Create the successor
		    debug( 2, ( "add node %s ", succ->DG_Name ) );
		    new DDG( succ, GR );
		    Initial_DDG( GR, succ );
		}
		DDG_Link( lastddg, succ->DG_Clone, succedge );
	    }
        }
    }End_Visit;
    // Look for back edges
    Visit_List( prededge, dgnode->DG_Preds, Ptr_DG ){
	P_DG pred = link( prededge );
	if( pred->DG_Show && pred->DG_Level >= l ){
	    debug( 2, ( "back edge from node %s at level %ld to node %s",
		    pred->DG_Name, pred->DG_Level, dgnode->DG_Name ) );
	    // back edge from the predecessor, treat it like a successor
	    // insert corner nodes at this level
	    debug( 2, ( "add a corner node at level %ld", l ) );
	    newddg = new DDG( prededge, ddg_top_corner, l, GR );
	    newddg->Sideways_To = ddg;
	    newddg->Sideways_Link = ddg->Sideways_Link;
	    ddg->Sideways_Link = newddg;
	    P_DDG lastddg = newddg;
	    for( h = l+1; h <= pred->DG_Level; ++h ){
		debug( 2, ( "add a way-point at level %ld", h ) );
		newddg = new DDG( prededge, ddg_rway, h, GR );
		DDG_Link( lastddg, newddg, prededge );
		lastddg = newddg;
	    }
	    if( lastddg->DDG_Type == ddg_rway ){
		lastddg->DDG_Type = ddg_bottom_corner;
	    }else{
		lastddg->DDG_Type = ddg_both_corners;
	    }
	    if( pred->DG_Clone == NULL ){
		// Create the predecessor
		debug( 2, ( "add predecessor node %s ", pred->DG_Name ) );
		new DDG( pred, GR );
		Initial_DDG( GR, pred );
	    }
	    lastddg->Sideways_From = pred->DG_Clone;
	    if( lastddg->DDG_Type == ddg_bottom_corner ){
		lastddg->Sideways_Link = pred->DG_Clone->Sideways_Link;
		pred->DG_Clone->Sideways_Link = lastddg;
	    }
	    debug( 2, ( "change way point at level %ld to a corner",
		    lastddg->DDG_Level ) );
	}
    }End_Visit;
}// Initial_DDG

static void Insert_Level( P_DDG ddg, P_DG_Graph GR ){
Uint l, p;
P_DDG_Head head;
    l = ddg->DDG_Level;
    head = &(GR->headers[l]);
    p = ++(head->Level_Width);
    ddg->DDG_Position = p;
    if( p > GR->DG_Width_Max ) GR->DG_Width_Max = p;
    debug( 3, ( "insert node %lx at level %d", ddg, l ) );
    insert_list_end( ddg, head->Level_List );
}// Insert_Level

static void Remove_Level( P_DDG ddg, P_DG_Graph GR ){
    Uint l = ddg->DDG_Level;
    for( P_DDG rnode = ddg; rnode != NULL; rnode = next( rnode ) ){
	--(rnode->DDG_Position);
    }
    --(GR->headers[l].Level_Width);
    remove_list( ddg, GR->headers[l].Level_List );
    ddg->DDG_Position = 0;
    if( first( ddg->DDG_Succs ) != NULL ){
	Remove_Level( link( first( ddg->DDG_Succs ) ), GR );
    }
}// Remove_Level

static void Insert_Recurse( P_DDG ddg, P_DG_Graph GR ){
    // First, insert back edges to this node.
    // Later, we will see if we want to insert them to the 'right' of the
    // subtree rooted here.
    if( ddg->DDG_Type == ddg_clone ){
	for( P_DDG Link = ddg->Sideways_Link; Link != NULL; 
		    Link = Link->Sideways_Link ){
	    if( Link->DDG_Type == ddg_top_corner || Link->DDG_Type == ddg_both_corners ){
		if( Link->DDG_Position == 0 ){
		    Insert_Recurse( Link, GR );
		}
	    }
	}
    }
    Insert_Level( ddg, GR );
    // Look at successors.
    // First, see if any successors should be placed to the left of all others
    // Second, place successors that have no other predecessors in order
    // Third, place successors that should be placed to the right of others

    // First stage, nothing, always place in 2nd or 3rd stages

    // Second stage, successors that have no other predecessors anyway,
    // or that have an odd number of predecessors and this is the
    // middle one
    Visit_Ptr_List( succ, ddg->DDG_Succs, DDG ){
	// See how many of them are already placed
	Uint placed = 0;
	Visit_Ptr_List( succpred, succ->DDG_Preds, DDG ){
	    if( succpred->DDG_Position > 0 ) ++placed;
	}End_Visit;
	// Is this the middle predecessor?
	succ->DDG_Extra = placed;
	if( placed*2 == succ->DDG_Pred_Count+1 ){
	    // recurse right away
	    Insert_Recurse( succ, GR );
	}
    }End_Visit;
    // Third stage, successors that have other predecessors 'to the right'
    Visit_Ptr_List( succ, ddg->DDG_Succs, DDG ){
	Uint placed = succ->DDG_Extra;
	if( placed*2 == succ->DDG_Pred_Count ){
	    // recurse after all other successors
	    Insert_Recurse( succ, GR );
	}
    }End_Visit;
    for( P_DDG tlink = ddg->Sideways_Link; tlink != NULL; 
		tlink = tlink->Sideways_Link ){
	if( tlink->DDG_Type == ddg_top_corner || tlink->DDG_Type == ddg_both_corners ){
	    // Find the bottom_corner for this link
	    P_DDG blink;
	    for( blink = tlink; first( blink->DDG_Succs ) != NULL;
		    blink = link( first( blink->DDG_Succs ) ) );
	    // See where it links from
	    P_DDG bnode = blink->Sideways_From;
	    // See how far bnode is from blink, and how far bnode is
	    // from where blink would be if blink were on the right.
	    Uint blinkpos = blink->DDG_Position;
	    Uint bnodepos = bnode->DDG_Position;
	    P_DDG rightnode;
	    for( rightnode = bnode; next( rightnode ) != NULL;
		    rightnode = next( rightnode ) );
	    Uint newpos = rightnode->DDG_Position + 1;
	    // How far is it now?  How far would it be?
	    if( newpos - bnodepos < bnodepos - blinkpos ){
		// Remove the line of nodes from 'tlink' to 'blink'
		// Reinsert them at the end.
		debug( 3, ( "removing nodes %lx to %lx", tlink, blink ) );
		Remove_Level( tlink, GR );
		debug( 3, ( "reinserting nodes %lx to %lx", tlink, blink ) );
		Insert_Recurse( tlink, GR );
	    }
	}
    }
}// Insert_Recurse

static void putn( P_DDG n ){
    if( n == NULL ){
	printf( ".null." );
    }else{
	P_DG p = n->DDG_Clone;
	if( p != NULL ){
	    printf( "%s(%ld)", p->DG_Name, n->DDG_Position );
	}else{
	    printf( "d(%ld.%ld)", n->DDG_Level, n->DDG_Position );
	}
    }
}

void DG_Dump( P_DG_Graph GR, Bool links ){
    for( Uint i = 1; i <= GR->DG_Level_Max; ++i ){
	printf( "Level %3ld:", i );
	Visit_List( ddg, GR->headers[i].Level_List, DDG ){
	    printf( "  " );
	    putn( ddg );
	}End_Visit;
	printf( "\n" );
	if( links ){
	    Visit_List( n, GR->headers[i].Level_List, DDG ){
		Visit_Ptr_List( succ, n->DDG_Succs, DDG ){
		    printf( " " );
		    putn( n );
		    printf( "-->" );
		    putn( succ );
		}End_Visit;
		if( n->Sideways_To != NULL ){
		    printf( " " );
		    putn( n );
		    printf( "==>" );
		    putn( n->Sideways_To );
		}
		if( n->Sideways_From != NULL ){
		    printf( " " );
		    putn( n );
		    printf( "<==" );
		    putn( n->Sideways_From );
		}
	    }End_Visit;
	    printf( "\n\n" );
	}
    }
}// DG_Dump

    
static void Debug_Initial_Levels( P_DG_Graph GR ){
    printf( "Initial assignment of levels:\n" );
    for( Uint i = 1; i <= GR->DG_Level_Max; ++i ){
	printf( "Level %d:", i );
	Visit_List( dg, GR->all_nodes, DG ){
	    if( dg->DG_Level == i ){
		printf( " %s", dg->DG_Name );
	    }
	}End_Visit;
	printf( "\n" );
    }
}

static void Debug_Second_Levels( P_DG_Graph GR ){
    for( Uint i = 1; i <= GR->DG_Level_Max; ++i ){
	if( debuglevel > 1 ){
	    printf( "\nLevel %3ld:", i );
	    Visit_List( ddg, GR->headers[i].Level_List, DDG ){
		printf( "  " );
		putn( ddg );
	    }End_Visit;
	    printf( "\n" );
	}
    }
}

static void Reorder_Edges( P_DG_Graph GR ){
    // Sort the predecessor/successor edges of each node in the order
    // they appear in that level
    // Use a bubble sort?
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	P_Ptr_DDG predptr, prevpredptr, nextpredptr;
	for( predptr = first( ddg->DDG_Preds ); predptr; predptr = nextpredptr ){
	    nextpredptr = next( predptr );
	    prevpredptr = prev( predptr );
	    Sint pos = link( predptr )->DDG_Position;
	    while( prevpredptr && link( prevpredptr )->DDG_Position > pos ){
		// move 'predptr' back until it's column is > column of prev
		remove_list( predptr, ddg->DDG_Preds );
		insert_list_before( predptr, ddg->DDG_Preds, prevpredptr );
		prevpredptr = prev( predptr );
	    }
	}
	P_Ptr_DDG succptr, prevsuccptr, nextsuccptr;
	for( succptr = first( ddg->DDG_Succs ); succptr; succptr = nextsuccptr ){
	    nextsuccptr = next( succptr );
	    prevsuccptr = prev( succptr );
	    Sint pos = link( succptr )->DDG_Position;
	    while( prevsuccptr && link( prevsuccptr )->DDG_Position > pos ){
		// move 'succptr' back until it's column is > column of prev
		remove_list( succptr, ddg->DDG_Succs );
		insert_list_before( succptr, ddg->DDG_Succs, prevsuccptr );
		prevsuccptr = prev( succptr );
	    }
	}
    }End_Visit;
}// Reorder_Edges

// Go through all drawn nodes, look at successors and predecessors
// Decide whether the line between the two nodes should be angled
// left, a la /, or angled right, a la \, or down, a la |.
static void Align_Edges( P_DG_Graph GR ){
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	Uint succnum = 0;
	Uint succtot = ddg->DDG_Succ_Count;
	P_Ptr_DDG lastleft = NULL;
	Visit_List( succptr, ddg->DDG_Succs, Ptr_DDG ){
	    // how close is this successor to the middle?
	    ++succnum;
	    P_Ptr_DG Clone = succptr->DDGL_Clone;
	    DDGL_Alignments CloneAlign = Clone->DGL_Align;
	    if( CloneAlign == ddgl_left ){
		succptr->DDGL_Align = ddgl_left;
	    }else if( CloneAlign == ddgl_right ){
		succptr->DDGL_Align = ddgl_right;
	    }else if( succnum*2 < succtot+1 ){
		succptr->DDGL_Align = ddgl_left;
		lastleft = succptr;
	    }else if( succnum*2 == succtot+1 ){
		// prefer to align above
		succptr->DDGL_Align = ddgl_down;
		lastleft = NULL;
	    }else if( succnum*2 > succtot+1 ){
		succptr->DDGL_Align = ddgl_right;
	    }
	    if( debuglevel > 2 ){
		printf( "align " ); putn( ddg );
		printf( " %s successor ", name(succptr->DDGL_Align) );
		putn( link( succptr ) );
		printf( "\n" );
	    }
	}End_Visit;
	if( lastleft != NULL ){
	    // lastleft is the last 'left' aligned successor.
	    // if it is a link to a dummy node, and the next successor
	    // is not a link to a dummy node, and it is not the first
	    // in the list, make this a 'down' link
	    // if it is not a link to a dummy node, but the next successor
	    // (the first 'right' aligned successor) is a dummy node,
	    // make that one a 'down' link
	    P_DDG lastleftsucc = link( lastleft );
	    P_Ptr_DDG firstright = next( lastleft );
	    P_DDG firstrightsucc = link( firstright );
	    if( lastleftsucc->DDG_Type == ddg_way && firstright != NULL &&
			firstrightsucc->DDG_Type != ddg_way &&
			prev( lastleft ) != NULL ){
		lastleft->DDGL_Align = ddgl_down;
		if( debuglevel > 2 ){
		    printf( "realign " ); putn( ddg );
		    printf( " %s successor ", name(lastleft->DDGL_Align) );
		    putn( lastleftsucc );
		    printf( "\n" );
		}
	    }else if( lastleftsucc->DDG_Type != ddg_way && firstright != NULL &&
			firstrightsucc->DDG_Type == ddg_way &&
			next( firstright ) != NULL ){
		firstright->DDGL_Align = ddgl_down;
		if( debuglevel > 2 ){
		    printf( "realign " ); putn( ddg );
		    printf( " %s successor ", name(firstright->DDGL_Align) );
		    putn( firstrightsucc );
		    printf( "\n" );
		}
	    }
	}
	Uint prednum = 0;
	Uint predtot = ddg->DDG_Pred_Count;
	P_Ptr_DDG lastright = NULL;
	Visit_List( predptr, ddg->DDG_Preds, Ptr_DDG ){
	    // how close is this predecessor to the middle?
	    ++prednum;
	    P_Ptr_DG Clone = predptr->DDGL_Clone;
	    DDGL_Alignments CloneAlign = Clone->DGL_Align;
	    if( CloneAlign == ddgl_left ){
		predptr->DDGL_Align = ddgl_left;
	    }else if( CloneAlign == ddgl_right ){
		predptr->DDGL_Align = ddgl_right;
	    }else if( prednum*2 < predtot+1 ){
		predptr->DDGL_Align = ddgl_right;
		lastright = predptr;
	    }else if( prednum*2 > predtot+1 ){
		predptr->DDGL_Align = ddgl_left;
	    }else if( prednum*2 == predtot+1 ){
		// prefer to align above
		predptr->DDGL_Align = ddgl_down;
		lastright = NULL;
	    }
	    if( debuglevel > 2 ){
		printf( "align predecessor " ); putn( link( predptr) );
		printf( " %s ", name(predptr->DDGL_Align) );
		putn( ddg );
		printf( "\n" );
	    }
	}End_Visit;
	if( lastright != NULL ){
	    // lastright is the last 'right' aligned predecessor.
	    // if it is a link to a dummy node, and the next successor
	    // is not a link to a dummy node, and this is not the first
	    // predecessor, make this a 'down' link
	    // if it is not a link to a dummy node, but the next successor
	    // (the first 'left' aligned successor) is a dummy node,
	    // make that one a 'down' link
	    // make sure the one being realigned is not the end point
	    P_DDG lastrightpred = link( lastright );
	    P_Ptr_DDG firstleft = next( lastright );
	    P_DDG firstleftpred = link( firstleft );
	    if( lastrightpred->DDG_Type == ddg_way && 
			firstleftpred->DDG_Type != ddg_way &&
			prev( lastright ) != NULL ){
		lastright->DDGL_Align = ddgl_down;
		if( debuglevel > 2 ){
		    printf( "realign " ); putn( ddg );
		    printf( " %s predecessor ", name(lastright->DDGL_Align) );
		    putn( lastrightpred );
		    printf( "\n" );
		}
	    }else if( lastrightpred->DDG_Type != ddg_way && 
			firstleftpred->DDG_Type == ddg_way &&
			next( firstleft ) != NULL ){
		firstleft->DDGL_Align = ddgl_down;
		if( debuglevel > 2 ){
		    printf( "realign " ); putn( ddg );
		    printf( " %s predecessor ", name(firstleft->DDGL_Align) );
		    putn( firstleftpred );
		    printf( "\n" );
		}
	    }
	}
    }End_Visit;
}// Align_Edges


static void Count_Alignments( P_DG_Graph GR ){
    Uint Alignments[5][5];
    for( DDGL_Alignments a1 = ddgl_none; a1 < ddgl_last; a1 = next( a1 ) ){
	for( DDGL_Alignments a2 = ddgl_none; a2 < ddgl_last; a2 = next ( a2 ) ){
	    Alignments[a1][a2] = 0;
	}
    }
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	// count how many alignment agreements, conflicts
	Visit_List( succptr, ddg->DDG_Succs, Ptr_DDG ){
	    P_Ptr_DDG predptr = succptr->DDGL_Match;
	    ++(Alignments[succptr->DDGL_Align][predptr->DDGL_Align]);
	}End_Visit;
    }End_Visit;
    printf( "Alignment agreements:\n" );
    for( DDGL_Alignments b1 = ddgl_left; b1 <= ddgl_right; b1 = next( b1 ) ){
	printf( "%5.5s %5ld\n", name( b1 ), Alignments[b1][b1] );
    }
    printf( "Alignment conflicts:\n" );
    printf( "%5.5s", " " );
    for( DDGL_Alignments c0 = ddgl_left; c0 <= ddgl_right; c0 = next( c0 ) ){
	printf( " %5.5s", name( c0 ) );
    }
    printf( "\n" );
    for( DDGL_Alignments c1 = ddgl_left; c1 <= ddgl_right; c1 = next( c1 ) ){
	printf( "%5.5s", name( c1 ) );
	for( DDGL_Alignments c2 = ddgl_left; c2 <= ddgl_right; c2 = next( c2 ) ){
	    if( c1 == c2 ){
		printf( " %5.5s", " " );
	    }else{
		printf( " %5ld", Alignments[c1][c2] );
	    }
	}
	printf( "\n" );
    }
}// Count_Alignments

// Change down/left, left/down conflicts to left/left
// Change right/down, down/right conflicts to right/right.
// Change left/right, right/left conflicts to conflict/conflict
static void Fix_Alignment_Conflicts( P_DG_Graph GR ){
    DDGL_Alignments NewAlignment[4][4];
    NewAlignment[ddgl_left][ddgl_left] = ddgl_left;
    NewAlignment[ddgl_left][ddgl_down] = ddgl_left;
    NewAlignment[ddgl_left][ddgl_right] = ddgl_conflict;
    NewAlignment[ddgl_down][ddgl_left] = ddgl_left;
    NewAlignment[ddgl_down][ddgl_down] = ddgl_down;
    NewAlignment[ddgl_down][ddgl_right] = ddgl_right;
    NewAlignment[ddgl_right][ddgl_left] = ddgl_conflict;
    NewAlignment[ddgl_right][ddgl_down] = ddgl_right;
    NewAlignment[ddgl_right][ddgl_right] = ddgl_right;
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	// count how many alignment agreements, conflicts
	Visit_List( succptr, ddg->DDG_Succs, Ptr_DDG ){
	    P_Ptr_DDG predptr = succptr->DDGL_Match;
	    predptr->DDGL_Align = succptr->DDGL_Align = 
		NewAlignment[predptr->DDGL_Align][succptr->DDGL_Align];
	}End_Visit;
    }End_Visit;
}// Fix_Alignment_Conflicts


// See if any node has a left edge after a down or right edge (consistency)
// See if any node has a left edge after a conflict edge
// See if any node has a down edge after a right edge (consistency)
// See if any node has >1 down edge (consistency)
// See if any node has a conflict edge after a right edge
static void Find_More_Conflicts( P_DG_Graph GR ){
    Uint nconflictleft = 0;
    Uint nrightconflict = 0;
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	Uint nleft = 0, nright = 0, ndown = 0, nconflict = 0;
	Visit_List( succptr, ddg->DDG_Succs, Ptr_DDG ){
	    switch( succptr->DDGL_Align ){
	    case ddgl_left:
		if( ndown > 0 ){
		    printf( "**node %lx with LEFT successor edge after DOWN edge\n", ddg );
		}
		if( nright > 0 ){
		    printf( "**node %lx with LEFT successor edge after RIGHT edge\n", ddg );
		}
		if( nconflict > 0 ) ++nconflictleft;
		++nleft;
		break;
	    case ddgl_down:
		if( ndown > 0 ){
		    printf( "**node %lx with >1 DOWN successor edge\n", ddg );
		}
		if( nright > 0 ){
		    printf( "**node %lx with DOWN successor edge after RIGHT edge\n", ddg );
		}
		++ndown;
		break;
	    case ddgl_right:
		++nright;
		break;
	    case ddgl_conflict:
		++nconflict;
		if( nright > 0 ) ++nrightconflict;
		break;
	    }
	}End_Visit;
    }End_Visit;
    if( debuglevel ){
	printf( "%5d cases with conflict edge before a left edge\n", nconflictleft );
	printf( "%5d cases with conflict edge after a right edge\n", nrightconflict );
    }
}// Find_More_Conflicts

static void Count_Connections( P_DG_Graph GR ){
    // Count the 'left' and 'right' connections from each node
    // A 'left' connection is a successor edge labeled 'left'
    //  or a predecessor edge labeled 'right', 
    // and vice versa for 'right' connection
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	ddg->DDG_Connect_Left = 0;
	ddg->DDG_Connect_Right = 0;
	Visit_List( succptr, ddg->DDG_Succs, Ptr_DDG ){
	    switch( succptr->DDGL_Align ){
	    case ddgl_left:
		++(ddg->DDG_Connect_Left);
		break;
	    case ddgl_right:
		++(ddg->DDG_Connect_Right);
		break;
	    }
	}End_Visit;
	Visit_List( predptr, ddg->DDG_Preds, Ptr_DDG ){
	    switch( predptr->DDGL_Align ){
	    case ddgl_left:
		++(ddg->DDG_Connect_Right);
		break;
	    case ddgl_right:
		++(ddg->DDG_Connect_Left);
		break;
	    }
	}End_Visit;
	if( debuglevel > 2 ){
	    printf( "node "); putn( ddg );
	    printf( " has %d left and %d right connections\n",
		ddg->DDG_Connect_Left, ddg->DDG_Connect_Right );
	}
    }End_Visit;
}// Count_Connections

inline Sint DGwidth( P_DDG ddg ){
    if( ddg->DDG_Type == ddg_clone && ddg->DDG_Clone != NULL ){
	return ddg->DDG_Clone->DG_Width;
    }
    return 1;
}// DGwidth

static void Find_String_Heads( P_DG_Graph GR ){
    // go through each level.
    // if there is a single predecessor, and that predecessor has a single
    // successor, then these two nodes form part of a 'string'
    // set string head of this node to the string head of the predecessor
    // Otherwise, set string head of this node to itself.
    for( Uint i = 1; i <= GR->DG_Level_Max; ++i ){
	Visit_List( ddg, GR->headers[i].Level_List, DDG ){
	    ddg->DDG_StringHead = ddg; // by default
	    ddg->DDG_StringWidth = DGwidth( ddg ); // by default
	    P_Ptr_DDG predlist = first( ddg->DDG_Preds );
	    if( predlist != NULL && next( predlist ) == NULL &&
			predlist->DDGL_Align == ddgl_down ){
		// one predecessor
		P_DDG pred = link( predlist );
		P_Ptr_DDG succlist = first( pred->DDG_Succs );
		if( succlist != NULL && next( succlist ) == NULL &&
				succlist->DDGL_Align == ddgl_down ){
		    // predecessor has one successor, this one
		    P_DDG stringhead = pred->DDG_StringHead;
		    ddg->DDG_StringHead = stringhead;
		    // add this node's connections to string heads
		    stringhead->DDG_Connect_Left += ddg->DDG_Connect_Left;
		    stringhead->DDG_Connect_Right += ddg->DDG_Connect_Right;
		    if( ddg->DDG_StringWidth > stringhead->DDG_StringWidth ){
			stringhead->DDG_StringWidth = ddg->DDG_StringWidth;
		    }
		}
	    }
	}End_Visit;
    }
}// Find_String_Heads

static P_DDG nextstring( P_DDG ddg ){
    P_Ptr_DDG succlist = first( ddg->DDG_Succs );
    if( succlist != NULL && next( succlist ) == NULL &&
		    succlist->DDGL_Align == ddgl_down ){
	P_DDG succ = link( succlist );
	P_Ptr_DDG predlist = first( succ->DDG_Preds );
	if( predlist != NULL && next( predlist ) == NULL &&
		    predlist->DDGL_Align == ddgl_down ){
	    // only successor has one predecessor, this one
	    return succ;
	}
    }
    return NULL;
}// nextstring

// return '-1' if 'ddg' is not a string head, or
// if any nodes in the string have some node to the 'left' (the previous node)
// not yet placed.
// Else, return the column number of the rightmost neighbor to the left of
// this string.
static Sint left_placed( P_DDG ddg ){
    debug( 3, ( "node %lx has string head at %lx", ddg, ddg->DDG_StringHead ) );
    if( ddg->DDG_StringHead != ddg ) return -1;
    Sint neighbor = 0;
    for( P_DDG string = ddg; string != NULL; string = nextstring( string ) ){
	P_DDG left = prev( string );
	debug( 3, ( "string at %lx node %lx has left neighbor at %lx", ddg, string, left ) );
	if( left != NULL ){
	    debug( 3, ( "left neighbor is at column %d", left->DDG_Position ) );
	    if( left->DDG_Position == 0 ) return -1;
	    Sint w = DGwidth( left );
	    if( left->DDG_Position+w > neighbor ) neighbor = left->DDG_Position+w;
	}
    }
    debug( 3, ( "returning %d", neighbor ) );
    return neighbor;
}// left_placed

static Bool ready_to_be_placed( P_DDG ddg ){
    // is a string head, not placed yet, 
    // targets of all 'left' edges done
    if( ddg->DDG_StringHead == ddg && ddg->DDG_Position == 0 && ddg->DDG_Extra == 0 ){
	return 1;
    }
    return 0;
}// ready_to_be_placed

static void Check_Place_Left( P_DDG ddg, Sint column ){
    if( ddg->DDG_StringHead != ddg ) return;
    if( column > ddg->DDG_XPosition ) ddg->DDG_XPosition = column;
}

Uint Placed_Left;
static void Place_Left( P_DG_Graph GR, P_DDG ddg ){
    if( ddg->DDG_StringHead != ddg ) return;
    // put this node to the right of the given column
    // Check that the 'left' neighbors are all done, too
    Sint column = ddg->DDG_XPosition;
    if( debuglevel ){
	printf( "string head " );
	putn( ddg );
	printf( " must be at least in column %ld\n", column );
    }
    Sint c = left_placed( ddg );
    debug( 2, ( "left neighbor in column %d", c ) );
    if( c < 0 ) return;
    if( column < c ) column = c;
    // Find maximum width of the string
    Sint width = ddg->DDG_StringWidth;
    if( debuglevel ){
	printf( "place string head " );
	putn( ddg );
	printf( " in column %ld with width %ld\n", column+width, width );
    }
    column = column + width;
    if( column > GR->DG_Width_Max ) GR->DG_Width_Max = column;
    for( P_DDG stringw = ddg; stringw != NULL; stringw = nextstring( stringw ) ){
	stringw->DDG_Position = column;
	++Placed_Left;
    }
    // Look at level neighbors to the right of the string, and see if they need placing
    for( P_DDG stringx = ddg; stringx != NULL; stringx = nextstring( stringx ) ){
	P_DDG n = next( stringx );
	if( n != NULL ){
	    P_DDG sh = n->DDG_StringHead;
	    if( debuglevel > 2 ){
		printf( "neighbor from " );
		putn( ddg );
		printf( " to " );
		putn( sh );
		printf( " at column %ld\n", column+width );
	    }
	    Check_Place_Left( sh, column + width );
	    if( ready_to_be_placed( sh ) ){
		if( debuglevel ){
		    printf( "place right neighbor " );
		    putn( sh );
		    printf( " at column %ld\n", column+width );
		}
		Place_Left( GR, sh );
	    }
	}
    }
    // Look at other predecessors of string head, other successors of string tail
    Visit_List( predptr, ddg->DDG_Preds, Ptr_DDG ){
	P_DDG pred = link( predptr );
	P_DDG sh = pred->DDG_StringHead;
	if( predptr->DDGL_Align == ddgl_left ){
	    --(sh->DDG_Extra);
	    if( debuglevel > 2 ){
		 printf( "predecessor of " );
		 putn( ddg );
		 printf( " is " );
		 putn( sh );
		 printf( " at column %ld\n", column );
	    }
	    Check_Place_Left( sh, column );
	}
	if( ready_to_be_placed( sh ) ){
	    if( debuglevel ){
		printf( "place predecessor " );
		putn( sh );
		printf( "\n" );
	    }
	    Place_Left( GR, sh );
	}
    }End_Visit;
    P_DDG stringtail = ddg;
    for( P_DDG stringy = ddg; stringy != NULL; stringy = nextstring( stringy ) ){
	stringtail = stringy;
    }
    Visit_List( succptr, stringtail->DDG_Succs, Ptr_DDG ){
	P_DDG succ = link( succptr );
	P_DDG sh = succ->DDG_StringHead;
	if( succptr->DDGL_Align == ddgl_right ){
	    --(sh->DDG_Extra);
	    Check_Place_Left( sh, column );
	    if( debuglevel > 2 ){
		printf( "successor of " );
		putn( ddg );
		printf( " is " );
		putn( sh );
		printf( " at column %ld\n", column );
	    }
	}
	if( ready_to_be_placed( sh ) ){
	    if( debuglevel ){
		printf( "place successor " );
		putn( sh );
		printf( "\n" );
	    }
	    Place_Left( GR, sh );
	}
    }End_Visit;
}// Place_Left

void Left_To_Right( P_DG_Graph GR ){
    // Initialize
    Visit_Ptr_List( init, GR->all_DDG, DDG ){
	init->DDG_Extra = init->DDG_Connect_Left;
	init->DDG_Position = 0;
	init->DDG_XPosition = 0;
    }End_Visit;

    // Visit leftmost node at each level.
    // See if it is a string head and is ready to be placed
    Placed_Left = 0;
    for( Uint i = 1; i <= GR->DG_Level_Max; ++i ){
	P_DDG ddg = first( GR->headers[i].Level_List );

	if( ddg->DDG_StringHead == ddg && ddg->DDG_Position == 0 && 
		ddg->DDG_Extra == 0 ){
	    // start this at first column
	    Check_Place_Left( ddg, 1 );
	    Place_Left( GR, ddg );
	}
    }
    if( debuglevel ) printf( "%5d positions placed\n", Placed_Left );

    // Visit each node; if a string head, see if is is ready to be placed.
//    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
//	if( ddg->DDG_Position == 0 && ddg->DDG_Extra == 0 && left_placed( ddg ) ){
//	    // ready to be placed, put 
//	}
//    }End_Visit;
}// Left_To_Right

static inline Sint stringwidth( P_DDG ddg ){
    return ddg->DDG_StringHead->DDG_StringWidth;
}

// doright == 1 means move to the right
// doright == -1 means move to the left
static Sint neighbors_done( P_DDG ddg, Bool doright ){
    P_DDG head = ddg->DDG_StringHead;
    for( P_DDG string = head; string != NULL; string = nextstring( string ) ){
	P_DDG neighbor;
	if( doright ){
	    neighbor = next( string );
	}else{
	    neighbor = prev( string );
	}
	if( neighbor != NULL ){
	    if( neighbor->DDG_XPosition == 0 ){
		return 0;
	    }
	}
    }
    return 1;
}// neighbors_done

static void Move( P_DG_Graph GR, P_DDG ddg, Bool doright  );

static void Check_Move( P_DG_Graph GR, P_DDG ddg, Bool doright ){
    P_DDG head = ddg->DDG_StringHead;
    --(head->DDG_Extra);
    if( head->DDG_Extra == 0 ){ // && neighbors_done( head, doright ) ){
	Move( GR, head, doright );
    }
}// Check_Move

static void Check_Neighbor( P_DG_Graph GR, P_DDG ddg, Bool doright ){
    P_DDG head = ddg->DDG_StringHead;
    if( head->DDG_Extra == 0 ){ // && neighbors_done( head, doright ) ){
	Move( GR, head, doright );
    }
}// Check_Neighbor

static void Move( P_DG_Graph GR, P_DDG ddg, Bool doright ){
    // Go through predecessors, successors, see how far to the 'right' or 'left'
    // this node can be moved.
    Sint most;
    DDGL_Alignments succedgealign, prededgealign;
    if( doright ){
	most = GR->DG_Width_Max + 99;
	succedgealign = ddgl_right;
	prededgealign = ddgl_left;
    }else{
	most = 0;
	succedgealign = ddgl_left;
	prededgealign = ddgl_right;
    }
    for( P_DDG str = ddg; str != NULL; str = nextstring( str ) ){
	Visit_List( succedge, str->DDG_Succs, Ptr_DDG ){
	    if( succedge->DDGL_Align == succedgealign ){
		P_DDG succ = link( succedge );
		Sint succpos = succ->DDG_Position - doright*stringwidth( succ );
		if( doright*succpos < doright*most ) most = succpos;
	    }
	}End_Visit;
	Visit_List( prededge, str->DDG_Preds, Ptr_DDG ){
	    if( prededge->DDGL_Align == prededgealign ){
		P_DDG pred = link( prededge );
		Sint predpos = pred->DDG_Position - doright*stringwidth( pred );
		if( doright*predpos < doright*most ) most = predpos;
	    }
	}End_Visit;
	P_DDG nxt;
	if( doright == 1 ){
	    nxt = next( str );
	}else{
	    nxt = prev( str );
	}
	if( nxt ){
	    Sint nxtpos = nxt->DDG_Position - doright*DGwidth( nxt );
	    if( doright*nxtpos < doright*most ) most = nxtpos;
	}
    }
    Sint wid = stringwidth( ddg );
    Sint newpos = most - doright*wid;
    if( newpos > GR->DG_Width_Max ) newpos = GR->DG_Width_Max;
    if( newpos < 2 ) newpos = 2;
    if( doright*newpos > doright*ddg->DDG_Position ){
	for( P_DDG string = ddg; string != NULL; string = nextstring( string ) ){
	    string->DDG_Position = newpos;
	}
    }
    for( P_DDG s = ddg; s != NULL; s = nextstring( s ) ){
	s->DDG_XPosition = s->DDG_Position;// mark as placed
    }
    for( P_DDG string = ddg; string != NULL; string = nextstring( string ) ){
	Visit_List( succedge1, string->DDG_Succs, Ptr_DDG ){
	    if( succedge1->DDGL_Align == prededgealign ){
		Check_Move( GR, link( succedge1 ), doright );
	    }
	}End_Visit;
	Visit_List( prededge1, string->DDG_Preds, Ptr_DDG ){
	    if( prededge1->DDGL_Align == succedgealign ){
		Check_Move( GR, link( prededge1 ), doright );
	    }
	}End_Visit;
	P_DDG nxt;
	if( doright == 1 ){
	    nxt = prev( string );
	}else{
	    nxt = next( string );
	}
	if( nxt ){
	    Check_Move( GR, nxt, doright );
	}
    }
}// Move

void Adjust( P_DG_Graph GR, Bool doright ){
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( doright ){
	    ddg->DDG_Extra = ddg->DDG_Connect_Right;
	}else{
	    ddg->DDG_Extra = ddg->DDG_Connect_Left;
	}
	ddg->DDG_XPosition = 0;
	if( ddg->DDG_StringHead == ddg ){
	    // a string header
	    // collect the number of neighbors of all nodes in the string
	    for( P_DDG string = ddg; string != NULL; string = nextstring( string ) ){
		P_DDG neighbor;
		if( doright == 1 ){
		    neighbor = next( string );
		}else{
		    neighbor = prev( string );
		}
		if( neighbor != NULL ){
		    ++(ddg->DDG_Extra);
		}
	    }
	}
    }End_Visit;
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	// Look for a string head with no more unplaced nodes to the right.
	// Move the string as far to the right as possible.
	if( ddg->DDG_Extra == 0 && ddg->DDG_StringHead == ddg ){
	    Move( GR, ddg, doright );
	}
    }End_Visit;
}// Adjust


static Bool TriviallyMovable( P_DDG ddg, Sint& MoveTo ){
    // See if ddg has any predecessors to the left.
    Visit_Ptr_List( pred, ddg->DDG_Preds, DDG ){
	if( pred->DDG_Position < ddg->DDG_Position ){
	    // no reason to move this string to the right, 
	    // edges to the left anyway.
	    return 0;
	}
    }End_Visit;

    P_DDG stringtail;
    // See where the string has another neighbor to the right
    for( P_DDG string = ddg; string != NULL; string = nextstring( string ) ){
	stringtail = string;
	P_DDG n = next( string );
	if( n != NULL ){
	    Sint np = n->DDG_Position - stringwidth( n ) - stringwidth( ddg );
	    if( np < MoveTo ) MoveTo = np;
	}
	// look at predecessors, successors that might be to the right
	Visit_List( succedge, string->DDG_Succs, Ptr_DDG ){
	    if( succedge->DDGL_Align == ddgl_right ){
		P_DDG n = link( succedge );
		Sint np = n->DDG_Position - 1;
		if( np < MoveTo ) MoveTo = np;
	    }
	}End_Visit;
	Visit_List( prededge, string->DDG_Preds, Ptr_DDG ){
	    if( prededge->DDGL_Align == ddgl_left ){
		P_DDG n = link( prededge );
		Sint np = n->DDG_Position - 1;
		if( np < MoveTo ) MoveTo = np;
	    }
	}End_Visit;
    }
    // See if the string has any successors to the left.
    Visit_Ptr_List( succ, stringtail->DDG_Succs, DDG ){
	if( succ->DDG_Position < ddg->DDG_Position ){
	    // no reason to move this string to the right, 
	    // edges to the left anyway.
	    return 0;
	}
    }End_Visit;
    return 1;
}// TriviallyMovable

static Uint count_edges;
static Uint count_left_edges;
static Uint count_right_edges;
static Uint count_long_left_edges;
static Uint count_strings_moved;

static void TestMove( P_DG_Graph GR, P_DDG ddg, P_DDG source, DDGL_Alignments align ){
    // See if the string at this node is trivially movable to the right
    Sint MoveTo = source->DDG_Position;
    if( align == ddgl_left ) MoveTo -= stringwidth( ddg );
    if( TriviallyMovable( ddg, MoveTo ) ){
	if( MoveTo > ddg->DDG_Position ){
	    // Move it over!
	    ++count_strings_moved;
	    for( P_DDG string = ddg; string != NULL;
		    string = nextstring( string ) ){
		string->DDG_Position = MoveTo;
	    }
	}
    }
}// TestMove

static void TryToMove( P_DG_Graph GR, P_DDG ddg, P_DDG source, DDGL_Alignments align ){
    // See if this points to the right or left
    Sint diff = ddg->DDG_Position - source->DDG_Position;
    if( diff > 0 ){
	++count_right_edges;
	// successor is 'to right' of ddg
    }else if( diff < 0 ){
	++count_left_edges;
	// successor is 'to left' of ddg
	if( align == ddgl_left && diff < -2 || diff < 0 ){
	    ++count_long_left_edges;
	    // anything is a long edge
	    if( ddg->DDG_StringHead == ddg ){
		TestMove( GR, ddg, source, align );
	    }
	}
    }
}// TryToMove

static P_DDG corner_to( P_DDG corner, P_DDG ddg ){
    if( corner != NULL &&
	    ( corner->DDG_Type == ddg_top_corner || 
	      corner->DDG_Type == ddg_both_corners ) 
	    && corner->Sideways_To == ddg ){
	// found a back edge to 'ddg' here
	return corner;
    }
    return NULL;
}// corner_to

void Correct( P_DG_Graph GR ){
    count_edges = 0;
    count_left_edges = 0;
    count_right_edges = 0;
    count_long_left_edges = 0;
    count_strings_moved = 0;
    // go through levels
    for( Uint i = 1; i <= GR->DG_Level_Max; ++i ){
	// go through all nodes at that level
	Visit_List_Backwards( ddg, GR->headers[i].Level_List, DDG ){
	    // get next string to the 'left'
	    P_DDG neighbor = corner_to( prev( ddg ), ddg );
	    // go through successors backwards
	    Visit_List_Backwards( succedge, ddg->DDG_Succs, Ptr_DDG ){
		P_DDG succ = link( succedge );
		++count_edges;
		if( neighbor != NULL && 
			succ->DDG_Position < neighbor->DDG_Position ){
		    // see about moving the back edge here
		    TryToMove( GR, neighbor, ddg, ddgl_left );
		    neighbor = corner_to( prev( neighbor ), ddg );
		}
		TryToMove( GR, succ, ddg, succedge->DDGL_Align );
	    }End_Visit;
	    for( neighbor = neighbor; neighbor != NULL; 
			neighbor = corner_to( prev( neighbor ), ddg ) ){
		TryToMove( GR, neighbor, ddg, ddgl_left );
	    }
	}End_Visit;
    }
    if( debuglevel ){
	printf( "%5ld edges\n", count_edges );
	printf( "%5ld right edges\n", count_right_edges );
	printf( "%5ld left edges\n", count_left_edges );
	printf( "%5ld long left edges\n", count_long_left_edges );
	printf( "%5ld moves made\n", count_strings_moved );
    }
}

void DG_Process_Simple( P_DG_Graph GR ){
    // Free up all old 'display nodes'
    Free_Old_Display_Nodes( GR );
    // Assign initial levels to each node
    Assign_Levels( GR );
    // Sink each node down as far as it can go (to shorten path lengths)
    Sink_Down( GR );
    // Float each node up as far as it can go (to put as much near the top as possible)
    Float_Up( GR );

    if( debuglevel ) Debug_Initial_Levels( GR );

    // set up level header nodes
    if( GR->DG_Level_Max != GR->DG_Old_Level_Max || GR->headers == NULL ){
	GR->headers = new DDG_Head[GR->DG_Level_Max + 1];
    }
    memset( GR->headers, 0, sizeof(DDG_Head)*(GR->DG_Level_Max+1) );
    Uint Count = 0;
    Visit_List( dg, GR->all_nodes, DG ){
	++Count;
    }End_Visit;
    debug( 2, ( " now creating drawn nodes for %ld real nodes", Count ) );
    if( Count == 0 ) return;

    // Now, find all nodes with no predecessors, 
    // do a depth first traversal to initially
    // place each node in the graph.  Place each node just to the
    // right of the previously placed node at this level.
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show && dg->DG_Clone == NULL ){
	    new DDG( dg, GR );
	    Initial_DDG( GR, dg );
	}
    }End_Visit;
    Uint CountAll = 0;
    Uint CountAllEdges = 0;
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	++CountAll;
	Visit_Ptr_List( preds, ddg->DDG_Preds, DDG ){
	    ++CountAllEdges;
	}End_Visit;
    }End_Visit;

    // Now, put nodes in correct left-to-right order in each level
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( ddg->DDG_Pred_Count == 0 && ddg->DDG_Position == 0 ){
	    Insert_Recurse( ddg, GR );
	}
    }End_Visit;
    
    if( debuglevel > 1 ) Debug_Second_Levels( GR );

    if( debuglevel ){
	printf( "%5ld nodes\n", Count );
	printf( "%5ld levels\n", GR->DG_Level_Max );
	printf( "%5ld columns\n", GR->DG_Width_Max );
	printf( "%5ld total points placed\n", CountAll );
	printf( "%5ld total edges placed\n", CountAllEdges );
    }
    if( debuglevel > 1 ) DG_Dump( GR, 1 );

    Reorder_Edges( GR );
    Align_Edges( GR );
    if( debuglevel ) Count_Alignments( GR );
    Fix_Alignment_Conflicts( GR );
    Find_More_Conflicts( GR );
    Count_Connections( GR );
    Find_String_Heads( GR );
    Left_To_Right( GR );
    Correct( GR );
    if( debuglevel > 1 ) DG_Dump( GR, 1 );
    if( Placed_Left != CountAll ) printf( "only %d points placed out of %d\n",
	Placed_Left, CountAll );
    if( debuglevel ) printf( "%5ld final width\n", GR->DG_Width_Max );
    
}// DG_Process_Simple

static Sint channel_stack[15];
static Uint channel_free;
static Sint channel_max;

static void channel_init(){
    for( Uint i = 0; i < 15; ++i ) channel_stack[i] = i+1;
    channel_free = 0;
    channel_max = 0;
}

static Sint channel_get(){
    Sint r = channel_stack[channel_free++];
    if( r > channel_max ){
	channel_max = r;
    }
    return r;
}

static void channel_return( Sint c ){
    channel_stack[--channel_free] = c;
}

void DG_Assign_Channels( P_DG_Graph GR ){
    for( Uint h = 1; h <= GR->DG_Level_Max; ++h ){
	P_DDG_Head head = GR->headers+h;
        // plot level 'head'
	Uint any_top_corners = 0;
	Uint any_bottom_corners = 0;
	Visit_List( dg, head->Level_List, DDG ){
	    if( dg->DDG_Show ){
		if( dg->DDG_Type == ddg_top_corner ) ++any_top_corners;
		if( dg->DDG_Type == ddg_bottom_corner ) ++any_bottom_corners;
		if( dg->DDG_Type == ddg_both_corners ){
		    ++any_top_corners;
		    ++any_bottom_corners;
		}
	    }
	}End_Visit;
	// look at corners, cross links
	if( any_top_corners ){
	    channel_init();
	    Visit_List( dgx, head->Level_List, DDG ){
		if( dgx->DDG_Show ){
		    if( dgx->DDG_Type == ddg_top_corner || dgx->DDG_Type == ddg_both_corners ){
			// a corner to another node
			// see if we are starting or ending here
			P_DDG tolink = dgx->Sideways_To;
			if( tolink->DDG_Position > dgx->DDG_Position ){
			    dgx->DDG_Top_Channel = channel_get();
			}else{
			    channel_return( dgx->DDG_Top_Channel );
			}
		    }else if( dgx->DDG_Type == ddg_clone && dgx->Sideways_Link ){
			// go through all links to this one
			for( P_DDG frlink = dgx->Sideways_Link; frlink;
				frlink = frlink->Sideways_Link ){
			    if( frlink->DDG_Type == ddg_top_corner || frlink->DDG_Type == ddg_both_corners ){
				if( frlink->DDG_Position > dgx->DDG_Position ){
				    frlink->DDG_Top_Channel = channel_get();
				}else{
				    channel_return( frlink->DDG_Top_Channel );
				}
			    }
			}
		    }
		}
	    }End_Visit;
	    Visit_List( dgy, head->Level_List, DDG ){
		if( dgy->DDG_Show ){
		    if( dgy->DDG_Type == ddg_top_corner || dgy->DDG_Type == ddg_both_corners ){
			dgy->DDG_Top_Channel = channel_max - dgy->DDG_Top_Channel;
		    }
		}
	    }End_Visit;
	}
	if( any_bottom_corners ){
	    channel_init();
	    Visit_List( dg, head->Level_List, DDG ){
		if( dg->DDG_Show ){
		    if( dg->DDG_Type == ddg_bottom_corner || dg->DDG_Type == ddg_both_corners ){
			// a corner to another node
			// see if we are starting or ending here
			P_DDG fromlink = dg->Sideways_From;
			if( fromlink->DDG_Position > dg->DDG_Position ){
			    dg->DDG_Bot_Channel = channel_get();
			}else{
			    channel_return( dg->DDG_Bot_Channel );
			}
		    }else if( dg->DDG_Type == ddg_clone && dg->Sideways_Link ){
			// go through all links to this one
			for( P_DDG tolink = dg->Sideways_Link; tolink;
				tolink = tolink->Sideways_Link ){
			    if( tolink->DDG_Type == ddg_bottom_corner || tolink->DDG_Type == ddg_both_corners ){
				if( tolink->DDG_Position > dg->DDG_Position ){
				    tolink->DDG_Bot_Channel = channel_get();
				}else{
				    channel_return( tolink->DDG_Bot_Channel );
				}
			    }
			}
		    }
		}
	    }End_Visit;
	    Visit_List( dgy, head->Level_List, DDG ){
		if( dgy->DDG_Show ){
		    if( dgy->DDG_Type == ddg_bottom_corner || dgy->DDG_Type == ddg_both_corners ){
			dgy->DDG_Bot_Channel = channel_max - dgy->DDG_Bot_Channel;
		    }
		}
	    }End_Visit;
	}
    }
}// DG_Assign_Channels



static void Insert_Tree( P_DG_Graph GR, P_DDG ddg, 
	    Sint position, Sint offset, Bool PTree ){
    // Is there room at this level?
    Sint l = ddg->DDG_Level;
    P_DDG_Head head = &(GR->headers[l]);
    Sint p = head->Level_Width;
    Sint w = ddg->DDG_Clone->DG_Width;
    debug( 1, ( "insert node %s at level %ld around position %ld (level width %ld, node width %ld)",
	ddg->DDG_Clone->DG_Name, l, position, p, w ) );
    if( p <= 0 ) p = 1;
    if( p >= position - w ){
	position = p + w;
    }
    Sint leftchild = 0;
    Sint leftposition;
    // Is there a 'left' child?
    DDGL_Alignments lookfor;
    List_Ptr_DDG list;
    if( !PTree ){
	lookfor = ddgl_left;
	list.Init( ddg->DDG_Succs );
    }else{
	lookfor = ddgl_right;
	list.Init( ddg->DDG_Preds );
    }
    Visit_List( linkptr, list, Ptr_DDG ){
	// Look for a 'left' child, which should be to the left of this node
	if( linkptr->DDGL_Align == lookfor ){
	    P_DDG child = link( linkptr );
	    debug( 1, ( " left link from %s to %s", ddg->DDG_Clone->DG_Name, child->DDG_Clone->DG_Name ) );
	    Insert_Tree( GR, child, position-1, 1, PTree );
	    leftposition = child->DDG_Position;
	    ++leftchild;
	}
    }End_Visit;
    if( leftchild && leftposition >= position ) position = leftposition+1;
    // Now, look for any unaligned children
    Sint nonechild = 0;
    Sint noneposition;
    Sint downchild = 0;
    Sint downposition;
    Visit_List( linkptr, list, Ptr_DDG ){
	if( linkptr->DDGL_Align == ddgl_none || linkptr->DDGL_Align == ddgl_down ){
	    P_DDG child = link( linkptr );
	    debug( 1, ( " unaligned link from %s to %s", ddg->DDG_Clone->DG_Name, child->DDG_Clone->DG_Name ) );
	    Insert_Tree( GR, child, position, 0, PTree );
	    noneposition = child->DDG_Position;
	    ++nonechild;
	    if( linkptr->DDGL_Align == ddgl_down ){
		downposition = child->DDG_Position;
		++downchild;
	    }
	}
    }End_Visit;
    if( downchild ){
	if( downposition > position ) position = downposition;
    }else if( nonechild == 1 ){
	// just one, put this node just below it
	if( noneposition > position ) position = noneposition;
    }
    // Now, look for any 'right' children
    if( !PTree ){
	lookfor = ddgl_right;
    }else{
	lookfor = ddgl_left;
    }
    Sint rightchild = 0;
    Sint rightposition;
    Visit_List( linkptr, list, Ptr_DDG ){
	// Look for a 'right' child, which should be to the right of this node
	if( linkptr->DDGL_Align == lookfor ){
	    P_DDG child = link( linkptr );
	    debug( 1, ( "right link from %s to %s", ddg->DDG_Clone->DG_Name, child->DDG_Clone->DG_Name ) );
	    Insert_Tree( GR, child, position+1, 0, PTree );
	    rightposition = child->DDG_Position;
	    ++rightchild;
	}
    }End_Visit;
    if( leftchild && rightchild ){
	position = (leftposition + rightposition + offset)/2;
    }else if( leftchild ){
	position = (leftposition+1);
    }else if( rightchild ){
	position = (rightposition-1);
    }else if( nonechild == 1 ){
	position = noneposition;
    }
    p = head->Level_Width;
    if( p + w >= position ){
	position = p + w;
    }
    ddg->DDG_Position = position;
    head->Level_Width = position + w;
    if( position > GR->DG_Width_Max ) GR->DG_Width_Max = position;
    debug( 1, ( "really insert node %s at level %ld position %ld", ddg->DDG_Clone->DG_Name, l, position ) );
    insert_list_end( ddg, head->Level_List );
}// Insert_Tree

static Bool DG_Process_Tree( P_DG_Graph GR, Bool PTree ){
    // Free up all old 'display nodes'
    Free_Old_Display_Nodes( GR );
    // Assign initial levels to each node
    Assign_Levels( GR );
    // Sink each node down as far as it can go (to shorten path lengths)
    Sink_Down( GR, PTree );
    if( !PTree ){
	// Float each node up as far as it can go (to put as much near the top as possible)
	Float_Up( GR );
    }
    if( debuglevel ) Debug_Initial_Levels( GR );

    // set up level header nodes
    if( GR->DG_Level_Max != GR->DG_Old_Level_Max || GR->headers == NULL ){
	GR->headers = new DDG_Head[GR->DG_Level_Max + 1];
    }
    memset( GR->headers, 0, sizeof(DDG_Head)*(GR->DG_Level_Max+1) );
    Uint Count = 0;
    Visit_List( dg, GR->all_nodes, DG ){
	++Count;
    }End_Visit;
    debug( 2, ( " now creating drawn nodes for %ld real nodes", Count ) );
    GR->DG_Count_Nodes = Count;
    if( Count == 0 ) return 0;

    // Make the ddg nodes
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show && dg->DG_Clone == NULL ){
	    new DDG( dg, GR );
	}
    }End_Visit;
    // make all the ddg links
    Visit_List( dg, GR->all_nodes, DG ){
	Visit_List( succedge, dg->DG_Succs, Ptr_DG ){
	    P_DG succ = link( succedge );
	    DDG_Link( dg->DG_Clone, succ->DG_Clone, succedge );
	}End_Visit;
    }End_Visit;
    Align_Edges( GR );
    // Do a depth-first traversal to place each node in the graph.
    Sint thispos;
    GR->DG_Width_Max = 1;
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( (PTree ? ddg->DDG_Succ_Count : ddg->DDG_Pred_Count) == 0 ){
	    thispos = GR->DG_Width_Max + ddg->DDG_Clone->DG_Width;
	    debug( 1, ( "root of tree at %s at position %ld",
			ddg->DDG_Clone->DG_Name, thispos ) );
	    Insert_Tree( GR, ddg, thispos, 0, PTree );
	} 
    }End_Visit;
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( ddg->DDG_Position == 0 ){
	    return 0;
	}
    }End_Visit;

    if( debuglevel > 1 ) DG_Dump( GR, 1 );
    if( debuglevel ) printf( "%5ld final width\n", GR->DG_Width_Max );
    return 1;
}// DG_Process_Tree


void DG_Process( P_DG_Graph GR ){
    getdebuglevel;
    // Find size of each node
    Find_Size( GR );

    debug( 1, ( "Graph %hd=Tree, %hd=PTree", GR->Tree, GR->PTree ) );
    if( GR->Tree && DG_Process_Tree( GR, 0 ) ){
	GR->PTree = 0;
    }else if( GR->PTree && DG_Process_Tree( GR, 1 ) ){
	GR->Tree = 0;
    }else{
	GR->PTree = 0;
	GR->Tree = 0;
	DG_Process_Simple( GR );
    }
    DG_Assign_Channels( GR );
}// DG_Process

void DG_Process_Interference( P_DG_Graph GR ){
    getdebuglevel;
    GR->Interference = 1;
    // Count the nodes, build the DDG nodes
    GR->DG_Count_Nodes = 0;
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show ){
	    P_DDG ddg = new DDG( dg, GR );
	    ddg->DDG_Position = GR->DG_Count_Nodes;
	    ++(GR->DG_Count_Nodes);
	}
    }End_Visit;
    Visit_List( dg, GR->all_nodes, DG ){
	if( dg->DG_Show ){
	    Visit_List( succedge, dg->DG_Succs, Ptr_DG ){
		P_DG succ = link( succedge );
		if( succ->DG_Show ){
		    DDG_Link( dg->DG_Clone, succ->DG_Clone, succedge );
		}
	    }End_Visit;
	}
    }End_Visit;
}// DG_Process_Interference

void DG_Hide_Node( P_DG_Graph GR, P_DG dg ){
P_DDG dgnode = dg->DG_Clone;
    if( dgnode == NULL ) return;
    if( GR->Interference ){
	dgnode->DDG_Show = 0;
    }else{
	// for this node, or any top_corner or bottom_corner connected to this node,
	// hide it, and hide any 'way' or 'rway' nodes connected to them
	Visit_List( dg, GR->headers[dgnode->DDG_Level].Level_List, DDG ){
	    Bool hide = 0;
	    if( dg == dgnode ){
		hide = 1;
	    }else if( dg->DDG_Type == ddg_top_corner && dg->Sideways_To == dgnode ){
		hide = 1;
	    }else if( dg->DDG_Type == ddg_bottom_corner && dg->Sideways_From == dgnode ){
		hide = 1;
	    }else if( dg->DDG_Type == ddg_both_corners && (dg->Sideways_To == dgnode || dg->Sideways_From == dgnode ) ){
		hide = 1;
	    }
	    if( hide ){
		dg->DDG_Show = 0;
		// also elide any way, rway nodes connected to this one
		Visit_Ptr_List( succ, dg->DDG_Succs, DDG ){
		    P_DDG n, nextn;
		    for( n = succ; n; n = nextn ){
			nextn = NULL;
			if( n->DDG_Show ){
			    switch( n->DDG_Type ){
			    case ddg_way:
			    case ddg_rway:
			    case ddg_top_corner:
			    case ddg_bottom_corner:
			    case ddg_both_corners:
				n->DDG_Show = 0;
				P_Ptr_DDG fsucc = first( n->DDG_Succs );
				if( fsucc ) nextn = link( fsucc );
			    }
			}
		    }
		}End_Visit;
		Visit_Ptr_List( pred, dg->DDG_Preds, DDG ){
		    P_DDG n, nextn;
		    for( n = pred; n; n = nextn ){
			nextn = NULL;
			if( n->DDG_Show ){
			    switch( n->DDG_Type ){
			    case ddg_way:
			    case ddg_rway:
			    case ddg_top_corner:
			    case ddg_bottom_corner:
			    case ddg_both_corners:
				n->DDG_Show = 0;
				P_Ptr_DDG fpred = first( n->DDG_Preds );
				if( fpred ) nextn = link( fpred );
			    }
			}
		    }
		}End_Visit;
	    }
	}End_Visit;
    }
}// DG_Hide_Node

void DG_Restore_All( P_DG_Graph GR ){
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	ddg->DDG_Show = 1;
    }End_Visit;
}

void DG_Copy_Hidden( P_DG_Graph GR ){
    Visit_Ptr_List( ddg, GR->all_DDG, DDG ){
	if( ddg->DDG_Type == ddg_clone && ddg->DDG_Clone != NULL ){
	    if( ddg->DDG_Show == 0 ){
		ddg->DDG_Clone->DG_Show = 0;
	    }
	}
    }End_Visit;
}


void DG_UnHide( P_DG_Graph GR ){
    Visit_List( dg, GR->all_nodes, DG ){
	dg->DG_Show = 1;
    }End_Visit;
}

void Test( P_DG_Graph GR ){
}// Test
