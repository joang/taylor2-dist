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

#define MAIN_C
#include <ctype.h>
#include "Header.h"
#include "SampleHeader.h"
#include "gmpHeader.h"
#include "qdheader.h"
#include "mpfrheader.h"
#include "longdoubleheader.h"
#include "float128Header.h"
#include "complexheader.h"
#include "longcomplexheader.h"
#include "complex128header.h"
#include "mpcheader.h"
#include "arfheader.h"
#include "jetIOHelper.h"
#include "VERSION"


int    debug = 0;
int    expandpower = 1;
int    expandsum = 10;
int    havesum = 0;
int    genHeader = 0;
int    genMyCloud = 0;
int    genMain = 0;
int    genMyJet = 0;
int    genMyCoef = 0;
int    genTestJet = 0;
int    genJet = 0;
int    genMyBlas = 0;
int    gencppwrapper = 0;
int    genStep = 0;
int    genExpressions = 0;
int    genPoincare = 0;
int    genRungeKutta = 0;
int    input_only = 0;
int    ignoreJetInconsistency=0;
int    genJetHelper = 0;
int    jetStorageType = 0;
int    coefStorageType = 0;
my_arith_t my_float_arith=ARITH_NONE;
int    mpc_rounding = 0;
int    mpc_precision[2]={0,0};
int    gmp_precision=0;
int    mfr_rounding = 0;
int    mpfr_precision=0;
int    ddouble=0,cdouble = 0,qd2 = 0,qd4 = 0; /* DEPRECATED */
int    f77hook = 0;
int    step_ctrl = 1;
int    use_rational_exponent=0;
char   *outNames[10];
char   *suffixes[10];
int    num_names = 0;
char   *outName = NULL;
char   *suffix = NULL;
char   *my_jet_prefix=NULL;
char   *my_jet_myblas_prefix=NULL;
my_arith_t my_jet_arith=ARITH_JET_NONE;
index_my_jet_prefix_t index_my_jet_prefix=PREFIX_NONE;
index_my_jet_header_t index_my_jet_header=HEADER_NONE;
index_my_jet_code_t index_my_jet_code=CODE_NONE;
char   *my_coef_prefix=NULL;
my_arith_t my_coef_arith=ARITH_MY_FLOAT;
index_my_coef_prefix_t index_my_coef_prefix=PREFIX_MY_FLOAT;
index_my_coef_header_t index_my_coef_header=HEADER_MY_FLOAT;
index_my_coef_code_t index_my_coef_code=CODE_MY_FLOAT;
my_coef_flags_t my_coef_flags=EMPTY;
char   *header_name = NULL;
char   *uss_name = NULL;
char   *uso_name = NULL;
FILE   *outfile = NULL;
FILE   *infile = NULL;
char   *saved_input_file=NULL;

// static int n_jets=0, d_jets=0;

extern FILE *yyin;

void help(const char *);
void genSampleHeader();
void genJetHelpers(int flag); 
int num_monomials(int nvars,int deg);
int coefficients_count(int nvars, int deg);
int list_monomials(int d, FILE *file);
int list_monomials2(int d, FILE *file);
int output_taylor_jet_reducer(FILE *outfile); 
void mn_multiply_jets( int m, int n, int flag );
int *mn_rearrange_indicies(int m, int n) ;
int *tree_monomial_order_map(int dim, int kmax);

extern int yyparse();

extern FILE *preprocessed_file(FILE *, char *);

