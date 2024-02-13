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

/******************************************************************
 *
 *     functions manipulating nodes
 *
 *****************************************************************/
#include <stdio.h>
#define MY_JET_CODE_C
#include "Header.h"

/****************************************************************/
int num_jet_vars=0;
int num_jet_symbols=0;
int deg_jet_vars=0;
int state_jet_vars=0;
int param_jet_vars=0;
int total_jet_vars=0;
int all_vars_are_jets=0;

/***********************************************************************
 The following code are for internal testing only,it implements
 series arithmetic of degree 1 and 2. 
***********************************************************************/

#include "my_jet_header.h"

/* *******  ***********  ******* */
/* *******  MY_JET_NONE  ******* */
/* *******  ***********  ******* */
#include "my_jet_none.h"

/* *******  *********  ******* */
/* *******  MY_JET1_1  ******* */
/* *******  *********  ******* */
#include "my_jet1_1.h"

/* *******  ********  ******* */
/* *******  MY_JET_1  ******* */
/* *******  ********  ******* */
#include "my_jet_1.h"

/* *******  *******  ******* */
/* *******  MY_JET1  ******* */
/* *******  *******  ******* */
#include "my_jet1.h"

/* *******  *******  ******* */
/* *******  MY_JET2  ******* */
/* *******  *******  ******* */
#include "my_jet2.h"


/* *******  *******  ******* */
/* *******  MY_JET_2 ******* */
/* *******  *******  ******* */
#include "my_jet_2.h"

/* *******  *******  ******* */
/* *******  MY_JET_m ******* */
/* *******  *******  ******* */
#include "my_jet_m.h"

/* *******  ***********  ******* */
/* *******  MY_JET_TREE  ******* */
/* *******  ***********  ******* */
#include "my_jet_tree.h"

#include "cppwrapper.h"

char *my_jet_prefixes[] = {\
 "jet_none",\
 "jet1_1", \
 "jet_1", \
 "jet1", \
 "jet2", \
 "jet_2", \
 "jet_m", \
 "jet_tree",\
 "jet1_1_bis", \
 "jet"
};

char *my_jet_myblas_prefixes[] = {\
 "jet_none_vec",\
 "jet1_1_vec", \
 "jet_1_vec", \
 "jet1_vec", \
 "jet2_vec", \
 "jet_2_vec", \
 "jet_m_vec", \
 "jet_tree_vec",\
 "jet1_1_bis_vec", \
 "jet_vec"
};

char *my_float_api_macros = \
    MY_FLOAT_MACROS(MY_FLOAT_FIXES);


