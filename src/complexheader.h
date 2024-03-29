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

static char *complex_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex', 'complex128', etc. \n\
 */\n\
\n\
#define _USE__COMPLEX_\n\
//#define MY_FLOAT  double complex\n\
\n\
#define   InitMyFloat(r)            \n\
#define   ClearMyFloat(r)           \n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       (a)=(b)\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         (r)=(double complex)(a)\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (MakeMyFloatA(r,a),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       MakeMyFloatA(r,a)\n\
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
#define   DivideMyFloatD(r,a,b)     (r=(a)/(double)(b))\n\
#define   DivideMyFloatSI(r,a,b)    DivideMyFloatD(r,a,b)\n\
#define   DivideDMyFloat(r,a,b)     (r=(double)(a)/(b))\n\
#define   DivideSIMyFloat(r,a,b)    DivideDMyFloat(r,a,b)\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) (r=(a)/(double)(i))\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       (r= -(a))\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         (r=csqrt(a))\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  (r=cpow((a),(b)))\n\
\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  (r=cpow((a),(b)))\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          (r=csin((a)))\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          (r=ccos((a)))\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          (r=ctan((a)))\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         (r=catan((a)))\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          (r=cexp((a)))\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          (r=clog((a)))\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         (r=csinh(a))\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         (r=ccosh(a))\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         (r=ctanh(a))\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        (r=clog10(a))\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         (r=cabs(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)(fa)))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    (dr=(double)(fa))\n\
\n\
\n\
/* boolean operation*/\n\
#define   MyFloatA_GE_B(a,b)        (creal(a)>=creal(b))\n\
#define   MyFloatA_GT_B(a,b)        (creal(a)> creal(b))\n\
#define   MyFloatA_LE_B(a,b)        (creal(a)<=creal(b))\n\
#define   MyFloatA_LT_B(a,b)        (creal(a)< creal(b))\n\
#define   MyFloatA_EQ_B(a,b)        (creal(a)==creal(b))\n\
#define   MyFloatA_NEQ_B(a,b)       (creal(a)!=creal(b))\n\
#define   MyFloatA_CMP_B(a,b)       (creal(a)-creal(b))\n\
#define   MyFloatA_CMPABS_B(a,b)    (fabs(creal(a))-fabs(creal(b)))\n\
\n\
\n\
#define   OutputMyFloat3(file,fmt,a) fprintf(file,fmt,(double)creal(a), (double)cimag(a))\n\
#define   OutputMyFloat(a)           fprintf(stdout,\"\%g,\%g \",(double)creal(a), (double)cimag(a))\n\
\n\
#define   InputMyFloat3(file,fmt,a)  fscanf(file,fmt,(double*)&(a),((double*)&(a))+1)\n\
#define   InputMyFloat(a)            InputMyFloat3(stdin,\"\%lf,\%lf \",a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (sscanf(s,fmt,(double*)&(a),((double*)&(a))+1,n),(s)+*(n))\n\
#define   StringToMyFloat3(s,fmt,a)  sscanf(s,fmt,(double*)&(a),((double*)&(a))+1)\n\
#define   StringToMyFloat(s,a)       StringToMyFloat3(s,\"\%lf,\%lf \",a)\n\
\n\
/************************************************************************/\n\
";

