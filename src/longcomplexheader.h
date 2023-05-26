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

static char *longcomplex_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex', 'long complex', 'complex128', etc. \n\
 */\n\
#define _USE_LONG_COMPLEX_\n\
\n\
/* for double or long double, don't need to initialize */\n\
#define   InitMyFloat(r)            \n\
#define   ClearMyFloat(r)           \n\
 \n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       {(a)=(b);}\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         (r=(long double complex)(a))\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (r=(double)(a),r)\n\
\n\
/* create a MY_FLOAT from string, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (r=(double)(a))\n\
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
#define   SubtractDMyFloat         SubtractMyFloatA\n\
#define   SubtractSIMyFloat         SubtractMyFloatA\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   (r=(a)*(b))\n\
#define   MultiplyMyFloatD          MultiplyMyFloatA\n\
#define   MultiplyMyFloatSI         MultiplyMyFloatA\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     (r=(a)/(b))\n\
#define   DivideMyFloatD(r,a,b)     (r=(a)/(long double)(b))\n\
#define   DivideMyFloatSI           DivideMyFloatD\n\
#define   DivideDMyFloat(r,a,b)     (r=(long double)(a)/(b))\n\
#define   DivideSIMyFloat           DivideDMyFloat\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i)    (r=(a)/(long double)(i))\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)          (r= -(a))\n\
\n\
/* square root r=sqrtl(a) */\n\
#define   sqrtMyFloatA(r,a)            (r=csqrtl(a))\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  (r=cpowl((a),(b)))\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b) (r=cpowl((a),(long double)(b)))\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)             (r=csinl((a)))\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)             (r=ccosl((a)))\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)             (r=ctanl((a)))\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)            (r=catanl((a)))\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)             (r=cexpl((a)))\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)             (r=clogl((a)))\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)            (r=csinhl(a))\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)            (r=ccoshl(a))\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)            (r=ctanhl(a))\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)           (r=clog10l((a)))\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)            (r=cabsl(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)          (ir=(int)(fa))\n\
/* convert to double */\n\
#define   MyFloatToDouble(ir,fa)       (ir=(double)(fa))\n\
/* convert to string */\n\
#define   MyFloatToString(s,a)         sprintf(s,\"\% .19e \% .19e\",creall(a),cimagl(a))\n\
\n\
\n\
/* boolean operation  */\n\
#define   MyFloatA_GE_B(a,b)        (creall(a)>=creall(b))\n\
#define   MyFloatA_GT_B(a,b)        (creall(a)> creall(b))\n\
#define   MyFloatA_LE_B(a,b)        (creall(a)<=creall(b))\n\
#define   MyFloatA_LT_B(a,b)        (creall(a)< creall(b))\n\
#define   MyFloatA_EQ_B(a,b)        (creall(a)==creall(b))\n\
#define   MyFloatA_NEQ_B(a,b)       (creall(a)!=creall(b))\n\
#define   MyFloatA_CMP_B(a,b)       (creall(a)-creall(b))\n\
#define   MyFloatA_CMPABS_B(a,b)    (fabsl(creall(a))-fabsl(creall(b)))\n\
\n\
\n\
#define   OutputMyFloat3(file,fmt,a) fprintf(file,fmt,(long double) creall(a), (long double) cimagl(a))\n\
#define   OutputMyFloat(a)           OutputMyFloat3(stdout,\"\%Lg,\%Lg \",(long double) creall(a), (long double) cimagl(a))\n\
\n\
#define   InputMyFloat3(file,fmt,a)  fscanf(file,fmt,(long double*)&(a),((long double*)&(a))+1)\n\
#define   InputMyFloat(a)            InputMyFloat3(stdin,\"\%Lf,\%Lf \",a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (sscanf(s,fmt,(long double*)&(a),((long double*)&(a))+1,n),(s)+*(n))\n\
#define   StringToMyFloat3(s,fmt,a)   sscanf(s,fmt,(long double*)&(a),((long double*)&(a))+1)\n\
#define   StringToMyFloat(s,a)        StringToMyFloat3(s,\"\%Lf,\%Lf \",a)\n\
\n\
/************************************************************************/\n\
";


