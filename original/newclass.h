// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
// newclass.h
// Definition of class naming macros
//
#ifndef Already_Included_NewClass
#define Already_Included_NewClass
// Naming conventions:
// x is a class (type) name
// P_x is a pointer to type x (x*)
// R_x is a reference to type x (x&)
// PC_x is a pointer to const type x (const x*)
// RC_x is a reference to const type x (const x&)
// RP_x is a reference to pointer to type x (x*&)
// Variables should generally be declared to be P_x

#define _DEF(name) class name;
#define _PDEF(name) typedef name * P_##name;
#define _RDEF(name) typedef name & R_##name;
#define _PCDEF(name) typedef const name * PC_##name;
#define _RCDEF(name) typedef const name & RC_##name;
#define _RPDEF(name) typedef name *& RP_##name;
#define new_Class(name) _DEF(name); _PDEF(name); _RDEF(name);\
	_PCDEF(name); _RCDEF(name); _RPDEF(name);
#define new_Type(name) _PDEF(name); _RDEF(name);\
	_PCDEF(name); _RCDEF(name); _RPDEF(name);


//
// get/set member access macros
//
// these macros are for creating friend functions to access members in
// classes; rather than writing 4 separate pieces,
//    .  declare friend prototype for "get"
//    .  declare friend prototype for "set"
//    .  declare friend function for "get"
//    .  declare friend function for "set"
// the new declare_friend_inline does it all in one place for you
// for examples, see below
//
#define declare_friend_inline(class,name,type,member)			\
  friend void set_##name(P_##class op, type x) { op->member = x; }	\
  friend type name(P_##class op) { return op->member; }

// same as previous, but allows assertions for union member references
#define declare_friend_inline_assert(class,name,type,member,asrt)	\
  friend void set_##name(P_##class op, type x) { assert(asrt); op->member = x; } \
  friend type name(P_##class op) { assert(asrt); return op->member; }

#endif
