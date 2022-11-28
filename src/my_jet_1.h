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

#define MY_JET_1_API(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_SCAL,SCAL,I) \
  MY_JET_APIS(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_1_PREHEADER(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET_1(t) " */\n" \

#define MY_JET_1_HEADER(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET_1_API(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_1_POSTHEADER(PREFIX_JET_1,MY_JET_1_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET_1(t) " */\n" \



/* *******  *************  ******* */
/* *******  MY_JET_1_CODE  ******* */
/* *******  *************  ******* */
#define MY_JET_1_PRECODE(PREFIX_JET_1,PREFIX_SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
/* CODE FOR " PREFIX_JET_1(t) " */\n\
int * " PREFIX_JET_1(monomial_counts) "(void) {return _monomial_counts_;}\n\
int * " PREFIX_JET_1(monomial_offsets) "(void) {return _monomial_offsets_;}\n\
\n"\

#define MY_JET_1_CODE(PREFIX_JET_1,PREFIX_SCAL,I) "\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n\
#ifndef _NUMBER_OF_JET_symbs_\n\
#define _NUMBER_OF_JET_symbs_ 0\n\
#endif\n\
#define my_scal_tmp " PREFIX_JET_1(tmp) "\n\
static int flag_init_jet_library=0;\n\
static " I " num_symbs=_MAX_SIZE_OF_JET_VAR_;\n\
static " PREFIX_SCAL(t) " my_scal_tmp;\n\
#pragma omp threadprivate(my_scal_tmp)\n\
\n" \
  "size_t " PREFIX_JET_1(init) "(" PREFIX_JET_1(ptr) " x)\n\
{\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\tif (num_symbs==0) return mem;\n\
\t*x = (__typeof__(*x)) malloc(num_symbs*sizeof(**x));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"not enough memory to allocate \%d\\n\",num_symbs);\n\
\t\texit(1);\n\
\t}\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(init) "((*x)[k]);}\n\
\tmem+= num_symbs*sizeof(**x);\n\
\treturn mem;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(initup) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (flag_init_jet_library==1) return;\n\
\tif (deg != 1) {\n\
\t\tfprintf(stderr, \"%d: Not allowed deg value: \%d!=1\\n\",__LINE__,deg);\n\
\t\texit(1);\n\
\t}\n\
\tif (num_symbs != 0 && num_symbs < nsymbs) {\n\
\t\tfprintf(stderr, \"%d: Not allowed num_symbs value \%d<\%d\\n\",__LINE__,num_symbs,nsymbs);\n\
\t\texit(1);\n\
\t}\n\
\tnum_symbs = nsymbs+1;\n\
\t" PREFIX_SCAL(init) "(my_scal_tmp);\n\
\tflag_init_jet_library=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_1(get_num_symbs) "(void)\
{\
return num_symbs;\
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
\tif (nsymbs > num_symbs) {\n\
\t\tfprintf(stderr, \"%d: Invalid num. variables \%d > \%d\\n\",__LINE__,nsymbs, num_symbs);\n\
\t\texit(1);\n\
\t}\n\
\tk = num_symbs;\n\
\tnum_symbs = nsymbs+1;\n\
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
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(clean) "((*x)[k]);}\n\
\tfree(*x); *x=NULL;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(cleanup) "(void)\n\
{\n\
\tif (flag_init_jet_library==0) return;\n\
\t" PREFIX_SCAL(clean) "(my_scal_tmp);\n\
\tnum_symbs = 0;\n\
\tflag_init_jet_library=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set) "(" PREFIX_JET_1(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_scal) "(" PREFIX_JET_1(t) " b, " PREFIX_SCAL(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set) "(b[0],a);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET_1(set_scal_array) "(" PREFIX_JET_1(t) " b, " PREFIX_SCAL(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_jet_2_scal_array) "(" PREFIX_SCAL(t) " *b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_d) "(" PREFIX_JET_1(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_d) "(b[0],a);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(set_si) "(" PREFIX_JET_1(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_si) "(b[0],a);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(to_scal) "(" PREFIX_SCAL(t) " t[1]," PREFIX_JET_1(t) " a) {" PREFIX_SCAL(set) "(*t,a[0]);}\n" \
  "\n" \
  "void " PREFIX_JET_1(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_fabs) "(*nrm,a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\n\
\t\t" PREFIX_SCAL(set_fabs) "(my_scal_tmp,a[k]);\n\
\t\tif (" PREFIX_SCAL(lt) "(*nrm,my_scal_tmp)) {" PREFIX_SCAL(set) "(*nrm,my_scal_tmp);}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(nrm2) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(mul2) "(*nrm,a[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k],a[k]);\n\
\t\t" PREFIX_SCAL(add2) "(*nrm,*nrm,my_scal_tmp);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(neg) "(" PREFIX_JET_1(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(neg) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(add2) "(c[k],a[k],b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(sub2) "(c[k],a[k],b[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\t/* c' = a'b + ab' */\n\
\tfor (k = 1; k < num_symbs; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k],b[0]);\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],a[0],b[k]);\n\
\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\n\
\t}\n\
\t" PREFIX_SCAL(mul2) "(c[0],a[0],b[0]);\n\
}\n" \
  "void " PREFIX_JET_1(div2) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_JET_1(t) " b)\n\
{\n\
\t" I " k;\n\
\t/* c' = (a' - ab'/b)/b */\n\
\tfor (k = 1; k < num_symbs; k++) {\n\
\t\t" PREFIX_SCAL(set) "(my_scal_tmp,a[k]);\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],a[0],b[k]);\n\
\t\t" PREFIX_SCAL(div2) "(c[k],c[k],b[0]);\n\
\t\t" PREFIX_SCAL(sub2) "(c[k],my_scal_tmp,c[k]);\n\
\t\t" PREFIX_SCAL(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
\t" PREFIX_SCAL(div2) "(c[0],a[0],b[0]);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_scal) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(add2) "(c[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_scal) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2) "(c[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(scal_sub2) "(" PREFIX_JET_1(t) " c, " PREFIX_SCAL(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2) "(c[0],b,a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_scal) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(mul2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_scal) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(div2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(scal_div2) "(" PREFIX_JET_1(t) " c, " PREFIX_SCAL(t) " b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(div2) "(c[0],b,a[0]);\n\
\t" PREFIX_SCAL(div2) "(my_scal_tmp,c[0],a[0]);\n\
\t" PREFIX_SCAL(neg) "(my_scal_tmp,my_scal_tmp);\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(c[k],a[k],my_scal_tmp);\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(add2_d) "(c[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_d) "(c[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(d_sub2) "(" PREFIX_JET_1(t) " c, double b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_d) " (c[0],a[0],b);\n\
\t" PREFIX_SCAL(neg) "(c[0],c[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(mul2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_d) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(div2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(d_div2) "(" PREFIX_JET_1(t) " c, double b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_d) "(my_scal_tmp,b);\n\
\t" PREFIX_SCAL(div2) "(c[0],my_scal_tmp,a[0]);\n\
\t" PREFIX_SCAL(div2) "(my_scal_tmp,c[0],a[0]);\n\
\t" PREFIX_SCAL(neg) "(my_scal_tmp,my_scal_tmp);\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(c[k],a[k],my_scal_tmp);\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(add2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(add2_si) "(c[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(sub2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_si) "(c[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(si_sub2) "(" PREFIX_JET_1(t) " c, int b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_si) "(c[0],a[0],b);\n\
\t" PREFIX_SCAL(neg) "(c[0],c[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {" PREFIX_SCAL(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET_1(mul2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(mul2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(div2_si) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {" PREFIX_SCAL(div2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET_1(si_div2) "(" PREFIX_JET_1(t) " c, int b, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_si) "(my_scal_tmp,b);\n\
\t" PREFIX_SCAL(div2) "(c[0],my_scal_tmp,a[0]);\n\
\t" PREFIX_SCAL(div2) "(my_scal_tmp,c[0],a[0]);\n\
\t" PREFIX_SCAL(neg) "(my_scal_tmp,my_scal_tmp);\n\
\t/* c' = -b(a'/a)/a */\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(c[k],a[k],my_scal_tmp);\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_sqrt) "(" PREFIX_JET_1(t) " s, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* s'=a'/(2s[0]) */\n\
\t" PREFIX_SCAL(set_sqrt) "(s[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\n\
\t\t" PREFIX_SCAL(div2) "(s[k],a[k],s[0]);\n\
\t\t" PREFIX_SCAL(div2_si) "(s[k],s[k],2);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_pow_scal) "(" PREFIX_JET_1(t) " p, " PREFIX_JET_1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_si) "(my_scal_tmp,b,1);\n\
\t/* p'= b p[0]/a[0] a' */\n\
\t" PREFIX_SCAL(set_pow) "(my_scal_tmp,a[0],my_scal_tmp);\n\
\t" PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,b);\n\
\t" PREFIX_SCAL(set_pow) "(p[0],a[0],b);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(p[k],a[k],my_scal_tmp);\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_exp) "(" PREFIX_JET_1(t) " e, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* e'=a'e[0] */\n\
\t" PREFIX_SCAL(set_exp) "(e[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(e[k],e[0],a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_log) "(" PREFIX_JET_1(t) " l, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* l'=a'/a[0] */\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(div2) "(l[k],a[k],a[0]);\
}\n\
\t" PREFIX_SCAL(set_log) "(l[0],a[0]);\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_1(set_sin) "(" PREFIX_JET_1(t) " s, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* s'=cos(a[0])a' */\n\
\t" PREFIX_SCAL(set_cos) "(my_scal_tmp,a[0]);\n\
\t" PREFIX_SCAL(set_sin) "(s[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(s[k],my_scal_tmp,a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_cos) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* c'=-sin(a[0])a' */\n\
\t" PREFIX_SCAL(set_sin) "(my_scal_tmp,a[0]);\n\
\t" PREFIX_SCAL(neg) "(my_scal_tmp,my_scal_tmp);\n\
\t" PREFIX_SCAL(set_cos) "(c[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(c[k],my_scal_tmp,a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_tan) "(" PREFIX_JET_1(t) " t, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* t'= (1+tan(a[0])^2)a'*/\n\
\t" PREFIX_SCAL(set_tan) "(t[0],a[0]);\n\
\t" PREFIX_SCAL(mul2) "(my_scal_tmp,t[0],t[0]);\n\
\t" PREFIX_SCAL(add2_si) "(my_scal_tmp,my_scal_tmp,1);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(t[k],a[k],my_scal_tmp);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_atan) "(" PREFIX_JET_1(t) " t, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* t'= a'/(1+(a[0])^2)*/\n\
\t" PREFIX_SCAL(set_atan) "(t[0],a[0]);\n\
\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[0],a[0]);\n\
\t" PREFIX_SCAL(add2_si) "(my_scal_tmp,my_scal_tmp,1);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(div2) "(t[k],a[k],my_scal_tmp);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_sinh) "(" PREFIX_JET_1(t) " s, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* s'=cosh(a[0])a' */\n\
\t" PREFIX_SCAL(set_cosh) "(my_scal_tmp,a[0]);\n\
\t" PREFIX_SCAL(set_sinh) "(s[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(s[k],my_scal_tmp,a[k]);\
}\n\
}\n" \
  "void " PREFIX_JET_1(set_cosh) "(" PREFIX_JET_1(t) " c, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\t/* c'=sinh(a[0])a' */\n\
\t" PREFIX_SCAL(set_sinh) "(my_scal_tmp,a[0]);\n\
\t" PREFIX_SCAL(set_cosh) "(c[0],a[0]);\n\
\tfor (k = 1; k < num_symbs; k++) {\
" PREFIX_SCAL(mul2) "(c[k],my_scal_tmp,a[k]);\
}\n\
}\n" \
"#undef my_scal_tmp\n"\
  "void " PREFIX_JET_1(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET_1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < num_symbs; k++) {\
OutputMyFloat3(file, fmt, a[k]);\
}\n\
}\n\
#undef my_scal_tmp\n" \

#define MY_JET_1_POSTCODE(PREFIX_JET_1,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET_1(t) " */\n" \

#endif /* MY_JET_1_H */
