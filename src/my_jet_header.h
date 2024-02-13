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

/* 
  MY_JET data type, API, and SUFFIXES
 */

#ifndef MY_JET_HEADER_H
#define MY_JET_HEADER_H

typedef enum { \
  PREFIX_NONE=0, \
  PREFIX_JET1_1, \
  PREFIX_JET_1, \
  PREFIX_JET1, \
  PREFIX_JET2, \
  PREFIX_JET_2, \
  PREFIX_JET_m, \
  PREFIX_JET_TREE, \
  PREFIX_JET1_1_BIS, } index_my_jet_prefix_t;

typedef enum { \
  HEADER_NONE=0, \
  HEADER_JET1_1, \
  HEADER_JET_1, \
  HEADER_JET1, \
  HEADER_JET2, \
  HEADER_JET_2,	\
  HEADER_JET_m,	\
  HEADER_JET_TREE, \
  HEADER_JET1_1_BIS } index_my_jet_header_t;

typedef enum { \
  CODE_NONE=0, \
  CODE_JET1_1, \
  CODE_JET_1, \
  CODE_JET1, \
  CODE_JET2, \
  CODE_JET_2, \
  CODE_JET_m, \
  CODE_JET_TREE, \
  CODE_JET1_1_BIS } index_my_jet_code_t;

#define MY_JET_MAX_NUM_SYMB_MACRO_NAME "_NUMBER_OF_MAX_SYMBOLS_"
#define MY_JET_MAX_DEGREE_MACRO_NAME "_MAX_DEGREE_OF_JET_VARS_"
#define MY_JET_TOTAL_COEFFS_MACRO_NAME "_JET_COEFFICIENTS_COUNT_TOTAL_"
#define MAX_SIZE_JET_VAR_MACRO_NAME "_MAX_SIZE_OF_JET_VAR_"

#define MY_JET_DATA_ACCESS "MY_JET_DATA"
#define MY_JET_RECURSION(f) f

#include "my_api_gen.h"

#define MY_JET_TYPE1(PREFIX_JET,COEF,I) "\
typedef " COEF "* " PREFIX_JET(t) ";\n\
typedef " PREFIX_JET(t) "* " PREFIX_JET(ptr) ";\n" \

#define MY_JET_TYPE1_DEF(PREFIX_JET,COEF,I) "\
#ifndef " PREFIX_JET(t) "\n\
#define " PREFIX_JET(t) " " COEF "*\n\
#endif\n\
#ifndef " PREFIX_JET(ptr) "\n\
#define " PREFIX_JET(ptr) " " PREFIX_JET(t) "*\n\
#endif\n" \

#define MY_JET_TYPE2(PREFIX_JET,COEF,I) "\
typedef " COEF " " PREFIX_JET(t) "[_NV_+1];\n\
typedef " PREFIX_JET(t) "*" PREFIX_JET(ptr) ";\n" \

#define MY_JET_TYPE3(PREFIX_JET,COEF,I) "\
typedef " COEF "** " PREFIX_JET(t) ";\n\
typedef " PREFIX_JET(t) "* " PREFIX_JET(ptr) ";\n" \


