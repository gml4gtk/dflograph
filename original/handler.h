// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// handler.h
//
// include file for handler routines
//
// 12 Oct 92 [mw]


#ifndef Already_Included_Handler
#define Already_Included_Handler

#ifdef NAS_ALPHA_OSF_GCC
extern "C" {
#include <setjmp.h>
}
#else
#include <setjmp.h>
#endif

class Error_Handler{
public:
    friend void save_handler( jmp_buf h );
    friend void restore_handler( jmp_buf h );
    static jmp_buf error_handler;
    static Uint error_nesting;
}; // class Error_Handler
extern jmp_buf error_handler;

inline void save_handler( jmp_buf h ){
    memcpy( h, Error_Handler::error_handler, sizeof( jmp_buf ) );
    return;
}// inline save_handler( jmp_buf h )

inline void restore_handler( jmp_buf h ){
    memcpy( Error_Handler::error_handler, h, sizeof( jmp_buf ) );
    return;
}// inline restore_handler( jmp_buf h )

// To protected regions of code:
// PROTECT{
// }HANDLE{
// };
// If there is a call to the 'Error' routine in the ERROR_PROTECT
// region of code, this will jump to the ERROR_HANDLER block.
// This block can pass the error up the 'tree' with the SIGNAL statement.
// Only one PROTECT/HANDLE block per function is allowed.

#define PROTECT	jmp_buf error_jump; int error_return; \
	++Error_Handler::error_nesting; \
	save_handler( error_jump ); \
	error_return = setjmp( Error_Handler::error_handler ); \
	if( error_return == 0 )

#define HANDLE --Error_Handler::error_nesting; \
	restore_handler( error_jump ); \
	if( error_return != 0 )


#define SIGNAL if( Error_Handler::error_nesting ) \
	longjmp( Error_Handler::error_handler, 1 ); \
	else Fatal( "No active error handler" );

#ifndef NODEBUG
extern void Set_Debug_Level( char *name, Uint level );
extern void Set_Debug_String( char *debugstring );
extern void Set_Debug_File( char *debugfilename );
#endif
#endif
