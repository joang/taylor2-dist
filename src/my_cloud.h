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
#ifndef MY_CLOUD_H
#define MY_CLOUD_H

#define MY_CLOUD_BASE "mycloud"
#define MY_CLOUD_HEADER_NAME "MY_CLOUD_H"
#define MY_CLOUD_PREFIX "__MY_CLOUD_PREFIX__"
#define MY_CLOUD_SUFFIX "__MY_CLOUD_SUFFIX__"
#define MY_CLOUD_NAME MY_CLOUD_PREFIX MY_CLOUD_BASE MY_CLOUD_SUFFIX
#define MY_REAL_ENV(x) "myfloat_" #x
#define CLOUD_ENV(x) x
#define MY_CLOUD_ENV(x) MY_CLOUD_NAME "_" #x

//#define MY_CLOUD_MY_COEF_ENV /*ARITH_DOUBLE_COMPLEX_ENV*/
#define MY_CLOUD_BASE_TYPE MY_CLOUD_ENV(t)

#define MY_CLOUD_MY_REAL "double" /*ARITH_DOUBLE_TYPE*/
//#define MY_CLOUD_MY_REAL_ENV /*ARITH_DOUBLE_ENV*/
#define MY_CLOUD_MY_INT "int"

#define MY_CLOUD_MY_CLOUD_FUN_NAME "MY_CLOUD_FUN"
#define MY_CLOUD_MY_CLOUD_FUN(x) MY_CLOUD_MY_CLOUD_FUN_NAME "(" #x ")"
#define MY_CLOUD_MAX_SIZE_MACRO_NAME "_MAX_CLOUD_SIZE_"

#define MY_CLOUD_TYPES(X,mycloud,mycloud_t,myreal,myint) "\
typedef " myreal "* " mycloud_t ";\n\
\n\
typedef " myreal "* " X(ptr) ";\n\
typedef const " myreal "* const " X(src) ";\n\
" \

#define MY_CLOUD_PREHEADER(X,mycloud,mycloud_t,myreal,myint) "\
#include <stdio.h>\n\
" \


#define MY_CLOUD_FUN_GEN(X,NAME,mycloud,mycloud_t,myreal,myint) "\
void " X(NAME) "(" X(ptr) ", " X(src) ");\n\
" \

#define MY_CLOUD_FUN_MYSCAL_GEN(X,NAME,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) ", " myscal ");\n\
" \

#define MY_CLOUD_MYSCAL_FUN_GEN MY_CLOUD_FUN_MYSCAL_GEN

#define MY_CLOUD_FUN2_GEN(X,NAME,mycloud,mycloud_t,myreal,myint) "\
void " X(NAME) "(" X(ptr) ", " X(src) ", " X(src) ");\n\
" \

#define MY_CLOUD_FUN2_MYSCAL_GEN(X,NAME,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) ", " X(src) ", " myscal ");\n\
" \

#define MY_CLOUD_MYSCAL_FUN2_GEN(X,NAME,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) ", " myscal ", " X(src) ");\n\
" \

#define MY_CLOUD_FUN_MYSCAL_GEN(X,NAME,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) ", " myscal ");\n\
" \

