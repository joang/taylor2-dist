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

static char *_float128_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex' etc. \n\
 */\n\
#define _USE__FLOAT128_\n\
static char f128_out_buf[128];\n\
\n\
//#define MY_FLOAT  __float128\n\
\n\
/* for double or long double, don't need to initialize */\n\
#define   InitMyFloat(r)            \n\
#define   ClearMyFloat(r)           \n\
 \n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       ((a)=(b))\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         (r=(__float128)(a))\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (MakeMyFloatA(r,a),r)\n\
\n\
/* create a MY_FLOAT from string, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (r=strtoflt128(s,NULL))\n\
\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        (r=(a)+(b))\n\
#define   AddMyFloatD               AddMyFloatA\n\
#define   AddMyFloatSI              AddMyFloatA\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)   (r=(a)-(b))\n\
#define   SubtractMyFloatD          SubtractMyFloatA\n\
#define   SubtractMyFloatSI         SubtractMyFloatA\n\
#define   SubtractDMyFloat          SubtractMyFloatA\n\
#define   SubtractSIMyFloat         SubtractMyFloatA\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   (r=(a)*(b))\n\
#define   MultiplyMyFloatD          MultiplyMyFloatA\n\
#define   MultiplyMyFloatSI         MultiplyMyFloatA\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     (r=(a)/(b))\n\
#define   DivideMyFloatD(r,a,b)     (r=(a)/(__float128)(b))\n\
#define   DivideMyFloatSI           DivideMyFloatD\n\
#define   DivideDMyFloat(r,a,b)     (r=(__float128)(a)/(b))\n\
#define   DivideSIMyFloat           DivideDMyFloat\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i)    (r=(a)/(__float128)(i))\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)          (r= -(a))\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)            (r=sqrtq(a))\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  (r=powq((a),(b)))\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b) (r=powq((a),(__float128)(b)))\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)             (r=sinq((a)))\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)             (r=cosq((a)))\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)             (r=tanq((a)))\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)            (r=atanq((a)))\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)             (r=expq((a)))\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)             (r=logq((a)))\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)            (r=sinhq(a))\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)            (r=coshq(a))\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)            (r=tanhq(a))\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)           (r=log10q((a)))\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)            (r=fabsq(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)          (ir=(int)(fa))\n\
/* convert to double */\n\
#define   MyFloatToDouble(ir,fa)       (ir=(double)(fa))\n\
/* convert to string */\n\
#define   MyFloatToString3(s,fmt,a)    quadmath_snprintf(s,fmt,a)\n\
#define   MyFloatToString(s,a)         MyFloatToString3(s,\"\%.33Qe\",a)\n\
\n\
\n\
/* boolean operation  */\n\
#define   MyFloatA_GE_B(a,b)        ((a)>=(b))\n\
#define   MyFloatA_GT_B(a,b)        ((a)> (b))\n\
#define   MyFloatA_LE_B(a,b)        ((a)<=(b))\n\
#define   MyFloatA_LT_B(a,b)        ((a)< (b))\n\
#define   MyFloatA_EQ_B(a,b)        ((a)==(b))\n\
#define   MyFloatA_NEQ_B(a,b)       ((a)!=(b))\n\
#define   MyFloatA_CMP_B(a,b)       ((a)-(b))\n\
#define   MyFloatA_CMPABS_B(a,b)    (fabsq(a)-fabsq(b))\n\
\n\
\n\
#define   OutputMyFloat3(file,fmt,a)  quadmath_snprintf(f128_out_buf, 128, fmt, a),fprintf(file,\"\%s \",f128_out_buf)\n\
#define   OutputMyFloat(a)            OutputMyFloat3(stdout, \"\% .33Qe\", a)\n\
\n\
#define   InputMyFloat3(file,fmt,a)   (fscanf(file,\"\%128s\",f128_out_buf),a=strtoflt128(f128_out_buf,NULL))\n\
#define   InputMyFloat(a)             InputMyFloat3(stdin,NULL,a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (a=strtoflt128(s,&(s)),s)\n\
#define   StringToMyFloat3(s,fmt,a)   a=strtoflt128(s,NULL)\n\
#define   StringToMyFloat(s,a)        StringToMyFloat3(s,NULL,a)\n\
\n\
/************************************************************************/\n\
";


