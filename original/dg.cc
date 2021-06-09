// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dg.cc
//
// 08 Dec 94 [mw]
// $Id: dg.cc,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $

#include "portable.h"
#include "standard.h"
#include "error.h"
#include "dg.h"
#include "list.h"
#include "alloc.h"

implement_list_functions(DG);
implement_list_functions(Ptr_DG);
implement_list_functions(DGtext);
implement_list_functions(DGbutton);
implement_list_functions(DDG);
implement_list_functions(Ptr_DDG);
implement_list_functions(DG_List_Item);

//
// Alloc
//
// Really allocate a block of records at one time;  use C++ 'new'
//   allocation for Alloc data structures
//
// the PSYM_Dim(), etc, fncs were changed to accomodate the extra
//   "active" word in each object ("making the number field negative",
//   as suggested, wouldn't work: not all objects have numbers, have
//   to worry about the "free list ptr" overwriting it, it's not an object
//   responsibility anyway, etc)
//

Alloc_Pointer(DG);
Alloc_Pointer(DDG);
Alloc_Pointer(Ptr_DG);
Alloc_Pointer(Ptr_DDG);
Alloc_Pointer(DDG_Head);
Alloc_Pointer(DGbutton);
Alloc_Pointer(DG_List);
Alloc_Pointer(DG_List_Item);


// ***************************************************

