// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// standard.h
//
// This points to the standard include files for:
// 	string routines: strcmp, strcpy, memcpy, ...
// 	standard routines: malloc, calloc, exit, ...
// 	io routines: open, close, ...
//
// 12 Oct 92 [mw]
//

#if defined(__GNUC__)
#include <stdio.h>
#include <stdlib.h>
// memset:
#include <memory.h>
#include <string.h>

#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#endif
