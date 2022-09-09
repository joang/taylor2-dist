/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
 *    Copyright (C) 2022 Joan Gimeno, Angel Jorba, Maorong Zou
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

/*********************************************************************
 *
 * Grammar for the YACC parser 
 *
 *******************************************************************/
%{
#include <stdio.h>
#include "Header.h"  

extern int  yylex();
static void yyerror(char *str);
extern char yytext[];
/*Node        current_id;*/
%}

%start program
%union { Node ntype; enum node_code code; }

%token IF ELSE ID INTCON FLOATCON EXTRN SUM DIFF INT REAL SHORT CHAR JET VARS DEG INCLUDE ALLVARS
%token INITIALV JINITIALV QSTRING

%nonassoc IF
%nonassoc ELSE

%left <code> OR
%left <code> AND
%left <code> EQ NEQ LE GE LT GT
%left <code> '+' '-'
%left <code> '*' '/' 
%right <code>  UNARY
%right <code>  '^'
%left <code> '(' 
%left <code> '[' 
%type <ntype> ID INTCON FLOATCON
%type <ntype> idexpr id term  arrayref one_idx 
%type <ntype> expr bexpr
%type <ntype> EXTRN SUM JET
%type <ntype> decl_id decl_array declare_one declrs jet_id jet_one jets
%type <ntype> jparameters jparm1 jparm_id 

%%
/*************************************************************
 *
 *  Yacc syntax specification 
 *
 *************************************************************/
program:
                     /* empty */
                    | stmts ';'
		    ;
stmts: 
                      stmt
                    | stmts ';' stmt
                    ;  
stmt:
                      derivative
                    | define
                    | declare
                    | jet
                    | control
		    | jetinit
                    ;

control:            INITIALV '=' initials
                    ;


initials:          expr
                    { addInitial($1); }
                   | initials ',' expr
                    { addInitial($3); }
                    ;

jetinit:            JINITIALV id '=' QSTRING
                    { record_jet_initv($2,current_qstring);}
                    ;

derivative:
                      id '\''  '='  expr 
                      { parse_one_equation($1, NULL, $4);}
                    | DIFF '(' id ',' id ')' '=' expr
                      { parse_one_equation($3, $5, $8);}
		    ;

define:             
                      id  '='  expr 
                      { define_one_variable($1, $3);}
		    ;


jet:          
                    JET jets VARS var jparms DEG deg
                     { }
		    ;

jparms:            /* empty */ 
                   { }
                   | INCLUDE jparameters
                   { check_jet_params();}
                   ;

jparameters:      jparm1
                  | jparameters ','  jparm1
                  ;

jparm1: 
                   jparm_id
                   ;
 
jparm_id:
		   ID
                   { $$ = markJetParameter(current_id);}
                   ;


var:
		    INTCON
                    { num_symbols = atoi(yytext);}
                    ;

deg:
		    INTCON
                    { deg_jet_vars = atoi(yytext);}
                    ;

jets:               ALLVARS
                    { $$ =NULL;  markAllVarsJet();  }
                    | jetlist
		    { $$ = 0;}
		    ;
		    
jetlist:
		    jet_one
		    | jetlist ',' jet_one
		    ;


jet_one:
		      jet_id
		      ;


jet_id:
		      ID
                     { $$ = markJet(current_id); num_jet_vars++; }
                      ;


declare:          
                    EXTRN  settype declrs
                     { }
		    ;

declrs:
		    declare_one
		    | declrs ',' declare_one
		    ;

declare_one:
		      decl_id
		    | declare_one  decl_array 
                      { $$ = declareArray($1,$2); }
		      ;

decl_id:
		      ID
                     { $$ = declareExternVar(current_id); }
                      ;

decl_array:        '[' INTCON ']'
                     { $$ = $2;}
                  | '[' ']'
                     { $$ = NULL;}
                      ;

settype:           /* empty */ 
                    {etype = 0;}                              
                   | INT
                    {etype = 4;}
                   | SHORT
                    {etype = 2;}
                   | CHAR
                    {etype = 1;}
                   | REAL
                    {etype = 0;}         
		    ;
		    

id:
                    ID
                     { $$ = current_id; }
                    ;


bexpr:
		       expr EQ  expr
                        { $$ = build_bop(EQ_EXPR,$1,$3); }
		     | expr NEQ expr
                        { $$ = build_bop(NEQ_EXPR,$1,$3); }
		     | expr GE  expr
                        { $$ = build_bop(GE_EXPR,$1,$3); }
		     | expr GT  expr
                        { $$ = build_bop(GT_EXPR,$1,$3); }
		     | expr LE  expr
                        { $$ = build_bop(LE_EXPR,$1,$3); }
		     | expr LT  expr
                        { $$ = build_bop(LT_EXPR,$1,$3); }
		     | bexpr AND  bexpr
                        { $$ = build_bop(AND_EXPR,$1,$3); }
		     | bexpr OR   bexpr
                        { $$ = build_bop(OR_EXPR,$1,$3); }
                     | '(' bexpr ')'
                        { $$ = $2; }
		       ;

expr:
                      term
		    |  expr '^' expr
                        { $$ = build_op(EXP_EXPR,$1,$3); }
		    | expr '*' expr
                        { $$ = build_op(MULT_EXPR,$1,$3); }
		    | expr '/' expr
                        { $$ = build_op(DIV_EXPR,$1,$3); }
		    | expr '+' expr
                        { $$ = build_op(PLUS_EXPR,$1,$3); }
		    | expr '-' expr
                        { $$ = build_op(MINUS_EXPR,$1,$3); }
                    | '-' expr   %prec UNARY
                        { $$ = build_op(NEGATE_EXPR, $2, NULL); }
                    | '+' expr   %prec UNARY
                        { $$ = $2; }
                    | IF '(' bexpr ')' '{' expr '}' ELSE '{' expr '}'
                      { $$ = build_if_else($3, $6, $10);}
                    ;

term:
		      idexpr
                      { check_array_ref($1);}
                    | idexpr arrayref
                      { $$ = build_array_ref($1, $2);}
	            | INTCON
	            | FLOATCON
	            | '(' expr ')'
                         { $$ = $2; }
		    | '(' error ')'
                         { $$ = error_node; }
	            | idexpr '(' expr ')'
                          { $$ = build_op(CALL_EXPR, $1, $3); }
                    | SUM 
                       {insum++;}
                      '(' expr ','  idexpr  '=' expr ',' expr ')'
                          { $$ = build_sum( $4, $6, $8, $10); insum--;}
                      ;

idexpr:
                    ID
                     { $$ = current_id;}
                    ;

arrayref:            one_idx
                   | arrayref one_idx
                    { $$ = build_array_idx($1, $2); }
                    ;

one_idx: 
                   '[' expr ']'
                   { $$ = build_array_idx(NULL, $2); }
                    ;
                  
/**************************************************************
 *
 *    End of syntax specification
 *
 **************************************************************/
%%

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


