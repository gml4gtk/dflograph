/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 11 "ggy.y" /* yacc.c:337  */

#include "standard.h"
#include "dg.h"
#include "gg.h"
int yylex();
void yyerror( char *s);
void add_link( P_GG from, P_GG to );
P_GG find_node( );
P_Var find_id( );
void add_assignment( P_GG node, P_Var lhs, P_Expression rhs );
void parse_init( );
void parse_die( );
void addedgelabel();
int intval();
P_Dataflow find_dataflow( );
P_DFExpression makeexp( operatortype op, P_Dataflow operand );
P_DFExpression makebinary( operatortype op, P_DFExpression left, 
	    P_DFExpression right );
P_Expression newexpr( ExType op, P_Var operand );
P_Expression newexpr( P_Expression left, ExType op, P_Expression right );

P_GG from_node, to_node, global_node;
P_Var global_id;
DGL_Arrows arrowtype;
DG_Styles arrowstyle;
P_Dataflow DF_LHS;


#line 99 "ggy.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "ggy.tab.h".  */
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
#line 40 "ggy.y" /* yacc.c:352  */

    Name_Type name;
    P_DFExpression dfexp;
    P_Dataflow df;
    P_Expression exp;
    P_Var var;
    int Intval;
    

#line 205 "ggy.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GGY_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  23
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   219

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  146
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  232

#define YYUNDEFTOK  2
#define YYMAXUTOK   307

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    97,    97,   100,   102,   104,   107,   107,   107,   110,
     111,   112,   112,   113,   113,   114,   114,   115,   115,   116,
     117,   117,   118,   118,   119,   119,   120,   121,   122,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     140,   139,   147,   146,   156,   155,   163,   162,   170,   171,
     172,   174,   176,   180,   186,   186,   188,   188,   192,   193,
     196,   197,   198,   202,   205,   208,   209,   212,   213,   215,
     216,   218,   219,   221,   222,   224,   225,   227,   228,   230,
     231,   232,   234,   235,   236,   237,   238,   240,   241,   242,
     244,   245,   246,   248,   249,   250,   251,   253,   254,   255,
     256,   257,   258,   260,   261,   262,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     282,   283,   287,   288,   292,   293,   297,   299,   300,   303,
     304,   303,   319,   322,   323,   324,   328,   329,   330,   333,
     334,   335,   339,   340,   343,   344,   345
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TID", "TSTRING", "TARROW", "TREVARROW",
  "TBOTHARROW", "TNOARROW", "TDOTARROW", "TDOTREVARROW", "TDOTBOTHARROW",
  "TDOTNOARROW", "TBARBAR", "TAMPAMP", "TBAR", "TUPARROW", "TAMP",
  "TEQUALEQUAL", "TNOTEQUAL", "TGREATEREQUAL", "TGREATER", "TLESSEQUAL",
  "TLESS", "TLEFTSHIFT", "TRIGHTSHIFT", "TPLUS", "TDASH", "TSTAR",
  "TSLASH", "TMOD", "TBANG", "TTILDE", "TCOLON", "TCOMMA", "TEQUAL", "TLP",
  "TRP", "TSEMI", "TDOT", "TLSB", "TRSB", "TLCB", "TRCB", "TALL", "TANY",
  "TPRED", "TSUCC", "TINT", "TIN", "TOUT", "TNODE", "TEDGE", "$accept",
  "graph", "things", "thing", "link", "$@1", "$@2", "$@3", "$@4", "$@5",
  "$@6", "$@7", "linksep", "from", "$@8", "$@9", "to", "$@10", "$@11",
  "edgelabel", "attributes", "list", "$@12", "$@13", "stufflist", "stuff",
  "stuffing", "lhsid", "lhs", "rhs", "rhs0", "rhs10", "rhs20", "rhs30",
  "rhs40", "rhs50", "rhs60", "rhs70", "rhs80", "rhs90", "rhs100", "rhs110",
  "atom", "factor", "term", "expression", "equationlist", "eqlist",
  "equation", "$@14", "$@15", "equationid", "nodeexpr", "inout", "initial",
  "value", "direction", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307
};
# endif

#define YYPACT_NINF -65

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-65)))

