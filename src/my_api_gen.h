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
#ifndef MY_API_GEN_H
#define MY_API_GEN_H

#define MY_API_MACROS_GEN(DSTENV,SRCENV) "\
#ifndef " DSTENV(t) "\n\
\n\
#define " DSTENV(t) " " SRCENV(t) "\n\
\n\
#define " DSTENV(init) " " SRCENV(init) "\n\
#define " DSTENV(clean) " " SRCENV(clean) "\n\
\n\
/* assign */\n\
#define " DSTENV(set) " " SRCENV(set) "\n\
#define " DSTENV(set_d) " " SRCENV(set_d) "\n\
#define " DSTENV(set_si) " " SRCENV(set_si) "\n\
#define " DSTENV(set_zero) " " SRCENV(set_zero) "\n\
\n\
/* elemental operations */\n\
#define " DSTENV(add2) " " SRCENV(add2) "\n\
#define " DSTENV(sub2) " " SRCENV(sub2) "\n\
#define " DSTENV(mul2) " " SRCENV(mul2) "\n\
#define " DSTENV(div2) " " SRCENV(div2) "\n\
\n\
#define " DSTENV(add2_d) " " SRCENV(add2_d) "\n\
#define " DSTENV(sub2_d) " " SRCENV(sub2_d) "\n\
#define " DSTENV(mul2_d) " " SRCENV(mul2_d) "\n\
#define " DSTENV(div2_d) " " SRCENV(div2_d) "\n\
#define " DSTENV(d_sub2) " " SRCENV(d_sub2) "\n\
#define " DSTENV(d_div2) " " SRCENV(d_div2) "\n\
\n\
#define " DSTENV(add2_si) " " SRCENV(add2_si) "\n\
#define " DSTENV(sub2_si) " " SRCENV(sub2_si) "\n\
#define " DSTENV(mul2_si) " " SRCENV(mul2_si) "\n\
#define " DSTENV(div2_si) " " SRCENV(div2_si) "\n\
#define " DSTENV(si_sub2) " " SRCENV(si_sub2) "\n\
#define " DSTENV(si_div2) " " SRCENV(si_div2) "\n\
\n\
#define " DSTENV(neg) " " SRCENV(neg) "\n\
\n\
/* elemental functions */\n\
#define " DSTENV(pow_d) " " SRCENV(pow_d) "\n\
#define " DSTENV(sqrt) " " SRCENV(sqrt) "\n\
#define " DSTENV(exp) " " SRCENV(exp) "\n\
#define " DSTENV(log) " " SRCENV(log) "\n\
#define " DSTENV(sin) " " SRCENV(sin) "\n\
#define " DSTENV(cos) " " SRCENV(cos) "\n\
#define " DSTENV(tan) " " SRCENV(tan) "\n\
#define " DSTENV(sinh) " " SRCENV(sinh) "\n\
#define " DSTENV(cosh) " " SRCENV(cosh) "\n\
#define " DSTENV(tanh) " " SRCENV(tanh) "\n\
\n\
#define " DSTENV(abs) " " SRCENV(abs) "\n\
\n\
/* boolean operations */\n\
#define " DSTENV(ge) "  " SRCENV(ge) "\n\
#define " DSTENV(gt) "  " SRCENV(gt) "\n\
#define " DSTENV(le) "  " SRCENV(le) "\n\
#define " DSTENV(lt) "  " SRCENV(lt) "\n\
#define " DSTENV(eq) "  " SRCENV(eq) "\n\
#define " DSTENV(neq) " " SRCENV(neq) "\n\
\n\
/* output format */\n\
#define " DSTENV(fprintf) " " SRCENV(fprintf) "\n\
\n\
#endif /* END " DSTENV(t) " */\n\
" \

