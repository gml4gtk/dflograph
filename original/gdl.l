%{
/* The source code in this file is hereby placed in the public domain.
 * 29 August 1996, Michael Wolfe, Oregon, USA */
#define yywrap()        1
%}

%%


"#".*				{ /* ignore a comment */;  }
"["				{ return TLSB; }
"]"				{ return TRSB; }
digraph				{ return TDIGRAPH; }
edge				{ return TEDGE; }
graph				{ return TGRAPH; }
node				{ return TNODE; }

[0-9][0-9]*			{ strcpy(lexname,yytext); return TINT; }

\"[^\"\n]+\"			{ strcpy(lexname,yytext); return TSTRING; }
\"[^\"\n]+			{ strcpy(lexname,yytext); return TSTRING; }

[\t" "]				{ }

[\n]				{ ++Lines; }
%%
