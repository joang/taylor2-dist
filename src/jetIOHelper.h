/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
 *    Copyright (C) 2022 Joan Gimeno, Angel Jorba, Maorong Zou
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
#ifndef JET_IO_HELPER_H
#define JET_IO_HELPER_H

#define JET_IO_HELPER_API(MYJET,MYCOEF,MYFLOAT,I) "\
\n\
/* Set Jet values  */\n\
\
int taylor_make_jet(" MYJET " a, " MYCOEF " *mycoefs, " MYFLOAT " *myfloats, double *values);\n\
int taylor_make_identity_jets(" MYJET " *inOut, " MYCOEF " *mycoefs, " MYFLOAT " *myfloats, double *values);\n\
int taylor_make_unit_jet(" MYJET " a, int idx, " MYCOEF " *mycoef, " MYFLOAT " *myfloat, double *value);\n\
int taylor_set_jet(" MYJET " a, " MYCOEF " *mycoefs, " MYFLOAT " *myfloats, double *values, int include_state);\n\
\n\
/* Convert Jet to array */\n\
\
" MYCOEF " *taylor_convert_jet_to_array(" MYJET " a, int include_state);\n\
\n\
/* Input/Output Jet */\n\
\
int taylor_input_jet_from_stdin(" MYJET " a, int idx) ;\n\
int taylor_input_jet_from_string(" MYJET " a, const char *str);\n\
int taylor_output_jet(FILE *file, const char *fmt, " MYJET " a);\n\
\n\
/* Init/Cleanup Library/Jet */\n\
\
void taylor_initialize_jet_library();\n\
void taylor_initialize_jet_library2(int nsymbs, int degree);\n\
void taylor_initialize_jet_variable(" MYJET " *jet);\n\
void taylor_clear_jet_variable(" MYJET " *jet);\n\
void taylor_clear_up_jet_library();\n\
int taylor_set_jet_variable_number_of_symbols(int);\n\
int taylor_set_jet_variable_degree(int);\n\
\n\
void taylor_jet_reduce(" MYJET ", double *);\n\
" MYFLOAT " *taylor_get_jet_data_array(" MYJET ");\n\
const char **taylor_get_variable_names();\n\
const char **taylor_get_jet_monomials();\n\
" \


#define JET_IO_HELPER_HEADER(MYJET,MYCOEF,MYFLOAT,I) "\
/* JET IO Helpers */\n\
" JET_IO_HELPER_API(MYJET,MYCOEF,MYFLOAT,I) "\n\
/* END... JET IO Helpers */\n\
" \

