// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// lists.h
//
// Definition and implementation of generic 'list' classes.
//
// 12 Oct 92 [mw]


#ifndef Already_Included_List
#define Already_Included_List

// A class C with the 'list' attribute has the following:
//  private data members:
//	P_C Next, Prev
//  friend functions:
//	next( P_C ), prev( P_C ), advance( P_C& ), retreat( P_C& ), more( P_C )
//  a class List_C with data members:
//	P_C First, Last
//  and friend functions:
//	first( List_C ), last( List_C )
//  and the appropriate functions:
//	insert_list_before( P_C, List_C, P_C ), 
//	insert_list_after( P_C, List_C, P_C ),
//      insert_list_begin( P_C, List_C ),		[mpg]
//      insert_list_end( P_C, List_C ),			[mpg]
//	remove_list( P_C, List_C )
//	append_list( List_C&, List_C& )
//  a loop control structure:
//	Visit_List( name, list, C )
//  where 'name' is a new name, to be declared of type 'P_C',
//  'list' is the name of a 'List_C', and 'C' is the name of a list type.
// A class Ptr_C with the 'ptr_list' attribute is a list 
// with additional attributes:
//  private data members:
//	P_C Link
//  friend functions:
//	get_link( P_Ptr_C )
//  and loop control structure:
//	Visit_Ptr_List( name, list, C )
//      & Visit_Ptr_List_Backwards( name, list, C )
//  where 'name' is a new name, declared of type 'P_C'
//  'list' is the name of a 'List_Ptr_C'


// To define a class C as having the 'list' attribute,
// add the line 'in_list(C)' in the declaration of the list:
//	class C{
//	 in_list(C);
//	 ...
//
// declare_list_functions(C) and declare_list_class(C) declare the 
// friend functions and the list class
// declare_list_class(C) should preceed the class C declaration
// declare_list_functions(C) should follow class C
// implement_list_functions(C) should be in a .C file somewhere.

// Because classes must be defined before being used, and because
// we might have a class that contains a data member of type List_D,
// where the class D contains a data member of type List_C,
// we have split up these declarations.

// in_list(C) declares data members Next, Prev,
// and the friend functions next, prev and advance
#define in_list(C) \
    P_##C Next, Prev;\
    friend P_##C next( P_##C );\
    friend P_##C prev( P_##C );\
    friend P_##C advance( RP_##C );\
    friend P_##C retreat( RP_##C );\
    friend Bool more( P_##C );\
    friend P_##C insert_list_before( P_##C, List_##C &, P_##C );\
    friend P_##C insert_list_after( P_##C, List_##C &, P_##C );\
    friend P_##C insert_list_begin( P_##C, List_##C & );\
    friend P_##C insert_list_end( P_##C, List_##C & );\
    friend P_##C remove_list( P_##C, List_##C& );\
    friend List_##C& append_list( List_##C&, List_##C& )

#define in_ptr_list(C) \
    in_list(Ptr_##C);\
    P_##C Link;\
    friend P_##C link( P_Ptr_##C );\
    friend void set_link( P_Ptr_##C ,P_##C )

// declare_list_functions(C) declares the inline 
// friend functions next, prev, ...
#define declare_list_functions(C) \
inline P_##C next( P_##C p ){\
    assert( p != NULL );\
    return p->Next;\
}\
inline P_##C prev( P_##C p ){\
    assert( p != NULL );\
    return p->Prev;\
}\
inline P_##C advance( RP_##C p ){\
    assert( p != NULL );\
    return p = p->Next;\
}\
inline P_##C retreat( RP_##C p ){\
    assert( p != NULL );\
    return p = p->Prev;\
}\
inline Bool more( P_##C p ){\
    return p != NULL;\
}

#define declare_ptr_list_functions(C) \
    declare_list_functions(Ptr_##C);\
inline P_##C link( P_Ptr_##C p ){\
    return p->Link;\
}\
inline void set_link( P_Ptr_##C p , P_##C b){\
    p -> Link = b;\
}

// declare_list_class(C) declares the class List_C,
// its data members and friend functions.

