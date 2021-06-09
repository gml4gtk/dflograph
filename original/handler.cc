// The source code in this file is hereby placed in the public domain.
// 29 August 1996, Michael Wolfe, Oregon, USA
//
// handler.cc
//
// Handle Warning, Error and Fatal messages.
//
// 08 Dec 94 [mw]

// Handle error-handling (with longjmp call to "error_handler") for 
// Error messages.  A main routine that needs to handle errors 
// should set "error_handler" with a setjmp call.  This is explained 
// in the error documentation for 'error.h'
// 
// This also handles the saving and retrieving of debuglevel values.
// When a (main) routine calls Set_Debug_Level, Set_Debug_String or
// Set_Debug_File, a linked list of names and values is saved.  When
// the "debuglevel = getdebuglevel" is invoked, as described in "error.doc",
// this list is searched for that file name and the corresponding value
// returned.
// 
// "handler.h" should be included by those main routines that need to set
// "error_handler" or call the Set_Debug_... routines


#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "portable.h"
#include "error.h"
#include "handler.h"

jmp_buf Error_Handler::error_handler;
Uint Error_Handler::error_nesting;

Bool Messages_to_Stderr = 1;

// Print message and continue
void Inform( const char* format, ... ){
va_list argptr;

    if( Messages_to_Stderr ){
	va_start( argptr, format );
	vfprintf( stderr, format, argptr );
	fprintf( stderr, "\n" );
	va_end( argptr );
    }else{
	va_start( argptr, format );
	vfprintf( stdout, format, argptr );
	fprintf( stdout, "\n" );
	va_end( argptr );
    }
}// void Inform( const char* format, ... )

// Print message and continue
void Message( const char* format, ... ){
va_list argptr;

    va_start( argptr, format );
    vfprintf( stdout, format, argptr );
    fprintf( stdout, "\n" );
    va_end( argptr );
}// void Message( const char* format, ... )

// Print message and continue
void Warn( const char* format, ... ){
va_list argptr;

    if( Messages_to_Stderr ){
        va_start( argptr, format );
        vfprintf( stderr, format, argptr );
        fprintf( stderr, "\n" );
        va_end( argptr );
    }else{
        va_start( argptr, format );
        vfprintf( stdout, format, argptr );
        fprintf( stdout, "\n" );
        va_end( argptr );
    }
}// void Warn( const char* format, ... )

// Print message, jump to jump buffer.
void Error( const char* format, ... ){
va_list argptr;

    if( Messages_to_Stderr ){
        va_start( argptr, format );
        vfprintf( stderr, format, argptr );
        fprintf( stderr, "\n" );
        va_end( argptr );
    }else{
        va_start( argptr, format );
        vfprintf( stdout, format, argptr );
        fprintf( stdout, "\n" );
        va_end( argptr );
    }
    longjmp( Error_Handler::error_handler, 2 );
}// void Error( const char* format, ... )

// Print message, quit immediately.
void Fatal( const char* format, ... ){
va_list argptr;

    if( Messages_to_Stderr ){
        va_start( argptr, format );
        vfprintf( stderr, format, argptr );
        fprintf( stderr, "\n" );
        va_end( argptr );
    }else{
        va_start( argptr, format );
        vfprintf( stdout, format, argptr );
        fprintf( stdout, "\n" );
        va_end( argptr );
    }
    abort();
}// void Fatal( const char* format, ... )

#ifndef NODEBUG
class debugentry{
friend class debuglist;
private:
    char*	routinename;
    Uint	routinelevel;
    debugentry*	next;
public:
    debugentry( char *name, Uint level ) {	
	routinename = name;
	routinelevel = level;
    }
    friend void Set_Debug_Level( char* name, Uint level );
    friend Uint Get_Debug_Level( char* name );
};// class debugentry
	

static debugentry* DebugHead = NULL;

