/* The source code in this file is hereby placed in the public domain.
 * 29 August 1996, Michael Wolfe, Oregon, USA */
/*
 * ggy.y
 *
 * 08 Dec 94 [mw]
 * $Id: ggy.y,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $
 */

/* declarations section */
%{
#include "standard.h"
#include "dg.h"
#include "gg.h"
int yylex();
void yyerror( char *s);
void add_link( P_GG from, P_GG to );
P_GG find_node( );
P_Var find_id( );
void add_assignment( P_GG node, P_Var lhs, P_Expression rhs );
void parse_init( );
void parse_die( );
void addedgelabel();
int intval();
P_Dataflow find_dataflow( );
P_DFExpression makeexp( operatortype op, P_Dataflow operand );
P_DFExpression makebinary( operatortype op, P_DFExpression left, 
	    P_DFExpression right );
P_Expression newexpr( ExType op, P_Var operand );
P_Expression newexpr( P_Expression left, ExType op, P_Expression right );

P_GG from_node, to_node, global_node;
P_Var global_id;
DGL_Arrows arrowtype;
DG_Styles arrowstyle;
P_Dataflow DF_LHS;

%}

%union {
    Name_Type name;
    P_DFExpression dfexp;
    P_Dataflow df;
    P_Expression exp;
    P_Var var;
    int Intval;
    }

/* add verbose debug */
%verbose

/* add debug */
%debug

%token TID TSTRING
%token TARROW TREVARROW TBOTHARROW TNOARROW
%token TDOTARROW TDOTREVARROW TDOTBOTHARROW TDOTNOARROW 
%token TBARBAR TAMPAMP TBAR TUPARROW TAMP TEQUALEQUAL TNOTEQUAL
%token TGREATEREQUAL TGREATER TLESSEQUAL TLESS TLEFTSHIFT TRIGHTSHIFT
%token TPLUS TDASH TSTAR TSLASH TMOD TBANG TTILDE
%token TCOLON TCOMMA TEQUAL TLP 
%token TRP TSEMI TBANG TDOT TLSB TRSB TLCB TRCB 
%token TALL TANY TPRED TSUCC TINT TIN TOUT
%token TNODE TEDGE

%type <dfexp> expression
%type <dfexp> factor
%type <dfexp> atom
%type <dfexp> term
%type <df> equationid
%type <Intval> direction
%type <Intval> inout
%type <Intval> value
%type <Intval> nodeexpr
%type <var> lhs
%type <var> lhsid
%type <exp> rhs 
%type <exp> rhs0 
%type <exp> rhs10 
%type <exp> rhs20 
%type <exp> rhs30 
%type <exp> rhs40 
%type <exp> rhs50 
%type <exp> rhs60 
%type <exp> rhs70 
%type <exp> rhs80 
%type <exp> rhs90 
%type <exp> rhs100 
%type <exp> rhs110

%start graph

%%
/* rules section */

graph : 
	things;

things :
	thing
 |
	things thing
 |
	things TSEMI thing
;

thing : link | list | equationlist ;

link : 
   TLP from linksep to TRP
 | TLP from linksep to TCOMMA edgelabel TRP
 | from TDOTARROW {arrowstyle = dg_dashed; }to 
 | from TDOTREVARROW { arrowtype = dgl_start; arrowstyle = dg_dashed; } to 
 | from TDOTBOTHARROW { arrowtype = dgl_both; arrowstyle = dg_dashed; } to 
 | from TDOTNOARROW { arrowtype = dgl_neither; arrowstyle = dg_dashed; } to 
 | from TARROW to 
 | from TREVARROW { arrowtype = dgl_start } to 
 | from TBOTHARROW { arrowtype = dgl_both } to 
 | from TNOARROW { arrowtype = dgl_neither } to 
 | from TDASH to 
 | from TCOLON to 
 | from TCOMMA to 
;

