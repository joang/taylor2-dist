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

#define MY_JET_2_API(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_SCAL,SCAL,I) \
    MY_JET_APIS(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_2_PREHEADER(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET_2(t) " */\n" \

#define MY_JET_2_HEADER(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET_2_API(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_2_POSTHEADER(PREFIX_JET_2,MY_JET_2_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET_2(t) " */\n" \

/* *******  *************  ******* */
/* *******  MY_JET_2_CODE  ******* */
/* *******  *************  ******* */
#define MY_JET_2_PRECODE(PREFIX_JET_2,PREFIX_SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
/* CODE FOR " PREFIX_JET_2(t) " */\n\
int * " PREFIX_JET_2(monomial_counts) "(void) {return _monomial_counts_;}\n\
int * " PREFIX_JET_2(monomial_offsets) "(void) {return _monomial_offsets_;}\n\
\n"\

#define MY_JET_2_CODE(PREFIX_JET_2,PREFIX_SCAL,I) "\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n\
#ifndef _NUMBER_OF_JET_VARS_\n\
#define _NUMBER_OF_JET_VARS_ 0\n\
#endif\n\
#define my_scal_tmp " PREFIX_JET_2(tmp) "\n\
static int flag_init_jet_library=0;\n\
static " I " num_symbs=_NUMBER_OF_JET_VARS_;\n\
static " PREFIX_SCAL(t) " my_scal_tmp;\n\
#pragma omp threadprivate(my_scal_tmp)\n\
\n \
  void " PREFIX_JET_2(initup) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (flag_init_jet_library==1) return;\n\
\tif (deg != 2) {\n\
\t\tfprintf(stderr, \"%d: Not allowed deg value: \%d!=2\\n\",__LINE__,deg);\n\
\t\texit(1);\n\
\t}\n\
\t" PREFIX_SCAL(init) "(my_scal_tmp);\n\
\tflag_init_jet_library=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_2(get_num_symbs) "(void)\
{\
return num_symbs;\
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
\t\tfprintf(stderr, \"%d: Change number of symbols is not implemented.\\n\",__LINE__);\n\
\tif (nsymbs > num_symbs) {\n\
\t\tfprintf(stderr, \"%d: Invalid num. variables \%d > \%d\\n\",__LINE__,nsymbs, num_symbs);\n\
\t\texit(1);\n\
\t}\n\
\tk = num_symbs;\n\
\tnum_symbs = nsymbs+1;\n\
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
  "void " PREFIX_JET_2(cleanup) "(void)\n\
{\n\
\tif ( num_symbs == 0) return;\n\
\t" PREFIX_SCAL(clean) "(my_scal_tmp);\n\
\tnum_symbs = 0;\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET_2(set_scal_array) "(" PREFIX_JET_2(t) " b, " PREFIX_SCAL(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < _MAX_SIZE_OF_JET_VAR_; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_2(set_jet_2_scal_array) "(" PREFIX_SCAL(t) " *b, " PREFIX_JET_2(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < _MAX_SIZE_OF_JET_VAR_; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_2(set_d) "(" PREFIX_JET_2(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_d) "(b[0],a);\n\
\tfor (k = 1; k < _MAX_SIZE_OF_JET_VAR_; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_2(set_si) "(" PREFIX_JET_2(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_si) "(b[0],a);\n\
\tfor (k = 1; k < _MAX_SIZE_OF_JET_VAR_; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_2(to_scal) "(" PREFIX_SCAL(t) " t[1]," PREFIX_JET_2(t) " a) {" PREFIX_SCAL(set) "(*t,a[0]);}\n" \
  "\n \
\n\
\n\
size_t " PREFIX_JET_2(init) "(" PREFIX_JET_2(ptr) " t) {\n\
  int i;\n\
  " PREFIX_JET_2(t) " a=(" PREFIX_JET_2(t) ")malloc( _MAX_SIZE_OF_JET_VAR_ * sizeof(" PREFIX_SCAL(t) ") );\n\
  memset( (char *)a,0, _MAX_SIZE_OF_JET_VAR_ * sizeof(" PREFIX_SCAL(t) "));\n\
  *t = a;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){  \n\
    " PREFIX_SCAL(init) "(a[i]);\n\
    " PREFIX_SCAL(set_d) "(a[i],0.0);\n\
  }\n\
  return 0;\n\
}\n\
\n\
void " PREFIX_JET_2(clean) "(" PREFIX_JET_2(t) " *a) {\n\
  (void) free(*a);\n\
}\n\
\n\
void " PREFIX_JET_2(neg) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " s) \n\
{\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(neg) "(t[i], s[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_2(set_jet_2_scal) "(" PREFIX_SCAL(t) " *t, " PREFIX_JET_2(t) " f) {\n\
  " PREFIX_SCAL(set) "(*t, f[0]);\n\
}\n\
\n\
void " PREFIX_JET_2(set_scal) "(" PREFIX_JET_2(t) " t, " PREFIX_SCAL(t) " f) {\n\
  int i;\n\
  for(i=1; i<_MAX_SIZE_OF_JET_VAR_; i++) {\n\
    " PREFIX_SCAL(set_d) "(t[i],0.0);\n\
  }\n\
  " PREFIX_SCAL(set) "(t[0],f);\n\
}\n\
\n\
\n\
void " PREFIX_JET_2(set) "(" PREFIX_JET_2(t) " t, " PREFIX_JET_2(t) " s) {\n\
  int i;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(set) "(t[i],s[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_2(add2) "(" PREFIX_JET_2(t) " t, " PREFIX_JET_2(t) " a, " PREFIX_JET_2(t) " b) {\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(add2) "(t[i],a[i],b[i]);\n\
  }\n\
}\n\
\n\
void add_float_jet_a_test(" PREFIX_JET_2(t) " t, " PREFIX_SCAL(t) " a, " PREFIX_JET_2(t) " b) {\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(set) "(t[i],b[i]);\n\
  }\n\
  " PREFIX_SCAL(add2) "(t[0],a,b[0]);\n\
}\n\
\n\
void " PREFIX_JET_2(add2_scal) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(set) "(t[i],a[i]);\n\
  }\n\
  " PREFIX_SCAL(add2) "(t[0],a[0],b);\n\
}\n\
\n\
\n\
void " PREFIX_JET_2(add2_d) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  int i;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(set) "(t[i],a[i]);\n\
  }\n\
  AddMyFloatD(t[0],a[0],b);\n\
}\n\
\n\
void " PREFIX_JET_2(add2_si) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " PREFIX_JET_2(add2_d) "(t,a, (double)b);\n\
}\n\
\n\
void " PREFIX_JET_2(sub2) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a," PREFIX_JET_2(t) " b) {\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(sub2) "(t[i],a[i],b[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_2(scal_sub2) "(" PREFIX_JET_2(t) " t, " PREFIX_SCAL(t) " a," PREFIX_JET_2(t) " b) {\n\
  int i; \n\
  for(i=1;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(neg) "(t[i],b[i]);\n\
  }\n\
  " PREFIX_SCAL(sub2) "(t[0],a,b[0]);\n\
}\n\
\n\
\n\
void " PREFIX_JET_2(d_sub2) "(" PREFIX_JET_2(t) " t, double a," PREFIX_JET_2(t) " b) {\n\
  int i; \n\
  for(i=1;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(neg) "(t[i],b[i]);\n\
  }\n\
  SubtractMyFloatD(t[0],b[0],a);\n\
  " PREFIX_SCAL(neg) "(t[0],t[0]);  \n\
}\n\
void " PREFIX_JET_2(si_sub2) "(" PREFIX_JET_2(t) " t, int a," PREFIX_JET_2(t) " b) {\n\
  " PREFIX_JET_2(d_sub2) "(t, (double)a, b);\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_scal) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i; \n\
  for(i=1;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(set) "(t[i], a[i]);\n\
  }\n\
  " PREFIX_SCAL(sub2) "(t[0],a[0],b);\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_d) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  int i; \n\
  for(i=1;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(set) "(t[i], a[i]);\n\
  }\n\
  SubtractMyFloatD(t[0],a[0],b);\n\
}\n\
\n\
void " PREFIX_JET_2(sub2_si) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " PREFIX_JET_2(sub2_si) "(t,a, (double)b);\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_scal) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(mul2) "(t[i],(a[i]), b);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_d) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    MultiplyMyFloatD(t[i],(a[i]), b);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_2(mul2_si) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, int b) {\n\
  " PREFIX_JET_2(mul2_d) "(t,a,(double)b);\n\
}\n\
\n\
void " PREFIX_JET_2(scal_mul2) "(" PREFIX_JET_2(t) " t, " PREFIX_SCAL(t) " a, " PREFIX_JET_2(t) " b) {\n\
  int i; \n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(mul2) "(t[i],(b[i]),a);\n\
  }\n\
}\n\
\n\
\n\
void " PREFIX_JET_2(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_2(t) " a)\n\
{\n\
  int  k;\n\
  fabsMyFloatA(*nrm,a[0]);\n\
  for (k = 1; k < _MAX_SIZE_OF_JET_VAR_; k++) {\n\
    fabsMyFloatA(my_scal_tmp,a[k]);\n\
    if(" PREFIX_SCAL(lt) "(*nrm,my_scal_tmp))\n\
      " PREFIX_SCAL(set) "(*nrm,my_scal_tmp);\n\
  }\n\
}\n\
\n\
void euclid_nrm(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_2(t) " a)\n\
{\n\
	int k;\n\
	" PREFIX_SCAL(mul2) "(*nrm,a[0],a[0]);\n\
	for (k = 1; k < _MAX_SIZE_OF_JET_VAR_; k++) {\n\
	  " PREFIX_SCAL(mul2) "(my_scal_tmp,a[k],a[k]);\n\
	  " PREFIX_SCAL(add2) "(*nrm,*nrm,my_scal_tmp);\n\
	}\n\
}\n\
\n\
/* t=a*b */\n\
void " PREFIX_JET_2(mul2) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_SCAL(t) " temp1, temp2, temp3, temp4, a0, b0;\n\
  static int inited = 0;\n\
#pragma omp threadprivate(temp1, temp2, temp3, temp4, a0, b0, inited)\n\
  int i,j,k,l,n1,n2,s1,s2,nv; \n\
\n\
  if(inited == 0) {\n\
    " PREFIX_SCAL(init) "(temp1); " PREFIX_SCAL(init) "(temp2);\n\
    " PREFIX_SCAL(init) "(temp3); " PREFIX_SCAL(init) "(temp4);\n\
    " PREFIX_SCAL(init) "(a0); " PREFIX_SCAL(init) "(b0);\n\
    inited = 1;\n\
  }\n\
  /* scaler */\n\
  " PREFIX_SCAL(mul2) "(t[0],a[0],b[0]); \n\
  /* degree 1 */\n\
  s1 = _monomial_offsets_[1];\n\
  n1 = _monomial_counts_[1];\n\
  for(i=1;i<=n1;i++){\n\
    " PREFIX_SCAL(mul2) "(temp1,(b[i]), a[0]);\n\
    " PREFIX_SCAL(mul2) "(temp2,(a[i]), b[0]);\n\
    " PREFIX_SCAL(add2) "(t[i],temp1,temp2);\n\
  }\n\
  if(_MAX_DEGREE_OF_JET_VARS_ > 1) {\n\
    /* quadratic */\n\
    s2 = _monomial_offsets_[2];\n\
    nv = _NUMBER_OF_MAX_SYMBOLS_;\n\
\n\
\n\
    " PREFIX_SCAL(set) "(a0,a[0]);\n\
    " PREFIX_SCAL(set) "(b0,b[0]);\n\
    for(k=0, i=1; i<= nv; i++) {\n\
      for(j=i; j<=nv; j++) {    \n\
	" PREFIX_SCAL(mul2) "(temp1, a0 ,  b[s2+k]);\n\
	" PREFIX_SCAL(mul2) "(temp2, b0 ,  a[s2+k]);\n\
	" PREFIX_SCAL(mul2) "(temp3, a[i],b[j]);\n\
	" PREFIX_SCAL(add2) "(temp4,temp1,temp2);\n\
\n\
	if(i != j) {\n\
	  " PREFIX_SCAL(mul2) "(temp1, a[j],  b[i]);\n\
	  " PREFIX_SCAL(add2) "(temp2,temp3,temp4);	  \n\
	  " PREFIX_SCAL(add2) "(t[s2+k],temp1,temp2);	  	  \n\
	} else {\n\
	  " PREFIX_SCAL(add2) "(t[s2+k],temp3,temp4);	  \n\
	}\n\
	k++;\n\
      }\n\
    }\n\
  }\n\
}\n\
\n\
/* t=a/b, b float   */\n\
void " PREFIX_JET_2(div2_scal) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    " PREFIX_SCAL(div2) "(t[i],(a[i]),b);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_2(div2_d)  "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a, double b) {\n\
  int i;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){\n\
    DivideMyFloatD(t[i],(a[i]),b);\n\
  }\n\
}\n\
\n\
\n\
/* t= a/b, a float, b jet */\n\
void " PREFIX_JET_2(scal_div2)  "(" PREFIX_JET_2(t) " t, " PREFIX_SCAL(t) " a, " PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, temp;\n\
#pragma omp threadprivate(tmp1,tmp2,tmp3,initialized, b0, temp)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(temp);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    " PREFIX_JET_2(init) "(&tmp3);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0, b[0]);\n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_JET_2(div2_scal) "(tmp2, tmp1, b0);\n\
  " PREFIX_SCAL(set_d) "(tmp2[0],0.0);\n\
  " PREFIX_JET_2(mul2) "(tmp1,tmp2,tmp2);\n\
  " PREFIX_JET_2(sub2) "(tmp3,tmp1,tmp2);\n\
  " PREFIX_SCAL(set_d) "(tmp3[0], 1.0);\n\
  " PREFIX_SCAL(div2) "(temp,a,b0);\n\
  " PREFIX_JET_2(mul2_scal) "(t, tmp3, temp);\n\
}\n\
\n\
void " PREFIX_JET_2(d_div2)  "(" PREFIX_JET_2(t) " t, double a, " PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " a0, b0, temp;\n\
#pragma omp threadprivate(tmp1,tmp2,tmp3,initialized, a0, b0, temp)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(b0);    \n\
    " PREFIX_SCAL(init) "(temp);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    " PREFIX_JET_2(init) "(&tmp3);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set_d) "(a0, a);\n\
  " PREFIX_SCAL(set) "(b0, b[0]);\n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_JET_2(div2_scal) "(tmp2, tmp1, b0);\n\
  " PREFIX_SCAL(set_d) "(tmp2[0],0.0);\n\
  " PREFIX_JET_2(mul2) "(tmp1,tmp2,tmp2);\n\
  " PREFIX_JET_2(sub2) "(tmp3,tmp1,tmp2);\n\
  " PREFIX_SCAL(set_d) "(tmp3[0], 1.0);\n\
  " PREFIX_SCAL(div2) "(temp,a0,b0);\n\
  " PREFIX_JET_2(mul2_scal) "(t, tmp3, temp);\n\
}\n\
\n\
void " PREFIX_JET_2(si_div2)  "(" PREFIX_JET_2(t) " t, int a, " PREFIX_JET_2(t) " b) {\n\
  " PREFIX_JET_2(si_div2)  "(t, (double) a, b);  \n\
}\n\
\n\
\n\
\n\
/* t = a/b,  a,b jets */\n\
void " PREFIX_JET_2(div2) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a," PREFIX_JET_2(t) " b)  {\n\
  static " PREFIX_JET_2(t) " tmp1;\n\
  static " PREFIX_SCAL(t) " one;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate(tmp1,one, initailzed)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set_d) "(one, 1.0);\n\
  " PREFIX_JET_2(scal_div2)  "(tmp1, one, b);\n\
  " PREFIX_JET_2(mul2) "(t,a,tmp1);\n\
}\n\
\n\
/* t=b^e, e float */\n\
void " PREFIX_JET_2(set_pow_scal) "(" PREFIX_JET_2(t) " t, " PREFIX_JET_2(t) " b, " PREFIX_SCAL(t) " e) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, s1, smp1,smp2,smp3,smp4,one;\n\
#pragma omp threadprivate(tmp1, tmp2, tmp3,b0, s1, smp1,smp2,smp3,smp4,one, initialized)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(s1);  \n\
    " PREFIX_SCAL(init) "(smp1);" PREFIX_SCAL(init) "(smp2);\n\
    " PREFIX_SCAL(init) "(smp3);" PREFIX_SCAL(init) "(smp4);\n\
    " PREFIX_SCAL(init) "(one);    \n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    " PREFIX_JET_2(init) "(&tmp3);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0, b[0]);\n\
  " PREFIX_SCAL(set_pow) "(s1,b0,e);\n\
