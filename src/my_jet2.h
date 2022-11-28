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

#define MY_JET2_API(PREFIX_JET2,MY_JET2_TYPE,PREFIX_SCAL,SCAL,I) \
  MY_JET_APIS(PREFIX_JET2,MY_JET2_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET2_PREHEADER(PREFIX_JET2,MY_JET2_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET2(t) " */\n"\

#define MY_JET2_HEADER(PREFIX_JET2,MY_JET2_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET2_API(PREFIX_JET2,MY_JET2_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET2_POSTHEADER(PREFIX_JET2,MY_JET2_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET2(t) " */\n" \


/* *******  *************  ******* */
/* *******  MY_MP2_HEADER  ******* */
/* *******  *************  ******* */
#define MY_JET2_MP_HEADER(PREFIX_MP2,PREFIX_SCAL,I,UI) "\
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
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I ");\
\n\
void " PREFIX_MP2(php0_add) "(\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I ");\
\n\
void " PREFIX_MP2(sphp_add) "(" PREFIX_SCAL(t) ",\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I ");\
\n\
void " PREFIX_MP2(sphp0_add) "(" PREFIX_SCAL(t) ",\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I ");\
\n\
void " PREFIX_MP2(iphp_add) "(" I ",\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I ");\
\n\
void " PREFIX_MP2(iphp0_add) "(" I ",\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I " ,\
" PREFIX_SCAL(t) " *," I ");\
\n\
\n" \

/* *******  ***********  ******* */
/* *******  MY_MP2_CODE  ******* */
/* *******  ***********  ******* */
#define MY_MP2_PRECODE(PREFIX_MP2,PREFIX_SCAL,I,UI) "\
" \

#define MY_MP2_CODE(PREFIX_MP2,PREFIX_SCAL,I,UI) "\
static unsigned ** " PREFIX_MP2(clmo) "=NULL;\n\
static " I " **" PREFIX_MP2(psi) "=NULL;\n\
\n\
static void " PREFIX_MP2(initup) "(" I " nr) {\n\
}\n\
\n\
static void " PREFIX_MP2(cleanup) "(" I " nr) {\n\
}\n\
\n\
static void " PREFIX_MP2(prxk) "(" I " k[]) {\n\
\tif (k[0] != 0) {k[0]--; k[1]++; return;} \n\
\tfprintf(stderr,\"%s:%d prxk error\\n\", __FILE__, __LINE__);\n\
\texit(1);\n\
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
" PREFIX_SCAL(t) " *p3," I " g3,\
" PREFIX_SCAL(t) " *p1," I " g1,\
" PREFIX_SCAL(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,p1[i],p2[j]);\n\
\t\t" PREFIX_SCAL(add2) "(p3[i+j],p3[i+j],my_scal_tmp);\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(php0_add) "(\
" PREFIX_SCAL(t) " *p3," I " g3,\
" PREFIX_SCAL(t) " *p1," I " g1,\
" PREFIX_SCAL(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_SCAL(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(php_add) "(p3,g3,p1,g1,p2,g2);\n\
}\n\
\n\
static void " PREFIX_MP2(sphp_add) "(" PREFIX_SCAL(t) " alph,\
" PREFIX_SCAL(t) " *p3," I " g3,\
" PREFIX_SCAL(t) " *p1," I " g1,\
" PREFIX_SCAL(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,p1[i],p2[j]);\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,alph);\n\
\t\t" PREFIX_SCAL(add2) "(p3[i+j],p3[i+j],my_scal_tmp);\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(sphp0_add) "(" PREFIX_SCAL(t) " alph,\
" PREFIX_SCAL(t) " *p3," I " g3,\
" PREFIX_SCAL(t) " *p1," I " g1,\
" PREFIX_SCAL(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_SCAL(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(sphp0_add) "(alph,p3,g3,p2,g2,p1,g1);\n\
}\n\
\n\
static void " PREFIX_MP2(iphp_add) "(" I " alph,\
" PREFIX_SCAL(t) " *p3," I " g3,\
" PREFIX_SCAL(t) " *p1," I " g1,\
" PREFIX_SCAL(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g1; ++i) for (j = 0; j <= g2; j++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,p1[i],p2[j]);\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,alph);\n\
\t\t" PREFIX_SCAL(add2) "(p3[i+j],p3[i+j],my_scal_tmp);\n\
\t}\n\
}\n\
\n\
static void " PREFIX_MP2(iphp0_add) "(" I " alph,\
" PREFIX_SCAL(t) " *p3," I " g3,\
" PREFIX_SCAL(t) " *p1," I " g1,\
" PREFIX_SCAL(t) " *p2," I " g2) {\n\
\t" I " i,j;\n\
\tfor (i = 0; i <= g3; ++i) {\
" PREFIX_SCAL(set_zero) "(p3[i]);\
}\n\
\t" PREFIX_MP2(iphp0_add) "(alph,p3,g3,p2,g2,p1,g1);\n\
}\n\
\n\
" \

#define MY_MP2_MACRO_CODE(PREFIX_MP2,PREFIX_SCAL,I,UI) "\
#define " PREFIX_MP2(initup) "(nr) {}\n\
\n\
#define " PREFIX_MP2(cleanup) "(nr) {}\n\
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
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,p1[i],p2[j]);\\\n\
\t\t" PREFIX_SCAL(add2) "(p3[i+j],p3[i+j],my_scal_tmp);\\\n\
\t}\\\n\
}\n\
\n\
#define " PREFIX_MP2(php0_add) "(\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_SCAL(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(php_add) "(p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
#define " PREFIX_MP2(sphp_add) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g1; i++) for (j = 0; j <= g2; j++) {\\\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,p1[i],p2[j]);\\\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,alph);\\\n\
\t\t" PREFIX_SCAL(add2) "(p3[i+j],p3[i+j],my_scal_tmp);\\\n\
\t}\\\n\
}\n\
#define " PREFIX_MP2(sphp0_add) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_SCAL(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(sphp_add) "(alph,p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
#define " PREFIX_MP2(siphp_add) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g1; i++) for (j = 0; j <= g2; j++) {\\\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,p1[i],p2[j]);\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,alph);\\\n\
\t\t" PREFIX_SCAL(add2) "(p3[i+j],p3[i+j],my_scal_tmp);\\\n\
\t}\\\n\
}\n\
#define " PREFIX_MP2(siphp0_add) "(alph,\
p3,g3,p1,g1,p2,g2,i,j) {\\\n\
\tfor (i = 0; i <= g3; i++) {" PREFIX_SCAL(set_zero) "(p3[i]);}\\\n\
\t" PREFIX_MP2(siphp_add) "(alph,p3,g3,p1,g1,p2,g2,i,j);\\\n\
}\n\
\n\
" \


