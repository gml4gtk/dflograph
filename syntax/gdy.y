/* The source code in this file is hereby placed in the public domain.
 * 29 August 1996, Michael Wolfe, Oregon, USA */
/*
 * gdy.y
 *
 * $Id: gdy.y,v 1.1 1996/07/31 22:31:17 mwolfe Exp $
 */

/* declarations section */
%{
#include "standard.h"
#include "dg.h"
#include "gg.h"
int yylex();
void yyerror( char *s);
void add_link( P_GG from, P_GG to );
P_GG find_node( int );
void parse_init( );
void parse_die( );
void addedgelabel();

DGL_Arrows arrowtype;
DG_Styles arrowstyle;


/* communication from lexer */
char lexname[120];

%}

/* add verbose debug */
%verbose

/* add debug */
%debug

%union {
    P_GG ggptr;
    Name_Type name;
    int Intval;
    }

%token TLSB TRSB 
%token TNODE TEDGE TGRAPH TDIGRAPH TINT TSTRING

%type <ggptr> nodenum
%type <name> nodestring
%start graph

%%
/* rules section */

graph : graphtype TLSB things TRSB;


graphtype :
	TGRAPH {arrowtype = dgl_neither; arrowstyle = dg_solid; }
|
	TDIGRAPH {arrowtype = dgl_end; arrowstyle = dg_solid; }
;

things :
	thing
|
	things thing
;

thing : node | edge;

node :
    TNODE TLSB nodenum TRSB
|
    TNODE TLSB nodenum nodestring {strcpy($3->GG_Name,$4); } TRSB
;

nodenum :
	TINT { $$ = find_node(atoi(lexname)); }
;

nodestring :
	TSTRING { strcpy( $$, lexname ); }
;

edge:
    TEDGE TLSB nodenum nodenum { add_link( $3,$4 ); } TRSB
;

%%
/* program section */

/* global variables: */
/* line number */
long Lines;

P_GG GG_Prev;

#include "gdl.c"

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
    BuiltinProperties = DataflowCount;
}/* parse_init */

/* find a node with this name */
P_GG find_node( int val ){
P_GG newgg;

    Visit_List(thisgg,GG_List,GG){
	if( thisgg->GG_Num == val ){
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
    newgg->GG_Num = val;
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

void yyerror( char *s)
{
    printf( "%s\n", s );
    parse_die();
}/* yyerror */
