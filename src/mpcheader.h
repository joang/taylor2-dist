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

static char *mpc_header="\
/*\n\
 *  MY_FLOAT is the data type to be used in computing derivatives. \n\
 *  It may be 'float', 'double' or user defined private data types\n\
 *  like 'long double', 'complex', 'complex128', etc. \n\
 */\n\
\n\
#define _USE_MPC_\n\
#define MY_FLOAT  mpc_t\n\
#define MY_FLOAT_ROUNDING GMP_RNDN\n\
\n\
#define   InitMyFloat(r)            mpc_init3(r,MY_FLOAT_REAL_PREC,MY_FLOAT_IMAG_PREC)\n\
#define   ClearMyFloat(r)           mpc_clear(r)\n\
\n\
/* assign b to a */\n\
#define   AssignMyFloat(a, b)       mpc_set(a,b,MY_FLOAT_ROUNDING)\n\
\n\
/* create a MY_FLOAT from a, assigne to r. a is an int, float, or double complex */\n\
#define   MakeMyFloatA(r,a)         mpc_set_dc(r,(double complex)(a),MY_FLOAT_ROUNDING)\n\
\n\
/* create a MY_FLOAT from a, assign to r and return r */\n\
#define   MakeMyFloatB(r,a)         (MakeMyFloatA(r,a),r)\n\
\n\
/* create a MY_FLOAT from a, a is an integer or a float, s is its string representation */\n\
#define   MakeMyFloatC(r,s,a)       (mpc_set_str(r,s,10,MY_FLOAT_ROUNDING))\n\
/* addition r=a+b   */\n\
#define   AddMyFloatA(r,a,b)        mpc_add(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   AddMyFloatD(r,a,b)        {mpfr_t _mpc_aux_; mpfr_init_set_d(_mpc_aux_,b,MY_FLOAT_ROUNDING); \
                                     mpc_add_fr(r,(a),_mpc_aux_,MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   AddMyFloatSI(r,a,b)       {mpfr_t _mpc_aux_; mpfr_init_set_si(_mpc_aux_,b,MY_FLOAT_ROUNDING); \
                                     mpc_add_fr(r,(a),_mpc_aux_,MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
