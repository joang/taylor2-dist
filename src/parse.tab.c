
/*  A Bison parser, made from parse.y
 by  GNU Bison version 1.27
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	IF	257
#define	ELSE	258
#define	ID	259
#define	INTCON	260
#define	FLOATCON	261
#define	EXTRN	262
#define	SUM	263
#define	DIFF	264
#define	INT	265
#define	REAL	266
#define	SHORT	267
#define	CHAR	268
#define	INITIALV	269
#define	STEP	270
#define	NSTEPS	271
#define	TOLERANCE	272
#define	STARTTIME	273
#define	STOPTIME	274
#define	ORDER	275
#define	OR	276
#define	AND	277
#define	EQ	278
#define	NEQ	279
#define	LE	280
#define	GE	281
#define	LT	282
#define	GT	283
#define	UNARY	284

#line 6 "parse.y"

#include <stdio.h>
#include "Header.h"  

extern int yylineno;
extern int  yylex();
static void yyerror(char *str);
Node        current_id;

#line 17 "parse.y"
typedef union { Node ntype; enum node_code code; } YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		148
#define	YYFLAG		-32768
#define	YYNTBASE	46

#define YYTRANSLATE(x) ((unsigned)(x) <= 284 ? yytranslate[x] : 68)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    41,    36,
    42,    32,    30,    40,    31,     2,    33,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    38,     2,
    39,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    37,     2,    43,    35,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    44,     2,    45,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    34
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     6,    10,    12,    14,    16,    18,    22,
    26,    30,    34,    38,    42,    46,    48,    52,    54,    56,
    59,    62,    67,    76,    80,    84,    86,    90,    92,    95,
    97,   101,   104,   105,   107,   109,   111,   113,   115,   119,
   123,   127,   131,   135,   139,   143,   147,   151,   153,   157,
   161,   165,   169,   173,   176,   179,   191,   193,   196,   198,
   200,   204,   208,   213,   214,   226,   228,   230,   233
};

static const short yyrhs[] = {    -1,
    47,    38,     0,    48,     0,    47,    38,    48,     0,    52,
     0,    53,     0,    54,     0,    49,     0,    15,    39,    50,
     0,    19,    39,    51,     0,    20,    39,    51,     0,    16,
    39,    51,     0,    17,    39,     6,     0,    21,    39,     6,
     0,    18,    39,    51,     0,    51,     0,    50,    40,    51,
     0,     6,     0,     7,     0,    31,     6,     0,    31,     7,
     0,    60,    41,    39,    62,     0,    10,    36,    60,    40,
    60,    42,    39,    62,     0,    60,    39,    62,     0,     8,
    59,    55,     0,    56,     0,    55,    40,    56,     0,    57,
     0,    56,    58,     0,     5,     0,    37,     6,    43,     0,
    37,    43,     0,     0,    11,     0,    13,     0,    14,     0,
    12,     0,     5,     0,    62,    24,    62,     0,    62,    25,
    62,     0,    62,    27,    62,     0,    62,    29,    62,     0,
    62,    26,    62,     0,    62,    28,    62,     0,    61,    23,
    61,     0,    61,    22,    61,     0,    36,    61,    42,     0,
    63,     0,    62,    35,    62,     0,    62,    32,    62,     0,
    62,    33,    62,     0,    62,    30,    62,     0,    62,    31,
    62,     0,    31,    62,     0,    30,    62,     0,     3,    36,
    61,    42,    44,    62,    45,     4,    44,    62,    45,     0,
    65,     0,    65,    66,     0,     6,     0,     7,     0,    36,
    62,    42,     0,    36,     1,    42,     0,    65,    36,    62,
    42,     0,     0,     9,    64,    36,    62,    40,    65,    39,
    62,    40,    62,    42,     0,     5,     0,    67,     0,    66,
    67,     0,    37,    62,    43,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    46,    48,    50,    52,    54,    56,    57,    58,    61,    62,
    64,    66,    68,    70,    72,    76,    78,    82,    83,    84,
    86,    90,    93,    97,   102,   107,   109,   112,   114,   118,
   123,   125,   129,   131,   133,   135,   137,   142,   148,   151,
   153,   155,   157,   159,   161,   163,   165,   169,   171,   173,
   175,   177,   179,   181,   183,   185,   189,   192,   194,   195,
   196,   198,   200,   202,   204,   208,   213,   214,   218
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","IF","ELSE",
"ID","INTCON","FLOATCON","EXTRN","SUM","DIFF","INT","REAL","SHORT","CHAR","INITIALV",
"STEP","NSTEPS","TOLERANCE","STARTTIME","STOPTIME","ORDER","OR","AND","EQ","NEQ",
"LE","GE","LT","GT","'+'","'-'","'*'","'/'","UNARY","'^'","'('","'['","';'",
"'='","','","'\\''","')'","']'","'{'","'}'","program","stmts","stmt","control",
"initials","number","derivative","define","declare","declrs","declare_one","decl_id",
"decl_array","settype","id","bexpr","expr","term","@1","idexpr","arrayref","one_idx", NULL
};
#endif

static const short yyr1[] = {     0,
    46,    46,    47,    47,    48,    48,    48,    48,    49,    49,
    49,    49,    49,    49,    49,    50,    50,    51,    51,    51,
    51,    52,    52,    53,    54,    55,    55,    56,    56,    57,
    58,    58,    59,    59,    59,    59,    59,    60,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    62,    62,    62,
    62,    62,    62,    62,    62,    62,    63,    63,    63,    63,
    63,    63,    63,    64,    63,    65,    66,    66,    67
};

static const short yyr2[] = {     0,
     0,     2,     1,     3,     1,     1,     1,     1,     3,     3,
     3,     3,     3,     3,     3,     1,     3,     1,     1,     2,
     2,     4,     8,     3,     3,     1,     3,     1,     2,     1,
     3,     2,     0,     1,     1,     1,     1,     1,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     1,     3,     3,
     3,     3,     3,     2,     2,    11,     1,     2,     1,     1,
     3,     3,     4,     0,    11,     1,     1,     2,     3
};

static const short yydefact[] = {     1,
    38,    33,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     3,     8,     5,     6,     7,     0,    34,    37,    35,
    36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     2,     0,     0,    30,    25,    26,    28,     0,    18,    19,
     0,     9,    16,    12,    13,    15,    10,    11,    14,     4,
     0,    66,    59,    60,    64,     0,     0,     0,    24,    48,
    57,     0,     0,     0,    29,     0,    20,    21,     0,     0,
     0,    55,    54,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    58,    67,    22,    27,     0,    32,     0,    17,
     0,     0,     0,     0,    62,    61,    52,    53,    50,    51,
    49,     0,     0,    68,    31,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    63,    69,
     0,    47,    46,    45,     0,    39,    40,    43,    41,    44,
    42,     0,    23,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    56,    65,     0,     0,     0
};

static const short yydefgoto[] = {   146,
    11,    12,    13,    42,    43,    14,    15,    16,    35,    36,
    37,    65,    22,    17,    92,    93,    60,    71,    61,    83,
    84
};

static const short yypact[] = {   141,
-32768,     5,   -23,    -7,     3,    15,    17,    18,    19,    24,
    26,-32768,-32768,-32768,-32768,-32768,     2,-32768,-32768,-32768,
-32768,    63,    67,    -4,    -4,    72,    -4,    -4,    -4,    80,
   141,   107,    49,-32768,    50,    54,-32768,    52,-32768,-32768,
    22,    62,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    68,-32768,-32768,-32768,-32768,   107,   107,    30,   195,-32768,
    16,   107,    63,    -5,-32768,    67,-32768,-32768,    -4,   114,
    70,    83,    83,    61,   139,   107,   107,   107,   107,   107,
   107,   107,    78,-32768,   195,    54,    79,-32768,    65,-32768,
    64,    -8,   177,   107,-32768,-32768,   -27,   -27,    83,    83,
    83,   152,    44,-32768,-32768,    95,    -2,   100,   114,   114,
    92,   107,   107,   107,   107,   107,   107,   183,-32768,-32768,
   107,-32768,   116,-32768,   107,   195,   195,   195,   195,   195,
   195,   135,   195,    66,   102,   143,   107,   104,   189,   107,
   107,   133,   158,-32768,-32768,   153,   154,-32768
};

static const short yypgoto[] = {-32768,
-32768,   121,-32768,-32768,   -18,-32768,-32768,-32768,-32768,   110,
-32768,-32768,-32768,   -11,   -87,   -32,-32768,-32768,    23,-32768,
    84
};


#define	YYLAST		230


static const short yytable[] = {    59,
    87,    39,    40,   107,    78,    79,    44,    80,    46,    47,
    48,    38,    23,   109,   110,    18,    19,    20,    21,   109,
   110,   123,   124,    72,    73,    75,    41,    67,    68,    85,
    74,    24,    51,   111,    52,    53,    54,    88,    55,   122,
    32,    25,    33,    97,    98,    99,   100,   101,   102,   103,
    90,    81,    82,    26,    89,    27,    28,    29,   108,    56,
    57,   118,    30,    31,    74,    58,    51,    34,    52,    53,
    54,     1,    55,    76,    77,    78,    79,    45,    80,   126,
   127,   128,   129,   130,   131,    49,   120,    62,   133,    63,
    64,    66,   134,    56,    57,    76,    77,    78,    79,    91,
    80,    69,    95,    70,   139,    94,   106,   142,   143,    51,
   136,    52,    53,    54,    82,    55,    51,    80,    52,    53,
    54,   105,    55,   112,   113,   114,   115,   116,   117,    76,
    77,    78,    79,   121,    80,   125,    56,    57,   110,    52,
   137,    96,    58,    56,    57,     1,   138,   140,     2,    91,
     3,    50,   147,   148,   135,     4,     5,     6,     7,     8,
     9,    10,    76,    77,    78,    79,   104,    80,    76,    77,
    78,    79,    86,    80,     0,     0,     0,   144,     0,     0,
    96,    76,    77,    78,    79,     0,    80,    76,    77,    78,
    79,     0,    80,   119,     0,     0,     0,     0,     0,   145,
   112,   113,   114,   115,   116,   117,    76,    77,    78,    79,
     0,    80,    76,    77,    78,    79,     0,    80,    76,    77,
    78,    79,   132,    80,    76,    77,    78,    79,   141,    80
};

static const short yycheck[] = {    32,
     6,     6,     7,    91,    32,    33,    25,    35,    27,    28,
    29,    23,    36,    22,    23,    11,    12,    13,    14,    22,
    23,   109,   110,    56,    57,    58,    31,     6,     7,    62,
     1,    39,     3,    42,     5,     6,     7,    43,     9,    42,
    39,    39,    41,    76,    77,    78,    79,    80,    81,    82,
    69,    36,    37,    39,    66,    39,    39,    39,    91,    30,
    31,    94,    39,    38,     1,    36,     3,     5,     5,     6,
     7,     5,     9,    30,    31,    32,    33,     6,    35,   112,
   113,   114,   115,   116,   117,     6,    43,    39,   121,    40,
    37,    40,   125,    30,    31,    30,    31,    32,    33,    36,
    35,    40,    42,    36,   137,    36,    42,   140,   141,     3,
    45,     5,     6,     7,    37,     9,     3,    35,     5,     6,
     7,    43,     9,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    39,    35,    44,    30,    31,    23,     5,
    39,    42,    36,    30,    31,     5,     4,    44,     8,    36,
    10,    31,     0,     0,   132,    15,    16,    17,    18,    19,
    20,    21,    30,    31,    32,    33,    83,    35,    30,    31,
    32,    33,    63,    35,    -1,    -1,    -1,    45,    -1,    -1,
    42,    30,    31,    32,    33,    -1,    35,    30,    31,    32,
    33,    -1,    35,    42,    -1,    -1,    -1,    -1,    -1,    42,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    -1,    35,    30,    31,    32,    33,    -1,    35,    30,    31,
    32,    33,    40,    35,    30,    31,    32,    33,    40,    35
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 10:
#line 63 "parse.y"
{ controlParams.startTime = yyvsp[0].ntype;;
    break;}
case 11:
#line 65 "parse.y"
{ controlParams.stopTime = yyvsp[0].ntype;;
    break;}
case 12:
#line 67 "parse.y"
{ controlParams.step = yyvsp[0].ntype;;
    break;}
case 13:
#line 69 "parse.y"
{ int jk; controlParams.nsteps = intNodeValue(yyvsp[0].ntype,&jk);;
    break;}
case 14:
#line 71 "parse.y"
{ int jk; controlParams.order = intNodeValue(yyvsp[0].ntype, &jk);;
    break;}
case 15:
#line 73 "parse.y"
{ controlParams.tolerance = yyvsp[0].ntype;;
    break;}
case 16:
#line 77 "parse.y"
{ addInitial(yyvsp[0].ntype); ;
    break;}
case 17:
#line 79 "parse.y"
{ addInitial(yyvsp[0].ntype); ;
    break;}
case 20:
#line 85 "parse.y"
{ yyval.ntype = build_op(NEGATE_EXPR, yyvsp[0].ntype, NULL); ;
    break;}
case 21:
#line 87 "parse.y"
{ yyval.ntype = build_op(NEGATE_EXPR, yyvsp[0].ntype, NULL); ;
    break;}
case 22:
#line 92 "parse.y"
{ parse_one_equation(yyvsp[-3].ntype, NULL, yyvsp[0].ntype);;
    break;}
case 23:
#line 94 "parse.y"
{ parse_one_equation(yyvsp[-5].ntype, yyvsp[-3].ntype, yyvsp[0].ntype);;
    break;}
case 24:
#line 99 "parse.y"
{ define_one_variable(yyvsp[-2].ntype, yyvsp[0].ntype);;
    break;}
case 25:
#line 104 "parse.y"
{ ;
    break;}
case 29:
#line 115 "parse.y"
{ yyval.ntype = declareArray(yyvsp[-1].ntype,yyvsp[0].ntype); ;
    break;}
case 30:
#line 120 "parse.y"
{ yyval.ntype = declareExternVar(current_id); ;
    break;}
case 31:
#line 124 "parse.y"
{ yyval.ntype = yyvsp[-1].ntype;;
    break;}
case 32:
#line 126 "parse.y"
{ yyval.ntype = NULL;;
    break;}
case 33:
#line 130 "parse.y"
{etype = 0;;
    break;}
case 34:
#line 132 "parse.y"
{etype = 4;;
    break;}
case 35:
#line 134 "parse.y"
{etype = 2;;
    break;}
case 36:
#line 136 "parse.y"
{etype = 1;;
    break;}
case 37:
#line 138 "parse.y"
{etype = 0;;
    break;}
case 38:
#line 144 "parse.y"
{ yyval.ntype = current_id; ;
    break;}
case 39:
#line 150 "parse.y"
{ yyval.ntype = build_bop(EQ_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 40:
#line 152 "parse.y"
{ yyval.ntype = build_bop(NEQ_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 41:
#line 154 "parse.y"
{ yyval.ntype = build_bop(GE_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 42:
#line 156 "parse.y"
{ yyval.ntype = build_bop(GT_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 43:
#line 158 "parse.y"
{ yyval.ntype = build_bop(LE_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 44:
#line 160 "parse.y"
{ yyval.ntype = build_bop(LT_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 45:
#line 162 "parse.y"
{ yyval.ntype = build_bop(AND_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 46:
#line 164 "parse.y"
{ yyval.ntype = build_bop(OR_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 47:
#line 166 "parse.y"
{ yyval.ntype = yyvsp[-1].ntype; ;
    break;}
case 49:
#line 172 "parse.y"
{ yyval.ntype = build_op(EXP_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 50:
#line 174 "parse.y"
{ yyval.ntype = build_op(MULT_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 51:
#line 176 "parse.y"
{ yyval.ntype = build_op(DIV_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 52:
#line 178 "parse.y"
{ yyval.ntype = build_op(PLUS_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 53:
#line 180 "parse.y"
{ yyval.ntype = build_op(MINUS_EXPR,yyvsp[-2].ntype,yyvsp[0].ntype); ;
    break;}
case 54:
#line 182 "parse.y"
{ yyval.ntype = build_op(NEGATE_EXPR, yyvsp[0].ntype, NULL); ;
    break;}
case 55:
#line 184 "parse.y"
{ yyval.ntype = yyvsp[0].ntype; ;
    break;}
case 56:
#line 186 "parse.y"
{ yyval.ntype = build_if_else(yyvsp[-8].ntype, yyvsp[-5].ntype, yyvsp[-1].ntype);;
    break;}
case 57:
#line 191 "parse.y"
{ check_array_ref(yyvsp[0].ntype);;
    break;}
case 58:
#line 193 "parse.y"
{ yyval.ntype = build_array_ref(yyvsp[-1].ntype, yyvsp[0].ntype);;
    break;}
case 61:
#line 197 "parse.y"
{ yyval.ntype = yyvsp[-1].ntype; ;
    break;}
case 62:
#line 199 "parse.y"
{ yyval.ntype = error_node; ;
    break;}
case 63:
#line 201 "parse.y"
{ yyval.ntype = build_op(CALL_EXPR, yyvsp[-3].ntype, yyvsp[-1].ntype); ;
    break;}
case 64:
#line 203 "parse.y"
{insum++;;
    break;}
case 65:
#line 205 "parse.y"
{ yyval.ntype = build_sum( yyvsp[-7].ntype, yyvsp[-5].ntype, yyvsp[-3].ntype, yyvsp[-1].ntype); insum--;;
    break;}
case 66:
#line 210 "parse.y"
{ yyval.ntype = current_id;;
    break;}
case 68:
#line 215 "parse.y"
{ yyval.ntype = build_array_idx(yyvsp[-1].ntype, yyvsp[0].ntype); ;
    break;}
case 69:
#line 220 "parse.y"
{ yyval.ntype = build_array_idx(NULL, yyvsp[-1].ntype); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 228 "parse.y"


void yyerror(s)
  char *s;
{
   extern int yylineno;
   extern char yytext[];
   static int n_syntax_errs = 0;

   if(n_syntax_errs++ > 10)
     {
       (void)fprintf(stderr,"Too many syntax errors, goodbye!\n");
       exit(0);
     }
   else
     (void)fprintf(stderr,"line %d near '%s':  %s\n", yylineno, yytext, s);
}
/****************************************************************/