#define YYTABLE_NINF -57

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      50,    -7,    18,    22,    -5,    37,    53,   -65,   -65,   152,
     -65,   -65,    51,    32,   -65,   -65,   160,   -65,   -31,   -65,
     -65,    51,    45,   -65,    50,   -65,    20,   -65,   -65,   -65,
     -65,   -65,   -65,   -65,    20,    20,    20,    56,   -65,     1,
     -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,
     -65,    20,    22,   -65,    31,   -65,     1,   -65,   -65,   -65,
     -65,    20,    20,    20,    20,    20,    20,    20,   -65,   -65,
     -65,   -65,   -65,   -65,    49,   -18,   -65,    96,    52,   -15,
     -65,   -39,    42,    70,    51,    51,   -65,   -65,   -65,   -65,
     -65,   -65,   -65,   -65,     1,     1,   -65,   -65,    21,    21,
      21,    21,    21,    52,   -65,   -65,   138,   139,   140,   168,
     175,    78,     8,    84,    76,    11,   -65,   -65,    10,   -65,
     -65,   -65,   -65,   -65,    90,   -65,   -65,   -65,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,   -11,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,   -65,   -65,   -65,   158,   -65,   -65,
     -65,   -65,   139,   140,   168,   175,    78,     8,     8,    84,
      84,    84,    84,    76,    76,    11,    11,   -65,   -65,   -65,
     -65,   156,   149,   163,   -65,   -26,    68,   149,   -65,    74,
      68,    77,   102,   148,   150,   -65,   -65,   172,   176,   164,
     165,   -65,   -20,    -2,     2,    15,    29,    74,    74,    68,
      68,   128,   -65,   -65,    74,   -65,    74,   -65,    74,   -65,
      74,   -65,   -65,   -65,   -65,   172,   -65,   -65,   -65,   -65,
     -65,   -65
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    40,     0,     0,    42,     0,     2,     3,     6,     0,
       7,     8,    51,     0,    40,    42,     0,   132,     0,   127,
     129,    51,     0,     1,     0,     4,     0,    20,    22,    24,
      11,    13,    15,    17,     0,     0,     0,    52,    41,    58,
      30,    33,    34,    35,    36,    37,    38,    39,    31,    32,
      29,     0,     0,   126,   138,    43,    58,     5,    44,    46,
      19,     0,     0,     0,     0,     0,     0,     0,    26,    27,
      28,    53,    64,    66,     0,    59,    60,     0,    68,     0,
     128,     0,   135,     0,    51,    51,    21,    23,    25,    12,
      14,    16,    18,    55,     0,     0,    65,   103,     0,     0,
       0,     0,     0,     0,   104,    63,    67,    69,    71,    73,
      75,    77,    79,    82,    87,    90,    93,    97,     0,     9,
     136,   137,   133,   134,   146,    57,    45,    47,    62,    61,
     100,   101,   102,    98,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    50,    49,     0,   144,   145,
     130,   105,    70,    72,    74,    76,    78,    80,    81,    83,
      84,    85,    86,    88,    89,    91,    92,    94,    96,    95,
      10,   139,   142,     0,   143,     0,     0,   142,   140,     0,
       0,     0,     0,     0,     0,   120,   122,   124,   131,   106,
       0,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   141,   119,     0,   109,     0,   114,     0,   110,
       0,   115,   113,   118,   123,   125,   107,   108,   111,   116,
     112,   117
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -65,   -65,   -65,     3,   -65,   -65,   -65,   -65,   -65,   -65,
     -65,   -65,   -65,   202,   -65,   -65,    71,   -65,   -65,   -65,
     -21,   -65,   -65,   -65,   151,   -65,    86,   -65,   -65,   -65,
     105,    69,    72,    73,    75,    79,    41,    30,    43,    39,
      -6,    28,   -64,     4,     5,    26,   -65,   -65,   166,   -65,
     -65,     0,   -65,   -65,   -65,    23,   -65
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    64,    65,    66,    67,    61,
      62,    63,    51,     9,    12,    21,    60,    84,    85,   157,
      38,    10,    13,    22,    74,    75,    76,    77,    78,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   195,   196,   197,   198,    11,    18,    19,    54,
     181,   199,   124,    82,   183,   185,   160
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,    17,   136,    20,    72,    17,   210,    52,   187,    25,
     120,   121,    53,   154,   155,   188,    94,   213,    17,   118,
      95,    14,   119,    58,    97,    17,   161,    57,   143,   144,
     145,   146,    17,   -54,   190,   -56,    73,    23,   190,   151,
     152,   153,   191,   192,   193,   194,   191,   192,   193,   194,
     214,   190,    20,     1,   216,    97,     1,   103,   156,   191,
     192,   193,   194,   126,   127,   190,    15,   218,    59,   104,
      81,    17,    39,   191,   192,   193,   194,    17,    98,    99,
     100,   220,    37,   101,   102,    56,     2,    71,   103,     2,
      93,    24,     3,   122,   123,     3,   141,   142,     4,   189,
     104,     4,   149,   150,   190,    68,    69,    70,   147,   148,
     190,   125,   191,   192,   193,   194,   158,   159,   191,   192,
     193,   194,    79,   203,   204,   201,   130,   131,   132,   133,
     134,    96,    86,    87,    88,    89,    90,    91,    92,   215,
     217,   219,   221,   222,   223,   177,   178,   179,   205,   206,
     228,   136,   229,   137,   230,   138,   231,    26,    27,    28,
      29,    30,    31,    32,    33,    40,    41,    42,    43,    44,
      45,    46,    47,   169,   170,   171,   172,   226,   227,    34,
     128,   129,   167,   168,   139,    35,    36,    48,   175,   176,
     173,   174,   140,    49,    50,   180,   182,   184,   186,   207,
     209,   208,   210,   211,    16,   162,   212,    83,   135,   163,
     200,   164,     0,   224,   165,   225,   202,     0,    80,   166
};