linksep:
   TCOMMA { arrowtype = dgl_end; }
 | TARROW { arrowtype = dgl_end; }
 | TDASH { arrowtype = dgl_end; }
 | TCOLON { arrowtype = dgl_end; }
 | TREVARROW { arrowtype = dgl_start } 
 | TBOTHARROW { arrowtype = dgl_both } 
 | TNOARROW { arrowtype = dgl_neither } 
 | TDOTARROW { arrowtype = dgl_end; arrowstyle = dg_dashed; }
 | TDOTREVARROW { arrowtype = dgl_start; arrowstyle = dg_dashed; } 
 | TDOTBOTHARROW { arrowtype = dgl_both; arrowstyle = dg_dashed; } 
 | TDOTNOARROW { arrowtype = dgl_neither; arrowstyle = dg_dashed; } 

from :
	TID
		{
		    arrowtype = dgl_end; arrowstyle = dg_solid;
		    global_node = from_node = find_node();
		}
	attributes
|
	TINT
		{
		    arrowtype = dgl_end; arrowstyle = dg_solid;
		    global_node = from_node = find_node();
		}
	attributes
;

to:
	TID
		{
		    global_node = to_node = find_node();
		    add_link( from_node, to_node );
		}
	attributes
|
	TINT
		{
		    global_node = to_node = find_node();
		    add_link( from_node, to_node );
		}
	attributes
;

edgelabel : TID { addedgelabel(); }
| TINT { addedgelabel(); }
| TSTRING { addedgelabel(); };

attributes:
 |
	TBANG
		{   global_node->ggtype = dg_box;
		}
 |
	TBANG TBANG
		{   global_node->ggtype = dg_hex;
		}
;

list:
	TID { global_node = from_node = find_node(); }
	TLSB stufflist TRSB
|	TINT { global_node = from_node = find_node(); }
	TLSB stufflist TRSB
;

stufflist:
 |	stuff
;

stuff:	stuffing
 |	stuff TSEMI stuffing
 |	stuff TCOMMA stuffing
;

stuffing:
	lhs rhs { add_assignment( global_node, $1, $2 ); };


lhsid:	TID { $$ = find_id(); };

lhs:
	lhsid TEQUAL { $$ = $1; } 
 |	TEQUAL { $$ = NULL; } 
;

rhs:	rhs0 { $$ = $1; }
|	{ $$ = NULL; };

rhs0:	rhs10 { $$ = $1; }
 |	rhs0 TBARBAR rhs10 { $$ = newexpr( $1, exOrElse, $3 ); };

rhs10:	rhs20 { $$ = $1; }
 |	rhs10 TAMPAMP rhs20 { $$ = newexpr( $1, exAndThen, $3 ); };

rhs20:	rhs30 { $$ = $1; }
 |	rhs20 TBAR rhs30 { $$ = newexpr( $1, exOr, $3 ); };

rhs30:	rhs40 { $$ = $1; }
 |	rhs30 TUPARROW rhs40 { $$ = newexpr( $1, exXor, $3 ); };

rhs40:	rhs50 { $$ = $1; }
 |	rhs40 TAMP rhs50 { $$ = newexpr( $1, exAnd, $3 ); };

rhs50:	rhs60 { $$ = $1; }
 |	rhs50 TEQUALEQUAL rhs60 { $$ = newexpr( $1, exEqual, $3 ); }
 |	rhs50 TNOTEQUAL rhs60 { $$ = newexpr( $1, exNotEqual, $3 ); };

rhs60:	rhs70 { $$ = $1; }
 |	rhs60 TGREATEREQUAL rhs70 { $$ = newexpr( $1, exGreaterEqual, $3 ); }
 |	rhs60 TGREATER rhs70 { $$ = newexpr( $1, exGreater, $3 ); }
 |	rhs60 TLESSEQUAL rhs70 { $$ = newexpr( $1, exLessEqual, $3 ); }
 |	rhs60 TLESS rhs70 { $$ = newexpr( $1, exLess, $3 ); };

rhs70:	rhs80 { $$ = $1; }
 |	rhs70 TLEFTSHIFT rhs80 { $$ = newexpr( $1, exLeftShift, $3 ); }
 |	rhs70 TRIGHTSHIFT rhs80 { $$ = newexpr( $1, exRightShift, $3 ); };

