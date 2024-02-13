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

#ifndef MY_JET_2_H
#define MY_JET_2_H

/* *******  ***************  ******* */
/* *******  MY_JET_2_HEADER  ******* */
/* *******  ***************  ******* */
#define MY_JET_2_FIXES MY_JET_SPACE
#define MY_JET_2_TYPE MY_JET_TYPE1

#define MY_JET_2_API(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                     MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \
    MY_JET_APIS(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET_2_PREHEADER(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                           MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* HEADER " PREFIX_JET_2(t) " */\n" \

#define MY_JET_2_HEADER(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET_2_API(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
               MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET_2_POSTHEADER(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END HEADER " PREFIX_JET_2(t) " */\n" \

/* *******  *************  ******* */
/* *******  MY_JET_2_CODE  ******* */
/* *******  *************  ******* */
#define MY_JET_2_PRECODE(PREFIX_JET_2,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_COEF_MACROS(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\n\
/* CODE FOR " PREFIX_JET_2(t) " */\n\
int * " PREFIX_JET_2(monomial_counts) "(void) {return _" PREFIX_JET_2(monomial_counts) "_;}\n\
int * " PREFIX_JET_2(monomial_offsets) "(void) {return _" PREFIX_JET_2(monomial_offsets) "_;}\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
#include <string.h>\n\
\n"\

#define MY_JET_2_CODE(PREFIX_JET_2,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,\
                      MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,\
                      MYDATA_ACCESS) "\
static int " PREFIX_JET_2(flag_init_jet_library) "=0;\n\
static " I " " PREFIX_JET_2(nsymb) "=_NUMBER_OF_JET_VARS_;\n\
static " PREFIX_MYFLOAT(t) " " PREFIX_JET_2(faux) ";\n\
static " PREFIX_MYCOEF(t) " " PREFIX_JET_2(caux) ";\n\
static " PREFIX_JET_2(t) " " PREFIX_JET_2(jaux) ";\n\
#pragma omp threadprivate(" PREFIX_JET_2(flag_init_jet_library) "," PREFIX_JET_2(faux) "," PREFIX_JET_2(caux) "," PREFIX_JET_2(jaux) ")\n\
\n\
size_t " PREFIX_JET_2(init) "(" PREFIX_JET_2(ptr) " x) {\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\t*x=(" PREFIX_JET_2(t) ")malloc((" MAX_COEFFS_COUNT_NAME ") * sizeof(" PREFIX_MYCOEF(t) "));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"not enough memory to allocate \%d\\n\"," MAX_COEFFS_COUNT_NAME ");\n\
\t\texit(1);\n\
\t}\n\
\tfor(k = 0;k < " MAX_COEFFS_COUNT_NAME "; k++){" PREFIX_MYCOEF(init) "((*x)[k]);}\n\
\tmem+= (" MAX_COEFFS_COUNT_NAME ")*sizeof(" PREFIX_MYCOEF(t) ");\n\
\treturn mem;\n\
}\n\
void " PREFIX_JET_2(initup2) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (" PREFIX_JET_2(flag_init_jet_library) "==1) return;\n\
\tif (deg != 2) {\n\
\t\tfprintf(stderr, \"%d: Not allowed deg value: \%d!=2\\n\",__LINE__,deg);\n\
\t\texit(1);\n\
\t}\n\
\tif (" MAX_NUM_SYMBOLS_NAME " < nsymbs) {\n\
\t\tfprintf(stderr, \"%d: Not allowed nsymbs value \%d<\%d\\n\",__LINE__," MAX_NUM_SYMBOLS_NAME ",nsymbs);\n\
\t\texit(1);\n\
\t}\n\
\t" PREFIX_JET_2(nsymb) " = nsymbs;\n\
\t" PREFIX_MYCOEF(initup) "();\n\
\t" PREFIX_MYCOEF(init) "(" PREFIX_JET_2(caux) ");\n\
\t" PREFIX_MYFLOAT(init) "(" PREFIX_JET_2(faux) ");\n\
\t" PREFIX_JET_2(init) "(&" PREFIX_JET_2(jaux) ");\n\
\t" PREFIX_JET_2(flag_init_jet_library) "=1;\n\
}\n" \
  I " " PREFIX_JET_2(get_num_symbs) "(void)\
{\
return " PREFIX_JET_2(nsymb) ";\
}\n" \
  "\n" \
  I " " PREFIX_JET_2(get_deg) "(void)\
{\
return 2;\
}\n" \
  "\n" \
  I " " PREFIX_JET_2(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\t" I " k;\n\
\t\tfprintf(stderr, \"%d: Change number of symbols is not implemented.\\n\",__LINE__); return 0;\n\
\tif (nsymbs > " MAX_NUM_SYMBOLS_NAME ") {\n\
\t\tfprintf(stderr, \"%d: Invalid num. variables \%d > \%d\\n\",__LINE__,nsymbs, " MAX_NUM_SYMBOLS_NAME ");\n\
\t\texit(1);\n\
\t}\n\
\t/* WARNING: in case of parallel region required out of the scope */\n\
\tk = " PREFIX_JET_2(nsymb) ";\n\
\t" PREFIX_JET_2(nsymb) " = nsymbs;\n\
\treturn k;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_2(set_deg) "(" I " deg)\n\
{\n\
\tfprintf(stderr, \"%d: Action not allowed\\n\",__LINE__);\n\
\texit(1);\n\
\treturn 0;\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET_2(clean) "(" PREFIX_JET_2(ptr) " x) {\n\
\t" I " k;\n\
\tif (*x == NULL) return;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(clean) "((*x)[k]);}\n\
\tfree(*x); *x=NULL;\n\
}\n\
void " PREFIX_JET_2(cleanup) "(void)\n\
{\n\
\tif (" PREFIX_JET_2(flag_init_jet_library) "==0) return;\n\
\t" PREFIX_JET_2(clean) "(&" PREFIX_JET_2(jaux) ");\n\
\t" PREFIX_MYFLOAT(clean) "(" PREFIX_JET_2(faux) ");\n\
\t" PREFIX_MYCOEF(clean) "(" PREFIX_JET_2(caux) ");\n\
\t" PREFIX_MYCOEF(cleanup) "();\n\
\t" PREFIX_JET_2(nsymb) " = 0;\n\
\t" PREFIX_JET_2(flag_init_jet_library) "=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_2(set_coef_array) "(" PREFIX_JET_2(t) " b, " PREFIX_MYCOEF(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_2(set_jet_2_coef_array) "(" PREFIX_MYCOEF(t) " *b, " PREFIX_JET_2(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_2(set) "(" PREFIX_JET_2(t) " t, " PREFIX_JET_2(t) " s)\n\
{\n\
  " I " i;\n\
  if (t != s) for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i],s[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(set_coef) "(" PREFIX_JET_2(t) " b, " PREFIX_MYCOEF(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set) "(b[0],a);\n\
\tfor (k = 1; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_2(set_myfloat) "(" PREFIX_JET_2(t) " b, " PREFIX_MYFLOAT(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_myfloat) "(b[0],a);\n\
\tfor (k = 1; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_2(set_d) "(" PREFIX_JET_2(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_d) "(b[0],a);\n\
\tfor (k = 1; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_2(set_si) "(" PREFIX_JET_2(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_si) "(b[0],a);\n\
\tfor (k = 1; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  PREFIX_MYFLOAT(t) "* " PREFIX_JET_2(to_myfloat) "(" PREFIX_JET_2(t) " a) \
{return " PREFIX_MYCOEF(to_myfloat) "(a[0]);}\n" \
  "\n\
void " PREFIX_JET_2(eval) "(" PREFIX_MYCOEF(t) " t[1], " PREFIX_JET_2(t) " a, " PREFIX_MYFLOAT(t) " s[]) \n\
{\n\
\t" I " i,j,k,n1,n2,s2;\n\
\t/* scaler */\n\
\t" PREFIX_MYCOEF(set) "(*t,a[0]);\n\
\t/* degree 1 */\n\
\tn1 = _" PREFIX_JET_2(monomial_counts) "_[1];\n\
\tfor (k = 1; k <= n1; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET_2(caux) ",a[k],s[k-1]);\n\
\t\t" PREFIX_MYCOEF(add2) "(*t,*t," PREFIX_JET_2(caux) ");\n\
\t}\n\
\t/* quadratic */\n\
\ts2 = _" PREFIX_JET_2(monomial_offsets) "_[2];\n\
\tfor(k=0, i=0; i< " MAX_NUM_SYMBOLS_NAME "; i++) {\n\
\t\tfor(j=i; j< " MAX_NUM_SYMBOLS_NAME "; j++) {\n\
\t\t\t" PREFIX_MYFLOAT(mul2) "(" PREFIX_JET_2(faux) ", s[i],s[j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET_2(caux) ",a[s2+k]," PREFIX_JET_2(faux) ");\n\
\t\t\t" PREFIX_MYCOEF(add2) "(*t,*t," PREFIX_JET_2(caux) ");\n\
\t\t\tk++;\n\
\t\t}\n\
\t}\n\
}\n\
\n\
void " PREFIX_JET_2(nrminf) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET_2(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {\n\
\t\t" PREFIX_MYCOEF(nrminf) "(&" PREFIX_JET_2(faux) ",a[k]);\n\
\t\tif (" PREFIX_MYFLOAT(lt) "(*nrm," PREFIX_JET_2(faux) ")) {" PREFIX_MYFLOAT(set) "(*nrm," PREFIX_JET_2(faux) ");}\n\
\t}\n\
}\n\
\n\
void " PREFIX_JET_2(nrm2) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET_2(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {\n\
\t\t" PREFIX_MYCOEF(set_fabs) "(" PREFIX_JET_2(caux) ",a[k]);\n\
\t\t" PREFIX_MYCOEF(nrm2) "(&" PREFIX_JET_2(faux) "," PREFIX_JET_2(caux) ");\n\
\t\t" PREFIX_MYFLOAT(add2) "(*nrm,*nrm," PREFIX_JET_2(faux) ");\n\
\t}\n\
\t" PREFIX_MYFLOAT(set_sqrt) "(*nrm,*nrm);\n\
}\n\
\n\
void " PREFIX_JET_2(neg) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " s) \n\
{\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(neg) "(t[i], s[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(set_jet_2_scal) "(" PREFIX_MYCOEF(t) " *t, " PREFIX_JET_2(t) " f) {\n\
  " PREFIX_MYCOEF(set) "(*t, f[0]);\n\
}\n\
\n\
void " PREFIX_JET_2(add2) "(" PREFIX_JET_2(t) " t, " PREFIX_JET_2(t) " a, " PREFIX_JET_2(t) " b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(add2) "(t[i],a[i],b[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(add2_coef) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYCOEF(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(add2) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i],a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(add2_myfloat) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYFLOAT(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(add2_myfloat) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i],a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(add2_d) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(add2_d) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i],a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(add2_si) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(add2_si) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i],a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(sub2) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a," PREFIX_JET_2(t) " b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(sub2) "(t[i],a[i],b[i]);}\n\
}\n\
\n\
\n\
void " PREFIX_JET_2(coef_sub2) "(" PREFIX_JET_2(t) " t, " PREFIX_MYCOEF(t) " a," PREFIX_JET_2(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(sub2) "(t[0],a,b[0]);\n\
  for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(neg) "(t[i],b[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(myfloat_sub2) "(" PREFIX_JET_2(t) " t, " PREFIX_MYFLOAT(t) " a," PREFIX_JET_2(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(myfloat_sub2) "(t[0],a,b[0]);\n\
  for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(neg) "(t[i],b[i]);}\n\
}\n\
void " PREFIX_JET_2(d_sub2) "(" PREFIX_JET_2(t) " t, double a," PREFIX_JET_2(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(d_sub2) "(t[0],a,b[0]);\n\
  for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(neg) "(t[i],b[i]);}\n\
}\n\
void " PREFIX_JET_2(si_sub2) "(" PREFIX_JET_2(t) " t, int a," PREFIX_JET_2(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(si_sub2) "(t[0],a,b[0]);\n\
  for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(neg) "(t[i],b[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_coef) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYCOEF(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(sub2) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i], a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_myfloat) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYFLOAT(t) " b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(sub2_myfloat) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i], a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_d) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(sub2_d) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i], a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_si) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " I " i;\n\
  " PREFIX_MYCOEF(sub2_si) "(t[0],a[0],b);\n\
  if (t != a) for(i=1;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set) "(t[i], a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_coef) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYCOEF(t) " b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(mul2) "(t[i],(a[i]), b);}\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_myfloat) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYFLOAT(t) " b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(mul2_myfloat) "(t[i],(a[i]), b);}\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_d) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(mul2_d) "(t[i],(a[i]), b);}\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_si) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(mul2_si) "(t[i],(a[i]), b);}\n\
}\n\
\n\
/* t=a*b */\n\
void " PREFIX_JET_2(mul2) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_MYCOEF(t) " temp1, temp2, temp3, temp4, a0, b0;\n\
  static int inited = 0;\n\
#pragma omp threadprivate(temp1, temp2, temp3, temp4, a0, b0, inited)\n\
  int i,j,k,l,n1,n2,s1,s2,nv; \n\
\n\
  if(inited == 0) {\n\
    " PREFIX_MYCOEF(init) "(temp1); " PREFIX_MYCOEF(init) "(temp2);\n\
    " PREFIX_MYCOEF(init) "(temp3); " PREFIX_MYCOEF(init) "(temp4);\n\
    " PREFIX_MYCOEF(init) "(a0); " PREFIX_MYCOEF(init) "(b0);\n\
    inited = 1;\n\
  }\n\
  /* scaler */\n\
  " PREFIX_MYCOEF(mul2) "(t[0],a[0],b[0]); \n\
  /* degree 1 */\n\
  s1 = _" PREFIX_JET_2(monomial_offsets) "_[1];\n\
  n1 = _" PREFIX_JET_2(monomial_counts) "_[1];\n\
  for(i=1;i<=n1;i++){\n\
    " PREFIX_MYCOEF(mul2) "(temp1,(b[i]), a[0]);\n\
    " PREFIX_MYCOEF(mul2) "(temp2,(a[i]), b[0]);\n\
    " PREFIX_MYCOEF(add2) "(t[i],temp1,temp2);\n\
  }\n\
  if(_MAX_DEGREE_OF_JET_VARS_ > 1) {\n\
    /* quadratic */\n\
    s2 = _" PREFIX_JET_2(monomial_offsets) "_[2];\n\
    nv = " MAX_NUM_SYMBOLS_NAME ";\n\
\n\
\n\
    " PREFIX_MYCOEF(set) "(a0,a[0]);\n\
    " PREFIX_MYCOEF(set) "(b0,b[0]);\n\
    for(k=0, i=1; i<= nv; i++) {\n\
      for(j=i; j<=nv; j++, k++) {    \n\
        " PREFIX_MYCOEF(mul2) "(temp1, a0 ,  b[s2+k]);\n\
        " PREFIX_MYCOEF(mul2) "(temp2, b0 ,  a[s2+k]);\n\
        " PREFIX_MYCOEF(mul2) "(temp3, a[i],b[j]);\n\
        " PREFIX_MYCOEF(add2) "(temp4,temp1,temp2);\n\
\n\
        if(i != j) {\n\
          " PREFIX_MYCOEF(mul2) "(temp1, a[j],  b[i]);\n\
          " PREFIX_MYCOEF(add2) "(temp2,temp3,temp4);\n\
          " PREFIX_MYCOEF(add2) "(t[s2+k],temp1,temp2);\n\
        } else {\n\
          " PREFIX_MYCOEF(add2) "(t[s2+k],temp3,temp4);\n\
        }\n\
      }\n\
    }\n\
  }\n\
}\n\
\n\
/* t=a/b, b float   */\n\
void " PREFIX_JET_2(div2_coef) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYCOEF(t) " b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(div2) "(t[i],(a[i]),b);}\n\
}\n\
\n\
void " PREFIX_JET_2(div2_myfloat) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_MYFLOAT(t) " b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(div2_myfloat) "(t[i],(a[i]),b);}\n\
}\n\
\n\
void " PREFIX_JET_2(div2_d)  "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(div2_d) "(t[i],(a[i]),b);}\n\
}\n\
\n\
void " PREFIX_JET_2(div2_si)  "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(div2_si) "(t[i],(a[i]),b);}\n\
}\n\
\n\
\n\
/* t= a/b, a float, b jet */\n\
void " PREFIX_JET_2(coef_div2)  "(" PREFIX_JET_2(t) " t, " PREFIX_MYCOEF(t) " a, " PREFIX_JET_2(t) " b) {\n\
  //fprintf(stderr, \"to test\\n\");exit(2);\n\
  " PREFIX_JET_2(div2_coef) "(t, b, b[0]);\n\
  " PREFIX_MYCOEF(set_si) "(t[0],0);\n\
  " PREFIX_JET_2(mul2) "(" PREFIX_JET_2(jaux) ",t,t);\n\
  " PREFIX_JET_2(sub2) "(" PREFIX_JET_2(jaux) "," PREFIX_JET_2(jaux) ",t);\n\
  " PREFIX_MYCOEF(set_si) "(" PREFIX_JET_2(jaux) "[0], 1);\n\
  " PREFIX_MYCOEF(div2) "(" PREFIX_JET_2(caux) ",a,b[0]);\n\
  " PREFIX_JET_2(mul2_coef) "(t, " PREFIX_JET_2(jaux) ", " PREFIX_JET_2(caux) ");\n\
}\n\
\n\
void " PREFIX_JET_2(myfloat_div2)  "(" PREFIX_JET_2(t) " t, " PREFIX_MYFLOAT(t) " a, " PREFIX_JET_2(t) " b) {\n\
      //fprintf(stderr, \"to test\\n\");exit(2);\n\
  " PREFIX_JET_2(div2_coef) "(t, b, b[0]);\n\
  " PREFIX_MYCOEF(set_si) "(t[0],0);\n\
  " PREFIX_JET_2(mul2) "(" PREFIX_JET_2(jaux) ",t,t);\n\
  " PREFIX_JET_2(sub2) "(" PREFIX_JET_2(jaux) "," PREFIX_JET_2(jaux) ",t);\n\
  " PREFIX_MYCOEF(set_d) "(" PREFIX_JET_2(jaux) "[0], 1.0);\n\
  " PREFIX_MYCOEF(myfloat_div2) "(" PREFIX_JET_2(caux) ",a,b[0]);\n\
  " PREFIX_JET_2(mul2_coef) "(t, " PREFIX_JET_2(jaux) ", " PREFIX_JET_2(caux) ");\n\
}\n\
\n\
void " PREFIX_JET_2(d_div2)  "(" PREFIX_JET_2(t) " t, double a, " PREFIX_JET_2(t) " b) {\n\
      //fprintf(stderr, \"to test\\n\");exit(2);\n\
  " PREFIX_JET_2(div2_coef) "(t, b, b[0]);\n\
  " PREFIX_MYCOEF(set_si) "(t[0],0);\n\
  " PREFIX_JET_2(mul2) "(" PREFIX_JET_2(jaux) ",t,t);\n\
  " PREFIX_JET_2(sub2) "(" PREFIX_JET_2(jaux) "," PREFIX_JET_2(jaux) ",t);\n\
  " PREFIX_MYCOEF(set_d) "(" PREFIX_JET_2(jaux) "[0], 1.0);\n\
  " PREFIX_MYCOEF(d_div2) "(" PREFIX_JET_2(caux) ",a,b[0]);\n\
  " PREFIX_JET_2(mul2_coef) "(t, " PREFIX_JET_2(jaux) ", " PREFIX_JET_2(caux) ");\n\
}\n\
\n\
void " PREFIX_JET_2(si_div2)  "(" PREFIX_JET_2(t) " t, int a, " PREFIX_JET_2(t) " b) {\n\
  " PREFIX_JET_2(div2_coef) "(t, b, b[0]);\n\
  " PREFIX_MYCOEF(set_si) "(t[0],0);\n\
  " PREFIX_JET_2(mul2) "(" PREFIX_JET_2(jaux) ",t,t);\n\
  " PREFIX_JET_2(sub2) "(" PREFIX_JET_2(jaux) "," PREFIX_JET_2(jaux) ",t);\n\
  " PREFIX_MYCOEF(set_d) "(" PREFIX_JET_2(jaux) "[0], 1.0);\n\
  " PREFIX_MYCOEF(si_div2) "(" PREFIX_JET_2(caux) ",a,b[0]);\n\
  " PREFIX_JET_2(mul2_coef) "(t, " PREFIX_JET_2(jaux) ", " PREFIX_JET_2(caux) ");\n\
}\n\
\n\
\n\
/* t = a/b,  a,b jets */\n\
void " PREFIX_JET_2(div2) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a," PREFIX_JET_2(t) " b)  {\n\
  " PREFIX_JET_2(si_div2) "(t, 1, b);\n\
  " PREFIX_JET_2(set) "(" PREFIX_JET_2(jaux) ", t);\n\
  " PREFIX_JET_2(mul2) "(t,a," PREFIX_JET_2(jaux) ");\n\
}\n\
\n\
/* t=b^e, e float */\n\
void " PREFIX_JET_2(set_pow_myfloat) "(" PREFIX_JET_2(t) " t, " PREFIX_JET_2(t) " b, " PREFIX_MYFLOAT(t) " e) {\n\
  static " PREFIX_JET_2(t) " tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_MYCOEF(t) " s1, smp1,smp2,smp4;\n\
#pragma omp threadprivate(tmp2,tmp3, s1,smp1,smp2,smp4, initialized)\n\
  if(!initialized) {\n\
    " PREFIX_MYCOEF(init) "(s1);  \n\
    " PREFIX_MYCOEF(init) "(smp1);" PREFIX_MYCOEF(init) "(smp2);" PREFIX_MYCOEF(init) "(smp4);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    " PREFIX_JET_2(init) "(&tmp3);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_MYCOEF(set_pow_myfloat) "(s1,b[0],e);\n\
\n\
  " PREFIX_MYCOEF(mul2_myfloat) "(smp4,s1,e);\n\
\n\
  " PREFIX_JET_2(div2_coef) "(tmp2, b, b[0]);\n\
  " PREFIX_MYCOEF(set_si) "(tmp2[0],0);\n\
  " PREFIX_JET_2(mul2) "(tmp3,tmp2,tmp2);  \n\
  " PREFIX_MYFLOAT(sub2_si) "(" PREFIX_JET_2(faux) ",e,1);\n\
  " PREFIX_MYFLOAT(div2_si) "(" PREFIX_JET_2(faux) "," PREFIX_JET_2(faux) ",2);\n\
  " PREFIX_MYCOEF(mul2_myfloat) "(smp2,smp4," PREFIX_JET_2(faux) ");\n\
  " PREFIX_JET_2(mul2_coef) "(t, tmp3, smp2);\n\
  " PREFIX_JET_2(mul2_coef) "(tmp3, tmp2, smp4);\n\
  " PREFIX_JET_2(add2) "(t,t,tmp3);\n\
  " PREFIX_MYCOEF(set) "(t[0],s1);\n\
}\n\
\n\
/* t=sqrt(a) */\n\
void " PREFIX_JET_2(set_sqrt) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a) {\n\
  " PREFIX_MYFLOAT(set_d) "(" PREFIX_JET_2(faux) ",0.5e0);\n\
  " PREFIX_JET_2(set_pow_myfloat) "(t, a, " PREFIX_JET_2(faux) ");\n\
}\n\
\n\
/* t=sin(b), b = b0 + X */\n\
// t = sin(b0) + cos(b0) X -1/2 sin(b0) X^2  \n\
void " PREFIX_JET_2(set_sin) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2;\n\
  static int initialized = 0;\n\
  static " PREFIX_MYCOEF(t) " _c, _s, smp;\n\
#pragma omp threadprivate(tmp1, tmp2, _c, _s, smp,initialized)\n\
  if(!initialized) {\n\
    " PREFIX_MYCOEF(init) "(_c);\n\
    " PREFIX_MYCOEF(init) "(_s);\n\
    " PREFIX_MYCOEF(init) "(smp);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_MYCOEF(set_cos) "(_c,b[0]);\n\
  " PREFIX_MYCOEF(set_sin) "(_s,b[0]);\n\
\n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_MYCOEF(set_d) "(tmp1[0],0.0);\n\
  // tmp1 = X\n\
  " PREFIX_JET_2(mul2) "(tmp2,tmp1,tmp1);    \n\
  " PREFIX_MYCOEF(mul2_d) "(smp, _s, -0.5);\n\
  " PREFIX_JET_2(mul2_coef) "(t,tmp2,smp);\n\
  // tmp3 = 1/2 sin(b0)X^2\n\
  " PREFIX_JET_2(mul2_coef) "(tmp2,tmp1,_c);\n\
  // tmp2 = cos(b0)X\n\
  " PREFIX_JET_2(add2) "(t,t,tmp2);  \n\
  " PREFIX_MYCOEF(set) "(t[0], _s);\n\
}\n\
\n\
/* t=cos(b) b=b0+X*/\n\
// t= cos(b0) - sin(b0) X -1/2 cos(b0) X^2\n\
\n\
void " PREFIX_JET_2(set_cos) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2;\n\
  static int initialized = 0;\n\
  static " PREFIX_MYCOEF(t) " _c, _s, smp;\n\
#pragma omp threadprivate(tmp1, tmp2, _c, _s, smp,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_MYCOEF(init) "(_c);\n\
    " PREFIX_MYCOEF(init) "(_s);\n\
    " PREFIX_MYCOEF(init) "(smp);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_MYCOEF(set_cos) "(_c,b[0]);\n\
  " PREFIX_MYCOEF(set_sin) "(_s,b[0]);\n\
  \n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_MYCOEF(set_d) "(tmp1[0],0.0);\n\
  // X = tmp1\n\
  " PREFIX_JET_2(mul2) "(tmp2,tmp1,tmp1);    \n\
  " PREFIX_MYCOEF(mul2_d) "(smp, _c, -0.5e0);\n\
  " PREFIX_JET_2(mul2_coef) "(t,tmp2,smp);\n\
  //  tmp3 = - 1/2 cos(b0) X^2\n\
  " PREFIX_MYCOEF(neg) "(smp, _s);\n\
  " PREFIX_JET_2(mul2_coef) "(tmp2,tmp1,smp);\n\
  // tmp2 = - sin(b0) X\n\
  " PREFIX_JET_2(add2) "(t,t,tmp2);  \n\
  " PREFIX_MYCOEF(set) "(t[0], _c);\n\
}\n\
\n\
/* t=exp(b)*/\n\
void " PREFIX_JET_2(set_exp) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_MYCOEF(t) " _c;\n\
#pragma omp threadprivate(t0, t1, t2, _c, initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_MYCOEF(init) "(_c);\n\
    " PREFIX_JET_2(init) "(&t0);\n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_MYCOEF(set_exp) "(_c,b[0]);\n\
  " PREFIX_JET_2(sub2_coef) "(t0, b, b[0]);\n\
  " PREFIX_JET_2(mul2) "(t1,t0,t0);\n\
  " PREFIX_JET_2(mul2_d) "(t2,t1,0.5e0);\n\
  " PREFIX_JET_2(add2) "(t1,t0,t2);\n\
\n\
  " PREFIX_JET_2(add2_si) "(t2,t1,1);\n\
\n\
  " PREFIX_JET_2(mul2_coef) "(t,t2,_c);\n\
  " PREFIX_MYCOEF(set) "(t[0], _c);\n\
}\n\
\n\
\n\
void " PREFIX_JET_2(set_tan) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " ts, tc;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate(ts, tc, initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_JET_2(init) "(&ts);    \n\
    " PREFIX_JET_2(init) "(&tc);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_2(set_sin) "(ts,b);\n\
  " PREFIX_JET_2(set_cos) "(tc,b);  \n\
  " PREFIX_JET_2(div2) "(t, ts,tc);  \n\
}\n\
\n\
/* t=ln(b)*/\n\
void " PREFIX_JET_2(set_log) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_MYCOEF(t) " lna;\n\
#pragma omp threadprivate(t0, t1, t2, lna, initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_MYCOEF(init) "(lna);\n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_MYCOEF(set_log) "(lna,b[0]);\n\
  \n\
  " PREFIX_JET_2(div2_coef) "(t0, b, b[0]);\n\
  " PREFIX_MYCOEF(set_d) "(t0[0],0.0);\n\
  " PREFIX_JET_2(mul2) "(t1,t0,t0);\n\
  " PREFIX_JET_2(div2_si) "(t2, t1, 2);\n\
  " PREFIX_JET_2(sub2) "(t,t0,t2);\n\
  " PREFIX_MYCOEF(set) "(t[0], lna);\n\
}\n\
\n\
/* t= atan(b) */\n\
// atan(x) = x - x^3/3 + x^5/5 + ... \n\
//\n\
// atan(b0+s) = atan( (b0+t)/(1-b0t)) = atan(b0) + atan(t)\n\
// where t = s/(1+b0^2 + b0 s)\n\
// atan(t) =~ t +o(t^3)\n\
//_\n\
void " PREFIX_JET_2(set_atan) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_MYCOEF(t) " b0, b02, b02p1;\n\
#pragma omp threadprivate(t0, t1, t2, b0, b02, b02p1, initialized)\n\
  if(!initialized) {\n\
    " PREFIX_MYCOEF(init) "(b0);\n\
    " PREFIX_MYCOEF(init) "(b02);\n\
    " PREFIX_MYCOEF(init) "(b02p1);        \n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_MYCOEF(set) "(b0,b[0]);\n\
  " PREFIX_MYCOEF(mul2) "(b02,b0, b0);\n\
  " PREFIX_MYCOEF(add2_si) "(b02p1, b02,  1);\n\
  " PREFIX_JET_2(sub2_coef) "(t0, b, b0);\n\
  " PREFIX_JET_2(mul2_coef) "(t1, t0, b0);\n\
  " PREFIX_JET_2(add2_coef) "(t2,t1,b02p1);\n\
  " PREFIX_JET_2(div2) "(t1, t0, t2);\n\
  // b0+t1/(1-b0 t1) = b0+t0 = b\n\
  " PREFIX_MYCOEF(set_atan) "(b02, b0);\n\
  " PREFIX_JET_2(add2_coef) "(t,t1,b02);  \n\
}\n\
  \n\
/* t=sinh(b) */\n\
void " PREFIX_JET_2(set_sinh) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t1, t2;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate(t1, t2, initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_2(neg) "(t1, b);\n\
  " PREFIX_JET_2(set_exp) "(t2,t1);\n\
  " PREFIX_JET_2(set_exp) "(t1,b);\n\
  " PREFIX_JET_2(sub2) "(t1,t1,t2);\n\
  " PREFIX_JET_2(div2_si) "(t,t1,2);\n\
}\n\
\n\
/* t=cosh(b) */\n\
void " PREFIX_JET_2(set_cosh) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t1, t2;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate(t1, t2, initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_2(neg) "(t1, b);\n\
  " PREFIX_JET_2(set_exp) "(t2,t1);\n\
  " PREFIX_JET_2(set_exp) "(t1,b);\n\
  " PREFIX_JET_2(add2) "(t1,t1,t2);\n\
  " PREFIX_JET_2(div2_si) "(t, t1, 2);\n\
}\n\
/* t=tanh(b) */\n\
void " PREFIX_JET_2(set_tanh) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) "  t1, t2;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate( t1, t2,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_2(set_sinh) "(t1,b);\n\
  " PREFIX_JET_2(set_cosh) "(t2,b);\n\
  " PREFIX_JET_2(div2) "(t, t1, t2);  \n\
}\n\
\n\
void " PREFIX_JET_2(set_fabs) "(" PREFIX_JET_2(t) " f, " PREFIX_JET_2(t) " a) {\n\
  " I " i;\n\
  for(i=0;i<" MAX_COEFFS_COUNT_NAME ";i++){" PREFIX_MYCOEF(set_fabs) "(f[i],a[i]);}\n\
}\n\
\n\
void " PREFIX_JET_2(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET_2(t) " a)\n\
{\n\
  " I " k;\n\
  for (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {\n\
    " PREFIX_MYCOEF(fprintf) "(file,fmt,a[k]);\n\
  }\n\
}\n\
void " PREFIX_JET_2(fscanf) "(FILE *file, const char *fmt, " PREFIX_JET_2(t) " a)\n\
{\n\
  " I " k;\n\
  for (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {\n\
    " PREFIX_MYCOEF(fscanf) "(file,fmt," MYDATA_ACCESS "(a,k));\n\
  }\n\
}\n\
" \

#define MY_JET_2_POSTCODE(PREFIX_JET_2,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END CODE " PREFIX_JET_2(t) " */\n" \

#endif /* MY_JET_2_H */