#define declare_list_class(C)	\
class List_##C;\
typedef List_##C & R_List_##C;\
class List_##C{\
    public:\
    inline void Init(){ First = NULL; Last = NULL; }\
    inline void Init( List_##C& l ){ First = l.First; Last = l.Last; }\
    private:\
    P_##C First, Last;\
    friend P_##C first( R_List_##C );\
    friend P_##C last( R_List_##C );\
    friend P_##C insert_list_before( P_##C, R_List_##C, P_##C );\
    friend P_##C insert_list_after( P_##C, R_List_##C, P_##C );\
    friend P_##C insert_list_begin( P_##C, R_List_##C );\
    friend P_##C insert_list_end( P_##C, R_List_##C );\
    friend P_##C remove_list( P_##C, R_List_##C );\
    friend List_##C& append_list( List_##C&, List_##C& );\
    friend Bool is_empty(R_List_##C);\
    friend Bool is_singular(R_List_##C);\
    friend Bool is_multiple(R_List_##C);\
};\
inline P_##C first( R_List_##C l ){\
    return l.First;\
}\
inline P_##C last( R_List_##C l ){\
    return l.Last;\
}\
inline Bool is_empty(R_List_##C l) {\
  return (l.First == NULL);\
}\
inline Bool is_singular(R_List_##C l) {\
  return (l.First == l.Last && l.First != NULL);\
}\
inline Bool is_multiple(R_List_##C l) {\
  return (l.First != l.Last);\
}

#define declare_ptr_list_class(C)	\
    declare_list_class(Ptr_##C);

#define ptr_list(C)	Ptr_##C

// macro to free all the elements of a list
#define Free_List( list, C )  \
{ P_##C Current_Element = first (list);\
  while (Current_Element != NULL) {\
    P_##C Next_Element = next (Current_Element);\
    delete Current_Element;\
    Current_Element = Next_Element;\
  }\
  list.Init();\
}

// loop to visit all elements of a list:
#define Visit_List( elt, list, C )     \
        { \
	for( P_##C elt = first( list ); more( elt ); advance( elt ) ){
#define Visit_List_Backwards( elt, list, C )     \
        { \
	for( P_##C elt = last( list ); more( elt ); retreat( elt ) ){
#define End_Visit  }}

// loop to visit all elements of a list, and to pull a link to another class
// out of the list.
#define Visit_Ptr_List( E, list, C ) \
	{ \
	for( P_Ptr_##C ptr_##E = first( list ); more( ptr_##E );\
	    advance( ptr_##E ) ){ \
	    P_##C E = link( ptr_##E );

#define Visit_Ptr_List_Backwards( E, list, C ) \
	{ \
	for( P_Ptr_##C ptr_##E = last( list ); more( ptr_##E );\
	    retreat( ptr_##E ) ){ \
	    P_##C E = link( ptr_##E );


#define implement_list_functions(C) \
P_##C insert_list_before( P_##C p, List_##C& l, P_##C b ){\
    assert( p != NULL );\
    if( b == NULL ){\
	p->Next = NULL;\
	p->Prev = l.Last;\
	l.Last = p;\
    }else{\
	p->Next = b;\
	p->Prev = b->Prev;\
	b->Prev = p;\
    }\
    if( p->Prev == NULL ){\
	l.First = p;\
    }else{\
	p->Prev->Next = p;\
    }\
    return p;\
}\
P_##C insert_list_after( P_##C p, List_##C& l, P_##C a ){\
    assert( p != NULL );\
    if( a == NULL ){\
	p->Prev = NULL;\
	p->Next = l.First;\
	l.First = p;\
    }else{\
	p->Prev = a;\
	p->Next = a->Next;\
	a->Next = p;\
    }\
    if( p->Next == NULL ){\
	l.Last = p;\
    }else{\
	p->Next->Prev = p;\
    }\
    return p;\
}\
P_##C insert_list_begin( P_##C p, List_##C& l ){\
    insert_list_after( p, l, NULL );\
    return p;\
}\
P_##C insert_list_end( P_##C p, List_##C& l ){\
    insert_list_before( p, l, NULL );\
    return p;\
}\
P_##C remove_list( P_##C p, List_##C& l ){\
    assert( p != NULL );\
    if( p->Next != NULL ){\
	p->Next->Prev = p->Prev;\
    }else{\
	l.Last = p->Prev;\
	if( p->Prev != NULL ){\
	    p->Prev->Next = NULL;\
	}\
    }\
    if( p->Prev != NULL ){\
	p->Prev->Next = p->Next;\
    }else{\
	l.First = p->Next;\
	if( p->Next != NULL ){\
	    p->Next->Prev = NULL;\
	}\
    }\
    p->Next = NULL;\
    p->Prev = NULL;\
    return p;\
}\
List_##C& append_list( List_##C& append, List_##C& tolist ){\
    if( append.First == NULL ){\
    }else if( tolist.First == NULL ){\
        tolist.First = append.First;\
        tolist.Last = append.Last;\
    }else{\
        tolist.Last->Next = append.First;\
        append.First->Prev = tolist.Last;\
        tolist.Last = append.Last;\
    }\
    return tolist;\
}

#endif