#define JET_IO_HELPER_CODE(MYJET,MYJET_FUN,MYCOEF,MYCOEF_FUN,MYFLOAT,MYFLOAT_FUN,I, \
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,MY_SIZE_JET_VAR,MY_JET_DATA) "\
#if " MY_SIZE_JET_VAR " > 1\n\
\n\
#include <stdio.h>\n\
#include <ctype.h>\n\
#include <string.h>\n\
\n\
/*\n\
 *  Jet IO Helpers.  These helpers assumes the jet libraries\n\
 *  order the monomials using the lexicographical order. I.e, a\n\
 *  jet is repesent as\n\
 *    a0 + a1 s_1 + a2 s_2 + a3 s_3 + a4 s1^2 + a5 s1 s2 + a6 s1 s3 + a7 s2^2 + a8 s2 s3 + a9 s3^2 ...\n\
 */\n\
\n\
static char *get_next_number(const char *s, int *j, char *buf) {\n\
  const char *p=s + *j;\n\
  int i=0, k=0;\n\
  while(p && *p && (!isdigit(*p)) && *p != '.' && *p != '-') {    k++; p++;  }\n\
  while(p && *p && (isdigit(*p) || *p == '.' || *p == '-')) {  \n\
    buf[i++] = *p++; k++;\n\
  }\n\
  buf[i++] = '\\0';\n\
  *j = *j+k;\n\
  return buf;\n\
}\n\
static char *get_next_number_stdin(char *buf) {\n\
  int i=0; char p = getc(stdin);\n\
  while(p && (!isdigit(p)) && p != '.' && p != '-') { p = getc(stdin);  }\n\
  while(p && (isdigit(p) || p == '.' || p == '-')) { buf[i++] = p; p = getc(stdin);}\n\
  buf[i++] = '\\0'; return buf;\n\
}\n\
\n\
\n\
static int taylor_gen_jetvar_prompt(int i, char *buf) {\n\
  /* extern char *ode_variable_names[], *jet_variable_monomials[];  */\n\
  \n\
  const char *me = ode_variable_names[i];\n\
  char tmp[128]; \n\
  int j;\n\
\n\
  sprintf(buf, \"%s\", me);\n\
  j = 0;\n\
  while(jet_variable_monomials[j] != NULL) {\n\
    sprintf(tmp, \",%s\", jet_variable_monomials[j]);\n\
    (void)strcat(buf, tmp);\n\
    j++;\n\
  }\n\
  return j+1;\n\
}\n\
\n\
/* \n\
 *  create jet var from an array of doubles\n\
 *  Parms:  a, JET var\n\
 *          values, an array of doubles assign to jet\n\
 *                  caller needs to make sure there are  \n\
 *                  enough number in values\n\
 */\n\
\n\
int taylor_make_jet(" MYJET " a, " MYCOEF " *mycoefs, " MYFLOAT " *myfloats, double *values) {\n\
  int i;\n\
  " MYCOEF " *dtmp = (" MYCOEF " *) malloc((" MY_SIZE_JET_VAR ") * sizeof(" MYCOEF "));\n\
  if (dtmp == NULL) {\n\
    fprintf(stderr, \"Unable to allocate \%d bytes.\", (int) ( (" MY_SIZE_JET_VAR ") * sizeof(" MYCOEF ")));\n\
    fflush(stderr); exit(9);\n\
  }\n\
  if (mycoefs != NULL) {\n\
    for (i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
      " MYCOEF_FUN(init) "(dtmp[i]);\n\
      " MYCOEF_FUN(set) "(dtmp[i],mycoefs[i]);\n\
    }\n\
  } else if (myfloats != NULL) {\n\
    for (i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
      " MYCOEF_FUN(init) "(dtmp[i]);\n\
      " MYCOEF_FUN(set_myfloat) "(dtmp[i],myfloats[i]);\n\
    }\n\
  } else if(values != NULL) {\n\
    for (i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
      " MYCOEF_FUN(init) "(dtmp[i]);\n\
      " MYCOEF_FUN(set_d) "(dtmp[i],values[i]);\n\
    }\n\
  }\n\
  // The monomials solicited here are in lexical order. We need to\n\
  // make sure our assigment macro/function are using the matching\n\
  // order\n\
  " MYJET_FUN(set_coef_array) "(a,dtmp);\n\
  for(i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
    " MYCOEF_FUN(clean) "(dtmp[i]);\n\
  }\n\
  (void)free(dtmp);\n\
  return 0;\n\
}\n\
\n\
/*\n\
 *  Make an identity jet. If values is give, set the value of the\n\
 *  state variables. Otherwise, leave the state variable alone.\n\
 */\n\