/* *******  ************  ******* */
/* *******  MY_JET2_CODE  ******* */
/* *******  ************  ******* */
#define MY_JET2_PRECODE(PREFIX_JET2,PREFIX_MP2,PREFIX_SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
/* CODE FOR " PREFIX_JET2(t) " */\n\
int * " PREFIX_JET2(monomial_counts) "(void) {return _monomial_counts_;}\n\
int * " PREFIX_JET2(monomial_offsets) "(void) {return _monomial_offsets_;}\n\
\n"\

#define MY_JET2_CODE(PREFIX_JET2,PREFIX_MP2,PREFIX_SCAL,I) "\
" MY_MP2_MACRO_CODE(PREFIX_MP2,PREFIX_SCAL,I,I) "\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n\
#define my_scal_tmp " PREFIX_JET2(saux) "\n\
#define my_jet_tmp " PREFIX_JET2(jaux) "\n\
#define nc " PREFIX_JET2(nc) "\n\
#define nct " PREFIX_JET2(nct) "\n\
#ifdef _JET_COEFFICIENTS_COUNT_TOTAL_\n\
static " I " *nc=_monomial_counts_,nct=_JET_COEFFICIENTS_COUNT_TOTAL_;\n\
#else\n\
static " I " *nc=NULL,nct=0;\n\
#endif\n\
#define nct " PREFIX_JET2(nct) "\n\
#ifndef _MAX_DEGREE_OF_JET_VARS_\n\
#define _MAX_DEGREE_OF_JET_VARS_ 0\n\
#endif\n\
static int flag_init_jet_library=0;\n\
static " I " max_deg=_MAX_DEGREE_OF_JET_VARS_+1, cur_deg=_MAX_DEGREE_OF_JET_VARS_+1;\n\
static " PREFIX_SCAL(t) " my_scal_tmp;\n\
static " PREFIX_JET2(t) " my_jet_tmp=NULL;\n\
#pragma omp threadprivate(cur_deg,my_scal_tmp,my_jet_tmp)\n\
  \n" \
  "\n" \
  "size_t " PREFIX_JET2(init) "(" PREFIX_JET2(ptr) " x)\n\
{\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\tif (max_deg==0) return mem;\n\
\t*x = (__typeof__(*x)) malloc(max_deg*sizeof(**x));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"%s:%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__,max_deg);\n\
\t\texit(1);\n\
\t}\n\
\tmem+= max_deg*sizeof(**x);\n\
\t(*x)[0] = (__typeof__(**x)) malloc(nct*sizeof(***x));\n\
\tif ((*x)[0] == NULL) {\n\
\t\tfprintf(stderr, \"%s:%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__,max_deg);\n\
\t\texit(1);\n\
\t}\n\
\tmem+= nct*sizeof(***x);\n\
\tfor (k = 0; k < nct; k++) {" PREFIX_SCAL(init) "((*x)[0][k]);}\n\
\tfor (k = 1; k < max_deg; k++) {(*x)[k] = (*x)[k-1] + nc[k-1];}\n\
\treturn mem;\n\
}\n" \
  "void " PREFIX_JET2(initup) "(" I " nsymb, " I " deg)\n\
{\n\
\tif (flag_init_jet_library==1) return;\n\
#pragma omp single\n\
{\n\
\t" I " k;\n\
\tif (nsymb != 2) {\n\
\t\tfprintf(stderr, \
\"%s:%d Number of symbols %d!=2 not allowed\\n\",\
__FILE__,__LINE__,nsymb);\n\
\t\texit(1);\n\
\t}\n\
\tif (max_deg != 0 && deg+1 != max_deg) {\n\
\t\tfprintf(stderr, \
\"%s:%d Maximum degree not allowed %d!=%d not allowed\\n\",\
__FILE__,__LINE__,deg,max_deg);\n\
\t\texit(1);\n\
\t}\n\
\tmax_deg = deg+1;\n\
\t" PREFIX_MP2(initup) "(max_deg);\n\
\tif (nc==NULL) {\n\
\t\tnc = (__typeof__(nc)) malloc(max_deg*sizeof(*nc));\n\
\t\tif (nc==NULL) {\n\
\t\t\tfprintf(stderr, \"%s:%d not enough memory to allocate \%d\\n\",__FILE__,__LINE__,max_deg);\n\
\t\t\texit(1);\n\
\t\t}\n\
\t\tnct=0;\n\
\t\tfor (k = 0; k < max_deg; ++k) {\n\
\t\t\tnc[k] = " PREFIX_MP2(ntph) "(k);\n\
\t\t\tnct+= nc[k];\n\
\t\t}\n\
\t}\n\
}\n\
\tcur_deg = max_deg;\n\
\t" PREFIX_JET2(init) "(&my_jet_tmp);\n\
\t" PREFIX_SCAL(init) "(my_scal_tmp);\n\
\tflag_init_jet_library=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET2(get_num_symbs) "(void)\n\
{\
return 2;\
}\n" \
  "\n" \
  I " " PREFIX_JET2(get_deg) "(void)\n\
{\
return cur_deg;\
}\n" \
  "\n" \
  I " " PREFIX_JET2(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\tfprintf(stderr, \"%s:%d Action not allowed\\n\",__FILE__,__LINE__);\n\
\texit(1);\n\
\treturn 0;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET2(set_deg) "(" I " deg)\n\
{\n\
\t" I " k;\n\
\tif (deg+1 > max_deg) {\n\
\t\tfprintf(stderr, \"%s:%d Invalid degree \%d > \%d\\n\",__FILE__,__LINE__, deg, max_deg);\n\
\t\texit(1);\n\
\t}\n\
\tk = cur_deg;\n\
\tcur_deg = deg+1;\n\
\treturn k;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(clean) "(" PREFIX_JET2(ptr) " x)\n\
{\n\
\t" I " k;\n\
\tif (*x == NULL) return;\n\
\tfor (k = 0; k < nct; k++) {" PREFIX_SCAL(clean) "((*x)[0][k]);}\n\
\tfor (k = 1; k < max_deg; k++) {(*x)[k]=NULL;}\n\
\tfree((*x)[0]); (*x)[0]=NULL;\n\
\tfree(*x); *x=NULL;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(cleanup) "(void)\n\
{\n\
\tif (flag_init_jet_library==0) return;\n\
\tif (max_deg == 0) return;\n\
\t" PREFIX_SCAL(clean) "(my_scal_tmp);\n\
\t" PREFIX_JET2(clean) "(&my_jet_tmp);\n\
\tcur_deg = 0;\n\
#pragma omp single\n\
{\n\
\t" PREFIX_MP2(cleanup) "();\n\
\tmax_deg = 0;\n\
}\n\
\tflag_init_jet_library=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set) "(" PREFIX_JET2(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(b[k][j],a[k][j]);\
}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_scal) "(" PREFIX_JET2(t) " b, " PREFIX_SCAL(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set) "(b[0][0],a);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set_zero) "(b[k][j]);}\n\
}\n" \
"\n"\
  "void " PREFIX_JET2(set_scal_array) "(" PREFIX_JET2(t) " b, " PREFIX_SCAL(t) " *a)\n\
{\n\
\t" I " j,k,m=1;\n\
\t" PREFIX_SCAL(set) "(b[0][0],a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(b[k][j],a[m]);m++;}\n\
}\n" \
  "void " PREFIX_JET2(set_jet_2_scal_array) "(" PREFIX_SCAL(t) " *b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,m=1;\n\
\t" PREFIX_SCAL(set) "(b[0],a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(b[m],a[k][j]);m++;}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_d) "(" PREFIX_JET2(t) " b, double a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_d) "(b[0][0],a);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set_zero) "(b[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(set_si) "(" PREFIX_JET2(t) " b, int a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_si) "(b[0][0],a);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set_zero) "(b[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(to_scal) "(" PREFIX_SCAL(t) " t[1], " PREFIX_JET2(t) " a) \
{" PREFIX_SCAL(set) "(*t,a[0][0]);}\n" \
  "\n" \
  "void " PREFIX_JET2(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_fabs) "(*nrm,a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_SCAL(set_fabs) "(my_scal_tmp,a[k][j]);\n\
\t\tif (" PREFIX_SCAL(lt) "(*nrm,my_scal_tmp)) \
{" PREFIX_SCAL(set) "(*nrm,my_scal_tmp);}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(nrm2) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(mul2) "(*nrm,a[0][0],a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k][j],a[k][j]);\n\
\t\t" PREFIX_SCAL(add2) "(*nrm,*nrm,my_scal_tmp);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(neg) "(" PREFIX_JET2(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(neg) "(b[k][j],a[k][j]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(add2) "(c[k][j],a[k][j],b[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(sub2) "(c[k][j],a[k][j],b[k][j]);}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET2(set_sq) "(s,a)\\\n\
{\\\n\
\t" I " j,k,ii,jj, nk;\\\n\
\t" PREFIX_SCAL(mul2) "(s[0][0],a[0][0],a[0][0]);\\\n\
\tnk = cur_deg - (cur_deg%2==0);\\\n\
\tfor (k = 1; k < nk; k++) {\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_MP2(php0_add) "(s[k],k,a[0],0,a[k],k,ii,jj);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \\\n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[k-j],k-j,ii,jj);}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) \\\n\t\t{\
" PREFIX_SCAL(mul2_si) "(s[k][j],s[k][j],2);}\\\n\
\t\tk++;\\\n\
\t\t/* k even */\\\n\
\t\t" PREFIX_MP2(php0_add) "(s[k],k,a[0],0,a[k],k,ii,jj);\\\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) \\\n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[k-j],k-j,ii,jj);}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) \\\n\t\t{\
" PREFIX_SCAL(mul2_si) "(s[k][j],s[k][j],2);}\\\n\
\t\tj = k/2;\\\n\
\t\t" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[j],j,ii,jj);\\\n\
\t}\\\n\
\tif (cur_deg%2==0) {\\\n\
\t\tk=cur_deg-1;\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_MP2(php0_add) "(s[k],k,a[0],0,a[k],k,ii,jj);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \\\n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,a[j],j,a[k-j],k-j,ii,jj);}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) \\\n\t\t{\
" PREFIX_SCAL(mul2_si) "(s[k][j],s[k][j],2);}\\\n\
\t}\\\n\
}\n" \
  "void " PREFIX_JET2(mul2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t//if (a==b) {\
" PREFIX_JET2(set_) "(c,a);\
return;}\n\
\t" I " j,k,ii,jj;\n\
\t/* c_k = sum(a_{j}b_{k-j}, j=0,...,k) */\n\
\t" PREFIX_SCAL(mul2) "(c[0][0],a[0][0],b[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,a[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j <= k; j++) {\n\
\t\t\t" PREFIX_MP2(php_add) "(c[k],k,a[j],j,b[k-j],k-j,ii,jj);\n\
\t\t}\n\
\t}\n\
}\n" \
  "void " PREFIX_JET2(div2) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(div2) "(c[0][0],a[0][0],b[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(sub2) "(c[k][j],a[k][j],c[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_scal) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(add2) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_scal) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(sub2) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(scal_sub2) "(" PREFIX_JET2(t) " c, " PREFIX_SCAL(t) " b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(sub2) "(c[0][0],b,a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_scal) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(mul2) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_scal) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(div2) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(scal_div2) "(" PREFIX_JET2(t) " c, " PREFIX_SCAL(t) " a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(div2) "(c[0][0],a,b[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(add2_d) "(c[0][0],a[0][0],b);\n\
\tif (c!=a) for (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(sub2_d) "(c[0][0],a[0][0],b);\n\
\tif (c!=a) for (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(d_sub2) "(" PREFIX_JET2(t) " c, double b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_d) "(my_scal_tmp,b);\n\
\t" PREFIX_SCAL(sub2) "(c[0][0],my_scal_tmp,a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(mul2_d) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_d) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, double b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(div2_d) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(d_div2) "(" PREFIX_JET2(t) " c, double a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(set_d) "(my_scal_tmp,a);\n\
\t" PREFIX_SCAL(div2) "(c[0][0],my_scal_tmp,b[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(add2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(add2_si) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(sub2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(sub2_si) "(c[0][0],a[0][0],b);\n\
\tif(c!=a) for (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(set) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(si_sub2) "(" PREFIX_JET2(t) " c, int b, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_si) "(my_scal_tmp,b);\n\
\t" PREFIX_SCAL(sub2) "(c[0][0],my_scal_tmp,a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(neg) "(c[k][j],a[k][j]);}\n\
}\n" \
  "void " PREFIX_JET2(mul2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(mul2_si) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(div2_si) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a, int b)\n\
{\n\
\t" I " j,k;\n\
\tfor (k = 0; k < cur_deg; k++) for (j = 0; j < nc[k]; j++) \n\t{\
" PREFIX_SCAL(div2_si) "(c[k][j],a[k][j],b);}\n\
}\n" \
  "void " PREFIX_JET2(si_div2) "(" PREFIX_JET2(t) " c, int a, " PREFIX_JET2(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(set_si) "(my_scal_tmp,a);\n\
\t" PREFIX_SCAL(div2) "(c[0][0],my_scal_tmp,b[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(c[k],k,c[0],0,b[k],k,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(c[k],k,c[j],j,b[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(neg) "(c[k][j],c[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(c[k][j],c[k][j],b[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_sqrt) "(" PREFIX_JET2(t) " s, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj, nk;\n\
#define tmp my_jet_tmp[0][0]\n\
\t" PREFIX_SCAL(set_sqrt) "(s[0][0],a[0][0]);\n\
\t" PREFIX_SCAL(mul2_si) "(tmp,s[0][0],2);\n\
\tnk = cur_deg - (cur_deg%2==0);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t/* k odd */\n\
\t\tfor (j = 0; j < nc[k]; j++) {" PREFIX_SCAL(set_zero) "(s[k][j]);}\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,s[j],j,s[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(s[k][j],s[k][j],2);\n\
\t\t\t" PREFIX_SCAL(sub2) "(s[k][j],a[k][j],s[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(s[k][j],s[k][j],tmp);\n\
\t\t}\n\
\t\tk++;\n\
\t\t/* k even */\n\
\t\tfor (j = 0; j < nc[k]; j++) {" PREFIX_SCAL(set_zero) "(s[k][j]);}\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(2,s[k],k,s[j],j,s[k-j],k-j,ii,jj);}\n\
\t\tj = k/2;\n\
\t\t" PREFIX_MP2(php_add) "(s[k],k,s[j],j,s[j],j,ii,jj);\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(sub2) "(s[k][j],a[k][j],s[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(s[k][j],s[k][j],tmp);\n\
\t\t}\n\
\t}\n\
\tif (cur_deg%2==0) {\n\
\t\tk=cur_deg-1;\n\
\t\t/* k odd */\n\
\t\tfor (j = 0; j < nc[k]; j++) {" PREFIX_SCAL(set_zero) "(s[k][j]);}\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(s[k],k,s[j],j,s[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[k][j],2);\n\
\t\t\t" PREFIX_SCAL(sub2) "(s[k][j],a[k][j],my_scal_tmp);\n\
\t\t\t" PREFIX_SCAL(div2) "(s[k][j],s[k][j],tmp);\n\
\t\t}\n\
\t}\n\
#undef tmp\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_pow_scal) "(" PREFIX_JET2(t) " p, " PREFIX_JET2(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " j,k,ii,jj;\n\
#define tmp my_jet_tmp[0][0]\n\
\t" PREFIX_SCAL(set_pow) "(p[0][0],a[0][0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2_si) "(tmp,b,k);\n\
\t\t" PREFIX_MP2(sphp0_add) "(tmp,p[k],k,a[k],k,p[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(tmp,b,k-j);\n\
\t\t\t" PREFIX_SCAL(sub2_si) "(tmp,tmp,j);\n\
\t\t\t" PREFIX_MP2(sphp_add) "(tmp,p[k],k,a[k-j],k-j,p[j],j,ii,jj);\n\
\t\t}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(tmp,a[0][0],k);\n\
\t\t\t" PREFIX_SCAL(div2) "(p[k][j],p[k][j],tmp);\n\
\t\t}\n\
\t}\n\
#undef tmp\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_exp) "(" PREFIX_JET2(t) " e, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(set_exp) "(e[0][0],a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,e[k],k,a[k],k,e[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(k-j,e[k],k,a[k-j],k-j,e[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) \n\t\t{\
" PREFIX_SCAL(div2_si) "(e[k][j],e[k][j],k);}\n\
\t}\n\
}\n" \
  "void " PREFIX_JET2(set_log) "(" PREFIX_JET2(t) " l, " PREFIX_JET2(t) " a)\n\
{\n\
\t/* Assump. l!=a */\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(set_log) "(l[0][0],a[0][0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_MP2(php0_add) "(l[k],k,a[k-1],k-1,l[1],1,ii,jj);\n\
\t\tfor (j = 2; j < k; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(j,l[k],k,a[k-j],k-j,l[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(div2_si) "(l[k][j],l[k][j],k);\n\
\t\t\t" PREFIX_SCAL(sub2) "(l[k][j],a[k][j],l[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(l[k][j],l[k][j],a[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET2(sincos) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k,ii,jj;\\\n\
\t" PREFIX_SCAL(set_sin) "(s[0][0],a[0][0]);\\\n\
\t" PREFIX_SCAL(set_cos) "(c[0][0],a[0][0]);\\\n\
\tfor (k = 1; k < cur_deg; k++) {\\\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,s[k],k,a[k],k,c[0],0,ii,jj);\\\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,c[k],k,a[k],k,s[0],0,ii,jj);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_MP2(siphp_add) "(k-j,s[k],k,a[k-j],k-j,c[j],j,ii,jj);\\\n\
\t\t\t" PREFIX_MP2(siphp_add) "(k-j,c[k],k,a[k-j],k-j,s[j],j,ii,jj);\\\n\
\t\t}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) {\\\n\
\t\t\t" PREFIX_SCAL(div2_si) "(s[k][j],s[k][j],k);\\\n\
\t\t\t" PREFIX_SCAL(div2_si) "(c[k][j],c[k][j],-k);\\\n\
\t\t}\\\n\
\t}\\\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_cos) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sincos) "(my_jet_tmp,c,a);}\n" \
  "\n" \
  "void " PREFIX_JET2(set_sin) "(" PREFIX_JET2(t) " s, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sincos) "(s,my_jet_tmp,a);}\n" \
  "\n" \
  "void " PREFIX_JET2(set_tan) "(" PREFIX_JET2(t) " t, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj ,nk;\n\
\t" PREFIX_SCAL(set_tan) "(t[0][0],a[0][0]);\n\
\t" PREFIX_SCAL(mul2) "(my_jet_tmp[0][0],t[0][0],t[0][0]);\n\
\tnk = cur_deg - (cur_deg%2==0);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,t[k],k,a[k],k,my_jet_tmp[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(k-j,t[k],k,a[k-j],k-j,my_jet_tmp[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k][j],t[k][j],a[k][j]);\n\
\t\t}\n\
\t\t/* k odd */\n\
\t\t" PREFIX_MP2(php0_add) "(my_jet_tmp[k],k,t[0],0,t[k],k,ii,jj);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++)\n\t\t{\
" PREFIX_MP2(php_add) "(my_jet_tmp[k],k,t[j],j,t[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++)\n\t\t{\
" PREFIX_SCAL(mul2_si) "(my_jet_tmp[k][j],my_jet_tmp[k][j],2);}\n\
\t\tk++;\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,t[k],k,a[k],k,my_jet_tmp[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(k-j,t[k],k,a[k-j],k-j,my_jet_tmp[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k][j],t[k][j],a[k][j]);\n\
\t\t}\n\
\t\t/* k even */\n\
\t\t" PREFIX_MP2(php0_add) "(my_jet_tmp[k],k,t[0],0,t[k],k,ii,jj);\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) \n\t\t{\
" PREFIX_MP2(php_add) "(my_jet_tmp[k],k,t[j],j,t[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) \n\t\t{\
" PREFIX_SCAL(mul2_si) "(my_jet_tmp[k][j],my_jet_tmp[k][j],2);}\n\
\t\tj = k/2;\n\
\t\t" PREFIX_MP2(php_add) "(my_jet_tmp[k],k,t[j],j,t[j],j,ii,jj);\n\
\t}\n\
\tif (cur_deg%2==0) {\n\
\t\tk=cur_deg-1;\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,t[k],k,a[k],k,my_jet_tmp[0],0,ii,jj);\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(k-j,t[k],k,a[k-j],k-j,my_jet_tmp[j],j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k][j],t[k][j],a[k][j]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET2(set_atan) "(" PREFIX_JET2(t) " t, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k,ii,jj;\n\
\t" PREFIX_SCAL(set_atan) "(t[0][0],a[0][0]);\n\
\t" PREFIX_JET2(set_sq) "(my_jet_tmp,a);\n\
\t" PREFIX_JET2(add2_si) "(my_jet_tmp,my_jet_tmp,1);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\tfor (j = 0; j < nc[k]; j++) \n\t\t{\
" PREFIX_SCAL(set_zero) "(t[k][j]);}\n\
\t\tfor (j = 1; j < k; j++) \n\t\t{\
" PREFIX_MP2(siphp_add) "(j,t[k],k,t[j],j,my_jet_tmp[k-j],k-j,ii,jj);}\n\
\t\tfor (j = 0; j < nc[k]; j++) {\n\
\t\t\t" PREFIX_SCAL(div2_si) "(t[k][j],t[k][j],k);\n\
\t\t\t" PREFIX_SCAL(sub2) "(t[k][j],a[k][j],t[k][j]);\n\
\t\t\t" PREFIX_SCAL(div2) "(t[k][j],t[k][j],my_jet_tmp[0][0]);\n\
\t\t}\n\
\t}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET2(sinhcosh) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k,ii,jj;\\\n\
\t" PREFIX_SCAL(set_sinh) "(s[0][0],a[0][0]);\\\n\
\t" PREFIX_SCAL(set_cosh) "(c[0][0],a[0][0]);\\\n\
\tfor (k = 1; k < cur_deg; k++) {\\\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,s[k],k,a[k],k,c[0],0,ii,jj);\\\n\
\t\t" PREFIX_MP2(siphp0_add) "(k,c[k],k,a[k],k,s[0],0,ii,jj);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_MP2(siphp_add) "(k-j,s[k],k,a[k-j],k-j,c[j],j,ii,jj);\\\n\
\t\t\t" PREFIX_MP2(siphp_add) "(k-j,c[k],k,a[k-j],k-j,s[j],j,ii,jj);\\\n\
\t\t}\\\n\
\t\tfor (j = 0; j < nc[k]; j++) {\\\n\
\t\t\t" PREFIX_SCAL(div2_si) "(s[k][j],s[k][j],k);\\\n\
\t\t\t" PREFIX_SCAL(div2_si) "(c[k][j],c[k][j],k);\\\n\
\t\t}\\\n\
\t}\\\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET2(set_cosh) "(" PREFIX_JET2(t) " c, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sinhcosh) "(my_jet_tmp,c,a);}\n" \
  "\n" \
  "void " PREFIX_JET2(set_sinh) "(" PREFIX_JET2(t) " s, " PREFIX_JET2(t) " a)\n\
{" PREFIX_JET2(sinhcosh) "(s,my_jet_tmp,a);}\n" \
  "\n" \
  "void " PREFIX_JET2(get_val) "(" PREFIX_SCAL(t) " val[1], " PREFIX_JET2(t) " a, const " I " k[])\n\
{\n\
\t" I " no=k[0]+k[1],ll=" PREFIX_MP2(exll) "(no,k);\n\
\t" PREFIX_SCAL(set) "(*val,a[no][ll]);\n\
}\n"\
  "\n" \
  "void " PREFIX_JET2(set_coef) "(" PREFIX_JET2(t) " a, const " I " k[], " PREFIX_SCAL(t) " val)\n\
{\n\
\t" I " no=k[0]+k[1], ll=" PREFIX_MP2(exll) "(no,k);\n\
\t" PREFIX_SCAL(set) "(a[no][ll],val);\n\
}\n"\
  "\n" \
  "void " PREFIX_JET2(fprintf) "(FILE *file, const char* fmt, " PREFIX_JET2(t) " a)\n\
{\n\
\t" I " j,k;\n\
\tfor(k = 0; k < max_deg; k++) \
for (j = 0; j < nc[k]; j++) \n\t{\
OutputMyFloat3(file, fmt, a[k][j]);}\n\
}\n\
#undef nct\n\
#undef nc\n\
#undef my_jet_tmp\n\
#undef my_scal_tmp\n" \

#define MY_JET2_POSTCODE(PREFIX_JET2,PREFIX_MP2,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET2(t) " */\n" \

#endif /* MY_JET2_H */
