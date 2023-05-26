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

/* *******  **************  ******* */
/* *******  MY_FLOAT_MACROS  ******* */
/* *******  **************  ******* */
#define MY_FLOAT_MACROS(FIXES) "\
/* MACROS TO LINK MY_FLOAT WITH MY_JET COEFFICIENTS */\n\
#ifndef " FIXES(t) "\n\
#define " FIXES(t) "          MY_FLOAT\n\
\n\
#define " FIXES(init) "       InitMyFloat\n\
#define " FIXES(clean) "      ClearMyFloat\n\
\n\
/* assign */\n\
#define " FIXES(set) "        AssignMyFloat\n\
#define " FIXES(set_d) "      MakeMyFloatB\n\
#define " FIXES(set_si) "     MakeMyFloatB\n\
#define " FIXES(set_zero) "(r) MakeMyFloatC(r,\"0\",0)\n\
\n\
/* elemental operations */\n\
#define " FIXES(add2) "       AddMyFloatA\n\
#define " FIXES(sub2) "       SubtractMyFloatA\n\
#define " FIXES(mul2) "       MultiplyMyFloatA\n\
#define " FIXES(div2) "       DivideMyFloatA\n\
\n\
#define " FIXES(add2_d) "     AddMyFloatD\n\
#define " FIXES(sub2_d) "     SubtractMyFloatD\n\
#define " FIXES(mul2_d) "     MultiplyMyFloatD\n\
#define " FIXES(div2_d) "     DivideMyFloatD\n\
#define " FIXES(d_sub2) "     SubtractDMyFloat\n\
#define " FIXES(d_div2) "     DivideDMyFloat\n\
\n\
#define " FIXES(add2_si) "    AddMyFloatSI\n\
#define " FIXES(sub2_si) "    SubtractMyFloatSI\n\
#define " FIXES(mul2_si) "    MultiplyMyFloatSI\n\
#define " FIXES(div2_si) "    DivideMyFloatSI\n\
#define " FIXES(si_sub2) "    SubtractSIMyFloat\n\
#define " FIXES(si_div2) "    DivideSIMyFloat\n\
\n\
#define " FIXES(neg) "        NegateMyFloatA\n\
\n\
/* built-in functions */\n\
#define " FIXES(set_sqrt) "   sqrtMyFloatA\n\
#define " FIXES(set_pow) "    ExponentiateMyFloatA\n\
#define " FIXES(set_pow_si) " ExponentiateMyFloatIA\n\
\n\
#define " FIXES(set_sin) "    sinMyFloatA\n\
#define " FIXES(set_cos) "    cosMyFloatA\n\
#define " FIXES(set_tan) "    tanMyFloatA\n\
#define " FIXES(set_atan) "   atanMyFloatA\n\
#define " FIXES(set_exp) "    expMyFloatA\n\
#define " FIXES(set_log) "    logMyFloatA\n\
#define " FIXES(set_sinh) "   sinhMyFloatA\n\
#define " FIXES(set_cosh) "   coshMyFloatA\n\
#define " FIXES(set_tanh) "   tanhMyFloatA\n\
#define " FIXES(set_log10) "  log10MyFloatA\n\
#define " FIXES(set_fabs) "   fabsMyFloatA\n\
#define " FIXES(to_si) "      MyFloatToInt\n\
#define " FIXES(to_d) "       MyFloatToDouble\n\
\n\
/* boolean operations */\n\
#define " FIXES(ge) "  MyFloatA_GE_B\n\
#define " FIXES(gt) "  MyFloatA_GT_B\n\
#define " FIXES(le) "  MyFloatA_LE_B\n\
#define " FIXES(lt) "  MyFloatA_LT_B\n\
#define " FIXES(eq) "  MyFloatA_EQ_B\n\
#define " FIXES(neq) " MyFloatA_NEQ_B\n\
\n\
/* output format */\n\
#define " FIXES(fprintf) " OutputMyFloat3\n\
\n\
/* input format */\n\
#define " FIXES(fscanf) " InputMyFloat3\n\
#endif /* END " FIXES(t) " */\n\
\n" \