#define MakeHeader(type,name,size,cleanup,active,dirty) \
    P_Alloc_Header A_##type = new Alloc_Header(sizeof(type),size,name,cleanup,&A_##type,active,dirty);\
    P_void type::operator new(size_t s){Uint m=s/sizeof(type);\
        return A_##type->get_many(m);}\
    void type::operator delete(P_void ptr){A_##type->put_one(ptr);}
// DG records

MakeHeader(DG,"Directed Graph Nodes",100,NULL,0,1)
MakeHeader(DGtext,"DGText records",100,NULL,0,1)
MakeHeader(DDG,"Directed Graph Dummy Nodes",100,NULL,0,1)
MakeHeader(Ptr_DG,"Directed Graph Link Nodes",200,NULL,0,1)
MakeHeader(Ptr_DDG,"Directed Graph Dummy Link Nodes",100,NULL,0,1)
MakeHeader(DDG_Head,"Directed Graph Head Nodes",200,NULL,0,1)
MakeHeader(DGbutton,"DGButton",100,NULL,0,1)
MakeHeader(DG_List,"DG_List records",40,NULL,0,1)
MakeHeader(DG_List_Item,"DG List Items",200,NULL,0,1)

// *** Strings are just characters
P_Alloc_Header A_String = new Alloc_Header( 1, 5000, "DGStrings", NULL, &A_String, 0, 1 );

// A charstring is zero terminated
P_char new_CharString( P_char string ){
P_char ptr;
Uint l;

    Uint length = strlen( string );
    ptr = (P_char)A_String->get_many( length+1 );
    for( l = 0; l < length; ++l ){
	ptr[l] = string[l];
    }
    ptr[length] = '\0';
    return ptr;
}// P_void new_CharString
// ***************************************************
void DG_Cleanup( void ) {
    A_DG->Cleanup();
    A_DDG->Cleanup();
    A_Ptr_DG->Cleanup();
    A_Ptr_DDG->Cleanup();
    A_DDG_Head->Cleanup();
    A_DGtext->Cleanup();
    A_DGbutton->Cleanup();
    A_DG_List->Cleanup();
    A_DG_List_Item->Cleanup();
}// DG_Cleanup

P_DG_Graph DG_Init( P_char nam ) {
    getdebuglevel;
    DG_Cleanup();
    return new DG_Graph(nam);
}// DG_Init

P_DG_Graph DG_ReInit( P_char nam ) {
    getdebuglevel;
    return new DG_Graph(nam);
}// DG_ReInit

DG_Graph::DG_Graph(P_char nam){
    memset( this, 0, sizeof( DG_Graph ) );
    all_nodes.Init();
    all_DDG.Init();
    DG_Level_Max = 0;
    DG_Width_Max = 0;
    Name = nam;
    Tree = 1;
    PTree = 1;
}// DG_Graph::DG_Graph

// build (allocate and fill in) a DG node
P_DG DG_Build( P_DG_Graph GR, char *name, DG_Shapes shape, DG_Styles styletype,
	Uint color, DG_Handler_Proc handler, Uint32 param ){
P_DG newdg;
Uint len;

    newdg = new DG( shape );
    debug( 1, ( "allocate DG record for %s at %lx", name, newdg ) );
    if( name != NULL ){
    	newdg->DG_Name = new_CharString( name );
    }
    newdg->DG_Color = color;
    newdg->DG_Style_Top = styletype;
    newdg->DG_Style_Bot = styletype;
    newdg->DG_Bar_Style = dg_blank;
    newdg->DG_Bar_Color = 0;
    newdg->DG_Handler = handler;
    newdg->DG_Param = param;
    newdg->DG_Enclosing_Graph = GR;
    insert_list_end( newdg, GR->all_nodes );
    return newdg;
}// DG_Build

void DG_Add_Text( P_DG dg, P_char text ){
    if( dg != NULL && text != NULL ){
	P_DGtext t = new DGtext;
	t->Text = new_CharString( text );
	insert_list_end( t, dg->DG_Text );
        ++(dg->DG_Lines);
    }
}// DG_Add_Text

void DG_Change_Name( P_DG dg, P_char newname ){
    if( newname == NULL ){
	dg->DG_Name = NULL;
    }else{
	dg->DG_Name = new_CharString( newname );
    }
}// DG_Change_Name

void DG_Change_Name( P_DG from, P_DG to, P_char newname ){
    // change name in a link from 'from' to 'to'
    if( from == NULL || to == NULL ) return;
    // look at successor links from 'from' to 'to', and change their text.
    Visit_List( edge, from->DG_Succs, Ptr_DG ){
	P_DG other = link( edge );
	if( other == to ){
	    if( newname == NULL ){
		edge->DGL_Text = NULL;
	    }else{
		edge->DGL_Text = new_CharString( newname );
	    }
	}
    }End_Visit;
    // same for predecessor links from 'to' to 'from'
    Visit_List( edge, to->DG_Preds, Ptr_DG ){
	P_DG other = link( edge );
	if( other == from ){
	    if( newname == NULL ){
		edge->DGL_Text = NULL;
	    }else{
		edge->DGL_Text = new_CharString( newname );
	    }
	}
    }End_Visit;
}// DG_Change_Name

void DG_Change_Name2( P_DG dg, P_char newname ){
    if( newname == NULL ){
	dg->DG_Name2 = NULL;
	dg->DG_Second_Line = 0;
    }else{
	dg->DG_Name2 = new_CharString( newname );
	dg->DG_Second_Line = 1;
    }
}// DG_Change_Name2

void DG_Change_Shape( P_DG dg, DG_Shapes newshape ){
    if( dg != NULL ){
	dg->DG_Shape_Top = newshape;
	dg->DG_Shape_Bot = newshape;
    }
}// DG_Change_Shape

void DG_Change_Shape( P_DG dg, DG_Shapes newshapetop, DG_Shapes newshapebot ){
    if( dg != NULL ){
	dg->DG_Shape_Top = newshapetop;
	dg->DG_Shape_Bot = newshapebot;
    }
}// DG_Change_Shape

void DG_Change_Shape_Top( P_DG dg, DG_Shapes newshapetop ){
    if( dg != NULL ){
	dg->DG_Shape_Top = newshapetop;
    }
}// DG_Change_Shape_Top

void DG_Change_Shape_Bot( P_DG dg, DG_Shapes newshapebot ){
    if( dg != NULL ){
	dg->DG_Shape_Bot = newshapebot;
    }
}// DG_Change_Shape_Bot

void DG_Change_Style( P_DG dg, DG_Styles newstyle ){
    if( dg != NULL ){
	dg->DG_Style_Top = newstyle;
	dg->DG_Style_Bot = newstyle;
    }
}// DG_Change_Style

void DG_Change_Style_Top( P_DG dg, DG_Styles newstyle ){
    if( dg != NULL ){
	dg->DG_Style_Top = newstyle;
    }
}// DG_Change_Style_Top

void DG_Change_Style_Bot( P_DG dg, DG_Styles newstyle ){
    if( dg != NULL ){
	dg->DG_Style_Bot = newstyle;
    }
}// DG_Change_Style_Bot

void DG_Change_Style( P_DG from, P_DG to, DG_Styles newstyle ){
    // change style of a link from 'from' to 'to'
    if( from == NULL || to == NULL ) return;
    // look at successor links from 'from' to 'to', and change their style.
    Visit_List( edge, from->DG_Succs, Ptr_DG ){
	P_DG other = link( edge );
	if( other == to ){
	    edge->DGL_Style = newstyle;
	}
    }End_Visit;
    // same for predecessor links from 'to' to 'from'
    Visit_List( edge, to->DG_Preds, Ptr_DG ){
	P_DG other = link( edge );
	if( other == from ){
	    edge->DGL_Style = newstyle;
	}
    }End_Visit;
}// DG_Change_Style

void DG_Change_Color( P_DG dg, Uint newcolor ){
    if( dg != NULL ) dg->DG_Color = newcolor;
}// DG_Change_Color

void DG_Change_Bar( P_DG dg, Uint newcolor ){
    if( dg != NULL ) dg->DG_Bar_Color = newcolor;
}// DG_Change_Bar

void DG_Change_Bar( P_DG dg, DG_Styles newstyle ){
    if( dg != NULL ) dg->DG_Bar_Style = newstyle;
}// DG_Change_Bar

// set up predecessor/successor links
void DG_Link( P_DG from, P_DG to, DGL_Types linktype, DG_Styles styletype, 
    Uint color, DGL_Arrows arrow, DDGL_Alignments align ){
    P_Ptr_DG news, newp;
    if( from->DG_Enclosing_Graph != to->DG_Enclosing_Graph ){
	printf( "Trying to link nodes from different graph\n" );
	return;
    }
    // First, see if there is already a link from 'from' to 'to'
    Visit_Ptr_List( succ, from->DG_Succs, DG ){
	if( succ == to ){
	    debug( 1, ( "already have a link from node %s to node %s", from->DG_Name, to->DG_Name ));
	    return;
	}
    }End_Visit;
    news = new Ptr_DG();
    newp = new Ptr_DG();
    news->Link = to;
    news->DGL_Match = newp;
    news->DGL_Type = linktype;
    news->DGL_Style = styletype;
    news->DGL_Color = color;
    news->DGL_Arrow = arrow;
    news->DGL_Align = align;
    insert_list_end( news, from->DG_Succs );
    // Is this a PTREE?  It is not if there is more than one successor
    if( next( first( from->DG_Succs ) ) != NULL ){
	from->DG_Enclosing_Graph->PTree = 0;
    }

    newp->Link = from;
    newp->DGL_Match = news;
    newp->DGL_Type = linktype;
    newp->DGL_Style = styletype;
    newp->DGL_Color = color;
    newp->DGL_Arrow = arrow;
    newp->DGL_Align = align;
    insert_list_end( newp, to->DG_Preds );
    // Is this a TREE?  It is not if there is more than one predecessor
    if( next( first( to->DG_Preds ) ) != NULL ){
	to->DG_Enclosing_Graph->Tree = 0;
    }
    debug( 1, ( "link node %s to node %s", from->DG_Name, to->DG_Name ));
    if( debuglevel ){
	if( linktype == dgl_invis ) printf ("    (invisible link)\n" );
    }
}// DG_Link


// Hide this node, and all connected edges.
void DG_Hide( P_DG dg ){
    dg->DG_Show = 0;
    Visit_List( succlink, dg->DG_Succs, Ptr_DG ){
	succlink->DGL_Show = 0;
	succlink->DGL_Match->DGL_Show = 0;
    }End_Visit;
    Visit_List( predlink, dg->DG_Preds, Ptr_DG ){
	predlink->DGL_Show = 0;
	predlink->DGL_Match->DGL_Show = 0;
    }End_Visit;
}// DG_Hide

// Expose this node, and all edges connected to it that go
// to exposed nodes.
void DG_Expose( P_DG dg ){
    dg->DG_Show = 1;
    Visit_List( succlink, dg->DG_Succs, Ptr_DG ){
	// See if this is connected to an exposed node
	P_DG succ = link( succlink );
	if( succ->DG_Show ){
	    succlink->DGL_Show = 1;
            succlink->DGL_Match->DGL_Show = 1;
        }
    }End_Visit;
    Visit_List( predlink, dg->DG_Preds, Ptr_DG ){
	// See if this is connected to an exposed node
	P_DG pred = link( predlink );
	if( pred->DG_Show ){
	    predlink->DGL_Show = 1;
	    predlink->DGL_Match->DGL_Show = 1;
        }
    }End_Visit;
}// DG_Expose

void DG_Expose_All( P_DG_Graph graph ){
    Visit_List( dg, graph->all_nodes, DG ){
	DG_Expose( dg );
    }End_Visit;
}// DG_Expose_All


// Insert links for DDG nodes
void DDG_Link( P_DDG from, P_DDG to, P_Ptr_DG edge ){
P_Ptr_DDG news, newp;

    news = new Ptr_DDG();
    newp = new Ptr_DDG();

    news->Link = to;
    news->DDGL_Match = newp;
    news->DDGL_Clone = edge;
    insert_list_end( news, from->DDG_Succs );

    newp->Link = from;
    newp->DDGL_Match = news;
    newp->DDGL_Clone = edge;
    insert_list_end( newp, to->DDG_Preds );

    ++(to->DDG_Pred_Count);
    ++(from->DDG_Succ_Count);

}// DDG_Link

DGbutton::DGbutton( P_DG_Graph GR, P_char text, Button_Handler_Proc handler, Uint32 param ){
    Button_Text = new_CharString( text );
    Button_Handler = handler;
    Button_Param = param;
    Button_Graph = GR;
    insert_list_end( this, GR->all_buttons );
}// DGbutton::DGbutton

DGbutton::DGbutton( P_DG_Graph GR, P_char text, P_DG_List MenuList ){
    Button_Text = new_CharString( text );
    Button_Menu = MenuList;
    Button_Graph = GR;
    insert_list_end( this, GR->all_buttons );
}// DGbutton::DGbutton

DGbutton::DGbutton( P_DG_List L, P_char text, Button_Handler_Proc handler, Uint32 param ){
    Button_Text = new_CharString( text );
    Button_Handler = handler;
    Button_Param = param;
    Button_List = L;
    insert_list_end( this, L->List_Buttons );
}// DGbutton::DGbutton

// Build a list
DG_List::DG_List( char *name ){
    List_Name = name;
    List_Buttons.Init();
    Item_List.Init();
}// DG_List::DG_List

DG_List_Item::DG_List_Item( P_DG_List L, P_char text, Item_Handler_Proc handler, Uint32 param ){
    Item_Text = new_CharString( text );
    Item_Handler = handler;
    Item_Param = param;
    List = L;
    insert_list_end( this, L->Item_List );
}// DG_List_Item::DG_List_Item

P_char Alignment_Names[6] = {
	"none",
	"Left",
	"Down",
	"Right",
	"Conflict",
	 "...."  };