int main(int ac, char *av[])
{
  int i, header=0, nariths=0, header_only = 0, jhelper=0, jhelper_only=0;
  int num_jet_symbols_tmp_copy, deg_jet_vars_tmp_copy;
  char *arith=NULL;
  char *overwriteJetLibrary=NULL;

  for(i = 1; i < ac; i++)
    {
      char *arg = av[i];
      if (arg[0] == '-')
	{
	  switch(arg[1])
	    {
            case 'a':
              if (!strcmp(arg,"-arf"))
              { if (nariths==0) {my_float_arith=ARITH_ARF; nariths++; arith="arf";} }
              break;
            case 'c':
              if (!strcmp(arg,"-constantsafe") || !strcmp(arg,"-const"))
                { cdouble = 1;}

              else if (!strcmp(arg,"-complex") || !strcmp(arg,"-cmplx"))
              { if (nariths==0) {my_float_arith=ARITH_COMPLEX_DOUBLE; nariths++; arith="cmplx";} }
              else if (!strcmp(arg,"-complex128") || !strcmp(arg,"-cmplx128"))
              { if (nariths==0) {my_float_arith=ARITH_COMPLEX128; nariths++; arith="cmplx128";} }

              else if (!strcmp(arg, "-cloud")) { genMyCloud=1; }
              else if (!strcmp(arg, "-cloud_prefix") || !strcmp(arg, "-cloudprefix"))
                {
                  if (ac > i+1) {
                      ++i;
                      int ii=0, len = strlen(av[i]);
                      char *s = av[i];
                      prefixMyCloud = (char *)malloc( (len+len+128)*sizeof(char));
                      for(ii=0; ii<len; ii++) {
                        char c = s[ii];
                        if (isalnum(c)) { prefixMyCloud[ii] =c;} else {prefixMyCloud[ii]='_';}
                      }
                      prefixMyCloud[ii]='_';
                      prefixMyCloud[++ii]='\0';

                      genMyCloud=1;
                    }
                }
              else if (!strcmp(arg, "-cloud_suffix") || !strcmp(arg, "-cloudsuffix"))
                {
                  if (ac > i+1) {
                      ++i;
                      int ii=0, len = strlen(av[i]);
                      char *s = av[i];
                      suffixMyCloud = (char *)malloc( (len+len+128)*sizeof(char));
                      suffixMyCloud[ii]='_';
                      for(ii=1; ii<len; ii++) {
                        char c = s[ii-1];
                        if (isalnum(c)) { suffixMyCloud[ii] =c;} else {suffixMyCloud[ii]='_';}
                      }
                      suffixMyCloud[ii]='\0';

                      genMyCloud=1;
                    }
                }
              else if (!strcmp(arg,"-coeflibrary") || !strcmp(arg,"-coef_library") || !strcmp(arg,"-coeflib")
                  || !strcmp(arg,"-coef_lib") || !strcmp(arg,"-clib"))
                {
                  if (ac > i+1)
                    {
                      if (!strcmp(av[i+1],"myfloat") || !strcmp(av[i+1],"my_float") || !strcmp(av[i+1],"MY_FLOAT"))
                        {
                          my_coef_arith = ARITH_MY_FLOAT;
                          i++;
                        }
                      else
                        {
                          if (!strcmp(av[i+1],"tree")||!strcmp(av[i+1],"jet_tree"))
                            {
                              my_coef_arith = ARITH_JET_TREE;
                              my_coef_flags = NON_EMPTY;
                              i++;
                            }
                          else
                            {
                              fprintf(stderr, "error: coef library unknown\n"); exit(10);
                            }

                          if (my_coef_flags & NON_EMPTY)
                            {
                              if (ac > i+1 && isdigit(av[i+1][0]))
                                {
                                  my_coef_num_symbols = atoi(av[i+1]); i++;
                                  my_coef_flags|=SYMBOLS;
                                  if (my_coef_num_symbols <= 0)
                                    {
                                      fprintf(stderr, "error: " MY_COEF_MAX_NUM_SYMB_MACRO_NAME " %d invalid value.\n",
                                              my_coef_num_symbols); fflush(stderr);
                                      exit(40);
                                    }
                                  if (ac > i+1 && isdigit(av[i+1][0]))
                                    {
                                      my_coef_deg = atoi(av[i+1]); i++;
                                      my_coef_flags|=DEGREE;
                                      if (my_coef_deg < 0)
                                        {
                                          fprintf(stderr, "error: " MY_COEF_MAX_DEGREE_MACRO_NAME " %d invalid value\n",
                                                  my_coef_num_symbols); fflush(stderr);
                                          exit(40);
                                        }
                                    }
                                  else
                                    {
                                      fprintf(stderr, "warning: " MY_COEF_MAX_DEGREE_MACRO_NAME " not provided. Default " MY_JET_MAX_DEGREE_MACRO_NAME "\n");
                                    }
                                }
                              else
                                {
                                  fprintf(stderr, "warning: " MY_COEF_MAX_NUM_SYMB_MACRO_NAME " not provided. Default " MY_JET_MAX_NUM_SYMB_MACRO_NAME "\n");
                                }
                            }
                        }
                    }
                  else
                    {
                      fprintf(stderr, "error: coef library type required\n"); exit(10);
                    }
                }
              else if (!strcmp(arg,"-cpp"))
                {
                  gencppwrapper=1;
                }
              break;
            case 'd':
              if (!strcmp(arg,"-debug") || !strcmp(arg,"-d"))
                debug = 1;
              break;
            case 'e':
              if (!strncmp(arg,"-expandsum",10))
                {
                  if (++i < ac) expandsum = atoi(av[i]);
                  else --i;
                }
              else if (!strncmp(arg,"-expandpower",15))
                {
                  if (++i < ac) expandpower = atoi(av[i]);
                  else --i;
                }
              if (!strncmp(arg,"-expression",14)) { genExpressions = 1;}
              break;
            case 'f':
              if (!strcmp(arg,"-f77")) { f77hook=1;}
              if (!strcmp(arg,"-float128") || !strcmp(arg,"-f128")) {
                if (nariths==0) {my_float_arith=ARITH_FLOAT128; nariths++; arith="float128";}
              }
              break;
            case 'g':
              if (!strcmp(arg,"-gmp_precision"))
                {
                  if (ac > i+1 && atoi(av[i+1]) > 0)
                    {
                      if (nariths==0)
                        {
                          my_float_arith=ARITH_GMP; nariths++; arith="gmp";
                          gmp_precision = atoi(av[i+1]);
                          i++;
                        }
                    } else {
                      fprintf(stderr,"\t The -gmp_precision flag must be followed by PRECISION in number of bits\n");
                      exit(1);
                    }
                }
              else if (!strcmp(arg,"-gmp")) {
                if (nariths==0) {my_float_arith=ARITH_GMP; nariths++; arith="gmp";}
              }
              break;
            case 'h':
              if (!strcmp(arg,"-header_name") || !strcmp(arg,"-headername")  ) { if (++i < ac) { header_name = av[i];} }
              else if (!strcmp(arg,"-header")) { header = 1;}
              else { help(av[0]);}
              break;
            case 'i':
              if (!strcmp(arg,"-ignore_jet_inconsistency") || !strcmp(arg,"-ignoreJI") || !strcmp(arg,"-ignoreji"))
                ignoreJetInconsistency=1;
	      if(!strcmp(arg,"-input_only") || !strcmp(arg,"-inputonly"))
            input_only = 1;
              break;
            case 'j':
              if (!strcmp(arg,"-jet")) { genJet = 1;genMyJet=1; }
              else if (!strcmp(arg,"-jhelper") || !strcmp(arg,"-jethelper") || !strcmp(arg,"-jet_helper")) { jhelper = 1;}
              else if (!strcmp(arg,"-jetlibrary") || !strcmp(arg,"-jet_library") || !strcmp(arg,"-jetlib")
                       || !strcmp(arg,"-jet_lib") || !strcmp(arg,"-jlib")) {
                if ( ac > i+1) {
                  overwriteJetLibrary=av[i+1];
                  i++;
                }
              }
              break;
            case 'l':
              if (!strcmp(arg,"-long_double") || !strcmp(arg,"-longdouble")) {
                if (nariths==0) {my_float_arith=ARITH_LONG_DOUBLE; nariths++; arith="longdouble";}
              }
              else if (!strcmp(arg,"-long_complex") || !strcmp(arg,"-longcomplex")) {
                if (nariths==0) {my_float_arith=ARITH_COMPLEX_LONG_DOUBLE; nariths++; arith="longcomplex";}
              }
              break;
            case 'm':
              if (!strcmp(arg,"-main_only") || !strcmp(arg,"-mainonly")) { genMain = 1; }
              else if (!strcmp(arg,"-main")) { genMain = 2; }
              else if (!strcmp(arg,"-mpfr"))
              { if (nariths==0) {my_float_arith=ARITH_MPFR; nariths++; arith="mpfr";} }
              else if (!strcmp(arg,"-mpfr_precision")) {
                if ( ac > i+1 && atoi(av[i+1]) > 0)
                  {
                    if (nariths==0)
                      {
                        my_float_arith=ARITH_MPFR; nariths++; arith="mpfr";
                        mpfr_precision = atoi(av[i+1]);
                        i++;
                      }
                  } else {
                    fprintf(stderr,"\t The -mpfr_precision flag must be followed by PRECISION in number of bits\n");
                    exit(1);
                  }
              }
              else if (!strcmp(arg,"-mpc"))
              { if (nariths==0) {my_float_arith=ARITH_MPC; nariths++; arith="mpc";} }
              else if (!strcmp(arg,"-mpc_precision")) {
                if ( ac > i+1 && atoi(av[i+1]) > 0)
                  {
                    if (nariths==0)
                      {
                        my_float_arith=ARITH_MPC; nariths++; arith="mpc";
                        mpc_precision[0] = atoi(av[i+1]); i++;
                        if ( ac > i+1 && atoi(av[i+1]) > 0) {
                          mpc_precision[1] = atoi(av[i+1]);
                          i++;
                        } else {
                          mpc_precision[1] = mpc_precision[0];
                        }
                      }
                  } else {
                    fprintf(stderr,"\t The -mpc_precision flag must be followed by [PRECISION_REAL | PRECISION_IMAG] in number of bits\n");
                    exit(1);
                  }
              }
            else if (!strcmp(arg,"-myblas") || !strcmp(arg,"-Myblas") || !strcmp(arg,"-MyBlas")) {
                genMyBlas = 1;
              }
              break;
	    case 'n':                   /* -name */
              if (!strcmp(arg,"-n") || !strcmp(arg,"-name"))
		{
                  if (++i < ac) {
                    suffixes[num_names++] = av[i];
                    if (num_names >= 10) {
                      fprintf(stderr, "Too many names ...\n"); 
                      exit(1);
                    }
                  }
		  else --i;
		}
	      /*
              else if (!strcmp(arg,"-njet")) {
                if (++i < ac) {
		  n_jets = atoi(av[i]);
		}
	      }
	      */
              break;
            case 'o':
              if (!strcmp(arg,"-o") || !strcmp(arg,"-out"))
                {
                  if (++i < ac)
                    {
                      char *str = av[i];
                      if ((outfile = fopen(str, "w")) == NULL)
                        {
                          fprintf(stderr, "Cannot open '%s'\n", str);
                          exit(1);
                        }
                    }
                  else --i;
                }
              break;
            case 'p':
              if (!strncmp(arg,"-poincare",10)) { genPoincare = 1; genExpressions = 1;}
              break;
            case 'r':
              if (!strncmp(arg,"-rungekutta",10) || !strncmp(arg,"-rk",10)) { genRungeKutta = 1;}
	      if (ac > i+1 && isdigit(av[i+1][0]))
		{
		  genRungeKutta = atoi(av[i+1]); i++;
		}
              break;	      
            case 's':
              if (!strcmp(arg,"-step")) {
                genStep = 1;
                step_ctrl = 1;
                if (++i < ac && strlen(av[i]) == 1 && isdigit(av[i][0])) {
                  step_ctrl = atoi(av[i]);
                  if ( step_ctrl < 0 || step_ctrl >= 4) {
                    fprintf(stderr, "Invalid step control parameter: %d\n", step_ctrl);
                    step_ctrl = 1;
                  }
                } else --i;                
              } else if ( !strcmp(arg, "-sqrt")) {
                use_rational_exponent=1;         
              }
              break;
            case 't':
//              if (!strcmp(arg,"-testjet")) { genMyJet = 1;}
              if (!strcmp(arg,"-testjet")) { genTestJet = 1;}
              break;
            case 'u':
              if (!strcmp(arg,"-u") || !strcmp(arg,"-ud") || !strncmp(arg,"-userdefined", 9))
                {
                  if (i+2 < ac && av[i+1][0] != '-' && av[i+2][0] != '-' ) {
                    i++; uss_name = av[i];
                    i++;  uso_name = av[i];
                  } else {  help(av[0]);  }
                }
              break;
	    case 'v':
              if (!strcmp(arg,"-v") ||  !strcmp(arg,"-verbose") ) debug = 1;
              else  if (!strcmp(arg,"-version"))
                {
                  fprintf(stderr,"%s\n", versionString);
                  exit(0);
                }
              break;
            default:
              break;
            }
	}
      else {
          if ((infile = fopen(av[i], "r")) == NULL)
            {
              fprintf(stderr, "Cannot open '%s'\n", av[i]);
              exit(2);
            }
          else
            {
              if(saved_input_file == NULL)
                {
                  int  len= strlen(av[i]);
                   saved_input_file = (char *)malloc( (len+len+128)*sizeof(char));
                   strcpy(saved_input_file, av[i]);
                }
             if (suffix == NULL)
               {
                 int ii, len = strlen(av[i]);
                 char *s = av[i];
                 suffix = (char *)malloc( (len+len+128)*sizeof(char));
                 for(ii=0; ii<len; ii++) {
                   char c = s[ii];
                   if ( isalnum(c)) { suffix[ii] =c; } else {suffix[ii]='_';}
                 }
                 suffix[ii]='\0';
               }
            }
        }
    }  
  if (suffixes[0] == NULL) {
    if (suffix != NULL) { suffixes[0] = suffix; } else { suffixes[0] = "_NoName";} num_names = 1;
  }
  if (nariths > 1) {
    fprintf(stderr, "\n Warning: You have specified more than one arithmetic options. Only \"%s\" will be used.\n\n", arith);
  }

  {
    int i, j, len;
    for(j = 0; j < num_names; j++) {
      len = strlen(suffixes[j]);
      for(i=0; i< len; i++) {
        if (!isalnum(suffixes[j][i]) && suffixes[j][i] != '_' ) {
          fprintf(stderr, "Name '%s' must be contain only alpha numeric characters.\n", suffixes[j]); exit(1);
        }
      }
      outNames[j] = (char *)malloc((len+32)*sizeof(char));
      sprintf(outNames[j], "taylor_coefficients_%s", suffixes[j]);
    }
  }
  suffix = suffixes[0];
  outName = outNames[0];

  if (suffixMyCloud == NULL)
    {
      suffixMyCloud = (char *) malloc((strlen(suffix)+1)*sizeof(char));
      suffixMyCloud[0] = '_'; strcpy(suffixMyCloud+1, suffix);
    }
  if (prefixMyCloud == NULL) prefixMyCloud = strdup("");
  if (infile == NULL) infile= stdin;
  if (outfile == NULL) outfile=stdout;

  if (header) {
    if ((genStep|genJet|genMain|genHeader) == 0) {
      // 20220420
      // relocated genSampleHeader to after parsing input
      // jet related vars/constants like _NUMBER_OF_JET_VARS_
      // depends on input.
      header_only = 1;
      //fprintf(outfile, "%s\n",sample_headerI);              
      //genSampleHeader();  
      //exit(0);
    } else {
      genHeader = 1;
    }
  }

  // 20220614
  if (jhelper) {
    if ((genStep|genJet|genMain|genHeader) == 0) {
      jhelper_only = 1;
    } else {
      genJetHelper = 1;
    }
  }
  // 20220614

  if ( (genStep|genJet|genMain|genHeader|genJetHelper) == 0) { genMain = 2;}

  if (genMain > 1) { genStep = 1; genJet = 1; genHeader = 1; genJetHelper=1; genMyJet=1;}

  if (header_name == NULL) { header_name = strdup("taylor.h");}

  infile = preprocessed_file(infile, saved_input_file);
  
  if(debug || input_only) {
    char *p, buffer[512];
    if(!input_only) fprintf(stderr, "/* input file contents start */\n");
    while(fgets(buffer, 512, infile)) {
      p = buffer;
      fprintf(input_only?stdout:stderr, "%s", p);
    }
    if(!input_only) fprintf(stderr, "/* input file contents end  */\n");    
    rewind(infile);
    if(input_only) exit(0);
  }
  
  initialize_vars();
  yyin = infile;
  yyparse();
  if(yyin != stdin) fclose(yyin);
  showPaserInfo();
  identifyFunctions();   
  checkVars();
  checkEquations();
  decompose();
  showVars();
  genVariables();

  if (neqns == 0) {fprintf(stderr, "\nNo ODEs present. Bye!\n\n"); exit(3);}
   
  checkJetVars();
 
  if(num_jet_vars==0) genMyJet = 0;

  if (genMyJet) genMyCoef=1;
  
  num_jet_symbols_tmp_copy = num_jet_symbols;
  deg_jet_vars_tmp_copy = deg_jet_vars;
  
  if (overwriteJetLibrary != NULL)
    {
      /*
       * if overwrite is requested, and the supplied library does not apply, 
       * we'll pick one that applies
       */
      
      if(!strcmp(overwriteJetLibrary, "1_1") || !strcmp(overwriteJetLibrary, "jet1_1")) {
          // Macros, can only be used in 1_1
          if (num_jet_symbols == 1 && deg_jet_vars == 1)
            {my_jet_arith = ARITH_JET1_1;}
          else
            {
              fprintf(stderr, "\nRequested 1_1 arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
      } else if(!strcmp(overwriteJetLibrary, "1_n") || !strcmp(overwriteJetLibrary, "jet1")) {
        // 1 symbol, arbitary degree
          if (num_jet_symbols == 1 && deg_jet_vars > 0)
            {my_jet_arith = ARITH_JET1;}
          else
            {
              fprintf(stderr, "\nRequested 1_n arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
      } else if(!strcmp(overwriteJetLibrary, "n_1") || !strcmp(overwriteJetLibrary, "jet_1")) {
        // n symbols,  degree 1
          if (num_jet_symbols > 0 && deg_jet_vars == 1)
            {my_jet_arith = ARITH_JET_1;}
          else
            {
              fprintf(stderr, "\nRequested n_1 arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
      } else if(!strcmp(overwriteJetLibrary, "2_n") || !strcmp(overwriteJetLibrary, "jet2")) {
	// 2 symbols, fall through by default
          if (num_jet_symbols == 2 && deg_jet_vars > 0)
            {my_jet_arith = ARITH_JET2;}
          else
            {
              fprintf(stderr, "\nRequested 2_n arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
      } else if(!strcmp(overwriteJetLibrary, "n_2") || !strcmp(overwriteJetLibrary, "jet_2")) {
	// deg=2, n symbols. only overrite 2_n with 2 symbols.
          if (num_jet_symbols > 0 && deg_jet_vars == 2)
            {my_jet_arith = ARITH_JET_2;}
          else
            {
              fprintf(stderr, "\nRequested n_2 arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
      } else if(!strcmp(overwriteJetLibrary, "m_n") || !strcmp(overwriteJetLibrary, "jet_m")) {
          // can overwrite all
          if (num_jet_symbols > 0 && deg_jet_vars > 0)
            {my_jet_arith = ARITH_JET_m;}
          else
            {
              fprintf(stderr, "\nRequested m_n arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
          num_jet_symbols_tmp_copy = -2;
          deg_jet_vars_tmp_copy= -2;
      } else if(!strcmp(overwriteJetLibrary, "tree") || !strcmp(overwriteJetLibrary, "jet_tree")) {
	// can overwrite all
          if (num_jet_symbols > 0 && deg_jet_vars > 0)
            {my_jet_arith = ARITH_JET_TREE;}
          else
            {
              fprintf(stderr, "\nRequested tree arithmetic with %d_%d inputs. Bye!\n\n", num_jet_symbols, deg_jet_vars);
              exit(4);
            }
      } else {
        fprintf(stderr, "jet lib invalid\n"); fflush(stderr); exit(1);
      }
    }
  else if (num_jet_symbols > 0 && deg_jet_vars > 0)
    {
      num_jet_symbols_tmp_copy = -1;
      deg_jet_vars_tmp_copy= -1;

      if (num_jet_symbols_tmp_copy ==0 || deg_jet_vars_tmp_copy==0)
        {my_jet_arith = ARITH_JET_NONE;}
      else if (num_jet_symbols_tmp_copy==1 && deg_jet_vars_tmp_copy==1)
        {my_jet_arith = ARITH_JET1_1;}
      else if (num_jet_symbols_tmp_copy > 0 && deg_jet_vars_tmp_copy==1)
        {my_jet_arith = ARITH_JET_1;}
      else if (num_jet_symbols_tmp_copy==1 && deg_jet_vars_tmp_copy > 0)
        {my_jet_arith = ARITH_JET1;}
      else if (num_jet_symbols_tmp_copy==2 && deg_jet_vars_tmp_copy > 0)
        {my_jet_arith = ARITH_JET2;}
      else if (num_jet_symbols_tmp_copy > 0 && deg_jet_vars_tmp_copy==2)
        {my_jet_arith = ARITH_JET_2;}
      else if (num_jet_symbols_tmp_copy==-2 && deg_jet_vars_tmp_copy==-2)
        {my_jet_arith = ARITH_JET_m;}
      else {my_jet_arith = ARITH_JET_TREE;}
    }
  else if (num_jet_symbols == 0 && deg_jet_vars == 0)
    {
      /* taylor version1 */
    }
  else
    {
      fprintf(stderr, "\nerror: %d_%d symbols and degree are not allowed. Bye!\n", num_jet_symbols, deg_jet_vars);
      fflush(stderr); exit(4);
    }

  if (my_coef_flags & NON_EMPTY)
    {
      /* default behaviours: if symbols or degree were not provided, we take them as the ones for my_jet */
      if (!(my_coef_flags & SYMBOLS)) {my_coef_num_symbols = num_jet_symbols;}
      if (!(my_coef_flags & DEGREE)) {my_coef_deg = deg_jet_vars;}

      /* if symbols and degree coincide with those of my_jet, some tables can be reused */
      if (my_coef_num_symbols == num_jet_symbols && my_coef_deg == deg_jet_vars && my_jet_arith == my_coef_arith)
        {my_coef_flags|= AS_MYJET;}

      if (my_jet_arith == ARITH_JET1_1) my_jet_arith = ARITH_JET1_1_BIS;
    }

  fprintf(outfile, "/* taylor %s */\n", versionString);
  setMyCoefIndexes();
  setMyJetIndexes();

  if (genMyCoef)
    {
      my_coef_prefix = my_coef_prefixes[index_my_coef_prefix];
    }

  if (header_only) {
    fprintf(outfile, "%s\n",sample_headerI);              
    genSampleHeader();
    exit(0);
  }

  if(jhelper_only) {
    genJetHelpers(1);
    exit(0);
  }

  genCode();

  //20230418
  if(genExpressions) genExpressionCode();

  if(genPoincare) genPoincareSectionCode();
  
  if(genRungeKutta) {
    genRungeKuttaMacros();    
    genGeneralRK45Code();
    genGeneralRK67Code();
    genGeneralRK78Code();
    genGeneralRK89Code();
    genRungeKuttaWrapper();
  }
  
  
  free(prefixMyCloud); free(suffixMyCloud);

  exit(0);
}

/********************************************************/
void help(const char *name)
{
  fprintf(stderr, "Taylor %s\n\n", versionString);
  fprintf(stderr, "Usage: %s \n", name);
  fprintf(stderr, "  [-name ODE_NAME ]\n");
  fprintf(stderr, "  [-o outfile ]\n");
  fprintf(stderr, "  [-long_double | -float128 |\n");
  fprintf(stderr, "   -arf |\n");
  fprintf(stderr, "   -mpfr | -mpfr_precision PRECISION |\n");
  fprintf(stderr, "   -gmp | -gmp_precision PRECISION |\n");
  fprintf(stderr, "   -complex |\n");
  fprintf(stderr, "   -long_complex | -complex128 |\n");
  fprintf(stderr, "   -mpc | -mpc_precision [PRECISION_REAL | PRECISION_IMAG] ]\n");
  fprintf(stderr, "  [-main | -header | -jet | -main_only ]\n");
  fprintf(stderr, "  [-jlib MY_JET_LIB | -jet_helper ]\n");
  fprintf(stderr, "  [-clib MY_COEF_LIB ]\n");
  fprintf(stderr, "  [-cloud | -cloud-prefix PREFIX | -cloud-suffix SUFFIX ]\n");
  fprintf(stderr, "  [-cpp ]\n");
  fprintf(stderr, "  [-rk ]\n");
  fprintf(stderr, "  [-myblas ]\n");
  fprintf(stderr, "  [-expression ]\n");
  fprintf(stderr, "  [-step STEP_CONTROL_METHOD ]\n");
  fprintf(stderr, "  [-u | -userdefined STEP_SIZE_FUNCTION_NAME ORDER_FUNCTION_NAME ]\n");
  fprintf(stderr, "  [-f77 ]\n");
  fprintf(stderr, "  [-sqrt ]\n");
  fprintf(stderr, "  [-headername HEADER_FILE_NAME ]\n");
  fprintf(stderr, "  [-debug] [-help] [-v]  file\n");
  exit(0);
}
/********************************************************/

void genSampleHeader(void)
{  
  int njets =num_jet_vars;
  int djets =deg_jet_vars;
  
  int i,jsize=1;

  fprintf(outfile, "#ifdef __cplusplus\nextern \"C\"\n{\n#endif\n");
  
  fprintf(outfile,"/***********************************************************************\n");
  fprintf(outfile," *\n");
  fprintf(outfile," * Code generated by the TAYLOR translator.\n");
  fprintf(outfile," */\n\n");

  fprintf(outfile,"#ifndef _NUMBER_OF_JET_VARS_\n");
  fprintf(outfile,"#define _NUMBER_OF_STATE_VARS_         %d\n", neqns - nonautonomous);  
  fprintf(outfile,"#define _NUMBER_OF_JET_VARS_           %d\n", njets);
  // 20220608+
  fprintf(outfile,"#define _NUMBER_OF_MAX_SYMBOLS_        %d\n", num_jet_symbols);
  
  fprintf(outfile,"#define _MAX_DEGREE_OF_JET_VARS_       %d\n", djets);
  
  // 20220608r
  //fprintf(outfile,"#define _JET_COEFFICIENTS_COUNT_TOTAL_ %d\n",coefficients_count(njets,djets));
  fprintf(outfile,"#define _JET_COEFFICIENTS_COUNT_TOTAL_ %d\n",coefficients_count(num_jet_symbols,djets));
  
//   fprintf(outfile,"static int _number_of_jet_vars_ =      %d;\n", njets);
  // 20220608+
//   fprintf(outfile,"static int _number_of_symbols_  =      %d;\n", num_jet_symbols);
  
//   fprintf(outfile,"static int _degree_of_jet_vars_ =      %d;\n", djets);
/*  fprintf(outfile,"static int _monomial_counts_[]  =      {1");
  for(i=1; i<= djets; i++) {
    int k=num_monomials(num_jet_symbols, i);
    jsize +=k;
    fprintf(outfile, ",%d", k);
  }
  fprintf(outfile, "};\n");
  fprintf(outfile,"static int _monomial_offsets_[] =      {0");
  n=0;
  for(i=0;i<= djets; i++) {
    // 20220608r
    //n += num_monomials(njets, i);
    n += num_monomials(num_jet_symbols, i);
    fprintf(outfile, ",%d", n);
  }
  fprintf(outfile, "};\n");*/
//   fprintf(outfile, "static int _size_of_jet_variable_ =    %d;\n", jsize);

  for(i=1; i<= djets; i++) jsize+=num_monomials(num_jet_symbols, i);
  fprintf(outfile,"#define _MAX_SIZE_OF_JET_VAR_          %d\n", jsize);
  fprintf(outfile,"#endif\n\n");

  fprintf(outfile,"#ifndef _NUMBER_OF_CLOUD_VARS_\n");
  fprintf(outfile,"#define _NUMBER_OF_CLOUD_VARS_         %d\n", ncloudVars); //MZ-TODO: nclouds from parse.y
  fprintf(outfile,"#define _MAX_CLOUD_SIZE_               %d\n", max_cloud_size); //MZ-TODO: scloud from parse.y
  fprintf(outfile,"#endif\n\n");


  fprintf(outfile, "#ifndef _TAYLOR_H_\n");
  fprintf(outfile, "#define _TAYLOR_H_\n");
  if (my_float_arith == ARITH_ARF) {
      fprintf(outfile,"#include <stdio.h>\n");
      fprintf(outfile,"#include <stdlib.h>\n");
      fprintf(outfile,"#include <math.h>\n");
      fprintf(outfile,"#include <string.h>\n");
      fprintf(outfile,"#include <ctype.h>\n");
      fprintf(outfile,"#include <arb.h> /* possible flags: -lflint -lflint-arb */\n");
      fprintf(outfile, "%s\n", arf_header);
      fprintf(outfile, "#endif\n");
  } else if (qd4 | qd2 ) { /* use library qd */
    fprintf(outfile, "#include <iostream>\n");
    fprintf(outfile, "#include <qd/qd_real.h>\n");
    fprintf(outfile, "#include <qd/fpu.h>\n");
    fprintf(outfile, "using namespace std;\n");    
    if (qd4) {  /* use quad double */
      fprintf(outfile,"typedef qd_real MY_FLOAT;\n");      
    } else if (qd2) { /* use double double */
      fprintf(outfile,"typedef dd_real MY_FLOAT;\n");      
    }
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");  
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");          
    fprintf(outfile, "%s\n",qd_header);

    fprintf(outfile, "#endif\n");    
  } else if (my_float_arith == ARITH_MPFR) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");
    fprintf(outfile,"#include <math.h>\n");   
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile,"#include <mpfr.h>\n");      
    fprintf(outfile, "%s\n", mpfr_header);
    
    fprintf(outfile, "#endif\n");        
  } else if (my_float_arith == ARITH_GMP) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");  
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile,"#include <gmp.h>\n");      
    fprintf(outfile,"#include <math.h>\n");      
    fprintf(outfile, "%s\n",gmp_header);

    fprintf(outfile, "#endif\n");    
  } else if (my_float_arith == ARITH_LONG_DOUBLE) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n"); 
    fprintf(outfile,"typedef long double MY_FLOAT;\n\n");
    fprintf(outfile,"#include <math.h>\n");   
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile, "%s\n",longdouble_header);
    
    fprintf(outfile, "#endif\n");    
  } else if (my_float_arith == ARITH_FLOAT128) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n"); 
    fprintf(outfile,"#include <quadmath.h>\n");
    fprintf(outfile,"typedef __float128 MY_FLOAT;\n\n");
    fprintf(outfile,"#include <math.h>\n");   
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");        
    fprintf(outfile, "%s\n",_float128_header);
    
    fprintf(outfile, "#endif\n");    
  } else if (my_float_arith == ARITH_COMPLEX_DOUBLE) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n"); 
    fprintf(outfile,"#include <complex.h>\n");      
    fprintf(outfile,"typedef double complex MY_FLOAT;\n\n");
    fprintf(outfile,"#include <math.h>\n");  
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile, "%s\n",complex_header);
    
    fprintf(outfile, "#endif\n");
  } else if (my_float_arith == ARITH_COMPLEX_LONG_DOUBLE) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");
    fprintf(outfile,"#include <complex.h>\n");      
    fprintf(outfile,"typedef long double complex MY_FLOAT;\n\n");
    fprintf(outfile,"#include <math.h>\n");    
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile, "%s\n",longcomplex_header);
    
    fprintf(outfile, "#endif\n");
  } else if (my_float_arith == ARITH_COMPLEX128) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");
    fprintf(outfile,"#include <complex.h>\n"); 
    fprintf(outfile,"#include <quadmath.h>\n");      
    fprintf(outfile,"typedef __complex128 MY_FLOAT;\n\n");
    fprintf(outfile,"#include <math.h>\n"); ;  
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile, "%s\n",complex128_header);
    
    fprintf(outfile, "#endif\n");
  } else if (my_float_arith == ARITH_MPC) {
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");
    fprintf(outfile,"#include <complex.h>\n");
    fprintf(outfile,"#include <mpc.h>\n");
    fprintf(outfile,"#include <math.h>\n");   
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");  
    
    if (mpc_precision[0] != 0)           
      fprintf(outfile,"#define MY_FLOAT_REAL_PREC %d\n", mpc_precision[0]);
    else 
      fprintf(outfile,"#define MY_FLOAT_REAL_PREC mpfr_get_default_prec()\n");
    
    if (mpc_precision[1] != 0)           
      fprintf(outfile,"#define MY_FLOAT_IMAG_PREC %d\n", mpc_precision[1]);
    else 
      fprintf(outfile,"#define MY_FLOAT_IMAG_PREC mpfr_get_default_prec()\n");
    
    fprintf(outfile, "%s\n", mpc_header);
    
    fprintf(outfile, "#endif\n");    
  } else {
    fprintf(outfile,"typedef double MY_FLOAT;\n\n"); 
    fprintf(outfile,"#include <stdio.h>\n");  
    fprintf(outfile,"#include <stdlib.h>\n");  
    fprintf(outfile,"#include <math.h>\n");     
    fprintf(outfile,"#include <string.h>\n");
    fprintf(outfile,"#include <ctype.h>\n");              
    fprintf(outfile, "%s\n",sample_header);
    
    fprintf(outfile, "#endif\n");
    
//    fprintf(outfile, "%s\n",sample_header_JET);
//    jetHeader = strlen(sample_header_JET);
  }

  if (genMyBlas) {
      fprintf(outfile, "\n#ifndef _MY_FLOAT_MYBLAS_H_\n");
      fprintf(outfile, "#define _MY_FLOAT_MYBLAS_H_\n");
      fprintf(outfile, "%s\n", MY_BLAS_HEADER(MY_FLOAT_MYBLAS_FIXES,"MY_FLOAT","MY_FLOAT","MY_FLOAT","int"));
      fprintf(outfile, "%s\n", MY_FLOAT_MYBLAS_FUN_MACRO);
      fprintf(outfile, "\n");
      fprintf(outfile, "\n#endif /* end _MY_FLOAT_MYBLAS_H_ */\n\n");
    }

  if (genMyCoef) {my_coef_prefix = my_coef_prefixes[index_my_coef_prefix]; genMyCoefHeader();}
  if (genMyCloud) genMyCloudHeader();
  else genMyJetHeader();

  // 20221219 20220614
  if( jsize > 1)
    {
      fprintf(outfile, "%s\n", JetIOHelperHeader);
    }
  // 20221219 202206014

  /*  function prototypes */
  {
    int i;
    for(i = 0; i < num_names; i++) {
      fprintf(outfile,"\nMY_FLOAT **%s(MY_FLOAT t, MY_FLOAT *x, int order);\n", outNames[i]);
      fprintf(outfile,"\nMY_FLOAT **%sA(MY_FLOAT t, MY_FLOAT *x, int order, int reuse_last_computation);\n", outNames[i]);
      fprintf(outfile,"\nMY_FLOAT **%s_A(MY_FLOAT t, MY_FLOAT *x, int order, int reuse_last_computation, MY_JET *jetIn, MY_JET ***jetOut);\n", outNames[i]);
      fprintf(outfile, "int       taylor_step_%s(MY_FLOAT *ti, MY_FLOAT *x, int dir, int step_ctl,\n", suffixes[i]);
      fprintf(outfile, "                         double log10abserr, double log10relerr,\n");
      fprintf(outfile, "                         MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetInOut);\n\n");
      fprintf(outfile, "int       taylor_uniform_step_%s_tag(MY_FLOAT *ti, MY_FLOAT *x, int dir, int step_ctl,\n", suffixes[i]);
      fprintf(outfile, "                         double log10abserr, double log10relerr,\n");
      fprintf(outfile, "                         MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetInOut,int tag);\n\n");
      fprintf(outfile, "int       taylor_uniform_step_%s(MY_FLOAT *ti, MY_FLOAT *x, int dir, int step_ctl,\n", suffixes[i]);
      fprintf(outfile, "                         double log10abserr, double log10relerr,\n");
      fprintf(outfile, "                         MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetInOut);\n\n");            
      fprintf(outfile, "int       taylor_step_%s_Twelve(MY_FLOAT *ti, MY_FLOAT *x, int dir, int step_ctl,\n", suffixes[i]);
      fprintf(outfile, "                         double log10abserr, double log10relerr,\n");
      fprintf(outfile, "                         MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetInOut, MY_FLOAT ***s_return, MY_JET ***jet_return);\n\n");      
      
    }
    if(genExpressions){
      Expression expr = expression_list;
      while(expr) {
	Node var = EXPRESSION_NAME(expr);
	expr =  EXPRESSION_NEXT(expr);
	//fprintf(outfile,"\nvoid %s(MY_FLOAT t, MY_FLOAT *in, MY_FLOAT *out);", NODE_GIVEN_NAME(var));
	//fprintf(outfile,"\nvoid %s_derivative(MY_FLOAT t, MY_FLOAT *in, MY_FLOAT *out);", NODE_GIVEN_NAME(var));		
	//fprintf(outfile,"\nMY_FLOAT *%s_value(MY_FLOAT t, MY_FLOAT *in, MY_FLOAT *out);", NODE_GIVEN_NAME(var));

	fprintf(outfile,"\nMY_FLOAT *%s(MY_FLOAT t, MY_FLOAT *in, MY_FLOAT *out, MY_JET *sIn, MY_JET ***sOut);", NODE_GIVEN_NAME(var));
	fprintf(outfile,"\nMY_FLOAT *%s_derivative(MY_FLOAT t, MY_FLOAT *in, MY_FLOAT *out, MY_JET *sIn, MY_JET ***sOut);", NODE_GIVEN_NAME(var));		
	fprintf(outfile,"\nMY_FLOAT **%s_taylor_coefficients(MY_FLOAT t, MY_FLOAT *in, int order, MY_JET *sIn, MY_JET ***sOut);\n", NODE_GIVEN_NAME(var));	
      }
    }
    if(genPoincare) {
      fprintf(outfile,"\nvoid poincare_section_%s(MY_FLOAT *xx_in, \n",suffix);
      fprintf(outfile,"\t\t int nsteps_in, int nintersections_in, int step_ctl_in,\n");
      fprintf(outfile,"\t\t double startT_in, double stopT_in,  double step, \n");
      fprintf(outfile,"\t\t double pEpsilon_in, double tolerance_in, double rtolerance_in, \n");
      fprintf(outfile,"\t\t MY_FLOAT *(*_poincare_section_f)(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *v,MY_JET *sIn, MY_JET ***sOut), \n");
      fprintf(outfile,"\t\t MY_FLOAT *(*_poincare_section_derivative_f)(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *v,MY_JET *sIn, MY_JET ***sOut),\n");
      fprintf(outfile,"\t\t int crossing, int intesection_method, char *out_file, MY_JET *sIn \n");
      fprintf(outfile,");\n");
      fprintf(outfile,"\nint RK4_step_%s(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl, double log10abserr, double log10relerr,\n",suffix);
      fprintf(outfile,"\t\t MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn);\n");
      fprintf(outfile,"\nint RK4QC_step_%s(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl, double log10abserr, double log10relerr,\n",suffix);
      fprintf(outfile,"\t\t MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn);\n");
    }

    if(genRungeKutta) {
      fprintf(outfile,"\nint RungeKutta_step_%s(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl,\n\
		               double log10abserr, double log10relerr,\n \
		               MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn, double *err_ret);\n\n",
	      suffix);
    }
  }
  fprintf(outfile, "/************************************************************************/\n\n");
  fprintf(outfile, "#ifdef __cplusplus\n}\n#endif\n");
}