#define MY_JET_MACROS(MY_JET_FIXES,InitCleanFlag,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\
/* Macros used in the integration */\n\
typedef " MY_JET_FIXES(t) " MY_JET;\n\
#define InitUpJet2(nsymbs,deg)     " MY_JET_FIXES(initup2) "(nsymbs,deg)\n\
#define InitUpJet()                InitUpJet2(" MAX_NUM_SYMBOLS_NAME "," MAX_DEGREE_NAME ")\n\
#define InitJet(a)                 " MY_JET_FIXES(init) "(" InitCleanFlag "(a))\n\
#define ClearJet(a)                " MY_JET_FIXES(clean) "(" InitCleanFlag "(a))\n\
#define ClearUpJet()               " MY_JET_FIXES(cleanup) "()\n\
#define GetJetVarDegree()          " MY_JET_FIXES(get_deg) "()\n\
#define GetJetNumSymbols()         " MY_JET_FIXES(get_num_symbs) "()\n\
#define SetJetVarDegree(a)         " MY_JET_FIXES(set_deg) "(a)\n\
#define SetJetNumSymbols(a)        " MY_JET_FIXES(set_num_symbs) "(a)\n\
#define InitJetParameter(a,i)      undefined\n\
\n\
#define NormJet(a,x)               {MakeMyFloatC(a,\"0\",0);" MY_JET_FIXES(nrminf) "(&(a),x);}\n\
#define NormJet2(a,x)              {MakeMyFloatC(a,\"0\",0);" MY_JET_FIXES(nrm2) "(&(a),x);}\n\
\n\
#define NegateJetA(t,s)            " MY_JET_FIXES(neg) "(t,s)\n\
#define AssignJetToFloat(t,s)      AssignMyFloat(t,*" MY_JET_FIXES(to_myfloat) "(s))\n\
#define AssignFloatToJet(t,f)      " MY_JET_FIXES(set_myfloat) "(t,f)\n\
\n\
\n\
#define AssignJetToFloatArray(t,s)  " MY_JET_FIXES(set_jet_2_myfloat_array) "(t,s)\n\
#define AssignFloatArrayToJet(t,f)  " MY_JET_FIXES(set_myfloat_array) "(t,f)\n\
\n\
\n\
#define AssignJetToJet(t,s)        " MY_JET_FIXES(set) "(t,s)\n\
#define AddJetJetA(t,a,b)          " MY_JET_FIXES(add2) "(t,a,b)\n\
#define AddFloatJetA(t,a,b)        " MY_JET_FIXES(add2_myfloat) "(t,b,a)\n\
#define AddJetFloatA(t,a,b)        " MY_JET_FIXES(add2_myfloat) "(t,a,b)\n\
\n\
#define MultiplyJetFloatA(t,a,b)   " MY_JET_FIXES(mul2_myfloat) "(t,a,b)\n\
#define MultiplyJetJetA(t,a,b)     " MY_JET_FIXES(mul2) "(t,a,b)\n\
#define MultiplyFloatJetA(t,a,b)   " MY_JET_FIXES(mul2_myfloat) "(t,b,a)\n\
\n\
#define DivideFloatJetA(t,a,b)     " MY_JET_FIXES(myfloat_div2) "(t,a,b)\n\
#define DivideJetJetA(t,a,b)       " MY_JET_FIXES(div2) "(t,a,b)\n\
#define DivideJetFloatA(t,a,b)     " MY_JET_FIXES(div2_myfloat) "(t,a,b)\n\
\n\
#define SubtractFloatJetA(t,a,b)   " MY_JET_FIXES(myfloat_sub2) "(t,a,b)\n\
#define SubtractJetFloatA(t,a,b)   " MY_JET_FIXES(sub2_myfloat) "(t,a,b)\n\
#define SubtractJetJetA(t,a,b)     " MY_JET_FIXES(sub2) "(t,a,b)\n\
\n\
#define ExponentiateJetCstA(t,s,e) " MY_JET_FIXES(set_pow_myfloat) "(t,s,e)\n\
\n\
#define cosJetA(t,a)               " MY_JET_FIXES(set_cos) "(t,a)\n\
#define sinJetA(t,a)               " MY_JET_FIXES(set_sin) "(t,a)\n\
#define sqrtJetA(t,a)              " MY_JET_FIXES(set_sqrt) "(t,a)\n\
#define tanJetA(r,a)               " MY_JET_FIXES(set_tan) "(r,a)\n\
#define atanJetA(r,a)              " MY_JET_FIXES(set_atan) "(r,a)\n\
#define expJetA(r,a)               " MY_JET_FIXES(set_exp) "(r,a)\n\
#define logJetA(r,a)               " MY_JET_FIXES(set_log) "(r,a)\n\
#define sinhJetA(r,a)              " MY_JET_FIXES(set_sinh) "(r,a)\n\
#define coshJetA(r,a)              " MY_JET_FIXES(set_cosh) "(r,a)\n\
#define tanhJetA(r,a)              undefined\n\
#define log10JetA(r,a)             undefined\n\
\n\
#define InputJetFromString(t,a)    taylor_input_jet_from_string(t,a)\n\
#define InputJet(t,a,b)            taylor_input_jet_from_stdin(t,b)\n\
#define OutputJet2File(file,fmt,t) " MY_JET_FIXES(fprintf) "(file,fmt,t)\n\
#define OutputJet(fmt,t)           OutputJet2File(stdout,fmt,t)\n\
#define InputFile2Jet(file,fmt,t) " MY_JET_FIXES(fscanf) "(file,fmt,t)\n\
\n\
#define EvalJet(v,x,s)             " MY_JET_FIXES(eval) "(&(v),x,s)\n\
#define JetCoefficient(x,idx)      " MY_JET_FIXES(get_coef) "(x,idx)\n\
/* END MACROS MY_JET */\n" \

#define MY_FLOAT_FIXES(x) "myfloat_" #x
#define MY_COEF_FIXES(x)  "mycoef_" #x

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
\n\
void " PREFIX_JET(set_pow_myfloat) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " MYFLOAT ");\n\
\n\
void " PREFIX_JET(set_exp) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_log) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(set_sin) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_cos) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_tan) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_atan) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(set_sinh) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_cosh) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
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
