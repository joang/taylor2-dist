/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 2022  Maorong Zou, Angel Jorba, Joan Gimeno
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

static char *complex128_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex', 'complex128', etc. \n\
 */\n\
#define _USE__COMPLEX128_\n\
static char f128_out_buf[128];\n\
\n\
//#define MY_FLOAT  __complex128\n\
\n\
#define   InitMyFloat(r)            \n\
#define   ClearMyFloat(r)           \n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       ((a)=(b))\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         ((r)=(__complex128)(a))\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (MakeMyFloatA(r,a),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (r=strtoflt128(s,NULL))\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        (r=(a)+(b))\n\
#define   AddMyFloatD(r,a,b)        AddMyFloatA(r,a,b)\n\
#define   AddMyFloatSI(r,a,b)       AddMyFloatA(r,a,b)\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)   (r=(a)-(b))\n\
#define   SubtractMyFloatD(r,a,b)   SubtractMyFloatA(r,a,b)\n\
#define   SubtractMyFloatSI(r,a,b)  SubtractMyFloatA(r,a,b)\n\
#define   SubtractDMyFloat(r,a,b)   SubtractMyFloatA(r,a,b)\n\
#define   SubtractSIMyFloat(r,a,b)  SubtractMyFloatA(r,a,b)\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   (r=(a)*(b))\n\
#define   MultiplyMyFloatD(r,a,b)   MultiplyMyFloatA(r,a,b)\n\
#define   MultiplyMyFloatSI(r,a,b)  MultiplyMyFloatA(r,a,b)\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     (r=(a)/(b))\n\
#define   DivideMyFloatD(r,a,b)     (r=(a)/(__float128)(b))\n\
#define   DivideMyFloatSI(r,a,b)    DivideMyFloatD(r,a,b)\n\
#define   DivideDMyFloat(r,a,b)     (r=(__float128)(a)/(b))\n\
#define   DivideSIMyFloat(r,a,b)    DivideDMyFloat(r,a,b)\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) (r=(a)/(__float128)(i))\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       (r= -(a))\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         (r=csqrtq(a))\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  (r=cpowq((a),(b)))\n\
\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  (r=cpowq((a),(b)))\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          (r=csinq((a)))\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          (r=ccosq((a)))\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          (r=ctanq((a)))\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         (r=catanq((a)))\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          (r=cexpq((a)))\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          (r=clogq((a)))\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         (r=csinhq(a))\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         (r=ccoshq(a))\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         (r=ctanhq(a))\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        (r=clog10q(a))\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         (r=cabsq(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)(fa)))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    (dr=(double)(fa))\n\
\n\
\n\
/* boolean operation*/\n\
#define   MyFloatA_GE_B(a,b)        (crealq(a)>=crealq(b))\n\
#define   MyFloatA_GT_B(a,b)        (crealq(a)> crealq(b))\n\
#define   MyFloatA_LE_B(a,b)        (crealq(a)<=crealq(b))\n\
#define   MyFloatA_LT_B(a,b)        (crealq(a)< crealq(b))\n\
#define   MyFloatA_EQ_B(a,b)        (crealq(a)==crealq(b))\n\
#define   MyFloatA_NEQ_B(a,b)       (crealq(a)!=crealq(b))\n\
#define   MyFloatA_CMP_B(a,b)       (crealq(a)-crealq(b))\n\
#define   MyFloatA_CMPABS_B(a,b)    (fabsq(crealq(a))-fabs(crealq(b)))\n\
\n\
\n\
\n\
#define   OutputMyFloat3(file,fmt,a) (quadmath_snprintf(f128_out_buf, 128, fmt, crealq(a)),\\\n\
                                     fprintf(file,\"\%s,\",f128_out_buf),\\\n\
                                     quadmath_snprintf(f128_out_buf, 128, fmt, cimagq(a)),\\\n\
                                     fprintf(file,\"\%s \",f128_out_buf))\n\
#define   OutputMyFloat(a)           OutputMyFloat3(stdout,\"\% .33Qe\",a)\n\
\n\
#define   InputMyFloat3(file,fmt,a)   (fscanf(file,\"\%[^,]128s\",f128_out_buf),\\\n\
                                      a=strtoflt128(f128_out_buf,NULL),\\\n\
                                      fscanf(file,\",\%128s\",f128_out_buf),\\\n\
                                      a+=_Complex_I*strtoflt128(f128_out_buf,NULL))\n\
#define   InputMyFloat(a)             InputMyFloat3(stdin,NULL,a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (a=strtoflt128(s,&(s)),a+=_Complex_I*(*(s)==\',\'?strtoflt128(s+1,&(s)):0),s)\n\
#define   StringToMyFloat3(s,fmt,a)   {char *__buf__=s;\\\n\
                                      a=strtoflt128(__buf__,&__buf__);\\\n\
                                      a+=_Complex_I*(*__buf__==\',\' ? strtoflt128(__buf__+1,&__buf__):0);}\n\
#define   StringToMyFloat(s,a)        StringToMyFloat3(s,NULL,a)\n\
\n\
/************************************************************************/\n\
";