\n\
int taylor_make_identity_jets(" MYJET " *inOut, " MYCOEF " *mycoefs, " MYFLOAT " *myfloats, double *values) {\n\
  int i,j, k;\n\
\n\
  k = _NUMBER_OF_JET_VARS_ > " MAX_NUM_SYMBOLS_NAME " ? " MAX_NUM_SYMBOLS_NAME " : _NUMBER_OF_JET_VARS_;\n\
  \n\
  for(i = 0; i < k; i++) {\n\
    for (j = 0; j < " MY_SIZE_JET_VAR "; j++) {\n\
      if (j == 0 ) {\n\
        if (mycoefs != NULL) {\n\
          " MYCOEF_FUN(set) "(" MY_JET_DATA "((inOut[i]),0),mycoefs[i]);\n\
        } else if(myfloats != NULL) {\n\
          " MYCOEF_FUN(set_myfloat) "(" MY_JET_DATA "((inOut[i]),0),myfloats[i]);\n\
	} else if( values != NULL) {\n\
          " MYCOEF_FUN(set_d) "(" MY_JET_DATA "((inOut[i]),0),values[i]);\n\
	}\n\
      } else {\n\
	if(i == j-1) { // offset by 1.\n\
          " MYCOEF_FUN(set_si) "( " MY_JET_DATA "((inOut[i]),j),1);\n\
	} else {\n\
          " MYCOEF_FUN(set_si) "( " MY_JET_DATA "((inOut[i]),j),0);\n\
	}\n\
      }\n\
    }\n\
  }\n\
  for (i = k; i < _NUMBER_OF_STATE_VARS_; i++) {\n\
    for (j = 0; j < " MY_SIZE_JET_VAR "; j++) {\n\
      if (j == 0) {\n\
        if (mycoefs != NULL) {\n\
           " MYCOEF_FUN(set) "(" MY_JET_DATA "((inOut[i]),0),mycoefs[i]);\n\
        } else if (myfloats != NULL) {\n\
          " MYCOEF_FUN(set_myfloat) "(" MY_JET_DATA "((inOut[i]),0),myfloats[i]);\n\
        } else if (values != NULL) {	\n\
          " MYCOEF_FUN(set_d) "(" MY_JET_DATA "((inOut[i]),0),values[i]);\n\
        } else {\n\
          " MYCOEF_FUN(set_si) "(" MY_JET_DATA "((inOut[i]),0),0);\n\
        }\n\
      } else {\n\
        " MYCOEF_FUN(set_si) "( " MY_JET_DATA "((inOut[i]),j),0);\n\
      }\n\
    }\n\
  }\n\
  return 0;\n\
}\n\
\n\
/*\n\
 *   make an unit jet\n\
 *     jet = v + s_idx\n\
 *   idx is 0 based, s0, s1, s2 etc. \n\
 */\n\
int taylor_make_unit_jet(" MYJET " a, int idx, " MYCOEF " *mycoef, " MYFLOAT " *myfloat, double *value) {\n\
  for (int j = 0; j < " MY_SIZE_JET_VAR "; j++) {\n\
    if (j == 0) {\n\
      if (mycoef != NULL) {\n\
        " MYCOEF_FUN(set) "(" MY_JET_DATA "((a),0),*mycoef);\n\
      } if (myfloat != NULL) {\n\
        " MYCOEF_FUN(set_myfloat) "(" MY_JET_DATA "((a),0),*myfloat);\n\
      } else if(value != NULL) {\n\
        " MYCOEF_FUN(set_d) "(" MY_JET_DATA "((a),0),*value);\n\
      }\n\
    } else {\n\
      if (j == idx+1) {\n\
        " MYCOEF_FUN(set_si) "(" MY_JET_DATA "((a),j),1);\n\
      } else {\n\
        " MYCOEF_FUN(set_si) "(" MY_JET_DATA "((a),j),0);\n\
      }\n\
    }\n\
  }\n\
  return 0;\n\
}\n\
\n\
\n\
/*\n\
 *  Set the value of jet using an array of MY_FLOATs.\n\
 *  optionally set the state variable. \n\
 */\n\
