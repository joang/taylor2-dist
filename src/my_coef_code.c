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
#define MY_COEF_CODE_C
#include "Header.h"

/****************************************************************/
int num_coef_vars=0;
int my_coef_num_symbols=-1;
int my_coef_deg=-1;
int total_my_coef_vars=0;

#include "my_coef_header.h"

/* *******  ************  ******* */
/* *******  MY_COEF_NONE  ******* */
/* *******  ************  ******* */
//#include "my_coef_none.h"

/* *******  ****************  ******* */
/* *******  MY_COEF_MY_FLOAT  ******* */
/* *******  ****************  ******* */
#include "my_coef_MY_FLOAT.h"

/* *******  **************  ******* */
/* *******  MY_COEF_double  ******* */
/* *******  **************  ******* */
#include "my_coef_double.h"

/* *******  ***************  ******* */
/* *******  MY_COEF_COMPLEX  ******* */
/* *******  ***************  ******* */
#include "my_coef_complex.h"

/* *******  **********  ******* */
/* *******  MY_COEF1_1  ******* */
/* *******  **********  ******* */
//#include "my_coef1_1.h"

/* *******  *********  ******* */
/* *******  MY_COEF_1  ******* */
/* *******  *********  ******* */
//#include "my_coef_1.h"

/* *******  ********  ******* */
/* *******  MY_COEF1  ******* */
/* *******  ********  ******* */
//#include "my_coef1.h"

/* *******  ********  ******* */
/* *******  MY_COEF2  ******* */
/* *******  ********  ******* */
//#include "my_coef2.h"


/* *******  *********  ******* */
/* *******  MY_COEF_2  ******* */
/* *******  *********  ******* */
//#include "MY_COEF_2.h"

/* *******  *********  ******* */
/* *******  MY_COEF_m  ******* */
/* *******  *********  ******* */
//#include "my_coef_m.h"

/* *******  ************  ******* */
/* *******  MY_COEF_TREE  ******* */
/* *******  ************  ******* */
#include "my_coef_jet_tree.h"


char *my_coef_prefixes[] = {\
 "mycoef_myfloat", \
 "mycoef_double", \
 "mycoef_complex", \
 "mycoef_jet_tree", \
};

