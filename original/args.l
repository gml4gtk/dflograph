%{
/* The source code in this file is hereby placed in the public domain.
 * 29 August 1996, Michael Wolfe, Oregon, USA */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "portable.h"
#include "args.h"
#include "handler.h"
char* command;
int commandlen;
#undef YY_INPUT
#define YY_INPUT(buf,ret,maxsize)  (ret = my_input( buf, maxsize ))
extern int my_input( char* buf, int maxsize );
%}

nonsep	[^ \n]
digit	[0-9]
nondigit	[^0-9\n ]

%option noyywrap
%option prefix="arg"

%s SWITCH DEBUG FONT LFONT RFONT 
%s PFONT PFONTSIZE NODESIZE LINESPACING BADSWITCH PFILE

%%
<INITIAL>"-"		{ BEGIN SWITCH; }
<INITIAL>" "		{;}
\n			{ return 0; }
<INITIAL>[^-" "\n]{nonsep}*	{ 
			struct filestruct* n = (struct filestruct*)malloc( sizeof( struct filestruct ) );
			n->next = NULL;
			n->name = (char*)malloc( strlen( argtext )+1 );
			strcpy( n->name, argtext );
			if( lastfile != NULL ) lastfile->next = n;
			else firstfile = n;
			lastfile = n;
			}

<SWITCH>x		{ BEGIN DEBUG; }
<DEBUG>" "		{ ; }
<DEBUG>{nonsep}+	{ 
#ifndef NODEBUG
			  Set_Debug_String( argtext ); 
#endif
			  BEGIN INITIAL; }
<SWITCH>i		{ Interference = 1; }
<SWITCH>p=?		{ Postscript_Only = 1; BEGIN PFILE; }
<SWITCH>" "		{ BEGIN INITIAL; }
<SWITCH>f(=|" ")" "*	{ BEGIN FONT; }
<SWITCH>font(=|" ")" "*	{ BEGIN FONT; }
<SWITCH>lf(=|" ")" "*	{ BEGIN LFONT; }
<SWITCH>lfont(=|" ")" "*	{ BEGIN LFONT; }
<SWITCH>rf(=|" ")" "*	{ BEGIN RFONT; }
<SWITCH>rfont(=|" ")" "*	{ BEGIN RFONT; }
<SWITCH>Pfont(=|" ")" "*	{ BEGIN PFONT;}
<SWITCH>Pf(=|" ")" "*		{ BEGIN PFONT;}
<SWITCH>Pnodesize(=|" ")" "*	{ BEGIN NODESIZE;}
<SWITCH>Pn(=|" ")" "*		{ BEGIN NODESIZE;}
<SWITCH>Pfontsize(=|" ")" "*	{ BEGIN PFONTSIZE;}
<SWITCH>Pfs(=|" ")" "*	{ BEGIN PFONTSIZE;}
<SWITCH>Pspacing(=|" ")" "*	{ BEGIN LINESPACING;}
<SWITCH>Pls(=|" ")" "*		{ BEGIN LINESPACING;}
<SWITCH>Psp(=|" ")" "*		{ BEGIN LINESPACING;}
<SWITCH>Pscalefont	{ postscriptscalefont = 1; }
<SWITCH>Pscaledrawing	{ postscriptscaledrawing = 1; }
<SWITCH>Pscalepage	{ postscriptscaledrawing = 1; }
<SWITCH>Pscale		{ postscriptscaledrawing = 1; }
<SWITCH>.		{ BEGIN BADSWITCH; fprintf( stderr, "bad switch %s", argtext ); }
<BADSWITCH>{nonsep}*	{ BEGIN INITIAL; fprintf( stderr, "%s\n", argtext ); }
<PFILE>" "		{ BEGIN INITIAL; }
<PFILE>{nonsep}+	{ postscriptfile = (char*)malloc(strlen(argtext)+1);
			  strcpy( postscriptfile, argtext );
			  BEGIN INITIAL; 
			}
<PFONT>{nonsep}+	{ postscriptfont = (char*)malloc(strlen(argtext)+1);
			  strcpy( postscriptfont, argtext );
			  BEGIN INITIAL; 
			}
<FONT>{nonsep}+		{ mainfont = (char*)malloc(strlen(argtext)+1);
			  strcpy( mainfont, argtext );
			  BEGIN INITIAL; 
			}
<LFONT>{nonsep}+	{ leftfont = (char*)malloc(strlen(argtext)+1);
			  strcpy( leftfont, argtext );
			  BEGIN INITIAL; 
			}
<RFONT>{nonsep}+	{ rightfont = (char*)malloc(strlen(argtext)+1);
			  strcpy( rightfont, argtext );
			  BEGIN INITIAL; 
			}
<NODESIZE>{digit}+	{ postscriptcircle = atoi( argtext ); BEGIN INITIAL; }
<NODESIZE>{nondigit}{nonsep}*	{ fprintf( stderr, "illegal nodesize %s\n", argtext ); BEGIN INITIAL; }
<PFONTSIZE>{digit}+	{ postscriptfontsize = atoi( argtext ); BEGIN INITIAL; }
<PFONTSIZE>.{nonsep}*	{ fprintf( stderr, "illegal fontsize %s\n", argtext ); BEGIN INITIAL; }
<LINESPACING>{digit}+	{ postscriptspacing = atoi( argtext ); BEGIN INITIAL; }
<LINESPACING>.{nonsep}*	{ fprintf( stderr, "illegal spacing %s\n", argtext ); BEGIN INITIAL; }


%%
int my_input( char* buf, int maxsize ){
    int n = maxsize;
    if( commandlen < maxsize ) n = commandlen;
    if( n > 0 ){
	memcpy( buf, command, n );
	commandlen -= n;
	command += n;
    }
    return n;
}

void arglist( int argc, char* argv[] ){
    int i, len;
    len = 0;
    for( i = 1; i < argc; ++i ){
	len += strlen( argv[i] );
	len += 1;
    }
    len += 1;
    command = (char*)malloc( len+1 );
    command[0] = '\0';
    for( i = 1; i < argc; ++i ){
	strcat( command, argv[i] );
	strcat( command, " " );
    }
    strcat( command, "\n" );
    commandlen = strlen( command );
    if( commandlen != len ){
	fprintf( stderr, "should have allocated allocated %d bytes, not %d\n",
		commandlen, len );
    }
    arglex();
}
