// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
#ifndef Already_Defined_Portable_H
#define Already_Defined_Portable_H
//
// portable.h
//
// all platform-dependent, non-portable information in in this file
//
// 12 Oct 92 [mw]


// we determine our specific platform here and define canonical names
// to be used for platform-dependent code elsewhere
//

// two ways to get a list of predefines w/ gcc
//   % touch a.cc ; gcc -E -dM a.cc
//   % gcc -v ; cat {the 'specs' file pointed to by gcc -v}

// This defines types that have been shown to be portable across
//   many (but not all!) arch/compiler combinations, including compilers
//   starting with 'g', compilers on operating systems ending in 'x', and the
//   Turbo C compiler under M*-D**.
//
// Generally, 'Sint' or 'Uint' should be used, except in large data structures
// where space is at a premium.

typedef signed long int		Sint;
typedef signed char		Sint8;
typedef signed short int	Sint16;
typedef signed long int		Sint32;
typedef unsigned long int	Uint;
typedef unsigned char		Uint8;
typedef unsigned short int	Uint16;
typedef unsigned long int	Uint32;
typedef unsigned char		Bool;


// For consistency, we use P_void to mean void*, P_char to mean char*, etc
typedef void* P_void;
typedef char* P_char;
typedef const char* PC_char;
typedef char*& RP_char;

const Bool True_v = 1;
const Bool False_v = 0;

#define MIN(a,b) ((a) <= (b) ? (a) : (b))
#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#endif