\n\
/* substraction r=a-b */\n\
#define   SubtractMyFloatA(r,a,b)   mpc_sub(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   SubtractMyFloatD(r,a,b)   {mpfr_t _mpc_aux_; mpfr_init_set_d(_mpc_aux_,b,MY_FLOAT_ROUNDING); \
                                     mpc_sub_fr(r,(a),_mpc_aux_,MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   SubtractMyFloatSI(r,a,b)  {mpfr_t _mpc_aux_; mpfr_init_set_si(_mpc_aux_,b,MY_FLOAT_ROUNDING); \
                                     mpc_sub_fr(r,(a),_mpc_aux_,MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   SubtractDMyFloat(r,a,b)  {mpfr_t _mpc_aux_; mpfr_init_set_d(_mpc_aux_,a,MY_FLOAT_ROUNDING); \
                                     mpc_fr_sub(r,_mpc_aux_,(b),MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   SubtractSIMyFloat(r,a,b)  {mpfr_t _mpc_aux_; mpfr_init_set_si(_mpc_aux_,a,MY_FLOAT_ROUNDING); \
                                     mpc_fr_sub(r,_mpc_aux_,(b),MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
\n\
/* multiplication r=a*b */\n\
#define   MultiplyMyFloatA(r,a,b)   mpc_mul(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   MultiplyMyFloatD(r,a,b)   {mpfr_t _mpc_aux_; mpfr_init_set_d(_mpc_aux_,b,MY_FLOAT_ROUNDING); \
                                     mpc_mul_fr(r,(a),_mpc_aux_,MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   MultiplyMyFloatSI(r,a,b)  mpc_mul_si(r,(a),(b),MY_FLOAT_ROUNDING)\n\
\n\
/* division r=a/b */\n\
#define   DivideMyFloatA(r,a,b)     mpc_div(r,(a),(b),MY_FLOAT_ROUNDING)\n\
#define   DivideMyFloatD(r,a,b)     {mpfr_t _mpc_aux_; mpfr_init_set_d(_mpc_aux_,b,MY_FLOAT_ROUNDING); \
                                     mpc_div_fr(r,(a),_mpc_aux_,MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   DivideMyFloatSI(r,a,b)    {mpc_div_ui(r,(a),(b),MY_FLOAT_ROUNDING);if (b<0) mpc_neg(r,r,MY_FLOAT_ROUNDING);}\n\
#define   DivideDMyFloat(r,a,b)     {mpfr_t _mpc_aux_; mpfr_init_set_d(_mpc_aux_,a,MY_FLOAT_ROUNDING); \
                                     mpc_fr_div(r,_mpc_aux_,(b),MY_FLOAT_ROUNDING); mpfr_clear(_mpc_aux_);}\n\
#define   DivideSIMyFloat(r,a,b)    {mpc_ui_div(r,(a),(b),MY_FLOAT_ROUNDING);if (a<0) mpc_neg(r,r,MY_FLOAT_ROUNDING);}\n\
\n\
/* division by an integer r=a/i */\n\
#define   DivideMyFloatByInt(r,a,i) mpc_div_ui(r,(a),(unsigned long)(i), MY_FLOAT_ROUNDING)\n\
/* negation r=-a*/\n\
#define   NegateMyFloatA(r,a)       mpc_neg(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
/* square root r=sqrt(a) */\n\
#define   sqrtMyFloatA(r,a)         mpc_sqrt(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
/* exponentiation r=a^b, b is an integer */\n\
#define   ExponentiateMyFloatIA(r,a,b)  mpc_pow_ui(r,(a),(unsigned long)(b),MY_FLOAT_ROUNDING)\n\
\n\
/* exponentiation r=a^b */\n\
#define   ExponentiateMyFloatA(r,a,b)  mpc_pow(r,(a),(b),MY_FLOAT_ROUNDING)\n\
/* sin(a)  r=sin(a) */\n\
#define   sinMyFloatA(r,a)          mpc_sin(r,(a),MY_FLOAT_ROUNDING)\n\
/* cos(a)  r=cos(a) */\n\
#define   cosMyFloatA(r,a)          mpc_cos(r,(a),MY_FLOAT_ROUNDING)\n\
/* tan(a)  r=tan(a) */\n\
#define   tanMyFloatA(r,a)          mpc_tan(r,(a),MY_FLOAT_ROUNDING)\n\
/* atan(a) r=atan(a) */\n\
#define   atanMyFloatA(r,a)         mpc_atan(r,(a),MY_FLOAT_ROUNDING)\n\
/* exp(a)  r=exp(a) */\n\
#define   expMyFloatA(r,a)          mpc_exp(r,(a),MY_FLOAT_ROUNDING)\n\
/* log(a)  r=log(a) */\n\
#define   logMyFloatA(r,a)          mpc_log(r,(a),MY_FLOAT_ROUNDING)\n\
/* sinh(a) r=sinh(a) */\n\
#define   sinhMyFloatA(r,a)         mpc_sinh(r,(a),MY_FLOAT_ROUNDING)\n\
/* cosh(a) r=cosh(a) */\n\
#define   coshMyFloatA(r,a)         mpc_cosh(r,(a),MY_FLOAT_ROUNDING)\n\
/* tanh(a) r=tanh(a) */\n\
#define   tanhMyFloatA(r,a)         mpc_tanh(r,(a),MY_FLOAT_ROUNDING)\n\
\n\
\n\
/* log10(a)  r=log10(a) */\n\
#define   log10MyFloatA(r,a)        mpc_log10(r,(a),MY_FLOAT_ROUNDING)\n\
/* fabs(a) r=fabs(a) */\n\
#define   fabsMyFloatA(r,a)         (mpfr_set_zero(mpc_imagref(r),+1),mpc_abs(mpc_realref(r),(a),MY_FLOAT_ROUNDING))\n\
\n\
/* convert to int */\n\
#define   MyFloatToInt(ir,fa)       (ir=(int)creal(mpc_get_dc(fa,MY_FLOAT_ROUNDING)))\n\
/* convert to double */\n\
#define   MyFloatToDouble(dr,fa)    dr=(double)creal(mpc_get_dc(fa,MY_FLOAT_ROUNDING))\n\
\n\
/* convert to double */\n\
#define   MyFloatToString(s,a)      mpfr_sprintf(s,\"(% .20R*e % .20R*e)\",MY_FLOAT_ROUNDING,mpc_realref(a),mpc_imagref(a))\n\
\n\
\n\
/* boolean operation for the REAL parts */\n\
#define   MyFloatA_GE_B(a,b)        (MPC_INEX_RE(mpc_cmp((a),(b)))>=0)\n\
#define   MyFloatA_GT_B(a,b)        (MPC_INEX_RE(mpc_cmp((a),(b)))>0)\n\
#define   MyFloatA_LE_B(a,b)        (MPC_INEX_RE(mpc_cmp((a),(b)))<=0)\n\
#define   MyFloatA_LT_B(a,b)        (MPC_INEX_RE(mpc_cmp((a),(b)))<0)\n\
#define   MyFloatA_EQ_B(a,b)        (MPC_INEX_RE(mpc_cmp((a),(b)))==0)\n\
#define   MyFloatA_NEQ_B(a,b)       (MPC_INEX_RE(mpc_cmp((a),(b)))!=0)\n\
\n\
\n\
/* boolean operation\n\
#define   MyFloatA_GE_B(a,b)        mpc_greaterequal_p((a),(b))\n\
#define   MyFloatA_GT_B(a,b)        mpc_greater_p((a),(b))\n\
#define   MyFloatA_LE_B(a,b)        mpc_lessequal_p((a),(b))\n\
#define   MyFloatA_LT_B(a,b)        mpc_less_p((a),(b))\n\
#define   MyFloatA_EQ_B(a,b)        mpc_equal_p((a),(b))\n\
#define   MyFloatA_NEQ_B(a,b)       ((mpc_equal_p((a),(b))) ? 0 : 1)\n\
#define   MyFloatA_CMP_B(a,b)       mpc_cmp((a),(b))\n\
#define   MyFloatA_CMPABS_B(a,b)    mpc_cmp_abs((a),(b))\n\
*/\n\
\n\
#define   OutputMyFloat3(file,fmt,a)  mpfr_fprintf(file,fmt,mpc_realref(a),mpc_imagref(a))\n\
#define   OutputMyFloat(a)            mpc_out_str(stdout,10,0,a,MY_FLOAT_ROUNDING),fprintf(stdout,\" \")\n\
\n\
#define   InputMyFloat3(file,fmt,a)   mpc_inp_str(a,file,NULL,10,MY_FLOAT_ROUNDING)\n\
#define   InputMyFloat(a)             InputMyFloat3(stdin,NULL,a)\n\
\n\
#define   StringToMyFloat4(s,fmt,a,n) (mpc_strtoc(a,s,&s,10,MY_FLOAT_ROUNDING),s)\n\
#define   StringToMyFloat3(s,fmt,a)   mpc_set_str(a,s,10,MY_FLOAT_ROUNDING)\n\
#define   StringToMyFloat(s,a)        StringToMyFloat3(s,NULL,a)\n\
\n\
/************************************************************************/\n\
";

