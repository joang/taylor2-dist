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

#ifndef MY_JET_m_H
#define MY_JET_m_H

/* *******  ***************  ******* */
/* *******  MY_JET_m_HEADER  ******* */
/* *******  ***************  ******* */
#define MY_JET_m_FIXES MY_JET_SPACE
#define MY_JET_m_TYPE MY_JET_TYPE1

#define MY_JET_m_API(PREFIX_JET_m,MY_JET_m_TYPE,PREFIX_SCAL,SCAL,I) \
    MY_JET_APIS(PREFIX_JET_m,MY_JET_m_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_m_PREHEADER(PREFIX_JET_m,MY_JET_m_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET_m(t) " */\n"\

#define MY_JET_m_HEADER(PREFIX_JET_m,MY_JET_m_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET_m_API(PREFIX_JET_m,MY_JET_m_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_m_POSTHEADER(PREFIX_JET_m,MY_JET_m_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET_m(t) " */\n" \

/* *******  *************  ******* */
/* *******  MY_JET_m_CODE  ******* */
/* *******  *************  ******* */
#define MY_JET_m_PRECODE(PREFIX_JET_m,PREFIX_SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
/* CODE FOR " PREFIX_JET_m(t) " */\n\
int * " PREFIX_JET_m(monomial_counts) "(void) {return _monomial_counts_;}\n\
int * " PREFIX_JET_m(monomial_offsets) "(void) {return _monomial_offsets_;}\n\
\n"\

#define MY_JET_m_CODE(PREFIX_JET_m,PREFIX_SCAL,I) "\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n\
#ifndef _NUMBER_OF_JET_VARS_\n\
#define _NUMBER_OF_JET_VARS_ 0\n\
#endif\n\
#define my_scal_tmp " PREFIX_JET_m(tmp) "\n\
static int flag_init_jet_library=0;\n\
static " I " num_symbs=_NUMBER_OF_JET_VARS_;\n\
static " I " nm_working_degree=_MAX_DEGREE_OF_JET_VARS_;\n\
static " I " nm_working_jetvar_size=_MAX_SIZE_OF_JET_VAR_;\n\
static " PREFIX_SCAL(t) " my_scal_tmp;\n\
#pragma omp threadprivate(my_scal_tmp)\n\
\n \
  void " PREFIX_JET_m(initup) "(" I " nsymbs, " I " deg)\n\
{\n\
\tif (flag_init_jet_library==1) return;\n\
\t" PREFIX_SCAL(init) "(my_scal_tmp);\n\
\tflag_init_jet_library=1;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_m(get_num_symbs) "(void)\
{\
return num_symbs;\
}\n" \
  "\n" \
  I " " PREFIX_JET_m(get_deg) "(void)\
{\
return nm_working_degree;\
}\n" \
  "\n" \
  I " " PREFIX_JET_m(set_num_symbs) "(" I " nsymbs)\n\
{\n\
\t\tfprintf(stderr, \"%d: Change number of symbols is Not allowed.\\n\",__LINE__);\n\
\treturn num_symbs;\n\
}\n" \
  "\n" \
  I " " PREFIX_JET_m(set_deg) "(" I " deg)\n\
{\n\
\t" I " tdg;\n\
\tif(deg > _MAX_DEGREE_OF_JET_VARS_ || deg <= 0) {\n\
\t\tfprintf(stderr, \"%d: Invalid degree %d. Must be between 1 and %d.\\n\",__LINE__,deg, _MAX_DEGREE_OF_JET_VARS_);\n\
\t\texit(1);\n\
\t}\n\
\ttdg=nm_working_degree;\n\
\tnm_working_degree=deg;\n\
\tnm_working_jetvar_size= _monomial_offsets_[deg+1];\n\
\t//_size_of_jet_variable_=nm_working_jetvar_size;\n\
\treturn tdg;\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET_m(cleanup) "(void)\n\
{\n\
\tif (flag_init_jet_library==0) return;\n\
\t" PREFIX_SCAL(clean) "(my_scal_tmp);\n\
\tnum_symbs = 0;\n\
\tflag_init_jet_library=0;\n\
}\n" \
  "\n" \
  "\n" \
  "void " PREFIX_JET_m(set_scal_array) "(" PREFIX_JET_m(t) " b, " PREFIX_SCAL(t) " *a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < nm_working_jetvar_size; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_m(set_jet_2_scal_array) "(" PREFIX_SCAL(t) " *b, " PREFIX_JET_m(t) " a)\n\
{\n\
\t" I " k;\n\
\tfor (k = 0; k < nm_working_jetvar_size; k++) {" PREFIX_SCAL(set) "(b[k],a[k]);}\n\
}\n" \
  "\n" \
  "void " PREFIX_JET_m(set_d) "(" PREFIX_JET_m(t) " b, double a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_d) "(b[0],a);\n\
\tfor (k = 1; k < nm_working_jetvar_size; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_m(set_si) "(" PREFIX_JET_m(t) " b, int a)\n\
{\n\
\t" I " k;\n\
\t" PREFIX_SCAL(set_si) "(b[0],a);\n\
\tfor (k = 1; k < nm_working_jetvar_size; k++) {" PREFIX_SCAL(set_zero) "(b[k]);}\n\
}\n" \
  "void " PREFIX_JET_m(to_scal) "(" PREFIX_SCAL(t) " t[1]," PREFIX_JET_m(t) " a) {" PREFIX_SCAL(set) "(*t,a[0]);}\n" \
  "\n \
\n\
void " PREFIX_JET_m(nm_mul_jet) "( " PREFIX_JET_m(t) " c, " PREFIX_JET_m(t) " a, " PREFIX_JET_m(t) " b);\n\
void " PREFIX_JET_m(nm_jet_polynomial_eval) "(" PREFIX_JET_m(t) " t, " PREFIX_SCAL(t) " coefs[], " PREFIX_JET_m(t) " a );\n\
\n\
size_t " PREFIX_JET_m(init) "(" PREFIX_JET_m(ptr) " t) {\n\
  int i;\n\
  " PREFIX_JET_m(t) " a=(" PREFIX_JET_m(t) ")malloc( _MAX_SIZE_OF_JET_VAR_ * sizeof(" PREFIX_SCAL(t) ") );\n\
  memset( (char *)a,0, _MAX_SIZE_OF_JET_VAR_ * sizeof(" PREFIX_SCAL(t) "));\n\
  *t = a;\n\
  for(i=0;i<_MAX_SIZE_OF_JET_VAR_;i++){  \n\
    " PREFIX_SCAL(init) "(a[i]);\n\
    " PREFIX_SCAL(set_d) "(a[i],0.0);\n\
  }\n\
  return 0;\n\
}\n\
\n\
void " PREFIX_JET_m(clean) "(" PREFIX_JET_m(t) " *a) {\n\
  (void) free(*a);\n\
}\n\
\n\
void " PREFIX_JET_m(neg) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " s) \n\
{\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(neg) "(t[i], s[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(set_jet_2_scal) "(" PREFIX_SCAL(t) " *t, " PREFIX_JET_m(t) " f) {\n\
  " PREFIX_SCAL(set) "(*t, f[0]);\n\
}\n\
\n\
void " PREFIX_JET_m(set_scal) "(" PREFIX_JET_m(t) " t, " PREFIX_SCAL(t) " f) {\n\
  int i;\n\
  for(i=1; i<nm_working_jetvar_size; i++) {\n\
    " PREFIX_SCAL(set_d) "(t[i],0.0);\n\
  }\n\
  " PREFIX_SCAL(set) "(t[0],f);\n\
}\n\
\n\
\n\
void " PREFIX_JET_m(set) "(" PREFIX_JET_m(t) " t, " PREFIX_JET_m(t) " s) {\n\
  int i;\n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(set) "(t[i],s[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(add2) "(" PREFIX_JET_m(t) " t, " PREFIX_JET_m(t) " a, " PREFIX_JET_m(t) " b) {\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(add2) "(t[i],a[i],b[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(add_scal_jet) "(" PREFIX_JET_m(t) " t, " PREFIX_SCAL(t) " a, " PREFIX_JET_m(t) " b) {\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(set) "(t[i],b[i]);\n\
  }\n\
  " PREFIX_SCAL(add2) "(t[0],a,b[0]);\n\
}\n\
\n\
void " PREFIX_JET_m(add2_scal) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i;\n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(set) "(t[i],a[i]);\n\
  }\n\
  " PREFIX_SCAL(add2) "(t[0],a[0],b);\n\
}\n\
\n\
\n\
void " PREFIX_JET_m(add2_d) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, double b) {\n\
  int i;\n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(set) "(t[i],a[i]);\n\
  }\n\
  AddMyFloatD(t[0],a[0],b);\n\
}\n\
\n\
void " PREFIX_JET_m(add2_si) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, int b) {\n\
  " PREFIX_JET_m(add2_d) "(t,a, (double)b);\n\
}\n\
\n\
void " PREFIX_JET_m(sub2) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a," PREFIX_JET_m(t) " b) {\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(sub2) "(t[i],a[i],b[i]);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(scal_sub2) "(" PREFIX_JET_m(t) " t, " PREFIX_SCAL(t) " a," PREFIX_JET_m(t) " b) {\n\
  int i; \n\
  for(i=1;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(neg) "(t[i],b[i]);\n\
  }\n\
  " PREFIX_SCAL(sub2) "(t[0],a,b[0]);\n\
}\n\
\n\
\n\
void " PREFIX_JET_m(d_sub2) "(" PREFIX_JET_m(t) " t, double a," PREFIX_JET_m(t) " b) {\n\
  int i; \n\
  for(i=1;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(neg) "(t[i],b[i]);\n\
  }\n\
  SubtractMyFloatD(t[0], b[0], a);\n\
  " PREFIX_SCAL(neg) "(t[0],t[0]);  \n\
}\n\
void " PREFIX_JET_m(si_sub2) "(" PREFIX_JET_m(t) " t, int a," PREFIX_JET_m(t) " b) {\n\
  " PREFIX_JET_m(d_sub2) "(t, (double)a, b);\n\
}\n\
\n\
void " PREFIX_JET_m(sub2_scal) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i; \n\
  for(i=1;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(set) "(t[i], a[i]);\n\
  }\n\
  " PREFIX_SCAL(sub2) "(t[0],a[0],b);\n\
}\n\
\n\
void " PREFIX_JET_m(sub2_d) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, double b) {\n\
  int i; \n\
  for(i=1;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(set) "(t[i], a[i]);\n\
  }\n\
  SubtractMyFloatD(t[0],a[0],b);\n\
}\n\
\n\
void " PREFIX_JET_m(sub2_si) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, int b) {\n\
  " PREFIX_JET_m(sub2_si) "(t,a, (double)b);\n\
}\n\
\n\
void " PREFIX_JET_m(mul2_scal) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(mul2) "(t[i],(a[i]), b);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(mul2_d) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, double b) {\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    MultiplyMyFloatD(t[i],(a[i]), b);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(mul2_si) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, int b) {\n\
  " PREFIX_JET_m(mul2_d) "(t,a,(double)b);\n\
}\n\
\n\
void " PREFIX_JET_m(scal_mul2) "(" PREFIX_JET_m(t) " t, " PREFIX_SCAL(t) " a, " PREFIX_JET_m(t) " b) {\n\
  int i; \n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(mul2) "(t[i],(b[i]),a);\n\
  }\n\
}\n\
\n\
\n\
void " PREFIX_JET_m(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_m(t) " a)\n\
{\n\
  int  k;\n\
  fabsMyFloatA(*nrm,a[0]);\n\
  for (k = 1; k < nm_working_jetvar_size; k++) {\n\
    fabsMyFloatA(my_scal_tmp,a[k]);\n\
    if(" PREFIX_SCAL(lt) "(*nrm,my_scal_tmp))\n\
      " PREFIX_SCAL(set) "(*nrm,my_scal_tmp);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(nrm2) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_m(t) " a)\n\
{\n\
	int k;\n\
	" PREFIX_SCAL(mul2) "(*nrm,a[0],a[0]);\n\
	for (k = 1; k < nm_working_jetvar_size; k++) {\n\
	  " PREFIX_SCAL(mul2) "(my_scal_tmp,a[k],a[k]);\n\
	  " PREFIX_SCAL(add2) "(*nrm,*nrm,my_scal_tmp);\n\
	}\n\
}\n\
\n\
/* t=a*b */\n\
\n\
void " PREFIX_JET_m(mul2) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a," PREFIX_JET_m(t) " b) {\n\
  " PREFIX_JET_m(nm_mul_jet) "(t, a,  b);\n\
}\n\
\n\
/* t=a/b, b float   */\n\
void " PREFIX_JET_m(div2_scal) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, " PREFIX_SCAL(t) " b) {\n\
  int i;\n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    " PREFIX_SCAL(div2) "(t[i],(a[i]),b);\n\
  }\n\
}\n\
\n\
void " PREFIX_JET_m(div2_d)  "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a, double b) {\n\
  int i;\n\
  for(i=0;i<nm_working_jetvar_size;i++){\n\
    DivideMyFloatD(t[i],(a[i]),b);\n\
  }\n\
}\n\
\n\
\n\
// compute c0 + c1 x + c2 x^2 + ... + c_d x^d using horner's method\n\
//  = c0 + (c1 + ...  (c_d_2 + (c_d_1 + c_d x) x) x ...)x\n\
// m: m-symbols, d-degree \n\
//\n\
void " PREFIX_JET_m(nm_jet_polynomial_eval) "(" PREFIX_JET_m(t) " jet_out, " PREFIX_SCAL(t) " coefs[], " PREFIX_JET_m(t) " jet ) {\n\
  int i,j,k, n, d;\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2;\n\
  static int initialized;\n\
#pragma omp threadprivate(tmp1,tmp2,initialized)  \n\
\n\
  if(! initialized) {\n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    initialized=1;\n\
  }\n\
  \n\
  d = _MAX_DEGREE_OF_JET_VARS_;\n\
  \n\
  " PREFIX_JET_m(scal_mul2) "(tmp1, coefs[d],jet);\n\
  n = d-1;\n\
\n\
  while( n > 0) {\n\
    " PREFIX_JET_m(add_scal_jet) "(tmp2, coefs[n], tmp1);\n\
    " PREFIX_JET_m(nm_mul_jet) "( tmp1, tmp2, jet);\n\
    n--;\n\
  }\n\
  " PREFIX_JET_m(add_scal_jet) "(jet_out, coefs[0], tmp1);\n\
}\n\
\n\
\n\
// t= a/b, a float, jet b= b0 + b_1 X + ... b_p X^p = b0(1 + b1/b0 X +...)\n\
//                       = b0 (1-Z),  Z =-b1/b0 X - b2/b0 X^2 ,,,\n\
//  \n\
//   (a/b) = (a/b0) * (1 + Z + Z^2 + ... Z^p)\n\
//\n\
void " PREFIX_JET_m(scal_div2)  "(" PREFIX_JET_m(t) " t, " PREFIX_SCAL(t) " a, " PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2;\n\
  static " PREFIX_SCAL(t) " coefs[_MAX_DEGREE_OF_JET_VARS_+1];  \n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, temp;\n\
  int i;\n\
#pragma omp threadprivate(tmp1,tmp2,initialized, b0, temp)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(temp);\n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    for(i = 0; i < _MAX_DEGREE_OF_JET_VARS_+1; i++) {\n\
      " PREFIX_SCAL(init) "(coefs[i]);      \n\
      " PREFIX_SCAL(set_d) "(coefs[i], 1.0);\n\
    }\n\
    initialized=1;\n\
  }\n\
  \n\
  " PREFIX_SCAL(set) "(b0,  b[0]);\n\
  for(i = 1; i< nm_working_jetvar_size; i++) {\n\
    " PREFIX_SCAL(div2) "(temp, b[i], b0);\n\
    " PREFIX_SCAL(neg) "(tmp1[i],temp);    \n\
  }\n\
  " PREFIX_SCAL(set_d) "(tmp1[0], 0.0);\n\
  // tmp1 = Z\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp2, coefs, tmp1);\n\
  \n\
  " PREFIX_SCAL(div2) "(temp, a, b0);\n\
  " PREFIX_JET_m(mul2_scal) "(t, tmp2, temp);  \n\
}\n\
\n\
// just a copy of the above\n\
void " PREFIX_JET_m(d_div2)  "(" PREFIX_JET_m(t) " t, double a, " PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2;\n\
  static " PREFIX_SCAL(t) " coefs[_MAX_DEGREE_OF_JET_VARS_+1];  \n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, temp, fa;\n\
  int i;\n\
#pragma omp threadprivate(tmp1,tmp2,initialized, b0, temp,fa)\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(temp);\n\
    " PREFIX_SCAL(init) "(fa);        \n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    for(i = 0; i < _MAX_DEGREE_OF_JET_VARS_+1; i++) {\n\
      " PREFIX_SCAL(init) "(coefs[i]);            \n\
      " PREFIX_SCAL(set_d) "(coefs[i], 1.0);\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set_d) "(fa, a);    \n\
  " PREFIX_SCAL(set) "(b0,  b[0]);\n\
  for(i = 1; i< nm_working_jetvar_size; i++) {\n\
    " PREFIX_SCAL(div2) "(temp, b[i], b0);\n\
    " PREFIX_SCAL(neg) "(tmp1[i],temp);\n\
  }\n\
  " PREFIX_SCAL(set_d) "(tmp1[0], 0.0);\n\
\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp2, coefs, tmp1);\n\
  " PREFIX_SCAL(div2) "(temp, fa, b0);\n\
  " PREFIX_JET_m(mul2_scal) "(t, tmp2, temp);  \n\
}\n\
\n\
void " PREFIX_JET_m(si_div2)  "(" PREFIX_JET_m(t) " t, int a, " PREFIX_JET_m(t) " b) {\n\
  " PREFIX_JET_m(d_div2)  "(t, (double) a, b);  \n\
}\n\
\n\
/* t = a/b,  a,b jets */\n\
//  t = a * 1/b\n\
void " PREFIX_JET_m(div2) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a," PREFIX_JET_m(t) " b)  {\n\
  static " PREFIX_JET_m(t) " tmp1;\n\
  static " PREFIX_SCAL(t) " one;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate(tmp1,one, initailzed)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set_d) "(one, 1.0);\n\
  " PREFIX_JET_m(scal_div2)  "(tmp1, one, b);\n\
  " PREFIX_JET_m(mul2) "(t,a,tmp1);\n\
}\n\
\n\
\n\
/* t=exp(b), b = b_0 + b_1 x + ... */\n\
// exp(b) = exp(b0) * exp(b_1 X + ...) = exp(b0) * exp(Z)\n\
// = exp(b0) * (1 + Z + 1/2! Z^2 + ... 1/p! Z^p)\n\
//\n\
void " PREFIX_JET_m(set_exp) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " t0, t1;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " tmp, b0, _c, zero, one, coefs[_MAX_DEGREE_OF_JET_VARS_+1];\n\
#pragma omp threadprivate(t0, t1, b0, tmp, _c, zero, one, initialized, coefs)\n\
  int i;  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(tmp);\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(zero);        \n\
    " PREFIX_SCAL(init) "(one);    \n\
    " PREFIX_JET_m(init) "(&t0);    \n\
    " PREFIX_JET_m(init) "(&t1);\n\
    " PREFIX_SCAL(set_d) "(zero, 0.0);\n\
    " PREFIX_SCAL(set_d) "(one, 1.0);            \n\
    for(i=0; i<=_MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(coefs[i]);\n\
      if(i == 0 || i == 1) {\n\
	" PREFIX_SCAL(set_d) "(coefs[i], 1.0);    \n\
      } else {\n\
	" PREFIX_SCAL(set_d) "(tmp, i);    	\n\
	" PREFIX_SCAL(div2) "(coefs[i],coefs[i-1],tmp);\n\
      }\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  expMyFloatA(_c,b0);\n\
\n\
  " PREFIX_JET_m(set) "(t0,b);\n\
  " PREFIX_SCAL(set) "(t0[0],zero);  \n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(t1, coefs, t0);\n\
\n\
  " PREFIX_JET_m(mul2_scal) "(t,t1,_c);\n\
}\n\
\n\
\n\
\n\
/* t=sin(b), b = b_0 + b_1 X + ...*/\n\
// sin(b) = sin(b0 + Z) = sin(b0) cos(Z) + cos(b0) sin(Z)\n\
// sin(Z) = Z - 1/3! z^3 + 1/5! Z^5 -...\n\
// cos(Z) = 1 - 1/2! Z^2 + 1/4! Z^4 -...\n\
//\n\
void " PREFIX_JET_m(set_sin) "(" PREFIX_JET_m(t) " out," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, _c, _s, smp, smp1, one, zero ;\n\
  static " PREFIX_SCAL(t) " odds[_MAX_DEGREE_OF_JET_VARS_+1], evens[_MAX_DEGREE_OF_JET_VARS_+1];  \n\
