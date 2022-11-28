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
int num_symbols=0;
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


char *my_jet_prefixes[] = {\
 "",\
 "jet1_1", \
 "jet_1", \
 "jet1", \
 "jet2",
 "jet_2",
 "jet_m",
 "jet_tree",
 "jet"
};

char *my_float_api_macros = \
    MY_SCAL_MACROS(MY_FLOAT_FIXES);


#define InitCleanFlag "&"
#define InitCleanFlagStruct ""
char *my_jet_preheaders[]= { \
  MY_JET_NONE_PREHEADER(NONE), \
  \
  MY_JET1_1_PREHEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                      MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET_1_PREHEADER(MY_JET_1_FIXES, MY_JET_1_TYPE, \
                     MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET1_PREHEADER(MY_JET1_FIXES, MY_JET1_TYPE, \
                    MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET2_PREHEADER(MY_JET2_FIXES, MY_JET2_TYPE, \
                    MY_FLOAT_FIXES, "MY_FLOAT", "int"),\
  \
  MY_JET_2_PREHEADER(MY_JET_2_FIXES, MY_JET_2_TYPE, \
                    MY_FLOAT_FIXES, "MY_FLOAT", "int"),  \
  \
  MY_JET_m_PREHEADER(MY_JET_m_FIXES, MY_JET_m_TYPE, \
                    MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET_TREE_PREHEADER(MY_JET_TREE_FIXES, MY_JET_TREE_TYPE, \
                        MY_FLOAT_FIXES, "MY_FLOAT", "int") \
};
char *my_jet_headers[]= { \
  MY_JET_NONE_HEADER(NONE), \
  \
  MY_JET1_1_HEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                   MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET1_1_FIXES,InitCleanFlag), \
  \
  MY_JET_1_HEADER(MY_JET_1_FIXES, MY_JET_1_TYPE, \
                  MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET_1_FIXES,InitCleanFlag), \
  \
  MY_JET1_HEADER(MY_JET1_FIXES, MY_JET1_TYPE, \
                 MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET1_FIXES,InitCleanFlag), \
  \
  MY_JET1_HEADER(MY_JET2_FIXES, MY_JET2_TYPE, \
                 MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET2_FIXES,InitCleanFlag), \
  \
  MY_JET_2_HEADER(MY_JET_2_FIXES, MY_JET_2_TYPE, \
                  MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET_2_FIXES,InitCleanFlag), \
  \
  MY_JET_m_HEADER(MY_JET_m_FIXES, MY_JET_m_TYPE, \
                  MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET_m_FIXES,InitCleanFlag), \
  \
  MY_JET_TREE_HEADER(MY_JET_TREE_FIXES, MY_JET_TREE_TYPE, \
                     MY_FLOAT_FIXES, "MY_FLOAT", "int") \
  "\n" \
  MY_JET_MACROS(MY_JET_TREE_FIXES,InitCleanFlag) \
};
char *my_jet_postheaders[]= { \
  MY_JET_NONE_POSTHEADER(NONE), \
  \
  MY_JET1_1_POSTHEADER(MY_JET1_1_FIXES, MY_JET1_1_TYPE, \
                       MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET_1_POSTHEADER(MY_JET_1_FIXES, MY_JET_1_TYPE, \
                      MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET1_POSTHEADER(MY_JET1_FIXES, MY_JET1_TYPE, \
                     MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET2_POSTHEADER(MY_JET2_FIXES, MY_JET2_TYPE, \
                     MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET_2_POSTHEADER(MY_JET_2_FIXES, MY_JET_2_TYPE, \
                      MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET_m_POSTHEADER(MY_JET_m_FIXES, MY_JET_m_TYPE, \
                      MY_FLOAT_FIXES, "MY_FLOAT", "int"), \
  \
  MY_JET_TREE_POSTHEADER(MY_JET_TREE_FIXES, MY_JET_TREE_TYPE, \
                         MY_FLOAT_FIXES, "MY_FLOAT", "int") \
};

char *my_jet_precodes[] = { \
  MY_JET_NONE_PRECODE(NONE), \
  MY_JET1_1_PRECODE(MY_JET1_1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET_1_PRECODE(MY_JET_1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET1_PRECODE(MY_JET1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET2_PRECODE(MY_JET2_FIXES, MY_MP2_FIXES,MY_FLOAT_FIXES, "int"),\
  MY_JET_2_PRECODE(MY_JET_2_FIXES, MY_FLOAT_FIXES, "int"), \
  MY_JET_m_PRECODE(MY_JET_m_FIXES, MY_FLOAT_FIXES, "int"), \
  MY_JET_TREE_PRECODE(MY_JET_TREE_FIXES,MY_TREE_FIXES,MY_FLOAT_FIXES, "int"), \
};
char *my_jet_codes[] = { \
  MY_JET_NONE_CODE(NONE), \
  MY_JET1_1_CODE(MY_JET1_1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET_1_CODE(MY_JET_1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET1_CODE(MY_JET1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET2_CODE(MY_JET2_FIXES, MY_MP2_FIXES,MY_FLOAT_FIXES, "int"),\
  MY_JET_2_CODE(MY_JET_2_FIXES, MY_FLOAT_FIXES, "int"), \
  MY_JET_m_CODE(MY_JET_m_FIXES, MY_FLOAT_FIXES, "int"), \
  MY_JET_TREE_CODE(MY_JET_TREE_FIXES,MY_TREE_FIXES,MY_FLOAT_FIXES, "int"), \
};
char *my_jet_postcodes[] = { \
  MY_JET_NONE_POSTCODE(NONE), \
  MY_JET1_1_POSTCODE(MY_JET1_1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET_1_POSTCODE(MY_JET_1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET1_POSTCODE(MY_JET1_FIXES, MY_FLOAT_FIXES, "int"),\
  MY_JET2_POSTCODE(MY_JET2_FIXES, MY_MP2_FIXES,MY_FLOAT_FIXES, "int"),\
  MY_JET_2_POSTCODE(MY_JET_2_FIXES, MY_FLOAT_FIXES, "int"), \
  MY_JET_m_POSTCODE(MY_JET_m_FIXES, MY_FLOAT_FIXES, "int"), \
  MY_JET_TREE_POSTCODE(MY_JET_TREE_FIXES,MY_TREE_FIXES,MY_FLOAT_FIXES, "int"), \
};

static char *words[2] = {MY_JET_PREFIX,MY_JET_SUFFIX};

static void print_and_subs(char *word[], char *str)
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
      fprintf(outfile, "%s", my_jet_prefix);

      matchWord(str, word[1], length, &location);
      for(i = 0; i < location; i++) {fputc(str[i], outfile);}
      str += location + wlen[1];
      length -= (location + wlen[1]);
      fprintf(outfile, "%s", suffix);
    }
  fprintf(outfile,"%s\n", str);
}

void genMyJetHeader(void) {
  if (outfile == NULL) return;

  fprintf(outfile, "\n/*** MY_JET ***/\n#ifndef _MY_JET_H_\n");
  fprintf(outfile, "#define _MY_JET_H_\n");


  //20200615
  if(jetStorageType==0) {
    fprintf(outfile, "#define MY_JET_DATA(x,i) ((x)[i])\n");        
  } else if(jetStorageType==1) {
      fprintf(outfile, "#define MY_JET_DATA(x,i) ((*(x))[i])\n");            
  } else if(jetStorageType==2) {
    print_jet_tree_monomial_index_mapper();      
    fprintf(outfile, "#define MY_JET_DATA(x,i) (((x)->coef)[_tr_idx_map_[i]])\n");        
  } else {
    fprintf(outfile, "#define MY_JET_DATA(x,i) ((x)[i])\n");        
  }
  //
  
  print_and_subs(words, my_jet_preheaders[index_my_jet_header]);

  print_and_subs(words, my_jet_headers[index_my_jet_header]);

  print_and_subs(words, my_jet_postheaders[index_my_jet_header]);
  fprintf(outfile, "#endif /*_MY_JET_H_ */ \n");
}

void genMyJetCode(void) {
  if (outfile == NULL) return;

//  print_and_subs(words, my_float_api_macros);
  
  // 221104
  extern int num_monomials(int, int);
  int i, k; 
  fprintf(outfile,"static int _monomial_counts_[]  =      {1");
  for(i=1; i<= deg_jet_vars; i++) {
    k=num_monomials(num_symbols, i);
    fprintf(outfile, ",%d", k);
  }
  fprintf(outfile, "};\n");
  fprintf(outfile,"static int _monomial_offsets_[] =      {0");
  k=0;
  for(i=0;i<= deg_jet_vars; i++) {
    k+= num_monomials(num_symbols, i);    
    fprintf(outfile, ",%d", k);
  }  
  fprintf(outfile, "};\n");
  // 221104
  
  print_and_subs(words, my_jet_precodes[index_my_jet_code]);
  
  if (index_my_jet_code == CODE_JET_TREE)
    {
      print_jet_tree_num_coef_homog_table(num_symbols, deg_jet_vars);
    }

  print_and_subs(words, my_jet_codes[index_my_jet_code]);
  output_jet_multiplication_code();

  print_and_subs(words, my_jet_postcodes[index_my_jet_code]);
}