/* *******  **************  ******* */
/* *******  MY_FLOAT_MACROS  ******* */
/* *******  **************  ******* */
#define MY_FLOAT_MACROS(MY_FLOAT_FIXES) "\
/* MACROS TO LINK MY_FLOAT */\n\
#ifndef " MY_FLOAT_FIXES(t) "\n\
#define " MY_FLOAT_FIXES(t) "          MY_FLOAT\n\
\n\
#define " MY_FLOAT_FIXES(init) "       InitMyFloat\n\
#define " MY_FLOAT_FIXES(clean) "      ClearMyFloat\n\
\n\
/* assign */\n\
#define " MY_FLOAT_FIXES(set) "        AssignMyFloat\n\
#define " MY_FLOAT_FIXES(set_d) "      MakeMyFloatB\n\
#define " MY_FLOAT_FIXES(set_si) "     MakeMyFloatB\n\
#define " MY_FLOAT_FIXES(set_zero) "(r) MakeMyFloatC(r,\"0\",0)\n\
\n\
/* elemental operations */\n\
#define " MY_FLOAT_FIXES(add2) "       AddMyFloatA\n\
#define " MY_FLOAT_FIXES(sub2) "       SubtractMyFloatA\n\
#define " MY_FLOAT_FIXES(mul2) "       MultiplyMyFloatA\n\
#define " MY_FLOAT_FIXES(div2) "       DivideMyFloatA\n\
\n\
#define " MY_FLOAT_FIXES(add2_myfloat) "     AddMyFloatA\n\
#define " MY_FLOAT_FIXES(sub2_myfloat) "     SubtractMyFloatA\n\
#define " MY_FLOAT_FIXES(mul2_myfloat) "     MultiplyMyFloatA\n\
#define " MY_FLOAT_FIXES(div2_myfloat) "     DivideMyFloatA\n\
#define " MY_FLOAT_FIXES(myfloat_sub2) "     SubtractMyFloatA\n\
#define " MY_FLOAT_FIXES(myfloat_div2) "     DivideMyFloatA\n\
\n\
#define " MY_FLOAT_FIXES(add2_d) "     AddMyFloatD\n\
#define " MY_FLOAT_FIXES(sub2_d) "     SubtractMyFloatD\n\
#define " MY_FLOAT_FIXES(mul2_d) "     MultiplyMyFloatD\n\
#define " MY_FLOAT_FIXES(div2_d) "     DivideMyFloatD\n\
#define " MY_FLOAT_FIXES(d_sub2) "     SubtractDMyFloat\n\
#define " MY_FLOAT_FIXES(d_div2) "     DivideDMyFloat\n\
\n\
#define " MY_FLOAT_FIXES(add2_si) "    AddMyFloatSI\n\
#define " MY_FLOAT_FIXES(sub2_si) "    SubtractMyFloatSI\n\
#define " MY_FLOAT_FIXES(mul2_si) "    MultiplyMyFloatSI\n\
#define " MY_FLOAT_FIXES(div2_si) "    DivideMyFloatSI\n\
#define " MY_FLOAT_FIXES(si_sub2) "    SubtractSIMyFloat\n\
#define " MY_FLOAT_FIXES(si_div2) "    DivideSIMyFloat\n\
\n\
#define " MY_FLOAT_FIXES(neg) "        NegateMyFloatA\n\
\n\
/* built-in functions */\n\
#define " MY_FLOAT_FIXES(set_sqrt) "   sqrtMyFloatA\n\
#define " MY_FLOAT_FIXES(set_pow) "    ExponentiateMyFloatA\n\
#define " MY_FLOAT_FIXES(set_pow_si) " ExponentiateMyFloatIA\n\
#define " MY_FLOAT_FIXES(set_pow_myfloat) " " MY_FLOAT_FIXES(set_pow) "\n\
\n\
#define " MY_FLOAT_FIXES(set_sin) "    sinMyFloatA\n\
#define " MY_FLOAT_FIXES(set_cos) "    cosMyFloatA\n\
#define " MY_FLOAT_FIXES(set_tan) "    tanMyFloatA\n\
#define " MY_FLOAT_FIXES(set_atan) "   atanMyFloatA\n\
#define " MY_FLOAT_FIXES(set_exp) "    expMyFloatA\n\
#define " MY_FLOAT_FIXES(set_log) "    logMyFloatA\n\
#define " MY_FLOAT_FIXES(set_sinh) "   sinhMyFloatA\n\
#define " MY_FLOAT_FIXES(set_cosh) "   coshMyFloatA\n\
#define " MY_FLOAT_FIXES(set_tanh) "   tanhMyFloatA\n\
#define " MY_FLOAT_FIXES(set_log10) "  log10MyFloatA\n\
#define " MY_FLOAT_FIXES(set_fabs) "   fabsMyFloatA\n\
\n\
#define " MY_FLOAT_FIXES(sqrt) " " MY_FLOAT_FIXES(set_sqrt) "\n\
#define " MY_FLOAT_FIXES(pow) " " MY_FLOAT_FIXES(set_pow) "\n\
#define " MY_FLOAT_FIXES(pow_si) " " MY_FLOAT_FIXES(set_pow_si) "\n\
\n\
#define " MY_FLOAT_FIXES(sin) " " MY_FLOAT_FIXES(set_sin) "\n\
#define " MY_FLOAT_FIXES(cos) " " MY_FLOAT_FIXES(set_cos) "\n\
#define " MY_FLOAT_FIXES(tan) " " MY_FLOAT_FIXES(set_tan) "\n\
#define " MY_FLOAT_FIXES(atan) " " MY_FLOAT_FIXES(set_atan) "\n\
#define " MY_FLOAT_FIXES(exp) " " MY_FLOAT_FIXES(set_exp) "\n\
#define " MY_FLOAT_FIXES(log) " " MY_FLOAT_FIXES(set_log) "\n\
#define " MY_FLOAT_FIXES(sinh) " " MY_FLOAT_FIXES(set_sinh) "\n\
#define " MY_FLOAT_FIXES(cosh) " " MY_FLOAT_FIXES(set_cosh) "\n\
#define " MY_FLOAT_FIXES(tanh) " " MY_FLOAT_FIXES(set_tanh) "\n\
#define " MY_FLOAT_FIXES(log10) " " MY_FLOAT_FIXES(set_log10) "\n\
#define " MY_FLOAT_FIXES(fabs) " " MY_FLOAT_FIXES(set_fabs) "\n\
\n\
/* casts */\n\
#define " MY_FLOAT_FIXES(to_si) "  MyFloatToInt\n\
#define " MY_FLOAT_FIXES(to_d) "   MyFloatToDouble\n\
\n\
/* boolean operations */\n\
#define " MY_FLOAT_FIXES(ge) "  MyFloatA_GE_B\n\
#define " MY_FLOAT_FIXES(gt) "  MyFloatA_GT_B\n\
#define " MY_FLOAT_FIXES(le) "  MyFloatA_LE_B\n\
#define " MY_FLOAT_FIXES(lt) "  MyFloatA_LT_B\n\
#define " MY_FLOAT_FIXES(eq) "  MyFloatA_EQ_B\n\
#define " MY_FLOAT_FIXES(neq) " MyFloatA_NEQ_B\n\
\n\
/* output format */\n\
#define " MY_FLOAT_FIXES(fprintf3) " OutputMyFloat3\n\
\n\
/* input format */\n\
#define " MY_FLOAT_FIXES(fscanf) " InputMyFloat3\n\
#endif /* END " MY_FLOAT_FIXES(t) " */\n\
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