static const yytype_int16 yycheck[] =
{
      21,     3,    13,     3,     3,     3,    26,    38,    34,     6,
      49,    50,    43,     3,     4,    41,    34,    37,     3,    34,
      38,     3,    37,     3,     3,     3,    37,    24,    20,    21,
      22,    23,     3,    40,    36,    40,    35,     0,    36,    28,
      29,    30,    44,    45,    46,    47,    44,    45,    46,    47,
      52,    36,    52,     3,    52,     3,     3,    36,    48,    44,
      45,    46,    47,    84,    85,    36,    48,    52,    48,    48,
      39,     3,    40,    44,    45,    46,    47,     3,    26,    27,
      28,    52,    31,    31,    32,    40,    36,    31,    36,    36,
      41,    38,    42,    51,    52,    42,    18,    19,    48,    31,
      48,    48,    26,    27,    36,    34,    35,    36,    24,    25,
      36,    41,    44,    45,    46,    47,    26,    27,    44,    45,
      46,    47,    51,    46,    47,   189,    98,    99,   100,   101,
     102,    35,    61,    62,    63,    64,    65,    66,    67,   203,
     204,   205,   206,   207,   208,   151,   152,   153,    46,    47,
     214,    13,   216,    14,   218,    15,   220,     5,     6,     7,
       8,     9,    10,    11,    12,     5,     6,     7,     8,     9,
      10,    11,    12,   143,   144,   145,   146,    49,    50,    27,
      94,    95,   141,   142,    16,    33,    34,    27,   149,   150,
     147,   148,    17,    33,    34,    37,    40,    48,    35,    51,
      28,    51,    26,    39,     2,   136,    41,    56,   103,   137,
     187,   138,    -1,   209,   139,   210,   190,    -1,    52,   140
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    36,    42,    48,    54,    55,    56,    57,    66,
      74,    99,    67,    75,     3,    48,    66,     3,   100,   101,
     104,    68,    76,     0,    38,    56,     5,     6,     7,     8,
       9,    10,    11,    12,    27,    33,    34,    31,    73,    40,
       5,     6,     7,     8,     9,    10,    11,    12,    27,    33,
      34,    65,    38,    43,   102,    73,    40,    56,     3,    48,
      69,    62,    63,    64,    58,    59,    60,    61,    69,    69,
      69,    31,     3,    35,    77,    78,    79,    80,    81,    69,
     101,    39,   106,    77,    70,    71,    69,    69,    69,    69,
      69,    69,    69,    41,    34,    38,    35,     3,    26,    27,
      28,    31,    32,    36,    48,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    34,    37,
      49,    50,    51,    52,   105,    41,    73,    73,    79,    79,
      94,    94,    94,    94,    94,    83,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,     3,     4,    48,    72,    26,    27,
     109,    37,    84,    85,    86,    87,    88,    89,    89,    90,
      90,    90,    90,    91,    91,    92,    92,    93,    93,    93,
      37,   103,    40,   107,    48,   108,    35,    34,    41,    31,
      36,    44,    45,    46,    47,    95,    96,    97,    98,   104,
     108,    95,    98,    46,    47,    46,    47,    51,    51,    28,
      26,    39,    41,    37,    52,    95,    52,    95,    52,    95,
      52,    95,    95,    95,    96,    97,    49,    50,    95,    95,
      95,    95
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    53,    54,    55,    55,    55,    56,    56,    56,    57,
      57,    58,    57,    59,    57,    60,    57,    61,    57,    57,
      62,    57,    63,    57,    64,    57,    57,    57,    57,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      67,    66,    68,    66,    70,    69,    71,    69,    72,    72,
      72,    73,    73,    73,    75,    74,    76,    74,    77,    77,
      78,    78,    78,    79,    80,    81,    81,    82,    82,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    88,    89,    89,    89,    89,    89,    90,    90,    90,
      91,    91,    91,    92,    92,    92,    92,    93,    93,    93,
      93,    93,    93,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      96,    96,    97,    97,    98,    98,    99,   100,   100,   102,
     103,   101,   104,   105,   105,   105,   106,   106,   106,   107,
     107,   107,   108,   108,   109,   109,   109
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     3,     1,     1,     1,     5,
       7,     0,     4,     0,     4,     0,     4,     0,     4,     3,
       0,     4,     0,     4,     0,     4,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     0,     3,     0,     3,     0,     3,     1,     1,
       1,     0,     1,     2,     0,     5,     0,     5,     0,     1,
       1,     3,     3,     2,     1,     2,     1,     1,     0,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     2,
       2,     2,     2,     1,     1,     3,     1,     3,     3,     3,
       3,     4,     4,     3,     3,     3,     4,     4,     3,     3,
       1,     2,     1,     3,     1,     3,     3,     1,     3,     0,
       0,     9,     1,     1,     1,     0,     2,     2,     0,     0,
       3,     5,     0,     1,     1,     1,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 11:
#line 112 "ggy.y" /* yacc.c:1652  */
    {arrowstyle = dg_dashed; }
#line 1472 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 113 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_start; arrowstyle = dg_dashed; }
#line 1478 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 114 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_both; arrowstyle = dg_dashed; }
#line 1484 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 115 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_neither; arrowstyle = dg_dashed; }
#line 1490 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 117 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_start }
#line 1496 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 118 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_both }
#line 1502 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 119 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_neither }
#line 1508 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 126 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_end; }
#line 1514 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 127 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_end; }
#line 1520 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 128 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_end; }
#line 1526 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 129 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_end; }
#line 1532 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 130 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_start }
#line 1538 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 131 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_both }
#line 1544 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 132 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_neither }
#line 1550 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 133 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_end; arrowstyle = dg_dashed; }
#line 1556 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 134 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_start; arrowstyle = dg_dashed; }
#line 1562 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 135 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_both; arrowstyle = dg_dashed; }
#line 1568 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 136 "ggy.y" /* yacc.c:1652  */
    { arrowtype = dgl_neither; arrowstyle = dg_dashed; }
#line 1574 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 140 "ggy.y" /* yacc.c:1652  */
    {
		    arrowtype = dgl_end; arrowstyle = dg_solid;
		    global_node = from_node = find_node();
		}
#line 1583 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 147 "ggy.y" /* yacc.c:1652  */
    {
		    arrowtype = dgl_end; arrowstyle = dg_solid;
		    global_node = from_node = find_node();
		}
#line 1592 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 156 "ggy.y" /* yacc.c:1652  */
    {
		    global_node = to_node = find_node();
		    add_link( from_node, to_node );
		}
#line 1601 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 163 "ggy.y" /* yacc.c:1652  */
    {
		    global_node = to_node = find_node();
		    add_link( from_node, to_node );
		}
#line 1610 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 170 "ggy.y" /* yacc.c:1652  */
    { addedgelabel(); }
#line 1616 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 171 "ggy.y" /* yacc.c:1652  */
    { addedgelabel(); }
#line 1622 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 172 "ggy.y" /* yacc.c:1652  */
    { addedgelabel(); }
#line 1628 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 177 "ggy.y" /* yacc.c:1652  */
    {   global_node->ggtype = dg_box;
		}
#line 1635 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 53:
#line 181 "ggy.y" /* yacc.c:1652  */
    {   global_node->ggtype = dg_hex;
		}
#line 1642 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 186 "ggy.y" /* yacc.c:1652  */
    { global_node = from_node = find_node(); }
#line 1648 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 188 "ggy.y" /* yacc.c:1652  */
    { global_node = from_node = find_node(); }
#line 1654 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 202 "ggy.y" /* yacc.c:1652  */
    { add_assignment( global_node, (yyvsp[-1].var), (yyvsp[0].exp) ); }
#line 1660 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 205 "ggy.y" /* yacc.c:1652  */
    { (yyval.var) = find_id(); }
#line 1666 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 208 "ggy.y" /* yacc.c:1652  */
    { (yyval.var) = (yyvsp[-1].var); }
#line 1672 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 209 "ggy.y" /* yacc.c:1652  */
    { (yyval.var) = NULL; }
#line 1678 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 212 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1684 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 213 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = NULL; }
#line 1690 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 215 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1696 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 216 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exOrElse, (yyvsp[0].exp) ); }
#line 1702 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 218 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1708 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 219 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exAndThen, (yyvsp[0].exp) ); }
#line 1714 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 221 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1720 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 222 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exOr, (yyvsp[0].exp) ); }
#line 1726 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 224 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1732 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 225 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exXor, (yyvsp[0].exp) ); }
#line 1738 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 227 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1744 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 228 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exAnd, (yyvsp[0].exp) ); }
#line 1750 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 230 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1756 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 231 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exEqual, (yyvsp[0].exp) ); }
#line 1762 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 232 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exNotEqual, (yyvsp[0].exp) ); }
#line 1768 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 234 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1774 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 83:
#line 235 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exGreaterEqual, (yyvsp[0].exp) ); }
#line 1780 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 236 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exGreater, (yyvsp[0].exp) ); }
#line 1786 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 237 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exLessEqual, (yyvsp[0].exp) ); }
#line 1792 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 238 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exLess, (yyvsp[0].exp) ); }
#line 1798 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 240 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1804 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 241 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exLeftShift, (yyvsp[0].exp) ); }
#line 1810 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 242 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exRightShift, (yyvsp[0].exp) ); }
#line 1816 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 244 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1822 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 245 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exAdd, (yyvsp[0].exp) ); }
#line 1828 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 246 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exSub, (yyvsp[0].exp) ); }
#line 1834 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 93:
#line 248 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1840 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 94:
#line 249 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exMul, (yyvsp[0].exp) ); }
#line 1846 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 95:
#line 250 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exMod, (yyvsp[0].exp) ); }
#line 1852 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 96:
#line 251 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( (yyvsp[-2].exp), exDiv, (yyvsp[0].exp) ); }
#line 1858 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 97:
#line 253 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1864 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 98:
#line 254 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( NULL, exNot, (yyvsp[0].exp) ); }
#line 1870 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 99:
#line 255 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( NULL, exCompl, (yyvsp[0].exp) ); }
#line 1876 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 100:
#line 256 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 1882 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 101:
#line 257 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( NULL, exNeg, (yyvsp[0].exp) ); }
#line 1888 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 102:
#line 258 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( NULL, exDeref, (yyvsp[0].exp) ); }
#line 1894 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 103:
#line 260 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( exVar, find_id() ); }
#line 1900 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 104:
#line 261 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = newexpr( exConst, find_id() ); }
#line 1906 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 105:
#line 262 "ggy.y" /* yacc.c:1652  */
    { (yyval.exp) = (yyvsp[-1].exp) }
#line 1912 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 106:
#line 266 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makeexp( opValue, (yyvsp[0].df) ); }
#line 1918 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 107:
#line 267 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makeexp( opInValue, (yyvsp[-2].df) ); }
#line 1924 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 108:
#line 268 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makeexp( opOutValue, (yyvsp[-2].df) ); }
#line 1930 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 109:
#line 269 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAllPred, NULL, (yyvsp[0].dfexp) ); }
#line 1936 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 110:
#line 270 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAnyPred, NULL, (yyvsp[0].dfexp) ); }
#line 1942 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 111:
#line 271 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAllPredEdges, NULL, (yyvsp[0].dfexp) ); }
#line 1948 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 112:
#line 272 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAnyPredEdges, NULL, (yyvsp[0].dfexp) ); }
#line 1954 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 113:
#line 273 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opPredNode, NULL, (yyvsp[0].dfexp) ); }
#line 1960 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 114:
#line 274 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAllSucc, NULL, (yyvsp[0].dfexp) ); }
#line 1966 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 115:
#line 275 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAnySucc, NULL, (yyvsp[0].dfexp) ); }
#line 1972 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 116:
#line 276 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAllSuccEdges, NULL, (yyvsp[0].dfexp) ); }
#line 1978 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 117:
#line 277 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAnySuccEdges, NULL, (yyvsp[0].dfexp) ); }
#line 1984 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 118:
#line 278 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opSuccNode, NULL, (yyvsp[0].dfexp) ); }
#line 1990 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 119:
#line 279 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = (yyvsp[-1].dfexp) }
#line 1996 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 120:
#line 282 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = (yyvsp[0].dfexp); }
#line 2002 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 121:
#line 283 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opNot, NULL, (yyvsp[0].dfexp) ); }
#line 2008 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 122:
#line 287 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = (yyvsp[0].dfexp); }
#line 2014 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 123:
#line 288 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opAnd, (yyvsp[-2].dfexp), (yyvsp[0].dfexp) ); }
#line 2020 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 124:
#line 292 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = (yyvsp[0].dfexp); }
#line 2026 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 125:
#line 293 "ggy.y" /* yacc.c:1652  */
    { (yyval.dfexp) = makebinary( opOr, (yyvsp[-2].dfexp), (yyvsp[0].dfexp) ); }
#line 2032 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 129:
#line 303 "ggy.y" /* yacc.c:1652  */
    { DF_LHS = (yyvsp[0].df); }
#line 2038 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 130:
#line 304 "ggy.y" /* yacc.c:1652  */
    { (yyvsp[-4].df)->Direction = (yyvsp[0].Intval); }
#line 2044 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 131:
#line 306 "ggy.y" /* yacc.c:1652  */
    {
		    if( (yyvsp[-5].Intval) == 1 )
			if( (yyvsp[-8].df)->expression[3] == NULL )
			    (yyvsp[-8].df)->expression[3] = (yyvsp[0].dfexp); 
			else printf( "Edge property %s defined twice\n", (yyvsp[-8].df)->Name );
		    else if( (yyvsp[-8].df)->expression[(yyvsp[-6].Intval)] == NULL )
			(yyvsp[-8].df)->expression[(yyvsp[-6].Intval)] = (yyvsp[0].dfexp); 
		    else if( (yyvsp[-6].Intval) == 0 ) printf( "Node property %s defined twice\n", (yyvsp[-8].df)->Name );
		    else if( (yyvsp[-6].Intval) == 1 ) printf( "Node property %s.in defined twice\n", (yyvsp[-8].df)->Name );
		    else if( (yyvsp[-6].Intval) == 2 ) printf( "Node property %s.out defined twice\n", (yyvsp[-8].df)->Name );
		}
#line 2060 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 132:
#line 319 "ggy.y" /* yacc.c:1652  */
    { (yyval.df) = find_dataflow( ); }
#line 2066 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 133:
#line 322 "ggy.y" /* yacc.c:1652  */
    { DF_LHS->EdgeNode |= 2; (yyval.Intval) = 2; }
#line 2072 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 134:
#line 323 "ggy.y" /* yacc.c:1652  */
    { DF_LHS->EdgeNode |= 1; (yyval.Intval) = 1; }
#line 2078 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 135:
#line 324 "ggy.y" /* yacc.c:1652  */
    { DF_LHS->EdgeNode |= 1; (yyval.Intval) = 1; }
#line 2084 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 136:
#line 328 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = 1; }
#line 2090 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 137:
#line 329 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = 2; }
#line 2096 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 138:
#line 330 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = 0; }
#line 2102 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 139:
#line 333 "ggy.y" /* yacc.c:1652  */
    { DF_LHS->Initially = 0; DF_LHS->Boundary = 0; }
#line 2108 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 140:
#line 334 "ggy.y" /* yacc.c:1652  */
    { DF_LHS->Initially = (yyvsp[-1].Intval); DF_LHS->Boundary = 0; }
#line 2114 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 141:
#line 336 "ggy.y" /* yacc.c:1652  */
    { DF_LHS->Initially = (yyvsp[-3].Intval); DF_LHS->Boundary = (yyvsp[-1].Intval); }
#line 2120 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 142:
#line 339 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = 0; }
#line 2126 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 143:
#line 340 "ggy.y" /* yacc.c:1652  */
    { if( intval() == 0 ) (yyval.Intval) = 0; else (yyval.Intval) = 1; }
#line 2132 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 144:
#line 343 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = +1; }
#line 2138 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 145:
#line 344 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = -1; }
#line 2144 "ggy.tab.c" /* yacc.c:1652  */
    break;

  case 146:
#line 345 "ggy.y" /* yacc.c:1652  */
    { (yyval.Intval) = 0; }
#line 2150 "ggy.tab.c" /* yacc.c:1652  */
    break;


#line 2154 "ggy.tab.c" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 348 "ggy.y" /* yacc.c:1918  */

/* program section */

/* global variables: */
/* line number */
long Lines;

P_GG GG_Prev;

/* communication from lexer */
char lexname[120];

#include "ggl.c"

/* parse_die */
void parse_die( void )
{
    printf( "Ended on line %d at >>>%s<<<\n", Lines, yytext );
    exit(1);
}/* parse_die */

/* Initialize everything. */
void parse_init( void ){
P_Dataflow df;
    GG_Nodes = 0;
    GG_Links = 0;
    GG_List.Init();
    Vars = 0;
    Var_List.Init();
    Dataflow_List.Init();
    Expression_List.Init();
    Lines = 1;
    DataflowCount = 0;

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "TRANSDOWN" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opTransDown, NULL );
    df->EdgeNode = 1;
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "TRANSUP" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opTransUp, NULL );
    df->EdgeNode = 1;
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "TRANS" );
    df->Position = DataflowCount;
    df->expression[0] = makeexp( opTrans, NULL );
    df->EdgeNode = 2; /* a node property */
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "EXPOSEUP" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opUXP, NULL );
    df->expression[0] = makeexp( opUXP, NULL );
    df->EdgeNode = 3; /* node and edge property */
    insert_list_end( df, Dataflow_List );

    ++DataflowCount;
    df = new Dataflow;
    memset( df, 0, sizeof( Dataflow ) );
    strcpy( df->Name, "EXPOSEDOWN" );
    df->Position = DataflowCount;
    df->expression[3] = makeexp( opDXP, NULL );
    df->expression[0] = makeexp( opDXP, NULL );
    df->EdgeNode = 3; /* node and edge property */
    insert_list_end( df, Dataflow_List );
    BuiltinProperties = DataflowCount;
}/* parse_init */