rhs80:	rhs90 { $$ = $1; }
 |	rhs80 TPLUS rhs90 { $$ = newexpr( $1, exAdd, $3 ); }
 |	rhs80 TDASH rhs90 { $$ = newexpr( $1, exSub, $3 ); };

rhs90:	rhs100 { $$ = $1; }
 |	rhs90 TSTAR rhs100 { $$ = newexpr( $1, exMul, $3 ); }
 |	rhs90 TMOD rhs100 { $$ = newexpr( $1, exMod, $3 ); }
 |	rhs90 TSLASH rhs100 { $$ = newexpr( $1, exDiv, $3 ); };

rhs100:	rhs110 { $$ = $1; }
 |	TBANG rhs110 { $$ = newexpr( NULL, exNot, $2 ); }
 |	TTILDE rhs110 { $$ = newexpr( NULL, exCompl, $2 ); }
 |	TPLUS rhs110 { $$ = $2; }
 |	TDASH rhs110 { $$ = newexpr( NULL, exNeg, $2 ); }
 |	TSTAR rhs110 { $$ = newexpr( NULL, exDeref, $2 ); };

rhs110: TID { $$ = newexpr( exVar, find_id() ); }
 |	TINT { $$ = newexpr( exConst, find_id() ); }
 |	TLP rhs0 TRP { $$ = $2 };


atom:
	equationid { $$ = makeexp( opValue, $1 ); }
	| equationid TDOT TIN { $$ = makeexp( opInValue, $1 ); } /* node only */
	| equationid TDOT TOUT { $$ = makeexp( opOutValue, $1 ); } /* node only */
	| TALL TPRED atom { $$ = makebinary( opAllPred, NULL, $3 ); }
	| TANY TPRED atom { $$ = makebinary( opAnyPred, NULL, $3 ); }
	| TALL TPRED TEDGE atom { $$ = makebinary( opAllPredEdges, NULL, $4 ); }
	| TANY TPRED TEDGE atom { $$ = makebinary( opAnyPredEdges, NULL, $4 ); }
	| TPRED TNODE atom { $$ = makebinary( opPredNode, NULL, $3 ); } /* edge only */
	| TALL TSUCC atom { $$ = makebinary( opAllSucc, NULL, $3 ); }
	| TANY TSUCC atom { $$ = makebinary( opAnySucc, NULL, $3 ); }
	| TALL TSUCC TEDGE atom { $$ = makebinary( opAllSuccEdges, NULL, $4 ); }
	| TANY TSUCC TEDGE atom { $$ = makebinary( opAnySuccEdges, NULL, $4 ); }
	| TSUCC TNODE atom { $$ = makebinary( opSuccNode, NULL, $3 ); } /* edge only */
	| TLP expression TRP { $$ = $2 };

factor:
	atom { $$ = $1; }
	| TBANG atom { $$ = makebinary( opNot, NULL, $2 ); }
;

term :
	factor { $$ = $1; } 
	| term TSTAR factor { $$ = makebinary( opAnd, $1, $3 ); }
;

expression :
	term { $$ = $1; } 
	| expression TPLUS term { $$ = makebinary( opOr, $1, $3 ); }
;


equationlist : TLCB eqlist TRCB;

eqlist:	equation
 |	eqlist TSEMI equation;

equation :
	equationid { DF_LHS = $1; } 
		inout nodeexpr direction { $1->Direction = $5; } 
		initial TEQUAL expression 
		{
		    if( $4 == 1 )
			if( $1->expression[3] == NULL )
			    $1->expression[3] = $9; 
			else printf( "Edge property %s defined twice\n", $1->Name );
		    else if( $1->expression[$3] == NULL )
			$1->expression[$3] = $9; 
		    else if( $3 == 0 ) printf( "Node property %s defined twice\n", $1->Name );
		    else if( $3 == 1 ) printf( "Node property %s.in defined twice\n", $1->Name );
		    else if( $3 == 2 ) printf( "Node property %s.out defined twice\n", $1->Name );
		} 