#define MY_CLOUD_MY_JET_MACROS(MY_CLOUD_FIXES,InitCleanFlag,MAX_NUM_SYMBOLS_NAME) "\
/* Macros used in the integration: MY_JET acts as MY_CLOUD */\n\
\n\
#define MY_JET " MY_CLOUD_FIXES(t) "\n\
\n\
#define InitUpJet()                " MY_CLOUD_FIXES(initup) "(" MAX_NUM_SYMBOLS_NAME ")\n\
#define InitJet(a)                 " MY_CLOUD_FIXES(init) "(" InitCleanFlag "(a))\n\
#define ClearJet(a)                " MY_CLOUD_FIXES(clean) "(" InitCleanFlag "(a))\n\
#define ClearUpJet()               " MY_CLOUD_FIXES(cleanup) "()\n\
#define GetJetVarDegree()          undefined\n\
#define GetJetNumSymbols()         undefined\n\
#define SetJetVarDegree(a)         undefined\n\
#define SetJetNumSymbols(a)        undefined\n\
#define InitJetParameter(a,i)      undefined\n\
\n\
#define NormJet(a,x)               {MakeMyFloatC(a,\"0\",0);" MY_CLOUD_FIXES(nrminf) "(&(a),x);}\n\
#define NormJet2(a,x)              undefined\n\
\n\
#define NegateJetA(t,s)            " MY_CLOUD_FIXES(neg) "(t,s)\n\
#define AssignJetToFloat(t,s)      AssignMyFloat(t,*" MY_CLOUD_FIXES(get) "(s,0))\n\
#define AssignFloatToJet(t,f)      " MY_CLOUD_FIXES(set_myfloat) "(t,f)\n\
\n\
\n\
#define AssignJetToFloatArray(t,s)  undefined\n\
#define AssignFloatArrayToJet(t,f)  undefined\n\
\n\
\n\
#define AssignJetToJet(t,s)        " MY_CLOUD_FIXES(set) "(t,s)\n\
#define AddJetJetA(t,a,b)          " MY_CLOUD_FIXES(add2) "(t,a,b)\n\
#define AddFloatJetA(t,a,b)        " MY_CLOUD_FIXES(add2_myfloat) "(t,b,a)\n\
#define AddJetFloatA(t,a,b)        " MY_CLOUD_FIXES(add2_myfloat) "(t,a,b)\n\
\n\
#define MultiplyJetFloatA(t,a,b)   " MY_CLOUD_FIXES(mul2_myfloat) "(t,a,b)\n\
#define MultiplyJetJetA(t,a,b)     " MY_CLOUD_FIXES(mul2) "(t,a,b)\n\
#define MultiplyFloatJetA(t,a,b)   " MY_CLOUD_FIXES(mul2_myfloat) "(t,b,a)\n\
\n\
#define DivideFloatJetA(t,a,b)     " MY_CLOUD_FIXES(myfloat_div2) "(t,a,b)\n\
#define DivideJetJetA(t,a,b)       " MY_CLOUD_FIXES(div2) "(t,a,b)\n\
#define DivideJetFloatA(t,a,b)     " MY_CLOUD_FIXES(div2_myfloat) "(t,a,b)\n\
\n\
#define SubtractFloatJetA(t,a,b)   " MY_CLOUD_FIXES(myfloat_sub2) "(t,a,b)\n\
#define SubtractJetFloatA(t,a,b)   " MY_CLOUD_FIXES(sub2_myfloat) "(t,a,b)\n\
#define SubtractJetJetA(t,a,b)     " MY_CLOUD_FIXES(sub2) "(t,a,b)\n\
\n\
#define ExponentiateJetCstA(t,s,e) " MY_CLOUD_FIXES(pow_myfloat) "(t,s,e)\n\
\n\
#define cosJetA(t,a)               " MY_CLOUD_FIXES(cos) "(t,a)\n\
#define sinJetA(t,a)               " MY_CLOUD_FIXES(sin) "(t,a)\n\
#define sqrtJetA(t,a)              " MY_CLOUD_FIXES(sqrt) "(t,a)\n\
#define tanJetA(r,a)               " MY_CLOUD_FIXES(tan) "(r,a)\n\
#define atanJetA(r,a)              " MY_CLOUD_FIXES(atan) "(r,a)\n\
#define expJetA(r,a)               " MY_CLOUD_FIXES(exp) "(r,a)\n\
#define logJetA(r,a)               " MY_CLOUD_FIXES(log) "(r,a)\n\
#define sinhJetA(r,a)              " MY_CLOUD_FIXES(sinh) "(r,a)\n\
#define coshJetA(r,a)              " MY_CLOUD_FIXES(cosh) "(r,a)\n\
#define tanhJetA(r,a)              undefined\n\
#define log10JetA(r,a)             undefined\n\
\n\
#define InputJetFromString(t,a)    undefined\n\
#define InputJet(t,a,b)            undefined\n\
#define OutputJet2File(file,fmt,t) " MY_CLOUD_FIXES(fprintf) "(file,fmt,t)\n\
#define OutputJet(fmt,t)           OutputJet2File(stdout,fmt,t)\n\
#define InputFile2Jet(file,fmt,t)  undefined\n\
\n\
#define EvalJet(v,x,s)             undefined\n\
#define JetCoefficient(x,idx)      " MY_CLOUD_FIXES(get) "(x,idx)\n\
/* END MACROS MY_CLOUD_MY_JET */\n" \

