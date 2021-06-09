// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// alloc.h
//
// allocation manager
//
// 17 Dec 94 [mw]

#ifndef Already_Included_Alloc
#define Already_Included_Alloc

#include "newclass.h"

// Initial sizes plus increment factors.  Simpler to play around with them
// if they are literal constants.


new_Class (Alloc);
new_Class (Alloc_Header);

// directory: GLOBAL
// manage dynamic memory allocations

//
// Alloc
//
// Really allocate a block of records at one time;  use C++ 'new'
//   allocation for Alloc data structures
//

class Alloc{
public:
    friend class Alloc_Header;
    P_Alloc Next;	// next block of entries
    Uint Last;		// index of last entry allocated
    P_char Array;	// pointer to array of entries
    Alloc( Uint element_size, Uint number_elements );
    Alloc ();
    ~Alloc ();
};// class Alloc


// Use Pointer(TYP) to declare a pointer A_TYP to an alloc_header for that type,
//  and a function PTYP(a,i) that returns a pointer to the i-th entry in 
//  the array of the Alloc structure to which 'a' points.  _APointer accounts
//  for an Active_Word field in the type [tla]

#define Alloc_Pointer(type) extern P_Alloc_Header A_##type; inline P_##type P##type( P_Alloc a, Uint n ){ return ((P_##type)(a->Array+n*(sizeof(type))));}

#define Alloc_APointer(type) extern P_Alloc_Header A_##type; inline P_##type P##type( P_Alloc a, Uint n ){ return ((P_##type)(a->Array+n*(sizeof(type)+sizeof(Uint))));}

typedef void (*Cleanup_Procedure)(void);

//
// Alloc_Header
//
class Alloc_Header{
public:
    Alloc_Header( Uint element_size, Uint number_elements, P_char name, 
		Cleanup_Procedure cleanup, P_Alloc_Header* pointer,
		Bool allocate_active_word = 0, Bool leave_dirty = 0,
		Bool interproc = 0);
    Alloc_Header();
    ~Alloc_Header ();

    Uint Size;		// size of each entry to be allocated
    Uint Number;	// number of elements to allocate at a time
    P_Alloc First;	// first allocated block of entries
    P_Alloc Curr;	// current block from which to allocate
    Uint Count;		// how many blocks allocated
    Uint Used;		// how many entries allocated in all
    P_void* Free;	// free list of entries.
    P_char Name;	// name of data structure this thing is pointing to
    P_Alloc_Header* Pointer;	// pointer to set when reallocating
    Cleanup_Procedure Local_Cleanup;
	// List of Alloc_Headers
    static P_Alloc_Header List_Head;
    P_Alloc_Header List_Next;
    Bool Leave_Dirty;	// Flag to not clean up this data structure
    Bool Active_Word;	// add an 'active' word to each entry?
    Bool IP_Data;       // Interprocedural phase data, manage differently

    void Cleanup();
    void Summary();
    void Summary( Uint alloc[4] );
    void put_one( P_void ptr );	// add to free list
    P_void get_many( Uint num );	// get many, newly allocated
};// class Alloc_Header

// call Alloc_Cleanup after each program unit to prepare for the next
void Alloc_Cleanup( );

// call Alloc_Summary to print an allocation report
void Alloc_Summary( void );
Uint Alloc_Count( void );
void Alloc_Summary( Uint sumblk[][5] );

//
// active, set_active
//
// in order to have For_Each_* work, we need to mark each object
//   in the Alloc block as "active" (used) or not (free) -- we
//   allocate one extra byte per object for this flag
//
inline static Uint *active_word(Uint Size, P_void s)
{
  return (Uint *)((char*)s+Size-sizeof (Uint)); // use last 4 bytes in object
}
inline static Bool active(Uint Size, P_void s)
{
  Uint x = *active_word(Size, s);
  debug(3,("testing active for %x + %d, returning %d\n", s, Size, x==1));
  return (x == 1);
}

inline static void set_active(Uint Size, P_void s)
{
  debug(3,("setting active for %x + %d\n", s, Size));
  Uint *x = active_word(Size, s);
  *x = 1;
}
#endif
