/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
 *    Copyright (C) 2022 Joan Gimeno, Angel Jorba, Maorong Zou
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
#ifndef CPPWRAPPER
#define CPPWRAPPER

#define CPP_NONE_WRAPPER(CPP_BASE_NAME,BASE_TYPE,BASE_PREFIX,MY_COEF) ""

#define CPP_MY_FLOAT_WRAPPER(CPP_BASE_NAME,BASE_TYPE,BASE_PREFIX,MY_FLOAT) "\
" CPP_BASE_NAME "& operator=(const " MY_FLOAT "& f) {" BASE_PREFIX(set_myfloat) "(x,f); return *this;}\n\
\n\
" CPP_BASE_NAME " operator+(const " MY_FLOAT "& f) const {" CPP_BASE_NAME " z; " BASE_PREFIX(add2_myfloat) "(z.x,x,f); return z;}\n\
" CPP_BASE_NAME " operator-(const " MY_FLOAT "& f) const {" CPP_BASE_NAME " z; " BASE_PREFIX(sub2_myfloat) "(z.x,x,f); return z;}\n\
" CPP_BASE_NAME " operator*(const " MY_FLOAT "& f) const {" CPP_BASE_NAME " z; " BASE_PREFIX(mul2_myfloat) "(z.x,x,f); return z;}\n\
" CPP_BASE_NAME " operator/(const " MY_FLOAT "& f) const {" CPP_BASE_NAME " z; " BASE_PREFIX(div2_myfloat) "(z.x,x,f); return z;}\n\
\n\
" CPP_BASE_NAME "& operator+=(const " MY_FLOAT "& f) {" BASE_PREFIX(add2_myfloat) "(x,x,f); return *this;}\n\
" CPP_BASE_NAME "& operator-=(const " MY_FLOAT "& f) {" BASE_PREFIX(sub2_myfloat) "(x,x,f); return *this;}\n\
" CPP_BASE_NAME "& operator*=(const " MY_FLOAT "& f) {" BASE_PREFIX(mul2_myfloat) "(x,x,f); return *this;}\n\
" CPP_BASE_NAME "& operator/=(const " MY_FLOAT "& f) {" BASE_PREFIX(div2_myfloat) "(x,x,f); return *this;}\n\
" \

#define CPP_MY_COEF_WRAPPER(CPP_BASE_NAME,BASE_TYPE,BASE_PREFIX,MY_COEF) "\
" CPP_BASE_NAME "& operator=(" MY_COEF "& c) {" BASE_PREFIX(set_coef) "(x,c); return *this;}\n\
\n\
" CPP_BASE_NAME " operator+(" MY_COEF "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(add2_coef) "(z.x,x,y); return z;}\n\
" CPP_BASE_NAME " operator-(" MY_COEF "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(sub2_coef) "(z.x,x,y); return z;}\n\
" CPP_BASE_NAME " operator*(" MY_COEF "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(mul2_coef) "(z.x,x,y); return z;}\n\
" CPP_BASE_NAME " operator/(" MY_COEF "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(div2_coef) "(z.x,x,y); return z;}\n\
\n\
" CPP_BASE_NAME "& operator+=(" MY_COEF "& c) {" BASE_PREFIX(add2_coef) "(x,x,c); return *this;}\n\
" CPP_BASE_NAME "& operator-=(" MY_COEF "& c) {" BASE_PREFIX(sub2_coef) "(x,x,c); return *this;}\n\
" CPP_BASE_NAME "& operator*=(" MY_COEF "& c) {" BASE_PREFIX(mul2_coef) "(x,x,c); return *this;}\n\
" CPP_BASE_NAME "& operator/=(" MY_COEF "& c) {" BASE_PREFIX(div2_coef) "(x,x,c); return *this;}\n\
" \

