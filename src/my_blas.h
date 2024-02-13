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
#ifndef MY_BLAS_H
#define MY_BLAS_H

#define MY_BLAS_HEADER(NAME,myjet_t,mycoef_t,myfloat_t,int_t) "\
//" myjet_t "* " NAME(alloc) "(" int_t " n);\n\
//void " NAME(free) "(" myjet_t " *x);\n\
\n\
// level 1 blas\n\
void " NAME(copy) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy); // y <- x\n\
void " NAME(axpy) "(" int_t " n, " myfloat_t " a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy); // y <- a*x + y\n\
void " NAME(scal) "(" int_t " n, " myfloat_t " a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy); // y <- a*x\n\
void " NAME(axpy_d) "(" int_t " n, double a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
void " NAME(scal_d) "(" int_t " n, double a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
void " NAME(swap) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy); // y <-> x\n\
" myfloat_t " " NAME(dot) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
" myfloat_t " " NAME(nrm2) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
" myfloat_t " " NAME(asum) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
int " NAME(idmax) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
\n\
// coordinatewise operations\n\
 // z <- x.op.y\n\
void " NAME(add2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
void " NAME(sub2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
void " NAME(mul2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
void " NAME(div2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy);\n\
\n\
 // elemental operations\n\
void " NAME(pow_myfloat) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myfloat_t " alph);\n\
void " NAME(sqrt) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(exp) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(log) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(sin) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(cos) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(tan) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(sinh) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
void " NAME(cosh) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx);\n\
" \

#define MY_BLAS_CODE(NAME,myjet_t,JNAME,mycoef_t,CNAME,myfloat_t,FNAME,int_t,jaux) "\
//" myjet_t "* " NAME(alloc) "(" int_t " n);\n\
//void " NAME(free) "(" myjet_t " *x);\n\
\n\
// level 1 blas\n\
void " NAME(copy) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
{\n\
  " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set) "(y[i*incy], x[i*incx]);}\n\
}\n\
void " NAME(axpy) "(" int_t " n, " myfloat_t " a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
//   fprintf(stderr, \"\%s:\%d to test\\n\", __FILE__, __LINE__); fflush(stderr); exit(33);\n\
   " int_t " i; extern " myjet_t " " jaux ";\n\
   for (i = 0; i < n; ++i) {" JNAME(mul2_myfloat) "(" jaux ", x[i*incx], a); " JNAME(add2) "(y[i*incy], " jaux ", y[i*incy]);}\n\
 }\n\
void " NAME(scal) "(" int_t " n, " myfloat_t " a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(mul2_myfloat) "(y[i*incy], x[i*incx], a);}\n\
 }\n\
void " NAME(axpy_d) "(" int_t " n, double a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; extern " myjet_t " " jaux ";\n\
   for (i = 0; i < n; ++i) {" JNAME(mul2_d) "(" jaux ", x[i*incx], a); " JNAME(add2) "(y[i*incy], " jaux ", y[i*incy]);}\n\
 }\n\
void " NAME(scal_d) "(" int_t " n, double a, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(mul2_d) "(y[i*incy], x[i*incx], a);}\n\
 }\n\
void " NAME(swap) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; extern " myjet_t " " jaux ";\n\
   for (i = 0; i < n; ++i) {" JNAME(set) "(" jaux ", x[i*incx]); " JNAME(set) "(x[i*incx], y[i*incy]); " JNAME(set) "(y[i*incy], " jaux "); }\n\
 }\n\
" myfloat_t " " NAME(dot) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   fprintf(stderr, \"\%s:\%d to implement\\n\", __FILE__, __LINE__); fflush(stderr); exit(33);\n\
 }\n\
" myfloat_t " " NAME(nrm2) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   fprintf(stderr, \"\%s:\%d to implement\\n\", __FILE__, __LINE__); fflush(stderr); exit(33);\n\
 }\n\
" myfloat_t " " NAME(asum) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   fprintf(stderr, \"\%s:\%d to implement\\n\", __FILE__, __LINE__); fflush(stderr); exit(33);\n\
 }\n\
int " NAME(idmax) "(" int_t " n, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   fprintf(stderr, \"\%s:\%d to implement\\n\", __FILE__, __LINE__); fflush(stderr); exit(33);\n\
 }\n\
\n\
/* coordinatewise operations */\n\
void " NAME(add2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(add2) "(z[i*incz], x[i*incx], y[i*incy]);}\n\
 }\n\
void " NAME(sub2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(sub2) "(z[i*incz], x[i*incx], y[i*incy]);}\n\
 }\n\
void " NAME(mul2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(mul2) "(z[i*incz], x[i*incx], y[i*incy]);}\n\
 }\n\
void " NAME(div2) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myjet_t " y[], " int_t " incy)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(div2) "(z[i*incz], x[i*incx], y[i*incy]);}\n\
 }\n\
\n\
/* elemental operations */\n\
void " NAME(pow_myfloat) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx, " myfloat_t " alph)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_pow_myfloat) "(z[i*incz], x[i*incx], alph);}\n\
 }\n\
void " NAME(sqrt) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_sqrt) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(exp) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_exp) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(log) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_log) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(sin) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_sin) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(cos) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_cos) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(tan) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_tan) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(sinh) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_sinh) "(z[i*incz], x[i*incx]);}\n\
 }\n\
void " NAME(cosh) "(" int_t " n, " myjet_t " z[], " int_t " incz, " myjet_t " x[], " int_t " incx)\n\
 {\n\
   " int_t " i; for (i = 0; i < n; ++i) {" JNAME(set_cosh) "(z[i*incz], x[i*incx]);}\n\
 }\n\
" \

#define MY_BLAS_PREFIX "__BLAS_NAME_"
#define MY_BLAS_SUFFIX "_BLAS_ODE_NAME__"
#define MY_BLAS_SPACE(x) MY_BLAS_PREFIX "_" #x "_" MY_BLAS_SUFFIX
#define MY_BLAS_FUN(x) "MY_BLAS_FUN(" #x ")"

#endif /* MY_BLAS_H */
