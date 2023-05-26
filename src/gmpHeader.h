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

static char *gmp_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex' etc. \n\
 */\n\
\n\
#define _USE_GMP_\n\
#define MY_FLOAT  mpf_t\n\
static  mpf_t gmp_j_aux;\n\
static  int gmp_j_aux_init=0;\n\
\n\
#define   InitMyFloat(r)            mpf_init(r)\n\
#define   ClearMyFloat(r)           mpf_clear(r)\n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       mpf_set(a,b)\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         mpf_set_d(r,(double)(a))\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (mpf_set_d(r,(double)(a)),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (mpf_set_str(r,s,10))\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        mpf_add(r,(a), (b))\n\
#define   AddMyFloatD(r,a,b)      {if(gmp_j_aux_init==0){gmp_j_aux_init=1;mpf_init(gmp_j_aux);}mpf_set_d(gmp_j_aux,(double)(b));mpf_add(r,(a),gmp_j_aux);}\n\
#define   AddMyFloatSI(r,a,b)       mpf_add_ui(r,(a), (unsigned long)(b))\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)  mpf_sub(r,(a),(b))\n\
#define   SubtractMyFloatD(r,a,b)  {if(gmp_j_aux_init==0){gmp_j_aux_init=1;mpf_init(gmp_j_aux);}mpf_set_d(gmp_j_aux,(double)(b));mpf_sub(r,(a),gmp_j_aux);}\n\
#define   SubtractMyFloatSI(r,a,b)  mpf_sub_ui(r,(a),(unsigned long)(b))\n\
#define   SubtractMyFloatD(r,b,a)  {if(gmp_j_aux_init==0){gmp_j_aux_init=1;mpf_init(gmp_j_aux);}mpf_set_d(gmp_j_aux,(double)(b));mpf_sub(r,(a),gmp_j_aux);}\n\
#define   SubtractSIMyFloat(r,a,b)  mpf_ui_sub(r,(unsigned long)(a),(b))\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   mpf_mul(r,(a), (b))\n\
#define   MultiplyMyFloatD(r,a,b)      {if(gmp_j_aux_init==0){gmp_j_aux_init=1;mpf_init(gmp_j_aux);}mpf_set_d(gmp_j_aux,(double)(b));mpf_mul(r,(a),gmp_j_aux);}\n\
#define   MultiplyMyFloatSI(r,a,b)   mpf_mul_ui(r,(a), (unsigned long)(b))\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     mpf_div(r,(a),(b))\n\
#define   DivideMyFloatD(r,a,b)      {if(gmp_j_aux_init==0){gmp_j_aux_init=1;mpf_init(gmp_j_aux);}mpf_set_d(gmp_j_aux,(double)(b));mpf_div(r,(a),gmp_j_aux);}\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatSI(r,a,i)    mpf_div_si(r,(a),(long)(i))\n\
#define   DivideDMyFloat(r,a,b)     undefined\n\
#define   DivideSIMyFloat(r,a,i)    undefined\n\
#define   DivideMyFloatByInt(r,a,i) mpf_div_si(r,(a),(long)(i))\n\
\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       mpf_neg(r, (a))\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         mpf_sqrt(r,(a))\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  mpf_pow_ui(r, (a),(unsigned long)(b))\n\
\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  {fprintf(stderr,\"Pow is unavailable in GMP!\\n\"); exit(1);}\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          {fprintf(stderr,\"sin is unavailable in GMP!\\n\"); exit(1);}\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          {fprintf(stderr,\"cos is unavailable in GMP!\\n\"); exit(1);}\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          {fprintf(stderr,\"tan is unavailable in GMP!\\n\"); exit(1);}\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         {fprintf(stderr,\"atan is unavailable in GMP!\\n\"); exit(1);}\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          {fprintf(stderr,\"exp is unavailable in GMP!\\n\"); exit(1);}\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          {fprintf(stderr,\"log is unavailable in GMP!\\n\"); exit(1);}\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         {fprintf(stderr,\"sinh is unavailable in GMP!\\n\"); exit(1);}\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         {fprintf(stderr,\"cosh is unavailable in GMP!\\n\"); exit(1);}\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         {fprintf(stderr,\"tanh is unavailable in GMP!\\n\"); exit(1);}\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        {fprintf(stderr,\"log10 is unavailable in GMP!\\n\"); exit(1);}\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         mpf_abs(r,(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)mpf_get_d(fa))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    dr=mpf_get_d(fa)\n\
/* convert to string */\n\
#define   MyFloatToString3(s,fmt,a) undefined\n\
#define   MyFloatToString(s,a)      undefined\n\
\n\
\n\
/* boolean operation  */\n\
#define   MyFloatA_GE_B(a,b)        (mpf_cmp((a),(b))>=0)\n\
#define   MyFloatA_GT_B(a,b)        (mpf_cmp((a),(b))>0)\n\
#define   MyFloatA_LE_B(a,b)        (mpf_cmp((a),(b))<=0)\n\
#define   MyFloatA_LT_B(a,b)        (mpf_cmp((a),(b))<0)\n\
#define   MyFloatA_EQ_B(a,b)        (mpf_cmp((a),(b))==0)\n\
#define   MyFloatA_NEQ_B(a,b)       (mpf_cmp((a),(b))!=0)\n\
\n\
\n\
#define   OutputMyFloat3(file,fmt,a) gmp_fprintf(file,fmt,a)\n\
#define   OutputMyFloat(a)           mpf_out_str(stdout,10,20,a),fprintf(stdout,\" \")\n\
\n\
#define   InputMyFloat3(file,fmt,a)  mpf_inp_str(a,file,10)\n\
#define   InputMyFloat(a)            InputMyFloat3(stdin,NULL,a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (gmp_sscanf(s,fmt,&(a),n),(s)+*(n))\n\
#define   StringToMyFloat3(s,fmt,a)  gmp_sscanf(s,fmt,&(a))\n\
#define   StringToMyFloat(s,a)       StringToMyFloat3(s,\"\%Ff \",a)\n\
\n\
/************************************************************************/\n\
";