#define InitCleanFlag "&"
#define InitCleanFlagStruct ""
char *my_jet_preheaders[]= { \
  MY_JET_NONE_PREHEADER(NONE), \
  \
  MY_JET1_1_PREHEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                      MY_COEF_FUN, "MY_COEF", "MY_FLOAT", "int", \
                      MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_1_PREHEADER(MY_JET_1_FIXES, MY_JET_1_TYPE, \
                     MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                     MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET1_PREHEADER(MY_JET1_FIXES, MY_JET1_TYPE, \
                    MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                    MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET2_PREHEADER(MY_JET2_FIXES, MY_JET2_TYPE, \
                    MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                    MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET_2_PREHEADER(MY_JET_2_FIXES, MY_JET_2_TYPE, \
                     MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                     MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),  \
  \
  MY_JET_m_PREHEADER(MY_JET_m_FIXES, MY_JET_m_TYPE, \
                     MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                     MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_TREE_PREHEADER(MY_JET_TREE_FIXES, MY_JET_TREE_TYPE, \
                        MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                        MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET1_1_PREHEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                      MY_COEF_FUN, "MY_COEF", "MY_FLOAT", "int", \
                      MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
};
char *my_jet_headers[]= { \
  MY_JET_NONE_HEADER(NONE), \
  \
  MY_JET1_1_HEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                   MY_COEF_FUN, "MY_COEF", "MY_FLOAT", "int", \
                   MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME,InitCleanFlagStruct) \
  "\n" \
  MY_JET_MACROS(MY_JET1_1_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET_1_HEADER(MY_JET_1_FIXES, MY_JET_1_TYPE, \
                  MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME) \
  "\n" \
  MY_JET_MACROS(MY_JET_1_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET1_HEADER(MY_JET1_FIXES, MY_JET1_TYPE, \
                 MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                 MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME) \
  "\n" \
  MY_JET_MACROS(MY_JET1_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET2_HEADER(MY_JET2_FIXES, MY_JET2_TYPE, \
                 MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                 MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME) \
  "\n" \
  MY_JET_MACROS(MY_JET2_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET_2_HEADER(MY_JET_2_FIXES, MY_JET_2_TYPE, \
                  MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME) \
  "\n" \
  MY_JET_MACROS(MY_JET_2_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET_m_HEADER(MY_JET_m_FIXES, MY_JET_m_TYPE, \
                  MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME) \
  "\n" \
  MY_JET_MACROS(MY_JET_m_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET_TREE_HEADER(MY_JET_TREE_FIXES, MY_JET_TREE_TYPE, \
                     MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                     MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME) \
  "\n" \
  MY_JET_MACROS(MY_JET_TREE_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
  \
  MY_JET1_1_HEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                   MY_COEF_FUN, "MY_COEF", "MY_FLOAT", "int", \
                   MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME,InitCleanFlag) \
  "\n" \
  MY_JET_MACROS(MY_JET1_1_FIXES,InitCleanFlag,MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME), \
};
char *my_jet_postheaders[]= { \
  MY_JET_NONE_POSTHEADER(NONE), \
  \
  MY_JET1_1_POSTHEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                       MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                       MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_1_POSTHEADER(MY_JET_1_FIXES, MY_JET_1_TYPE, \
                      MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                      MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET1_POSTHEADER(MY_JET1_FIXES, MY_JET1_TYPE, \
                     MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                     MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET2_POSTHEADER(MY_JET2_FIXES, MY_JET2_TYPE, \
                     MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                     MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_2_POSTHEADER(MY_JET_2_FIXES, MY_JET_2_TYPE, \
                      MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                      MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_m_POSTHEADER(MY_JET_m_FIXES, MY_JET_m_TYPE, \
                      MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                      MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_TREE_POSTHEADER(MY_JET_TREE_FIXES, MY_JET_TREE_TYPE, \
                         MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                         MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET1_1_POSTHEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                       MY_COEF_FIXES, "MY_COEF", "MY_FLOAT", "int", \
                       MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
};

char *my_jet_precodes[] = { \
  MY_JET_NONE_PRECODE(NONE), \
  \
  MY_JET1_1_PRECODE(MY_JET1_1_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_1_PRECODE(MY_JET_1_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET1_PRECODE(MY_JET1_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET2_PRECODE(MY_JET2_FIXES, MY_MP2_FIXES,MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET_2_PRECODE(MY_JET_2_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_m_PRECODE(MY_JET_m_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_TREE_PRECODE(MY_JET_TREE_FIXES,MY_TREE_FIXES,MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET1_1_PRECODE(MY_JET1_1_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
};
char *my_jet_codes[] = { \
  MY_JET_NONE_CODE(NONE), \
  \
  MY_JET1_1_CODE(MY_JET1_1_FIXES,MY_COEF_FUN, MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME,InitCleanFlagStruct),\
  \
  MY_JET_1_CODE(MY_JET_1_FIXES, MY_COEF_FIXES,MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME, MY_JET_DATA_ACCESS),\
  \
  MY_JET1_CODE(MY_JET1_FIXES, MY_COEF_FIXES,MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME, MY_JET_DATA_ACCESS),\
  \
  MY_JET2_CODE(MY_JET2_FIXES, MY_MP2_FIXES,MY_COEF_FIXES,MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME, MY_JET_DATA_ACCESS),\
  \
  MY_JET_2_CODE(MY_JET_2_FIXES, MY_COEF_FIXES,MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME, MY_JET_DATA_ACCESS), \
  \
  MY_JET_m_CODE(MY_JET_m_FIXES, MY_COEF_FIXES,MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_TREE_CODE(MY_JET_TREE_FIXES,MY_TREE_FIXES,MY_COEF_FIXES,MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME, MY_JET_DATA_ACCESS,MY_JET_RECURSION), \
  \
  MY_JET1_1_CODE(MY_JET1_1_FIXES,MY_COEF_FUN, MY_FLOAT_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME,InitCleanFlag),\
};
char *my_jet_postcodes[] = { \
  MY_JET_NONE_POSTCODE(NONE), \
  \
  MY_JET1_1_POSTCODE(MY_JET1_1_FIXES, MY_COEF_FUN, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET_1_POSTCODE(MY_JET_1_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET1_POSTCODE(MY_JET1_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET2_POSTCODE(MY_JET2_FIXES, MY_MP2_FIXES,MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
  \
  MY_JET_2_POSTCODE(MY_JET_2_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_m_POSTCODE(MY_JET_m_FIXES, MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET_TREE_POSTCODE(MY_JET_TREE_FIXES,MY_TREE_FIXES,MY_COEF_FIXES, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_JET1_1_POSTCODE(MY_JET1_1_FIXES, MY_COEF_FUN, "int", \
  MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME),\
};

char *my_jet_myblas_headers[]= { \
  "",\
  MY_BLAS_HEADER(MY_JET1_1_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET_1_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET1_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET2_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET_2_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET_m_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET_TREE_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
  MY_BLAS_HEADER(MY_JET1_1_FIXES,"MY_JET","MY_COEF","MY_FLOAT","int"), \
};

char *my_jet_myblas_codes[]= { \
  "",\
  MY_BLAS_CODE(MY_JET1_1_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET_1_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET1_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET2_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET_2_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET_m_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET_TREE_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
  MY_BLAS_CODE(MY_JET1_1_FIXES,"MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FUN,"MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(jaux)"), \
};
#define MY_JET_MYBLAS_PREFIX MY_JET_PREFIX
#define MY_JET_MYBLAS_SUFFIX MY_JET_SUFFIX
#define MY_JET_MYBLAS_SPACE(x) MY_JET_MYBLAS_PREFIX "_" #x "_" MY_JET_MYBLAS_SUFFIX
#define MY_JET_MYBLAS_FUN(x) "MY_JET_MYBLAS_FUN(" #x ")"

#define MY_JET_MYBLAS_FUN_MACRO "#define " MY_JET_MYBLAS_FUN(x) " " MY_JET_MYBLAS_PREFIX "_ ## x ## _" MY_JET_MYBLAS_SUFFIX

char *my_jet_cppwrapper[]= { \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_NONE_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_NONE_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_NONE_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_NONE_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_1_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_1_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_1_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_1_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_1_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_1_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_1_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_1_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET1_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET2_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET2_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET2_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET2_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_2_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_2_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_2_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_2_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_m_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_m_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_m_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_m_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
\
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_TREE_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_NONE_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_TREE_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"),
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_TREE_FIXES,CPP_NONE_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
  CPP_WRAPPER("MyJet","MY_JET",MY_JET_TREE_FIXES,CPP_MY_COEF_WRAPPER,"MY_COEF",CPP_MY_FLOAT_WRAPPER,"MY_FLOAT"), \
};

static char *words[] = {\
  MY_JET_PREFIX, MY_JET_SUFFIX, \
};
#define MY_JET_FUN_MACRO "#define " MY_JET_FUN(x) " " MY_JET_PREFIX "_ ## x ## _" MY_JET_SUFFIX

void print_and_subs(char *word[], const char *str, const char *prefix, const char *suffix)
{
  if(str == NULL) return;
  int length = strlen(str);
  int wlen[2] = {strlen(word[0]),strlen(word[1])};
  int i, location = 0;

  while (matchWord(str, word[0], length, &location))
    {
      for(i = 0; i < location; i++) {fputc(str[i], outfile);}
      str += location + wlen[0];
      length -= (location + wlen[0]);
      fprintf(outfile, "%s", prefix);

      matchWord(str, word[1], length, &location);
      for(i = 0; i < location; i++) {fputc(str[i], outfile);}
      str += location + wlen[1];
      length -= (location + wlen[1]);
      fprintf(outfile, "%s", suffix);
    }
  fprintf(outfile, "%s", str);
}


#include <limits.h>
void print_jet_tree_num_coef_homog_table(const char *prefix, const char *suffix, int nsymb, int deg)
{
  int **nch=NULL, m, k;

  /* memory allocation */
  k = (nsymb+1)*sizeof(__typeof__(*nch));
  nch = (__typeof__(nch)) malloc(k);
  if (nch == NULL)
    {
      fprintf(stderr, "%s:%d not enough memory to allocate %d\n", __FILE__, __LINE__, k);
      fflush(stderr); exit(2);
    }
  for (m = 0; m <= nsymb; ++m)
    {
      k = (deg+1)*sizeof(__typeof__(**nch));
      nch[m] = (__typeof__(*nch)) malloc(k);
      if (nch[m] == NULL)
        {
          fprintf(stderr, "%s:%d not enough memory to allocate %d\n", __FILE__, __LINE__, k);
          fflush(stderr); exit(2);
        }
    }

  /* table computation */
  nch[0][0] = 1;
  for (k = 1; k <= deg; ++k) {nch[0][k] = 0;}
  for (m = 1; m <= nsymb; ++m)
    {
      nch[m][0]= 1;
      for (k = 1; k <= deg; ++k)
        {
          nch[m][k]= nch[m][k-1] + nch[m-1][k];
          if (nch[m][k]> UINT_MAX - nch[m-1][k])
            {
              fprintf(stderr, "%s:%d impossible to generate table for nsymb=%d and deg=%d\n",
                      __FILE__, __LINE__, nsymb, deg);
              fflush(stderr); exit(3);
            }
        }
    }

  /* printing */
  fprintf(outfile, "/*table for %d symbols and degree %d*/\n",nsymb, deg);
  fprintf(outfile, "static int %s_num_coefs_homogeneous_%s[] = {\\\n",prefix,suffix);
  for (m = 0; m <= nsymb; ++m)
    {
      for (k = 0; k <= deg; k++)
        {
          fprintf(outfile, "%d, ", nch[m][k]);
        }
      fprintf(outfile, "\\\n");
    }
  fprintf(outfile, "};\n");

  /* deallocate memory */
  for (m = 0; m <= nsymb; ++m) {free(nch[m]); nch[m]=NULL;}
  free(nch); nch=NULL;
}

int setMyJetIndexes(void) {
  if (my_jet_arith == ARITH_JET_NONE)
    {
      index_my_jet_prefix = PREFIX_NONE;
      index_my_jet_header = HEADER_NONE;
      index_my_jet_code = CODE_NONE;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* No jet */\n");
    }
  else if (my_jet_arith == ARITH_JET1_1)
    {
      index_my_jet_prefix = PREFIX_JET1_1;
      index_my_jet_header = HEADER_JET1_1;
      index_my_jet_code = CODE_JET1_1;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* Using jet lib: jet1_1. one symbol, degree one */\n");
    }
  else if (my_jet_arith == ARITH_JET1)
    {
      index_my_jet_prefix = PREFIX_JET1;
      index_my_jet_header = HEADER_JET1;
      index_my_jet_code = CODE_JET1;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* Using jet lib: jet1. one symbol, degree n */\n");
    }
  else if (my_jet_arith == ARITH_JET_1)
    {
      index_my_jet_prefix = PREFIX_JET_1;
      index_my_jet_header = HEADER_JET_1;
      index_my_jet_code = CODE_JET_1;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* Using jet lib: jet_1. n symbol, degree 1 */\n");
    }
  else if (my_jet_arith == ARITH_JET2)
    {
      index_my_jet_prefix = PREFIX_JET2;
      index_my_jet_header = HEADER_JET2;
      index_my_jet_code = CODE_JET2;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      jetStorageType=1;
      fprintf(outfile, "/* Using jet lib: jet2. 2 symbol, degree n */\n");
    }
  else if (my_jet_arith == ARITH_JET_2)
    {
      index_my_jet_prefix = PREFIX_JET_2;
      index_my_jet_header = HEADER_JET_2;
      index_my_jet_code = CODE_JET_2;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* Using jet lib: jet_2. n symbol, degree 2 */\n");
    }
  else if (my_jet_arith == ARITH_JET_m)
    {
      index_my_jet_prefix = PREFIX_JET_m;
      index_my_jet_header = HEADER_JET_m;
      index_my_jet_code = CODE_JET_m;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* Using jet lib: jet_m. m symbol, degree n */\n");
    }
  else if (my_jet_arith == ARITH_JET_TREE)
    {
      index_my_jet_prefix = PREFIX_JET_TREE;
      index_my_jet_header = HEADER_JET_TREE;
      index_my_jet_code = CODE_JET_TREE;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      jetStorageType=2;
      fprintf(outfile, "/* Using jet lib: jet_tree. m symbol, degree n */\n");
    }
  else if (my_jet_arith == ARITH_JET1_1_BIS)
    {      
      index_my_jet_prefix = PREFIX_JET1_1_BIS;
      index_my_jet_header = HEADER_JET1_1_BIS;
      index_my_jet_code = CODE_JET1_1_BIS;
      my_jet_prefix = my_jet_prefixes[index_my_jet_prefix];
      fprintf(outfile, "/* Using jet lib: jet1_1_bis. one symbol, degree one */\n");
    }
  else
    {
      /* jet lib invalid */
      return 1;
    }
  return 0;
}

void genMyJetHeader(void) {
  if (outfile == NULL) return;
  fprintf(outfile, "\n/*** MY_JET ***/\n#ifndef _MY_JET_H_\n#define _MY_JET_H_\n\n");

  //20200615
  if (my_jet_arith == ARITH_JET_NONE) {}
  else if(jetStorageType==0) {
    fprintf(outfile, "#define " MY_JET_DATA_ACCESS "(x,i) ((x)[i])\n");
  } else if(jetStorageType==1) {
      fprintf(outfile, "#define " MY_JET_DATA_ACCESS "(x,i) ((*(x))[i])\n");
  } else if(jetStorageType==2) {
    print_jet_tree_monomial_index_mapper(my_jet_prefix, suffix, num_jet_symbols, deg_jet_vars);
    fprintf(outfile, "#define " MY_JET_DATA_ACCESS "(x,i) (((x)->coef)[_%s_tr_idx_map_%s_[i]])\n",my_jet_prefix,suffix);
  } else {
    fprintf(outfile, "#define " MY_JET_DATA_ACCESS "(x,i) ((x)[i])\n");
  }
  //

  print_and_subs(words, my_jet_preheaders[index_my_jet_header], my_jet_prefix, suffix);
  fprintf(outfile, "\n");
  print_and_subs(words, my_jet_headers[index_my_jet_header], my_jet_prefix, suffix);
  fprintf(outfile, "\n");
  print_and_subs(words, my_jet_postheaders[index_my_jet_header], my_jet_prefix, suffix);
  fprintf(outfile, "\n");
//   if (my_jet_arith != ARITH_JET_NONE)
  print_and_subs(words, MY_JET_FUN_MACRO, my_jet_prefix, suffix);
  fprintf(outfile, "\n");

  if (genMyBlas) {
      fprintf(outfile, "\n#ifndef _MY_JET_MYBLAS_H_\n");
      fprintf(outfile, "#define _MY_JET_MYBLAS_H_\n");
      print_and_subs(words, my_jet_myblas_headers[index_my_jet_header], my_jet_myblas_prefixes[index_my_jet_prefix], suffix);
      fprintf(outfile, "\n");
      print_and_subs(words, MY_JET_MYBLAS_FUN_MACRO, my_jet_myblas_prefixes[index_my_jet_prefix], suffix);
      fprintf(outfile, "\n");
      fprintf(outfile, "\n#endif /* end _MY_JET_MYBLAS_H_ */\n\n");
    }

  if (gencppwrapper) {
      fprintf(outfile, "\n#endif\n");

      if (my_float_arith == ARITH_NONE && my_coef_arith == ARITH_MY_FLOAT)
        {print_and_subs(words, my_jet_cppwrapper[4*index_my_jet_header+0], my_jet_prefix, suffix);}
      else if (my_float_arith == ARITH_NONE && my_coef_arith != ARITH_MY_FLOAT)
        {print_and_subs(words, my_jet_cppwrapper[4*index_my_jet_header+1], my_jet_prefix, suffix);}
      else if (my_float_arith != ARITH_NONE && my_coef_arith == ARITH_MY_FLOAT)
        {print_and_subs(words, my_jet_cppwrapper[4*index_my_jet_header+2], my_jet_prefix, suffix);}
      else if (my_float_arith != ARITH_NONE && my_coef_arith != ARITH_MY_FLOAT)
        {print_and_subs(words, my_jet_cppwrapper[4*index_my_jet_header+3], my_jet_prefix, suffix);}

      fprintf(outfile, "#ifdef __cplusplus\nextern \"C\"\n\n");
    }

  fprintf(outfile, "#endif /* _MY_JET_H_ */\n\n");
}

void genMyJetCode(void) {
  if (outfile == NULL) return;
  
  // 221104
  int i, k; 
  fprintf(outfile,"static int _%s_monomial_counts_%s_[]  =      {1", my_jet_prefix, suffix);
  for(i=1; i<= deg_jet_vars; i++) {
    k=num_monomials(num_jet_symbols, i);
    fprintf(outfile, ",%d", k);
  }
  fprintf(outfile, "};\n");
  fprintf(outfile,"static int _%s_monomial_offsets_%s_[] =      {0", my_jet_prefix, suffix);
  k=0;
  for(i=0;i<= deg_jet_vars; i++) {
    k+= num_monomials(num_jet_symbols, i);
    fprintf(outfile, ",%d", k);
  }  
  fprintf(outfile, "};\n");
  // 221104

  if (index_my_jet_code == CODE_JET_TREE)
    {
      print_jet_tree_num_coef_homog_table(my_jet_prefix, suffix, num_jet_symbols, deg_jet_vars);
    }
  
  print_and_subs(words, my_jet_precodes[index_my_jet_code], my_jet_prefix, suffix);
  fprintf(outfile, "\n");
  print_and_subs(words, my_jet_codes[index_my_jet_code], my_jet_prefix, suffix);
  fprintf(outfile, "\n");

  output_jet_multiplication_code();

  print_and_subs(words, my_jet_postcodes[index_my_jet_code], my_jet_prefix, suffix);
  fprintf(outfile, "\n");

  if (genMyBlas) {
      print_and_subs(words, my_jet_myblas_codes[index_my_jet_prefix], my_jet_myblas_prefixes[index_my_jet_prefix], suffix);

      fprintf(outfile, "%s\n", MY_BLAS_CODE(MY_FLOAT_MYBLAS_FIXES,\
                                            "MY_FLOAT",MY_FLOAT_FUN,\
                                            "MY_FLOAT",MY_FLOAT_FUN,\
                                            "MY_FLOAT",MY_FLOAT_FUN,"int","MY_JET_FUN(faux)"));
    }
}