#pragma omp threadprivate(tmp1, tmp2, tmp3, one, zero,b0, _c, _s,smp, smp1,initialized,odds[_MAX_DEGREE_OF_JET_VARS_+1], evens[_MAX_DEGREE_OF_JET_VARS_+1])\n\
  int i, esign, osign;\n\
  if(!initialized) {\n\
    esign = -1; osign= 1;    \n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(_s);\n\
    " PREFIX_SCAL(init) "(smp);\n\
    " PREFIX_SCAL(init) "(smp1);\n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_SCAL(init) "(zero);        \n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    " PREFIX_JET_m(init) "(&tmp3);\n\
    for(i =0; i<= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(odds[i]);\n\
      " PREFIX_SCAL(init) "(evens[i]);\n\
    }\n\
    " PREFIX_SCAL(set_d) "(zero, 0.0);    \n\
    " PREFIX_SCAL(set_d) "(one, 1.0);\n\
    " PREFIX_SCAL(set) "(odds[0], zero);\n\
    " PREFIX_SCAL(set) "(evens[0], one);    \n\
    " PREFIX_SCAL(set) "(smp, one);\n\
    for(i = 1; i <= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(set_d) "(smp1, i);\n\
      " PREFIX_SCAL(div2) "(smp, smp, smp1) ;\n\
      if( i & 1) {\n\
	" PREFIX_SCAL(set) "(evens[i],zero);\n\
	if( osign ==1 ) {\n\
	  " PREFIX_SCAL(set) "(odds[i],  smp);\n\
	} else {\n\
	  " PREFIX_SCAL(neg) "(odds[i], smp);	  \n\
	}\n\
	osign = -osign;\n\
      } else {\n\
	" PREFIX_SCAL(set) "(odds[i], zero);\n\
	if( esign == 1) {	\n\
	  " PREFIX_SCAL(set) "(evens[i],smp);\n\
	} else {\n\
	  " PREFIX_SCAL(neg) "(evens[i],smp);	  \n\
	}\n\
	esign = - esign;\n\
      }\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(set_cos) "(_c,b0);\n\
  " PREFIX_SCAL(set_sin) "(_s,b0);\n\
  for(i = 1; i< nm_working_jetvar_size; i++) {\n\
    " PREFIX_SCAL(set) "(tmp1[i], b[i]);\n\
  }\n\
  " PREFIX_SCAL(set) "(tmp1[0], zero);  \n\
  // tmp1 = Z\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp2, odds, tmp1);  // sin(Z)\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp3, evens, tmp1); // cos(Z)\n\
  \n\
  " PREFIX_JET_m(scal_mul2) "(tmp1, _c, tmp2);  //cos(b0)sin(Z)\n\
  " PREFIX_JET_m(scal_mul2) "(tmp2, _s, tmp3);  //sin(b0)cos(Z)\n\
\n\
  " PREFIX_JET_m(add2) "(out, tmp1, tmp2);\n\
\n\
}\n\
\n\
/* t=cos(b), b = b_0 + b_1 X + ...*/\n\
// cos(b) = cos(b0 + Z) = cos(b0) cos(Z) - sin(b0) sin(Z)\n\
// sin(Z) = Z - 1/3! z^3 + 1/5! Z^5 -...\n\
// cos(Z) = 1 - 1/2! Z^2 + 1/4! Z^4 -...\n\
\n\
void " PREFIX_JET_m(set_cos) "(" PREFIX_JET_m(t) " out," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2, tmp3;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, _c, _s, smp, smp1, one, zero ;\n\
  static " PREFIX_SCAL(t) " odds[_MAX_DEGREE_OF_JET_VARS_+1], evens[_MAX_DEGREE_OF_JET_VARS_+1];  \n\
#pragma omp threadprivate(tmp1, tmp2, tmp3, one, zero,b0, _c, _s, smp, smp1,initialized,odds[_MAX_DEGREE_OF_JET_VARS_+1], evens[_MAX_DEGREE_OF_JET_VARS_+1])\n\
  int i, esign,osign;\n\
  if(!initialized) {\n\
    esign=-1; osign = 1;\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(_s);\n\
    " PREFIX_SCAL(init) "(smp);\n\
    " PREFIX_SCAL(init) "(smp1);\n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_SCAL(init) "(zero);            \n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    " PREFIX_JET_m(init) "(&tmp3);\n\
    for(i =0; i<= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(odds[i]); " PREFIX_SCAL(init) "(evens[i]);\n\
    }\n\
    " PREFIX_SCAL(set_d) "(zero, 0.0);    \n\
    " PREFIX_SCAL(set_d) "(one, 1.0);\n\
    " PREFIX_SCAL(set) "(odds[0], zero);\n\
    " PREFIX_SCAL(set) "(evens[0], one);    \n\
    " PREFIX_SCAL(set) "(smp, one);\n\
    for(i = 1; i <= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(set_d) "(smp1, i);\n\
      " PREFIX_SCAL(div2) "(smp, smp, smp1) ;\n\
\n\
      if( i & 1) {\n\
	" PREFIX_SCAL(set) "(evens[i],zero);\n\
	if( osign ==1 ) {\n\
	  " PREFIX_SCAL(set) "(odds[i],  smp);\n\
	} else {\n\
	  " PREFIX_SCAL(neg) "(odds[i], smp);	  \n\
	}\n\
	osign= -osign;\n\
      } else {\n\
	" PREFIX_SCAL(set) "(odds[i], zero);\n\
	if( esign == 1) {	\n\
	  " PREFIX_SCAL(set) "(evens[i],smp);\n\
	} else {\n\
	  " PREFIX_SCAL(neg) "(evens[i],smp);	  \n\
	}\n\
	esign = - esign;\n\
      }\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(set_cos) "(_c,b0);\n\
  " PREFIX_SCAL(set_sin) "(_s,b0);\n\
\n\
  for(i = 1; i< nm_working_jetvar_size; i++) {\n\
    " PREFIX_SCAL(set) "(tmp1[i], b[i]);\n\
  }\n\
  " PREFIX_SCAL(set) "(tmp1[0], zero);  \n\
  // tmp1 = Z\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp2, odds, tmp1); // sin(Z)\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp3, evens, tmp1); // cos(Z)\n\
  \n\
  " PREFIX_JET_m(scal_mul2) "(tmp1, _s, tmp2);  //sin(b0)sin(Z)\n\
  " PREFIX_JET_m(scal_mul2) "(tmp2, _c, tmp3);  //cos(b0)cos(Z)\n\
\n\
  " PREFIX_JET_m(sub2) "(out, tmp2, tmp1);\n\
}\n\
\n\
/* t = tan(b) */\n\
// t = sin(b)/cos(b)\n\
//\n\
void " PREFIX_JET_m(set_tan) "(" PREFIX_JET_m(t) " out," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, _c, _s, smp, smp1, one, zero ;\n\
  static " PREFIX_SCAL(t) " odds[_MAX_DEGREE_OF_JET_VARS_+1], evens[_MAX_DEGREE_OF_JET_VARS_+1];  \n\
#pragma omp threadprivate(tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, one, zero,b0, _c, _s,smp, smp1,initialized,odds[_MAX_DEGREE_OF_JET_VARS_+1], evens[_MAX_DEGREE_OF_JET_VARS_+1])\n\
  int i, osign, esign;\n\
  if(!initialized) {\n\
    esign=-1; osign = 1;          \n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(_c);\n\
    " PREFIX_SCAL(init) "(_s);\n\
    " PREFIX_SCAL(init) "(smp);\n\
    " PREFIX_SCAL(init) "(smp1);\n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_SCAL(init) "(zero);            \n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    " PREFIX_JET_m(init) "(&tmp3);\n\
    " PREFIX_JET_m(init) "(&tmp4);\n\
    " PREFIX_JET_m(init) "(&tmp5);\n\
    " PREFIX_JET_m(init) "(&tmp6);\n\
    for(i =0; i<= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(odds[i]);\n\
      " PREFIX_SCAL(init) "(evens[i]);\n\
    }\n\
    " PREFIX_SCAL(set_d) "(zero, 0.0);    \n\
    " PREFIX_SCAL(set_d) "(one, 1.0);\n\
    " PREFIX_SCAL(set) "(odds[0], zero);\n\
    " PREFIX_SCAL(set) "(evens[0], one);    \n\
    " PREFIX_SCAL(set) "(smp, one);\n\
    for(i = 1; i <= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(set_d) "(smp1, i);\n\
      " PREFIX_SCAL(div2) "(smp, smp, smp1) ;\n\
      if( i & 1) {\n\
	" PREFIX_SCAL(set) "(evens[i],zero);\n\
	if( osign ==1 ) {\n\
	  " PREFIX_SCAL(set) "(odds[i],  smp);\n\
	} else {\n\
	  " PREFIX_SCAL(neg) "(odds[i], smp);	  \n\
	}\n\
	osign = -osign;\n\
      } else {\n\
	" PREFIX_SCAL(set) "(odds[i], zero);\n\
	if( esign == 1) {	\n\
	  " PREFIX_SCAL(set) "(evens[i],smp);\n\
	} else {\n\
	  " PREFIX_SCAL(neg) "(evens[i],smp);	  \n\
	}\n\
	esign=-esign;\n\
      }\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(set_cos) "(_c,b0);\n\
  " PREFIX_SCAL(set_sin) "(_s,b0);\n\
\n\
  for(i = 1; i< nm_working_jetvar_size; i++) {\n\
    " PREFIX_SCAL(set) "(tmp1[i], b[i]);\n\
  }\n\
  " PREFIX_SCAL(set) "(tmp1[0], zero);  \n\
  // tmp1 = Z\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp2, odds, tmp1); // sin(Z)\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(tmp3, evens, tmp1); // cos(Z)\n\
\n\
  " PREFIX_JET_m(scal_mul2) "(tmp4, _c, tmp2);  //cos(b0)sin(Z)\n\
  " PREFIX_JET_m(scal_mul2) "(tmp5, _s, tmp3);  //sin(b0)cos(Z)\n\
\n\
  " PREFIX_JET_m(add2) "(tmp6, tmp4, tmp5);  //sin\n\
  \n\
  " PREFIX_JET_m(scal_mul2) "(tmp1, _s, tmp2);  //sin(b0)sin(Z)\n\
  " PREFIX_JET_m(scal_mul2) "(tmp2, _c, tmp3);  //cos(b0)cos(Z)\n\
\n\
  " PREFIX_JET_m(sub2) "(tmp3, tmp2, tmp1); //cos\n\
\n\
  " PREFIX_JET_m(div2) "(out,tmp6,tmp3); // sin/cos\n\
}\n\
\n\
/* t=ln(b), b = b_0 + b_1 X + b_2 X^2 + ...*/\n\
//\n\
//  ln(b) = ln[b_0( 1+ b_1/b_0 X + ...)]  = ln(b_0) + ln(1+Z)\n\
//   = ln(b_0) + (Z - 1/2 Z^2 + 1/3 Z^3 ... + 1/p Z^p)\n\
\n\
void " PREFIX_JET_m(set_log) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " t0;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " tmp, one, b0, lna,  coefs[_MAX_DEGREE_OF_JET_VARS_+1];\n\
#pragma omp threadprivate(t0, tmp,b0, lna, coefs, initialized)\n\
  int i,sign=1;  \n\
  if(!initialized) {\n\
    sign=1;\n\
    " PREFIX_SCAL(init) "(tmp);        \n\
    " PREFIX_SCAL(init) "(one);    \n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(lna);\n\
    " PREFIX_JET_m(init) "(&t0);\n\
    " PREFIX_SCAL(set_d) "(one,1.0);\n\
    \n\
    for(i =0; i <= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(coefs[i]);\n\
      if(i==0) {\n\
	" PREFIX_SCAL(set_d) "(coefs[i], 0);	\n\
      } else {\n\
	if(i&1){      \n\
	  " PREFIX_SCAL(set_d) "(tmp, i);\n\
	} else {\n\
	  " PREFIX_SCAL(set_d) "(tmp, -i);	\n\
	}\n\
	" PREFIX_SCAL(div2) "(coefs[i],one, tmp);\n\
      }\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  logMyFloatA(lna,b0);\n\
  \n\
  " PREFIX_JET_m(div2_scal) "(t0, b, b0);\n\
  " PREFIX_SCAL(set_d) "(t0[0],0.0);\n\
\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(t, coefs, t0);\n\
  \n\
  " PREFIX_SCAL(set) "(t[0], lna);\n\
}\n\
\n\
\n\
/* t=b^e, e float, b=b_0 + b_1 X + b_2 X^2 + ... */\n\
// b^2 = exp(e ln b) = exp( e ln[(b0)( 1 + b_1/b_0 X+ ...)])\n\
//     =  exp( e [ ln(b0) + ln( 1+ b_1/b_0 X+ ...)])\n\
//     = exp( e ln(b_0) ) * exp (e * ln( 1+Z))\n\
//     = b_0^e * exp( e ln( 1 + Z))\n\
//\n\
void " PREFIX_JET_m(set_pow_scal) "(" PREFIX_JET_m(t) " t, " PREFIX_JET_m(t) " b, " PREFIX_SCAL(t) " e) {\n\
  static " PREFIX_JET_m(t) " tmp1, tmp2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " b0, s1, smp1,smp2,one,zero, coefs[_MAX_DEGREE_OF_JET_VARS_+1];\n\
#pragma omp threadprivate(tmp1, tmp2, b0, s1, smp1,smp2,one,zero, initialized)\n\
  if(!initialized) {\n\
    int i;\n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(s1);  \n\
    " PREFIX_SCAL(init) "(smp1);\n\
    " PREFIX_SCAL(init) "(smp2);\n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_SCAL(init) "(zero);\n\
    " PREFIX_SCAL(set_d) "(one, 1.0);\n\
    " PREFIX_SCAL(set_d) "(zero,0.0);    \n\
    " PREFIX_JET_m(init) "(&tmp1);\n\
    " PREFIX_JET_m(init) "(&tmp2);\n\
    " PREFIX_SCAL(init) "(coefs[0]);    \n\
    " PREFIX_SCAL(set_d) "(coefs[0], 1.0);\n\
    for(i =1; i <= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(coefs[i]);          \n\
      " PREFIX_SCAL(set_d) "(smp1, i);\n\
      " PREFIX_SCAL(div2) "(coefs[i], coefs[i-1], smp1);\n\
    }\n\
    initialized=1;\n\
  }\n\
  \n\
  if(MyFloatA_EQ_B(e,one)) {\n\
    " PREFIX_JET_m(set) "(t,b);\n\
  } else if(MyFloatA_EQ_B(e,zero)) {\n\
    " PREFIX_JET_m(set_scal) "(t,one);    \n\
  } else {\n\
    int i;\n\
\n\
    " PREFIX_SCAL(set) "(b0, b[0]);\n\
    " PREFIX_SCAL(set_pow) "(s1,b0,e);\n\
\n\
    " PREFIX_JET_m(set) "(tmp1, b);\n\
    " PREFIX_JET_m(div2_scal) "(tmp2, tmp1, b0);\n\
    " PREFIX_SCAL(set) "(tmp2[0],one);\n\
    // tmp2 = b/b0 with 1 in the cst term, 1+Z\n\
    " PREFIX_JET_m(set_log) "(tmp1,tmp2); \n\
    // tmp1 = log(tmp2) =ln(1+Z)\n\
    " PREFIX_JET_m(mul2_scal) "(tmp2, tmp1, e);\n\
    // tmp2 = e * tmp1 = e * ln(1+Z)\n\
    " PREFIX_JET_m(set_exp) "(tmp1,tmp2);\n\
    // tmp1 = exp(tmp2) = exp( e ln(1+Z));\n\
    " PREFIX_JET_m(mul2_scal) "(t, tmp1, s1);\n\
    // t = b_0^e * exp( e ln(1+Z));\n\
  }\n\
}\n\
\n\
/* t=sqrt(a) */\n\
void " PREFIX_JET_m(set_sqrt) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " a) {\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " half;\n\
#pragma omp threadprivate(half, initialized)  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_SCAL(set_d) "(half, 0.5);    \n\
  }\n\
  " PREFIX_JET_m(set_pow_scal) "(t, a, half);\n\
}\n\
\n\
/* t= atan(b) */\n\
// atan(x) = x - x^3/3 + x^5/5 + ... \n\
//\n\
// atan(b0+s) = atan( (b0+t)/(1-b0t)) = atan(b0) + atan(t)\n\
// where t = s/(1+b0^2 + b0 s)\n\
//_\n\
void "  PREFIX_JET_m(set_atan) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " tmp, b0, b02, b02p1, one, zero ,coefs[_MAX_DEGREE_OF_JET_VARS_+1];;\n\
#pragma omp threadprivate(t0, t1, t2, tmp, b0, b02, b02p1, one, zero, coefs,initialized)\n\
  int i;\n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(tmp);    \n\
    " PREFIX_SCAL(init) "(b0);\n\
    " PREFIX_SCAL(init) "(b02);\n\
    " PREFIX_SCAL(init) "(b02p1);        \n\
    " PREFIX_SCAL(init) "(one);\n\
    " PREFIX_SCAL(init) "(zero);    \n\
    " PREFIX_JET_m(init) "(&t0);    \n\
    " PREFIX_JET_m(init) "(&t1);\n\
    " PREFIX_JET_m(init) "(&t2);\n\
    " PREFIX_SCAL(set_d) "(one, 1.0);\n\
    " PREFIX_SCAL(set_d) "(zero, 0);    \n\
    for(i =0; i <= _MAX_DEGREE_OF_JET_VARS_; i++) {\n\
      " PREFIX_SCAL(init) "(coefs[i]);\n\
      if(i&1) {\n\
	if((i+1)%4 == 0) {\n\
	  " PREFIX_SCAL(set_d) "(tmp, -i);\n\
	} else {\n\
	  " PREFIX_SCAL(set_d) "(tmp, i);	  \n\
	}\n\
	" PREFIX_SCAL(div2) "(coefs[i], one, tmp);\n\
      } else {\n\
	" PREFIX_SCAL(set) "(coefs[i],zero);\n\
      }\n\
    }\n\
    initialized=1;\n\
  }\n\
  " PREFIX_SCAL(set) "(b0,b[0]);\n\
  " PREFIX_SCAL(mul2) "(b02,b0, b0);\n\
  " PREFIX_SCAL(add2) "(b02p1, b02,  one);\n\
  " PREFIX_JET_m(sub2_scal) "(t0, b, b0);\n\
  " PREFIX_JET_m(scal_mul2) "(t1, b0, t0);\n\
  " PREFIX_JET_m(add2_scal) "(t2,t1,b02p1);\n\
  " PREFIX_JET_m(div2) "(t1, t0, t2);\n\
  // b0+t1/(1-b0 t1) = b0+t0 = b\n\
  // now compute atan(b0) + atan(t1).\n\
  atanMyFloatA(b02, b0);\n\
  " PREFIX_JET_m(nm_jet_polynomial_eval) "(t, coefs, t1);\n\
  " PREFIX_SCAL(add2) "(t[0], t[0], b02);\n\
}\n\
\n\
\n\
/* t=sinh(b) */\n\
void " PREFIX_JET_m(set_sinh) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " half;\n\
#pragma omp threadprivate(t0, t1, t2, half,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_JET_m(init) "(&t0);    \n\
    " PREFIX_JET_m(init) "(&t1);\n\
    " PREFIX_JET_m(init) "(&t2);\n\
    " PREFIX_SCAL(set_d) "(half, 0.5);    \n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_m(neg) "(t0, b);\n\
  " PREFIX_JET_m(set_exp) "(t1,b);\n\
  " PREFIX_JET_m(set_exp) "(t2,t0);\n\
  " PREFIX_JET_m(sub2) "(t0,t1,t2);\n\
  " PREFIX_JET_m(scal_mul2) "(t, half, t0);\n\
}\n\
\n\
/* t=cosh(b) */\n\
void " PREFIX_JET_m(set_cosh) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) " t0, t1, t2;\n\
  static int initialized = 0;\n\
  static " PREFIX_SCAL(t) " half;\n\
#pragma omp threadprivate(t0, t1, t2, half,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_SCAL(init) "(half);\n\
    " PREFIX_JET_m(init) "(&t0);    \n\
    " PREFIX_JET_m(init) "(&t1);\n\
    " PREFIX_JET_m(init) "(&t2);\n\
    " PREFIX_SCAL(set_d) "(half, 0.5);    \n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_m(neg) "(t0, b);\n\
  " PREFIX_JET_m(set_exp) "(t1,b);\n\
  " PREFIX_JET_m(set_exp) "(t2,t0);\n\
  " PREFIX_JET_m(add2) "(t0,t1,t2);\n\
  " PREFIX_JET_m(scal_mul2) "(t, half, t0);\n\
}\n\
/* t=tanh(b) */\n\
void " PREFIX_JET_m(set_tanh) "(" PREFIX_JET_m(t) " t," PREFIX_JET_m(t) " b) {\n\
  static " PREFIX_JET_m(t) "  t1, t2;\n\
  static int initialized = 0;\n\
#pragma omp threadprivate( t1, t2,initialized)\n\
  \n\
  if(!initialized) {\n\
    " PREFIX_JET_m(init) "(&t1);\n\
    " PREFIX_JET_m(init) "(&t2);\n\
    initialized=1;\n\
  }\n\
  " PREFIX_JET_m(set_sinh) "(t1,b);\n\
  " PREFIX_JET_m(set_cosh) "(t2,b);\n\
  " PREFIX_JET_m(div2) "(t, t1, t2);  \n\
}\n\
\n\
\n\
void " PREFIX_JET_m(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET_m(t) " a)\n\
{\n\
  int k;\n\
  for (k = 0; k < nm_working_jetvar_size; k++) {\n\
    OutputMyFloat3(file, fmt, a[k]);\n\
  }\n\
}\n\
\n\
// multiplication two jets\n\
//void " PREFIX_JET_m(nm_mul_jet) "( " PREFIX_JET_m(t) " c, " PREFIX_JET_m(t) " a, " PREFIX_JET_m(t) " b);\n\
"

#define MY_JET_m_POSTCODE(PREFIX_JET_m,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET_m(t) " */\n" \


#endif /* MY_JET_m_H */
