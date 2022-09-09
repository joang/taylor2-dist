/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
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

static char *qd_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex' etc. \n\
 */\n\
#ifndef _TAYLOR_\n\
#define _TAYLOR_\n\
\n\
#define _USE_QD_\n\
\n\
/* for double or long double, don't need to initialize */\n\
#define   InitMyFloat(r)            \n\
#define   ClearMyFloat(r)           \n\
 \n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       {(a)=(b);}\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         (r=(double)(a))\n\
\n\
/* create a MY_FLOAT from string, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (r=s)\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (r=(double)(a),r)\n\
\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        (r=(a)+(b))\n\
#define   AddMyFloatD(r,a,b)        (r=(a)+(double)(b))\n\
#define   AddMyFloatSI(r,a,b)        (r=(a)+(double)(b))\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)  (r=(a)-(b))\n\
#define   SubtractMyFloatD(r,a,b)  (r=(a)-(double)(b))\n\
#define   SubtractMyFloatSI(r,a,b)  (r=(a)-(double)(b))\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   (r=(a)*(b))\n\
#define   MultiplyMyFloatD(r,a,b)   (r=(a)*(double)(b))\n\
#define   MultiplyMyFloatSI(r,a,b)   (r=(a)*(double)(b))\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     (r=(a)/(b))\n\
#define   DivideMyFloatD(r,a,b)     (r=(a)/(double)(b))\n\
#define   DivideMyFloatSI(r,a,b)     (r=(a)/(double)(b))\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) (r=(a)/(double)(i))\n\
\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       (r= -(a))\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         (r=sqrt(a))\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  (r=exp(b * log(a)))\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  (r=pow((a),(double)(b)))\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          (r=sin((a)))\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          (r=cos((a)))\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          (r=tan((a)))\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         (r=atan((a)))\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          (r=exp((a)))\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          (r=log((a)))\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         (r=sinh(a))\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)          (r=cosh(a))\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)          (r=tanh(a))\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)         (r=log10((a)))\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)          (r=fabs(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=to_int(fa))\n\
/* convert to double */\n\
#define   MyFloatToDouble(ir,fa)    (ir=to_double(fa))\n\
\n\
\n\
/* boolean operation  */\n\
#define   MyFloatA_GE_B(a,b)        ((a)>=(b))\n\
#define   MyFloatA_GT_B(a,b)        ((a)> (b))\n\
#define   MyFloatA_LE_B(a,b)        ((a)<=(b))\n\
#define   MyFloatA_LT_B(a,b)        ((a)< (b))\n\
#define   MyFloatA_EQ_B(a,b)        ((a)==(b))\n\
#define   MyFloatA_NEQ_B(a,b)       ((a)!=(b))\n\
\n\
\n\
#define   OutputMyFloat(a)           cout<<a \n\
#define   OutputMyFloat3(file,fmt,a)           cout<<a \n\
/************************************************************************/\n\
#endif\n\
";