/********************************************************/
void genJetHelpers(int add_header_h) {
  int i, jsize=1, djets =deg_jet_vars;
  
  for(i=1; i<= djets; i++) {
    int k=num_monomials(num_jet_symbols, i);
    jsize +=k;
  }

  if( jsize > 1)  {          
    fprintf(outfile, "#ifndef _JET_IO_HELPERS_\n#define _JET_IO_HELPERS_\n\n");

    if(add_header_h)
      fprintf(outfile, "#include \"%s\"\n\n", header_name);

    //fprintf(outfile, "#ifndef JET_IO_NAMES\n#define JET_IO_NAMES\n");
    
    int i;
    fprintf(outfile,"\nstatic const char *ode_variable_names[]={ ");
    for(i = 0; i <neqns; i++) {
      fprintf(outfile, "\"%s\", ", NODE_GIVEN_NAME(equations[i].name));
    }
    fprintf(outfile, "NULL };\n");
    int djets = deg_jet_vars;

    fprintf(outfile,"\nstatic const char *jet_variable_monomials[]={\n");    
    for(i = 1; i <= djets; i++) {list_monomials2(i, outfile);}
    fprintf(outfile,"\n\nNULL\n\n};\n\n");
    
    //fprintf(outfile, "#endif\n");

    fprintf(outfile, "%s\n", JetIOHelperCode);
    
    // 20230411
    //output_taylor_jet_reducer(outfile);
    // 20230411
    
    fprintf(outfile, "#endif /* _JET_IO_HELPERS_ */\n\n");
  }
}
  
 

