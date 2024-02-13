/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
 *    Copyright (C) 2022 Joan Gimeno, Angel Jorba, Maorong Zou
 *    Copyright (C) 2023 Joan Gimeno, Angel Jorba, Maorong Zou
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

static char *arf_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex' etc. \n\
 */\n\
\n\
#define _USE_ARF_\n\
#define MY_FLOAT  arf_t\n\
extern slong myfloat_arf_prec;\n\
extern arf_rnd_t myfloat_arf_rnd; /* ARF_RND_NEAR */\n\
#define MY_FLOAT_RND myfloat_arf_rnd\n\
#define MY_FLOAT_PREC myfloat_arf_prec /* MY_FLOAT_DIGITS = MY_FLOAT_PREC/3.32192809488736+1 */\n\
\n\
#define   InitMyFloat(r)            arf_init(r)\n\
#define   ClearMyFloat(r)           arf_clear(r)\n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       arf_set(a,b)\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an integer or a float */\n\
#define   MakeMyFloatA(r,a)         arf_set_d(r,(double)(a))\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (MakeMyFloatA(r,a),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       MakeMyFloatB(r,a)\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        arf_add(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
#define   AddMyFloatD(r,a,b)        {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,b); AddMyFloatA(r,(a),__b); ClearMyFloat(__b);}\n\
#define   AddMyFloatSI(r,a,b)       arf_add_si(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)   arf_sub(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
#define   SubtractMyFloatD(r,a,b)   {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,b); SubtractMyFloatA(r,(a),__b); ClearMyFloat(__b);}\n\
#define   SubtractMyFloatSI(r,a,b)  arf_sub_si(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
#define   SubtractDMyFloat(r,a,b)   {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,a); SubtractMyFloatA(r,__b,(b)); ClearMyFloat(__b);}\n\
#define   SubtractSIMyFloat(r,a,b)  {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,a); SubtractMyFloatA(r,__b,(b)); ClearMyFloat(__b);}\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   arf_mul(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
#define   MultiplyMyFloatD(r,a,b)   {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,b); MultiplyMyFloatA(r,(a),__b); ClearMyFloat(__b);}\n\
#define   MultiplyMyFloatSI(r,a,b)  arf_mul_si(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     arf_div(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
#define   DivideMyFloatD(r,a,b)     {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,b); DivideMyFloatA(r,(a),__b); ClearMyFloat(__b);}\n\
#define   DivideMyFloatSI(r,a,b)    arf_div_si(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
#define   DivideDMyFloat(r,a,b)     {MY_FLOAT __b; InitMyFloat(__b); MakeMyFloatA(__b,a); DivideMyFloatA(r,__b,(b)); ClearMyFloat(__b);}\n\
#define   DivideSIMyFloat(r,a,b)    arf_si_div(r,(a),(b),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) arf_div_ui(r,(a),(unsigned long)(i),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       arf_neg(r,(a))\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         arf_sqrt(r,(a),MY_FLOAT_PREC,MY_FLOAT_RND)\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_pow_ui(__a,__a,(unsigned long)(b), MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  {arb_t __a,__b; arb_init(__a); arb_init(__b); arb_set_arf(__a,a); arb_set_arf(__b,b); arb_pow(__a,__a,__b, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__b); arb_clear(__a);}\n\
\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_sin(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_cos(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_tan(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         {arb_t __a; arb_init(__a); arb_atan_arf(__a,a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_exp(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          {arb_t __a; arb_init(__a); arb_log_arf(__a,a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_sinh(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_cosh(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         {arb_t __a; arb_init(__a); arb_set_arf(__a,a); arb_tanh(__a,__a, MY_FLOAT_PREC); AssignMyFloat(r, arb_midref(__a)); arb_clear(__a);}\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        undefined\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         arf_abs(r,(a))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)arf_get_si(fa,MY_FLOAT_RND))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    dr=arf_get_d(fa,MY_FLOAT_RND)\n\
/* convert to string WARNING: char memory!*/\n\
#define   MyFloatToString(s,a)      {char *__s = arf_dump_str(a); strcpy(s,__s); flint_free(__s);}\n\
\n\
/* boolean operation */\n\
#define   MyFloatA_GE_B(a,b)        (arf_cmp((a),(b))>=0)\n\
#define   MyFloatA_GT_B(a,b)        (arf_cmp((a),(b))> 0)\n\
#define   MyFloatA_LE_B(a,b)        (arf_cmp((a),(b))<=0)\n\
#define   MyFloatA_LT_B(a,b)        (arf_cmp((a),(b))< 0)\n\
#define   MyFloatA_EQ_B(a,b)        (arf_cmp((a),(b))==0)\n\
#define   MyFloatA_NEQ_B(a,b)       (arf_cmp((a),(b))!=0)\n\
#define   MyFloatA_CMP_B(a,b)        arf_cmp((a),(b))\n\
#define   MyFloatA_CMPABS_B(a,b)     arf_cmpabs((a),(b))\n\
\n\
\n\
#define   OutputMyFloat3(file,fmt,a)  arf_fprintd(file,a,MY_FLOAT_PREC/3.32192809488736+1)\n\
#define   OutputMyFloat(a)            arf_dump_file(stdout,a),printf(\" \")\n\
\n\
#define   InputMyFloat3(file,fmt,a)   arf_load_file(a,file)\n\
#define   InputMyFloat(a)             InputMyFloat3(stdin,NULL,a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) arf_load_str(s,a)\n\
#define   StringToMyFloat3(s,fmt,a)   StringToMyFloat4(s,fmt,a,0)\n\
#define   StringToMyFloat(s,a)        StringToMyFloat3(s,NULL,a)\n\
\n\
/************************************************************************/\n\
";
