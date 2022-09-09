static char *JetVarIOHelpers =
"#if _SIZE_OF_JET_VAR_ > 1\n\
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
}  \n\
\n\
/* \n\
 *  create jet var from an array of doubles\n\
 *  Parms:  a, JET var\n\
 *          values, an array of doubles assign to jet\n\
 *                  caller needs to make sure there are  \n\
 *                  enough number in values\n\
 */\n\
\n\
int taylor_make_jet(MY_JET a, MY_FLOAT *myfloats, double *values) {\n\
  int i;\n\
  MY_FLOAT *dtmp = (MY_FLOAT *)malloc((_size_of_jet_variable_) * sizeof(MY_FLOAT));\n\
  if(dtmp == NULL) {\n\
    fprintf(stderr, \"Unable to allocate %d bytes.\", (int) ( (_size_of_jet_variable_) * sizeof(MY_FLOAT)));\n\
    exit(9);\n\
  }  \n\
  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
    InitMyFloat(dtmp[i]);\n\
    if(myfloats != NULL) {\n\
      AssignMyFloat( dtmp[i],myfloats[i]);\n\
    } else if(values != NULL) {\n\
      MakeMyFloatA(dtmp[i],values[i]);\n\
    }\n\
  }\n\
  // The monomials solicited here are in lexical order. We need to\n\
  // make sure our assigment macro/function are using the matching\n\
  // order\n\
  AssignFloatArrayToJet(a,dtmp);\n\
  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
    ClearMyFloat(dtmp[i]);\n\
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
int taylor_make_identity_jets(MY_JET *inOut, MY_FLOAT *myfloats, double *values) {\n\
  int i,j, k;\n\
\n\
  k = _number_of_jet_vars_ > _number_of_symbols_ ? _number_of_symbols_  : _number_of_jet_vars_;\n\
  \n\
  for(i = 0; i < k ; i++) {\n\
    for(j = 0; j < _size_of_jet_variable_; j++) {\n\
      if(j == 0 ) {\n\
	if(myfloats != NULL) {\n\
	  AssignMyFloat( MY_JET_DATA((inOut[i]),0),myfloats[i]);    	\n\
	} else if( values != NULL) {\n\
	    MakeMyFloatA( MY_JET_DATA((inOut[i]),0),values[i]);    	\n\
	}\n\
      } else {\n\
	if(i == j-1) { // offset by 1.\n\
	  MakeMyFloatA( MY_JET_DATA((inOut[i]),j),1.0);    		  \n\
	} else {\n\
	  MakeMyFloatA( MY_JET_DATA((inOut[i]),j),0.0);    		  \n\
	}\n\
      }\n\
    }\n\
  }\n\
  for(i = k; i < _NUMBER_OF_STATE_VARS_; i++) {\n\
    for(j = 0; j < _size_of_jet_variable_; j++) {\n\
      if(j == 0 ) {\n\
	if(myfloats != NULL) {\n\
	  AssignMyFloat( MY_JET_DATA((inOut[i]),0),myfloats[i]);    	\n\
	} else if( values != NULL) {	\n\
	  MakeMyFloatA( MY_JET_DATA((inOut[i]),0),values[i]);    	\n\
	}\n\
      } else {\n\
	MakeMyFloatA( MY_JET_DATA((inOut[i]),j),0.0);    		  \n\
      }\n\
    }\n\
  }  \n\
  return 0;\n\
}\n\
\n\
/*\n\
 *   make an unit jet\n\
 *     jet = v + s_idx\n\
 *   idx is 0 based, s0, s1, s2 etc. \n\
 */\n\
int taylor_make_unit_jet(MY_JET a, int idx, MY_FLOAT *myfloat, double *value) {\n\
  for(int j = 0; j < _size_of_jet_variable_; j++) {\n\
    if(j == 0) {\n\
      if(myfloat != NULL) {\n\
	AssignMyFloat( MY_JET_DATA((a),0),*myfloat);    		\n\
      } else if(value != NULL) {\n\
	MakeMyFloatA( MY_JET_DATA((a),0),*value);    		\n\
      }\n\
    } else {\n\
      if(j == idx+1) {\n\
	MakeMyFloatA( MY_JET_DATA((a),j),1.0);    		  	\n\
      } else {\n\
	MakeMyFloatA( MY_JET_DATA((a),j),0.0);\n\
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
int taylor_set_jet(MY_JET a, MY_FLOAT *myfloats, double *values, int include_state) {\n\
  int j, offset = 1;\n\
  if(include_state) offset = 0;\n\
  else {MakeMyFloatA( MY_JET_DATA((a),0),0.0);}\n\
  \n\
  for(j = offset; j < _size_of_jet_variable_; j++) {\n\
    if(myfloats != NULL) {\n\
      AssignMyFloat( MY_JET_DATA((a),j),myfloats[j-offset]);\n\
    } else if(values != NULL) {\n\
      MakeMyFloatA( MY_JET_DATA((a),j),values[j-offset]);    		          \n\
    } \n\
  }\n\
  return 0;\n\
}\n\
\n\
/*\n\
 *  Output a jet to an array of MY_FLOATs, optionally include\n\
 *  the state variable.\n\
 */\n\
MY_FLOAT *taylor_convert_jet_to_array(MY_JET a, int include_state) {\n\
  int i,offset = 1;\n\
  if(include_state) offset = 0;\n\
  \n\
  static MY_FLOAT *dtmp = NULL;\n\
#pragma omp threadprivate(dtmp)\n\
\n\
  if(dtmp == NULL) {\n\
    dtmp = (MY_FLOAT *)malloc((_size_of_jet_variable_) * sizeof(MY_FLOAT));\n\
    if(dtmp == NULL) {\n\
      fprintf(stderr, \"Unable to allocate %d bytes.\", (int) ( (_size_of_jet_variable_) * sizeof(MY_FLOAT)));\n\
      exit(9);\n\
    }  \n\
    for(i = 0; i < _size_of_jet_variable_; i++) {\n\
      InitMyFloat(dtmp[i]);\n\
      MakeMyFloatA(dtmp[i], 0.0);\n\
    }\n\
  }\n\
  for(i = offset; i < _size_of_jet_variable_; i++) {\n\
    AssignMyFloat(dtmp[i-offset],  MY_JET_DATA((a),i));    \n\
  }\n\
  return dtmp;\n\
}\n\
\n\
\n\
/*\n\
 *  taylor_input_jet: input jet from stdin,  prompt with list of monomials in jet var \n\
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
int taylor_input_jet_from_stdin(MY_JET a, int idx) {\n\
  /* extern char *ode_variable_names[]; */\n\
  char buf[2048];\n\
  int i, count, nbytes;\n\
\n\
  MY_FLOAT *dtmp = (MY_FLOAT *)malloc((_size_of_jet_variable_) * sizeof(MY_FLOAT));\n\
  if(dtmp == NULL) {\n\
    fprintf(stderr, \"Unable to allocate %d bytes.\", (int) ( (_size_of_jet_variable_) * sizeof(MY_FLOAT)));\n\
    exit(9);\n\
  }  \n\
  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
    InitMyFloat(dtmp[i]);\n\
  }\n\
  \n\
  bzero(buf, 2048);\n\
  count = taylor_gen_jetvar_prompt(idx, buf);\n\
\n\
  fprintf(stderr, \"Enter values for JET Var %s: %s \\n\", ode_variable_names[idx], buf);\n\
  buf[0] =0;\n\
  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
    char *q = get_next_number_stdin(buf);\n\
    double f = atof(q);\n\
    MakeMyFloatC(dtmp[i], buf, f);    \n\
  }\n\
  \n\
  taylor_make_jet(a,dtmp, NULL);\n\
  \n\
  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
    ClearMyFloat(dtmp[i]);\n\
  }  \n\
  (void) free(dtmp);\n\
\n\
  return 0;\n\
}\n\
\n\
int taylor_input_jet_from_string(MY_JET a, const char *str) {\n\
  static char buf[256];\n\
  static MY_FLOAT *dtmp = NULL;  \n\
#pragma omp threadprivate(buf,dtmp)\n\
  int i, j=0;\n\
  if(dtmp == NULL) {\n\
    dtmp = (MY_FLOAT *)malloc((_size_of_jet_variable_) * sizeof(MY_FLOAT));\n\
    if(dtmp == NULL) {\n\
      fprintf(stderr, \"Unable to allocate %d bytes.\", (int) ( (_size_of_jet_variable_) * sizeof(MY_FLOAT)));\n\
      exit(9);\n\
    }  \n\
    for(i = 0; i < _size_of_jet_variable_; i++) {\n\
      InitMyFloat(dtmp[i]);\n\
      MakeMyFloatA(dtmp[i], 0.0);\n\
    }\n\
  }\n\
  for(i=0; i< _size_of_jet_variable_; i++) {\n\
    get_next_number(str, &j, buf);\n\
    double f=atof(buf);\n\
    MakeMyFloatC(dtmp[i], buf, f);\n\
  }\n\
\n\
  taylor_make_jet(a, dtmp, NULL);\n\
  return 0;\n\
}\n\
\n\
int taylor_output_jet(FILE *file, char *fmt, MY_JET a) {\n\
  int i;\n\
  static MY_FLOAT *dtmp = NULL;\n\
#pragma omp threadprivate(dtmp)\n\
\n\
  if(dtmp == NULL) {\n\
    dtmp = (MY_FLOAT *)malloc((_size_of_jet_variable_) * sizeof(MY_FLOAT));\n\
    if(dtmp == NULL) {\n\
      fprintf(stderr, \"Unable to allocate %d bytes.\", (int) ( (_size_of_jet_variable_) * sizeof(MY_FLOAT)));\n\
      exit(9);\n\
    }  \n\
    for(i = 0; i < _size_of_jet_variable_; i++) {\n\
      InitMyFloat(dtmp[i]);\n\
      MakeMyFloatA(dtmp[i], 0.0);\n\
    }\n\
  }\n\
  //  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
  //    MakeMyFloatA(dtmp[i], 0.0);    \n\
  //  }\n\
\n\
  AssignJetToFloatArray(dtmp,a);  \n\
  \n\
  for(i = 0; i < _size_of_jet_variable_; i++) {\n\
    OutputMyFloat3(file, fmt, dtmp[i]);\n\
    //#if defined _USE_MPFR_ || defined  _USE_GMP_\n\
    //    fprintf(file, \" \");\n\
    //#endif\n\
  }\n\
  return 0;  \n\
}\n\
\n\
void taylor_initialize_jet_library2(int nvars, int degree) {\n\
  InitUpJet2(nvars, degree);\n\
}\n\
void taylor_initialize_jet_library(void) {\n\
  InitUpJet();\n\
}\n\
\n\
void taylor_initialize_jet_variable(MY_JET *jet) {\n\
  InitJet(*jet);\n\
}\n\
\n\
void taylor_clear_jet_variable(MY_JET *jet) {\n\
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
  return SetNumSymbols(nsyms);\n\
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
\n\
";
