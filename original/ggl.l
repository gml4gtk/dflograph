%{
/* The source code in this file is hereby placed in the public domain.
 * 29 August 1996, Michael Wolfe, Oregon, USA */
#define yywrap()        1
%}

%s STRING
%%


<INITIAL>\"				{ BEGIN STRING; }
<STRING>[^\"\n]+		{ strcpy(lexname,yytext); return(TSTRING); }
<STRING>\"			{ BEGIN INITIAL; }
<STRING>\n			{ BEGIN INITIAL; }
"->"				{ return(TARROW); }
"<-"				{ return(TREVARROW); }
"<->"				{ return(TBOTHARROW); }
"--"				{ return(TNOARROW); }
"..>"				{ return(TDOTARROW); }
"<.."				{ return(TDOTREVARROW); }
"<..>"				{ return(TDOTBOTHARROW); }
".."				{ return(TDOTNOARROW); }
":"				{ return(TCOLON); }
","				{ return(TCOMMA); }
"||"				{ return(TBARBAR); }
"&&"				{ return(TAMPAMP); }
"|"				{ return(TBAR); }
"^"				{ return(TUPARROW); }
"&"				{ return(TAMP); }
"=="				{ return(TEQUALEQUAL); }
"!="				{ return(TNOTEQUAL); }
">="				{ return(TGREATEREQUAL); }
">"				{ return(TGREATER); }
"<="				{ return(TLESSEQUAL); }
"<"				{ return(TLESS); }
"<<"				{ return(TLEFTSHIFT); }
">>"				{ return(TRIGHTSHIFT); }
"+"				{ return(TPLUS); }
"-"				{ return(TDASH); }
"*"				{ return(TSTAR); }
"/"				{ return(TSLASH); }
"%"				{ return(TMOD); }
"!"				{ return(TBANG); }
"~"				{ return(TTILDE); }
"("				{ return(TLP); }
")"				{ return(TRP); }
";"				{ return(TSEMI); }
"."				{ return(TDOT); }
"="				{ return(TEQUAL); }
"["				{ return(TLSB); }
"]"				{ return(TRSB); }
"{"				{ return(TLCB); }
"}"				{ return(TRCB); }
"all"				{ return(TALL); }
"intersection"			{ return(TALL); }
"any"				{ return(TANY); }
"union"				{ return(TANY); }
"pred"				{ return(TPRED); }
"preds"				{ return(TPRED); }
"predecessor"			{ return(TPRED); }
"predecessors"			{ return(TPRED); }
"succ"				{ return(TSUCC); }
"succs"				{ return(TSUCC); }
"successor"			{ return(TSUCC); }
"successors"			{ return(TSUCC); }
"edge"				{ return(TEDGE); }
"edges"				{ return(TEDGE); }
"node"				{ return(TNODE); }
"in"				{ return(TIN); }
"out"				{ return(TOUT); }


[0-9][0-9]*			{ strcpy(lexname,yytext); return(TINT); }
[0-9a-zA-Z][a-zA-Z0-9_]*		{ strcpy(lexname,yytext); return(TID); }
[\t" "]				{ }

[\n]				{ ++Lines; }
%%
