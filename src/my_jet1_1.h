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

#ifndef MY_JET1_1_H
#define MY_JET1_1_H

/* *******  ****************  ******* */
/* *******  MY_JET1_1_HEADER  ******* */
/* *******  ****************  ******* */
#define MY_JET1_1_FIXES MY_JET_SPACE

#define MY_JET1_1_TYPE(PREFIX_JET1_1,MYCOEF,I) "\
typedef " MYCOEF " " PREFIX_JET1_1(t) "[2];\n\
typedef " PREFIX_JET1_1(t) "* " PREFIX_JET1_1(ptr) ";\n" \

#define MY_JET1_1_API(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                      MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,InitCleanFlag) "\
" MY_JET1_1_TYPE(PREFIX_JET1_1,MYCOEF,I) "\n\
\n\
void " PREFIX_JET1_1(initup2) "(" I "," I ");\n\
#define " PREFIX_JET1_1(initup) "() " PREFIX_JET1_1(initup2) "(" MAX_NUM_SYMBOLS_NAME ", " MAX_DEGREE_NAME ")\n\
\n\
#define " PREFIX_JET1_1(init) "(x) {\\\n\
\t" PREFIX_MYCOEF(init) "(" InitCleanFlag "(*x)[0]);\\\n\
\t" PREFIX_MYCOEF(init) "(" InitCleanFlag "(*x)[1]);\
}\n\
#define " PREFIX_JET1_1(clean) "(x) {\\\n\
\t" PREFIX_MYCOEF(clean) "(" InitCleanFlag "(*x)[1]);\\\n\
\t" PREFIX_MYCOEF(clean) "(" InitCleanFlag "(*x)[0]);\
}\n\
void " PREFIX_JET1_1(cleanup) "(void);\n\
\n\
#define " PREFIX_JET1_1(get_num_symbs) "(x) (1)\n\
#define " PREFIX_JET1_1(get_deg) "(x) (1)\n\
#define " PREFIX_JET1_1(set_num_symbs) "(x) (fprintf(stderr,\"\%s:\%d Not allowed\\n\",__FILE__,__LINE__), exit(1),1)\n\
#define " PREFIX_JET1_1(set_deg) "(x) (fprintf(stderr,\"\%s:\%d Not allowed\\n\",__FILE__,__LINE__), exit(1),1)\n\
\n\
/* y' = x' */\n\
#define " PREFIX_JET1_1(set) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set) "(y[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(set) "(y[1],x[1]);\
}\n\
\n\
/* y' = (x[0],x[1]) */\n\
#define " PREFIX_JET1_1(set_coef_array) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set) "(y[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(set) "(y[1],x[1]);\
}\n\
/* (y[0],y[1])=x */\n\
#define " PREFIX_JET1_1(set_jet_2_coef_array) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set) "(y[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(set) "(y[1],x[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_scal) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set) "(y[0],x);\\\n\
\t" PREFIX_MYCOEF(set_zero) "(y[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_myfloat) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set_myfloat) "(y[0],x);\\\n\
\t" PREFIX_MYCOEF(set_zero) "(y[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_d) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set_d) "(y[0],x);\\\n\
\t" PREFIX_MYCOEF(set_zero) "(y[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_si) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(set_si) "(y[0],x);\\\n\
\t" PREFIX_MYCOEF(set_zero) "(y[1]);\
}\
\n\
\n\
#define " PREFIX_JET1_1(to_myfloat) "(x) " PREFIX_MYCOEF(to_myfloat) "(x[0])\n\
\n\
#define " PREFIX_JET1_1(eval) "(t,x,s) {\\\n\
\t" PREFIX_MYCOEF(mul2_myfloat) "(*(t),x[1],*(s));\\\n\
\t" PREFIX_MYCOEF(add2) "(*(t),*(t),x[0]);}\n\
\n\
/* y' = -x' */\n\
#define " PREFIX_JET1_1(neg) "(y,x) {\\\n\
\t" PREFIX_MYCOEF(neg) "(y[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(y[1],x[1]);\
}\n\
\n\
/* a = max(|x|,|x'|) */\n\
#define " PREFIX_JET1_1(nrminf) "(a,x) {\\\n\
\t" PREFIX_MYCOEF(nrminf) "(a,x[0]);\\\n\
\t" PREFIX_MYCOEF(nrminf) "(&" PREFIX_JET1_1(faux) ",x[1]);\\\n\
\tif (MyFloatA_LT_B(*(a)," PREFIX_JET1_1(faux) ")) {AssignMyFloat(*(a)," PREFIX_JET1_1(faux) ");}\
}\n\
/* a = (x^2+x'^2)^.5 */\n\
#define " PREFIX_JET1_1(nrm2) "(a,x) {\\\n\
\t" PREFIX_MYCOEF(nrm2) "(a,x[0]);\\\n\
\t" PREFIX_MYCOEF(nrm2) "(&" PREFIX_JET1_1(faux) ",x[1]);\\\n\
\tAddMyFloatA(" PREFIX_JET1_1(faux) "," PREFIX_JET1_1(faux) ",*(a));\\\n\
\tsqrtMyFloatA(*(a)," PREFIX_JET1_1(faux) ");\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(add2) "(z[0],x[0],y[0]);\\\n\
\t" PREFIX_MYCOEF(add2) "(z[1],x[1],y[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(sub2) "(z[0],x[0],y[0]);\\\n\
\t" PREFIX_MYCOEF(sub2) "(z[1],x[1],y[1]);\
}\n\
\n\
/* z' = x'*y + y'*x\n\
 * Assump. z[1]!=x[1] && z[0]!=y[0] in memory \n\
 */\n\
#define " PREFIX_JET1_1(mul2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1],x[0],y[1]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[0],x[1],y[0]);\\\n\
\t" PREFIX_MYCOEF(add2) "(z[1],z[0],z[1]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[0],x[0],y[0]);\
}\n\
\n\
/* z' = (x'*y-x*y')/y^2 = (x' - z*y')/y\n\
 * Assump. y[0]!=0, z[1]!=x[1] && z[0]!=y[0] in memory \n\
 */\n\
#define " PREFIX_JET1_1(div2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2) "(z[0],x[0],y[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1],z[0],y[1]);\\\n\
\t" PREFIX_MYCOEF(sub2) "(" PREFIX_JET1_1(caux) ",x[1],z[1]);\\\n\
\t" PREFIX_MYCOEF(div2) "(z[1]," PREFIX_JET1_1(caux) ",y[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_coef) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(add2) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_coef) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(sub2) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(coef_sub2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(sub2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_coef) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0  */\n\
#define " PREFIX_JET1_1(div2_coef) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(div2) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = -x*y'/y^2 = -z*y'/y. Assump. y!=0  */\n\
#define " PREFIX_JET1_1(coef_div2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET1_1(caux) ",z[1]);\\\n\
\t" PREFIX_MYCOEF(div2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1]," PREFIX_JET1_1(caux) ",z[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_myfloat) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(add2_myfloat) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_myfloat) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(sub2_myfloat) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(myfloat_sub2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(myfloat_sub2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_myfloat) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(mul2_myfloat) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(mul2_myfloat) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0 */\n\
#define " PREFIX_JET1_1(div2_myfloat) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2_myfloat) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(div2_myfloat) "(z[1],x[1],y);\
}\n\
/* z' = -z*y'/y. Assump. y!=0 and z[0]!=y[0] in memory */\n\
#define " PREFIX_JET1_1(myfloat_div2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET1_1(caux) ",z[1]);\\\n\
\t" PREFIX_MYCOEF(myfloat_div2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1]," PREFIX_JET1_1(caux) ",z[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_d) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(add2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_d) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(sub2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(d_sub2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(d_sub2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_d) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(mul2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(mul2_d) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0 */\n\
#define " PREFIX_JET1_1(div2_d) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(div2_d) "(z[1],x[1],y);\
}\n\
/* z' = -z*y'/y. Assump. y!=0 and z[0]!=y[0] in memory */\n\
#define " PREFIX_JET1_1(d_div2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET1_1(caux) ",z[1]);\\\n\
\t" PREFIX_MYCOEF(d_div2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1]," PREFIX_JET1_1(caux) ",z[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_si) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(add2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_si) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(sub2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(set) "(z[1],x[1]);\
}\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(si_sub2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(si_sub2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_si) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(mul2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(mul2_si) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0 */\n\
#define " PREFIX_JET1_1(div2_si) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_MYCOEF(div2_si) "(z[1],x[1],y);\
}\n\
/* z' = -z*y'/y. Assump. z[0]!=y[0] in memory */\n\
#define " PREFIX_JET1_1(si_div2) "(z,x,y) {\\\n\
\t" PREFIX_MYCOEF(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_MYCOEF(neg) "(" PREFIX_JET1_1(caux) ",z[1]);\\\n\
\t" PREFIX_MYCOEF(si_div2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(z[1]," PREFIX_JET1_1(caux) ",z[0]);\
}\n\
\n\
/* s' = x'/(2s). Assump. s!=0 */\n\
#define " PREFIX_JET1_1(set_sqrt) "(s,x) {\\\n\
\t" PREFIX_MYCOEF(set_sqrt) "(s[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(div2) "(s[1],x[1],s[0]);\\\n\
\t" PREFIX_MYCOEF(div2_si) "(s[1],s[1],2);\
}\
\n\
/* p' = e*p*x'/x. Assump. x[0] != 0 */\n\
#define " PREFIX_JET1_1(set_pow_myfloat) "(p,x,e) {\\\n\
\t" PREFIX_MYCOEF(div2) "(p[1],x[1],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_JET1_1(caux) ",p[1],e);\\\n\
\t" PREFIX_MYCOEF(set_pow_myfloat) "(p[0],x[0],e);\\\n\
\t" PREFIX_MYCOEF(mul2) "(p[1]," PREFIX_JET1_1(caux) ",p[0]);\
}\
\n\
/* e' = e*x' */\n\
#define " PREFIX_JET1_1(set_exp) "(e,x) {\\\n\
\t" PREFIX_MYCOEF(set_exp) "(e[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(e[1],e[0],x[1]);\
}\
\n\
/* l' = x'/x */\n\
#define " PREFIX_JET1_1(set_log) "(l,x) {\\\n\
\t" PREFIX_MYCOEF(div2) "(l[1],x[1],x[0]);\\\n\
\t" PREFIX_MYCOEF(set_log) "(l[0],x[0]);\
}\
\n\
/* c' =-sin(x)*x'. Assump. s!=x && c'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_cos) "(c,x) {\\\n\
\t" PREFIX_MYCOEF(set_sin) "(c[1],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(c[0],c[1],x[1]);\\\n\
\t" PREFIX_MYCOEF(neg) "(c[1],c[0]);\\\n\
\t" PREFIX_MYCOEF(set_cos) "(c[0],x[0]);\
}\
\n\
/* s' = cos(x)*x'. Assump. s!=x && s'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_sin) "(s,x) {\\\n\
\t" PREFIX_MYCOEF(set_cos) "(s[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(s[1],s[0],x[1]);\\\n\
\t" PREFIX_MYCOEF(set_sin) "(s[0],x[0]);\
}\
\n\
/* t' = (t^2+1)*x'. Assump. t!=x && t'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_tan) "(t,x) {\\\n\
\t" PREFIX_MYCOEF(set_tan) "(t[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(t[1],t[0],t[0]);\\\n\
\t" PREFIX_MYCOEF(add2_si) "(" PREFIX_JET1_1(caux) ",t[1],1);\\\n\
\t" PREFIX_MYCOEF(mul2) "(t[1]," PREFIX_JET1_1(caux) ",x[1]);\
}\
\n\
/* t' = x'/(x^2+1). Assump. t!=x && t'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_atan) "(t,x) {\\\n\
\t" PREFIX_MYCOEF(mul2) "(t[0],x[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(add2_si) "(t[0],t[0],1);\\\n\
\t" PREFIX_MYCOEF(div2) "(t[1],x[1],t[0]);\\\n\
\t" PREFIX_MYCOEF(set_atan) "(t[0],x[0]);\\\n\
}\
\n\
/* c' =sinh(x)*x'. Assump. c!=x && c'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_cosh) "(c,x) {\\\n\
\t" PREFIX_MYCOEF(set_sinh) "(c[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(c[1],c[0],x[1]);\\\n\
\t" PREFIX_MYCOEF(set_cosh) "(c[0],x[0]);\
}\
\n\
/* s' = cosh(x)*x'. Assump. s!=x && s'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_sinh) "(s,x) {\\\n\
\t" PREFIX_MYCOEF(set_cosh) "(s[0],x[0]);\\\n\
\t" PREFIX_MYCOEF(mul2) "(s[1],s[0],x[1]);\\\n\
\t" PREFIX_MYCOEF(set_sinh) "(s[0],x[0]);\
}\
\n\
\n\
#define " PREFIX_JET1_1(fprintf) "(file,fmt,s) {\\\n\
\t" PREFIX_MYCOEF(fprintf) "(file,fmt,s[0]);\\\n\
\t" PREFIX_MYCOEF(fprintf) "(file,fmt,s[1]);\\\n\
}\n\
#define " PREFIX_JET1_1(printf) "(fmt,s) " PREFIX_JET1_1(fprintf) "(stdout,fmt,s)\n\
\n\
\n\
#define " PREFIX_JET1_1(fscanf) "(file,fmt,s) {\\\n\
\t" PREFIX_MYCOEF(fscanf) "(file,fmt,s[0]);\\\n\
\t" PREFIX_MYCOEF(fscanf) "(file,fmt,s[1]);\\\n\
}\
\n\
void " PREFIX_JET1_1(sscanf) "(const char *, const char *, " PREFIX_JET1_1(t) ");\n\
\n\
" \

#define MY_JET1_1_PREHEADER(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* HEADER " PREFIX_JET1_1(t) " */\n\
extern " MYFLOAT " " PREFIX_JET1_1(faux) ";\n\
extern " MYCOEF " " PREFIX_JET1_1(caux) ";\n\
#pragma omp threadprivate(" PREFIX_JET1_1(faux) "," PREFIX_JET1_1(caux) ")\n\
" \

//  " MY_COEF_MACROS(MY_COEF_FUN,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\n

#define MY_JET1_1_HEADER(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,InitCleanFlag) "\
\n\
" MY_JET1_1_API(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,InitCleanFlag) "\n\
" \

#define MY_JET1_1_POSTHEADER(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END HEADER " PREFIX_JET1_1(t) " */\n" \

/* *******  **************  ******* */
/* *******  MY_JET1_1_CODE  ******* */
/* *******  **************  ******* */
#define MY_JET1_1_PRECODE(PREFIX_JET1_1,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* CODE FOR " PREFIX_JET1_1(t) " */\n\
int * " PREFIX_JET1_1(monomial_counts) "(void) {return _" PREFIX_JET1_1(monomial_counts) "_;}\n\
int * " PREFIX_JET1_1(monomial_offsets) "(void) {return _" PREFIX_JET1_1(monomial_offsets) "_;}\n\
\n" \

#define MY_JET1_1_CODE(PREFIX_JET1_1,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,\
                       MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,InitCleanFlag) "\
" PREFIX_MYFLOAT(t) " " PREFIX_JET1_1(faux) ";\n\
" PREFIX_MYCOEF(t) " " PREFIX_JET1_1(caux) ";\n\
static int " PREFIX_JET1_1(flag_init_jet_library) "=0;\n\
#pragma omp threadprivate(" PREFIX_JET1_1(faux) "," PREFIX_JET1_1(caux) "," PREFIX_JET1_1(flag_init_jet_library) ")\n\
\n\
void " PREFIX_JET1_1(initup2) "(" I " nsymbs, " I " degree) {\n\
  if (" PREFIX_JET1_1(flag_init_jet_library) "==1) return;\n\
  if (nsymbs != 1) {\n\
    fprintf(stderr, \"\%d: Not allowed nsymbs value: \%d!=1\\n\",__LINE__,nsymbs);\n\
    fflush(stderr); exit(1);\n\
  }\n\
  if (degree != 1) {\n\
    fprintf(stderr, \"\%d: Not allowed degree value: \%d!=1\\n\",__LINE__,degree);\n\
    fflush(stderr); exit(1);\n\
  }\n\
  " PREFIX_MYCOEF(initup) "();\n\
  " PREFIX_MYCOEF(init) "(" InitCleanFlag "" PREFIX_JET1_1(caux) ");\n\
  " PREFIX_MYFLOAT(init) "(" PREFIX_JET1_1(faux) ");\n\
  " PREFIX_JET1_1(flag_init_jet_library) "=1;\n\
}\n\
\n\
void " PREFIX_JET1_1(cleanup) "(void) {\n\
  " PREFIX_MYFLOAT(clean) "(" PREFIX_JET1_1(faux) ");\n\
  " PREFIX_MYCOEF(clean)"(" InitCleanFlag "" PREFIX_JET1_1(caux) ");\n\
  " PREFIX_MYCOEF(cleanup)"();\n\
  " PREFIX_JET1_1(flag_init_jet_library) "=0;\n\
}\n\
\n\
static char * " PREFIX_JET1_1(sscanf4) "(char *ptr, const char *myfmt, " PREFIX_JET1_1(t) " s, int *l) {\n\
  if (*ptr !=\'\\0\') {ptr = " PREFIX_MYCOEF(sscanf4) "(ptr, myfmt, s[0], l);}\n\
  if (*ptr !=\'\\0\') {ptr = " PREFIX_MYCOEF(sscanf4) "(ptr, myfmt, s[1], l);}\n\
  return ptr;\n\
}\n\
void " PREFIX_JET1_1(sscanf) "(const char *str, const char *fmt, " PREFIX_JET1_1(t) " s) {\n\
  int l;\n\
  const int len=fmt?strnlen(fmt,128):0; /* NOTE: safety string length of 128 */\n\
  char myfmt[len+3], *mystr = strdup(str);\n\
  sprintf(myfmt, \"\%s%%n\",fmt);\n\
  " PREFIX_JET1_1(sscanf4) "(mystr, myfmt, s, &l);\n\
  free(mystr);\n\
}\n\
\n" \

#define MY_JET1_1_POSTCODE(PREFIX_JET1_1,PREFIX_MYCOEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END CODE " PREFIX_JET1_1(t) " */\n" \

#endif /* MY_JET1_1_H */
