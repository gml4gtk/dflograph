// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// gg.cc
//
// 08 Dec 94 [mw]
// $Id: gg.cc,v 1.1 1996/07/31 22:31:17 mwolfe Exp mwolfe $
//

#include "portable.h"
#include "standard.h"
#include "error.h"

#include "dg.h"
#include "gg.h"

implement_list_functions(GG);
implement_list_functions(Ptr_GG);
implement_list_functions(Var);
implement_list_functions(Assignment);
implement_list_functions(Expression);
implement_list_functions(Dataflow);

List_GG GG_List;
List_Var Var_List;
List_Dataflow Dataflow_List;
List_Expression Expression_List;
long Vars, DataflowCount;
long BuiltinProperties;
