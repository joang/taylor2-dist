#ifndef MY_COEF_JET_TREE_H
#define MY_COEF_JET_TREE_H

#include "my_jet_tree.h"
#include "my_coef_MY_FLOAT.h"

/* *******  ***********************  ******* */
/* *******  MY_COEF_JET_TREE_HEADER  ******* */
/* *******  ***********************  ******* */

#define MY_COEF_JET_TREE_FIXES MY_COEF_SPACE
#define MY_COEF_JET_TREE_MY_COEF_FIXES(x) "myfloat_" #x

#define MY_COEF_TREE_FIXES(x) MY_COEF_JET_TREE_FIXES(x) "_node"

#define MY_COEF_TREE_TYPE MY_TREE_TYPE1

#define MY_COEF_JET_TREE_TYPE(PREFIX_JET,COEF,I) \
  MY_JET_TREE_TYPE1(PREFIX_JET,MY_COEF_TREE_TYPE,MY_COEF_TREE_FIXES,COEF,I)


#define MY_COEF_JET_TREE_PREHEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET_TREE_PREHEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
" \

#define MY_COEF_JET_TREE_HEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET_TREE_HEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
" \

#define MY_COEF_JET_TREE_POSTHEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET_TREE_POSTHEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_COEF,MYCOEF,MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
#define MY_COEF " PREFIX_JET_TREE(t) "\n\
" \

/* *******  *********************  ******* */
/* *******  MY_COEF_JET_TREE_CODE  ******* */
/* *******  *********************  ******* */

#define MY_COEF_JET_TREE_PRECODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_COEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* BEGIN MY_COEF_JET_TREE_PRECODE */\n\
" MY_COEF_MY_FLOAT_CODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
\n\
" MY_JET_TREE_PRECODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_COEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
/* END MY_COEF_JET_TREE_PRECODE */\n\
" \

#define MY_COEF_JET_TREE_CODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_COEF,PREFIX_MYFLOAT,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,MYDATA_ACCESS,MY_RECURSION) "\
/* BEGIN MY_COEF_JET_TREE_CODE */\n\
" MY_JET_TREE_CODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_COEF,PREFIX_MYFLOAT,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,MYDATA_ACCESS,MY_RECURSION) "\n\
/* END MY_COEF_JET_TREE_CODE */\n\
" \

#define MY_COEF_JET_TREE_POSTCODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_COEF,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* BEGIN MY_COEF_JET_TREE_POSTCODE */\n\
" MY_JET_TREE_POSTCODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_COEF,I,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
\n\
" MY_COEF_MACROS(PREFIX_JET_TREE,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) "\n\
/* END MY_COEF_JET_TREE_POSTCODE */\n\
" \

#endif /* MY_COEF_JET_TREE_H */
