// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// dg.h
//
// 08 Dec 94 [mw]
// $Id: dg.h,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $

// Directed Graph plotter
#ifndef Already_Included_DG
#define Already_Included_DG
#include "portable.h"
#include "newclass.h"
#include "list.h"
#include "error.h"

// data structure used by DG
// some global record type definitions
new_Class(DG);		// graph node
new_Class(Ptr_DG);      // List of DG nodes
new_Class(DG_Graph);	// Points to a Header list, a single graph
new_Class(DGtext);	// text in a text box
new_Class(DGbutton);	// button text, call-back
new_Class(DDG);		// the drawn graph nodes
new_Class(Ptr_DDG);	// List of drawn nodes
new_Class(DDG_Head);	// Header of a row of nodes
new_Class(DG_List);	// list structure
new_Class(DG_List_Item);// item in a list structure


// define the information in the DG record

// What shape to draw
typedef enum DG_Shapes{
    dg_box,	// fixed size square
    dg_circle,	// fixed size circle                      	
    dg_hex,	// fixed size hexagon                         	
    dg_diamond,	// fixed size diamond
    dg_oct,	// stop sign
    dg_text,	// text node, filled with several lines of text
    dg_nothing	// draw nothing
};// DG_Shapes

// What style of line to use
typedef enum DG_Styles{	// line styles
    dg_solid,	// default, solid line
    dg_bold,	// bold line
    dg_dashed,	// dashed line
    dg_dotted,	// dotted line
    dg_blank	// invisible blank line
};// DG_Styles


// Handler routine to call, when a node is "clicked"
typedef Uint (*DG_Handler_Proc)( P_DG_Graph graph, P_DG dgnode, Uint param );
// Handler routine to call, when a button is "clicked"
typedef Uint (*Button_Handler_Proc)( P_DG_Graph graph, Uint32 param );


// DG nodes are the REAL graph nodes.
declare_ptr_list_class(DG);
declare_list_class(DG);
declare_list_class(DGtext);
class DG{
public:
    List_Ptr_DG DG_Succs;	// list of successors
    List_Ptr_DG DG_Preds;	// list of predecessors
    P_DDG DG_Clone;		// the drawn node
    Sint DG_Level;		// what level in the graph
    P_char DG_Name;		// What to print at this node
    P_char DG_Name2;		// Second line, if any
    P_char DG_LeftName, DG_RightName;
    Bool DG_Second_Line;	// Whether there is a 2nd line?
    DG_Styles DG_Style_Top;	// highlight this one?
    DG_Styles DG_Style_Bot;	// highlight this one?
    DG_Shapes DG_Shape_Top;	// How to draw this DG node (top half)
    DG_Shapes DG_Shape_Bot;	// How to draw this DG node (bottom half)
    Uint32 DG_Color;		// Color (integer >= 0);
    DG_Handler_Proc DG_Handler;	// handler to call when this node is clicked
    Uint32 DG_Param;		// argument to pass to handler
    Bool DG_Show;		// Initially TRUE; set false to erase this node
    DG_Styles DG_Bar_Style;	// Bar across the node
    Uint32 DG_Bar_Color;	// color of bar 
    // Text stuff not implemented yet:
    Sint DG_Height;		// how tall to make this node, if a text box
    Sint DG_Width;		// how wide to make this node, in columns
    List_DGtext DG_Text;	// If this is a text box, what do we put here
    Sint32 DG_Lines;		// How many lines of Text?
    P_DG_Graph DG_Enclosing_Graph;	// the graph holding this node
    Sint DG_Extra;
    P_void operator new( size_t );
    void operator delete( P_void );
    DG( DG_Shapes );
    DG();
    in_list(DG);    		// keep a list of all DG nodes
};
declare_list_functions(DG);
inline DG::DG(){ DG_Show = 1; };
inline DG::DG( DG_Shapes shape ){
    DG_Shape_Top = shape;
    DG_Shape_Bot = shape;
    DG_Show = 1;
}// DG::DG


// Again, text stuff is not implemented:
class DGtext{
public:
    P_char Text;
    in_list(DGtext);
    P_void operator new( size_t );
    void operator delete( P_void );
};// class DGtext
declare_list_functions(DGtext);