;

equationid : TID { $$ = find_dataflow( ); };

nodeexpr :
	TNODE { DF_LHS->EdgeNode |= 2; $$ = 2; }
 |	TEDGE { DF_LHS->EdgeNode |= 1; $$ = 1; }
 |	{ DF_LHS->EdgeNode |= 1; $$ = 1; }
;

inout:
	TDOT TIN { $$ = 1; }
	| TDOT TOUT { $$ = 2; }
	| { $$ = 0; }
;

initial : { DF_LHS->Initially = 0; DF_LHS->Boundary = 0; }
 |	TLSB value TRSB { DF_LHS->Initially = $2; DF_LHS->Boundary = 0; }
 |	TLSB value TCOMMA value TRSB 
		{ DF_LHS->Initially = $2; DF_LHS->Boundary = $4; }
;

value : { $$ = 0; }
 |	TINT { if( intval() == 0 ) $$ = 0; else $$ = 1; }
;

direction : TPLUS { $$ = +1; }
	| TDASH { $$ = -1; }
	| { $$ = 0; }
;

%%
/* program section */

/* global variables: */
/* line number */
long Lines;

P_GG GG_Prev;

/* communication from lexer */
char lexname[120];

#include "ggl.c"

/* parse_die */
void parse_die( void )
{
    printf( "Ended on line %d at >>>%s<<<\n", Lines, yytext );
    exit(1);
}/* parse_die */

/* Initialize everything. */
void parse_init( void ){
P_Dataflow df;
    GG_Nodes = 0;
    GG_Links = 0;
    GG_List.Init();
    Vars = 0;
    Var_List.Init();
    Dataflow_List.Init();
    Expression_List.Init();
    Lines = 1;
    DataflowCount = 0;

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "TRANSDOWN" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opTransDown, NULL );
    df->EdgeNode = 1;
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "TRANSUP" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opTransUp, NULL );
    df->EdgeNode = 1;
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "TRANS" );
    df->Position = DataflowCount;
    df->expression[0] = makeexp( opTrans, NULL );
    df->EdgeNode = 2; /* a node property */
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "EXPOSEUP" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opUXP, NULL );
    df->expression[0] = makeexp( opUXP, NULL );
    df->EdgeNode = 3; /* node and edge property */
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "EXPOSEDOWN" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opDXP, NULL );
    df->expression[0] = makeexp( opDXP, NULL );
    df->EdgeNode = 3; /* node and edge property */
    insert_list_end( df, Dataflow_List );
    BuiltinProperties = DataflowCount;
}/* parse_init */

/* find a node with this name */
P_GG find_node( void ){
P_GG newgg;

    Visit_List(thisgg,GG_List,GG){
	if( strcmp( thisgg->GG_Name, lexname ) == 0 ){
	    return thisgg;
	}
    }End_Visit;

    /* must add a new one */
    ++GG_Nodes;
    newgg = new GG;
    memset( newgg, 0, sizeof( GG ) );
    newgg->ggtype = dg_circle;
    newgg->GG_Succs.Init();
    newgg->GG_Preds.Init();
    strcpy( newgg->GG_Name, lexname );
    newgg->GG_Number = 0;
    newgg->Lowlink = 0;
    newgg->InStack = 0;
    newgg->Stack_Next = NULL;

    insert_list_end( newgg, GG_List );
    return newgg;
}/* find_node */

P_Ptr_GG link1, link2;
void add_link( P_GG from, P_GG to ){
    ++GG_Links;
    link1 = new Ptr_GG;
    memset( link1, 0, sizeof( Ptr_GG ) );
    link2 = new Ptr_GG;
    memset( link2, 0, sizeof( Ptr_GG ) );

    link1->Link = to;
    link1->GGL_Match = link2;
    link1->GGL_IN_Tree = 0;
    link1->ggarrow = arrowtype;
    link1->ggstyle = arrowstyle;
    insert_list_end( link1, from->GG_Succs );

    link2->Link = from;
    link2->GGL_Match = link1;
    link2->GGL_IN_Tree = 0;
    link2->ggarrow = arrowtype;
    link2->ggstyle = arrowstyle;
    insert_list_end( link2, to->GG_Preds );
}/* add_link */

