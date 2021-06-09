// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// gg.h
//
// 08 Dec 94 [mw]
// $Id: gg.h,v 1.2 1996/08/28 21:13:32 mwolfe Exp mwolfe $


// data structure used by GG
// some global record type definitions
#ifndef Already_Included_GG
#define Already_Included_GG

#include "newclass.h"
#include "dg.h"
new_Class(GG);
new_Class(Ptr_GG);
new_Class(Var);
new_Class(Assignment);
new_Class(Expression);
new_Class(Dataflow);
new_Class(DFExpression);

#define Name_Len	100
typedef char Name_Type[Name_Len];

// define the information in the Var record
extern long Vars;
declare_list_class(Var);
class Var{
public:
    Name_Type Name;
    long Num;
    in_list(Var);
};
declare_list_functions(Var);

enum ExType{
	exOrElse,
	exAndThen,
	exOr,
	exXor,
	exAnd,
	exEqual,
	exNotEqual,
	exGreaterEqual,
	exGreater,
	exLessEqual,
	exLess,
	exLeftShift,
	exRightShift,
	exAdd,
	exSub,
	exMul,
	exMod,
	exDiv,
	exNot,
	exCompl,
	exNeg,
	exDeref,
	exConst,
	exVar
}; /* ExType */

declare_list_class(Expression);
class Expression{
public:
    ExType OP;
    P_Expression Left, Right;
    P_Var Operand;
    in_list(Expression);
};
declare_list_functions(Expression);

declare_list_class(Assignment);
class Assignment{
public:
    P_Var LHS;
    P_Expression RHS;
    in_list(Assignment);
};
declare_list_functions(Assignment);

extern long DataflowCount;
declare_list_class(Dataflow);
class Dataflow{
public:
    Name_Type Name;
    long Position;	// bit position in bit vector
    int Initially, Boundary;	// initial value, boundary value
    int Direction;	// +1 = forward, -1 = backward, 0 = not dataflow
    int EdgeNode;	// 1 = edge property, 2 = node property, 3 = both
    P_DFExpression expression[4]; // 0 - not in/out, 1 = in, 2 = out, 4 = edge
    in_list(Dataflow);
};
declare_list_functions(Dataflow);

enum operatortype{
    opAnd,
    opOr,
    opNot,
    opAllPredEdges,
    opAllSuccEdges,
    opAnyPredEdges,
    opAnySuccEdges,
    opPredNode,
    opSuccNode,
    opAllPred,
    opAnyPred,
    opAllSucc,
    opAnySucc,
    opValue,
    opInValue,
    opOutValue,
    opTrans,
    opTransUp,
    opTransDown,
    opDXP,
    opUXP
};

class DFExpression{
public:
    operatortype OP;
    P_DFExpression Left, Right;
    P_Dataflow Operand;
};


// define the information in the GG record

declare_ptr_list_class(GG);
declare_list_class(GG);
class GG{
public:
    List_Ptr_GG GG_Succs, GG_Preds;
    Name_Type GG_Name;	// What to print at this node
    long GG_Num;
    long GG_Number;		// tree number
    long Lowlink;
    short InStack;
    P_GG Stack_Next;
    P_DG DG_Pointer;
    DG_Shapes ggtype;
    List_Assignment Assignments;
    Uint32 Value[3];	// dataflow values
    in_list(GG);
    };
declare_list_functions(GG);

// link records

class ptr_list(GG){
public:
    P_Ptr_GG GGL_Match;	// point to the matching succ or pred link
    Bool GGL_IN_Tree;	// set to '1' if this is a 'tree' arc
    DGL_Arrows ggarrow;
    DG_Styles ggstyle;
    Name_Type GG_EdgeName;
    Uint32 Value;
    in_ptr_list(GG);
    };
declare_ptr_list_functions(GG);


extern List_GG GG_List;
extern List_Var Var_List;
extern List_Dataflow Dataflow_List;
extern List_Expression Expression_List;
extern long GG_Nodes, GG_Links;
extern long BuiltinProperties;

extern void GG_Dump( void );

extern void parse_init();

#endif