#define CPP_WRAPPER(CPP_BASE_NAME,BASE_TYPE,BASE_PREFIX,\
  CPP_MY_COEF_WRAPPER,MY_COEF,\
  CPP_MY_FLOAT_WRAPPER,MY_FLOAT) "\
#ifdef __cplusplus\n\
#include <cstdio>\n\
namespace " CPP_BASE_NAME "Lib {\n\
  struct " CPP_BASE_NAME " {\n\
    " BASE_TYPE " x; //public attribute\n\
    //typedef " CPP_BASE_NAME "& " CPP_BASE_NAME "_ref;\n\
    //typedef const " CPP_BASE_NAME "& " CPP_BASE_NAME "_srcref;\n\
    \n\
    " CPP_BASE_NAME "(void) {InitUpJet(); " BASE_PREFIX(init) "(&x);}\n\
    " CPP_BASE_NAME "(const " CPP_BASE_NAME "& y) {" BASE_PREFIX(init) "(&x); " BASE_PREFIX(set) "(x,y.x);}\n\
    ~" CPP_BASE_NAME "(void) {" BASE_PREFIX(clean) "(&x);}\n\
    \n\
  \n\
  \n\
    " CPP_BASE_NAME "& operator=(const " CPP_BASE_NAME "& y) {" BASE_PREFIX(set) "(x,y.x); return *this;}\n\
    \n\
    " CPP_BASE_NAME " operator+(const " CPP_BASE_NAME "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(add2) "(z.x,x,y.x); return z;}\n\
    " CPP_BASE_NAME " operator-(const " CPP_BASE_NAME "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(sub2) "(z.x,x,y.x); return z;}\n\
    " CPP_BASE_NAME " operator*(const " CPP_BASE_NAME "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(mul2) "(z.x,x,y.x); return z;}\n\
    " CPP_BASE_NAME " operator/(const " CPP_BASE_NAME "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(div2) "(z.x,x,y.x); return z;}\n\
    \n\
    const " CPP_BASE_NAME "& operator+=(const " CPP_BASE_NAME "& y) {" BASE_PREFIX(add2) "(x,x,y.x); return *this;}\n\
    const " CPP_BASE_NAME "& operator-=(const " CPP_BASE_NAME "& y) {" BASE_PREFIX(sub2) "(x,x,y.x); return *this;}\n\
    const " CPP_BASE_NAME "& operator*=(const " CPP_BASE_NAME "& y) {extern " BASE_TYPE " " BASE_PREFIX(jaux) "; " BASE_PREFIX(set) "(x," BASE_PREFIX(jaux) "); " BASE_PREFIX(mul2) "(x," BASE_PREFIX(jaux) ",y.x); return *this;}\n\
    const " CPP_BASE_NAME "& operator/=(const " CPP_BASE_NAME "& y) {extern " BASE_TYPE " " BASE_PREFIX(jaux) "; " BASE_PREFIX(set) "(x," BASE_PREFIX(jaux) "); " BASE_PREFIX(div2) "(x," BASE_PREFIX(jaux) ",y.x); return *this;}\n\
  \n\
  \n\
    " CPP_BASE_NAME "& operator=(const " BASE_TYPE "& y) {" BASE_PREFIX(set) "(x,y); return *this;}\n\
    \n\
    " CPP_BASE_NAME " operator+(const " BASE_TYPE "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(add2) "(z.x,x,y); return z;}\n\
    " CPP_BASE_NAME " operator-(const " BASE_TYPE "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(sub2) "(z.x,x,y); return z;}\n\
    " CPP_BASE_NAME " operator*(const " BASE_TYPE "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(mul2) "(z.x,x,y); return z;}\n\
    " CPP_BASE_NAME " operator/(const " BASE_TYPE "& y) const {" CPP_BASE_NAME " z; " BASE_PREFIX(div2) "(z.x,x,y); return z;}\n\
    \n\
    const " CPP_BASE_NAME "& operator+=(const " BASE_TYPE "& y) {" BASE_PREFIX(add2) "(x,x,y); return *this;}\n\
    const " CPP_BASE_NAME "& operator-=(const " BASE_TYPE "& y) {" BASE_PREFIX(sub2) "(x,x,y); return *this;}\n\
    const " CPP_BASE_NAME "& operator*=(const " BASE_TYPE "& y) {extern " BASE_TYPE " " BASE_PREFIX(jaux) "; " BASE_PREFIX(set) "(x," BASE_PREFIX(jaux) "); " BASE_PREFIX(mul2) "(x," BASE_PREFIX(jaux) ",y); return *this;}\n\
    const " CPP_BASE_NAME "& operator/=(const " BASE_TYPE "& y) {extern " BASE_TYPE " " BASE_PREFIX(jaux) "; " BASE_PREFIX(set) "(x," BASE_PREFIX(jaux) "); " BASE_PREFIX(div2) "(x," BASE_PREFIX(jaux) ",y); return *this;}\n\
  \n\
  \n\
    " CPP_MY_COEF_WRAPPER(CPP_BASE_NAME,BASE_TYPE,BASE_PREFIX,MY_COEF) " \
  \n\
    " CPP_MY_FLOAT_WRAPPER(CPP_BASE_NAME,BASE_TYPE,BASE_PREFIX,MY_FLOAT) " \
  \n\
  \n\
    " CPP_BASE_NAME "& operator=(double d) {" BASE_PREFIX(set_d) "(x,d); return *this;}\n\
    \n\
    " CPP_BASE_NAME " operator+(double d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(add2_d) "(z.x,x,d); return z;}\n\
    " CPP_BASE_NAME " operator-(double d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(sub2_d) "(z.x,x,d); return z;}\n\
    " CPP_BASE_NAME " operator*(double d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(mul2_d) "(z.x,x,d); return z;}\n\
    " CPP_BASE_NAME " operator/(double d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(div2_d) "(z.x,x,d); return z;}\n\
    \n\
    const " CPP_BASE_NAME "& operator+=(double d) {" BASE_PREFIX(add2_d) "(x,x,d); return *this;}\n\
    const " CPP_BASE_NAME "& operator-=(double d) {" BASE_PREFIX(sub2_d) "(x,x,d); return *this;}\n\
    const " CPP_BASE_NAME "& operator*=(double d) {" BASE_PREFIX(mul2_d) "(x,x,d); return *this;}\n\
    const " CPP_BASE_NAME "& operator/=(double d) {" BASE_PREFIX(div2_d) "(x,x,d); return *this;}\n\
  \n\
  \n\
    const " CPP_BASE_NAME "& operator=(int d) {" BASE_PREFIX(set_si) "(x,d); return *this;}\n\
    \n\
    " CPP_BASE_NAME " operator+(int d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(add2_si) "(z.x,x,d); return z;}\n\
    " CPP_BASE_NAME " operator-(int d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(sub2_si) "(z.x,x,d); return z;}\n\
    " CPP_BASE_NAME " operator*(int d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(mul2_si) "(z.x,x,d); return z;}\n\
    " CPP_BASE_NAME " operator/(int d) const {" CPP_BASE_NAME " z; " BASE_PREFIX(div2_si) "(z.x,x,d); return z;}\n\
    \n\
    const " CPP_BASE_NAME "& operator+=(int d) {" BASE_PREFIX(add2_si) "(x,x,d); return *this;}\n\
    const " CPP_BASE_NAME "& operator-=(int d) {" BASE_PREFIX(sub2_si) "(x,x,d); return *this;}\n\
    const " CPP_BASE_NAME "& operator*=(int d) {" BASE_PREFIX(mul2_si) "(x,x,d); return *this;}\n\
    const " CPP_BASE_NAME "& operator/=(int d) {" BASE_PREFIX(div2_si) "(x,x,d); return *this;}\n\
  \n\
  \n\
    " CPP_BASE_NAME " operator-(void) const {" CPP_BASE_NAME " z; " BASE_PREFIX(neg) "(z.x,x); return z;}\n\
    \n\
  \n\
  \n\
    " MY_COEF "& operator[](int idx) {return MY_JET_DATA(x,idx);}\n\
    " MY_COEF "& operator()(int idx[]) {return *" BASE_PREFIX(get_coef) "(x,idx);}\n\
    \n\
    friend " MY_COEF "* coef0(const " CPP_BASE_NAME "& y) {return " BASE_PREFIX(coef0) "(y.x);}\n\
    friend " MY_COEF "* coef1(const " CPP_BASE_NAME "& y, int idx) {return " BASE_PREFIX(coef1) "(y.x, idx);}\n\
  \n\
  \n\
    void printf(const char* fmt) {return " BASE_PREFIX(printf) "(fmt,x);}\n\
    void fprintf(FILE *file, const char* fmt) {return " BASE_PREFIX(fprintf) "(file,fmt,x);}\n\
    void fscanf(FILE *file, const char* fmt) {return " BASE_PREFIX(fscanf) "(file,fmt,x);}\n\
\n\
    operator " BASE_TYPE "&(void) {return x;}\n\
  };\n\
\n\
//\n\
// Special functions\n\
//\n\
static " CPP_BASE_NAME " sqrt(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_sqrt) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME "  sin(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_sin) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME "  cos(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_cos) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME "  tan(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_tan) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME " atan(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_atan) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME " sinh(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_sinh) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME " cosh(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_cosh) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME " pow(const " CPP_BASE_NAME "& y, const " MY_FLOAT "& a) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_pow_myfloat) "(z.x,y.x,a); return z;}\n\
static " CPP_BASE_NAME " exp(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_exp) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME " log(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_log) "(z.x,y.x); return z;}\n\
static " CPP_BASE_NAME " abs(const " CPP_BASE_NAME "& y) {" CPP_BASE_NAME " z; " BASE_PREFIX(set_fabs) "(z.x,y.x); return z;}\n\
}\n\
namespace std {\n\
  using " CPP_BASE_NAME "Lib::exp;\n\
  using " CPP_BASE_NAME "Lib::log;\n\
  using " CPP_BASE_NAME "Lib::pow;\n\
  using " CPP_BASE_NAME "Lib::sqrt;\n\
  using " CPP_BASE_NAME "Lib::abs;\n\
  using " CPP_BASE_NAME "Lib::sinh;\n\
  using " CPP_BASE_NAME "Lib::cosh;\n\
  using " CPP_BASE_NAME "Lib::sin;\n\
  using " CPP_BASE_NAME "Lib::cos;\n\
  using " CPP_BASE_NAME "Lib::tan;\n\
  using " CPP_BASE_NAME "Lib::atan;\n\
  using " CPP_BASE_NAME "Lib::sinh;\n\
  using " CPP_BASE_NAME "Lib::cosh;\n\
}\n\
#endif\n\
" \

#endif /* CPPWRAPPER */