char *my_coef_api_macros[] = {\
    MY_COEF_MACROS(MY_COEF_FIXES,\
                 MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME), \
  \
    MY_COEF_MACROS_MY_FLOAT(MY_COEF_FIXES,\
                            MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME), \
  \
    MY_COEF_MY_FLOAT_BASIC(MY_COEF_MY_FLOAT_FIXES,\
                           MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
};

char *my_coef_preheaders[]= { \
  MY_COEF_MY_FLOAT_PREHEADER(MY_COEF_MY_FLOAT_FIXES, \
                             MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_DOUBLE_PREHEADER(MY_COEF_DOUBLE_FIXES, \
                           MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_COMPLEX_PREHEADER(MY_COEF_COMPLEX_FIXES), \
};
char *my_coef_of_jets_preheaders[]= { \
  MY_COEF_JET_TREE_PREHEADER(MY_COEF_JET_TREE_FIXES, MY_COEF_JET_TREE_TYPE, \
                             MY_COEF_MY_FLOAT_FIXES, "MY_FLOAT", "MY_FLOAT", "int", \
                             MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
};
char *my_coef_headers[]= { \
  MY_COEF_MY_FLOAT_HEADER(MY_COEF_MY_FLOAT_FIXES, \
                          MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_DOUBLE_HEADER(MY_COEF_DOUBLE_FIXES, \
                        MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_COMPLEX_HEADER(MY_COEF_COMPLEX_FIXES), \
  \
};
char *my_coef_of_jets_headers[]= { \
  MY_COEF_JET_TREE_HEADER(MY_COEF_JET_TREE_FIXES, MY_COEF_JET_TREE_TYPE, \
                          MY_COEF_MY_FLOAT_FIXES, "MY_FLOAT", "MY_FLOAT", "int", \
                          MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
};
char *my_coef_postheaders[]= { \
  MY_COEF_MY_FLOAT_POSTHEADER(MY_COEF_MY_FLOAT_FIXES, \
                              MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_DOUBLE_POSTHEADER(MY_COEF_DOUBLE_FIXES, \
                            MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_COMPLEX_POSTHEADER(MY_COEF_COMPLEX_FIXES), \
  \
};
char *my_coef_of_jets_postheaders[]= { \
  MY_COEF_JET_TREE_POSTHEADER(MY_COEF_JET_TREE_FIXES, MY_COEF_JET_TREE_TYPE, \
                              MY_COEF_MY_FLOAT_FIXES, "MY_FLOAT", "MY_FLOAT", "int", \
                              MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
};

char *my_coef_precodes[] = { \
  MY_COEF_MY_FLOAT_PRECODE(MY_COEF_MY_FLOAT_FIXES, \
                           MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_DOUBLE_PRECODE(MY_COEF_DOUBLE_FIXES, \
                         MY_COEF_MAX_NUM_SYMB_MACRO_NAME,MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_COMPLEX_PRECODE(MY_COEF_COMPLEX_FIXES), \
  \
};
char *my_coef_of_jets_precodes[] = { \
  MY_COEF_JET_TREE_PRECODE(MY_COEF_JET_TREE_FIXES, MY_COEF_JET_TREE_FIXES, \
                           MY_COEF_JET_TREE_MY_COEF_FIXES, "int", \
                           MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
};
char *my_coef_codes[] = { \
  MY_COEF_MY_FLOAT_CODE(MY_COEF_MY_FLOAT_FIXES, \
                        MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_DOUBLE_CODE(MY_COEF_DOUBLE_FIXES, \
                      MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_COMPLEX_CODE(MY_COEF_COMPLEX_FIXES), \
  \
};
char *my_coef_of_jets_codes[] = { \
  MY_COEF_JET_TREE_CODE(MY_COEF_JET_TREE_FIXES,MY_COEF_TREE_FIXES,\
                        MY_COEF_JET_TREE_MY_COEF_FIXES,MY_COEF_JET_TREE_MY_COEF_FIXES, "int", \
                        MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME,MY_COEF_DATA_ACCESS,MY_COEF_RECURSION), \
};
char *my_coef_postcodes[] = { \
  MY_COEF_MY_FLOAT_POSTCODE(MY_COEF_MY_FLOAT_FIXES, \
                            MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_DOUBLE_POSTCODE(MY_COEF_DOUBLE_FIXES, \
                          MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
  \
  MY_COEF_COMPLEX_POSTCODE(MY_COEF_COMPLEX_FIXES), \
  \
};
char *my_coef_of_jets_postcodes[] = { \
  MY_COEF_JET_TREE_POSTCODE(MY_COEF_JET_TREE_FIXES,MY_COEF_TREE_FIXES,\
                            MY_COEF_JET_TREE_MY_COEF_FIXES, "int", \
                            MY_COEF_MAX_NUM_SYMB_MACRO_NAME, MY_COEF_MAX_DEGREE_MACRO_NAME,MY_COEF_TOTAL_COEFFS_MACRO_NAME), \
};

static char *words[2] = {MY_COEF_PREFIX,MY_COEF_SUFFIX};
#define MY_COEF_FUN_MACRO "#define " MY_COEF_FUN(x) " " MY_COEF_PREFIX "_ ## x ## _" MY_COEF_SUFFIX

int setMyCoefIndexes(void) {
  if (my_coef_arith == ARITH_NONE)
    {
      //index_my_coef_prefix = PREFIX_NONE;
      index_my_coef_prefix =  PREFIX_MY_FLOAT;
      //index_my_coef_header = HEADER_NONE;
      index_my_coef_header = HEADER_MY_FLOAT;      
      //index_my_coef_code = CODE_NONE;
      index_my_coef_code = CODE_MY_FLOAT;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* No coef lib */\n");
    }
  else if (my_coef_arith == ARITH_MY_FLOAT)
    {
      index_my_coef_code = CODE_MY_FLOAT;
      index_my_coef_header = HEADER_MY_FLOAT;
      index_my_coef_prefix = PREFIX_MY_FLOAT;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* Using coef lib: MY_FLOAT */\n");
    }
  else if (my_coef_arith == ARITH_JET1_1)
    {
      index_my_coef_prefix = PREFIX_MYCOEF1_1;
      index_my_coef_header = HEADER_COEF1_1;
      index_my_coef_code = CODE_COEF1_1;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* Using coef lib: jet1_1. one symbol, degree one */\n");
    }
  else if (my_coef_arith == ARITH_JET1)
    {
      index_my_coef_prefix = PREFIX_MYCOEF1;
      index_my_coef_header = HEADER_COEF1;
      index_my_coef_code = CODE_COEF1;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* Using coef lib: jet1. one symbol, degree n */\n");
    }
  else if (my_coef_arith == ARITH_JET_1)
    {
      index_my_coef_prefix = PREFIX_MYCOEF_1;
      index_my_coef_header = HEADER_COEF_1;
      index_my_coef_code = CODE_COEF_1;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* Using coef lib: jet_1. n symbol, degree 1 */\n");
    }
  else if (my_coef_arith == ARITH_JET2)
    {
      index_my_coef_prefix = PREFIX_MYCOEF2;
      index_my_coef_header = HEADER_COEF2;
      index_my_coef_code = CODE_COEF2;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      coefStorageType=1;
      fprintf(outfile, "/* Using coef lib: jet2. 2 symbol, degree n */\n");
    }
  else if (my_coef_arith == ARITH_JET_2)
    {
      index_my_coef_prefix = PREFIX_MYCOEF_2;
      index_my_coef_header = HEADER_COEF_2;
      index_my_coef_code = CODE_COEF_2;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* Using coef lib: jet_2. n symbol, degree 2 */\n");
    }
  else if (my_coef_arith == ARITH_JET_m)
    {
      index_my_coef_prefix = PREFIX_MYCOEF_m;
      index_my_coef_header = HEADER_COEF_m;
      index_my_coef_code = CODE_COEF_m;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      fprintf(outfile, "/* Using coef lib: jet_m. m symbol, degree n */\n");
    }
  else if (my_coef_arith == ARITH_JET_TREE)
    {
      index_my_coef_prefix = PREFIX_MYCOEF_TREE;
      index_my_coef_header = HEADER_COEF_TREE;
      index_my_coef_code = CODE_COEF_TREE;
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
      coefStorageType=2;
      fprintf(outfile, "/* Using coef lib: jet_tree. m symbol, degree n */\n");
    }
  else
    {
      /* coef lib invalid */
      return 1;
    }
  return 0;
}

void genMyCoefHeader(void) {
  extern char *my_coef_prefix, *suffix;
  if (outfile == NULL) return;

  fprintf(outfile, "\n/*** MY_COEF ***/\n#ifndef _MY_COEF_H_\n#define _MY_COEF_H_\n\n");

  if (my_coef_flags & NON_EMPTY)
    {
      total_my_coef_vars=1;
      int k;
      for (k = 1; k <= my_coef_deg; ++k) {total_my_coef_vars+= num_monomials(my_coef_num_symbols, k);}

      fprintf(outfile, "#define " MY_COEF_MAX_NUM_SYMB_MACRO_NAME " %d\n", my_coef_num_symbols);
      fprintf(outfile, "#define " MY_COEF_MAX_DEGREE_MACRO_NAME " %d\n", my_coef_deg);
      fprintf(outfile, "#define " MY_COEF_TOTAL_COEFFS_MACRO_NAME " %d\n", total_my_coef_vars);

      if (my_coef_flags & AS_MYJET)
        {
          fprintf(outfile, "#define " MY_COEF_DATA_ACCESS " " MY_JET_DATA_ACCESS "\n");
        }
      else if (coefStorageType==2)
        {
          print_jet_tree_monomial_index_mapper(my_coef_prefix, suffix, my_coef_num_symbols, my_coef_deg);
          fprintf(outfile, "#define " MY_COEF_DATA_ACCESS "(x,i) (((x)->coef)[_%s_tr_idx_map_%s_[i]])\n", my_coef_prefix,suffix);
        }
      else
        {
          fprintf(outfile, "#define " MY_COEF_DATA_ACCESS " " MY_JET_DATA_ACCESS "\n");
        }

      print_and_subs(words, my_coef_of_jets_preheaders[index_my_coef_header], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
//      if (my_jet_arith == ARITH_JET1_1) print_and_subs(words, my_coef_api_macros[2], my_coef_prefix, suffix);
      print_and_subs(words, my_coef_of_jets_headers[index_my_coef_header], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
      print_and_subs(words, my_coef_of_jets_postheaders[index_my_coef_header], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
    }
  else
    {
      fprintf(outfile, "#define " MY_COEF_MAX_NUM_SYMB_MACRO_NAME " 0\n");
      fprintf(outfile, "#define " MY_COEF_MAX_DEGREE_MACRO_NAME " 0\n");
      fprintf(outfile, "#define " MY_COEF_TOTAL_COEFFS_MACRO_NAME " 1\n");

      print_and_subs(words, my_coef_preheaders[index_my_coef_header], my_coef_prefix, suffix);
      fprintf(outfile, "\n");

      if (my_jet_arith == ARITH_JET1_1) print_and_subs(words, my_coef_api_macros[2], my_coef_prefix, suffix);
      print_and_subs(words, my_coef_headers[index_my_coef_header], my_coef_prefix, suffix);
      fprintf(outfile, "\n");

      print_and_subs(words, my_coef_postheaders[index_my_coef_header], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
    }

  print_and_subs(words, MY_COEF_FUN_MACRO, my_coef_prefix, suffix);
  fprintf(outfile, "\n");

  fprintf(outfile, "#endif /* _MY_COEF_H_ */\n\n");
}

void genMyCoefCode(void) {
  extern char *my_jet_prefix, *my_coef_prefix, *suffix;
  int i, k;
  if (outfile == NULL) return;

  if (my_coef_flags & NON_EMPTY)
    {
      if (my_coef_flags & AS_MYJET)
        {
          fprintf(outfile,"#define _%s_monomial_counts_%s_ _%s_monomial_counts_%s_\n", my_coef_prefix, suffix, my_jet_prefix, suffix);
          fprintf(outfile,"#define _%s_monomial_offsets_%s_ _%s_monomial_offsets_%s_\n", my_coef_prefix, suffix, my_jet_prefix, suffix);
        }
      else
        {
          fprintf(outfile,"static int _%s_monomial_counts_%s_[]  =      {1", my_coef_prefix, suffix);
          for(i=1; i<= my_coef_deg; i++) {
              k=num_monomials(my_coef_num_symbols, i);
              fprintf(outfile, ",%d", k);
            }
          fprintf(outfile, "};\n");
          fprintf(outfile,"static int _%s_monomial_offsets_%s_[] =      {0", my_coef_prefix, suffix);
          k=0;
          for(i=0;i<= my_coef_deg; i++) {
              k+= num_monomials(my_coef_num_symbols, i);
              fprintf(outfile, ",%d", k);
            }
          fprintf(outfile, "};\n");
        }

      if (index_my_coef_code == CODE_COEF_TREE)
        {
          if ((my_coef_flags & NON_EMPTY) && (my_coef_flags & AS_MYJET))
            {
              fprintf(outfile,"#define %s_num_coefs_homogeneous_%s %s_num_coefs_homogeneous_%s\n", my_coef_prefix, suffix, my_jet_prefix, suffix);
            }
          else {print_jet_tree_num_coef_homog_table(my_coef_prefix, suffix, my_coef_num_symbols, my_coef_deg);}
        }

      print_and_subs(words, my_coef_of_jets_precodes[index_my_coef_code], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
//      if (my_jet_arith != ARITH_JET1_1) print_and_subs(words, my_coef_api_macros[2], my_coef_prefix, suffix);
      print_and_subs(words, my_coef_of_jets_codes[index_my_coef_code], my_coef_prefix, suffix);
      fprintf(outfile, "\n");

      print_and_subs(words, my_coef_of_jets_postcodes[index_my_coef_code], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
    }
  else
    {
      print_and_subs(words, my_coef_precodes[index_my_coef_code], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
      if (my_jet_arith != ARITH_JET1_1) print_and_subs(words, my_coef_api_macros[2], my_coef_prefix, suffix);
      print_and_subs(words, my_coef_codes[index_my_coef_code], my_coef_prefix, suffix);
      fprintf(outfile, "\n");

      print_and_subs(words, my_coef_postcodes[index_my_coef_code], my_coef_prefix, suffix);
      fprintf(outfile, "\n");
    }
}