/* number of degree deg monomials in nvars variables */

int num_monomials(int nvars0, int deg0) {
  long int nvars, deg;
  nvars = (long int) nvars0;
  deg = (long int) deg0;
  long int t=nvars+deg-1, d=0, n=1,  k = nvars - 1, c;
  c = (deg > k)? k : deg;
  if (c == 0) return(1);
  
  while(d<c) {
    n *= (t-d);
    n /= (1+d);
    d++;
  }
  return (int)n;
}

/* total number of coeffcients of upto degree deg*/
int coefficients_count(int nvars, int deg) {
  return num_monomials(nvars+1, deg);

}

/******************************************************************************/
/*
 * Helper for generating n_m JET multiplication function.
 *
 */
int mn_degree(int n,int *x) {
  int i, v = 0;
  for(i = 0;  i < n; i++)
    v += x[i];
  return v;
}


int n_choose_k ( int n0, int k0 ) {
  long int n, k,  d, r = 1, c;
  n = (long int)n0;
  k = (long int)k0;
  if(k > n)
    return 0;
  c = n - k;
  c=(c < k)? c: k;
  for(d =1; d <= c; ++d) {
    r *= n--;
    r /= d;
  }
  return (int) r;
}

/***************************************************************************************/
/*
 *  Credits: The grlex ranking code is modified from John Burkardt's monomial library.
 *        https://people.sc.fsu.edu/~jburkardt/c_src/monomial/monomial.html
 */