// Types for the Drawn DG nodes
typedef enum DDG_Types{
    ddg_clone,	// clone of a real node
    ddg_way,	// dummy way-point to carry an edge between two levels
    ddg_rway,	// dummy way-point to carry a back edge between two levels
    ddg_holder,	// place holder, for multi-level text node
    ddg_bottom_corner,	// bottom corner for back edges
    ddg_top_corner,	// top corner for back edges
    ddg_both_corners	// both top and bottom corner for back edges
};

// DDG nodes are the nodes as drawn
declare_ptr_list_class(DDG);
declare_list_class(DDG);
class DDG{
public:
    DDG_Types DDG_Type;		// what type of drawn node is this?
    P_DG DDG_Clone;		// the Real node for this drawn node
    P_Ptr_DG DDG_Ptr_Clone;	// orelse, the edge for which this is a waypoint
    List_Ptr_DDG DDG_Succs;	// list of successors
    List_Ptr_DDG DDG_Preds;	// list of predecessors
    Sint32 DDG_Pred_Count;	// How many predecessors
    Sint32 DDG_Succ_Count;	// How many successors
    Sint32 DDG_Level;		// what level in the graph
    Sint32 DDG_Position;	// if 'level' is Y value, this is X value
    Sint32 DDG_XPosition;	// temporary X value
    Bool DDG_Show;		// Initially TRUE; set false to erase this node
    P_DDG Sideways_To;  	// Sideways arc from here to there
    P_DDG Sideways_From;	// Sideways arc from there to here
    P_DDG Sideways_Link;	// For targets of 'sideways_to' edges,
				// this points to first top_corner node with 
				// sideways_to link to it.  For top_corner 
				// nodes, points to another node with same
				// sideways_to link.
    Sint DDG_Top_Channel, DDG_Bot_Channel;
				// channel position to use for corners
    Sint DDG_Extra;
    Sint DDG_Connect_Left, DDG_Connect_Right;	// left/right connections
    P_DDG DDG_StringHead;	// top node in a 'string' of nodes
    Sint DDG_StringWidth;
    P_void operator new( size_t );
    void operator delete( P_void );
    DDG( P_Ptr_DG, DDG_Types, Uint level, P_DG_Graph );
    DDG( P_DG, P_DG_Graph );
    in_list(DDG);
};
declare_list_functions(DDG);

// List of graph level headers
class DDG_Head{
public:
    Sint32 Level_Width;		// maximum width
    Sint32 Level_Depth;		// how far down                           	
    List_DDG Level_List;	// list of nodes at this level
    P_void operator new( size_t );
    void operator delete( P_void );
};

// Links between nodes:
typedef enum DGL_Types{	// used internally
    dgl_normal,		// normal link, forward
    dgl_invis		// invisible link, used to MAKE successor appear below
};

typedef enum DGL_Arrows{
    dgl_end,  		// arrow at the target
    dgl_start,		// arrow at the source                                             	
    dgl_both,		// two arrows
    dgl_neither		// no arrows
};

typedef enum DDGL_Alignments{
    ddgl_none,		// no alignment preference so far
    ddgl_left, 		// line goes to the left, like "/"
    ddgl_down,		// line goes down, like "|"
    ddgl_right,		// line goes to the right, like "\"
    ddgl_conflict,	// dont know which way to go
    ddgl_last
};

inline DDGL_Alignments next( DDGL_Alignments a ){ return (DDGL_Alignments)(1+((short)a)); }

extern P_char Alignment_Names[6];
inline P_char name( DDGL_Alignments a ){ return Alignment_Names[a]; }


class Ptr_DG{
public:
    P_Ptr_DG DGL_Match;		// point to the matching succ or pred link
    DGL_Types DGL_Type;		// set this to the edge type
    DG_Styles DGL_Style;	// style of line
    Uint32 DGL_Color;		// color of line 
    P_char DGL_Text;		// text on this line
    DGL_Arrows DGL_Arrow;	// where to put the arrow, if any
    Bool DGL_Show;		// Initially TRUE; set false to erase this node
    DDGL_Alignments DGL_Align;	// Any preference for arrow alignment?
    Ptr_DG();
    Ptr_DG( P_DG ptr );
    Ptr_DG( P_Ptr_DG lptr );
    P_void operator new( size_t );
    void operator delete( P_void ptr );
    in_ptr_list(DG);
};
declare_ptr_list_functions(DG);
inline Ptr_DG::Ptr_DG(){ DGL_Show = 1;};
inline Ptr_DG::Ptr_DG( P_DG ptr ){ Link = ptr; DGL_Show = 1; };
inline Ptr_DG::Ptr_DG( P_Ptr_DG lptr ){ 
    DGL_Type = lptr->DGL_Type;
    DGL_Style = lptr->DGL_Style;
    DGL_Color = lptr->DGL_Color;
    DGL_Arrow = lptr->DGL_Arrow;
    DGL_Show = lptr->DGL_Show;
};