#define MY_FLOAT_API_NAME "myfloat"
#define MY_FLOAT_FIXES(x) MY_FLOAT_API_NAME "_" #x
#define MY_FLOAT_FUN(x) "MY_FLOAT_FUN(" #x ")"
#define MY_FLOAT_FUN_MACRO "#define " MY_FLOAT_FUN(x) " " MY_FLOAT_API_NAME "_ ## x"

#define MY_FLOAT_MYBLAS_API_NAME "myfloat_vec"
#define MY_FLOAT_MYBLAS_FIXES(x) MY_FLOAT_MYBLAS_API_NAME "_" #x
#define MY_FLOAT_MYBLAS_FUN(x) "MY_FLOAT_MYBLAS_FUN(" #x ")"
#define MY_FLOAT_MYBLAS_FUN_MACRO "#define " MY_FLOAT_MYBLAS_FUN(x) " " MY_FLOAT_MYBLAS_API_NAME "_ ## x"



#define MY_COEF_API_NAME "mycoef"
#define MY_COEF_FIXES(x) MY_COEF_API_NAME "_" #x

#define MY_JET_API_NAME "myjet"
#define MY_JET_FIXES(x) MY_JET_API_NAME "_" #x

#define MY_CLOUD_API_NAME "mycloud"
#define MY_CLOUD_FIXES(x) MY_CLOUD_API_NAME "_" #x

#endif /* MY_API_GEN_H */