// friend to class debugentry
void Set_Debug_Level( char *name, Uint level ){
debugentry* e;
char *ch;

    // set name to all lower case
    for( ch = name; *ch != '\0'; ++ch ){
	if( isupper( *ch ) ) *ch = tolower( *ch );
    }
    for( e = DebugHead; e != NULL; e = e->next ){
	if( strcmp( e->routinename, name ) == 0 ){
	    // reset debug level
	    e->routinelevel = level;
	    debug( 1, ( "Reset debug level for %s to %ld", name, level ) );
	    return;
	}
    }
    e = new debugentry( name, level );
    e->next = DebugHead;
    DebugHead = e;
#ifndef NODEBUG
    if( strncmp( name, "handler", 7 ) == 0 ){
	debuglevel = level;
    }
#endif
    debug( 1, ( "Set debug level for %s to %ld", e->routinename, e->routinelevel ) );
}// void Set_Debug_Level( char *name, Uint level )


// string is something like file[=value][,file[=value]]...
// separate into comma-separated substrings, process each substring
// call Set_Debug_Level for each substring
void Set_Debug_String( char *debugstring ){
char *ch, *name, *value;

    debug( 1, ( "process debug string %s", debugstring ) );
    name = debugstring;
    value = NULL;
    for( ch = debugstring; *ch != '\0'; ++ch ){
	// look for equal sign, comma
	if( *ch == '=' ){
	    *ch = '\0';		// terminate the "name" string
	    value = ch+1;	// start the "value" string
	}else if( *ch == ',' ){
	    *ch = '\0';		// terminate the "value" or "name" string
	    if( value == NULL ){
		Set_Debug_Level( name, 1 );
	    }else{
		Set_Debug_Level( name, atoi( value ) );
	    }
	    value = NULL;
	    name = ch+1;
	}
    }
    if( name != NULL && *name != '\0' ){
	if( value == NULL ){
	    Set_Debug_Level( name, 1 );
	}else{
	    Set_Debug_Level( name, atoi( value ) );
	}
    }
} // void Set_Debug_String( char *debugstring )


// read lines like file=value, file=value, ...
// call Set_Debug_String for each input string.
void Set_Debug_File( char *debugfilename ){
char line[512];
FILE* debugfile;

    debug( 1, ( "process debug file %s", debugfilename ) );
    debugfile = fopen( debugfilename, "r" );
    if( debugfile == NULL ){
	Warn( "debugfile %s was not found\n", debugfile );
	return;
    }
    while( fscanf( debugfile, "%s", line ) > 0 ){
	Set_Debug_String( strdup( line ) );
    }
    fclose( debugfile );
} // void Set_Debug_File( char *debugfilename )


Uint Get_Debug_Level( char* oname ){
debugentry* e;
char *ch;
char *afterslash;
int len_to_dot, len_so_far;
char name[100];

    strcpy( name, oname );
    // set name to all lower case
    afterslash = name;
    len_to_dot = -1;
    len_so_far = 0;
    for( ch = name; *ch != '\0'; ++ch ){
	if( *ch == '/' || *ch == '\\' ){
	    afterslash = ch+1;
	}else if( *ch == '.' ){
	    len_to_dot = len_so_far;
	}
	++len_so_far;
    }
    for( e = DebugHead; e != NULL; e = e->next ){
	debug( 9, ( "compare %s to %s=%ld", name, e->routinename, e->routinelevel ) );
	if( strcmp( e->routinename, name ) == 0 ){
	    // return debug level
	    debug( 1, ( "Return level %ld for %s", e->routinelevel, name ) );
	    return e->routinelevel;
	}
	if( len_to_dot > 0 ){
	    name[len_to_dot] = '\0';
	    if( strcmp( e->routinename, name ) == 0 ){
		// return debug level
		debug( 1, ( "Return level %ld for %s", e->routinelevel, name ) );
		return e->routinelevel;
	    }
	    name[len_to_dot] = '.';
	}
	if( afterslash != name ){
	    if( strcmp( e->routinename, afterslash ) == 0 ){
		// return debug level
		debug( 1, ( "Return level %ld for %s", e->routinelevel, afterslash ) );
		return e->routinelevel;
	    }
	    if( len_to_dot > 0 ){
		name[len_to_dot] = '\0';
		if( strcmp( e->routinename, afterslash ) == 0 ){
		    // return debug level
		    debug( 1, ( "Return level %ld for %s", e->routinelevel, afterslash ) );
		    return e->routinelevel;
		}
		name[len_to_dot] = '.';
	    }
	}
    }
    debug( 5, ( "notfound: Return level 0 for %s", name ) );
    return 0;	// not found
}// Uint Get_Debug_Level( char* filename )

#endif
