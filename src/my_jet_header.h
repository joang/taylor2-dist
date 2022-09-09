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
  sample_header_JET need to define MY_JET data type and the
  following macros. It shoule be defined for each arithmetic
  we support.
   qd/dd:  qdheader_JET.h
   gmp:    gmpHeader_JET.h
   mpfr:   mpfrheader_JET.h
*/

#ifndef MY_JET_HEADER_H
#define MY_JET_HEADER_H

typedef enum { \
  HEADER_NONE=0, \
  HEADER_JET1_1, \
  HEADER_JET_1, \
  HEADER_JET1, \
  HEADER_JET2, \
  HEADER_JET_2,	\
  HEADER_JET_m,	\
  HEADER_JET_TREE, \
  HEADER_JET } index_my_jet_header_t;
typedef enum { \
  CODE_NONE=0, \
  CODE_JET1_1, \
  CODE_JET_1, \
  CODE_JET1, \
  CODE_JET2, \
  CODE_JET_2, \
  CODE_JET_m, \
  CODE_JET_TREE, \
  CODE_JET} index_my_jet_code_t;

/* *******  **************  ******* */
/* *******  MY_SCAL_MACROS  ******* */
/* *******  **************  ******* */
#define MY_SCAL_MACROS(PREFIX_SCAL) "\
/* MACROS TO LINK MY_FLOAT WITH MY_JET COEFFICIENTS */\n\
#ifndef " PREFIX_SCAL(t) "\n\
#define " PREFIX_SCAL(t) "          MY_FLOAT\n\
\n\
#define " PREFIX_SCAL(init) "       InitMyFloat\n\
#define " PREFIX_SCAL(clean) "      ClearMyFloat\n\
\n\
/* assign */\n\
#define " PREFIX_SCAL(set) "        AssignMyFloat\n\
#define " PREFIX_SCAL(set_d) "      MakeMyFloatB\n\
#define " PREFIX_SCAL(set_si) "     MakeMyFloatB\n\
#define " PREFIX_SCAL(set_zero) "(r) MakeMyFloatC(r,\"0\",0)\n\
\n\
/* elemental operations */\n\
#define " PREFIX_SCAL(add2) "       AddMyFloatA\n\
#define " PREFIX_SCAL(sub2) "       SubtractMyFloatA\n\
#define " PREFIX_SCAL(mul2) "       MultiplyMyFloatA\n\
#define " PREFIX_SCAL(div2) "       DivideMyFloatA\n\
\n\
#define " PREFIX_SCAL(add2_d) "     AddMyFloatD\n\
#define " PREFIX_SCAL(sub2_d) "     SubtractMyFloatD\n\
#define " PREFIX_SCAL(mul2_d) "     MultiplyMyFloatD\n\
#define " PREFIX_SCAL(div2_d) "     DivideMyFloatD\n\
\n\
#define " PREFIX_SCAL(add2_si) "    AddMyFloatSI\n\
#define " PREFIX_SCAL(sub2_si) "    SubtractMyFloatSI\n\
#define " PREFIX_SCAL(si_sub2) "    SubtractSIMyFloat\n\
#define " PREFIX_SCAL(mul2_si) "    MultiplyMyFloatSI\n\
#define " PREFIX_SCAL(div2_si) "    DivideMyFloatSI\n\
\n\
#define " PREFIX_SCAL(neg) "        NegateMyFloatA\n\
\n\
/* built-in functions */\n\
#define " PREFIX_SCAL(set_sqrt) "   sqrtMyFloatA\n\
#define " PREFIX_SCAL(set_pow) "    ExponentiateMyFloatA\n\
#define " PREFIX_SCAL(set_pow_si) " ExponentiateMyFloatIA\n\
\n\
#define " PREFIX_SCAL(set_sin) "    sinMyFloatA\n\
#define " PREFIX_SCAL(set_cos) "    cosMyFloatA\n\
#define " PREFIX_SCAL(set_tan) "    tanMyFloatA\n\
#define " PREFIX_SCAL(set_atan) "   atanMyFloatA\n\
#define " PREFIX_SCAL(set_exp) "    expMyFloatA\n\
#define " PREFIX_SCAL(set_log) "    logMyFloatA\n\
#define " PREFIX_SCAL(set_sinh) "   sinhMyFloatA\n\
#define " PREFIX_SCAL(set_cosh) "   coshMyFloatA\n\
#define " PREFIX_SCAL(set_tanh) "   tanhMyFloatA\n\
#define " PREFIX_SCAL(set_log10) "  log10MyFloatA\n\
#define " PREFIX_SCAL(set_fabs) "   fabsMyFloatA\n\
#define " PREFIX_SCAL(to_si) "      MyFloatToInt\n\
#define " PREFIX_SCAL(to_d) "       MyFloatToDouble\n\
\n\
/* boolean operations */\n\
#define " PREFIX_SCAL(ge) "  MyFloatA_GE_B\n\
#define " PREFIX_SCAL(gt) "  MyFloatA_GT_B\n\
#define " PREFIX_SCAL(le) "  MyFloatA_LE_B\n\
#define " PREFIX_SCAL(lt) "  MyFloatA_LT_B\n\
#define " PREFIX_SCAL(eq) "  MyFloatA_EQ_B\n\
#define " PREFIX_SCAL(neq) " MyFloatA_NEQ_B\n\
\n\
/* output format */\n\
#define " PREFIX_SCAL(fprintf) " OutputMyFloat3\n\
#endif /* END MY_SCAL */\n\
\n" \

