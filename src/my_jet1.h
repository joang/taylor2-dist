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

#define MY_JET1_API(PREFIX_JET1,MY_JET1_TYPE,PREFIX_SCAL,SCAL,I) \
  MY_JET_APIS(PREFIX_JET1,MY_JET1_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET1_PREHEADER(PREFIX_JET1,MY_JET1_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET1(t) " */\n"\

#define MY_JET1_HEADER(PREFIX_JET1,MY_JET1_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET1_API(PREFIX_JET1,MY_JET1_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET1_POSTHEADER(PREFIX_JET1,MY_JET1_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET1(t) " */\n" \


/* *******  ************  ******* */
/* *******  MY_JET1_CODE  ******* */
/* *******  ************  ******* */
#define MY_JET1_PRECODE(PREFIX_JET1,PREFIX_SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
/* CODE FOR " PREFIX_JET1(t) " */\n\
int * " PREFIX_JET1(monomial_counts) "(void) {return _monomial_counts_;}\n\
int * " PREFIX_JET1(monomial_offsets) "(void) {return _monomial_offsets_;}\n\
\n"\

#define MY_JET1_CODE(PREFIX_JET1,PREFIX_SCAL,I) "\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n\
#define my_scal_tmp " PREFIX_SCAL(tmp) "\n\
#define my_jet_tmp " PREFIX_JET1(tmp) "\n\
#ifndef _MAX_DEGREE_OF_JET_VARS_\n\
#define _MAX_DEGREE_OF_JET_VARS_ 0\n\
#endif\n\
static int flag_init_jet_library=0;\n\
static " I " max_deg=_MAX_DEGREE_OF_JET_VARS_+1, cur_deg=_MAX_DEGREE_OF_JET_VARS_+1;\n\
static " PREFIX_SCAL(t) " my_scal_tmp;\n\
static " PREFIX_JET1(t) " my_jet_tmp;\n\
#pragma omp threadprivate(cur_deg,my_scal_tmp,my_jet_tmp)\n\
  \n" \
  "\n" \
  "size_t " PREFIX_JET1(init) "(" PREFIX_JET1(ptr) " x)\n\
{\n\
\tsize_t mem=0;\n\
\t" I " k;\n\
\tif (max_deg==0) return mem;\n\
\t*x = (__typeof__(*x)) malloc(max_deg*sizeof(**x));\n\
\tif (*x == NULL) {\n\
\t\tfprintf(stderr, \"%d: not enough memory to allocate \%d\\n\",__LINE__,max_deg);\n\
\t\texit(1);\n\
\t}\n\
\tfor (k = 0; k < max_deg; k++) {" PREFIX_SCAL(init) "((*x)[k]);}\n\
\tmem+= max_deg*sizeof(**x);\n\
\treturn mem;\n\
}\n" \
  "void " PREFIX_JET1(initup) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (flag_init_jet_library==1) return;\n\
\tif (nsymbs != 1) {\n\
\t\tfprintf(stderr, \"%d: Not allowed nsymbs value: \%d!=1\\n\",__LINE__,nsymbs);\n\
\t\texit(1);\n\
\t}\n\
\tif (max_deg != 0 && max_deg < deg) {\n\
\t\tfprintf(stderr, \"%d: Not allowed max_deg value: %d < %d\\n\",__LINE__, max_deg,deg);\n\
\t\texit(1);\n\
\t}\n\
\tmax_deg = deg+1;\n\
\tcur_deg = max_deg;\n\
\t" PREFIX_SCAL(init) "(my_scal_tmp);\n\
\t" PREFIX_JET1(init) "(&my_jet_tmp);\n\
\tflag_init_jet_library=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET1(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\tfprintf(stderr, \"%d: Action not allowed\\n\",__LINE__);\n\
\texit(1);\n\
\treturn 0;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET1(set_deg) "(" I " deg)\n\
{\n\
\t" I " k;\n\
\tif (deg+1 > max_deg) {\n\
\t\tfprintf(stderr, \"%d: Invalid degree \%d > \%d\\n\",__LINE__, deg, max_deg);\n\
\t\texit(1);\n\
\t}\n\
\t/* WARNING: in case of parallel region required out of the scope */\n\
\tk = cur_deg;\n\
\tcur_deg = deg+1;\n\
\treturn k;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(clean) "(" PREFIX_JET1(ptr) " x)\n\
{\n\
\t" I " k;\n\
\tif (*x == NULL) return;\n\
\tfor (k = 0; k < max_deg; k++) {" PREFIX_SCAL(clean) "((*x)[k]);}\n\
\tfree(*x); *x=NULL;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(cleanup) "(void)\n\
{\n\
\tif (flag_init_jet_library==0) return;\n\
\tif (max_deg == 0) return;\n\
\t\tcur_deg = 0;\n\
\t\t" PREFIX_JET1(clean) "(&my_jet_tmp);\n\
\t\t" PREFIX_SCAL(clean) "(my_scal_tmp);\n\
\tmax_deg = 0;\n\
\tflag_init_jet_library=0;\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set) "(" PREFIX_JET1(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_scal) "(" PREFIX_JET1(t) " b, " PREFIX_SCAL(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set) "(b[0],a);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_scal_array) "(" PREFIX_JET1(t) " b, " PREFIX_SCAL(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n	\
}\n" \
 "\n" \
  "void " PREFIX_JET1(set_jet_2_scal_array) "(" PREFIX_SCAL(t) " *b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n	\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_d) "(" PREFIX_JET1(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_d) "(b[0],a);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(set_si) "(" PREFIX_JET1(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_si) "(b[0],a);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(to_scal) "(" PREFIX_SCAL(t) " t[1], "PREFIX_JET1(t) " a) \
{" PREFIX_SCAL(set) "(*t,a[0]);}\n" \
  "\n" \
  "void " PREFIX_JET1(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_fabs) "(*nrm,a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(set_fabs) "(my_scal_tmp,a[k]);\n\
\t\tif (" PREFIX_SCAL(lt) "(*nrm,my_scal_tmp)) {" PREFIX_SCAL(set) "(*nrm,my_scal_tmp);}\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(nrm2) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(mul2) "(*nrm,a[0],a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k],a[k]);\n\
\t\t" PREFIX_SCAL(add2) "(*nrm,*nrm,my_scal_tmp);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(neg) "(" PREFIX_JET1(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(neg) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(add2) "(c[k],a[k],b[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(sub2) "(c[k],a[k],b[k]);}\n\
}\n" \
"\n" \
  "#define " PREFIX_JET1(set_sq) "(s,a)\\\n\
{\\\n\
\t" I " j,k, nk;\\\n\
\t" PREFIX_SCAL(mul2) "(s[0],a[0],a[0]);\\\n\
\tnk = cur_deg - (cur_deg%2==0);\\\n\
\tfor (k = 1; k < nk; k++) {\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_SCAL(mul2) "(s[k],a[0],a[k]);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[j],a[k-j]);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\\\n\
\t\t}\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(s[k],s[k],2);\\\n\
\t\tk++;\\\n\
\t\t/* k even */\\\n\
\t\t" PREFIX_SCAL(mul2) "(s[k],a[0],a[k]);\\\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) {\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[j],a[k-j]);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\\\n\
\t\t}\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(s[k],s[k],2);\\\n\
\t\tj = k/2;\\\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[j],a[j]);\\\n\
\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\\\n\
\t}\\\n\
\tif (cur_deg%2==0) {\\\n\
\t\tk=cur_deg-1;\\\n\
\t\t/* k odd */\\\n\
\t\t" PREFIX_SCAL(mul2) "(s[k],a[0],a[k]);\\\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[j],a[k-j]);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\\\n\
\t\t}\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(s[k],s[k],2);\\\n\
\t}\\\n\
}\n" \
"\n" \
  "void " PREFIX_JET1(mul2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t/* c_k = sum(a_{j}b_{k-j}, j=0,...,k) */\n\
\t" PREFIX_SCAL(mul2) "(c[0],a[0],b[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],a[0],b[k]);\n\
\t\tfor (j = 1; j <= k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[j],b[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\n\
\t\t}\n\
\t}\n\
}\n" \
  "void " PREFIX_JET1(div2) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(div2) "(c[0],a[0],b[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,c[j],b[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(sub2) "(c[k],a[k],c[k]);\n\
\t\t" PREFIX_SCAL(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2_scal) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(add2) "(c[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_scal) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2) "(c[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(scal_sub2) "(" PREFIX_JET1(t) " c, " PREFIX_SCAL(t) " b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2) "(c[0],b,a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_scal) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(mul2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(div2_scal) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(div2) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(scal_div2) "(" PREFIX_JET1(t) " c, " PREFIX_SCAL(t) " a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(div2) "(c[0],a,b[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,c[j],b[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_SCAL(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(add2_d) "(c[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_d) "(c[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(d_sub2) "(" PREFIX_JET1(t) " c, double b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_d) "(c[0],a[0],b);\n\
\t" PREFIX_SCAL(neg) "(c[0],c[9]);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(mul2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(div2_d) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, double b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(div2_d) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(d_div2) "(" PREFIX_JET1(t) " c, double a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_d) "(my_scal_tmp,a);\n\
\t" PREFIX_SCAL(div2) "(c[0],my_scal_tmp,b[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,c[j],b[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_SCAL(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(add2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(add2_si) "(c[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(sub2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_si) "(c[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(set) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(si_sub2) "(" PREFIX_JET1(t) " c, int b, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(sub2_si) "(c[0],a[0],b);\n\
\t" PREFIX_SCAL(neg) "(a[0],a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {" PREFIX_SCAL(neg) "(c[k],a[k]);}\n\
}\n" \
  "void " PREFIX_JET1(mul2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(mul2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(div2_si) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a, int b)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {" PREFIX_SCAL(div2_si) "(c[k],a[k],b);}\n\
}\n" \
  "void " PREFIX_JET1(si_div2) "(" PREFIX_JET1(t) " c, int a, " PREFIX_JET1(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_si) "(my_scal_tmp,a);\n\
\t" PREFIX_SCAL(div2) "(c[0],my_scal_tmp,b[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],c[0],b[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,c[j],b[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(neg) "(c[k],c[k]);\n\
\t\t" PREFIX_SCAL(div2) "(c[k],c[k],b[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_sqrt) "(" PREFIX_JET1(t) " s, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k, nk;\n\
\t" PREFIX_SCAL(set_sqrt) "(s[0],a[0]);\n\
\tnk = cur_deg - (cur_deg%2==0);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t/* k odd */\n\
\t\t" PREFIX_SCAL(set_zero) "(s[k]);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,s[j],s[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[k],2);\n\
\t\t" PREFIX_SCAL(sub2) "(s[k],a[k],my_scal_tmp);\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[0],2);\n\
\t\t" PREFIX_SCAL(div2) "(s[k],s[k],my_scal_tmp);\n\
\t\tk++;\n\
\t\t/* k even */\n\
\t\t" PREFIX_SCAL(set_zero) "(s[k]);\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,s[j],s[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[k],2);\n\
\t\t" PREFIX_SCAL(sub2) "(s[k],a[k],my_scal_tmp);\n\
\t\tj = k/2;\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,s[j],s[j]);\n\
\t\t" PREFIX_SCAL(sub2) "(s[k],s[k],my_scal_tmp);\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[0],2);\n\
\t\t" PREFIX_SCAL(div2) "(s[k],s[k],my_scal_tmp);\n\
\t}\n\
\tif (cur_deg%2==0) {\n\
\t\tk=cur_deg-1;\n\
\t\t/* k odd */\n\
\t\t" PREFIX_SCAL(set_zero) "(s[k]);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,s[j],s[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[k],2);\n\
\t\t" PREFIX_SCAL(sub2) "(s[k],a[k],my_scal_tmp);\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,s[0],2);\n\
\t\t" PREFIX_SCAL(div2) "(s[k],s[k],my_scal_tmp);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_pow_scal) "(" PREFIX_JET1(t) " p, " PREFIX_JET1(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_pow) "(p[0],a[0],b);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(p[k],a[k],p[0]);\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp, b, k);\n\
\t\t" PREFIX_SCAL(mul2) "(p[k],p[k],my_scal_tmp);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,b,k-j);\n\
\t\t\t" PREFIX_SCAL(sub2_si) "(my_scal_tmp,my_scal_tmp,j);\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,a[k-j]);\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,p[j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(p[k],p[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,a[0],k);\n\
\t\t" PREFIX_SCAL(div2) "(p[k],p[k],my_scal_tmp);\n\
\t}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_exp) "(" PREFIX_JET1(t) " e, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_exp) "(e[0],a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k],e[0]);\n\
\t\t" PREFIX_SCAL(mul2_si) "(e[k],my_scal_tmp,k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],e[j]);\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\n\
\t\t\t" PREFIX_SCAL(add2) "(e[k],e[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(div2_si) "(e[k],e[k],k);\n\
\t}\n\
}\n" \
  "void " PREFIX_JET1(set_log) "(" PREFIX_JET1(t) " l, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t/* Assump. l!=a */\n\
\t" PREFIX_SCAL(set_log) "(l[0],a[0]);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(set_zero) "(l[k]);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],l[j]);\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,j);\n\
\t\t\t" PREFIX_SCAL(add2) "(l[k],l[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(div2_si) "(l[k],l[k],k);\n\
\t\t" PREFIX_SCAL(sub2) "(l[k],a[k],l[k]);\n\
\t\t" PREFIX_SCAL(div2) "(l[k],l[k],a[0]);\n\
\t}\n\
}\n" \
  "\n" \
  "#define " PREFIX_JET1(sincos) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k;\\\n\
\t" PREFIX_SCAL(set_sin) "(s[0],a[0]);\\\n\
\t" PREFIX_SCAL(set_cos) "(c[0],a[0]);\\\n\
\tfor (k = 1; k < cur_deg; k++) {\\\n\
\t\t" PREFIX_SCAL(mul2) "(s[k],a[k],c[0]);\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],a[k],s[0]);\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(c[k],c[k],k);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],c[j]);\\\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\\\n\
\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],s[j]);\\\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\\\n\
\t\t}\\\n\
\t\t" PREFIX_SCAL(div2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_SCAL(div2_si) "(c[k],c[k],-k);\\\n\
\t}\\\n\
}\n" \
  "\n" \
  "void " PREFIX_JET1(set_cos) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sincos) "(my_jet_tmp,c,a);}\n" \
  "\n" \
  "void " PREFIX_JET1(set_sin) "(" PREFIX_JET1(t) " s, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sincos) "(s,my_jet_tmp,a);}\n" \
  "\n" \
  "void " PREFIX_JET1(set_tan) "(" PREFIX_JET1(t) " t, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k, nk;\n\
\t" PREFIX_SCAL(set_tan) "(t[0],a[0]);\n\
\tnk = cur_deg - (cur_deg%2==0);\n\
\t" PREFIX_SCAL(mul2) "(my_jet_tmp[0],t[0],t[0]);\n\
\tfor (k = 1; k < nk; k++) {\n\
\t\t" PREFIX_SCAL(mul2) "(t[k],a[k],my_jet_tmp[0]);\n\
\t\t" PREFIX_SCAL(mul2_si) "(t[k],t[k],k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],my_jet_tmp[j]);\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k],t[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_SCAL(add2) "(t[k],t[k],a[k]);\n\
\t\t/* k odd */\n\
\t\t" PREFIX_SCAL(mul2) "(my_jet_tmp[k],t[0],t[k]);\n\
\t\tfor (j = 1; j <= (k-1)/2; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,t[j],t[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(my_jet_tmp[k],my_jet_tmp[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_jet_tmp[k],my_jet_tmp[k],2);\n\
\t\tk++;\n\
\t\t" PREFIX_SCAL(mul2) "(t[k],a[k],my_jet_tmp[0]);\n\
\t\t" PREFIX_SCAL(mul2_si) "(t[k],t[k],k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],my_jet_tmp[j]);\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k],t[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_SCAL(add2) "(t[k],t[k],a[k]);\n\
\t\t/* k even */\n\
\t\t" PREFIX_SCAL(mul2) "(my_jet_tmp[k],t[0],t[k]);\n\
\t\tfor (j = 1; j <= (k-2)/2; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,t[j],t[k-j]);\n\
\t\t\t" PREFIX_SCAL(add2) "(my_jet_tmp[k],my_jet_tmp[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(mul2_si) "(my_jet_tmp[k],my_jet_tmp[k],2);\n\
\t\tj = k/2;\n\
\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,t[j],t[j]);\n\
\t\t" PREFIX_SCAL(add2) "(my_jet_tmp[k],my_jet_tmp[k],my_scal_tmp);\n\
\t}\n\
\tif (cur_deg%2==0) {\n\
\t\tk=cur_deg-1;\n\
\t\t" PREFIX_SCAL(mul2) "(t[k],a[k],my_jet_tmp[0]);\n\
\t\t" PREFIX_SCAL(mul2_si) "(t[k],t[k],k);\n\
\t\tfor (j = 1; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],my_jet_tmp[j]);\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k],t[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_SCAL(add2) "(t[k],t[k],a[k]);\n\
\t}\n\
}\n"\
"\n" \
  "\n" \
  "void " PREFIX_JET1(set_atan) "(" PREFIX_JET1(t) " t, " PREFIX_JET1(t) " a)\n\
{\n\
\t" I " j,k;\n\
\t" PREFIX_SCAL(set_atan) "(t[0],a[0]);\n\
\t" PREFIX_JET1(set_sq) "(my_jet_tmp,a);\n\
\t" PREFIX_JET1(add2_si) "(my_jet_tmp,my_jet_tmp,1);\n\
\tfor (k = 1; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(set_zero) "(t[k]);\n\
\t\tfor (j = 0; j < k; j++) {\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,t[j],my_jet_tmp[k-j]);\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,j);\n\
\t\t\t" PREFIX_SCAL(add2) "(t[k],t[k],my_scal_tmp);\n\
\t\t}\n\
\t\t" PREFIX_SCAL(div2_si) "(t[k],t[k],k);\n\
\t\t" PREFIX_SCAL(sub2) "(t[k],a[k],t[k]);\n\
\t\t" PREFIX_SCAL(div2) "(t[k],t[k],my_jet_tmp[0]);\n\
\t}\n\
}\n"\
"\n" \
  "#define " PREFIX_JET1(sinhcosh) "(s,c,a)\\\n\
{\\\n\
\t" I " j,k;\\\n\
\t" PREFIX_SCAL(set_sinh) "(s[0],a[0]);\\\n\
\t" PREFIX_SCAL(set_cosh) "(c[0],a[0]);\\\n\
\tfor (k = 1; k < cur_deg; k++) {\\\n\
\t\t" PREFIX_SCAL(mul2) "(s[k],a[k],c[0]);\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_SCAL(mul2) "(c[k],a[k],s[0]);\\\n\
\t\t" PREFIX_SCAL(mul2_si) "(c[k],c[k],k);\\\n\
\t\tfor (j = 1; j < k; j++) {\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],c[j]);\\\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(s[k],s[k],my_scal_tmp);\\\n\
\\\n\
\t\t\t" PREFIX_SCAL(mul2) "(my_scal_tmp,a[k-j],s[j]);\\\n\
\t\t\t" PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,k-j);\\\n\
\t\t\t" PREFIX_SCAL(add2) "(c[k],c[k],my_scal_tmp);\\\n\
\t\t}\\\n\
\t\t" PREFIX_SCAL(div2_si) "(s[k],s[k],k);\\\n\
\t\t" PREFIX_SCAL(div2_si) "(c[k],c[k],k);\\\n\
\t}\\\n\
}\n" \
"\n" \
"void " PREFIX_JET1(set_cosh) "(" PREFIX_JET1(t) " c, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sinhcosh) "(my_jet_tmp,c,a);}\n" \
"\n" \
"void " PREFIX_JET1(set_sinh) "(" PREFIX_JET1(t) " s, " PREFIX_JET1(t) " a)\n\
{" PREFIX_JET1(sinhcosh) "(s,my_jet_tmp,a);}\n" \
"\n" \
"void " PREFIX_JET1(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET1(t) " s)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < cur_deg; k++) {\n\
\t\t" PREFIX_SCAL(fprintf) "(file,fmt,s[k]);\n\
\t}\n\
}\n\
#undef my_jet_tmp\n\
#undef my_scal_tmp\n"\

#define MY_JET1_POSTCODE(PREFIX_JET1,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET1(t) " */\n" \

#endif /* MY_JET1_H */