class Ptr_DDG{
public:
    P_Ptr_DDG DDGL_Match;	// point to the matching succ or pred link
    P_Ptr_DG DDGL_Clone;	// point to the clone succ or pred link
    Bool DDGL_Show;		// Initially TRUE; set false to erase this node
    DDGL_Alignments DDGL_Align;	// how should it be aligned.
    Ptr_DDG();
    Ptr_DDG( P_DDG ptr );
    Ptr_DDG( P_Ptr_DDG lptr );
    P_void operator new( size_t );
    void operator delete( P_void ptr );
    in_ptr_list(DDG);
};
declare_ptr_list_functions(DDG);
inline Ptr_DDG::Ptr_DDG(){ DDGL_Show = 1;};
inline Ptr_DDG::Ptr_DDG( P_DDG ptr ){ Link = ptr; DDGL_Show = 1; };


declare_list_class(DGbutton);
class DGbutton{
public:
    P_char Button_Text;
    Button_Handler_Proc Button_Handler;	// call when this button is clicked
    Uint32 Button_Param;		// argument to pass to handler
    P_DG_Graph Button_Graph;	// either this or Button_List is set.
    P_DG_List Button_List;
    P_DG_List Button_Menu;	// Menu to display when this button is pressed
    in_list(DGbutton);
    P_void operator new( size_t );
    void operator delete( P_void );
    DGbutton( P_DG_Graph GR, P_char text, Button_Handler_Proc handler, Uint32 param );
    DGbutton( P_DG_Graph GR, P_char text, P_DG_List MenuList );
    DGbutton( P_DG_List L, P_char text, Button_Handler_Proc handler, Uint32 param );
};// DGbutton
declare_list_functions(DGbutton);

// This is a handler for a button to quit the program
extern Uint DG_Quit( P_DG_Graph graph, Uint g );

// This is a handler for a button to go to the next program 
extern Uint DG_Next( P_DG_Graph graph, Uint g );

class DG_Graph{ 
public: 
    List_DG all_nodes;
    List_Ptr_DDG all_DDG;
    List_DGbutton all_buttons;
    P_DDG_Head headers;
    P_char Name;
    Uint32 Graphics;	// used to link back to the graphics context
    Sint32 DG_Old_Level_Max;	// old maximum level
    Sint32 DG_Level_Max;	// maximum level
    Sint32 DG_Width_Max;	// maximum initial width, used in weights
    Bool Tree; // A graph is a tree if every node has one at most predecessor (and no cycles) 
    Bool PTree;// or is a PTree if every node has at most one successor
    Bool Interference;	// set this to show it as an interference graph
    Uint DG_Count_Nodes;
    DG_Graph(P_char);
};

inline DDG::DDG( P_Ptr_DG link, DDG_Types t, Uint level, P_DG_Graph graph ){
    DDG_Ptr_Clone = link;
    DDG_Show = 1;
    DDG_Type = t;
    DDG_Level = level;
    insert_list_before( new Ptr_DDG( this ), graph->all_DDG, NULL );
}
inline DDG::DDG( P_DG dg, P_DG_Graph graph ){
    DDG_Type = ddg_clone;
    DDG_Clone = dg;
    dg->DG_Clone = this;
    DDG_Show = 1;
    DDG_Level = dg->DG_Level;
    insert_list_before( new Ptr_DDG( this ), graph->all_DDG, NULL );
}


// Call DG_Init to initialize all storage
extern P_DG_Graph DG_Init( P_char );

// Call DG_ReInit to reinitialize for a new graph, saving the old one
extern P_DG_Graph DG_ReInit( P_char );

// Call DG_Cleanup to reinitialize delete an old graph
extern void DG_Cleanup( void );