\n\
  " PREFIX_SCAL(mul2) "(smp4,s1,e);\n\
\n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_JET_2(div2_scal) "(tmp2, tmp1, b0);\n\
  " PREFIX_SCAL(set_d) "(tmp2[0],0.0);\n\
  " PREFIX_JET_2(mul2) "(tmp3,tmp2,tmp2);  \n\
  " PREFIX_SCAL(set_d) "(one, 1.0);\n\
  " PREFIX_SCAL(set_d) "(smp1, 0.5);\n\
  " PREFIX_SCAL(sub2) "(smp2,e,one);\n\
  " PREFIX_SCAL(mul2) "(smp3,smp1,smp2);\n\
  " PREFIX_SCAL(mul2) "(smp2,smp3,smp4);\n\
  " PREFIX_JET_2(mul2_scal) "(tmp1, tmp3, smp2);\n\
  " PREFIX_JET_2(mul2_scal) "(tmp3, tmp2, smp4);  \n\
  " PREFIX_JET_2(add2) "(t,tmp1,tmp3);\n\
  " PREFIX_SCAL(set) "(t[0],s1);\n\
}\n\
\n\
/* t=sqrt(a) */\n\
void " PREFIX_JET_2(set_sqrt) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " a) {\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " half;\n\
#pragma omp threadprivate(half, initialized)  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(half);\n\
  }\n\
  " PREFIX_SCAL(set_d) "(half, 0.5);\n\
  " PREFIX_JET_2(set_pow_scal) "(t, a, half);\n\
}\n\
\n\
/* t=sin(b), b = b0 + X */\n\
// t = sin(b0) + cos(b0) X -1/2 sin(b0) X^2  \n\
void " PREFIX_JET_2(set_sin) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, _c, _s, half,smp;\n\
#pragma omp threadprivate(tmp1, tmp2, tmp3, b0, _c, _s, half,smp,initialized)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(_s);\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_SCAL(init) "(smp);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    " PREFIX_JET_2(init) "(&tmp3);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(set_cos) "(_c,b0);\n\
  " PREFIX_SCAL(set_sin) "(_s,b0);\n\
\n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_SCAL(set_d) "(tmp1[0],0.0);\n\
  // tmp1 = X\n\
  " PREFIX_JET_2(mul2) "(tmp2,tmp1,tmp1);    \n\
  " PREFIX_SCAL(set_d) "(half, -0.5);\n\
  " PREFIX_SCAL(mul2) "(smp, half, _s);\n\
  " PREFIX_JET_2(mul2_scal) "(tmp3,tmp2,smp);\n\
  // tmp3 = 1/2 sin(b0)X^2\n\
  " PREFIX_JET_2(mul2_scal) "(tmp2,tmp1,_c);\n\
  // tmp2 = cos(b0)X\n\
  " PREFIX_JET_2(add2) "(t,tmp3,tmp2);  \n\
  " PREFIX_SCAL(set) "(t[0], _s);\n\
}\n\
\n\
/* t=cos(b) b=b0+X*/\n\
// t= cos(b0) - sin(b0) X -1/2 cos(b0) X^2\n\
\n\
void " PREFIX_JET_2(set_cos) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, _c, _s, half,smp;\n\
#pragma omp threadprivate(tmp1, tmp2, tmp3, b0, _c, _s, half,smp,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(_s);\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_SCAL(init) "(smp);\n\
    " PREFIX_JET_2(init) "(&tmp1);\n\
    " PREFIX_JET_2(init) "(&tmp2);\n\
    " PREFIX_JET_2(init) "(&tmp3);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(set_cos) "(_c,b0);\n\
  " PREFIX_SCAL(set_sin) "(_s,b0);\n\
  \n\
  " PREFIX_JET_2(set) "(tmp1, b);\n\
  " PREFIX_SCAL(set_d) "(tmp1[0],0.0);\n\
  // X = tmp1\n\
  " PREFIX_JET_2(mul2) "(tmp2,tmp1,tmp1);    \n\
  " PREFIX_SCAL(set_d) "(half, -0.5);\n\
  " PREFIX_SCAL(mul2) "(smp, half, _c);\n\
  " PREFIX_JET_2(mul2_scal) "(tmp3,tmp2,smp);\n\
  //  tmp3 = - 1/2 cos(b0) X^2\n\
  " PREFIX_SCAL(neg) "(smp, _s);\n\
  " PREFIX_JET_2(mul2_scal) "(tmp2,tmp1,smp);\n\
  // tmp2 = - sin(b0) X\n\
  " PREFIX_JET_2(add2) "(t,tmp3,tmp2);  \n\
  " PREFIX_SCAL(set) "(t[0], _c);\n\
}\n\
\n\
/* t=exp(b)*/\n\
void " PREFIX_JET_2(set_exp) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, _c,  half, one;\n\
#pragma omp threadprivate(t0, t1, t2, b0, _c, one, half,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_SCAL(init) "(one);    \n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set_d) "(half, 0.5);\n\
  " PREFIX_SCAL(set_d) "(one, 1.0);    \n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  expMyFloatA(_c,b0);\n\
  " PREFIX_JET_2(sub2_scal) "(t0, b, b0);\n\
  " PREFIX_JET_2(mul2) "(t1,t0,t0);\n\
  " PREFIX_JET_2(mul2_scal) "(t2,t1,half);\n\
  " PREFIX_JET_2(add2) "(t1,t0,t2);\n\
  add_float_jet_a_test(t2, one, t1);  \n\
  " PREFIX_JET_2(mul2_scal) "(t,t2,_c);\n\
  " PREFIX_SCAL(set) "(t[0], _c);\n\
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
  static " PREFIX_SCAL(t) " b0, lna,  half;\n\