int taylor_set_jet(" MYJET " a, " MYCOEF " *mycoefs, " MYFLOAT " *myfloats, double *values, int include_state) {\n\
  int j, offset = 1;\n\
  if (include_state) offset = 0;\n\
  else {" MYCOEF_FUN(set_si) "( " MY_JET_DATA "((a),0),0);}\n\
  \n\
  if (mycoefs != NULL) {\n\
    for(j = offset; j < " MY_SIZE_JET_VAR "; j++) {\n\
      " MYCOEF_FUN(set) "( " MY_JET_DATA "((a),j),mycoefs[j-offset]);\n\
    }\n\
  } else if (myfloats != NULL) {\n\
    for(j = offset; j < " MY_SIZE_JET_VAR "; j++) {\n\
      " MYCOEF_FUN(set_myfloat) "( " MY_JET_DATA "((a),j),myfloats[j-offset]);\n\
    }\n\
  } else if (values != NULL) {\n\
    for(j = offset; j < " MY_SIZE_JET_VAR "; j++) {\n\
      " MYCOEF_FUN(set_d) "( " MY_JET_DATA "((a),j),values[j-offset]);\n\
    }\n\
  }\n\
  return 0;\n\
}\n\
\n\
/*\n\
 *  Output a jet to an array of MY_FLOATs, optionally include\n\
 *  the state variable.\n\
 */\n\
