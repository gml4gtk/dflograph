/* The source code in this file is hereby placed in the public domain.
 * 29 August 1996, Michael Wolfe, Oregon, USA */
extern void arglist( int argc, char* argv[] );

struct filestruct{
    struct filestruct* next;
    char* name;
};
extern struct filestruct* firstfile;
extern struct filestruct* lastfile;
extern Bool Interference;
extern Bool Postscript_Only;
extern char* postscriptfont;
extern char* postscriptfile;
extern Sint postscriptcircle;
extern Sint postscriptfontsize;
extern Sint postscriptspacing;
extern Bool postscriptscalefont;
extern Bool postscriptscaledrawing;
extern char* mainfont;
extern char* leftfont;
extern char* rightfont;