/* find a node with this name */
P_GG find_node( void ){
P_GG newgg;

    Visit_List(thisgg,GG_List,GG){
	if( strcmp( thisgg->GG_Name, lexname ) == 0 ){
	    return thisgg;
	}
    }End_Visit;

    /* must add a new one */
    ++GG_Nodes;
    newgg = new GG;
    memset( newgg, 0, sizeof( GG ) );
    newgg->ggtype = dg_circle;
    newgg->GG_Succs.Init();
    newgg->GG_Preds.Init();
    strcpy( newgg->GG_Name, lexname );
    newgg->GG_Number = 0;
    newgg->Lowlink = 0;
    newgg->InStack = 0;
    newgg->Stack_Next = NULL;

    insert_list_end( newgg, GG_List );
    return newgg;
}/* find_node */

P_Ptr_GG link1, link2;
void add_link( P_GG from, P_GG to ){
    ++GG_Links;
    link1 = new Ptr_GG;
    memset( link1, 0, sizeof( Ptr_GG ) );
    link2 = new Ptr_GG;
    memset( link2, 0, sizeof( Ptr_GG ) );

    link1->Link = to;
    link1->GGL_Match = link2;
    link1->GGL_IN_Tree = 0;
    link1->ggarrow = arrowtype;
    link1->ggstyle = arrowstyle;
    insert_list_end( link1, from->GG_Succs );

    link2->Link = from;
    link2->GGL_Match = link1;
    link2->GGL_IN_Tree = 0;
    link2->ggarrow = arrowtype;
    link2->ggstyle = arrowstyle;
    insert_list_end( link2, to->GG_Preds );
}/* add_link */