void addedgelabel( void ){
    strcpy( link1->GG_EdgeName, lexname );
    strcpy( link2->GG_EdgeName, lexname );
}/* addedgelabel */

P_Var find_id( void ){
P_Var newvar;
    Visit_List(thisid,Var_List,Var){
	if( strcmp( thisid->Name, lexname ) == 0 ){
	    return thisid;
	}
    }End_Visit;

    /* must add a new one */
    ++Vars;
    newvar = new Var;
    memset( newvar, 0, sizeof( Var ) );
    strcpy( newvar->Name, lexname );
    newvar->Num = Vars;
    insert_list_end( newvar, Var_List );
    return newvar;
}/* find_id */

void add_assignment( P_GG node, P_Var lhs, P_Expression rhs ){
P_Assignment newref;
    newref = new Assignment;
    memset( newref, 0, sizeof( Assignment ) );
    newref->LHS = lhs;
    newref->RHS = rhs;
    insert_list_end( newref, node->Assignments );
}/* add_ref */

P_Dataflow find_dataflow( ){
    P_Dataflow newdf;
    Visit_List(thisdf,Dataflow_List,Dataflow){
	if( strcmp( thisdf->Name, lexname ) == 0 ){
	    return thisdf;
	}
    }End_Visit;

    /* must add a new one */
    ++DataflowCount;
    newdf = new Dataflow;
    memset( newdf, 0, sizeof( Dataflow ) );
    strcpy( newdf->Name, lexname );
    newdf->Position = DataflowCount;
    insert_list_end( newdf, Dataflow_List );
    return newdf;
}/* find_dataflow */

P_DFExpression makeexp( operatortype op, P_Dataflow operand ){
    P_DFExpression newexp = new DFExpression;
    memset( newexp, 0, sizeof( DFExpression ) );
    newexp->OP = op;
    newexp->Operand = operand;
    return newexp;
}/* makeexp */

P_DFExpression makebinary( operatortype op, P_DFExpression left, 
	    P_DFExpression right ){
    P_DFExpression newexp = new DFExpression;
    memset( newexp, 0, sizeof( DFExpression ) );
    newexp->OP = op;
    newexp->Left = left;
    newexp->Right = right;
    return newexp;
}/* makebinary */

P_Expression findexpr( ExType op, P_Expression left, P_Expression right,
		P_Var operand ){
    Visit_List( ex, Expression_List, Expression ){
	if( ex->OP == op &&
	    ex->Left == left &&
	    ex->Right == right &&
	    ex->Operand == operand ) return ex;
    }End_Visit;
    return NULL;
}// findexpr

P_Expression newexpr( P_Expression left, ExType op, P_Expression right ){
    P_Expression newexp = findexpr( op, left, right, NULL );
    if( newexp ) return newexp;
    newexp = new Expression;
    memset( newexp, 0, sizeof( Expression ) );
    newexp->OP = op;
    newexp->Left = left;
    newexp->Right = right;
    insert_list_end( newexp, Expression_List );
    return newexp;
}/* newexpr */

P_Expression newexpr( ExType op, P_Var operand ){
    P_Expression newexp = findexpr( op, NULL, NULL, operand );
    if( newexp ) return newexp;
    newexp = new Expression;
    memset( newexp, 0, sizeof( Expression ) );
    newexp->OP = op;
    newexp->Operand = operand;
    insert_list_end( newexp, Expression_List );
    return newexp;
}/* newexpr */

int intval(){
    int v = 0;
    char *s = lexname;
    while( *s ){
	v = v*10 + (*s-'0');
	++s;
    }
    return v;
}/* intval */

void yyerror( char *s)
{
    printf( "%s\n", s );
    parse_die();
}/* yyerror */