int exponent_to_rank( int m, int *x ) {
  int i, j, n, degree, rank, tim1, *xs;

  if( m < 1 ){
    fprintf( stderr, "exponents to rank: invalid dimension %d\n", m );
    exit(1);
  }

  for( i = 0; i < m; i++ ) {
    if( x[i] < 0 ) {
      fprintf ( stderr, "exponent to rank: invalid exponent x[%d]=%d\n", i, x[i]);
      exit(2);
    }
  }

  degree = mn_degree(m,x); // total degree

  xs = ( int * ) malloc ( m * sizeof ( int ) );

  xs[0] = x[0] + 1;
  for( i = 2; i < m; i++ ) {
    xs[i-1] = xs[i-2] + x[i-1] + 1;
  }

  rank = 1;
  for( i = 1; i <= m-1; i++ ) {
    if( i == 1 ) {
      tim1 = 0;
    } else  {
      tim1 = xs[i-2];
    }

    if( tim1 + 1 <= xs[i-1] - 1 ) {
      for ( j = tim1 + 1; j <= xs[i-1] - 1; j++ ){
        rank = rank + n_choose_k ( degree + m - 1 - j, m -1 - i );
      }
    }
  }

  for( n = 0; n < degree; n++ )  {
    rank = rank + n_choose_k ( n + m - 1, n );
  }

  free ( xs );

  return rank;
}