#define MY_CLOUD_ALL_FUN_HEADER(X,Xp,mycloud,mycloud_t,myreal,myint,Rp) \
  "/* set functions */\n" \
  MY_CLOUD_FUN_GEN(X,Xp(set),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(set_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(set_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(set_si),mycloud,mycloud_t,myreal,myint,"int") \
  "\n" \
  "\n/* binary arith. functions */\n" \
  MY_CLOUD_FUN2_GEN(X,Xp(add2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN2_GEN(X,Xp(sub2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN2_GEN(X,Xp(mul2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN2_GEN(X,Xp(div2),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(add2_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(sub2_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(mul2_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(div2_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_MYSCAL_FUN2_GEN(X,Xp(myfloat_sub2),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_MYSCAL_FUN2_GEN(X,Xp(myfloat_div2),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  "\n" \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(add2_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(sub2_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(mul2_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(div2_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_MYSCAL_FUN2_GEN(X,Xp(d_sub2),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_MYSCAL_FUN2_GEN(X,Xp(d_div2),mycloud,mycloud_t,myreal,myint,"double") \
  "\n" \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(add2_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(sub2_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(mul2_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(div2_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_MYSCAL_FUN2_GEN(X,Xp(si_sub2),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_MYSCAL_FUN2_GEN(X,Xp(si_div2),mycloud,mycloud_t,myreal,myint,"int") \
  "\n/* unary assign arith. functions */\n" \
  MY_CLOUD_FUN_GEN(X,Xp(neg),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(add),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(sub),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(mul),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(div),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(add_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(sub_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(mul_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(div_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_MYSCAL_FUN_GEN(X,Xp(myfloat_sub),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_MYSCAL_FUN_GEN(X,Xp(myfloat_div),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  "\n" \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(add_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(sub_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(mul_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(div_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_MYSCAL_FUN_GEN(X,Xp(d_sub),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_MYSCAL_FUN_GEN(X,Xp(d_div),mycloud,mycloud_t,myreal,myint,"double") \
  "\n" \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(add_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(sub_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(mul_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_FUN_MYSCAL_GEN(X,Xp(div_si),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_MYSCAL_FUN_GEN(X,Xp(si_sub),mycloud,mycloud_t,myreal,myint,"int") \
  MY_CLOUD_MYSCAL_FUN_GEN(X,Xp(si_div),mycloud,mycloud_t,myreal,myint,"int") \
  "\n/* elemental functions */\n" \
  MY_CLOUD_FUN2_MYSCAL_GEN(X,Xp(pow_myfloat),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_GEN(X,Xp(sqrt),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(sin),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(cos),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(tan),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(exp),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(log),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(sinh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(cosh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(tanh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_GEN(X,Xp(atan),mycloud,mycloud_t,myreal,myint) \

#define MY_CLOUD_HEADER(X,mycloud,mycloud_t,myreal,myint,CLOUD_ENV,\
  REAL_ENV,MY_CLOUD_ALL_FUN_HEADER) "\
" MY_CLOUD_TYPES(X,mycloud,mycloud_t,myreal,myint) "\n\
\n\
extern " myint " " X(max_size) ", " X(current_size) ";\n\
#pragma omp threadprivate(" X(current_size) ")\n\
\n\
/* init/clean functions */\n\
void " X(initup) "(" myint ");\n\
void " X(cleanup) "(void);\n\
\n\
size_t " X(init3) "(" X(ptr) " *, " myint ", " myreal " **);\n\
#define " X(init2) "(p,n) " X(init3) "(p,n,NULL)\n\
#define " X(init) "(p) " X(init3) "(p," X(max_size) ",NULL)\n\
\n\
void " X(clean) "(" X(ptr) " *);\n\
\n\
/* basic functions */\n\
" myint " " X(set_current_size) "(" myint ");\n\
//" myint " " X(get_current_size) "(void);\n\
#define " X(get_current_size) " " X(current_size) "\n\
\n\
#define " X(size) "(p) " X(current_size) "\n\
//" myint " " X(size) "(" X(src) ");\n\
\n\
#define " X(get) "(p,idx) ((p)+(idx))\n\
//" myreal "* " X(get) "(" X(ptr) ", " myint ");\n\
\n\
/* norms */\n\
void " X(nrm1) "(" myreal "[1], " X(src) ");\n\
void " X(nrm2) "(" myreal "[1], " X(src) ");\n\
void " X(nrminf) "(" myreal "[1], " X(src) ");\n\
void " X(minabs) "(" myreal "[1], " X(src) "); /* minimum in absolute value */\n\
\n\
\n\n\
/* I/O functions */\n\
void " X(fscanf) "(FILE *, const char *, " X(ptr) ");\n\
void " X(fprintf) "(FILE *, const char *, " X(src) ");\n\
#define " X(printf) "(fmt,p) " X(fprintf) "(stdout,fmt,p)\n\
\n\
\n\n\
/* elemental functions */\n\
" MY_CLOUD_ALL_FUN_HEADER(X,CLOUD_ENV,mycloud,mycloud_t,myreal,myint,REAL_ENV) "\
\n\
" \

/********************************************************
 * CODE
 ********************************************************/

#define MY_CLOUD_PRECODE(X,mycloud,mycloud_t,myreal,myint) "\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
\n" \

#define MY_CLOUD_FUN_CODE_GEN(X,NAME,FUN,mycloud,mycloud_t,myreal,myint) "\
void " X(NAME) "(" X(ptr) " p, " X(src) " x)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" FUN "(p[k], x[k]);}\n\
}\n" \

#define MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,NAME,FUN,mycloud,mycloud_t,myreal,myint) "\
#define " X(NAME) "(p, x)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" FUN "((p)[" X(_k_) "], (x)[" X(_k_) "]);}\\\n\
}\n" \


#define MY_CLOUD_FUN_MYSCAL_CODE_GEN(X,NAME,FUN,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) " p, " myscal " b)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" FUN "(p[k], b);}\n\
}\n" \

#define MY_CLOUD_FUN_MYSCAL_CODE_GEN_BY_MACROS(X,NAME,FUN,mycloud,mycloud_t,myreal,myint,myscal) "\
#define " X(NAME) "(p, b)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" FUN "((p)[" X(_k_) "], b);}\\\n\
}\n" \


#define MY_CLOUD_FUN2_MYSCAL_CODE_GEN(X,NAME,FUN,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) " p, " X(src) " x, " myscal " b)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" FUN "(p[k], x[k], b);}\n\
}\n" \

#define MY_CLOUD_FUN2_MYSCAL_CODE_GEN_BY_MACROS(X,NAME,FUN,mycloud,mycloud_t,myreal,myint,myscal) "\
#define " X(NAME) "(p, x, b)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" FUN "((p)[" X(_k_) "], (x)[" X(_k_) "], b);}\\\n\
}\n" \


#define MY_CLOUD_ARITH_FUN2_GEN_CODE(X,NAME,OP,mycloud,mycloud_t,myreal,myint) "\
void " X(NAME) "(" X(ptr) " p, " X(src) " a, " X(src) " b)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" OP "(p[k], a[k], b[k]);}\n\
}\n" \

#define MY_CLOUD_ARITH_FUN2_GEN_CODE_BY_MACROS(X,NAME,OP,mycloud,mycloud_t,myreal,myint) "\
#define " X(NAME) "(p, a, b)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" OP "((p)[" X(_k_) "], (a)[" X(_k_) "], (b)[" X(_k_) "]);}\\\n\
}\n" \


#define MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) " p, " X(src) " a, " myscal " b)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" OP "(p[k], a[k], b);}\n\
}\n" \

#define MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
#define " X(NAME) "(p, a, b)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" OP "((p)[" X(_k_) "], (a)[" X(_k_) "], b);}\\\n\
}\n" \


#define MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) " p, " myscal " b, " X(src) " a)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" OP "(p[k], b, a[k]);}\n\
}\n" \

#define MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
#define " X(NAME) "(p, b, a)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" OP "((p)[" X(_k_) "], b, (a)[" X(_k_) "]);}\\\n\
}\n"


#define MY_CLOUD_ARITH_FUN_GEN_CODE(X,NAME,OP,mycloud,mycloud_t,myreal,myint) "\
void " X(NAME) "(" X(ptr) " p, " X(src) " a)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" OP "(p[k], p[k], a[k]);}\n\
}\n" \

#define MY_CLOUD_ARITH_FUN_GEN_CODE_BY_MACROS(X,NAME,OP,mycloud,mycloud_t,myreal,myint) "\
#define " X(NAME) "(p, a)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" OP "((p)[" X(_k_) "], (p)[" X(_k_) "], (a)[" X(_k_) "]);}\\\n\
}\n" \


#define MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) " p, " myscal " a)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" OP "(p[k], p[k], a);}\n\
}\n" \

#define MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
#define " X(NAME) "(p, a)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" OP "((p)[" X(_k_) "], (p)[" X(_k_) "], (a));}\\\n\
}\n" \


#define MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
void " X(NAME) "(" X(ptr) " p, " myscal " a)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) {" OP "(p[k], a, p[k]);}\n\
}\n" \

#define MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,NAME,OP,mycloud,mycloud_t,myreal,myint,myscal) "\
#define " X(NAME) "(p, a)\\\n\
{\\\n\
  " myint " " X(_k_) ";\\\n\
  for (" X(_k_) " = 0; " X(_k_) " < " X(current_size) "; ++" X(_k_) ") {" OP "((p)[" X(_k_) "], (a), (p)[" X(_k_) "]);}\\\n\
}\n" \

#define MY_CLOUD_ALL_FUN_CODE(X,Xp,mycloud,mycloud_t,myreal,myint,Rp) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(set),Rp(set),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_MYSCAL_CODE_GEN(X,Xp(set_myfloat),Rp(set),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_MYSCAL_CODE_GEN(X,Xp(set_d),Rp(set_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN_MYSCAL_CODE_GEN(X,Xp(set_si),Rp(set_si),mycloud,mycloud_t,myreal,myint,"int") \
  "\n" \
  MY_CLOUD_ARITH_FUN2_GEN_CODE(X,Xp(add2),Rp(add2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN2_GEN_CODE(X,Xp(sub2),Rp(sub2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN2_GEN_CODE(X,Xp(mul2),Rp(mul2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN2_GEN_CODE(X,Xp(div2),Rp(div2),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(add2_myfloat),Rp(add2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(sub2_myfloat),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(mul2_myfloat),Rp(mul2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(div2_myfloat),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,Xp(myfloat_sub2),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,Xp(myfloat_div2),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  "\n" \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(add2_d),Rp(add2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(sub2_d),Rp(sub2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(mul2_d),Rp(mul2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(div2_d),Rp(div2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,Xp(d_sub2),Rp(d_sub2),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,Xp(d_div2),Rp(d_div2),mycloud,mycloud_t,myreal,myint, "double") \
  "\n" \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(add2_si),Rp(add2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(sub2_si),Rp(sub2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(mul2_si),Rp(mul2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE(X,Xp(div2_si),Rp(div2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,Xp(si_sub2),Rp(si_sub2),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE(X,Xp(si_div2),Rp(si_div2),mycloud,mycloud_t,myreal,myint, "int") \
  "\n\n" \
  MY_CLOUD_ARITH_FUN_GEN_CODE(X,Xp(add),Rp(add2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN_GEN_CODE(X,Xp(sub),Rp(sub2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN_GEN_CODE(X,Xp(mul),Rp(mul2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN_GEN_CODE(X,Xp(div),Rp(div2),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(add_myfloat),Rp(add2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(sub_myfloat),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(mul_myfloat),Rp(mul2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(div_myfloat),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,Xp(myfloat_sub),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,Xp(myfloat_div),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  "\n" \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(add_d),Rp(add2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(sub_d),Rp(sub2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(mul_d),Rp(mul2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(div_d),Rp(div2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,Xp(d_sub),Rp(d_sub2),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,Xp(d_div),Rp(d_div2),mycloud,mycloud_t,myreal,myint, "double") \
  "\n" \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(add_si),Rp(add2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(sub_si),Rp(sub2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(mul_si),Rp(mul2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE(X,Xp(div_si),Rp(div2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,Xp(si_sub),Rp(si_sub2),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE(X,Xp(si_div),Rp(si_div2),mycloud,mycloud_t,myreal,myint, "int") \
  "\n\n" \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(neg),Rp(neg),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_FUN2_MYSCAL_CODE_GEN(X,Xp(pow_myfloat),Rp(pow),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  "\n" \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(sqrt),Rp(sqrt),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(sin),Rp(sin),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(cos),Rp(cos),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(tan),Rp(tan),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(exp),Rp(exp),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(log),Rp(log),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(sinh),Rp(sinh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(cosh),Rp(cosh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(tanh),Rp(tanh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN(X,Xp(atan),Rp(atan),mycloud,mycloud_t,myreal,myint) \


#define MY_CLOUD_ALL_FUN_CODE_BY_MACROS(X,Xp,mycloud,mycloud_t,myreal,myint,Rp) ""

#define MY_CLOUD_ALL_FUN_HEADER_BY_MACROS(X,Xp,mycloud,mycloud_t,myreal,myint,Rp) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(set),Rp(set),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_MYSCAL_CODE_GEN_BY_MACROS(X,Xp(set_myfloat),Rp(set),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  MY_CLOUD_FUN_MYSCAL_CODE_GEN_BY_MACROS(X,Xp(set_d),Rp(set_d),mycloud,mycloud_t,myreal,myint,"double") \
  MY_CLOUD_FUN_MYSCAL_CODE_GEN_BY_MACROS(X,Xp(set_si),Rp(set_si),mycloud,mycloud_t,myreal,myint,"int") \
  "\n" \
  MY_CLOUD_ARITH_FUN2_GEN_CODE_BY_MACROS(X,Xp(add2),Rp(add2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN2_GEN_CODE_BY_MACROS(X,Xp(sub2),Rp(sub2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN2_GEN_CODE_BY_MACROS(X,Xp(mul2),Rp(mul2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN2_GEN_CODE_BY_MACROS(X,Xp(div2),Rp(div2),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(add2_myfloat),Rp(add2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(sub2_myfloat),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(mul2_myfloat),Rp(mul2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(div2_myfloat),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,Xp(myfloat_sub2),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,Xp(myfloat_div2),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  "\n" \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(add2_d),Rp(add2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(sub2_d),Rp(sub2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(mul2_d),Rp(mul2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(div2_d),Rp(div2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,Xp(d_sub2),Rp(d_sub2),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,Xp(d_div2),Rp(d_div2),mycloud,mycloud_t,myreal,myint, "double") \
  "\n" \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(add2_si),Rp(add2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(sub2_si),Rp(sub2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(mul2_si),Rp(mul2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN2_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(div2_si),Rp(div2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,Xp(si_sub2),Rp(si_sub2),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN2_GEN_CODE_BY_MACROS(X,Xp(si_div2),Rp(si_div2),mycloud,mycloud_t,myreal,myint, "int") \
  "\n\n" \
  MY_CLOUD_ARITH_FUN_GEN_CODE_BY_MACROS(X,Xp(add),Rp(add2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN_GEN_CODE_BY_MACROS(X,Xp(sub),Rp(sub2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN_GEN_CODE_BY_MACROS(X,Xp(mul),Rp(mul2),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_ARITH_FUN_GEN_CODE_BY_MACROS(X,Xp(div),Rp(div2),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(add_myfloat),Rp(add2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(sub_myfloat),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(mul_myfloat),Rp(mul2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(div_myfloat),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,Xp(myfloat_sub),Rp(sub2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,Xp(myfloat_div),Rp(div2),mycloud,mycloud_t,myreal,myint, "MY_FLOAT") \
  "\n" \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(add_d),Rp(add2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(sub_d),Rp(sub2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(mul_d),Rp(mul2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(div_d),Rp(div2_d),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,Xp(d_sub),Rp(d_sub2),mycloud,mycloud_t,myreal,myint, "double") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,Xp(d_div),Rp(d_div2),mycloud,mycloud_t,myreal,myint, "double") \
  "\n" \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(add_si),Rp(add2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(sub_si),Rp(sub2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(mul_si),Rp(mul2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_FUN_MYSCAL_GEN_CODE_BY_MACROS(X,Xp(div_si),Rp(div2_si),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,Xp(si_sub),Rp(si_sub2),mycloud,mycloud_t,myreal,myint, "int") \
  MY_CLOUD_ARITH_MYSCAL_FUN_GEN_CODE_BY_MACROS(X,Xp(si_div),Rp(si_div2),mycloud,mycloud_t,myreal,myint, "int") \
  "\n\n" \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(neg),Rp(neg),mycloud,mycloud_t,myreal,myint) \
  "\n" \
  MY_CLOUD_FUN2_MYSCAL_CODE_GEN_BY_MACROS(X,Xp(pow_myfloat),Rp(pow),mycloud,mycloud_t,myreal,myint,"MY_FLOAT") \
  "\n" \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(sqrt),Rp(sqrt),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(sin),Rp(sin),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(cos),Rp(cos),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(tan),Rp(tan),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(exp),Rp(exp),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(log),Rp(log),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(sinh),Rp(sinh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(cosh),Rp(cosh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(tanh),Rp(tanh),mycloud,mycloud_t,myreal,myint) \
  MY_CLOUD_FUN_CODE_GEN_BY_MACROS(X,Xp(atan),Rp(atan),mycloud,mycloud_t,myreal,myint) \



#define MY_CLOUD_CODE(X,mycloud,mycloud_t,myreal,myint,CLOUD_ENV,REAL_ENV,\
     MY_CLOUD_ALL_FUN_CODE) "\
\n\
#define " X(alloc) "(n,x) x = (__typeof__(x)) malloc((n)*sizeof(*(x)))\n\
#define " X(dealloc) "(x) if (x) free(x), x=NULL\n\
\n\
" myint " " X(max_size) "=0, " X(current_size) "=0, " X(flag_init) "=0;\n\
#pragma omp threadprivate(" X(current_size) ")\n\
#define " X(SIZE) "(x) " X(current_size) "\n\
\n\
void " X(initup) "(" myint " maxn)\n\
{\n\
  if (" X(flag_init) "==0) {" X(max_size) " = " X(current_size) " = maxn; " X(flag_init) "=1;}\n\
}\n\
\n\
\n\
void " X(cleanup) "(void)\n\
{\n\
  if (" X(flag_init) "==1) {" X(max_size) " = " X(current_size) " = 0; " X(flag_init) "=0;}\n\
}\n\
\n\
size_t " X(init3) "(" X (ptr) " *p, " myint " N, " myreal " **c)\n\
{\n\
  size_t mem=0;\n\
  " myint " k;\n\
\n\
  if (N != " X(current_size) ")\n\
    {\n\
      fprintf(stderr, \"\%s:\%d currentsize \%d != \%d\\n\", __FILE__, __LINE__, " X(current_size) ", N); fflush(stderr); exit(10);\n\
    }\n\
  if (c) { *p = *c; }\n\
  else\n\
    {\n\
      " X(alloc) "(N, *p);\n\
      if (*p == NULL)\n\
        {\n\
          fprintf(stderr, \"\%s:\%d not enough memory\", __FILE__, __LINE__); fflush(stderr); exit(20);\n\
        }\n\
      mem+= N*sizeof(*(*p));\n\
      for (k = 0; k < N; ++k) {" REAL_ENV(init) "((*p)[k]);}\n\
    }\n\
  return mem;\n\
}\n\
//size_t " X(init) "(" X (ptr) " *p)\n\
//{ return " X(init3) "(p, " X(max_size) ", NULL); }\n\
\n\
void " X(clean) "(" X (ptr) " *p)\n\
{\n\
  " myint " k, n = " X(max_size) ";\n\
  for (k = 0; k < n; ++k) {" REAL_ENV(clean) "((*p)[k]);}\n\
  " X(dealloc) "(*p);\n\
}\n\
\n\
" myint " " X(set_current_size) "(" myint " n)\n\
{\n\
  " myint " old = " X(current_size) ";\n\
  " X(current_size) " = n;\n\
  return old;\n\
}\n\
//" myint " " X(get_current_size) "(void) { return " X(current_size) "; }\n\
\n\
//" myint " " X(size) "(" X (src) " p) { return " X(SIZE) "(p); }\n\
\n\
//" myreal "* " X(get) "(" X(ptr) " p, " myint " k) { return p+k; }\n\
\n\
void " X(nrm1) "(" myreal " r[1], " X(src) " p)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  " myreal " tmp;\n\
  " REAL_ENV(init) "(tmp);\n\
  " REAL_ENV(set_si) "(*r, 0);\n\
  for (k = 0; k < n; ++k)\n\
    {\n\
      " REAL_ENV(fabs) "(tmp, p[k]);\n\
      " REAL_ENV(add2) "(*r, *r, tmp);\n\
    }\n\
  " REAL_ENV(div2_si) "(*r, *r, n);\n\
  " REAL_ENV(clean) "(tmp);\n\
}\n\
void " X(nrm2) "(" myreal " r[1], " X(src) " p)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  " myreal " tmp;\n\
  " REAL_ENV(init) "(tmp);\n\
  " REAL_ENV(set_si) "(*r, 0);\n\
  for (k = 0; k < n; ++k)\n\
    {\n\
      " REAL_ENV(mul2) "(tmp, p[k], p[k]);\n\
      " REAL_ENV(add2) "(*r, *r, tmp);\n\
    }\n\
  " REAL_ENV(div2_si) "(tmp, *r, n);\n\
  " REAL_ENV(sqrt) "(*r, tmp);\n\
  " REAL_ENV(clean) "(tmp);\n\
}\n\
void " X(nrminf) "(" myreal " r[1], " X(src) " p)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  " myreal " tmp;\n\
  " REAL_ENV(init) "(tmp);\n\
  " REAL_ENV(set_si) "(*r, 0);\n\
  for (k = 0; k < n; ++k)\n\
    {\n\
      " REAL_ENV(fabs) "(tmp, p[k]);\n\
      if (" REAL_ENV(lt) "(*r, tmp)) {" REAL_ENV(set) "(*r, tmp);}\n\
    }\n\
  " REAL_ENV(clean) "(tmp);\n\
}\n\
void " X(minabs) "(" myreal " r[1], " X(src) " p)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  " myreal " tmp;\n\
  " REAL_ENV(init) "(tmp);\n\
  " REAL_ENV(fabs) "(*r, p[0]);\n\
   for (k = 1; k < n; ++k)\n\
    {\n\
      " REAL_ENV(fabs) "(tmp, p[k]);\n\
      if (" REAL_ENV(lt) "(tmp, *r)) {" REAL_ENV(set) "(*r, tmp);}\n\
    }\n\
  " REAL_ENV(clean) "(tmp);\n\
}\n\
\n\
/* elemental functions */\n\
" MY_CLOUD_ALL_FUN_CODE(X,CLOUD_ENV,mycloud,mycloud_t,myreal,myint,REAL_ENV) "\n\
\n\
\n\
void " X(fscanf) "(FILE *file, const char *fmt, " X(ptr) " p)\n\
{\n\
  fprintf(stderr, \"\%s:\%d to implement\\n\", __FILE__, __LINE__); fflush(stderr); exit(20);\n\
}\n\
void " X(fprintf) "(FILE *file, const char *fmt, " X(src) " p)\n\
{\n\
  " myint " k, n = " X(SIZE) "(p);\n\
  for (k = 0; k < n; ++k) { " REAL_ENV(fprintf3) "(file, fmt, p[k]); }\n\
}\n\
" \

#endif /* MY_CLOUD_H */
