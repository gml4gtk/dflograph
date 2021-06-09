// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// ggmain.cc
//
// 08 Dec 94 [mw]
// $Id: ggmain.cc,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $

// make a directed graph, then try to plot it interactively

#include "portable.h"
#include "standard.h"
#include "error.h"
#include "handler.h"

#include "dg.h"

#include "dgplot.h"
#include "dgpost.h"

#include "gg.h"
#include "args.h"

struct filestruct* firstfile = NULL;
struct filestruct* lastfile = NULL;
P_char infilename = NULL;
P_DG_Graph globalgraph;

char* mainfont = "fixed";
char* leftfont = "fixed";
char* rightfont = "fixed";

void GG_Dump( void );

Bool Interference = 0;
Bool Postscript_Only = 0;

static void(*Node_Handler)( P_DG_Graph GR, P_DG dgnode );
char* printExpression( P_Expression ex );
void printDFExpression( P_DFExpression e, int precedence );

void handle_hide( P_DG_Graph graph, P_DG dgnode ){
    DG_Hide_Node( graph, dgnode );
    DG_Clear( graph );
}

void handle_nothing( P_DG_Graph GR, P_DG dgnode ){
    
}

Uint handler( P_DG_Graph graph, P_DG dgnode, Uint current ){
P_GG gg = (P_GG)(current);
    printf( "at node %s\n", gg->GG_Name );
    if( first( gg->Assignments ) ){
	Visit_List( thisref, gg->Assignments, Assignment ){
	    if( thisref->LHS ){
		printf( "%s", thisref->LHS->Name );
	    }
	    printf( " = %s\n", printExpression( thisref->RHS ) );
	}End_Visit;
    }
    if( Node_Handler ) (*Node_Handler)( graph, dgnode );
    return 0;
};

static Bool AmHiding = 0;
Uint starthiding( P_DG_Graph graph, Uint g ){
    if( AmHiding ){
	AmHiding = 0;
	RestoreCursor( graph );
	Node_Handler = handle_nothing;
    }else{
	AmHiding = 1;
	PirateCursor( graph );
	Node_Handler = handle_hide;
    }
    return 0;
}

Uint restoreall( P_DG_Graph graph, Uint g ){
    AmHiding = 0;
    WaitCursor( graph );
    Node_Handler = handle_nothing;
    DG_Restore_All( graph );
    DG_Clear( graph );
    RestoreCursor( graph );
    return 0;
}

Uint replot( P_DG_Graph graph, Uint g ){
    AmHiding = 0;
    WaitCursor( graph );
    Node_Handler = handle_nothing;
    DG_Copy_Hidden( graph );
    if( Interference ){
	DG_Process_Interference( graph );
    }else{
	DG_Process( graph );
    }
    DG_Clear( graph );
    RestoreCursor( graph );
    return 0;
}

Uint resurrect( P_DG_Graph graph, Uint g ){
    AmHiding = 0;
    WaitCursor( graph );
    Node_Handler = handle_nothing;
    DG_UnHide( graph );
    if( Interference ){
	DG_Process_Interference( graph );
    }else{
	DG_Process( graph );
    }
    DG_Clear( graph );
    RestoreCursor( graph );
    return 0;
}

Uint test( P_DG_Graph graph, Uint g ){
    WaitCursor( graph );
    Test( graph );
    DG_Clear( graph );
    RestoreCursor( graph );
    return 0;
}

Uint dump( P_DG_Graph graph, Uint g ){
    DG_Dump( graph, 0 );
    return 0;
}

Uint doggdump( P_DG_Graph graph, Uint g ){
    GG_Dump();
}

char* postscriptfile = "PS";
char* postscriptfont = "Times-Italic";
Sint postscriptcircle = 18;
Sint postscriptfontsize = 50;
Sint postscriptspacing = 5;
Bool postscriptscalefont = 0;
Bool postscriptscaledrawing = 0;

Uint doPostscript( P_DG_Graph graph, Uint g ){
    PostFile = fopen( postscriptfile, "w" );
    DG_Post_Init( postscriptcircle, postscriptfont, postscriptfontsize,
	postscriptspacing, postscriptscalefont );
    DG_Post_Graph( graph, postscriptscaledrawing );
    fclose( PostFile );
    PostFile = NULL;
}


