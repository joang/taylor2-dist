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
#ifndef MY_COEF_MY_FLOAT_H
#define MY_COEF_MY_FLOAT_H

/* *******  ***********************  ******* */
/* *******  MY_COEF_MY_FLOAT_HEADER  ******* */
/* *******  ***********************  ******* */

#define MY_COEF_MY_FLOAT_FIXES MY_COEF_SPACE

#define MY_COEF_MY_FLOAT_PREHEADER(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
#define MY_COEF MY_FLOAT\n\
" \

#define MY_COEF_MY_FLOAT_HEADER(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" \

#define MY_COEF_MY_FLOAT_POSTHEADER(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" \

/* *******  *********************  ******* */
/* *******  MY_COEF_MY_FLOAT_CODE  ******* */
/* *******  *********************  ******* */

#define MY_COEF_MY_FLOAT_PRECODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* BEGIN PRECODE " PREFIX_COEF(t) " */\n\
#define " PREFIX_COEF(t) "          MY_COEF\n\
/* END PRECODE " PREFIX_COEF(t) " */\n\
\n\
" \


#define MY_COEF_MY_FLOAT_BASIC(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* BEGIN MY_COEF_MY_FLOAT_BASIC " PREFIX_COEF(t) " */\n\
#define " PREFIX_COEF(initup2) "(s,d)    \n\
#define " PREFIX_COEF(initup) "()        \n\
#define " PREFIX_COEF(init) "            InitMyFloat\n\
#define " PREFIX_COEF(clean) "           ClearMyFloat\n\
#define " PREFIX_COEF(cleanup) "()       \n\
\n\
/* assign */\n\
#define " PREFIX_COEF(set) "             AssignMyFloat\n\
#define " PREFIX_COEF(set_myfloat) "     AssignMyFloat\n\
#define " PREFIX_COEF(set_d) "           MakeMyFloatB\n\
#define " PREFIX_COEF(set_si) "          MakeMyFloatB\n\
#define " PREFIX_COEF(set_zero) "(r)     MakeMyFloatC(r,\"0\",0)\n\
\n\
/* elemental operations */\n\
#define " PREFIX_COEF(add2) "            AddMyFloatA\n\
#define " PREFIX_COEF(sub2) "            SubtractMyFloatA\n\
#define " PREFIX_COEF(mul2) "            MultiplyMyFloatA\n\
#define " PREFIX_COEF(div2) "            DivideMyFloatA\n\
\n\
#define " PREFIX_COEF(add2_myfloat) "    AddMyFloatA\n\
#define " PREFIX_COEF(sub2_myfloat) "    SubtractMyFloatA\n\
#define " PREFIX_COEF(mul2_myfloat) "    MultiplyMyFloatA\n\
#define " PREFIX_COEF(div2_myfloat) "    DivideMyFloatA\n\
#define " PREFIX_COEF(myfloat_sub2) "    SubtractMyFloatA\n\
#define " PREFIX_COEF(myfloat_div2) "    DivideMyFloatA\n\
\n\
#define " PREFIX_COEF(add2_d) "          AddMyFloatD\n\
#define " PREFIX_COEF(sub2_d) "          SubtractMyFloatD\n\
#define " PREFIX_COEF(mul2_d) "          MultiplyMyFloatD\n\
#define " PREFIX_COEF(div2_d) "          DivideMyFloatD\n\
#define " PREFIX_COEF(d_sub2) "          SubtractDMyFloat\n\
#define " PREFIX_COEF(d_div2) "          DivideDMyFloat\n\
\n\
#define " PREFIX_COEF(add2_si) "         AddMyFloatSI\n\
#define " PREFIX_COEF(sub2_si) "         SubtractMyFloatSI\n\
#define " PREFIX_COEF(mul2_si) "         MultiplyMyFloatSI\n\
#define " PREFIX_COEF(div2_si) "         DivideMyFloatSI\n\
#define " PREFIX_COEF(si_sub2) "         SubtractSIMyFloat\n\
#define " PREFIX_COEF(si_div2) "         DivideSIMyFloat\n\
\n\
#define " PREFIX_COEF(neg) "             NegateMyFloatA\n\
\n\
/* built-in functions */\n\
#define " PREFIX_COEF(set_sqrt) "        sqrtMyFloatA\n\
#define " PREFIX_COEF(set_pow_myfloat) " ExponentiateMyFloatA\n\
#define " PREFIX_COEF(set_pow_si) "      ExponentiateMyFloatIA\n\
\n\
#define " PREFIX_COEF(set_sin) "         sinMyFloatA\n\
#define " PREFIX_COEF(set_cos) "         cosMyFloatA\n\
#define " PREFIX_COEF(set_tan) "         tanMyFloatA\n\
#define " PREFIX_COEF(set_atan) "        atanMyFloatA\n\
#define " PREFIX_COEF(set_exp) "         expMyFloatA\n\
#define " PREFIX_COEF(set_log) "         logMyFloatA\n\
#define " PREFIX_COEF(set_sinh) "        sinhMyFloatA\n\
#define " PREFIX_COEF(set_cosh) "        coshMyFloatA\n\
#define " PREFIX_COEF(set_tanh) "        tanhMyFloatA\n\
#define " PREFIX_COEF(set_log10) "       log10MyFloatA\n\
#define " PREFIX_COEF(set_fabs) "        fabsMyFloatA\n\
\n\
/* castings */\n\
#define " PREFIX_COEF(to_myfloat) "(a)   &(a)\n\
#define " PREFIX_COEF(to_si) "           MyFloatToInt\n\
#define " PREFIX_COEF(to_d) "            MyFloatToDouble\n\
\n\
/* boolean operations */\n\
#define " PREFIX_COEF(ge) "  MyFloatA_GE_B\n\
#define " PREFIX_COEF(gt) "  MyFloatA_GT_B\n\
#define " PREFIX_COEF(le) "  MyFloatA_LE_B\n\
#define " PREFIX_COEF(lt) "  MyFloatA_LT_B\n\
#define " PREFIX_COEF(eq) "  MyFloatA_EQ_B\n\
#define " PREFIX_COEF(neq) " MyFloatA_NEQ_B\n\
\n\
/* norms */\n\
#define " PREFIX_COEF(nrminf) "(a,b) " PREFIX_COEF(set_fabs) "(*(a),b)\n\
#define " PREFIX_COEF(nrm2) "(a,b) " PREFIX_COEF(mul2) "(*(a),b,b)\n\
\n\
/* output format */\n\
#define " PREFIX_COEF(fprintf) " OutputMyFloat3\n\
\n\
/* input format */\n\
#define " PREFIX_COEF(fscanf) " InputMyFloat3\n\
\n\
/* string scanf */\n\
#define " PREFIX_COEF(sscanf4) " StringToMyFloat4\n\
/* END MY_COEF_MY_FLOAT_BASIC " PREFIX_COEF(t) " */\n\
\n" \


#define MY_COEF_MY_FLOAT_CODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_COEF_MY_FLOAT_BASIC(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_COEF_MY_FLOAT_POSTCODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* BEGIN MY_COEF_MY_FLOAT_POSTCODE " PREFIX_COEF(t) " */\n\
" MY_COEF_MACROS_MY_FLOAT(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) " \n\
/* END MY_COEF_MY_FLOAT_POSTCODE " PREFIX_COEF(t) " */\n\
" \

#endif /* MY_COEF_MY_FLOAT_H */