void addedgelabel( void ){
    strcpy( link1->GG_EdgeName, lexname );
    strcpy( link2->GG_EdgeName, lexname );
}/* addedgelabel */

P_Var find_id( void ){
P_Var newvar;
    Visit_List(thisid,Var_List,Var){
	if( strcmp( thisid->Name, lexname ) == 0 ){
	    return thisid;
	}
    }End_Visit;

    /* must add a new one */
    ++Vars;
    newvar = new Var;
    memset( newvar, 0, sizeof( Var ) );
    strcpy( newvar->Name, lexname );
    newvar->Num = Vars;
    insert_list_end( newvar, Var_List );
    return newvar;
}/* find_id */

void add_assignment( P_GG node, P_Var lhs, P_Expression rhs ){
P_Assignment newref;
    newref = new Assignment;
    memset( newref, 0, sizeof( Assignment ) );
    newref->LHS = lhs;
    newref->RHS = rhs;
    insert_list_end( newref, node->Assignments );
}/* add_ref */

P_Dataflow find_dataflow( ){
    P_Dataflow newdf;
    Visit_List(thisdf,Dataflow_List,Dataflow){
	if( strcmp( thisdf->Name, lexname ) == 0 ){
	    return thisdf;
	}
    }End_Visit;

    /* must add a new one */
    ++DataflowCount;
    newdf = new Dataflow;
    memset( newdf, 0, sizeof( Dataflow ) );
    strcpy( newdf->Name, lexname );
    newdf->Position = DataflowCount;
    insert_list_end( newdf, Dataflow_List );
    return newdf;
}/* find_dataflow */