static void Init( P_Dataflow df ){
    if( df->Initially == 1 ){
	int Position = df->Position;
	Uint32 bit = 1 << Position;
	Visit_List( gg, GG_List, GG ){
	    gg->Value[0] |= bit;
	    gg->Value[1] |= bit;
	    gg->Value[2] |= bit;
	    Visit_List( succggptr, gg->GG_Succs, Ptr_GG ){
		succggptr->Value |= bit;
		succggptr->GGL_Match->Value |= bit;
	    }End_Visit;
	}End_Visit;
    }
}// Init

static Bool found( P_Var var, P_Expression e ){
    if( e == NULL ) return 0;
    if( e->OP == exVar && e->Operand == var ) return 1;
    if( e->Left && found( var, e->Left ) ) return 1;
    if( e->Right && found( var, e->Right ) ) return 1;
    return 0;
}

static Bool foundexpr( P_Expression lookfor, P_Expression e ){
    if( e == NULL ) return 0;
    if( e == lookfor ) return 1;
    if( foundexpr( lookfor, e->Left ) ) return 1;
    if( foundexpr( lookfor, e->Right ) ) return 1;
    return 0;
}

static Bool CheckTransparent( P_GG gg, P_Expression expr ){
    Visit_List( r, gg->Assignments, Assignment ){
	if( found( r->LHS, expr ) ) return 0;
    }End_Visit;
    return 1;
}// checktransparent

static Bool CheckExposed( P_GG gg, P_Expression expr, int Direction ){
    if( Direction > 0 ){
	Visit_List( r, gg->Assignments, Assignment ){
	    if( foundexpr( expr, r->RHS ) ) return 1;
	    if( found( r->LHS, expr ) ) return 0; /* killed */
	}End_Visit;
	return 0;
    }else{
	Visit_List_Backwards( r, gg->Assignments, Assignment ){
	    if( found( r->LHS, expr ) ) return 0; /* killed */
	    if( foundexpr( expr, r->RHS ) ) return 1;
	}End_Visit;
	return 0;
    }
}// checkexposed

static Bool EvaluateEdge( P_GG from, P_GG to, P_Ptr_GG link, P_DFExpression dfexpr, 
		P_Expression expr, P_Dataflow df );

