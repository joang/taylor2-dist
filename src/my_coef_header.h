#ifndef MY_COEF_HEADER_H
#define MY_COEF_HEADER_H

typedef enum {\
  EMPTY=0,\
  NON_EMPTY=1, \
  SYMBOLS=2, \
  DEGREE=4, \
  AS_MYJET=8 } my_coef_flags_t;

typedef enum { \
  PREFIX_MY_FLOAT=0, \
  PREFIX_DOUBLE, \
  PREFIX_COMPLEX, \
  PREFIX_MYCOEF_TREE=0, \
  PREFIX_MYCOEF1_1, \
  PREFIX_MYCOEF_1, \
  PREFIX_MYCOEF1, \
  PREFIX_MYCOEF2, \
  PREFIX_MYCOEF_2, \
  PREFIX_MYCOEF_m, \
  PREFIX_MYCOEF } index_my_coef_prefix_t;

typedef enum { \
  HEADER_MY_FLOAT=0, \
  HEADER_DOUBLE, \
  HEADER_COMPLEX, \
  HEADER_COEF_TREE=0, \
  HEADER_COEF1_1, \
  HEADER_COEF_1, \
  HEADER_COEF1, \
  HEADER_COEF2, \
  HEADER_COEF_2, \
  HEADER_COEF_m, \
  HEADER_COEF } index_my_coef_header_t;
typedef enum { \
  CODE_MY_FLOAT=0, \
  CODE_DOUBLE, \
  CODE_COMPLEX, \
  CODE_COEF_TREE=0, \
  CODE_COEF1_1, \
  CODE_COEF_1, \
  CODE_COEF1, \
  CODE_COEF2, \
  CODE_COEF_2, \
  CODE_COEF_m, \
  CODE_COEF} index_my_coef_code_t;

#define MY_COEF_MAX_NUM_SYMB_MACRO_NAME "_MY_COEF_MAX_NUM_SYMBOLS_"
#define MY_COEF_MAX_DEGREE_MACRO_NAME "_MY_COEF_MAX_DEGREE_"
#define MY_COEF_TOTAL_COEFFS_MACRO_NAME "_MY_COEF_TOTAL_COEFFICIENTS_COUNT_"

#define MY_COEF_DATA_ACCESS "MY_COEF_DATA"
#define MY_COEF_RECURSION(f)

