/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
 *
 *
 *  This file is part of taylor.
 *
 * Taylor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * Taylor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taylor; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 *************************************************************************/

%{
#include <stdio.h>
#include "Header.h"
#include "y.tab.h"

#ifndef  EOS
#define  EOS '\0'
#endif

extern Node         current_id;
extern char         *current_qstring;
/****************************************************************************/
%}

%%

[ \t\n\r]     { ; /* white spaces     */}

"/*"        {   /* C style comment */
                  register int c = input();
                  while(1) 
                   { 
		     if( c == '*')  
                      {
                         c = input();
                         if( c  == '/')  break;
                      }
  		    else if( c == '\0')
		      {
                        (void)fprintf(stderr,
                        "Error at line %d, unterminated comment\n", yylineno);
                        exit(1);  
		      }
		    else  c = input();
                  }
           }
"//"       { /* C++ style comment */
                  register int c = input();
                  while(1) 
                   { 
		     if( c == '\n' || c == '\0')  break;
		     else  c = input();
		   }
            }
"#"       { /* shell script comment */
                  register int c = input();
                  while(1) 
                   { 
		     if( c == '\n' || c == '\0')  break;
		     else  c = input();
		   }
           }

allvars                         { return(ALLVARS); }
all                             { return(ALLVARS); }

diff                            { return(DIFF); }

extern                          { return(EXTRN); }

jet                             { return(JET); }
cloud                           { return(CLOUD); }
size                            { return(SIZE); }

vars                            { return(VARS); }
symbols                         { return(VARS); }

variables                       { return(VARS); }
deg                             { return(DEG); }
degree                          { return(DEG); }
include                         { return(INCLUDE); }
includes                        { return(INCLUDE); }
including                       { return(INCLUDE); }

expression                      { return(EXPR);}
expr                            { return(EXPR);}

sum                             { return(SUM); }

int                             { return(INT); }
short                           { return(SHORT); }
char                            { return(CHAR); }

MY_FLOAT                        { return(REAL); }
float                           { return(REAL); }
double                          { return(REAL); }

if                              { return(IF); }

else                            { return(ELSE); }

jetInitialValue                 { return(JINITIALV);}
JetInitialValue                 { return(JINITIALV);}
jetInitialValues                { return(JINITIALV);}
JetInitialValues                { return(JINITIALV);}
jet_initial_value               { return(JINITIALV);}
jet_initial_values              { return(JINITIALV);}

initialValue                    { return(INITIALV);}
InitialValue                    { return(INITIALV);}
initialValues                   { return(INITIALV);}
InitialValues                   { return(INITIALV);}
initial_value                   { return(INITIALV);}
Initial_Value                   { return(INITIALV);}
initial_values                  { return(INITIALV);}
Initial_Values                  { return(INITIALV);}


\"([^\"]+)\"                    {
                                  current_qstring = AllocCopyString(&yytext[1]);
                                  return(QSTRING);
                                } 


[A-Za-z][A-Za-z0-9_]*           {   /*  identifier      */ 
                                    current_id = install_id(yytext);
                                    return(ID);  
                                }

[0-9]+                          {   /*  integer const   */
                                  yylval.ntype = build_int(yytext);
                                  return(INTCON);
                                }
[0-9]*"."[0-9]*([Ee][-]?[0-9]+)? {   /*  floating point const   */
                                  yylval.ntype = build_float(yytext);
                                  return(FLOATCON);
                                }

"+"           {
                 yylval.code = PLUS_EXPR;
                 return('+');        
              }
"-"           {
                 yylval.code = MINUS_EXPR;
                 return('-');        
              }
"*"           {
                 yylval.code = MULT_EXPR;
                 return('*');        
              }
"/"           {
                 yylval.code = DIV_EXPR;
                 return('/');        
              }
"^"           {
                 yylval.code = EXP_EXPR;
                 return('^');        
              }
"**"          {
                 yylval.code = EXP_EXPR;
                 return(EXP);        
              }
"<"           {
                 yylval.code = LT_EXPR;
                 return(LT);        
              }
"<="           {
                 yylval.code = LE_EXPR;
                 return(LE);        
              }
">"           {
                 yylval.code = GT_EXPR;
                 return(GT);        
              }
">="           {
                 yylval.code = GE_EXPR;
                 return(GE);        
              }
"=="           {
                 yylval.code = EQ_EXPR;
                 return(EQ);        
              }
"!="           {
                 yylval.code = NEQ_EXPR;
                 return(NEQ);        
              }

"&&"           {
                 yylval.code = AND_EXPR;
                 return(AND);        
              }
"||"           {
                 yylval.code = OR_EXPR;
                 return(OR);        
              }


"="           { return('=');         }
"("           { return('(');         }
")"           { return(')');         }
"["           { return('[');         }
"]"           { return(']');         }
"{"           { return('{');         }
"}"           { return('}');         }
";"           { return(';');         }
","           { return(',');         }
"'"           { return('\'');        }
.             {
                (void)fprintf(stderr,"Error: line %d, near '%s': illegal character\n",yylineno, yytext);
		if(0)yyunput(' ', NULL);
                return(yytext[0]);   
              }
%%