static Bool EvaluateNode( P_GG node, P_DFExpression dfexpr, 
		P_Expression expr, P_Dataflow df ){
    Bool l, r, ret;
    Uint32 P, bit, val;
    ret = 0;
    if( debuglevel > 3 ){
	printf( "called EvaluateNode(" );
	printDFExpression( dfexpr, 0 );
	printf( ") for %s expression %s on %s\n", node->GG_Name, printExpression( expr ), df->Name );
    }
    switch( dfexpr->OP ){
    case opNot:
	r = EvaluateNode( node, dfexpr->Right, expr, df );
	if( !r ) ret = 1;
	break;
    case opAnd:
	l = EvaluateNode( node, dfexpr->Left, expr, df );
	r = EvaluateNode( node, dfexpr->Right, expr, df );
	if( l && r ) ret = 1;
	break;
    case opOr:
	l = EvaluateNode( node, dfexpr->Left, expr, df );
	r = EvaluateNode( node, dfexpr->Right, expr, df );
	if( l || r ) ret = 1;
	break;
    case opAllPred:
	if( is_empty( node->GG_Preds ) ){
	    ret = df->Boundary;
	}else{
	    ret = 1;
	    Visit_Ptr_List( fromptr, node->GG_Preds, GG ){
		ret = ret & 
		    EvaluateNode( fromptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAnyPred:
	if( is_empty( node->GG_Preds ) ){
	    ret = df->Boundary;
	}else{
	    ret = 0;
	    Visit_Ptr_List( fromptr, node->GG_Preds, GG ){
		ret = ret |
		    EvaluateNode( fromptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAllSucc:
	if( is_empty( node->GG_Succs ) ){
	    ret = df->Boundary;
	}else{
	    ret = 1;
	    Visit_Ptr_List( toptr, node->GG_Succs, GG ){
		ret = ret &
		    EvaluateNode( toptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAnySucc:
	if( is_empty( node->GG_Succs ) ){
	    ret = df->Boundary;
	}else{
	    ret = 0;
	    Visit_Ptr_List( toptr, node->GG_Succs, GG ){
		ret = ret |
		    EvaluateNode( toptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAllPredEdges:
	if( is_empty( node->GG_Preds ) ){
	    ret = df->Boundary;
	}else{
	    ret = 1;
	    Visit_List( fromptr, node->GG_Preds, Ptr_GG ){
		ret = ret & 
		    EvaluateEdge( fromptr->Link, node, fromptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAnyPredEdges:
	if( is_empty( node->GG_Preds ) ){
	    ret = df->Boundary;
	}else{
	    ret = 0;
	    Visit_List( fromptr, node->GG_Preds, Ptr_GG ){
		ret = ret |
		    EvaluateEdge( fromptr->Link, node, fromptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAllSuccEdges:
	if( is_empty( node->GG_Succs ) ){
	    ret = df->Boundary;
	}else{
	    ret = 1;
	    Visit_List( toptr, node->GG_Succs, Ptr_GG ){
		ret = ret &
		    EvaluateEdge( node, toptr->Link, toptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAnySuccEdges:
	if( is_empty( node->GG_Succs ) ){
	    ret = df->Boundary;
	}else{
	    ret = 0;
	    Visit_List( toptr, node->GG_Succs, Ptr_GG ){
		ret = ret |
		    EvaluateEdge( node, toptr->Link, toptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opInValue:
	P = dfexpr->Operand->Position;
	bit = 1<<P;
	val = node->Value[1];
	if( val & bit ) ret = 1;
	break;
    case opOutValue:
	P = dfexpr->Operand->Position;
	bit = 1<<P;
	val = node->Value[2];
	if( val & bit ) ret = 1;
	break;
    case opValue:
	P = dfexpr->Operand->Position;
	bit = 1<<P;
	val = node->Value[0];
	if( val & bit ) ret = 1;
	break;
    case opTrans:
	if( CheckTransparent( node, expr ) ) ret = 1;
	break;
    case opUXP:
	if( CheckExposed( node, expr, +1 ) ) ret = 1;
	break;
    case opDXP:
	if( CheckExposed( node, expr, -1 ) ) ret = 1;
	break;
    default:
	printf( "expression is defined only for nodes in dataflow equation %s:\n",
		df->Name );
	printf( "EvaluateNode(" );
	printDFExpression( dfexpr, 0 );
	printf( ")\n" );
	exit(1);
	break;
    }// switch

    if( debuglevel > 2 ){
	printf( "EvaluateNode(" );
	printDFExpression( dfexpr, 0 );
	printf( ") for %s expression %s is %d\n", node->GG_Name,
		printExpression( expr ), ret );
    }
    return ret;
}// EvaluateNode

static Bool EvaluateEdge( P_GG from, P_GG to, P_Ptr_GG link, P_DFExpression dfexpr, 
		P_Expression expr, P_Dataflow df ){
    Bool l, r, ret;
    Uint32 P, bit, val;
    ret = 0;
    switch( dfexpr->OP ){
    case opNot:
	r = EvaluateEdge( from, to, link, dfexpr->Right, expr, df );
	if( !r ) ret = 1;
	break;
    case opAnd:
	l = EvaluateEdge( from, to, link, dfexpr->Left, expr, df );
	r = EvaluateEdge( from, to, link, dfexpr->Right, expr, df );
	if( l && r ) ret = 1;
	break;
    case opOr:
	l = EvaluateEdge( from, to, link, dfexpr->Left, expr, df );
	r = EvaluateEdge( from, to, link, dfexpr->Right, expr, df );
	if( l || r ) ret = 1;
	break;
    case opAllPred:
    case opAllPredEdges:
	if( is_empty( from->GG_Preds ) ){
	    ret = df->Boundary;
	}else{
	    ret = 1;
	    Visit_List( frompredptr, from->GG_Preds, Ptr_GG ){
		ret = ret & 
		    EvaluateEdge( frompredptr->Link, from, frompredptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAnyPred:
    case opAnyPredEdges:
	if( is_empty( from->GG_Preds ) ){
	    ret = df->Boundary;
	}else{
	    ret = 0;
	    Visit_List( frompredptr, from->GG_Preds, Ptr_GG ){
		ret = ret | 
		    EvaluateEdge( frompredptr->Link, from, frompredptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAllSucc:
    case opAllSuccEdges:
	if( is_empty( to->GG_Succs ) ){
	    ret = df->Boundary;
	}else{
	    ret = 1;
	    Visit_List( tosuccptr, to->GG_Succs, Ptr_GG ){
		ret = ret & 
		    EvaluateEdge( to, tosuccptr->Link, tosuccptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opAnySucc:
    case opAnySuccEdges:
	if( is_empty( to->GG_Succs ) ){
	    ret = df->Boundary;
	}else{
	    ret = 0;
	    Visit_List( tosuccptr, to->GG_Succs, Ptr_GG ){
		ret = ret | 
		    EvaluateEdge( to, tosuccptr->Link, tosuccptr, dfexpr->Right, expr, df );
	    }End_Visit;
	}
	break;
    case opValue:
	P = dfexpr->Operand->Position;
	bit = 1<<P;
	val = link->Value;
	if( val & bit ) ret = 1;
	break;
    case opTransUp:
	if( CheckTransparent( to, expr ) ) ret = 1;
	break;
    case opTransDown:
	if( CheckTransparent( from, expr ) ) ret = 1;
	break;
    case opUXP:
	if( CheckExposed( to, expr, +1 ) ) ret = 1;
	break;
    case opDXP:
	if( CheckExposed( from, expr, -1 ) ) ret = 1;
	break;
    case opPredNode:
	ret = EvaluateNode( from, dfexpr->Right, expr, df );
	break;
    case opSuccNode:
	ret = EvaluateNode( to, dfexpr->Right, expr, df );
	break;
    default:
	printf( "expression is defined only for edges in dataflow equation %s:\n",
		df->Name );
	printf( "EvaluateEdge(" );
	printDFExpression( dfexpr, 0 );
	printf( ")\n" );
	exit(1);
	break;
    }// switch
    if( debuglevel > 2 ){
	printf( "EvaluateEdge(" );
	printDFExpression( dfexpr, 0 );
	printf( ") for %s->%s expression %s is %d\n", from->GG_Name, to->GG_Name,
		printExpression( expr ), ret );
    }
    return ret;
}// EvaluateEdge

Bool TestEdgeVal( P_GG gg, P_GG succgg, P_Ptr_GG ggptr,
		P_Expression expr, Uint32 bit, P_Dataflow df ){
    Bool changed = 0;
    Uint32 V = ggptr->Value;
    if( EvaluateEdge( gg, succgg, ggptr, df->expression[3], expr, df ) ){
	if( ! ( V & bit ) ){
	    V = V | bit;
	    changed = 1;
	}
    }else{
	if( ( V & bit ) ){
	    V = V & ~bit;
	    changed = 1;
	}
    }
    ggptr->Value = V;
    ggptr->GGL_Match->Value = V;
    return changed;
}// TestEdgeVal

Bool TestNodeVal( P_GG gg, P_Expression expr, Uint32 bit, P_Dataflow df,
		int first, int second ){
    Bool changed = 0;
    if( df->expression[0] ){
	Uint32 V = gg->Value[0];
	if( EvaluateNode( gg, df->expression[0], expr, df ) ){
	    if( ! ( V & bit ) ){
		V = V | bit;
		changed = 1;
	    }
	}else{
	    if( ( V & bit ) ){
		V = V & ~bit;
		changed = 1;
	    }
	}
	gg->Value[0] = V;
    }

    if( df->expression[first] ){
	Uint32 V = gg->Value[first];
	if( EvaluateNode( gg, df->expression[first], expr, df ) ){
	    if( ! ( V & bit ) ){
		V = V | bit;
		changed = 1;
	    }
	}else{
	    if( ( V & bit ) ){
		V = V & ~bit;
		changed = 1;
	    }
	}
	gg->Value[first] = V;
    }

    if( df->expression[second] ){
	Uint32 V = gg->Value[second];
	if( EvaluateNode( gg, df->expression[second], expr, df ) ){
	    if( ! ( V & bit ) ){
		V = V | bit;
		changed = 1;
	    }
	}else{
	    if( ( V & bit ) ){
		V = V & ~bit;
		changed = 1;
	    }
	}
	gg->Value[second] = V;
    }

    return changed;
}// TestNodeVal


static void Solve( P_Dataflow df, P_Expression expr ){
    int Position = df->Position;
    Uint32 bit = 1 << Position;
    printf( "solving %s for variable %s\n", df->Name, printExpression( expr ) );

    Bool changed = 0;
    if( df->Direction >= 0 ){
	do{
	    changed = 0;
	    Visit_List( gg, GG_List, GG ){
		if( df->EdgeNode & 1 ){
		    Visit_List( succggptr, gg->GG_Succs, Ptr_GG ){
			if( TestEdgeVal( gg, succggptr->Link, succggptr, 
				expr, bit, df )) {
			    changed = 1;
			}
		    }End_Visit;
		}
		if( df->EdgeNode & 2 ){
		    if( TestNodeVal( gg, expr, bit, df, 1, 2 )) {
			changed = 1;
		    }
		}
	    }End_Visit;
	}while( changed );
    }else if( df->Direction < 0 ){
	do{
	    changed = 0;
	    Visit_List_Backwards( gg, GG_List, GG ){
		if( df->EdgeNode & 1 ){
		    Visit_List( predggptr, gg->GG_Preds, Ptr_GG ){
			if( TestEdgeVal( predggptr->Link, gg, predggptr, 
				expr, bit, df )) {
			    changed = 1;
			}
		    }End_Visit;
		}
		if( df->EdgeNode & 2 ){
		    if( TestNodeVal( gg, expr, bit, df, 2, 1 )) {
			changed = 1;
		    }
		}
	    }End_Visit;
	}while( changed );
    }
}// Solve

Uint ShowExpr( P_DG_List list, Uint32 e ){
    P_Expression expr = (P_Expression)e;

    Visit_List( gg, GG_List, GG ){
	Visit_List( succggptr, gg->GG_Succs, Ptr_GG ){
	    DG_Change_Style( gg->DG_Pointer, succggptr->Link->DG_Pointer, dg_solid );
	    succggptr->Value = 0;
	    succggptr->GGL_Match->Value = 0;
	}End_Visit;
	DG_Change_Shape( gg->DG_Pointer, dg_circle );
	DG_Change_Style( gg->DG_Pointer, dg_solid );
	// find a killing assignment and/or appearance of this expression
	int i = 0;
	char name1[100];
	char name2[100]; 
	name1[0] = 0;
	name2[0] = 0;
	Bool upwards = 0, downwards = 0;
	P_Var kills = NULL;
	Visit_List( refs, gg->Assignments, Assignment ){
	    // see if there is an appearance of this expression
	    // above all killing defs, or below
	    if( foundexpr( expr, refs->RHS ) ){
		downwards = 1;
		if( !kills ) upwards = 1;
	    }
	    if( found( refs->LHS, expr ) ){
		kills = refs->LHS;
		downwards = 0;
	    }
	}End_Visit;
	if( kills ){
	    char n[Name_Len+1];
	    strcpy( n, kills->Name );
	    strcat( n, "=" );
	    if( upwards ){
		DG_Change_Name( gg->DG_Pointer, printExpression( expr ) );
		DG_Change_Name2( gg->DG_Pointer, n );
	    }else if( downwards ){
		DG_Change_Name( gg->DG_Pointer, n );
		DG_Change_Name2( gg->DG_Pointer, printExpression( expr ) );
	    }else{
		DG_Change_Name( gg->DG_Pointer, n );
		DG_Change_Name2( gg->DG_Pointer, NULL );
	    }
	}else{
	    if( upwards ){
		DG_Change_Name( gg->DG_Pointer, printExpression( expr ) );
		DG_Change_Name2( gg->DG_Pointer, NULL );
	    }else{
		DG_Change_Name( gg->DG_Pointer, NULL );
		DG_Change_Name2( gg->DG_Pointer, NULL );
	    }
	}
    }End_Visit;
    if( DataflowCount > BuiltinProperties ){
	Visit_List( df, Dataflow_List, Dataflow ){
	    Init( df );
	    Solve( df, expr );
	}End_Visit;
    }
    DG_Clear( globalgraph );
}// ShowExpr

Uint ShowDF( P_DG_List list, Uint32 d ){
    P_Dataflow df = (P_Dataflow)d;
    int Position = df->Position;
    Uint32 bit = 1 << Position;
    if( df->expression[0] ){
	printf( "%s node = ", df->Name );
	printDFExpression( df->expression[0], 0 );
	printf( "\n" );
    }
    if( df->expression[1] ){
	printf( "%s.in = ", df->Name );
	printDFExpression( df->expression[1], 0 );
	printf( "\n" );
    }
    if( df->expression[2] ){
	printf( "%s.out = ", df->Name );
	printDFExpression( df->expression[2], 0 );
	printf( "\n" );
    }
    if( df->expression[3] ){
	printf( "%s edge = ", df->Name );
	printDFExpression( df->expression[3], 0 );
	printf( "\n" );
    }
    Visit_List( gg, GG_List, GG ){
	DG_Change_Style( gg->DG_Pointer, dg_solid );
	DG_Change_Shape( gg->DG_Pointer, dg_circle );
	Visit_List( succggptr, gg->GG_Succs, Ptr_GG ){
	    DG_Change_Style( gg->DG_Pointer, 
		    succggptr->Link->DG_Pointer, dg_solid );
	}End_Visit;
    }End_Visit;
    if( df->EdgeNode & 1 ){
	Visit_List( gg, GG_List, GG ){
	    Visit_List( succggptr, gg->GG_Succs, Ptr_GG ){
		if( bit & succggptr->Value ){
		    DG_Change_Style( gg->DG_Pointer, 
			succggptr->Link->DG_Pointer, dg_bold );
		}
	    }End_Visit;
	}End_Visit;
    }
    if( df->EdgeNode & 2 ){
	Visit_List( gg, GG_List, GG ){
	    if( df->expression[1] ){
		if( bit & gg->Value[1] ){
		    DG_Change_Style_Top( gg->DG_Pointer, dg_bold );
		}
	    }
	    if( df->expression[2] ){
		if( bit & gg->Value[2] ){
		    DG_Change_Style_Bot( gg->DG_Pointer, dg_bold );
		}
	    }
	    if( df->expression[0] ){
		if( bit & gg->Value[0] ){
		    DG_Change_Style( gg->DG_Pointer, dg_bold );
		}
	    }
	}End_Visit;
    }
    DG_Clear( globalgraph );
}// ShowDF

// build the test graph, call the plot routine
void doDG(){
P_DG_Graph graph;

    graph = DG_Init(infilename);
    globalgraph = graph;

    // allocate the nodes
    Visit_List( visitgg, GG_List, GG){
	visitgg->DG_Pointer = DG_Build( graph, visitgg->GG_Name, visitgg->ggtype, dg_solid, 0, handler, (Uint32)(visitgg) );
    }End_Visit;

    // get each link
    Visit_List( predgg, GG_List, GG ){
	Visit_Ptr_List( succgg, predgg->GG_Succs, GG ){
	    DG_Link( predgg->DG_Pointer, succgg->DG_Pointer, dgl_normal,
		ptr_succgg->ggstyle, 0, ptr_succgg->ggarrow );
	    if( ptr_succgg->GG_EdgeName != NULL ){
		DG_Change_Name( predgg->DG_Pointer, succgg->DG_Pointer,
			ptr_succgg->GG_EdgeName );
	    }
	}End_Visit;
    }End_Visit;

    if( Interference ){
	DG_Process_Interference( graph );
    }else{
	DG_Process( graph );
    }

    if( Postscript_Only ){
	PostFile = fopen( postscriptfile, "w" );
	DG_Post_Init( postscriptcircle, postscriptfont, postscriptfontsize,
	    postscriptspacing, postscriptscalefont );
	DG_Post_Graph( graph, postscriptscaledrawing );
	fclose( PostFile );
	PostFile = NULL;
	return;
    }

    new DGbutton( graph, "Exit/Quit", DG_Quit, (Uint32)graph );
    /* new DGbutton( graph, "Hide", starthiding, (Uint32)graph );*/
    /*new DGbutton( graph, "RePlot", replot, (Uint32)graph ); */
    /*new DGbutton( graph, "Restore", restoreall, (Uint32)graph ); */
    /*new DGbutton( graph, "Resurrect", resurrect, (Uint32)graph ); */
    if( debuglevel ){
	new DGbutton( graph, "dump", dump, (Uint32)graph );
	new DGbutton( graph, "debug", doggdump, (Uint32)graph );
    }

    P_DG_List exlist = NULL;
    if( first( Expression_List ) ){
	exlist = new DG_List( "Expressions" );
	Visit_List( ex, Expression_List, Expression ){
	    new DG_List_Item( exlist, printExpression( ex ), ShowExpr, (Uint32)(ex) );
	}End_Visit;
	new DGbutton( graph, "Expressions", exlist );
    }

    if( DataflowCount > BuiltinProperties ){
	Uint nproperties = 0, eproperties = 0;
	Visit_List( df, Dataflow_List, Dataflow ){
	    if( df->Position > BuiltinProperties ){
		if( df->EdgeNode & 1 ) ++eproperties;
		if( df->EdgeNode & 2 ) ++nproperties;
	    }
	}End_Visit;
	if( nproperties ){
	    P_DG_List dflist = new DG_List( "Node Properties" );
	    Visit_List( df, Dataflow_List, Dataflow ){
		if( df->Position > BuiltinProperties && df->EdgeNode & 2 ){
		    new DG_List_Item( dflist, df->Name, ShowDF, (Uint32)(df) );
		}
	    }End_Visit;
	    new DGbutton( graph, "Node Properties", dflist );
	}
	if( eproperties ){
	    P_DG_List dflist = new DG_List( "Edge Properties" );
	    Visit_List( df, Dataflow_List, Dataflow ){
		if( df->Position > BuiltinProperties && df->EdgeNode & 1 ){
		    new DG_List_Item( dflist, df->Name, ShowDF, (Uint32)(df) );
		}
	    }End_Visit;
	    new DGbutton( graph, "Edge Properties", dflist );
	}
    }
    new DGbutton( graph, "Postscript", doPostscript, (Uint32)graph );
    //Adjust( graph, 1 );
    //Adjust( graph, -1 );

    Node_Handler = 0;

    DG_Graph_Init( );
    Uint val = DG_Graph_Plot( graph, "Final Graph", mainfont, leftfont, rightfont );
    DG_Graph_Fini( );
}


extern void yyparse( void );

P_GG GG_Head;
long GG_Nodes, GG_Links;
extern FILE *yyin;

void printDFExpression( P_DFExpression e, int precedence ){
    if( e == NULL ) return;
    switch( e->OP ){
    case opNot:
	if( precedence > 40 ) printf( "(" );
	printf( "!" );
	printDFExpression( e->Right, 41 );
	if( precedence > 40 ) printf( ")" );
	break;
    case opAnd:
	if( precedence > 30 ) printf( "(" );
	printDFExpression( e->Left, 30 );
	printf( "*" );
	printDFExpression( e->Right, 31 );
	if( precedence > 30 ) printf( ")" );
	break;
    case opOr:
	if( precedence > 20 ) printf( "(" );
	printDFExpression( e->Left, 20 );
	printf( "+" );
	printDFExpression( e->Right, 21 );
	if( precedence > 20 ) printf( ")" );
	break;
    case opAllPredEdges:
	printf( "All Pred Edges( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAllPred:
	printf( "All Preds( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAnyPredEdges:
	printf( "Any Pred Edges( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAnyPred:
	printf( "Any Preds( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opPredNode:
	printf( "Pred Node( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAllSuccEdges:
	printf( "All Succ Edges( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAllSucc:
	printf( "All Succs( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAnySuccEdges:
	printf( "Any Succ Edges( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opAnySucc:
	printf( "Any Succs( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opSuccNode:
	printf( "Succ Node( " );
	printDFExpression( e->Right, 0 );
	printf( ")" );
	break;
    case opInValue:
	printf( "%s.in", e->Operand->Name );
	break;
    case opOutValue:
	printf( "%s.out", e->Operand->Name );
	break;
    case opValue:
	printf( "%s", e->Operand->Name );
	break;
    case opTrans:
	printf( "Transparent" );
	break;
    case opTransUp:
	printf( "TransparentUp" );
	break;
    case opTransDown:
	printf( "TransparentDown" );
	break;
    case opUXP:
	printf( "UpwardExposed" );
	break;
    case opDXP:
	printf( "DownwardExposed" );
	break;
    default:
	printf( "{" );
	if( e->Left ) printDFExpression( e->Left, 0 );
	printf( "[%d]", e->OP );
	if( e->Right ) printDFExpression( e->Right, 0 );
	printf( "}" );
    }
}// printDFExpression


void printExpressionNode( P_Expression e, int precedence, char* string );

void printBinary( char* s, P_Expression e, 
	int parentprecedence, int thisprecedence, char* string ){
    if( parentprecedence > thisprecedence ) strcat( string, "(" );
    if( e->Left ) printExpressionNode( e->Left, thisprecedence, string );
    strcat( string, s );
    if( e->Right ) printExpressionNode( e->Right, thisprecedence+1, string );
    if( parentprecedence > thisprecedence ) strcat( string, ")" );
}// printBinary

void printExpressionNode( P_Expression e, int precedence, char* string ){
    if( e == NULL ) return;
    switch( e->OP ){
    case exOrElse: printBinary( "||", e, precedence, 10, string ); break;
    case exAndThen: printBinary( "&&", e, precedence, 20, string ); break;
    case exOr: printBinary( "|", e, precedence, 30, string ); break;
    case exXor: printBinary( "^", e, precedence, 40, string ); break;
    case exAnd: printBinary( "&", e, precedence, 50, string ); break;
    case exEqual: printBinary( "==", e, precedence, 60, string ); break;
    case exNotEqual: printBinary( "!=", e, precedence, 60, string ); break;
    case exGreaterEqual: printBinary( ">=", e, precedence, 70, string ); break;
    case exGreater: printBinary( ">", e, precedence, 70, string ); break;
    case exLessEqual: printBinary( "<=", e, precedence, 70, string ); break;
    case exLess: printBinary( "<", e, precedence, 70, string ); break;
    case exLeftShift: printBinary( "<<", e, precedence, 80, string ); break;
    case exRightShift: printBinary( ">>", e, precedence, 80, string ); break;
    case exAdd: printBinary( "+", e, precedence, 90, string ); break;
    case exSub: printBinary( "-", e, precedence, 90, string ); break;
    case exMul: printBinary( "*", e, precedence, 100, string ); break;
    case exMod: printBinary( "%", e, precedence, 100, string ); break;
    case exDiv: printBinary( "/", e, precedence, 100, string ); break;
    case exNot: printBinary( "!", e, precedence, 110, string ); break;
    case exCompl: printBinary( "~", e, precedence, 110, string ); break;
    case exNeg: printBinary( "-", e, precedence, 110, string ); break;
    case exDeref: printBinary( "*", e, precedence, 110, string ); break;
    case exConst:
    case exVar:
	strcat( string, e->Operand->Name );
	break;

    default:
	strcat( string, "{" );
	if( e->Left ) printExpressionNode( e->Left, 0, string );
        char ff[10];
	sprintf( ff, "[%d]", e->OP );
	strcat( string, ff );
	if( e->Right ) printExpressionNode( e->Right, 0, string );
	strcat( string, "}" );
    }
}// printExpressionNode

char* printExpression( P_Expression ex ){
static char string[500];
    string[0] = 0;
    printExpressionNode( ex, 0, string );
    return string;
}

// dump the graph
void GG_Dump( void ){

    Visit_List( thisgg, GG_List, GG ){
	printf( "%s (%ld):", thisgg->GG_Name, thisgg->GG_Number );
	Visit_Ptr_List( succgg, thisgg->GG_Succs, GG ){
	    if( ptr_succgg->GGL_IN_Tree ){
		printf( " (%s,%s,T)", thisgg->GG_Name, succgg->GG_Name );
	    }else{
		printf( " (%s,%s)", thisgg->GG_Name, succgg->GG_Name );
	    }
	}End_Visit;
	printf( "\n" );
	if( first( thisgg->Assignments ) ){
	    printf( "  [" );
	    Visit_List( thisref, thisgg->Assignments, Assignment ){
		if( thisref->LHS ){
		    printf( "%s", thisref->LHS->Name );
		}
		printf( " = %s", printExpression( thisref->RHS ) );
	    }End_Visit;
	    printf( "]\n" );
	}
    }End_Visit;

    Visit_List( df, Dataflow_List, Dataflow ){
	for( int i = 0; i < 4; ++i ){
	    if( df->expression[i] ){
		printf( "%d:%s", df->Position, df->Name );
		switch(i){
		case 0: break;
		case 1: printf( ".in" ); break;
		case 2: printf( ".out" ); break;
		case 3: printf( " edge" ); break;
		}
		if( df->Direction > 0 ) printf( " +" );
		else if( df->Direction < 0 ) printf( " -" );
		printf( " =" );
		printDFExpression( df->expression[i], 0 );
		printf( "\n" );
	    }
	}
    }End_Visit;
}// GG_Dump


// main routine ... build the test graph
int main( int argc, char *argv[] ){

    if( argc == 1 ){
	fprintf( stderr, "dflo [filename] [-xroutine=debuglevel]\r\n" );
	fprintf( stderr, "got %d arguments\r\n", argc );
	exit( 1 );
    }

    arglist( argc, argv );

    parse_init();
    for( struct filestruct* a = firstfile; a; a = a->next ){
	yyin = fopen( a->name, "r" );
	if( yyin == 0 ){
	    fprintf( stderr, "file not found: %s\n", a->name );
	    exit( 1 );
	}else{
	    if( infilename == NULL ) infilename = a->name;
	    yyparse();
	}
    }


    printf( "Parsed %ld nodes with %ld links\n", GG_Nodes, GG_Links );

    if( debuglevel > 3 ){
	GG_Dump();
    }

    doDG();
    return 0;
}// main