// Call DG_Build for each node in the graph.
extern P_DG DG_Build( P_DG_Graph graph,
		char * name = "",
		DG_Shapes shape = dg_circle,
		DG_Styles styletype = dg_solid,
		Uint color = 0,
		DG_Handler_Proc handler = 0, Uint32 param = 0);


// Call DG_Link for each link in the graph.
extern void DG_Link( P_DG from, P_DG to,
		DGL_Types linktype = dgl_normal,
		DG_Styles styletype = dg_solid,
		Uint color = 0,
		DGL_Arrows arrow = dgl_end,
		DDGL_Alignments align = ddgl_none );

extern void DG_Add_Text( P_DG dg, char * text );

// Change the type or highlight for a node
extern void DG_Change_Name( P_DG dg, P_char newname );
extern void DG_Change_Name( P_DG from, P_DG to, P_char newname );
extern void DG_Change_Name2( P_DG dg, P_char newname );
extern void DG_Change_Shape( P_DG dg, DG_Shapes shape );
extern void DG_Change_Shape( P_DG dg, DG_Shapes shapetop, DG_Shapes shapebottom );
extern void DG_Change_Shape_Top( P_DG dg, DG_Shapes shapetop );
extern void DG_Change_Shape_Bot( P_DG dg, DG_Shapes shapebottom );
extern void DG_Change_Style( P_DG dg, DG_Styles newstyle );
extern void DG_Change_Style_Top( P_DG dg, DG_Styles newstyle );
extern void DG_Change_Style_Bot( P_DG dg, DG_Styles newstyle );
extern void DG_Change_Style( P_DG from, P_DG to, DG_Styles newstyle );
extern void DG_Change_Color( P_DG dg, Uint newcolor );
extern void DG_Change_Bar( P_DG dg, DG_Styles newstyle );
extern void DG_Change_Bar( P_DG dg, Uint newcolor );
extern void DG_Hide( P_DG dg );
extern void DG_Expose( P_DG dg );
extern void DG_Expose_All( P_DG_Graph graph );

extern void DDG_Link( P_DDG from, P_DDG to, P_Ptr_DG edge );


// Call DG_Process to place the nodes and links for a pretty picture
extern void DG_Process( P_DG_Graph );
extern void DG_Process_Interference( P_DG_Graph );

extern void DG_Dump( P_DG_Graph, Bool );

// Initial left-to-right orientation, do this after some hiding
extern void Left_To_Right( P_DG_Graph GR );

// adjust initial orientation, either right-to-left (doright==1) or
// left-to-right (doright==-1)
extern void Adjust( P_DG_Graph GR, Bool doright );

// Hide a single drawn node, and all links to it
extern void DG_Hide_Node( P_DG_Graph GR, P_DG dg );
// Copy hidden bit to real nodes
extern void DG_Copy_Hidden( P_DG_Graph GR );
// Unhide all hidden drawn nodes, essentially unhides all
// nodes since last 'DG_Copy_Hidden' call
extern void DG_UnHide( P_DG_Graph GR );
// Restore all nodes, preparatory to calling 'Left_To_Right' again
extern void DG_Restore_All( P_DG_Graph GR );

declare_list_class(DG_List_Item);
class DG_List{
public:
    P_char List_Name;
    List_DGbutton List_Buttons;
    List_DG_List_Item Item_List;
    P_void operator new( size_t );
    void operator delete( P_void );
    DG_List(P_char);
}; // class DG_List

// Handler routine to call, when a list item is "clicked"
typedef Uint (*Item_Handler_Proc)( P_DG_List list, Uint32 param );
class DG_List_Item{
public:
    P_char Item_Text;
    Item_Handler_Proc Item_Handler;	// call when this button is clicked
    Uint32 Item_Param;		// argument to pass to handler
    P_DG_List List;
    in_list(DG_List_Item);
    P_void operator new( size_t );
    void operator delete( P_void );
    DG_List_Item( P_DG_List L, P_char text, Item_Handler_Proc handler, Uint32 param );
};// class DG_List_Item
declare_list_functions(DG_List_Item);

extern void DG_Display_List( P_DG_List );
extern void DG_Hide_List( P_DG_List ); 

// test routine
extern void Test( P_DG_Graph GR );
#endif