int *rank_to_exponent( int m, int rank ) {
  int i,  j,  nm, ns, r, rank1, rank2, *x, *xs;

  if( m < 1 ) {
    fprintf ( stderr, "rank to exponent: invalid dimension %d\n",m );
    exit(4);
  }

  if( rank < 1 ) {
    fprintf ( stderr, "rank to exponent: invalid rank %d\n",m );
    exit( 5 );
  }

  if( m == 1 ) {
    x = ( int * )malloc ( m * sizeof( int ) );
    x[0] = rank - 1;
    return x;
  }

  rank1 = 1;
  nm = -1;
  for( ; ; ) {
    nm = nm + 1;
    r = n_choose_k ( nm + m - 1, nm );
    if( rank < rank1 + r ) {
      break;
    }
    rank1 = rank1 + r;
  }

  rank2 = rank - rank1;

  ns = nm + m - 1;
  xs = ( int * )malloc ( m * sizeof ( int ) );
  
  j = 1;

  for( i = 1; i <= m - 1; i++ ) {
    r = n_choose_k ( ns - j, m - 1 - i );

    while( r <= rank2 && 0 < r ) {
      rank2 = rank2 - r;
      j = j + 1;
      r = n_choose_k ( ns - j, m - 1 - i );
    }
    xs[i-1] = j;
    j = j + 1;
  }

  x = ( int * ) malloc ( m * sizeof ( int ) );
  x[0] = xs[0] - 1;
  for ( i = 2; i < m; i++ )
  {
    x[i-1] = xs[i-1] - xs[i-2] - 1;
  }
  x[m-1] = ns - xs[m - 2];

  free ( xs );

  return x;
}