P_DFExpression makeexp( operatortype op, P_Dataflow operand ){
    P_DFExpression newexp = new DFExpression;
    memset( newexp, 0, sizeof( DFExpression ) );
    newexp->OP = op;
    newexp->Operand = operand;
    return newexp;
}/* makeexp */

P_DFExpression makebinary( operatortype op, P_DFExpression left, 
	    P_DFExpression right ){
    P_DFExpression newexp = new DFExpression;
    memset( newexp, 0, sizeof( DFExpression ) );
    newexp->OP = op;
    newexp->Left = left;
    newexp->Right = right;
    return newexp;
}/* makebinary */

P_Expression findexpr( ExType op, P_Expression left, P_Expression right,
		P_Var operand ){
    Visit_List( ex, Expression_List, Expression ){
	if( ex->OP == op &&
	    ex->Left == left &&
	    ex->Right == right &&
	    ex->Operand == operand ) return ex;
    }End_Visit;
    return NULL;
}// findexpr

P_Expression newexpr( P_Expression left, ExType op, P_Expression right ){
    P_Expression newexp = findexpr( op, left, right, NULL );
    if( newexp ) return newexp;
    newexp = new Expression;
    memset( newexp, 0, sizeof( Expression ) );
    newexp->OP = op;
    newexp->Left = left;
    newexp->Right = right;
    insert_list_end( newexp, Expression_List );
    return newexp;
}/* newexpr */

P_Expression newexpr( ExType op, P_Var operand ){
    P_Expression newexp = findexpr( op, NULL, NULL, operand );
    if( newexp ) return newexp;
    newexp = new Expression;
    memset( newexp, 0, sizeof( Expression ) );
    newexp->OP = op;
    newexp->Operand = operand;
    insert_list_end( newexp, Expression_List );
    return newexp;
}/* newexpr */

int intval(){
    int v = 0;
    char *s = lexname;
    while( *s ){
	v = v*10 + (*s-'0');
	++s;
    }
    return v;
}/* intval */

void yyerror( char *s)
{
    printf( "%s\n", s );
    parse_die();
}/* yyerror */