#define MY_JET_MACROS(MY_JET_FIXES,InitCleanFlag) "\
/* Macros used in the integration */\n\
typedef " MY_JET_FIXES(t) " MY_JET;\n\
#define InitUpJet2(nsymbs,deg)     " MY_JET_FIXES(initup) "(nsymbs,deg)\n\
#define InitUpJet()                InitUpJet2(_NUMBER_OF_SYMBOLS_,_DEGREE_OF_JET_VARS_)\n\
#define InitJet(a)                 " MY_JET_FIXES(init) "(" InitCleanFlag "(a))\n\
#define ClearJet(a)                " MY_JET_FIXES(clean) "(" InitCleanFlag "(a))\n\
#define ClearUpJet()               " MY_JET_FIXES(cleanup) "()\n\
#define SetJetVarDegree(a)         " MY_JET_FIXES(set_deg) "(a)\n\
#define SetNumSymbols(a)           " MY_JET_FIXES(set_num_symbs) "(a)\n\
#define InitJetParameter(a,i)      undefined\n	\
\n\
#define NormJet(a,x)               " MY_JET_FIXES(nrminf) "(&(a),x)\n\
#define NormJet2(a,x)              " MY_JET_FIXES(nrm2) "(&(a),x)\n\
\n\
#define NegateJetA(t,s)            " MY_JET_FIXES(neg) "(t,s)\n\
#define AssignJetToFloat(t,s)      " MY_JET_FIXES(to_scal) "(&(t),s)\n\
#define AssignFloatToJet(t,f)      " MY_JET_FIXES(set_scal) "(t,f)\n\
\n\
\n\
#define AssignJetToFloatArray(t,s)  " MY_JET_FIXES(set_jet_2_scal_array) "(t,s)\n\
#define AssignFloatArrayToJet(t,f)  " MY_JET_FIXES(set_scal_array) "(t,f)\n\
\n\
\n\
#define AssignJetToJet(t,s)        " MY_JET_FIXES(set) "(t,s)\n\
#define AddJetJetA(t,a,b)          " MY_JET_FIXES(add2) "(t,a,b)\n\
#define AddFloatJetA(t,a,b)        " MY_JET_FIXES(add2_scal) "(t,b,a)\n\
#define AddJetFloatA(t,a,b)        " MY_JET_FIXES(add2_scal) "(t,a,b)\n\
\n\
#define MultiplyJetFloatA(t,a,b)   " MY_JET_FIXES(mul2_scal) "(t,a,b)\n\
#define MultiplyJetJetA(t,a,b)     " MY_JET_FIXES(mul2) "(t,a,b)\n\
#define MultiplyFloatJetA(t,a,b)   " MY_JET_FIXES(mul2_scal) "(t,b,a)\n\
\n\
#define DivideFloatJetA(t,a,b)     " MY_JET_FIXES(scal_div2) "(t,a,b)\n\
#define DivideJetJetA(t,a,b)       " MY_JET_FIXES(div2) "(t,a,b)\n\
#define DivideJetFloatA(t,a,b)     " MY_JET_FIXES(div2_scal) "(t,a,b)\n\
\n\
#define SubtractFloatJetA(t,a,b)   " MY_JET_FIXES(scal_sub2) "(t,a,b)\n\
#define SubtractJetFloatA(t,a,b)   " MY_JET_FIXES(sub2_scal) "(t,a,b)\n\
#define SubtractJetJetA(t,a,b)     " MY_JET_FIXES(sub2) "(t,a,b)\n\
\n\
#define ExponentiateJetCstA(t,s,e) " MY_JET_FIXES(set_pow_scal) "(t,s,e)\n\
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
#define tanhJetA(r,a)              undefined  \n\
#define log10JetA(r,a)             undefined  \n\
\n\
#define InputJetFromString(t,a)    taylor_input_jet_from_string(t,a)\n\
#define InputJet(t,a,b)            taylor_input_jet_from_stdin(t,b)\n\
#define OutputJet2File(file,fmt,t) " MY_JET_FIXES(fprintf) "(file,fmt,t)\n\
#define OutputJet(fmt,t)           OutputJet2File(stdout,fmt,t)\n\
\n\
#define EvalJet(v,x,s)             " MY_JET_FIXES(eval) "(&(v),x,s)\n\
#define GetJetCoefficient(t,x,idx) " MY_JET_FIXES(get_coef) "(&(t),x,idx)\n\
#define SetJetCoefficient(x,idx,t) " MY_JET_FIXES(set_coef) "(x,idx,t)\n\
/* END MACROS MY_JET */\n" \