/******************************************************************************/

struct _mn_term {
  int i, j;
  struct _mn_term *next;
};

typedef struct _mn_term mn_term;

struct _mn_bin {
  mn_term *term;
};

typedef struct _mn_bin mn_bin;

mn_term *mn_new_term(int i, int j) {
  
  mn_term *term = (mn_term *)malloc(sizeof(mn_term));
  
  if(term == NULL) {
    fprintf(stderr, "Out of memory when allocating new term.\n");
    exit(9);
  }
  
  (void)bzero((void *)term, sizeof(term));
  term->i = i;
  term->j = j;

  return term;
}

void mn_destroy_bin( int n, mn_bin *bin) {
  int i;

  for(i = 1; i < n; i++) {
    mn_term *next, *term = bin[i].term;
    bin[i].term = NULL;
    while(term) {
      next = term->next;
      free(term);
      term = next;
    }
  }
}

int *mn_rearrange_indicies(int m, int n) {
  int i,j,k,d,e, order;
  int *indicies;
  
  order = 1; // one xtra, o count starts with 1.
  for(d = 0; d <= n; d++) {
    order += num_monomials(m, d);
  }
  indicies = (int *)malloc( order * sizeof(int));
  if(indicies == NULL) {
    fprintf(stderr, "Unable to allocate %d bytes.\n", (int)(order * sizeof(int)));
    exit(2);
  }
  indicies[0] = 0;
  e = 0;
  j = 0;
  for(d = 0; d <= n; d++) {
    k = num_monomials(m, d);
    for(i = 0; i <k; i++) {
      indicies[j++] = e+k-i-1;
    }
    e += k;
  }
  return indicies;
}

/*
 *  m: num vars
 *  n: degree, inclusive
 */
void mn_multiply_jets( int m, int n, int flag ) {
  int i, j, k, e, d, order, *f1, *f2;
  int *indicies;
  mn_term *new_term, *old_term;

  order = 1; // one xtra, o count starts with 1.
  for(d = 0; d <= n; d++) {
    order += num_monomials(m, d);
  }
  int *e1 = (int *) malloc( order * sizeof(int));
  int *e2 = (int *) malloc( order * sizeof(int));
  mn_bin *o = (mn_bin *) malloc( order * sizeof(mn_bin));  
  int *f  = (int *) malloc( m * sizeof ( int ) );

  indicies = mn_rearrange_indicies(m, n);
  
  if( e1 == NULL || e2 == NULL || f == NULL || o == NULL) {
    fprintf(stderr, "Out of memory when allocationg e1/e2/f/o.\n");
    exit(10);
  }
  for(i=0; i < order; i++) {
    e1[i] = i+1;
    e2[i] = i+1;
  }
  bzero(o, order * sizeof(mn_bin));

  for ( j = 0; j < order; j++ ) {
    
    for ( i = 0; i < order; i++ )  {
      
      f1 = rank_to_exponent( m, e1[i] );
      f2 = rank_to_exponent( m, e2[j] );

      for ( k = 0; k < m; k++ ) {
        f[k] = f1[k] + f2[k];
      }
      e = exponent_to_rank( m, f );
      if(e < order) {
	new_term = mn_new_term(i,j);     
	old_term = o[e].term;
	new_term->next = old_term;
	o[e].term = new_term;
      }

      free( f1 );
      free( f2 );
    }
  }

  free( e1);
  free( e2);     
  free( f );

  fprintf(outfile, "void jet_m_nm_mul_jet_%s( MY_JET c, MY_JET a, MY_JET b) { \n", suffix);
  fprintf(outfile, "\tstatic mycoef_t tmp1, tmp2;\n");
  fprintf(outfile, "\tstatic int inited = 0;\n");
  fprintf(outfile,"#pragma omp threadprivate(tmp1, tmp2, inited)\n");
  fprintf(outfile,"\t if(inited == 0) {\n");
  fprintf(outfile,"\t\tmycoef_init(tmp1); mycoef_init(tmp2);\n");
  fprintf(outfile,"\t\tinited =1;\n\t}\n"); 

  
#define IDX(i) indicies[i]
  e=0;
  for(d = 0; d <= n; d++) {
    k = num_monomials(m, d);
    if(d > 0)   fprintf(outfile, "\tif(nm_working_degree >= %d) {\n\n", d); 	          
    for(i = e+k; i >e; i--) {
      //for(i=1; i < order; i++) {
      mn_term *term = o[i].term;
      if(term) {
	int cnt = 0;
	int out=0;	
	mn_term *t = term;
	while(t) { cnt++; t = t->next;}
	
	if(cnt == 1) { // special one term case
	  if(d > 0)
            fprintf(outfile, "\t\tmycoef_mul2(c[%d], a[%d], b[%d]);\n\n", IDX(i-1), IDX(term->i), IDX(term->j));
	  else
            fprintf(outfile, "\tmycoef_mul2(c[%d], a[%d], b[%d]);\n\n", IDX(i-1), IDX(term->i), IDX(term->j));
	} else {
	  //fprintf(outfile, "\tMakeMyFloatA(tmp2, 0);\n");
	  while(term) {
            fprintf(outfile, "\t\tmycoef_mul2(tmp1, a[%d], b[%d]);\n", IDX(term->i), IDX(term->j));
	    if(term->next != NULL) {
	      if(out == 0) {
                fprintf(outfile, "\t\tmycoef_set(tmp2, tmp1);\n");
		out = 1;
	      } else {
                fprintf(outfile, "\t\tmycoef_add2(tmp2, tmp1, tmp2);\n");
	      }
	    } else {
              fprintf(outfile, "\t\tmycoef_add2(c[%d], tmp1, tmp2);\n", IDX(i-1));
	    }
	    term = term->next;
	  }
	  fprintf(outfile,"\n");
	  //fprintf(outfile, "\tAssignMyFloat(c[%d], tmp2);\n", IDX(i-1));	      	  
	}
      } else {
        fprintf(outfile, "\tmycoef_set_d(c[%d], 0);\n", IDX(i-1)); // shouldn't be here
      }
    }
    if(d>0) fprintf(outfile, "\t}\n"); 	      	    
    e += k;
  }
#undef IDX
  
  if(flag) {
    fprintf(outfile, "\n/*Bases:\n");
    d=0;
    for(i= 1; i < order; i++) {
      if(i == 1) {
	fprintf(outfile, "%d => %d:  1\n", i-1, indicies[i-1] );
      } else {
	f1 = rank_to_exponent ( m, i );
	k = mn_degree(m, f1);
	if( k != d) {
	  fprintf(outfile, "\n");
	  d = k;
	}
	fprintf(outfile, "%d => %d: ", i-1, indicies[i-1]);
      
	for(j = 0; j < m; j++) {
	  if(f1[j]) {
	    if(f1[j] == 1) {
	      fprintf(outfile, " s%d", j+1);
	    } else {
	      fprintf(outfile, " s%d^%d", j+1,f1[j]);
	    }
	  }
	}
	fprintf(outfile, "\n");
	free( f1 );    
      }
    }
    fprintf(outfile, "*/\n");
  }
  fprintf(outfile, "}\n\n");
  mn_destroy_bin(order, o);

  free( indicies );
  
  return;
}


void genlexia(int *c, int n) {
  int ordp, i, inz;
  for (inz=0; inz<n; inz++) if (c[inz]!=0) break;
  if (inz==n) return;
  if (inz==n-1) { c[0]=c[n-1]; c[n-1]=0; return; }
  ordp=c[inz]+c[inz+1];
  c[inz+1]++;
  for (i=1; i<=inz; i++) c[i]=0;
  c[0]=ordp-c[inz+1];
}

