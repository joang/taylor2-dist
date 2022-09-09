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

static char *mpfr_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex' etc. \n\
 */\n\
#ifndef _TAYLOR_\n\
#define _TAYLOR_\n\
\n\
#define _USE_MPFR_\n\
#define MY_FLOAT  mpfr_t\n\
\n\
#define   InitMyFloat(r)            mpfr_init(r)\n\
#define   ClearMyFloat(r)           mpfr_clear(r)\n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       mpfr_set(a,b,GMP_RNDN)\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         mpfr_set_d(r,(double)(a),GMP_RNDN)\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (mpfr_set_d(r,(double)(a),GMP_RNDN),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (mpfr_set_str(r,s,10,GMP_RNDN))\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        mpfr_add(r,(a),(b),GMP_RNDN)\n\
#define   AddMyFloatD(r,a,b)        mpfr_add_d(r,(a),(b),GMP_RNDN)\n\
#define   AddMyFloatSI(r,a,b)       mpfr_add_si(r,(a),(b),GMP_RNDN)\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)   mpfr_sub(r,(a),(b),GMP_RNDN)\n\
#define   SubtractMyFloatD(r,a,b)   mpfr_sub_d(r,(a),(b),GMP_RNDN)\n\
#define   SubtractMyFloatSI(r,a,b)  mpfr_sub_si(r,(a),(b),GMP_RNDN)\n\
#define   SubtractSIMyFloat(r,a,b)  mpfr_si_sub(r,(a),(b),GMP_RNDN)\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   mpfr_mul(r,(a),(b),GMP_RNDN)\n\
#define   MultiplyMyFloatD(r,a,b)   mpfr_mul_d(r,(a),(b),GMP_RNDN)\n\
#define   MultiplyMyFloatSI(r,a,b)  mpfr_mul_si(r,(a),(b),GMP_RNDN)\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     mpfr_div(r,(a),(b),GMP_RNDN)\n\
#define   DivideMyFloatD(r,a,b)     mpfr_div_d(r,(a),(b),GMP_RNDN)\n\
#define   DivideMyFloatSI(r,a,b)    mpfr_div_si(r,(a),(b),GMP_RNDN)\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) mpfr_div_ui(r,(a),(unsigned long)(i), GMP_RNDN)\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       mpfr_neg(r,(a),GMP_RNDN)\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         mpfr_sqrt(r,(a),GMP_RNDN)\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  mpfr_pow_ui(r,(a),(unsigned long)(b),GMP_RNDN)\n\
\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  mpfr_pow(r,(a),(b),GMP_RNDN)\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          mpfr_sin(r,(a),GMP_RNDN)\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          mpfr_cos(r,(a),GMP_RNDN)\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          mpfr_tan(r,(a),GMP_RNDN)\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         mpfr_atan(r,(a),GMP_RNDN)\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          mpfr_exp(r,(a),GMP_RNDN)\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          mpfr_log(r,(a),GMP_RNDN)\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         mpfr_sinh(r,(a),GMP_RNDN)\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         mpfr_cosh(r,(a),GMP_RNDN)\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         mpfr_tanh(r,(a),GMP_RNDN)\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        mpfr_log10(r,(a),GMP_RNDN)\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         mpfr_abs(r,(a),GMP_RNDN)\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)mpfr_get_d(fa,GMP_RNDN))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    dr=mpfr_get_d(fa,GMP_RNDN)\n\
\n\
\n\
/* boolean operation\n\
#define   MyFloatA_GE_B(a,b)        (mpfr_cmp((a),(b))>=0)\n\
#define   MyFloatA_GT_B(a,b)        (mpfr_cmp((a),(b))>0)\n\
#define   MyFloatA_LE_B(a,b)        (mpfr_cmp((a),(b))<=0)\n\
#define   MyFloatA_LT_B(a,b)        (mpfr_cmp((a),(b))<0)\n\
#define   MyFloatA_EQ_B(a,b)        (mpfr_cmp((a),(b))==0)\n\
#define   MyFloatA_NEQ_B(a,b)       (mpfr_cmp((a),(b))!=0)\n\
*/\n\
\n\
/* boolean operation */\n\
#define   MyFloatA_GE_B(a,b)        mpfr_greaterequal_p((a),(b))\n\
#define   MyFloatA_GT_B(a,b)        mpfr_greater_p((a),(b))\n\
#define   MyFloatA_LE_B(a,b)        mpfr_lessequal_p((a),(b))\n\
#define   MyFloatA_LT_B(a,b)        mpfr_less_p((a),(b))\n\
#define   MyFloatA_EQ_B(a,b)        mpfr_equal_p((a),(b))\n\
#define   MyFloatA_NEQ_B(a,b)       ((mpfr_equal_p((a),(b))) ? 0 : 1)\n\
\n\
\n\
#define   OutputMyFloat(a)          mpfr_out_str(stdout,10,20,a,GMP_RNDN),fprintf(stdout,\" \")\n\
#define   OutputMyFloat3(file,fmt,a)        mpfr_fprintf(file,fmt,a)\n\
\n\
/************************************************************************/\n\
#endif\n\
";