" MYCOEF " *taylor_convert_jet_to_array(" MYJET " a, int include_state) {\n\
  int i,offset = 1;\n\
  if (include_state) offset = 0;\n\
  \n\
  static " MYCOEF " *dtmp = NULL;\n\
#pragma omp threadprivate(dtmp)\n\
  \n\
  if(dtmp == NULL) {\n\
    dtmp = (" MYCOEF " *) malloc((" MY_SIZE_JET_VAR ") * sizeof(" MYCOEF "));\n\
    if(dtmp == NULL) {\n\
      fprintf(stderr, \"Unable to allocate \%d bytes.\", (int) ( (" MY_SIZE_JET_VAR ") * sizeof(" MYCOEF ")));\n\
      fflush(stderr); exit(9);\n\
    }\n\
    for(i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
      " MYCOEF_FUN(init) "(dtmp[i]);\n\
      " MYCOEF_FUN(set_si) "(dtmp[i], 0);\n\
    }\n\
  }\n\
  for(i = offset; i < " MY_SIZE_JET_VAR "; i++) {\n\
    " MYCOEF_FUN(set) "(dtmp[i-offset],  " MY_JET_DATA "((a),i));\n\
  }\n\
  return dtmp;\n\
}\n\
\n\
\n\
/*\n\
 *  taylor_input_jet: input jet from stdin,  prompt with list of monomials in jet var\n\
 *\n\
 *  Parms:  a, JET var\n\
 *          idx, index of a in Jet Array. The order\n\
 *             of the Jet Array is affixed with the\n\
 *             order of LHS of ODEs \n\
 *          values, an array of doubles assign to jet\n\
 *                  caller needs to make sure there are  \n\
 *                  enough number in values\n\
 */\n\
\n\
int taylor_input_jet_from_stdin(" MYJET " a, int idx) {\n\
  /* extern char *ode_variable_names[]; */\n\
  char buf[2048];\n\
  int i, count, nbytes;\n\
\n\
  " MYFLOAT " *dtmp = (" MYFLOAT " *)malloc((" MY_SIZE_JET_VAR ") * sizeof(" MYFLOAT "));\n\
  if(dtmp == NULL) {\n\
    fprintf(stderr, \"Unable to allocate \%d bytes.\", (int) ( (" MY_SIZE_JET_VAR ") * sizeof(" MYFLOAT ")));\n\
    exit(9);\n\
  }\n\
  for(i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
    InitMyFloat(dtmp[i]);\n\
  }\n\
  \n\
  bzero(buf, 2048);\n\
  count = taylor_gen_jetvar_prompt(idx, buf);\n\
\n\
  fprintf(stderr, \"Enter values for JET Var \%s: \%s \\n\", ode_variable_names[idx], buf);\n\
  buf[0] =0;\n\
  for(i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
    char *q = get_next_number_stdin(buf);\n\
    double f = atof(q);\n\
    MakeMyFloatC(dtmp[i], buf, f);    \n\
  }\n\
  \n\
  taylor_make_jet(a, NULL, dtmp, NULL);\n\
  \n\
  for(i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
    ClearMyFloat(dtmp[i]);\n\
  }\n\
  (void) free(dtmp);\n\
\n\
  return 0;\n\
}\n\
\n\
int taylor_input_jet_from_string(" MYJET " a, const char *str) {\n\
  static char buf[256];\n\
  static " MYFLOAT " *dtmp = NULL;  \n\
#pragma omp threadprivate(buf,dtmp)\n\
  int i, j=0;\n\
  if(dtmp == NULL) {\n\
    dtmp = (" MYFLOAT " *)malloc((" MY_SIZE_JET_VAR ") * sizeof(" MYFLOAT "));\n\
    if(dtmp == NULL) {\n\
      fprintf(stderr, \"Unable to allocate \%d bytes.\", (int) ( (" MY_SIZE_JET_VAR ") * sizeof(" MYFLOAT ")));\n\
      exit(9);\n\
    }\n\
    for(i = 0; i < " MY_SIZE_JET_VAR "; i++) {\n\
      InitMyFloat(dtmp[i]);\n\
      MakeMyFloatA(dtmp[i], 0.0);\n\
    }\n\
  }\n\
  for(i=0; i< " MY_SIZE_JET_VAR "; i++) {\n\
    get_next_number(str, &j, buf);\n\
    double f=atof(buf);\n\
    MakeMyFloatC(dtmp[i], buf, f);\n\
  }\n\
\n\
  taylor_make_jet(a, NULL, dtmp, NULL);\n\
  return 0;\n\
}\n\
\n\
int taylor_output_jet(FILE *file, const char *fmt, " MYJET " a) {\n\
  OutputJet2File(file,fmt,a);\n\
}\n\
\n\
void taylor_initialize_jet_library2(int nsymbs, int degree) {\n\
  InitUpJet2(nsymbs, degree);\n\
}\n\
void taylor_initialize_jet_library(void) {\n\
  InitUpJet();\n\
}\n\
\n\
void taylor_initialize_jet_variable(" MYJET " *jet) {\n\
  InitJet(*jet);\n\
}\n\
\n\
void taylor_clear_jet_variable(" MYJET " *jet) {\n\
  ClearJet(*jet);\n\
}\n\
void taylor_clear_up_jet_library(void) {\n\
  ClearUpJet();\n\
}\n\
\n\
int taylor_set_jet_variable_degree(int deg) {\n\
  return SetJetVarDegree(deg);\n\
}\n\
int taylor_set_jet_variable_number_of_symbols(int nsyms) {\n\
  return SetJetNumSymbols(nsyms);\n\
}\n\
\n\
const char **taylor_get_variable_names() {\n\
  return ode_variable_names;\n\
}\n\
\n\
const char **taylor_get_jet_monomials() {\n\
  return jet_variable_monomials;\n\
}\n\
#endif\n\
\n\
\n\
" \

 static char *JetIOHelperHeader = \
       JET_IO_HELPER_HEADER("MY_JET","MY_COEF","MY_FLOAT","int");

static char *JetIOHelperCode = \
      JET_IO_HELPER_CODE("MY_JET",MY_JET_FUN,"MY_COEF",MY_COEF_FIXES,"MY_FLOAT",MY_FLOAT_FIXES, "int", \
                         MY_JET_MAX_NUM_SYMB_MACRO_NAME,MY_JET_MAX_DEGREE_MACRO_NAME,MY_JET_TOTAL_COEFFS_MACRO_NAME,\
                         MAX_SIZE_JET_VAR_MACRO_NAME, MY_JET_DATA_ACCESS);

#endif /* JET_IO_HELPER_H */