int *tree_monomial_order_map(int dim, int kmax) {
  int i, j, k, m, r, order,*im, *indicies, *map;

  im = ( int * ) malloc ( (1+dim) * sizeof ( int ) );
  indicies = mn_rearrange_indicies(dim, kmax);
  order = 1; 
  for(i = 0; i <= kmax; i++) order += num_monomials(dim, i);

  map = (int *)malloc( order * sizeof(int));
  if(map == NULL) {
    fprintf(stderr, "Unable to allocate %d bytes.\n", (int)(order * sizeof(int)));
    exit(2);
  }  
  bzero(map, order * sizeof(int));
  map[order-1] = -1;
  
  j = 0;
  for(k=0; k<=kmax; k++) {
    im[0]=k;
    for(i=1; i<dim; i++) im[i]=0;
    
    for(m=0; m < num_monomials(dim,k); m++,genlexia(im,dim)) {
      r = exponent_to_rank (dim,im);
      //fprintf(stderr, "%d %d %d: ", indicies[r-1], j, r-1);
      map[indicies[r-1]] = j++;
      //for (i=0; i<dim; i++) fprintf(stderr, "%d ", im[i]);
      //if (m==0) { fprintf(stderr," ( ord %d )", k); }
      //fprintf(stderr, "\n");
    }
  }
  if(indicies) free(indicies);
  return map;
}

void print_jet_tree_monomial_index_mapper(const char *prefix, const char *suffix, int dim, int kmax) {
  int j=0, *p, *mapper;  
  fprintf(outfile, "static int _%s_tr_idx_map_%s_[] = {\\\n",prefix,suffix);

  mapper = tree_monomial_order_map(dim, kmax);
  p = mapper;
  while(*p >= 0) {
    fprintf(outfile, "%3d,", *p);
    p++; j++;
    if(j == 20) {
      fprintf(outfile, "\\\n");
      j=0;
    }
  }
  free(mapper);
  fprintf(outfile, " -1};\n");
}

void output_jet_multiplication_code(void) {
  if (outfile == NULL) return;  
  
  if(index_my_jet_code == CODE_JET_m) {
    fprintf(outfile, "/*Multiplication table for degree %d in %d symbols.*/\n", deg_jet_vars,num_jet_symbols);
    mn_multiply_jets( num_jet_symbols, deg_jet_vars,0);
  }
}


int list_monomials2(int degree, FILE *file ) {
  int i, j, k, d, order, *f1;
  int *indicies;
  int m = num_jet_symbols;
  int n = degree;

  order = 1; // one xtra, o count starts with 1.
  for(d = 0; d <= n; d++) {
    order += num_monomials(m, d);
  }

  if(order > 1000) return 1;
  
  indicies = mn_rearrange_indicies(m, n);

  if(1) {
    d=0;
    for(i= 1; i < order; i++) {
      if(i > 1) {
	f1 = rank_to_exponent ( m, indicies[i-1]+1 );
	k = mn_degree(m, f1);
	if( k != d) {
	  fprintf(outfile, "\n");
	  d = k;
	}
	if(k == degree) {
	  fprintf(outfile, "\"");	  
	  for(j = 0; j < m; j++) {
	    if(f1[j]) {
	      if(f1[j] == 1) {
		fprintf(outfile, "s%d", j+1);
	      } else {
		fprintf(outfile, "s%d^%d", j+1,f1[j]);
	      }
	    }
	  }
	  fprintf(outfile, "\",");	  
	}
	free( f1 );    
      }
    }
  }
  free( indicies );
  
  return 0;
}    


int output_taylor_jet_reducer(FILE *outfile) {
  int i, j, d, order, *f1;
  int *indicies;
  int m = num_jet_symbols;
  int n = deg_jet_vars;

  order = 1; // one xtra, o count starts with 1.
  for(d = 0; d <= n; d++) {
    order += num_monomials(m, d);
  }
  
  indicies = mn_rearrange_indicies(m, n);

  fprintf(outfile, "static MY_FLOAT *taylor_jet_monomial_values(double *params) {\n");
  fprintf(outfile, "\tMY_FLOAT ftmp, ptmp, qtmp, one;\n");    
  fprintf(outfile, "\tint i;\n");
  fprintf(outfile, "\tMY_FLOAT *monomial_values = (MY_FLOAT *)malloc( (%d) * sizeof(MY_FLOAT));\n\n", order);


  fprintf(outfile, "\tfor(i=0;i<%d;i++) {\n",order);
  fprintf(outfile, "\t\tInitMyFloat(monomial_values[i]);\n");
  fprintf(outfile, "\t}\n");
  fprintf(outfile, "\tInitMyFloat(ftmp);\n");
  fprintf(outfile, "\tInitMyFloat(ptmp);\n");
  fprintf(outfile, "\tInitMyFloat(qtmp);\n");    
  fprintf(outfile, "\tInitMyFloat(one);\n");
  fprintf(outfile, "\tMakeMyFloatA(one,1);\n");  
  
  if(1) {
    for(i= 1; i < order; i++) {
      if(i > 1) {
        f1 = rank_to_exponent( m, indicies[i-1]+1 );

	{
	  fprintf(outfile, "\t/* \"");	  	  
	  for(j = 0; j < m; j++) {
	    if(f1[j]) {
	      if(f1[j] == 1) {
		fprintf(outfile, "s%d", j+1);
	      } else {
		fprintf(outfile, "s%d^%d", j+1,f1[j]);
	      }
	    }
	  }
	  fprintf(outfile, "\" */\n");
	  fprintf(outfile, "\tAssignMyFloat(ftmp, one);\n");	  
	  for(j = 0; j < m; j++) {	  
	    if(f1[j]) {
	      if(f1[j] == 1) {
		fprintf(outfile, "\tMultiplyMyFloatD(ftmp,  ftmp,  params[%d]);\n", j);
	      } else {
		fprintf(outfile, "\tMakeMyFloatA(ptmp,  params[%d]);\n", j);
		fprintf(outfile, "\tExponentiateMyFloatIA(qtmp, ptmp, (double)(%d));;\n", f1[j]);				
		fprintf(outfile, "\tMultiplyMyFloatA(ftmp, ftmp, qtmp);\n");		
	      }
	    }
	  }
	  fprintf(outfile, "\tAssignMyFloat(monomial_values[%d], ftmp);\n\n", i-1);
	}
	free( f1 );
      }
    }
    fprintf(outfile, "\tClearMyFloat(ftmp);\n");
    fprintf(outfile, "\tClearMyFloat(ptmp);\n");
    fprintf(outfile, "\tClearMyFloat(qtmp);\n");    
    fprintf(outfile, "\tClearMyFloat(one);\n\n");

    //fprintf(outfile,"for(i=0;i<%d;i++)fprintf( stderr, \"%%f \",monomial_values[i]);fprintf(stderr,\"\\n\");", order);

    fprintf(outfile, "\treturn monomial_values;\n");    
    fprintf(outfile, "}\n\n");


    fprintf(outfile, "void taylor_jet_reduce(MY_JET a, double *params) {\n");
    fprintf(outfile, "\tstatic int inited=0;\n");
    fprintf(outfile, "\tstatic MY_FLOAT ftmp;\n");
    fprintf(outfile, "#pragma omp threadprivate(inited,ftmp)\n");
    fprintf(outfile, "\tint i;\n");
    fprintf(outfile, "\tMY_FLOAT *monomials = taylor_jet_monomial_values(params);\n\n");
    fprintf(outfile, "\tif(inited==0){ inited=1;InitMyFloat(ftmp); }\n");    

    //fprintf(outfile, "\tfor(i=1; i< %d; i++) {\n", order-1); fprintf(outfile, "fprintf(stderr, \"%%f \"," MY_JET_DATA_ACCESS "((a),i);}\nfprintf(stderr,\"\\n\");\n");
    
    fprintf(outfile, "\tfor(i=1; i< %d; i++) {\n", order-1);
    fprintf(outfile, "\t\tMultiplyMyFloatA(ftmp, " MY_JET_DATA_ACCESS "((a),i),monomials[i]);\n");
    fprintf(outfile, "\t\tAddMyFloatA(" MY_JET_DATA_ACCESS "(a,0), " MY_JET_DATA_ACCESS "(a,0), ftmp);\n");
    fprintf(outfile, "\t}\n");
    fprintf(outfile, "\tfor(i=0; i< %d; i++) {ClearMyFloat(monomials[i]);}\n", order);    
    fprintf(outfile, "\tfree(monomials);\n");
    fprintf(outfile, "}\n");
  }
  free( indicies );
  return 0;    
}
