// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// error.h
//
// error-handling macros
//
// 12 Oct 92 [mw]


// Message will print a simple message to stdout and continue.
// Inform will print an informational message to stderr (by default)
// and continue.
// Warn will print a warning message to stderr (by default) and continue.
// Error will print the same warning message, then jump to the "error
//   handler".  The error handler must be a location in a currently
//   active routine, set up as:
// 	#include <setjmp.h>
// 	...
// 	extern jmp_buf error_handler;
// 	...
// 	somewhere(){
// 	    if( setjmp( error_handler ) ){
// 		// If I reach here, there was a call to Error somewhere
// 		// below.  There is no way to "return" from this Error.
// 		// This can be used for such things as skipping to the
// 		// end of the current subroutine.
// 	    }else{
// 		// Normally, make all kinds of calls.  Calls to 'Error'
// 		// here will trap to the 'setjmp' call above, with non-zero
// 		// return value.
// 	    }
// 	}
// The name "error_handler" must be used.  If no error_handler was specified,
// then the program is aborted.
// 
// Fatal will print the warning message, then terminate the program
//   immediately, using "abort".
// 
// Message, Inform, Warn, Error, Fatal have the same arguments as 'printf'.
// 
// Debug output is handled by the "debug(level,(format,args))" macro.
// This include file also declares a static global variable "debuglevel."
// This will give a separate variable for each source file, for use in
// the "debug" statements.
// If "debuglevel >= level" then the output is produced via printf,
// along with the file and line number.  Note that the 'printf' args
// must be in an additional set of parentheses.  Also note that no
// "\n" is needed, since the "debug" statement automatically appends a
// newline.  I have found no classy C++ way to do this as efficiently, 
// and still be able to remove the code entirely when NODEBUG is set.
// 
// Assertions can be tested with the "assert(condition)" macro.  
// If the condition is true, execution continues.  If the condition
// turns out to be false, then a message comprising the file name,
// line number, and failed condition is printed via Error.
// 
// The "debug" and "assert" macros use the ANSI C __FILE__ and __LINE__
// macros, which give the file name (as a string) and line number, to
// identify from where the output is coming.
// 
// The "getdebuglevel" macro turns into a call to "Get_Debug_Level" with
// the current file name, to ask what debug level to use for the current
// file name.  The suggested usage is:
// 	    getdebuglevel;
// 
// If the global macro name NODEBUG is defined, 
// then "debug", "assert" and "getdebuglevel" turn into no-ops.


#ifndef Already_Included_Error
#define Already_Included_Error

extern void Message( const char* format, ... );

extern void Inform( const char* format, ... );

extern void Warn( const char* format, ... );

extern void Error( const char* format, ... );

extern void Fatal( const char* format, ... );

static Uint debuglevel = 0;

extern Bool Messages_to_Stderr;

#undef debug
#undef assert
#undef getdebuglevel
#ifndef NODEBUG
#define debug(l,o)      ((debuglevel<l) ? (void)0 : \
			((void)printf("%s(%d): ", __FILE__, __LINE__),\
			(void)printf o, (void)printf("\n")))
#define assert(a)	((a) ? (void)0 : Fatal( \
                        "File %s(%d): Assertion failed: %s\n", \
                        __FILE__, __LINE__, #a ) )
extern Uint Get_Debug_Level( char* filename );
#define getdebuglevel	debuglevel = Get_Debug_Level( __FILE__ )
#else
#define assert(a)	(void)0
#define debug(l,i)	(void)0
#define getdebuglevel
#endif
#endif
