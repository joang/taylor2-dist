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

#ifndef MY_JET1_H
#define MY_JET1_H

/* *******  **************  ******* */
/* *******  MY_JET1_HEADER  ******* */
/* *******  **************  ******* */
#define MY_JET1_FIXES MY_JET_SPACE
#define MY_JET1_TYPE MY_JET_TYPE1

#define MY_JET1_API(PREFIX_JET1,MY_JET1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                    MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \
  MY_JET_APIS(PREFIX_JET1,MY_JET1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET1_PREHEADER(PREFIX_JET1,MY_JET1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* HEADER " PREFIX_JET1(t) " */\n"\

#define MY_JET1_HEADER(PREFIX_JET1,MY_JET1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET1_API(PREFIX_JET1,MY_JET1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET1_POSTHEADER(PREFIX_JET1,MY_JET1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END HEADER " PREFIX_JET1(t) " */\n" \


/* *******  ************  ******* */
/* *******  MY_JET1_CODE  ******* */
/* *******  ************  ******* */
#define MY_JET1_PRECODE(PREFIX_JET1,PREFIX_MYCOEF,I,\
                        MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_COEF_MACROS(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\n\
/* CODE FOR " PREFIX_JET1(t) " */\n\
int * " PREFIX_JET1(monomial_counts) "(void) {return _" PREFIX_JET1(monomial_counts) "_;}\n\
int * " PREFIX_JET1(monomial_offsets) "(void) {return _" PREFIX_JET1(monomial_offsets) "_;}\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
#include <string.h>\n\
\n"\

#define MY_JET1_CODE(PREFIX_JET1,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,\
                     MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,\
                     MYDATA_ACCESS) "\
\n\
static int " PREFIX_JET1(flag_init_jet_library) "=0;\n\
static " I " " PREFIX_JET1(deg) ";\n\
static " PREFIX_MYFLOAT(t) " " PREFIX_JET1(faux) ";\n\
static " PREFIX_MYCOEF(t) " " PREFIX_JET1(caux) ";\n\
static " PREFIX_JET1(t) " " PREFIX_JET1(jaux) ";\n\
#pragma omp threadprivate(" PREFIX_JET1(deg) "," PREFIX_JET1(flag_init_jet_library) "," PREFIX_JET1(faux) "," PREFIX_JET1(caux) "," PREFIX_JET1(jaux) ")\n\
  \n" \
  "\n" \
  "size_t " PREFIX_JET1(init) "(" PREFIX_JET1(ptr) " x)\n\
{\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\t*x = (__typeof__(*x)) malloc((" MAX_COEFFS_COUNT_NAME ")*sizeof(**x));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"\%d: not enough memory to allocate \%d\\n\",__LINE__," MAX_COEFFS_COUNT_NAME ");\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(init) "((*x)[k]);}\n\
\tmem+= (" MAX_COEFFS_COUNT_NAME ")*sizeof(**x);\n\
\treturn mem;\n\
}\n" \
  "void " PREFIX_JET1(initup2) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (" PREFIX_JET1(flag_init_jet_library) "==1) return;\n\
\tif (nsymbs != 1) {\n\
\t\tfprintf(stderr, \"\%d: Not allowed nsymbs value: \%d!=1\\n\",__LINE__,nsymbs);\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\tif (" MAX_DEGREE_NAME " < deg) {\n\
\t\tfprintf(stderr, \"\%d: Not allowed max deg value: \%d < \%d\\n\",__LINE__, " MAX_DEGREE_NAME ",deg); fflush(stderr); exit(1);\n\
\t}\n\
\t" PREFIX_JET1(deg) " = deg;\n\
\t" PREFIX_MYCOEF(initup) "();\n\
\t" PREFIX_MYCOEF(init) "(" PREFIX_JET1(caux) ");\n\
\t" PREFIX_MYFLOAT(init) "(" PREFIX_JET1(faux) ");\n\
\t" PREFIX_JET1(init) "(&" PREFIX_JET1(jaux) ");\n\
\t" PREFIX_JET1(flag_init_jet_library) "=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET1(get_num_symbs) "(void)\
{\
return 1;\
}\n" \
  "\n" \
  I " " PREFIX_JET1(get_deg) "(void)\
{\
return " PREFIX_JET1(deg) ";\
}\n" \
  "\n" \
  I " " PREFIX_JET1(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\tfprintf(stderr, \"\%d: Action not allowed\\n\",__LINE__);\n\
\tfflush(stderr); exit(1);\n\
\treturn 0;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET1(set_deg) "(" I " deg)\n\
{\n\
\t" I " k;\n\
\tif (deg > " MAX_DEGREE_NAME ") {\n\
\t\tfprintf(stderr, \"\%d: Invalid degree \%d > \%d\\n\",__LINE__, deg, " MAX_DEGREE_NAME ");\n\
\t\tfflush(stderr); exit(1);\n\
\t}\n\
\t/* WARNING: in case of parallel region required out of the scope */\n\
\tk = " PREFIX_JET1(deg) ";\n\
\t" PREFIX_JET1(deg) " = deg;\n\
\treturn k;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(clean) "(" PREFIX_JET1(ptr) " x)\n\
{\n\
\t" I " k;\n\
\tif (*x == NULL) return;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(clean) "((*x)[k]);}\n\
\tfree(*x); *x=NULL;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(cleanup) "(void)\n\
{\n\
\tif (" PREFIX_JET1(flag_init_jet_library) "==0) return;\n\
\t" PREFIX_JET1(clean) "(&" PREFIX_JET1(jaux) ");\n\
\t" PREFIX_MYCOEF(clean) "(" PREFIX_JET1(caux) ");\n\
\t" PREFIX_MYFLOAT(clean) "(" PREFIX_JET1(faux) ");\n\
\t" PREFIX_MYCOEF(cleanup) "();\n\
\t" PREFIX_JET1(deg) " = 0;\n\
\t" PREFIX_JET1(flag_init_jet_library) "=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set) "(" PREFIX_JET1(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_coef_array) "(" PREFIX_JET1(t) " b, " PREFIX_MYCOEF(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n	\
}\n" \
 "\n" \
  "void " PREFIX_JET1(set_jet_2_coef_array) "(" PREFIX_MYCOEF(t) " *b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(b[k],a[k]);}\n	\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_coef) "(" PREFIX_JET1(t) " b, " PREFIX_MYCOEF(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(set_myfloat) "(" PREFIX_JET1(t) " b, " PREFIX_MYFLOAT(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_myfloat) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(set_d) "(" PREFIX_JET1(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_d) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(set_si) "(" PREFIX_JET1(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_si) "(b[0],a);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set_zero) "(b[k]);}\n\
}\n" \
  PREFIX_MYFLOAT(t) "* " PREFIX_JET1(to_myfloat) "(" PREFIX_JET1(t) " a) \
{ return " PREFIX_MYCOEF(to_myfloat) "(a[0]);}\n" \
  "\n" \
  "void " PREFIX_JET1(eval) "(" PREFIX_MYCOEF(t) " t[1], " PREFIX_JET1(t) " a, " PREFIX_MYFLOAT(t) " s[]) \n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(set_zero) "(*t);\n\
\tfor (k = " PREFIX_JET1(deg) "; k > 0; k--) {\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET1(caux) ",*t,*s);\n\
\t\t" PREFIX_MYCOEF(add2) "(*t,a[k]," PREFIX_JET1(caux) ");\n\
\t}\n\
\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET1(caux) ",*t,*s);\n\
\t" PREFIX_MYCOEF(add2) "(*t,a[0]," PREFIX_JET1(caux) ");\n\
}\n" \
  "\n" \
  PREFIX_MYCOEF(t) " * " PREFIX_JET1(get_coef) "(" PREFIX_JET1(t) " a, const " I " idx[]) \
{\
return &a[*idx];\
}\n" \
  "\n" \
  PREFIX_MYCOEF(t) " * " PREFIX_JET1(coef0) "(" PREFIX_JET1(t) " a) \
{\
return &a[0];\
}\n" \
  "\n" \
  PREFIX_MYCOEF(t) " * " PREFIX_JET1(coef1) "(" PREFIX_JET1(t) " a, " I " idx) \
{\
return &a[1+idx];\
}\n" \
  "void " PREFIX_JET1(nrminf) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(nrminf) "(&" PREFIX_JET1(faux) ",a[k]);\n\
\t\tif (" PREFIX_MYFLOAT(lt) "(*nrm," PREFIX_JET1(faux) ")) \
{" PREFIX_MYFLOAT(set) "(*nrm," PREFIX_JET1(faux) ");}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(nrm2) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* WARNING initial value for nrm GIVEN!! */\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(set_fabs) "(" PREFIX_JET1(caux) ",a[k]);\n\
\t\t" PREFIX_MYCOEF(nrm2) "(&" PREFIX_JET1(faux) "," PREFIX_JET1(caux) ");\n\
\t\t" PREFIX_MYFLOAT(add2) "(*nrm,*nrm," PREFIX_JET1(faux) ");\n\
\t}\n\
\t" PREFIX_MYFLOAT(set_sqrt) "(*nrm,*nrm);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(neg) "(" PREFIX_JET1(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(neg) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(add2) "(c[k],a[k],b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(sub2) "(c[k],a[k],b[k]);}\n\
}\n" \
"\n" \
  "#define " PREFIX_JET1(set_sq) "(s,a)\\\n\
{\\\n\
\t" I " j,k, nk;\\\n\
\t" PREFIX_MYCOEF(mul2) "(s[0],a[0],a[0]);\\\n\
\tnk = (" PREFIX_JET1(deg) "+1) - ((" PREFIX_JET1(deg) "+1)%2==0);\\\n\
\tfor (k = 1; k < nk; k++) {\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(s[k],a[0],a[k]);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[j],a[k-j]);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\\\n\
\t\t}\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(s[k],s[k],2);\\\n\
\t\tk++;\\\n\
\t\t/* k even */\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(s[k],a[0],a[k]);\\\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[j],a[k-j]);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\\\n\
\t\t}\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(s[k],s[k],2);\\\n\
\t\tj = k/2;\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[j],a[j]);\\\n\
\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\\\n\
\t}\\\n\
\tif ((" PREFIX_JET1(deg) "+1)%2==0) {\\\n\
\t\tk=" PREFIX_JET1(deg) ";\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(s[k],a[0],a[k]);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[j],a[k-j]);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\\\n\
\t\t}\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(s[k],s[k],2);\\\n\
\t}\\\n\
}\n" \
"\n" \
  "void " PREFIX_JET1(mul2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t/* c_k = sum(a_{j}b_{k-j}, j=0,...,k) */\n\
\t" PREFIX_MYCOEF(mul2) "(c[0],a[0],b[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],a[0],b[k]);\n\
\t\tfor (j = 1; j <= k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[j],b[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t}\n\
}\n" \
  "void " PREFIX_JET1(div2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(div2) "(c[0],a[0],b[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",c[j],b[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(sub2) "(c[k],a[k],c[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2_coef) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2) "(c[0],a[0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_coef) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2) "(c[0],a[0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(coef_sub2) "(" PREFIX_JET1(t) " c, " PREFIX_MYCOEF(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2) "(c[0],b,a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_coef) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(mul2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(div2_coef) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(div2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(coef_div2) "(" PREFIX_JET1(t) " c, " PREFIX_MYCOEF(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(div2) "(c[0],a,b[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",c[j],b[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2_myfloat) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2_myfloat) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(add2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2_d) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_myfloat) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_myfloat) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_d) "(c[0],a[0],b);\n\
\tif (c != a) for (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(myfloat_sub2) "(" PREFIX_JET1(t) " c, " PREFIX_MYFLOAT(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(myfloat_sub2) "(c[0],b,a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(d_sub2) "(" PREFIX_JET1(t) " c, double b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(d_sub2) "(c[0],b,a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_myfloat) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(mul2_myfloat) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(mul2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(div2_myfloat) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(div2_myfloat) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(myfloat_div2) "(" PREFIX_JET1(t) " c, " PREFIX_MYFLOAT(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(myfloat_div2) "(c[0],a,b[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",c[j],b[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "void " PREFIX_JET1(div2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(div2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(d_div2) "(" PREFIX_JET1(t) " c, double a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(d_div2) "(c[0],a,b[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",c[j],b[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(add2_si) "(c[0],a[0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_si) "(c[0],a[0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(si_sub2) "(" PREFIX_JET1(t) " c, int b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_MYCOEF(sub2_si) "(c[0],a[0],b);\n\
\t" PREFIX_MYCOEF(neg) "(a[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(mul2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(div2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {" PREFIX_MYCOEF(div2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(si_div2) "(" PREFIX_JET1(t) " c, int a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(si_div2) "(c[0],a,b[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",c[j],b[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_sqrt) "(" PREFIX_JET1(t) " s, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k, nk;\n\
\t" PREFIX_MYCOEF(set_sqrt) "(s[0],a[0]);\n\
\tnk = " PREFIX_JET1(deg) " - (" PREFIX_JET1(deg) "%2==1);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t/* k odd */\n\
\t\t" PREFIX_MYCOEF(set_zero) "(s[k]);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",s[j],s[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",s[k],2);\n\
\t\t" PREFIX_MYCOEF(sub2) "(s[k],a[k]," PREFIX_JET1(caux) ");\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",s[0],2);\n\
\t\t" PREFIX_MYCOEF(div2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t\tk++;\n\
\t\t/* k even */\n\
\t\t" PREFIX_MYCOEF(set_zero) "(s[k]);\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",s[j],s[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",s[k],2);\n\
\t\t" PREFIX_MYCOEF(sub2) "(s[k],a[k]," PREFIX_JET1(caux) ");\n\
\t\tj = k/2;\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",s[j],s[j]);\n\
\t\t" PREFIX_MYCOEF(sub2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",s[0],2);\n\
\t\t" PREFIX_MYCOEF(div2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t}\n\
\tif (" PREFIX_JET1(deg) "%2==1) {\n\
\t\tk=" PREFIX_JET1(deg) ";\n\
\t\t/* k odd */\n\
\t\t" PREFIX_MYCOEF(set_zero) "(s[k]);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",s[j],s[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",s[k],2);\n\
\t\t" PREFIX_MYCOEF(sub2) "(s[k],a[k]," PREFIX_JET1(caux) ");\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",s[0],2);\n\
\t\t" PREFIX_MYCOEF(div2) "(s[k],s[k]," PREFIX_JET1(caux) ");\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_pow_myfloat) "(" PREFIX_JET1(t) " p, " PREFIX_JET1(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
\t" I " j,k;\n\
#define tmp " PREFIX_JET1(jaux) "[0]\n\
\t" PREFIX_MYCOEF(set_pow_myfloat) "(p[0],a[0],b);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(p[k],a[k],p[0]);\n\
\t\t" PREFIX_MYFLOAT(mul2_si) "(" PREFIX_JET1(faux) ", b, k);\n\
\t\t" PREFIX_MYCOEF(mul2_myfloat) "(p[k],p[k]," PREFIX_JET1(faux) ");\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYFLOAT(mul2_si) "(" PREFIX_JET1(faux) ",b,k-j);\n\
\t\t\t" PREFIX_MYFLOAT(sub2_si) "(" PREFIX_JET1(faux) "," PREFIX_JET1(faux) ",j);\n\
\t\t\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET1(caux) ",a[k-j]," PREFIX_JET1(faux) ");\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(tmp," PREFIX_JET1(caux) ",p[j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(p[k],p[k],tmp);\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) ",a[0],k);\n\
\t\t" PREFIX_MYCOEF(div2) "(p[k],p[k]," PREFIX_JET1(caux) ");\n\
\t}\n\
#undef tmp\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_exp) "(" PREFIX_JET1(t) " e, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_exp) "(e[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k],e[0]);\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(e[k]," PREFIX_JET1(caux) ",k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j],e[j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(e[k],e[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(div2_si) "(e[k],e[k],k);\n\
\t}\n\
}\n" \
  "void " PREFIX_JET1(set_log) "(" PREFIX_JET1(t) " l, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t/* Assump. l!=a */\n\
\t" PREFIX_MYCOEF(set_log) "(l[0],a[0]);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(set_zero) "(l[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j],l[j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",j);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(l[k],l[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(div2_si) "(l[k],l[k],k);\n\
\t\t" PREFIX_MYCOEF(sub2) "(l[k],a[k],l[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(l[k],l[k],a[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET1(sincos) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k;\\\n\
\t" PREFIX_MYCOEF(set_sin) "(s[0],a[0]);\\\n\
\t" PREFIX_MYCOEF(set_cos) "(c[0],a[0]);\\\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(s[k],a[k],c[0]);\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],a[k],s[0]);\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(c[k],c[k],k);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j],c[j]);\\\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\\\n\
\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j],s[j]);\\\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\\\n\
\t\t}\\\n\
\t\t" PREFIX_MYCOEF(div2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_MYCOEF(div2_si) "(c[k],c[k],-k);\\\n\
\t}\\\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_cos) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sincos) "(" PREFIX_JET1(jaux) ",c,a);}\n" \
  "\n" \
  "void " PREFIX_JET1(set_sin) "(" PREFIX_JET1(t) " s, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sincos) "(s," PREFIX_JET1(jaux) ",a);}\n" \
  "\n" \
  "void " PREFIX_JET1(set_tan) "(" PREFIX_JET1(t) " t, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k, nk;\n\
\t" PREFIX_MYCOEF(set_tan) "(t[0],a[0]);\n\
\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(jaux) "[0],t[0],t[0]);\n\
\n\
\tnk = (" PREFIX_JET1(deg) "+1) - ((" PREFIX_JET1(deg) "+1)%2==0);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t" PREFIX_MYCOEF(mul2) "(t[k],a[k]," PREFIX_JET1(jaux) "[0]);\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(t[k],t[k],k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j]," PREFIX_JET1(jaux) "[j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k],a[k]);\n\
\t\t/* k odd */\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(jaux) "[k],t[0],t[k]);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",t[j],t[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(" PREFIX_JET1(jaux) "[k]," PREFIX_JET1(jaux) "[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(jaux) "[k]," PREFIX_JET1(jaux) "[k],2);\n\
\t\tk++;\n\
\t\t" PREFIX_MYCOEF(mul2) "(t[k],a[k]," PREFIX_JET1(jaux) "[0]);\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(t[k],t[k],k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j]," PREFIX_JET1(jaux) "[j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k],a[k]);\n\
\t\t/* k even */\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(jaux) "[k],t[0],t[k]);\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",t[j],t[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(" PREFIX_JET1(jaux) "[k]," PREFIX_JET1(jaux) "[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(jaux) "[k]," PREFIX_JET1(jaux) "[k],2);\n\
\n\
\t\tj = k/2;\n\
\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",t[j],t[j]);\n\
\t\t" PREFIX_MYCOEF(add2) "(" PREFIX_JET1(jaux) "[k]," PREFIX_JET1(jaux) "[k]," PREFIX_JET1(caux) ");\n\
\t}\n\
\tif ((" PREFIX_JET1(deg) "+1)%2==0) {\n\
\t\tk=" PREFIX_JET1(deg) ";\n\
\t\t" PREFIX_MYCOEF(mul2) "(t[k],a[k]," PREFIX_JET1(jaux) "[0]);\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(t[k],t[k],k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j]," PREFIX_JET1(jaux) "[j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k],a[k]);\n\
\t}\n\
}\n"\
"\n" \
  "\n" \
  "void " PREFIX_JET1(set_atan) "(" PREFIX_JET1(t) " t, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_MYCOEF(set_atan) "(t[0],a[0]);\n\
\t" PREFIX_JET1(set_sq) "(" PREFIX_JET1(jaux) ",a);\n\
\t" PREFIX_JET1(add2_si) "(" PREFIX_JET1(jaux) "," PREFIX_JET1(jaux) ",1);\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(set_zero) "(t[k]);\n\
\t\tfor (j = 0; j < k; j++) {\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",t[j]," PREFIX_JET1(jaux) "[k-j]);\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",j);\n\
\t\t\t" PREFIX_MYCOEF(add2) "(t[k],t[k]," PREFIX_JET1(caux) ");\n\
\t\t}\n\
\t\t" PREFIX_MYCOEF(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_MYCOEF(sub2) "(t[k],a[k],t[k]);\n\
\t\t" PREFIX_MYCOEF(div2) "(t[k],t[k]," PREFIX_JET1(jaux) "[0]);\n\
\t}\n\
}\n"\
"\n" \
  "#define " PREFIX_JET1(sinhcosh) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k;\\\n\
\t" PREFIX_MYCOEF(set_sinh) "(s[0],a[0]);\\\n\
\t" PREFIX_MYCOEF(set_cosh) "(c[0],a[0]);\\\n\
\tfor (k = 1; k <= " PREFIX_JET1(deg) "; k++) {\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(s[k],a[k],c[0]);\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_MYCOEF(mul2) "(c[k],a[k],s[0]);\\\n\
\t\t" PREFIX_MYCOEF(mul2_si) "(c[k],c[k],k);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j],c[j]);\\\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(s[k],s[k]," PREFIX_JET1(caux) ");\\\n\
\\\n\
\t\t\t" PREFIX_MYCOEF(mul2) "(" PREFIX_JET1(caux) ",a[k-j],s[j]);\\\n\
\t\t\t" PREFIX_MYCOEF(mul2_si) "(" PREFIX_JET1(caux) "," PREFIX_JET1(caux) ",k-j);\\\n\
\t\t\t" PREFIX_MYCOEF(add2) "(c[k],c[k]," PREFIX_JET1(caux) ");\\\n\
\t\t}\\\n\
\t\t" PREFIX_MYCOEF(div2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_MYCOEF(div2_si) "(c[k],c[k],k);\\\n\
\t}\\\n\
}\n" \
"\n" \
"void " PREFIX_JET1(set_cosh) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sinhcosh) "(" PREFIX_JET1(jaux) ",c,a);}\n" \
"\n" \
"void " PREFIX_JET1(set_sinh) "(" PREFIX_JET1(t) " s, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sinhcosh) "(s," PREFIX_JET1(jaux) ",a);}\n" \
"\n" \
  "void " PREFIX_JET1(set_fabs) "(" PREFIX_JET1(t) " f, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(set_fabs) "(f[k],a[k]);\n\
\t}\n\
}\n" \
"void " PREFIX_JET1(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET1(t) " s)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <= " PREFIX_JET1(deg) "; k++) {\n\
\t\t" PREFIX_MYCOEF(fprintf) "(file,fmt,s[k]);\n\
\t}\n\
}\n\
" \
"void " PREFIX_JET1(fscanf) "(FILE *file, const char *fmt, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < " MAX_COEFFS_COUNT_NAME "; k++) {\n\
\t\t" PREFIX_MYCOEF(fscanf) "(file, fmt, " MYDATA_ACCESS "(a,k));\n\
\t}\n\
}\n\
" \
  "static char *" PREFIX_JET1(sscanf4) "(char *ptr, const char *myfmt, " PREFIX_JET1(t) " a, int *l)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k <" MAX_COEFFS_COUNT_NAME " && *ptr!=\'\\0\'; k++){\n\
\t\tptr = " PREFIX_MYCOEF(sscanf4) "(ptr, myfmt, " MYDATA_ACCESS "(a,k), l);\n\
\t}\n\
\tfor ( ; k <" MAX_COEFFS_COUNT_NAME "; k++) {" PREFIX_MYCOEF(set_zero) "(" MYDATA_ACCESS "(a,k));}\n\
\treturn ptr;\n\
}\n\
void " PREFIX_JET1(sscanf) "(const char *str, const char *fmt, " PREFIX_JET1(t) " a)\n\
{\n\
\tint l;\n\
\tconst int len=fmt?strnlen(fmt,128):0; /* NOTE: safety string length of 128 */\n\
\tchar myfmt[len+3], *mystr = strdup(str);\n\
\tsprintf(myfmt, \"\%s%%n\",fmt);\n\
\t" PREFIX_JET1(sscanf4) "(mystr, myfmt, a, &l);\n\
\tfree(mystr);\n\
}\n" \

#define MY_JET1_POSTCODE(PREFIX_JET1,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END CODE " PREFIX_JET1(t) " */\n" \

#endif /* MY_JET1_H */