#pragma omp threadprivate(t0, t1, t2, b0, lna, half,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(lna);\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set_d) "(half, 0.5);\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  logMyFloatA(lna,b0);\n\
  \n\
  " PREFIX_JET_2(div2_scal) "(t0, b, b0);\n\
  " PREFIX_SCAL(set_d) "(t0[0],0.0);\n\
  " PREFIX_JET_2(mul2) "(t1,t0,t0);\n\
  " PREFIX_JET_2(scal_mul2) "(t2, half, t1);\n\
  " PREFIX_JET_2(sub2) "(t,t0,t2);\n\
  " PREFIX_SCAL(set) "(t[0], lna);\n\
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
  static " PREFIX_SCAL(t) " b0, b02, b02p1, one ;\n\
#pragma omp threadprivate(t0, t1, t2, b0, one, b02, b02p1, initialized)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(b02);\n\
    " PREFIX_SCAL(init) "(b02p1);        \n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    " PREFIX_SCAL(set_d) "(one, 1.0);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(mul2) "(b02,b0, b0);\n\
  " PREFIX_SCAL(add2) "(b02p1, b02,  one);\n\
  " PREFIX_JET_2(sub2_scal) "(t0, b, b0);\n\
  " PREFIX_JET_2(scal_mul2) "(t1, b0, t0);\n\
  " PREFIX_JET_2(add2_scal) "(t2,t1,b02p1);\n\
  " PREFIX_JET_2(div2) "(t1, t0, t2);\n\
  // b0+t1/(1-b0 t1) = b0+t0 = b\n\
  atanMyFloatA(b02, b0);\n\
  " PREFIX_JET_2(add2_scal) "(t,t1,b02);  \n\
}\n\
  \n\
/* t=sinh(b) */\n\
void " PREFIX_JET_2(set_sinh) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " half;\n\
#pragma omp threadprivate(t0, t1, t2, half,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    " PREFIX_SCAL(set_d) "(half, 0.5);    \n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_2(neg) "(t0, b);\n\
  " PREFIX_JET_2(set_exp) "(t1,b);\n\
  " PREFIX_JET_2(set_exp) "(t2,t0);\n\
  " PREFIX_JET_2(sub2) "(t0,t1,t2);\n\
  " PREFIX_JET_2(scal_mul2) "(t, half, t0);\n\
}\n\
\n\
/* t=cosh(b) */\n\
void " PREFIX_JET_2(set_cosh) "(" PREFIX_JET_2(t) " t," PREFIX_JET_2(t) " b) {\n\
  static " PREFIX_JET_2(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " half;\n\
#pragma omp threadprivate(t0, t1, t2, half,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_JET_2(init) "(&t0);    \n\
    " PREFIX_JET_2(init) "(&t1);\n\
    " PREFIX_JET_2(init) "(&t2);\n\
    " PREFIX_SCAL(set_d) "(half, 0.5);    \n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_2(neg) "(t0, b);\n\
  " PREFIX_JET_2(set_exp) "(t1,b);\n\
  " PREFIX_JET_2(set_exp) "(t2,t0);\n\
  " PREFIX_JET_2(add2) "(t0,t1,t2);\n\
  " PREFIX_JET_2(scal_mul2) "(t, half, t0);\n\
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
\n\
\n\
\n\
\n\
void " PREFIX_JET_2(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET_2(t) " a)\n\
{\n\
  int k;\n\
  for (k = 0; k < _MAX_SIZE_OF_JET_VAR_; k++) {\n\
    OutputMyFloat3(file, fmt, a[k]);\n\
  }\n\
}\n\
" \

#define MY_JET_2_POSTCODE(PREFIX_JET_2,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET_2(t) " */\n" \

#endif /* MY_JET_2_H */