/* *******  **************  ******* */
/* *******  MY_COEF_MACROS  ******* */
/* *******  **************  ******* */
#define MY_COEF_MACROS_GENERAL(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,FLAG_INIT_CLEAN) "\
/* MACROS TO LINK MY_COEF */\n\
#ifndef mycoef_t \n\
#define mycoef_t " PREFIX_MYCOEF(t) "\n\
\n\
#define mycoef_initup() " PREFIX_MYCOEF(initup2) "(" MAX_NUM_SYMBOLS_NAME "," MAX_DEGREE_NAME ")\n\
#define mycoef_init(a)  " PREFIX_MYCOEF(init) "(" FLAG_INIT_CLEAN "(a))\n\
#define mycoef_clean(a) " PREFIX_MYCOEF(clean) "(" FLAG_INIT_CLEAN "(a))\n\
#define mycoef_cleanup  " PREFIX_MYCOEF(cleanup) "\n\
\n\
/* norm functions */\n\
#define mycoef_nrminf " PREFIX_MYCOEF(nrminf) "\n\
#define mycoef_nrm2 " PREFIX_MYCOEF(nrm2) "\n\
\n\
/* casts */\n\
#define mycoef_to_myfloat " PREFIX_MYCOEF(to_myfloat) "\n\
\n\
/* assign */\n\
#define mycoef_set " PREFIX_MYCOEF(set) "\n\
#define mycoef_set_zero " PREFIX_MYCOEF(set_zero) "\n\
#define mycoef_set_myfloat " PREFIX_MYCOEF(set_myfloat) "\n\
#define mycoef_set_d " PREFIX_MYCOEF(set_d) "\n\
#define mycoef_set_si " PREFIX_MYCOEF(set_si) "\n\
\n\
/* elemental operations */\n\
#define mycoef_add2 " PREFIX_MYCOEF(add2) "\n\
#define mycoef_sub2 " PREFIX_MYCOEF(sub2) "\n\
#define mycoef_mul2 " PREFIX_MYCOEF(mul2) "\n\
#define mycoef_div2 " PREFIX_MYCOEF(div2) "\n\
\n\
#define mycoef_add2_myfloat " PREFIX_MYCOEF(add2_myfloat) "\n\
#define mycoef_sub2_myfloat " PREFIX_MYCOEF(sub2_myfloat) "\n\
#define mycoef_myfloat_sub2 " PREFIX_MYCOEF(myfloat_sub2) "\n\
#define mycoef_mul2_myfloat " PREFIX_MYCOEF(mul2_myfloat) "\n\
#define mycoef_div2_myfloat " PREFIX_MYCOEF(div2_myfloat) "\n\
#define mycoef_myfloat_div2 " PREFIX_MYCOEF(myfloat_div2) "\n\
\n\
#define mycoef_add2_d " PREFIX_MYCOEF(add2_d) "\n\
#define mycoef_sub2_d " PREFIX_MYCOEF(sub2_d) "\n\
#define mycoef_mul2_d " PREFIX_MYCOEF(mul2_d) "\n\
#define mycoef_div2_d " PREFIX_MYCOEF(div2_d) "\n\
#define mycoef_d_sub2 " PREFIX_MYCOEF(d_sub2) "\n\
#define mycoef_d_div2 " PREFIX_MYCOEF(d_div2) "\n\
\n\
#define mycoef_add2_si " PREFIX_MYCOEF(add2_si) "\n\
#define mycoef_sub2_si " PREFIX_MYCOEF(sub2_si) "\n\
#define mycoef_mul2_si " PREFIX_MYCOEF(mul2_si) "\n\
#define mycoef_div2_si " PREFIX_MYCOEF(div2_si) "\n\
#define mycoef_si_sub2 " PREFIX_MYCOEF(si_sub2) "\n\
#define mycoef_si_div2 " PREFIX_MYCOEF(si_div2) "\n\
\n\
#define mycoef_neg " PREFIX_MYCOEF(neg) "\n\
\n\
/* built-in functions */\n\
#define mycoef_set_sqrt " PREFIX_MYCOEF(set_sqrt) "\n\
#define mycoef_set_pow_myfloat " PREFIX_MYCOEF(set_pow_myfloat) "\n\
#define mycoef_set_pow_si " PREFIX_MYCOEF(set_pow_si) "\n\
\n\
#define mycoef_set_sin " PREFIX_MYCOEF(set_sin) "\n\
#define mycoef_set_cos " PREFIX_MYCOEF(set_cos) "\n\
#define mycoef_set_tan " PREFIX_MYCOEF(set_tan) "\n\
#define mycoef_set_atan " PREFIX_MYCOEF(set_atan) "\n\
#define mycoef_set_exp " PREFIX_MYCOEF(set_exp) "\n\
#define mycoef_set_log " PREFIX_MYCOEF(set_log) "\n\
#define mycoef_set_sinh " PREFIX_MYCOEF(set_sinh) "\n\
#define mycoef_set_cosh " PREFIX_MYCOEF(set_cosh) "\n\
#define mycoef_set_tanh " PREFIX_MYCOEF(set_tanh) "\n\
#define mycoef_set_log10 " PREFIX_MYCOEF(set_log10) "\n\
#define mycoef_set_fabs " PREFIX_MYCOEF(set_fabs) "\n\
\n\
/* castings */\n\
#define mycoef_to_si " PREFIX_MYCOEF(to_si) "\n\
#define mycoef_to_d " PREFIX_MYCOEF(to_d) "\n\
\n\
/* boolean operations */\n\
#define mycoef_ge " PREFIX_MYCOEF(ge) "\n\
#define mycoef_gt " PREFIX_MYCOEF(gt) "\n\
#define mycoef_le " PREFIX_MYCOEF(le) "\n\
#define mycoef_lt " PREFIX_MYCOEF(lt) "\n\
#define mycoef_eq " PREFIX_MYCOEF(eq) "\n\
#define mycoef_neq " PREFIX_MYCOEF(neq) "\n\
\n\
/* output format */\n\
#define mycoef_fprintf " PREFIX_MYCOEF(fprintf) "\n\
\n\
/* input format */\n\
#define mycoef_fscanf " PREFIX_MYCOEF(fscanf) "\n\
\n\
/* string scanf */\n\
#define mycoef_sscanf4 " PREFIX_MYCOEF(sscanf4) "\n\
#endif /* END my_coef_t */\n\
\n" \

#define InitCleanFlag "&"
#define InitCleanFlagStruct ""
#define MY_COEF_MACROS(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) \
 MY_COEF_MACROS_GENERAL(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,InitCleanFlag)

#define MY_COEF_MACROS_MY_FLOAT(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) \
 MY_COEF_MACROS_GENERAL(PREFIX_MYCOEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,InitCleanFlagStruct)

#define MY_COEF_FIXES(x) "mycoef_" #x

#define MY_COEF_PREFIX "__COEF_NAME_"
#define MY_COEF_SUFFIX "_COEF_ODE_NAME__"
#define MY_COEF_SPACE(x) MY_COEF_PREFIX "_" #x "_" MY_COEF_SUFFIX
#define MY_COEF_FUN(x) "MY_COEF_FUN(" #x ")"

#endif /* MY_COEF_HEADER_H */
