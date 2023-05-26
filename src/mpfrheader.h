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
\n\
#define _USE_MPFR_\n\
#define MY_FLOAT  mpfr_t\n\
#define MY_FLOAT_ROUNDING GMP_RNDN\n\
\n\
#define   InitMyFloat(r)            mpfr_init(r)\n\
#define   ClearMyFloat(r)           mpfr_clear(r)\n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       mpfr_set(a,b,MY_FLOAT_ROUNDING)\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         mpfr_set_d(r,(double)(a),MY_FLOAT_ROUNDING)\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (MakeMyFloatA(r,a),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (mpfr_set_str(r,s,10,MY_FLOAT_ROUNDING))\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        mpfr_add(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   AddMyFloatD(r,a,b)        mpfr_add_d(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   AddMyFloatSI(r,a,b)       mpfr_add_si(r,(a),(b),MY_FLOAT_ROUNDING)\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)   mpfr_sub(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   SubtractMyFloatD(r,a,b)   mpfr_sub_d(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   SubtractMyFloatSI(r,a,b)  mpfr_sub_si(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   SubtractDMyFloat(r,a,b)   mpfr_d_sub(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   SubtractSIMyFloat(r,a,b)  mpfr_si_sub(r,(a),(b),MY_FLOAT_ROUNDING)\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   mpfr_mul(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   MultiplyMyFloatD(r,a,b)   mpfr_mul_d(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   MultiplyMyFloatSI(r,a,b)  mpfr_mul_si(r,(a),(b),MY_FLOAT_ROUNDING)\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     mpfr_div(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   DivideMyFloatD(r,a,b)     mpfr_div_d(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   DivideMyFloatSI(r,a,b)    mpfr_div_si(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   DivideDMyFloat(r,a,b)     mpfr_d_div(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   DivideSIMyFloat(r,a,b)    mpfr_si_div(r,(a),(b),MY_FLOAT_ROUNDING)\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) mpfr_div_ui(r,(a),(unsigned long)(i), MY_FLOAT_ROUNDING)\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       mpfr_neg(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         mpfr_sqrt(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  mpfr_pow_ui(r,(a),(unsigned long)(b),MY_FLOAT_ROUNDING)\n\
\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  mpfr_pow(r,(a),(b),MY_FLOAT_ROUNDING)\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          mpfr_sin(r,(a),MY_FLOAT_ROUNDING)\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          mpfr_cos(r,(a),MY_FLOAT_ROUNDING)\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          mpfr_tan(r,(a),MY_FLOAT_ROUNDING)\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         mpfr_atan(r,(a),MY_FLOAT_ROUNDING)\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          mpfr_exp(r,(a),MY_FLOAT_ROUNDING)\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          mpfr_log(r,(a),MY_FLOAT_ROUNDING)\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         mpfr_sinh(r,(a),MY_FLOAT_ROUNDING)\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         mpfr_cosh(r,(a),MY_FLOAT_ROUNDING)\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         mpfr_tanh(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        mpfr_log10(r,(a),MY_FLOAT_ROUNDING)\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         mpfr_abs(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)mpfr_get_d(fa,MY_FLOAT_ROUNDING))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    dr=mpfr_get_d(fa,MY_FLOAT_ROUNDING)\n\
/* convert to string */\n\
#define   MyFloatToString(s,a)      mpfr_sprintf(s,\"% .20R*e\",MY_FLOAT_ROUNDING,a)\n\
\n\
/* boolean operation\n\
#define   MyFloatA_GE_B(a,b)        (mpfr_cmp((a),(b))>=0)\n\
#define   MyFloatA_GT_B(a,b)        (mpfr_cmp((a),(b))> 0)\n\
#define   MyFloatA_LE_B(a,b)        (mpfr_cmp((a),(b))<=0)\n\
#define   MyFloatA_LT_B(a,b)        (mpfr_cmp((a),(b))< 0)\n\
#define   MyFloatA_EQ_B(a,b)        (mpfr_cmp((a),(b))==0)\n\
#define   MyFloatA_NEQ_B(a,b)       (mpfr_cmp((a),(b))!=0)\n\
#define   MyFloatA_CMP_B(a,b)        mpfr_cmp((a),(b))\n\
#define   MyFloatA_CMPABS_B(a,b)     mpfr_cmpabs((a),(b))\n\
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
#define   OutputMyFloat3(file,fmt,a)  mpfr_fprintf(file,fmt,a)\n\
#define   OutputMyFloat(a)            mpfr_out_str(stdout,10,0,a,MY_FLOAT_ROUNDING),fprintf(stdout,\" \")\n\
\n\
#define   InputMyFloat3(file,fmt,a)   mpfr_inp_str(a,file,10,MY_FLOAT_ROUNDING)\n\
#define   InputMyFloat(a)             InputMyFloat3(stdin,NULL,a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (mpfr_strtofr(a,s,&s,10,MY_FLOAT_ROUNDING),s)\n\
#define   StringToMyFloat3(s,fmt,a)   mpfr_set_str(a,s,10,MY_FLOAT_ROUNDING)\n\
#define   StringToMyFloat(s,a)        StringToMyFloat3(s,NULL,a)\n\
\n\
/************************************************************************/\n\
";

