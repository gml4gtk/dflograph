// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// alloc.cc
//
// Allocation manager
//
// This is done to allow reuse of already-allocated data structures,
//   and to avoid use of malloc/free or new/delete
//
// 19 Dec 94 [mw]


#include "portable.h"
#include "standard.h"
#include "error.h"

#include "alloc.h"

//
// Alloc::Alloc
//
Alloc::Alloc( Uint element_size, Uint number_elements ){
    Next = NULL;
    Last = 0;
    Array = new char[ element_size * number_elements ];
}// Alloc::Alloc( Uint element_size, Uint number_elements )

Alloc::Alloc()
{
  return;
}// Alloc::Alloc()

//
// Alloc::~Alloc
//
Alloc::~Alloc ()
{
  if (Next != NULL) delete Next;
  delete[] Array;
  return;
}

Alloc_Header* Alloc_Header::List_Head = NULL;

//
// Alloc_Header::Alloc_Header
//
Alloc_Header::Alloc_Header(Uint element_size, Uint number_elements, P_char name,
                           Cleanup_Procedure cleanup, P_Alloc_Header* pointer,
                           Bool allocate_active_word, Bool leave_dirty,
                           Bool interproc)
{
    Size = element_size;
    if( allocate_active_word ){
        Size = Size + sizeof (Uint);
    }
    Number = number_elements;
    First = new Alloc( Size, Number );
    Curr = First;
    Count = 1;        // one block allocated
    Used = 0;
    Free = NULL;
    Name = name;
    Local_Cleanup = cleanup;
    Pointer = pointer;
    (*pointer) = this;
    List_Next = List_Head;
    List_Head = this;
    Leave_Dirty = leave_dirty;
    Active_Word = allocate_active_word;
    IP_Data = interproc;
}// Alloc_Header::Alloc_Header( Uint element_size, Uint number_elements )

Alloc_Header::Alloc_Header ()
{
  return;
}

//
// Alloc_Header::~Alloc_Header
//
Alloc_Header::~Alloc_Header ()
{
    delete First;
    // remove from linked list
    if( List_Head == this ){
        List_Head = this->List_Next;
    }else{
        for( Alloc_Header* a = List_Head; a; a = a->List_Next ){
            if( a->List_Next == this ){
                a->List_Next = this->List_Next;
            }
        }
    }
    return;
}

//
// Alloc_Header::Cleanup
//
void Alloc_Header::Cleanup(){
    Curr = First;
    Curr->Last = 0;
    Used = 0;
    Free = NULL;
    if( Local_Cleanup ) (*Local_Cleanup)();
}// Alloc_Header::Cleanup()



//
// Alloc_Header::Summary
//
void Alloc_Header::Summary(){
Uint size;

    size = sizeof( Alloc ) + Size*Number;
    Message( "%3ld %s blocks of size %ld. %d entries of %d allocated\n",
        Count, Name, size, Used, Count * Number );
}// Alloc_Header::Summary()

void Alloc_Header::Summary(Uint alloc[5]){
Uint size;
    size = sizeof( Alloc ) + Size*Number;
    alloc[0] = (Uint)Name;
    alloc[1] = size;
    alloc[2] += Count;
    alloc[3] += Used;
    alloc[4] += Count * Number;
    return;
}// Alloc_Header::Summary( Uint [5] )

P_void Alloc_Header::get_many( Uint num ){
Alloc* n;
Uint l;
Uint cnt;
P_void s;

    cnt = num;
    debug( 1, ( "allocate %ld of %s", cnt, Name ) );
    if( Curr->Last + cnt - 1 >= Number ){
      if( Curr->Next == NULL ){
	if( cnt > Number ){
	    n = new Alloc( Size, cnt );
	}else{
	    n = new Alloc( Size, Number );
	}
        if( n == NULL ){
          Alloc_Summary();
          Error( "ran out of memory allocating more %s blocks", Name );
        }
        ++Count;
        debug( 1, ( "Allocated %ld %s blocks\n", Count, Name ));
        Curr->Next = n;
      }
      Curr = Curr->Next;
      Curr->Last = 0;
    }
    l = Curr->Last;
    Curr->Last += cnt;
    s = Curr->Array+l*Size;
    memset( s, 0, Size*cnt );
    Used += cnt;
    return s;
}// P_void Alloc_Header::get_many( Uint num )

//
// Alloc_Header::put_one( P_void ptr )
//
void Alloc_Header::put_one( P_void ptr ){
  // we're just freeing the space, so no need to do a memset;
  //   but do it anyway in case we reference that deleted object
  //   later by accident
  // note that the memset implicitly does a "set_inactive(s)" for us
  memset( ptr, 0, Size );
  
  *((P_void*)ptr) = Free;        // store the 'free' list link right here
  Free = (P_void*)ptr;
//  Used--;
}// void Alloc_Header::put_one( P_void ptr )

// free up all the storage after compiling a program unit
void Alloc_Cleanup( )
{
    // Now to simulate the memory cleanup by actually creating new space ...

    P_Alloc_Header thead = Alloc_Header::List_Head;
    Alloc_Header::List_Head = NULL;

    for( Alloc_Header* a = thead; a; a= a->List_Next ){
        if( !(a->IP_Data) )
          (void) new Alloc_Header( a->Active_Word ? a->Size - 4 : a->Size,
              a->Number, a->Name,          /* Major drag! --^-- [tla 2/95] */
              a->Local_Cleanup, a->Pointer, a->Active_Word, a->Leave_Dirty,
              a->IP_Data);
        if (a->Local_Cleanup)
          (*a->Local_Cleanup) ();
    }
}// void Alloc_Cleanup( void )

// print out a summary of allocation record usage
void Alloc_Summary( void ){
    printf( "Storage Allocation Report\n" );
    for( Alloc_Header* a = Alloc_Header::List_Head; a; a = a->List_Next ){
        a->Summary();
    }
}// void Alloc_Summary( void )

// Collect summary of allocation record usage
Uint Alloc_Count( void ){
    Uint count = 0;
    for( Alloc_Header* a = Alloc_Header::List_Head; a; a = a->List_Next ){
        ++count;
    }
    return count;
}// Alloc_Count

void Alloc_Summary( Uint sumblk[][5] )
{
    Uint count = 0;
    Bool ipdata = 0;
    for( Alloc_Header* a = Alloc_Header::List_Head; a; a = a->List_Next ){
        if (a->IP_Data)
          ipdata = 1;
        else {
          a->Summary(sumblk[count]);
          count++;
	  }
    }
    for( P_Alloc_Header b = Alloc_Header::List_Head; b && ipdata;
       b = b->List_Next ) {
       if (b->IP_Data)
	 {
         b->Summary(sumblk[count]);
         count++;
         }
    }
}// Alloc_Summary
