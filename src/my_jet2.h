/*********************************************************************
 *
 *       Taylor  
 *
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

#ifndef MY_JET2_H
#define MY_JET2_H


/* *******  **************  ******* */
/* *******  MY_JET2_HEADER  ******* */
/* *******  **************  ******* */

#define MY_MP2_FIXES(x) "mp2_"#x
#define MY_JET2_FIXES MY_JET_SPACE
#define MY_JET2_TYPE MY_JET_TYPE3

#define MY_JET2_API(PREFIX_JET2,MY_JET2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                    MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \
  MY_JET_APIS(PREFIX_JET2,MY_JET2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET2_PREHEADER(PREFIX_JET2,MY_JET2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* HEADER " PREFIX_JET2(t) " */\n"\

#define MY_JET2_HEADER(PREFIX_JET2,MY_JET2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                       MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET2_API(PREFIX_JET2,MY_JET2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET2_POSTHEADER(PREFIX_JET2,MY_JET2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                           MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END HEADER " PREFIX_JET2(t) " */\n" \


/* *******  *************  ******* */
/* *******  MY_MP2_HEADER  ******* */
/* *******  *************  ******* */
#define MY_JET2_MP_HEADER(PREFIX_MP2,PREFIX_MYCOEF,I,UI) "\
void " PREFIX_MP2(initup) "(" I ");\
\n\
void " PREFIX_MP2(cleanup) "(void);\
\n\
void " PREFIX_MP2(prxk) "(" I ");\
\n\
void " PREFIX_MP2(ntph) "(" I ");\
\n\
void " PREFIX_MP2(llex) "(" I " ," I "*," I ");\
\n\
void " PREFIX_MP2(exll) "(" I " *," I ");\
\n\
void " PREFIX_MP2(php_add) "(\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I ");\
\n\
void " PREFIX_MP2(php0_add) "(\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I ");\
 \n\
 void " PREFIX_MP2(sphp_add) "(" PREFIX_MYCOEF(t) ",\
 " PREFIX_MYCOEF(t) " *," I " ,\
 " PREFIX_MYCOEF(t) " *," I " ,\
 " PREFIX_MYCOEF(t) " *," I ");\
 \n\
 void " PREFIX_MP2(sphp0_add) "(" PREFIX_MYCOEF(t) ",\
 " PREFIX_MYCOEF(t) " *," I " ,\
 " PREFIX_MYCOEF(t) " *," I " ,\
 " PREFIX_MYCOEF(t) " *," I ");\
\n\
void " PREFIX_MP2(sphp_add_myfloat) "(" PREFIX_MYFLOAT(t) ",\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I ");\
\n\
void " PREFIX_MP2(sphp0_add_myfloat) "(" PREFIX_MYFLOAT(t) ",\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I ");\
\n\
void " PREFIX_MP2(php_add_si) "(" I ",\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I ");\
\n\
void " PREFIX_MP2(php0_add_si) "(" I ",\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I " ,\
" PREFIX_MYCOEF(t) " *," I ");\
\n\
\n" \

/* *******  ***********  ******* */
/* *******  MY_MP2_CODE  ******* */
/* *******  ***********  ******* */
#define MY_MP2_PRECODE(PREFIX_MP2,PREFIX_MYCOEF,I,UI) "\
" \

#define MY_MP2_CODE(PREFIX_MP2,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,UI) "\
static unsigned ** " PREFIX_MP2(clmo) "=NULL;\n\
static " I " **" PREFIX_MP2(psi) "=NULL;\n\
static " PREFIX_MYFLOAT(t) " *" PREFIX_JET2(v) "[2];\n\
\n\
static void " PREFIX_MP2(initup) "(" I " nr) {\n\
\t" I " k,j;\n\
\tfor (k = 0; k < 2; ++k) {\n\
\t\t" PREFIX_JET2(v) "[k] = (__typeof__(*v)) malloc((" MAX_DEGREE_NAME "+1)*sizeof(**v));\n\
\t\t\tif (" PREFIX_JET2(v) "[k]==NULL) {\n\
\t\t\t\tfprintf(stderr, \"\%s:\%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__," MAX_DEGREE_NAME ");\n\
\t\t\t\tfflush(stderr); exit(1);\n\
\t\t\t}\n\
\t\tfor (j = 0; j <= " MAX_DEGREE_NAME "; ++j) {" PREFIX_MYFLOAT(init) "(" PREFIX_JET2(v) "[k][j]);}\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(cleanup) "(" I " nr) {\n\
}\n\
\n\
static void " PREFIX_MP2(prxk) "(" I " k[]) {\n\
\tif (k[0] != 0) {k[0]--; k[1]++; return;} \n\
\tfprintf(stderr,\"\%s:\%d prxk error\\n\", __FILE__, __LINE__);\n\
\tfflush(stderr); exit(1);\n\
}\n\
\n\
static void " PREFIX_MP2(ntph) "(" I " no) {\
return no+1;\
}\n\
\n\
static void " PREFIX_MP2(llex) "(" I " lloc," I " k[]," I " no) {\n\
\tk[1] = lloc; k[0] = no - lloc;\n\
}\n\
\n\
static void " PREFIX_MP2(exll) "(" I " no," I " k[]) {\
return k[1];\
}\n\
\n\
static void " PREFIX_MP2(php_add) "(\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(php0_add) "(\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_MYCOEF(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(php_add) "(p3,g3,p1,g1,p2,g2);\n\
}\n\
\n\
static void " PREFIX_MP2(sphp_add) "(" PREFIX_MYCOEF(t) " alph,\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) "," PREFIX_JET2(caux) ",alph);\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(sphp0_add) "(" PREFIX_MYCOEF(t) " alph,\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_MYCOEF(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(sphp0_add) "(alph,p3,g3,p2,g2,p1,g1);\n\
}\n\
\n\
static void " PREFIX_MP2(php_add_myfloat) "(" PREFIX_MYFLOAT(t) " alph,\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET2(caux) "," PREFIX_JET2(caux) ",alph);\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(sphp0_add_myfloat) "(" PREFIX_MYFLOAT(t) " alph,\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_MYCOEF(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(sphp0_add_myfloat) "(alph,p3,g3,p2,g2,p1,g1);\n\
}\n\
\n\
static void " PREFIX_MP2(php_add_si) "(" I " alph,\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET2(caux) "," PREFIX_JET2(caux) ",alph);\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(php0_add_si) "(" I " alph,\
" PREFIX_MYCOEF(t) " *p3," I " g3,\
" PREFIX_MYCOEF(t) " *p1," I " g1,\
" PREFIX_MYCOEF(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_MYCOEF(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(php0_add_si) "(alph,p3,g3,p2,g2,p1,g1);\n\
}\n\
\n\
" \

#define MY_MP2_MACRO_CODE(PREFIX_MP2,PREFIX_JET2,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,UI) "\
#define " PREFIX_MP2(initup) "(nr) {\\\n\
  " PREFIX_JET2(v) "[0] = (__typeof__(" PREFIX_JET2(v) "[0])) malloc((nr+1)*sizeof(*" PREFIX_JET2(v) "[0]));\\\n\
  if (" PREFIX_JET2(v) "[0]==NULL){\\\n\
  fprintf(stderr, \"\%s:\%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__,nr);\\\n\
  fflush(stderr); exit(1);\\\n\
  }\\\n\
  for (k = 0; k <= nr; ++k) {\\\n\
  " PREFIX_MYFLOAT(init) "(" PREFIX_JET2(v) "[0][k]);\\\n\
  }\\\n\
  \\\n\
  " PREFIX_JET2(v) "[1] = (__typeof__(" PREFIX_JET2(v) "[1])) malloc((nr+1)*sizeof(*" PREFIX_JET2(v) "[1]));\\\n\
  if (" PREFIX_JET2(v) "[1]==NULL){\\\n\
  fprintf(stderr, \"\%s:\%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__,nr);\\\n\
  fflush(stderr); exit(1);\\\n\
  }\\\n\
  \\\n\
  for (k = 0; k <= nr; ++k) {\\\n\
  " PREFIX_MYFLOAT(init) "(" PREFIX_JET2(v) "[1][k]);\\\n\
  }\\\n\
  }\n\
\n\
#define " PREFIX_MP2(cleanup) "(nr) {\\\n\
  for (k = 0; k <= nr; ++k) {\\\n\
  " PREFIX_MYFLOAT(clean) "(" PREFIX_JET2(v) "[1][k]);\\\n\
  }\\\n\
  free(" PREFIX_JET2(v) "[1]); " PREFIX_JET2(v) "[1]=NULL;\\\n\
  \\\n\
  for (k = 0; k <= nr; ++k) {\\\n\
  " PREFIX_MYFLOAT(clean) "(" PREFIX_JET2(v) "[0][k]);\\\n\
  }\\\n\
  free(" PREFIX_JET2(v) "[0]); " PREFIX_JET2(v) "[0]=NULL;\\\n\
  }\n\
\n\
/* Assump. k[0]!=0 */\n\
#define " PREFIX_MP2(prxk) "(k) {k[0]--; k[1]++;}\n\
\n\
#define " PREFIX_MP2(ntph) "(no) no+1\n\
\n\
#define " PREFIX_MP2(llex) "(lloc,k,no) {k[1] = lloc; k[0] = no - lloc;}\n\
\n\
#define " PREFIX_MP2(exll) "(no,k) k[1]\n\
\n\
#define " PREFIX_MP2(php_add) "(\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g1; i++) for (j = 0; j <= g2; j++) {\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\\\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\\\n\
\t}\\\n\
}\n\
\n\
#define " PREFIX_MP2(php0_add) "(\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_MYCOEF(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(php_add) "(p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
#define " PREFIX_MP2(sphp_add) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g1; i++) for (j = 0; j <= g2; j++) {\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) "," PREFIX_JET2(caux) ",alph);\\\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\\\n\
\t}\\\n\
}\n\
#define " PREFIX_MP2(sphp0_add) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_MYCOEF(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(sphp_add) "(alph,p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
#define " PREFIX_MP2(sphp_add_myfloat) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g1; i++) for (j = 0; j <= g2; j++) {\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\\\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET2(caux) "," PREFIX_JET2(caux) ",alph);\\\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\\\n\
\t}\\\n\
}\n\
#define " PREFIX_MP2(sphp0_add_myfloat) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_MYCOEF(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(sphp_add_myfloat) "(alph,p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
#define " PREFIX_MP2(sphp_add_si) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g1; i++) for (j = 0; j <= g2; j++) {\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(caux) ",p1[i],p2[j]);\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET2(caux) "," PREFIX_JET2(caux) ",alph);\\\n\
\t\t" PREFIX_MYCOEF(add2) "(p3[i+j],p3[i+j]," PREFIX_JET2(caux) ");\\\n\
\t}\\\n\
}\n\
#define " PREFIX_MP2(sphp0_add_si) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_MYCOEF(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(sphp_add_si) "(alph,p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
" \


/* *******  ************  ******* */
/* *******  MY_JET2_CODE  ******* */
/* *******  ************  ******* */
#define MY_JET2_PRECODE(PREFIX_JET2,PREFIX_MP2,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_COEF_MACROS(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\n\
/* CODE FOR " PREFIX_JET2(t) " */\n\
int * " PREFIX_JET2(monomial_counts) "(void) {return _" PREFIX_JET2(monomial_counts) "_;}\n\
int * " PREFIX_JET2(monomial_offsets) "(void) {return _" PREFIX_JET2(monomial_offsets) "_;}\n\
\n" \

#define MY_JET2_CODE(PREFIX_JET2,PREFIX_MP2,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,\
                     MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,\
                     MYDATA_ACCESS) "\
" MY_MP2_MACRO_CODE(PREFIX_MP2,PREFIX_JET2,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,I) "\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n\
#define nc " PREFIX_JET2(nc) "\n\
#define nct " PREFIX_JET2(nct) "\n\
#ifdef " MAX_COEFFS_COUNT_NAME "\n\
static " I " *nc=_" PREFIX_JET2(monomial_counts) "_,nct=" MAX_COEFFS_COUNT_NAME ";\n\
#else\n\
static " I " *nc=NULL,nct=0;\n\
#endif\n\
#define nct " PREFIX_JET2(nct) "\n\
static int " PREFIX_JET2(flag_init_jet_library) "=0;\n\
static " I " " PREFIX_JET2(deg) "=" MAX_DEGREE_NAME ";\n\
static " PREFIX_MYFLOAT(t) " " PREFIX_JET2(faux) ", *" PREFIX_JET2(v) "[2];\n\
static " PREFIX_MYCOEF(t) " " PREFIX_JET2(caux) ";\n\
static " PREFIX_JET2(t) " " PREFIX_JET2(jaux) "=NULL;\n\
#pragma omp threadprivate(" PREFIX_JET2(deg) "," PREFIX_JET2(flag_init_jet_library) "," PREFIX_JET2(v) "," PREFIX_JET2(faux) "," PREFIX_JET2(caux) "," PREFIX_JET2(jaux) ")\n\
  \n" \
  "\n" \
  "size_t " PREFIX_JET2(init) "(" PREFIX_JET2(ptr) " x)\n\
{\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\t*x = (__typeof__(*x)) malloc((" MAX_DEGREE_NAME "+1)*sizeof(**x));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"\%s:\%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__," MAX_DEGREE_NAME ");\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\tmem+= (" MAX_DEGREE_NAME "+1)*sizeof(**x);\n\
\t(*x)[0] = (__typeof__(**x)) malloc(nct*sizeof(***x));\n\
\tif ((*x)[0] == NULL) {\n\
\t\tfprintf(stderr, \"\%s:\%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__," MAX_DEGREE_NAME ");\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\tmem+= nct*sizeof(***x);\n\
\tfor (k = 0; k < nct; k++) {" PREFIX_MYCOEF(init) "((*x)[0][k]);}\n\
\tfor (k = 1; k <= " MAX_DEGREE_NAME "; k++) {(*x)[k] = (*x)[k-1] + nc[k-1];}\n\
\treturn mem;\n\
}\n" \
  "void " PREFIX_JET2(initup2) "(" I " nsymb, " I " deg)\n\
{\n\
\tif (" PREFIX_JET2(flag_init_jet_library) "==1) return;\n\
#pragma omp single\n\
{\n\
\t" I " k, j;\n\
\tif (nsymb != 2) {\n\
\t\tfprintf(stderr, \
\"\%s:\%d Number of symbols \%d!=2 not allowed\\n\",__FILE__,__LINE__,nsymb);\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\tif (deg > " MAX_DEGREE_NAME ") {\n\
\t\tfprintf(stderr, \
\"\%s:\%d Maximum degree not allowed \%d!=\%d not allowed\\n\",__FILE__,__LINE__,deg," MAX_DEGREE_NAME ");\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\t" PREFIX_JET2(deg) " = deg;\n\
\t" PREFIX_MYCOEF(initup) "();\n\
\t" PREFIX_MYFLOAT(init) "(" PREFIX_JET2(faux) ");\n\
\t" PREFIX_MYCOEF(init) "(" PREFIX_JET2(caux) ");\n\
\t" PREFIX_MP2(initup) "(" MAX_DEGREE_NAME ");\n\
\tif (nc==NULL) {\n\
\t\tnc = (__typeof__(nc)) malloc((" MAX_DEGREE_NAME ")*sizeof(*nc));\n\
\t\tif (nc==NULL) {\n\
\t\t\tfprintf(stderr, \"\%s:\%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__," MAX_DEGREE_NAME ");\n\
\t\t\tfflush(stderr); exit(1);\n\
\t\t}\n\
\t\tnct=0;\n\
\t\tfor (k = 0; k <= " MAX_DEGREE_NAME "; ++k) {\n\
\t\t\tnc[k] = " PREFIX_MP2(ntph) "(k);\n\
\t\t\tnct+= nc[k];\n\
\t\t}\n\
\t}\n\
}\n\
\t" PREFIX_JET2(init) "(&" PREFIX_JET2(jaux) ");\n\
\t\n\
\t" PREFIX_JET2(flag_init_jet_library) "=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET2(get_num_symbs) "(void)\n\
{\
return 2;\
}\n" \
  "\n" \
  I " " PREFIX_JET2(get_deg) "(void)\n\
{\
return " PREFIX_JET2(deg) ";\
}\n" \
  "\n" \
  I " " PREFIX_JET2(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\tfprintf(stderr, \"\%s:\%d Action not allowed\\n\",__FILE__,__LINE__);\n\
\tfflush(stderr); exit(1);\n\
\treturn 0;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET2(set_deg) "(" I " deg)\n\
{\n\
\t" I " k;\n\
\tif (deg > " MAX_DEGREE_NAME ") {\n\
\t\tfprintf(stderr, \"\%s:\%d Invalid degree \%d > \%d\\n\",__FILE__,__LINE__, deg, " MAX_DEGREE_NAME ");\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\tk = " PREFIX_JET2(deg) ";\n\
\t" PREFIX_JET2(deg) " = deg;\n\
\treturn k;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(clean) "(" PREFIX_JET2(ptr) " x)\n\
{\n\
\t" I " k;\n\
\tif (*x == NULL) return;\n\
\tfor (k = 0; k < nct; k++) {" PREFIX_MYCOEF(clean) "((*x)[0][k]);}\n\
\tfor (k = 1; k <= " MAX_DEGREE_NAME "; k++) {(*x)[k]=NULL;}\n\
\tfree((*x)[0]); (*x)[0]=NULL;\n\
\tfree(*x); *x=NULL;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(cleanup) "(void)\n\
{\n\
\t" I " k;\n\
\tif (" PREFIX_JET2(flag_init_jet_library) "==0) return;\n\
\t" PREFIX_JET2(clean) "(&" PREFIX_JET2(jaux) ");\n\
\t" PREFIX_MYCOEF(clean) "(" PREFIX_JET2(caux) ");\n\
\t" PREFIX_MYFLOAT(clean) "(" PREFIX_JET2(faux) ");\n\
#pragma omp single\n\
{\n\
\t" PREFIX_MP2(cleanup) "(" MAX_DEGREE_NAME ");\n\
\t" PREFIX_MYCOEF(cleanup) "();\n\
}\n\
\t" PREFIX_JET2(deg) " = 0;\n\
\t" PREFIX_JET2(flag_init_jet_library) "=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set) "(" PREFIX_JET2(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(b[k][j],a[k][j]);\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_coef) "(" PREFIX_JET2(t) " b, " PREFIX_MYCOEF(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set) "(b[0][0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set_zero) "(b[k][j]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_myfloat) "(" PREFIX_JET2(t) " b, " PREFIX_MYFLOAT(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_myfloat) "(b[0][0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set_zero) "(b[k][j]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_d) "(" PREFIX_JET2(t) " b, double a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_d) "(b[0][0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set_zero) "(b[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(set_si) "(" PREFIX_JET2(t) " b, int a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_si) "(b[0][0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set_zero) "(b[k][j]);}\n\
}\n" \
  "\n"\
  "void " PREFIX_JET2(set_coef_array) "(" PREFIX_JET2(t) " b, " PREFIX_MYCOEF(t) " *a)\n\
{\n\
\t" I " j,k,m=1;\n\
\t" PREFIX_MYCOEF(set) "(b[0][0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(b[k][j],a[m]);m++;}\n\
}\n" \
  "void " PREFIX_JET2(set_jet_2_coef_array) "(" PREFIX_MYCOEF(t) " *b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,m=1;\n\
\t" PREFIX_MYCOEF(set) "(b[0],a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(b[m],a[k][j]);m++;}\n\
}\n" \
  PREFIX_MYFLOAT(t) "* " PREFIX_JET2(to_myfloat) "(" PREFIX_JET2(t) " a) \
{ return " PREFIX_MYCOEF(to_myfloat) "(a[0][0]); }\n" \
  "\n" \
  "void " PREFIX_JET2(eval) "(" PREFIX_MYCOEF(t) " t[1], " PREFIX_JET2(t) " a, " PREFIX_MYFLOAT(t) " s[]) \n\
{\n\
\t" I " k[2], i,j;\n\
\t\n\
\tfor (i = 0; i < 2; ++i) {\n\
\t\t" PREFIX_MYFLOAT(set_si) "(" PREFIX_JET2(v) "[i][0], 1);\n\
\t\tfor (j = 1; j <= " PREFIX_JET2(deg) "; ++j) {\n\
\t\t\t" PREFIX_MYFLOAT(mul2) "(" PREFIX_JET2(v) "[i][j], s[i], " PREFIX_JET2(v) "[i][j-1]);\n\
\t\t}\n\
\t}\n\
\t\n\
\t" PREFIX_MYCOEF(set_zero) "(*t);\n\
\tfor (i = " PREFIX_JET2(deg) "; i > 0; --i) for (j = 0; j < nc[i]; ++j) {\n\
\t\t" PREFIX_MP2(llex) "(j,k,i);\n\
\t\t" PREFIX_MYFLOAT(mul2) "(" PREFIX_JET2(faux) ", " PREFIX_JET2(v) "[0][k[0]]," PREFIX_JET2(v) "[1][k[1]]);\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET2(caux) ",a[i][j]," PREFIX_JET2(faux) ");\n\
\t\t" PREFIX_MYCOEF(add2) "(*t,*t," PREFIX_JET2(caux) ");\n\
\t}\n\
\t" PREFIX_MYCOEF(add2) "(*t,*t,a[0][0]);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(nrminf) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_MYCOEF(nrminf) "(&" PREFIX_JET2(faux) ",a[k][j]);\n\
\t\tif (" PREFIX_MYFLOAT(lt) "(*nrm," PREFIX_JET2(faux) ")) \
{" PREFIX_MYFLOAT(set) "(*nrm," PREFIX_JET2(faux) ");}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(nrm2) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_MYCOEF(nrm2) "(&" PREFIX_JET2(faux) ",a[k][j]);\n\
\t\t" PREFIX_MYFLOAT(add2) "(*nrm,*nrm," PREFIX_JET2(faux) ");\n\
\t}\n\
\t" PREFIX_MYFLOAT(set_sqrt) "(*nrm,*nrm);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(neg) "(" PREFIX_JET2(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(neg) "(b[k][j],a[k][j]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(add2) "(c[k][j],a[k][j],b[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(sub2) "(c[k][j],a[k][j],b[k][j]);}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET2(set_sq) "(s,a)\\\n\
{\\\n\
\t" I " j,k,ii,jj, nk;\\\n\
\t" PREFIX_MYCOEF(mul2) "(s[0][0],a[0][0],a[0][0]);\\\n\
\tnk = " PREFIX_JET2(deg) " - (" PREFIX_JET2(deg) "%2==1);\\\n\
\tfor (k = 1; k < nk; k++) {\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_MP2(php0_add) "(s[k],k,a[0],0,a[k],k,ii,jj);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \\\n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[k-j],k-j,ii,jj);}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) \\\n\t\t{\
" PREFIX_MYCOEF(mul2_si) "(s[k][j],s[k][j],2);}\\\n\
\t\tk++;\\\n\
\t\t/* k even */\\\n\
\t\t" PREFIX_MP2(php0_add) "(s[k],k,a[0],0,a[k],k,ii,jj);\\\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) \\\n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[k-j],k-j,ii,jj);}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) \\\n\t\t{\
" PREFIX_MYCOEF(mul2_si) "(s[k][j],s[k][j],2);}\\\n\
\t\tj = k/2;\\\n\
\t\t" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[j],j,ii,jj);\\\n\
\t}\\\n\
\tif (" PREFIX_JET2(deg) "%2==1) {\\\n\
\t\tk=" PREFIX_JET2(deg) ";\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_MP2(php0_add) "(s[k],k,a[0],0,a[k],k,ii,jj);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \\\n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[k-j],k-j,ii,jj);}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) \\\n\t\t{\
" PREFIX_MYCOEF(mul2_si) "(s[k][j],s[k][j],2);}\\\n\
\t}\\\n\
}\n" \
  "void " PREFIX_JET2(mul2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\tif (a==b) {\
" PREFIX_JET2(set_sq) "(c,a);\
return;}\n\
\t" I " j,k,ii,jj;\n\
\t/* c_k = sum(a_{j}b_{k-j}, j=0,...,k) */\n\
\t" PREFIX_MYCOEF(mul2) "(c[0][0],a[0][0],b[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,a[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j <= k; j++) {\n\
\t\t\t" PREFIX_MP2(php_add) "(c[k],k,a[j],j,b[k-j],k-j,ii,jj);\n\
\t\t}\n\
\t}\n\
}\n" \
  "void " PREFIX_JET2(div2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(div2) "(c[0][0],a[0][0],b[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(sub2) "(c[k][j],a[k][j],c[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_coef) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(add2) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_coef) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(sub2) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(coef_sub2) "(" PREFIX_JET2(t) " c, " PREFIX_MYCOEF(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(sub2) "(c[0][0],b,a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_coef) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(mul2) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_coef) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(div2) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(coef_div2) "(" PREFIX_JET2(t) " c, " PREFIX_MYCOEF(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(div2) "(c[0][0],a,b[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_myfloat) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(add2_myfloat) "(c[0][0],a[0][0],b);\n\
\tif (c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_myfloat) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(sub2_myfloat) "(c[0][0],a[0][0],b);\n\
\tif (c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(myfloat_sub2) "(" PREFIX_JET2(t) " c, " PREFIX_MYFLOAT(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_myfloat) "(" PREFIX_JET2(caux) ",b);\n\
\t" PREFIX_MYCOEF(sub2) "(c[0][0]," PREFIX_JET2(caux) ",a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_myfloat) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(mul2_myfloat) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_myfloat) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(div2_myfloat) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(myfloat_div2) "(" PREFIX_JET2(t) " c, " PREFIX_MYFLOAT(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_myfloat) "(" PREFIX_JET2(caux) ",a);\n\
\t" PREFIX_MYCOEF(div2) "(c[0][0]," PREFIX_JET2(caux) ",b[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(add2_d) "(c[0][0],a[0][0],b);\n\
\tif (c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(sub2_d) "(c[0][0],a[0][0],b);\n\
\tif (c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(d_sub2) "(" PREFIX_JET2(t) " c, double b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_d) "(" PREFIX_JET2(caux) ",b);\n\
\t" PREFIX_MYCOEF(sub2) "(c[0][0]," PREFIX_JET2(caux) ",a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(mul2_d) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(div2_d) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(d_div2) "(" PREFIX_JET2(t) " c, double a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_d) "(" PREFIX_JET2(caux) ",a);\n\
\t" PREFIX_MYCOEF(div2) "(c[0][0]," PREFIX_JET2(caux) ",b[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(add2_si) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(sub2_si) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(si_sub2) "(" PREFIX_JET2(t) " c, int b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_si) "(" PREFIX_JET2(caux) ",b);\n\
\t" PREFIX_MYCOEF(sub2) "(c[0][0]," PREFIX_JET2(caux) ",a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(mul2_si) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_MYCOEF(div2_si) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(si_div2) "(" PREFIX_JET2(t) " c, int a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_si) "(" PREFIX_JET2(caux) ",a);\n\
\t" PREFIX_MYCOEF(div2) "(c[0][0]," PREFIX_JET2(caux) ",b[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_sqrt) "(" PREFIX_JET2(t) " s, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj, nk;\n\
#define tmp " PREFIX_JET2(jaux) "[0][0]\n\
\t" PREFIX_MYCOEF(set_sqrt) "(s[0][0],a[0][0]);\n\
\t" PREFIX_MYCOEF(mul2_si) "(tmp,s[0][0],2);\n\
\tnk = " PREFIX_JET2(deg) " - (" PREFIX_JET2(deg) "%2==1);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t/* k odd */\n\
\t\tfor (j = 0; j < nc[k]; j++) {" PREFIX_MYCOEF(set_zero) "(s[k][j]);}\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,s[j],j,s[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(s[k][j],s[k][j],2);\n\
\t\t\t" PREFIX_MYCOEF(sub2) "(s[k][j],a[k][j],s[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(s[k][j],s[k][j],tmp);\n\
\t\t}\n\
\t\tk++;\n\
\t\t/* k even */\n\
\t\tfor (j = 0; j < nc[k]; j++) {" PREFIX_MYCOEF(set_zero) "(s[k][j]);}\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(2,s[k],k,s[j],j,s[k-j],k-j,ii,jj);}\n\
\t\tj = k/2;\n\
\t\t" PREFIX_MP2(php_add) "(s[k],k,s[j],j,s[j],j,ii,jj);\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(sub2) "(s[k][j],a[k][j],s[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(s[k][j],s[k][j],tmp);\n\
\t\t}\n\
\t}\n\
\tif (" PREFIX_JET2(deg) "%2==1) {\n\
\t\tk=" PREFIX_JET2(deg) ";\n\
\t\t/* k odd */\n\
\t\tfor (j = 0; j < nc[k]; j++) {" PREFIX_MYCOEF(set_zero) "(s[k][j]);}\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,s[j],j,s[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET2(caux) ",s[k][j],2);\n\
\t\t\t" PREFIX_MYCOEF(sub2) "(s[k][j],a[k][j]," PREFIX_JET2(caux) ");\n\
\t\t\t" PREFIX_MYCOEF(div2) "(s[k][j],s[k][j],tmp);\n\
\t\t}\n\
\t}\n\
#undef tmp\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_pow_myfloat) "(" PREFIX_JET2(t) " p, " PREFIX_JET2(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_pow_myfloat) "(p[0][0],a[0][0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MYFLOAT(mul2_si) "(" PREFIX_JET2(faux) ",b,k);\n\
\t\t" PREFIX_MP2(sphp0_add_myfloat) "(" PREFIX_JET2(faux) ",p[k],k,a[k],k,p[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYFLOAT(mul2_si) "(" PREFIX_JET2(faux) ",b,k-j);\n\
\t\t\t" PREFIX_MYFLOAT(sub2_si) "(" PREFIX_JET2(faux) "," PREFIX_JET2(faux) ",j);\n\
\t\t\t" PREFIX_MP2(sphp_add_myfloat) "(" PREFIX_JET2(faux) ",p[k],k,a[k-j],k-j,p[j],j,ii,jj);\n\
\t\t}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET2(caux) ",a[0][0],k);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(p[k][j],p[k][j]," PREFIX_JET2(caux) ");\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_exp) "(" PREFIX_JET2(t) " e, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_exp) "(e[0][0],a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,e[k],k,a[k],k,e[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(k-j,e[k],k,a[k-j],k-j,e[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) \n\t\t{\
" PREFIX_MYCOEF(div2_si) "(e[k][j],e[k][j],k);}\n\
\t}\n\
}\n" \
  "void " PREFIX_JET2(set_log) "(" PREFIX_JET2(t) " l, " PREFIX_JET2(t) " a)\n\
{\n\
\t/* Assump. l!=a */\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_log) "(l[0][0],a[0][0]);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(l[k],k,a[k-1],k-1,l[1],1,ii,jj);\n\
\t\tfor (j = 2; j < k; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(j,l[k],k,a[k-j],k-j,l[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(l[k][j],l[k][j],k);\n\
\t\t\t" PREFIX_MYCOEF(sub2) "(l[k][j],a[k][j],l[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(l[k][j],l[k][j],a[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET2(sincos) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k,ii,jj;\\\n\
\t" PREFIX_MYCOEF(set_sin) "(s[0][0],a[0][0]);\\\n\
\t" PREFIX_MYCOEF(set_cos) "(c[0][0],a[0][0]);\\\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\\\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,s[k],k,a[k],k,c[0],0,ii,jj);\\\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,c[k],k,a[k],k,s[0],0,ii,jj);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_MP2(sphp_add_si) "(k-j,s[k],k,a[k-j],k-j,c[j],j,ii,jj);\\\n\
\t\t\t" PREFIX_MP2(sphp_add_si) "(k-j,c[k],k,a[k-j],k-j,s[j],j,ii,jj);\\\n\
\t\t}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(s[k][j],s[k][j],k);\\\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(c[k][j],c[k][j],-k);\\\n\
\t\t}\\\n\
\t}\\\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_cos) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sincos) "(" PREFIX_JET2(jaux) ",c,a);}\n" \
  "\n" \
  "void " PREFIX_JET2(set_sin) "(" PREFIX_JET2(t) " s, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sincos) "(s," PREFIX_JET2(jaux) ",a);}\n" \
  "\n" \
  "void " PREFIX_JET2(set_tan) "(" PREFIX_JET2(t) " t, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj ,nk;\n\
\t" PREFIX_MYCOEF(set_tan) "(t[0][0],a[0][0]);\n\
\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET2(jaux) "[0][0],t[0][0],t[0][0]);\n\
\tnk = " PREFIX_JET2(deg) " - (" PREFIX_JET2(deg) "%2==1);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,t[k],k,a[k],k," PREFIX_JET2(jaux) "[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(k-j,t[k],k,a[k-j],k-j," PREFIX_JET2(jaux) "[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k][j],t[k][j],a[k][j]);\n\
\t\t}\n\
\t\t/* k odd */\n\
\t\t" PREFIX_MP2(php0_add) "(" PREFIX_JET2(jaux) "[k],k,t[0],0,t[k],k,ii,jj);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++)\n\t\t{\
" PREFIX_MP2(php_add) "(" PREFIX_JET2(jaux) "[k],k,t[j],j,t[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++)\n\t\t{\
" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET2(jaux) "[k][j]," PREFIX_JET2(jaux) "[k][j],2);}\n\
\t\tk++;\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,t[k],k,a[k],k," PREFIX_JET2(jaux) "[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(k-j,t[k],k,a[k-j],k-j," PREFIX_JET2(jaux) "[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k][j],t[k][j],a[k][j]);\n\
\t\t}\n\
\t\t/* k even */\n\
\t\t" PREFIX_MP2(php0_add) "(" PREFIX_JET2(jaux) "[k],k,t[0],0,t[k],k,ii,jj);\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(" PREFIX_JET2(jaux) "[k],k,t[j],j,t[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) \n\t\t{\
" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET2(jaux) "[k][j]," PREFIX_JET2(jaux) "[k][j],2);}\n\
\t\tj = k/2;\n\
\t\t" PREFIX_MP2(php_add) "(" PREFIX_JET2(jaux) "[k],k,t[j],j,t[j],j,ii,jj);\n\
\t}\n\
\tif (" PREFIX_JET2(deg) "%2==1) {\n\
\t\tk=" PREFIX_JET2(deg) ";\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,t[k],k,a[k],k," PREFIX_JET2(jaux) "[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(k-j,t[k],k,a[k-j],k-j," PREFIX_JET2(jaux) "[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k][j],t[k][j],a[k][j]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_atan) "(" PREFIX_JET2(t) " t, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_MYCOEF(set_atan) "(t[0][0],a[0][0]);\n\
\t" PREFIX_JET2(set_sq) "(" PREFIX_JET2(jaux) ",a);\n\
\t" PREFIX_JET2(add2_si) "(" PREFIX_JET2(jaux) "," PREFIX_JET2(jaux) ",1);\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\n\
\t\tfor (j = 0; j < nc[k]; j++) \n\t\t{\
" PREFIX_MYCOEF(set_zero) "(t[k][j]);}\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(sphp_add_si) "(j,t[k],k,t[j],j," PREFIX_JET2(jaux) "[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_MYCOEF(sub2) "(t[k][j],a[k][j],t[k][j]);\n\
\t\t\t" PREFIX_MYCOEF(div2) "(t[k][j],t[k][j]," PREFIX_JET2(jaux) "[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET2(sinhcosh) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k,ii,jj;\\\n\
\t" PREFIX_MYCOEF(set_sinh) "(s[0][0],a[0][0]);\\\n\
\t" PREFIX_MYCOEF(set_cosh) "(c[0][0],a[0][0]);\\\n\
\tfor (k = 1; k <= " PREFIX_JET2(deg) "; k++) {\\\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,s[k],k,a[k],k,c[0],0,ii,jj);\\\n\
\t\t" PREFIX_MP2(sphp0_add_si) "(k,c[k],k,a[k],k,s[0],0,ii,jj);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_MP2(sphp_add_si) "(k-j,s[k],k,a[k-j],k-j,c[j],j,ii,jj);\\\n\
\t\t\t" PREFIX_MP2(sphp_add_si) "(k-j,c[k],k,a[k-j],k-j,s[j],j,ii,jj);\\\n\
\t\t}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(s[k][j],s[k][j],k);\\\n\
\t\t\t" PREFIX_MYCOEF(div2_si) "(c[k][j],c[k][j],k);\\\n\
\t\t}\\\n\
\t}\\\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET2(set_cosh) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sinhcosh) "(" PREFIX_JET2(jaux) ",c,a);}\n" \
  "\n" \
  "void " PREFIX_JET2(set_sinh) "(" PREFIX_JET2(t) " s, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sinhcosh) "(s," PREFIX_JET2(jaux) ",a);}\n" \
  "\n" \
  "" PREFIX_MYCOEF(t) " * " PREFIX_JET2(get_coef) "(" PREFIX_JET2(t) " a, const " I " k[])\n\
{\n\
\t" I " no=k[0]+k[1],ll=" PREFIX_MP2(exll) "(no,k);\n\
\treturn &a[no][ll];\n\
}\n" \
  "\n" \
  "" PREFIX_MYCOEF(t) " * " PREFIX_JET2(coef0) "(" PREFIX_JET2(t) " a)\n\
{\n\
\treturn &a[0][0];\n\
}\n" \
  "\n" \
  "" PREFIX_MYCOEF(t) " * " PREFIX_JET2(coef1) "(" PREFIX_JET2(t) " a, " I " idx)\n\
{\n\
\tfprintf(stderr,\"\%s:\%d to implement\\n\",__FILE__,__LINE__); fflush(stderr); exit(2);\n\
\treturn &a[1][1-idx];\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(fprintf) "(FILE *file, const char* fmt, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) \
for (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_MYCOEF(fprintf) "(file, fmt, a[k][j]);\n\
\t}\n\
}\n\
void " PREFIX_JET2(fscanf) "(FILE *file, const char* fmt, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) \
for (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_MYCOEF(fscanf) "(file, fmt, a[k][j]);\n\
\t}\n\
}\n\
static char * " PREFIX_JET2(sscanf4) "(char *ptr, const char* myfmt, " PREFIX_JET2(t) " a, int *l)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) \
for (j = 0; j < nc[k]; j++) {\n\
\t\tptr = " PREFIX_MYCOEF(sscanf4) "(ptr, myfmt, a[k][j], l);\n\
\t\tif (*ptr == \'\\0\') goto end;\n\
\t}\n\
end:\n\
\tfor (k = 0; k <= " PREFIX_JET2(deg) "; k++) \
\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_MYCOEF(set_zero) "(a[k][j]);\n\
\t}\n\
\treturn ptr;\n\
}\n\
void " PREFIX_JET2(sscanf) "(const char *str, const char *fmt, " PREFIX_JET2(t) " a)\n\
{\n\
\tint l;\n\
\tconst int len=fmt?strnlen(fmt,128):0; /* NOTE: safety string length of 128 */\n\
\tchar myfmt[len+3], *mystr = strdup(str);\n\
\tsprintf(myfmt, \"%s%%n\",fmt);\n\
\t" PREFIX_JET2(sscanf4) "(mystr, myfmt, a, &l);\n\
\tfree(mystr);\n\
}\n\
#undef nct\n\
#undef nc\n" \

#define MY_JET2_POSTCODE(PREFIX_JET2,PREFIX_MP2,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END CODE " PREFIX_JET2(t) " */\n" \

#endif /* MY_JET2_H */