#define MY_JET_APIS(PREFIX_JET,MY_JET_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
#include <stdio.h>\n\
\n\
" MY_JET_TYPE(PREFIX_JET,MYCOEF,I) "\
\n\
int* " PREFIX_JET(monomial_counts) "(void);\n\
int* " PREFIX_JET(monomial_offsets) "(void);\n\
\n\
void " PREFIX_JET(initup2) "(" I ", " I ");\n\
#define " PREFIX_JET(initup) "() " PREFIX_JET(initup2) "(" MAX_NUM_SYMBOLS_NAME "," MAX_DEGREE_NAME ")\n\
size_t " PREFIX_JET(init) "(" PREFIX_JET(ptr) ");\n\
void " PREFIX_JET(clean) "(" PREFIX_JET(ptr) ");\n\
void " PREFIX_JET(cleanup) "(void);\n\
\n\
" I " " PREFIX_JET(get_num_symbs) "(void);\n\
" I " " PREFIX_JET(get_deg) "(void);\n\
" I " " PREFIX_JET(set_num_symbs) "(" I ");\n\
" I " " PREFIX_JET(set_deg) "(" I ");\n\
\n\
/* assign */\n\
void " PREFIX_JET(set) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(set_coef) "(" PREFIX_JET(t) ", " MYCOEF ");\n\
void " PREFIX_JET(set_myfloat) "(" PREFIX_JET(t) ", " MYFLOAT ");\n\
void " PREFIX_JET(set_d) "(" PREFIX_JET(t) ", double);\n\
void " PREFIX_JET(set_si) "(" PREFIX_JET(t) ", int);\n\
\n\
void " PREFIX_JET(set_coef_array) "(" PREFIX_JET(t) ", " MYCOEF "*);\n\
void " PREFIX_JET(set_jet_2_coef_array) "(" MYCOEF "*, " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(set_myfloat_array) "(" PREFIX_JET(t) ", " MYFLOAT "*);\n\
void " PREFIX_JET(set_jet_2_myfloat_array) "(" MYFLOAT "*, " PREFIX_JET(t) ");\n\
\n\
" MYFLOAT "* " PREFIX_JET(to_myfloat) "(" PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(eval) "(" MYCOEF "[1], " PREFIX_JET(t) ", " MYFLOAT "[]);\n\
\n\
/* access to coef */\n\
" MYCOEF "* " PREFIX_JET(get_coef) "(" PREFIX_JET(t) ", const " I "[]);\n\
" MYCOEF "* " PREFIX_JET(coef0) "(" PREFIX_JET(t) ");\n\
" MYCOEF "* " PREFIX_JET(coef1) "(" PREFIX_JET(t) ", " I ");\n\
\n\
/* norms */\n\
void " PREFIX_JET(nrminf) "(" MYFLOAT "[1], " PREFIX_JET(t) ");\n\
void " PREFIX_JET(nrm2) "(" MYFLOAT "[1], " PREFIX_JET(t) ");\n\
\n\
/* elemental arithmetic */\n\
void " PREFIX_JET(neg) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(sub2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(div2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2_coef) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYCOEF ");\n\
void " PREFIX_JET(sub2_coef) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYCOEF ");\n\
void " PREFIX_JET(coef_sub2) "(" PREFIX_JET(t) ", " MYCOEF ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2_coef) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYCOEF ");\n\
void " PREFIX_JET(div2_coef) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYCOEF ");\n\
void " PREFIX_JET(coef_div2) "(" PREFIX_JET(t) ", " MYCOEF ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2_myfloat) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYFLOAT ");\n\
void " PREFIX_JET(sub2_myfloat) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYFLOAT ");\n\
void " PREFIX_JET(myfloat_sub2) "(" PREFIX_JET(t) ", " MYFLOAT ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2_myfloat) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYFLOAT ");\n\
void " PREFIX_JET(div2_myfloat) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYFLOAT ");\n\
void " PREFIX_JET(myfloat_div2) "(" PREFIX_JET(t) ", " MYFLOAT ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2_d) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", double);\n\
void " PREFIX_JET(sub2_d) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", double);\n\
void " PREFIX_JET(d_sub2) "(" PREFIX_JET(t) ", double, " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2_d) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", double);\n\
void " PREFIX_JET(div2_d) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", double);\n\
void " PREFIX_JET(d_div2) "(" PREFIX_JET(t) ", double, " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2_si) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", int);\n\
void " PREFIX_JET(sub2_si) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", int);\n\
void " PREFIX_JET(si_sub2) "(" PREFIX_JET(t) ", int, " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2_si) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", int);\n\
void " PREFIX_JET(div2_si) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", int);\n\
void " PREFIX_JET(si_div2) "(" PREFIX_JET(t) ", int, " PREFIX_JET(t) ");\n\
\n\
/* elemental functions */\n\
void " PREFIX_JET(set_sqrt) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
//#define " PREFIX_JET(sqrt) " " PREFIX_JET(set_sqrt) "\n\
\n\
void " PREFIX_JET(set_pow_myfloat) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYFLOAT ");\n\
//#define " PREFIX_JET(pow_myfloat) " " PREFIX_JET(set_pow_myfloat) "\n\
\n\
void " PREFIX_JET(set_exp) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_log) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
//#define " PREFIX_JET(exp) " " PREFIX_JET(set_exp) "\n\
//#define " PREFIX_JET(log) " " PREFIX_JET(set_log) "\n\
\n\
void " PREFIX_JET(set_sin) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_cos) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_tan) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_atan) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
//#define " PREFIX_JET(sin) "  " PREFIX_JET(set_sin) "\n\
//#define " PREFIX_JET(cos) "  " PREFIX_JET(set_cos) "\n\
//#define " PREFIX_JET(tan) "  " PREFIX_JET(set_tan) "\n\
//#define " PREFIX_JET(atan) " " PREFIX_JET(set_atan) "\n\
\n\
void " PREFIX_JET(set_sinh) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_cosh) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
//#define " PREFIX_JET(sinh) " " PREFIX_JET(set_sinh) "\n\
//#define " PREFIX_JET(cosh) " " PREFIX_JET(set_cosh) "\n\
\n\
void " PREFIX_JET(set_fabs) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
//#define " PREFIX_JET(fabs) " " PREFIX_JET(set_fabs) "\n\
\n\
/* I/O handles */\n\
void " PREFIX_JET(fprintf) "(FILE *file, const char *, " PREFIX_JET(t) ");\n\
#define " PREFIX_JET(printf) "(fmt,a) " PREFIX_JET(fprintf) "(stdout,fmt,a)\n\
\n\
void " PREFIX_JET(fscanf) "(FILE *file, const char *, " PREFIX_JET(t) ");\n\
void " PREFIX_JET(sscanf) "(const char *, const char *, " PREFIX_JET(t) ");\n\
" \

#define MY_JET_PREFIX "__JET_NAME_"
#define MY_JET_SUFFIX "_ODE_NAME__"
#define MY_JET_SPACE(x) MY_JET_PREFIX "_" #x "_" MY_JET_SUFFIX
#define MY_JET_FUN(x) "MY_JET_FUN(" #x ")"

#endif /* MY_JET_HEADER_H */