#define MY_FLOAT_FIXES(x) "myscal_" #x

#define MY_JET_TYPE1(PREFIX_JET,SCAL,I) "\
typedef " SCAL "* " PREFIX_JET(t) ";\n\
typedef " PREFIX_JET(t) "* " PREFIX_JET(ptr) ";\n" \

#define MY_JET_TYPE1_DEF(PREFIX_JET,SCAL,I) "\
#ifndef " PREFIX_JET(t) "\n\
#define " PREFIX_JET(t) " " SCAL "*\n\
#endif\n\
#ifndef " PREFIX_JET(ptr) "\n\
#define " PREFIX_JET(ptr) " " PREFIX_JET(t) "*\n\
#endif\n" \

#define MY_JET_TYPE2(PREFIX_JET,SCAL,I) "\
typedef " SCAL " " PREFIX_JET(t) "[_NV_+1];\n\
typedef " PREFIX_JET(t) "*" PREFIX_JET(ptr) ";\n" \

#define MY_JET_TYPE3(PREFIX_JET,SCAL,I) "\
typedef " SCAL "** " PREFIX_JET(t) ";\n\
typedef " PREFIX_JET(t) "* " PREFIX_JET(ptr) ";\n" \


#define MY_JET_APIS(PREFIX_JET,MY_JET_TYPE,PREFIX_SCAL,SCAL,I) "\
#include <stdio.h>\n\
" MY_JET_TYPE(PREFIX_JET,SCAL,I) "\
\n\
size_t " PREFIX_JET(init) "(" PREFIX_JET(ptr) ");\n\
void " PREFIX_JET(initup) "(" I ", " I ");\n\
" I " " PREFIX_JET(set_num_symbs) "(" I ");\n\
" I " " PREFIX_JET(set_deg) "(" I ");\n\
void " PREFIX_JET(cleanup) "(void);\n\
void " PREFIX_JET(clean) "(" PREFIX_JET(ptr) ");\n\
\n\
void " PREFIX_JET(set) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(set_scal) "(" PREFIX_JET(t) ", " SCAL ");\n\
\n\
void " PREFIX_JET(set_scal_array) "(" PREFIX_JET(t) ", " SCAL "*);\n\
void " PREFIX_JET(set_jet_2_scal_array) "("SCAL "*, " PREFIX_JET(t)");\n\
\n\
void " PREFIX_JET(set_d) "(" PREFIX_JET(t) ", double);\n\
void " PREFIX_JET(set_si) "(" PREFIX_JET(t) ", int);\n\
\n\
void " PREFIX_JET(to_scal) "(" SCAL "[1], " PREFIX_JET(t) ");\n\
void " PREFIX_JET(eval) "(" SCAL "[1], " PREFIX_JET(t) ", " SCAL "[]);\n\
void " PREFIX_JET(get_coef) "(" SCAL "[1], " PREFIX_JET(t) ", const " I "[]);\n\
void " PREFIX_JET(set_coef) "(" PREFIX_JET(t) ", const " I "[], " SCAL ");\n\
\n\
void " PREFIX_JET(nrminf) "(" SCAL "[1], " PREFIX_JET(t) ");\n\
void " PREFIX_JET(nrm2) "(" SCAL "[1], " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(neg) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(sub2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(div2) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(add2_scal) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " SCAL ");\n\
void " PREFIX_JET(sub2_scal) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " SCAL ");\n\
void " PREFIX_JET(scal_sub2) "(" PREFIX_JET(t) ", " SCAL ", " PREFIX_JET(t) ");\n\
void " PREFIX_JET(mul2_scal) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " SCAL ");\n\
void " PREFIX_JET(div2_scal) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " SCAL ");\n\
void " PREFIX_JET(scal_div2) "(" PREFIX_JET(t) ", " SCAL ", " PREFIX_JET(t) ");\n\
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
void " PREFIX_JET(set_sqrt) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ");\n\
\n\
void " PREFIX_JET(set_pow_scal) "(" PREFIX_JET(t) ", " PREFIX_JET(t) ", " SCAL ");\n\
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
void " PREFIX_JET(fprintf) "(FILE *file, const char *, " PREFIX_JET(t) ");\n\
#define " PREFIX_JET(printf) "(fmt,a) \
" PREFIX_JET(fprintf) "(stdout,fmt,a)\n\
" \

#define MY_JET_PREFIX "__JET_NAME_"
#define MY_JET_SUFFIX "_ODE_NAME__"
#define MY_JET_SPACE(x) MY_JET_PREFIX "_" #x "_" MY_JET_SUFFIX

#endif /* MY_JET_HEADER_H */
