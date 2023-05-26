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

#ifndef MY_JET_1_H
#define MY_JET_1_H

/* *******  ***************  ******* */
/* *******  MY_JET_1_HEADER  ******* */
/* *******  ***************  ******* */
#define MY_JET_1_FIXES MY_JET_SPACE
#define MY_JET_1_TYPE MY_JET_TYPE1

#define MY_JET_1_API(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                     MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \
  MY_JET_APIS(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET_1_PREHEADER(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* HEADER " PREFIX_JET_1(t) " */\n" \

#define MY_JET_1_HEADER(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET_1_API(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
               MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET_1_POSTHEADER(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END HEADER " PREFIX_JET_1(t) " */\n" \


/* *******  *************  ******* */
/* *******  MY_JET_1_CODE  ******* */
/* *******  *************  ******* */
#define MY_JET_1_PRECODE(PREFIX_JET_1,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_COEF_MACROS(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\n\
/* CODE FOR " PREFIX_JET_1(t) " */\n\
int * " PREFIX_JET_1(monomial_counts) "(void) {return _" PREFIX_JET_1(monomial_counts) "_;}\n\
int * " PREFIX_JET_1(monomial_offsets) "(void) {return _" PREFIX_JET_1(monomial_offsets) "_;}\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
#include <string.h>\n\
\n"\

#define MY_JET_1_CODE(PREFIX_JET_1,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,\
                      MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,\
                      MYDATA_ACCESS) "\
\n\
static int " PREFIX_JET_1(flag_init_jet_library) "=0;\n\
static " I " " PREFIX_JET_1(nsymb) "=" MAX_NUM_SYMBOLS_NAME ";\n\
static " PREFIX_MYFLOAT(t) " " PREFIX_JET_1(faux) ";\n\
static " PREFIX_MYCOEF(t) " " PREFIX_JET_1(caux) ";\n\
#pragma omp threadprivate(" PREFIX_JET_1(flag_init_jet_library) "," PREFIX_JET_1(faux) "," PREFIX_JET_1(caux) ")\n\
\n" \
  "size_t " PREFIX_JET_1(init) "(" PREFIX_JET_1(ptr) " x)\n\
{\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\t*x = (__typeof__(*x)) malloc((" MAX_COEFFS_COUNT_NAME ")*sizeof(**x));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"not enough memory to allocate \%d\\n\"," MAX_COEFFS_COUNT_NAME ");\n\
\t\texit(1);\n\
\t}\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(init) "((*x)[k]);}\n\
\tmem+= (" MAX_COEFFS_COUNT_NAME ")*sizeof(**x);\n\
\treturn mem;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(initup2) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (" PREFIX_JET_1(flag_init_jet_library) "==1) return;\n\
\tif (deg != 1) {\n\
\t\tfprintf(stderr, \"%d: Not allowed deg value: \%d!=1\\n\",__LINE__,deg);\n\
\t\texit(1);\n\
\t}\n\
\tif (" MAX_NUM_SYMBOLS_NAME " < nsymbs) {\n\
\t\tfprintf(stderr, \"%d: Not allowed nsymbs value \%d<\%d\\n\",__LINE__," MAX_NUM_SYMBOLS_NAME ",nsymbs);\n\
\t\texit(1);\n\
\t}\n\
\t" PREFIX_JET_1(nsymb) " = nsymbs;\n\
\t" PREFIX_MYCOEF(initup) "();\n\
\t" PREFIX_MYFLOAT(init) "(" PREFIX_JET_1(faux) ");\n\
\t" PREFIX_MYCOEF(init) "(" PREFIX_JET_1(caux) ");\n\
\t" PREFIX_JET_1(flag_init_jet_library) "=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_1(get_num_symbs) "(void)\
{\
return " PREFIX_JET_1(nsymb) ";\
}\n" \
  "\n" \
  I " " PREFIX_JET_1(get_deg) "(void)\
{\
return 1;\
}\n" \
  "\n" \
  I " " PREFIX_JET_1(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\t" I " k;\n\
\tif (nsymbs > " MAX_NUM_SYMBOLS_NAME ") {\n\
\t\tfprintf(stderr, \"%d: Invalid num. variables \%d > \%d\\n\",__LINE__,nsymbs, " MAX_NUM_SYMBOLS_NAME ");\n\
\t\texit(1);\n\
\t}\n\
\t/* WARNING: in case of parallel region required out of the scope */\n\
\tk = " PREFIX_JET_1(nsymb) ";\n\
\t" PREFIX_JET_1(nsymb) " = nsymbs;\n\
\treturn k;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_1(set_deg) "(" I " deg)\n\
{\n\
\tfprintf(stderr, \"%d: Action not allowed\\n\",__LINE__);\n\
\texit(1);\n\
\treturn 0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(clean) "(" PREFIX_JET_1(ptr) " x)\n\
{\n\
\t" I " k;\n\
\tif (*x == NULL) return;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(clean) "((*x)[k]);}\n\
\tfree(*x); *x=NULL;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(cleanup) "(void)\n\
{\n\
\tif (" PREFIX_JET_1(flag_init_jet_library) "==0) return;\n\
\t" PREFIX_MYCOEF(clean) "(" PREFIX_JET_1(caux) ");\n\
\t" PREFIX_MYFLOAT(clean) "(" PREFIX_JET_1(faux) ");\n\
\t" PREFIX_MYCOEF(cleanup) "();\n\
\t" PREFIX_JET_1(nsymb) " = 0;\n\
\t" PREFIX_JET_1(flag_init_jet_library) "=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set) "(" PREFIX_JET_1(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_coef_array) "(" PREFIX_JET_1(t) " b, " PREFIX_MYCOEF(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n	\
}\n" \
 "\n" \
  "void " PREFIX_JET_1(set_jet_2_coef_array) "(" PREFIX_MYCOEF(t) " *b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n	\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_coef) "(" PREFIX_JET_1(t) " b, " PREFIX_MYCOEF(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_myfloat) "(" PREFIX_JET_1(t) " b, " PREFIX_MYFLOAT(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_myfloat) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(set_d) "(" PREFIX_JET_1(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_d) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(set_si) "(" PREFIX_JET_1(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_si) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  PREFIX_MYFLOAT(t) "* " PREFIX_JET_1(to_myfloat) "(" PREFIX_JET_1(t) " a) \
{ return " PREFIX_MYCOEF(to_myfloat) "(a[0]);}\n" \
  "\n" \
  "void " PREFIX_JET_1(eval) "(" PREFIX_MYCOEF(t) " t[1], " PREFIX_JET_1(t) " a, " PREFIX_MYFLOAT(t) " s[]) \n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_zero) "(*t);\n\
\tfor (k = 0; k < " PREFIX_JET_1(nsymb) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET_1(caux) ",a[k+1],s[k]);\n\
\t\t" PREFIX_MYCOEF(add2) "(*t,*t," PREFIX_JET_1(caux) ");\n\
\t}\n\
\t" PREFIX_MYCOEF(add2) "(*t,*t,a[0]);\n\
}\n" \
  "void " PREFIX_JET_1(nrminf) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {\n\
\t\t" PREFIX_MYCOEF(nrminf) "(&" PREFIX_JET_1(faux) ",a[k]);\n\
\t\tif (" PREFIX_MYFLOAT(lt) "(*nrm," PREFIX_JET_1(faux) ")) {" PREFIX_MYFLOAT(set) "(*nrm," PREFIX_JET_1(faux) ");}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(nrm2) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {\n\
\t\t" PREFIX_MYCOEF(nrm2) "(&" PREFIX_JET_1(faux) ",a[k]);\n\
\t\t" PREFIX_MYFLOAT(add2) "(*nrm,*nrm," PREFIX_JET_1(faux) ");\n\
\t}\n\
\t" PREFIX_MYFLOAT(set_sqrt) "(*nrm,*nrm);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(neg) "(" PREFIX_JET_1(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(neg) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(add2) "(c[k],a[k],b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(sub2) "(c[k],a[k],b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\t/* c' = a'b + ab' */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET_1(caux) ",a[k],b[0]);\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],a[0],b[k]);\n\
\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET_1(caux) ");\n\
\t}\n\
\t" PREFIX_MYCOEF(mul2) "(c[0],a[0],b[0]);\n\
}\n" \
  "void " PREFIX_JET_1(div2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\t/* c' = (a' - ab'/b)/b */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\n\
\t\t" PREFIX_MYCOEF(set) "(" PREFIX_JET_1(caux) ",a[k]);\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],a[0],b[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t\t" PREFIX_MYCOEF(sub2) "(c[k]," PREFIX_JET_1(caux) ",c[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
\t" PREFIX_MYCOEF(div2) "(c[0],a[0],b[0]);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_coef) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_coef) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(coef_sub2) "(" PREFIX_JET_1(t) " c, " PREFIX_MYCOEF(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2) "(c[0],b,a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_coef) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(mul2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_coef) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(div2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(coef_div2) "(" PREFIX_JET_1(t) " c, " PREFIX_MYCOEF(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(div2) "(c[0],b,a[0]);\n\
\t" PREFIX_MYCOEF(div2) "(" PREFIX_JET_1(caux) ",c[0],a[0]);\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ");\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(c[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_myfloat) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2_myfloat) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_myfloat) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_myfloat) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(myfloat_sub2) "(" PREFIX_JET_1(t) " c, " PREFIX_MYFLOAT(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_myfloat) " (c[0],a[0],b);\n\
\t" PREFIX_MYCOEF(neg) "(c[0],c[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_myfloat) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(mul2_myfloat) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_myfloat) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(div2_myfloat) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(myfloat_div2) "(" PREFIX_JET_1(t) " c, " PREFIX_MYFLOAT(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(myfloat_div2) "(c[0],b,a[0]);\n\
\t" PREFIX_MYCOEF(div2) "(" PREFIX_JET_1(caux) ",c[0],a[0]);\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ");\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(c[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2_d) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_d) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(d_sub2) "(" PREFIX_JET_1(t) " c, double b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_d) " (c[0],a[0],b);\n\
\t" PREFIX_MYCOEF(neg) "(c[0],c[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(mul2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(div2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(d_div2) "(" PREFIX_JET_1(t) " c, double b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(d_div2) "(c[0],b,a[0]);\n\
\t" PREFIX_MYCOEF(div2) "(" PREFIX_JET_1(caux) ",c[0],a[0]);\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ");\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(c[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2_si) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_si) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(si_sub2) "(" PREFIX_JET_1(t) " c, int b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_si) "(c[0],a[0],b);\n\
\t" PREFIX_MYCOEF(neg) "(c[0],c[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(mul2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {" PREFIX_MYCOEF(div2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(si_div2) "(" PREFIX_JET_1(t) " c, int b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(si_div2) "(c[0],b,a[0]);\n\
\t" PREFIX_MYCOEF(div2) "(" PREFIX_JET_1(caux) ",c[0],a[0]);\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ");\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(c[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_sqrt) "(" PREFIX_JET_1(t) " s, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* s'=a'/(2s[0]) */\n\
\t" PREFIX_MYCOEF(set_sqrt) "(s[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\n\
\t\t" PREFIX_MYCOEF(div2) "(s[k],a[k],s[0]);\n\
\t\t" PREFIX_MYCOEF(div2_si) "(s[k],s[k],2);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_pow_myfloat) "(" PREFIX_JET_1(t) " p, " PREFIX_JET_1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYFLOAT(sub2_si) "(" PREFIX_JET_1(faux) ",b,1);\n\
\t/* p'= b p[0]/a[0] a' */\n\
\t" PREFIX_MYCOEF(set_pow_myfloat) "(" PREFIX_JET_1(caux) ",a[0]," PREFIX_JET_1(faux) ");\n\
\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ",b);\n\
\t" PREFIX_MYCOEF(set_pow_myfloat) "(p[0],a[0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(p[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_exp) "(" PREFIX_JET_1(t) " e, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* e'=a'e[0] */\n\
\t" PREFIX_MYCOEF(set_exp) "(e[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(e[k],e[0],a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_log) "(" PREFIX_JET_1(t) " l, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* l'=a'/a[0] */\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(div2) "(l[k],a[k],a[0]);\
}\n\
\t" PREFIX_MYCOEF(set_log) "(l[0],a[0]);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_sin) "(" PREFIX_JET_1(t) " s, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* s'=cos(a[0])a' */\n\
\t" PREFIX_MYCOEF(set_cos) "(" PREFIX_JET_1(caux) ",a[0]);\n\
\t" PREFIX_MYCOEF(set_sin) "(s[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(s[k]," PREFIX_JET_1(caux) ",a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_cos) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* c'=-sin(a[0])a' */\n\
\t" PREFIX_MYCOEF(set_sin) "(" PREFIX_JET_1(caux) ",a[0]);\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ");\n\
\t" PREFIX_MYCOEF(set_cos) "(c[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(c[k]," PREFIX_JET_1(caux) ",a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_tan) "(" PREFIX_JET_1(t) " t, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* t'= (1+tan(a[0])^2)a'*/\n\
\t" PREFIX_MYCOEF(set_tan) "(t[0],a[0]);\n\
\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET_1(caux) ",t[0],t[0]);\n\
\t" PREFIX_MYCOEF(add2_si) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ",1);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(t[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_atan) "(" PREFIX_JET_1(t) " t, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* t'= a'/(1+(a[0])^2)*/\n\
\t" PREFIX_MYCOEF(set_atan) "(t[0],a[0]);\n\
\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET_1(caux) ",a[0],a[0]);\n\
\t" PREFIX_MYCOEF(add2_si) "(" PREFIX_JET_1(caux) "," PREFIX_JET_1(caux) ",1);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(div2) "(t[k],a[k]," PREFIX_JET_1(caux) ");\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_sinh) "(" PREFIX_JET_1(t) " s, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* s'=cosh(a[0])a' */\n\
\t" PREFIX_MYCOEF(set_cosh) "(" PREFIX_JET_1(caux) ",a[0]);\n\
\t" PREFIX_MYCOEF(set_sinh) "(s[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(s[k]," PREFIX_JET_1(caux) ",a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_cosh) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* c'=sinh(a[0])a' */\n\
\t" PREFIX_MYCOEF(set_sinh) "(" PREFIX_JET_1(caux) ",a[0]);\n\
\t" PREFIX_MYCOEF(set_cosh) "(c[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET_1(nsymb) "; k++) {\
" PREFIX_MYCOEF(mul2) "(c[k]," PREFIX_JET_1(caux) ",a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET_1(nsymb) "; k++) {\
\t\t" PREFIX_MYCOEF(fprintf) "(file,fmt,a[k]);\n\
\t}\n\
}\n" \
  "void " PREFIX_JET_1(fscanf) "(FILE *file, const char *fmt, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {\
\t\t" PREFIX_MYCOEF(fscanf) "(file, fmt, " MYDATA_ACCESS "(a,k));\n\
\t}\n\
}\n" \
  "static char *" PREFIX_JET_1(sscanf4) "(char *ptr, const char *myfmt, " PREFIX_JET_1(t) " a, int *l)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <" MAX_COEFFS_COUNT_NAME " && *ptr!=\'\\0\'; k++){\n\
\t\tptr = " PREFIX_MYCOEF(sscanf4) "(ptr, myfmt, " MYDATA_ACCESS "(a,k), l);\n\
\t}\n\
\tfor ( ; k <" MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set_zero) "(" MYDATA_ACCESS "(a,k));}\n\
\treturn ptr;\n\
}\n\
void " PREFIX_JET_1(sscanf) "(const char *str, const char *fmt, " PREFIX_JET_1(t) " a)\n\
{\n\
\tint l;\n\
\tconst int len=fmt?strnlen(fmt,128):0; /* NOTE: safety string length of 128 */\n\
\tchar myfmt[len+3], *mystr = strdup(str);\n\
\tsprintf(myfmt, \"%s%%n\",fmt);\n\
\t" PREFIX_JET_1(sscanf4) "(mystr, myfmt, a, &l);\n\
\tfree(mystr);\n\
}\n" \

#define MY_JET_1_POSTCODE(PREFIX_JET_1,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END CODE " PREFIX_JET_1(t) " */\n" \

#endif /* MY_JET_1_H */
