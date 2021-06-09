/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_GGY_TAB_H_INCLUDED
# define YY_YY_GGY_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TID = 258,
    TSTRING = 259,
    TARROW = 260,
    TREVARROW = 261,
    TBOTHARROW = 262,
    TNOARROW = 263,
    TDOTARROW = 264,
    TDOTREVARROW = 265,
    TDOTBOTHARROW = 266,
    TDOTNOARROW = 267,
    TBARBAR = 268,
    TAMPAMP = 269,
    TBAR = 270,
    TUPARROW = 271,
    TAMP = 272,
    TEQUALEQUAL = 273,
    TNOTEQUAL = 274,
    TGREATEREQUAL = 275,
    TGREATER = 276,
    TLESSEQUAL = 277,
    TLESS = 278,
    TLEFTSHIFT = 279,
    TRIGHTSHIFT = 280,
    TPLUS = 281,
    TDASH = 282,
    TSTAR = 283,
    TSLASH = 284,
    TMOD = 285,
    TBANG = 286,
    TTILDE = 287,
    TCOLON = 288,
    TCOMMA = 289,
    TEQUAL = 290,
    TLP = 291,
    TRP = 292,
    TSEMI = 293,
    TDOT = 294,
    TLSB = 295,
    TRSB = 296,
    TLCB = 297,
    TRCB = 298,
    TALL = 299,
    TANY = 300,
    TPRED = 301,
    TSUCC = 302,
    TINT = 303,
    TIN = 304,
    TOUT = 305,
    TNODE = 306,
    TEDGE = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 40 "ggy.y" /* yacc.c:1921  */

    Name_Type name;
    P_DFExpression dfexp;
    P_Dataflow df;
    P_Expression exp;
    P_Var var;
    int Intval;
    

#line 121 "ggy.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GGY_TAB_H_INCLUDED  */
