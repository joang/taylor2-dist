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
#include "_float128Header.h"
#include "complexheader.h"
#include "jetIOHelper.h"
#include "VERSION"
int    debug = 0;
int    ddouble = 0;
int    expandpower = 1;
int    expandsum = 10;
int    havesum = 0;
int    genHeader = 0;
int    genMain = 0;
int    genStep = 0;
int    genMyJet = 0;
int    genTestJet = 0;
int    genJet = 0;
int    ignoreJetInconsistency=0;
int    genJetHelper = 0;
int    jetStorageType = 0;
int    cdouble = 0;
int    complexx = 0;
int    gmp = 0;
int    mpfr = 0;
int    gmp_precision=0;
int    mpfr_precision=0;
int    ldouble=0;
int    float128=0;
int    qd2 = 0;
int    qd4 = 0;
int    f77hook = 0;
int    step_ctrl = 1;
int    use_rational_exponent=0;
char   *outNames[10];
char   *suffixes[10];
int    num_names = 0;
char   *outName = NULL;
char   *suffix = NULL;
char   *my_jet_prefix=NULL;
index_my_jet_header_t index_my_jet_header=0;
index_my_jet_code_t index_my_jet_code=0;
char   *header_name = NULL;
char   *uss_name = NULL;
char   *uso_name = NULL;
FILE   *outfile = NULL;
FILE   *infile = NULL;

// static int n_jets=0, d_jets=0;

extern FILE *yyin;

void help(char *);
void genSampleHeader();
void genJetHelpers(int flag); 
int num_monomials(int nvars,int deg);
int coefficients_count(int nvars, int deg);
int list_monomials(int d, FILE *file);
int list_monomials2(int d, FILE *file);
int output_taylor_jet_reducer(FILE *outfile); 
void mn_multiply_jets( int m, int n, int flag );
int *mn_rearrange_indicies(int m, int n) ;
int *tree_monomial_order_map();

extern int yyparse();

int main(int ac, char **av)
{
  int i, header=0, nariths=0, header_only = 0, jhelper=0, jhelper_only=0;
  int num_symbols_tmp_copy, deg_jet_vars_tmp_copy;
  char *arith=NULL;
  char *overwriteJetLibrary=NULL;

  for(i = 1; i < ac; i++)
    {
      char *arg = av[i];
      if (arg[0] == '-')
	{
	  switch(arg[1])
	    {
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
	    case 'u':
              if (!strcmp(arg,"-u") || !strcmp(arg,"-ud") || !strncmp(arg,"-userdefined", 9))
		{
                  if (i+2 < ac && av[i+1][0] != '-' && av[i+2][0] != '-' ) {
		    i++; uss_name = av[i];
                    i++;  uso_name = av[i];
		  } else {  help(av[0]);  }
		}
              break;
	    case 'f':
              if (!strcmp(arg,"-f77")) { f77hook=1;}
              if (!strcmp(arg,"-float128") || !strcmp(arg,"-f128")) {
		qd4 = 0; qd2 = 0; ddouble=0; gmp=0;  ldouble=0; float128=1; nariths++; arith="_float128";
	      }
	      break;	      	      
	      break;
            case 'm':
              if (!strcmp(arg,"-main_only") || !strcmp(arg,"-mainonly")) { genMain = 1; }
              else if (!strcmp(arg,"-main")) { genMain = 2; }
              else if (!strcmp(arg,"-mpfr"))
              { ddouble = 0; gmp=0; ldouble=0; mpfr=1; nariths++; arith="mpfr";}
              else if (!strcmp(arg,"-mpfr_precision")) {
                if ( ac > i+1 && atoi(av[i+1]) > 0)
                  {
                    ddouble=0; ldouble=0; float128=0;
                    gmp =0; qd4 = qd2 = 0; mpfr=1;
                    mpfr_precision = atoi(av[i+1]);
                    i++;
                    nariths++; arith="mpfr";
                  } else {
                    fprintf(stderr,"\t The -mpfr_precision flag must be followed by PRECISION in number of bits\n");
                    exit(1);
                  }	
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
            case 't':
//              if (!strcmp(arg,"-testjet")) { genMyJet = 1;}
              if (!strcmp(arg,"-testjet")) { genTestJet = 1;}
              break;
	    case 'h':
              if (!strcmp(arg,"-header_name") || !strcmp(arg,"-headername")  ) { if (++i < ac) { header_name = av[i];} }
              else if (!strcmp(arg,"-header")) { header = 1;}
	      else { help(av[0]);}
	      break;
	    case 'd':
              if (!strcmp(arg,"-debug") || !strcmp(arg,"-d"))
		debug = 1;
              break;
	    case 'l':
              if (!strcmp(arg,"-long_double") || !strcmp(arg,"-longdouble")) {
		 float128=0; qd4 = 0; qd2 = 0; ddouble=0; gmp=0;  ldouble=1; nariths++; arith="longdouble";
	      }
	      break;
            case 'g':
              if (!strcmp(arg,"-gmp_precision")) {
                if ( ac > i+1 && atoi(av[i+1]) > 0)
                  {
                    ddouble=0; ldouble=0; float128=0;
                    gmp =1; qd4 = qd2 = 0;
                    gmp_precision = atoi(av[i+1]);
                    i++;
                    nariths++; arith="gmp";
                  } else {
                    fprintf(stderr,"\t The -gmp_precision flag must be followed by PRECISION in number of bits\n");
                    exit(1);
                  }
              } else if (!strcmp(arg,"-gmp")) {
                ddouble=0; ldouble=0; float128=0;
                gmp =1; qd4 = qd2 = 0;
                nariths++; arith="gmp";
              }
              break;
            case 'c':
              if (!strcmp(arg,"-constantsafe") || !strcmp(arg,"-const"))
                { cdouble = 1;}
              else if (!strcmp(arg,"-complex") || !strcmp(arg,"-cmplx")) 
              {ddouble = 0; gmp=0; ldouble=0; mpfr=0;complexx=1; arith="cmplx";}
	      break; 
	    case 'e':
              if (!strncmp(arg,"-expandsum",8))
		{
                  if (++i < ac) expandsum = atoi(av[i]);
		  else --i;
		}
              else if (!strncmp(arg,"-expandpower",8))
		{
                  if (++i < ac) expandpower = atoi(av[i]);
		  else --i;
		}
	      break;
            case 'i':
              if (!strcmp(arg,"-ignore_jet_inconsistency") || !strcmp(arg,"-ignoreJI") || !strcmp(arg,"-ignoreji"))
		ignoreJetInconsistency=1;
	      break;
	    case 'v':
              if (!strcmp(arg,"-v") ||  !strcmp(arg,"-verbose") ) debug = 1;
              else  if (!strcmp(arg,"-version"))
                {
                  fprintf(stderr,"%s\n", versionString);
                  exit(0);
                }
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
	    default:
	      break;
	    }
	}
      else {
        if ((infile = fopen(av[i], "r")) == NULL)
          {
            fprintf(stderr, "Cannot open '%s'\n", av[i]);
            exit(2);
          } else {
            if (suffix == NULL)
              {
                int ii, len = strlen(av[i]);
                char *s = av[i];
                suffix = (char *)malloc( (len+len+128)*sizeof(char));
                for(ii=0; ii<len; ii++) {
                  char c = s[ii];
                  if ( isalnum(c)) { suffix[ii] =c;} else {suffix[ii]='_';}
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

  if (header_name == NULL) { header_name = "taylor.h";}

  initialize_vars();
  yyin = infile;
  yyparse();
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
  
  num_symbols_tmp_copy = num_symbols;
  deg_jet_vars_tmp_copy = deg_jet_vars;
  
  if(overwriteJetLibrary != NULL)
    {
      /*
       * if overwrite is requested, and the supplied library does not apply, 
       * we'll pick one that applies
       */
      
      if(!strcmp(overwriteJetLibrary, "1_1") || !strcmp(overwriteJetLibrary, "jet1_1")) {
	// Macros, can only be used in 1_1
      } else if(!strcmp(overwriteJetLibrary, "1_n") || !strcmp(overwriteJetLibrary, "jet1")) {
        // 1 symbol, arbitary degree
      } else if(!strcmp(overwriteJetLibrary, "n_1") || !strcmp(overwriteJetLibrary, "jet_1")) {
        // n symbols,  degree 1
      } else if(!strcmp(overwriteJetLibrary, "2_n") || !strcmp(overwriteJetLibrary, "jet2")) {
	// 2 symbols, fall through by default
      } else if(!strcmp(overwriteJetLibrary, "n_2") || !strcmp(overwriteJetLibrary, "jet_2")) {
	// deg=2, n symbols. only overrite 2_n with 2 symbols.
      } else if(!strcmp(overwriteJetLibrary, "m_n") || !strcmp(overwriteJetLibrary, "jet_m")) {
          // can overwrite all
          num_symbols_tmp_copy = -2;
          deg_jet_vars_tmp_copy= -2;
      } else if(!strcmp(overwriteJetLibrary, "tree") || !strcmp(overwriteJetLibrary, "jet_tree")) {
	// can overwrite all
        num_symbols_tmp_copy = -1;
        deg_jet_vars_tmp_copy= -1;
      } else {
        fprintf(stderr, "jet lib invalid\n"); fflush(stderr); exit(1);
      }
    }
  else if(num_symbols > 0 && deg_jet_vars > 0)
    {
      num_symbols_tmp_copy = -1;
      deg_jet_vars_tmp_copy= -1;
    }

  if (num_symbols_tmp_copy ==0 || deg_jet_vars_tmp_copy==0)
    {
      index_my_jet_header = HEADER_NONE;
      index_my_jet_code = CODE_NONE;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      fprintf(outfile, "/* No jet */\n");      
    }
  else if (num_symbols_tmp_copy==1 && deg_jet_vars_tmp_copy==1)
    {
      index_my_jet_header = HEADER_JET1_1;
      index_my_jet_code = CODE_JET1_1;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      fprintf(outfile, "/* Using jet lib: jet1_1. one symbol, degree one */\n");      
    }
  else if (deg_jet_vars_tmp_copy==1)
    {
      index_my_jet_header = HEADER_JET_1;
      index_my_jet_code = CODE_JET_1;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      fprintf(outfile, "/* Using jet lib: jet_1. degree one, n symbols*/\n");            
    }
  else if (num_symbols_tmp_copy==1)
    {
      index_my_jet_header = HEADER_JET1;
      index_my_jet_code = CODE_JET1;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      fprintf(outfile, "/* Using jet lib: jet1. one symbol, degree n */\n");            
    }
  else if (num_symbols_tmp_copy==2)
    {
      index_my_jet_header = HEADER_JET2;
      index_my_jet_code = CODE_JET2;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      jetStorageType=1;
      fprintf(outfile, "/* Using jet lib: jet2. 2 symbols, degree n */\n");            
    }
  else if (deg_jet_vars_tmp_copy==2)
    {
      index_my_jet_header = HEADER_JET_2;
      index_my_jet_code = CODE_JET_2;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      fprintf(outfile, "/* Using jet lib: jet_2. n symbols, degree 2 */\n");            
    }
  else if (deg_jet_vars_tmp_copy==-2)
    {
      index_my_jet_header = HEADER_JET_m;
      index_my_jet_code = CODE_JET_m;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      fprintf(outfile, "/* Using jet lib: jet_m.   general lib */\n");            
    }
  else
    {
      index_my_jet_header = HEADER_JET_TREE;
      index_my_jet_code = CODE_JET_TREE;
      my_jet_prefix = my_jet_prefixes[index_my_jet_code];
      jetStorageType=2;
      fprintf(outfile, "/* Using jet lib: jet_tree. general lib */\n");            
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
  exit(0);
}
/********************************************************/
void help(char *name)
{
  fprintf(stderr, "Taylor %s\n\n", versionString);
  fprintf(stderr, "Usage: %s \n", name);
  fprintf(stderr, "  [-name ODE_NAME]\n");
  fprintf(stderr, "  [-o outfile] \n");
  fprintf(stderr, "  [-qd_real | -dd_real | -gmp | -gmp_precision PRECISION ] \n");
  fprintf(stderr, "  [-mpfr | -mpfr_precision PRECISION ] \n");
  fprintf(stderr, "  [-complex ] \n");
  fprintf(stderr, "  [-main | -header | -jet | -jet_helper | -main_only] \n");
  fprintf(stderr, "  [-step STEP_CONTROL_METHOD ]\n");
  fprintf(stderr, "  [-u | -userdefined ] STEP_SIZE_FUNCTION_NAME ORDER_FUNCTION_NAME \n");
  fprintf(stderr, "  [-f77 ]\n");
  fprintf(stderr, "  [-sqrt ]\n");
  fprintf(stderr, "  [-headername HEADER_FILE_NAME]\n");
  fprintf(stderr, "  [-debug] [-help] [-v]  file\n");
  exit(0);
}
/********************************************************/

void genSampleHeader(void)
{  
  int njets =num_jet_vars;
  int djets =deg_jet_vars;
  
  int i,n,jsize=1;

  fprintf(outfile, "#ifdef __cplusplus\nextern \"C\"\n{\n#endif\n");
  
  fprintf(outfile,"/***********************************************************************\n");
  fprintf(outfile," *\n");
  fprintf(outfile," * Code generated by the TAYLOR translator.\n");
  fprintf(outfile," */\n\n");

  fprintf(outfile,"#ifndef _NUMBER_OF_JET_VARS_\n");
  fprintf(outfile,"#define _NUMBER_OF_STATE_VARS_         %d\n", neqns - nonautonomous);  
  fprintf(outfile,"#define _NUMBER_OF_JET_VARS_           %d\n", njets);
  // 20220608+
  fprintf(outfile,"#define _NUMBER_OF_SYMBOLS_            %d\n", num_symbols);
  
  fprintf(outfile,"#define _DEGREE_OF_JET_VARS_           %d\n", djets);
  
  // 20220608r
  //fprintf(outfile,"#define _JET_COEFFICIENTS_COUNT_TOTAL_ %d\n",coefficients_count(njets,djets));
  fprintf(outfile,"#define _JET_COEFFICIENTS_COUNT_TOTAL_ %d\n",coefficients_count(num_symbols,djets));
  
  fprintf(outfile,"static int _number_of_jet_vars_ =      %d;\n", njets);
  // 20220608+
  fprintf(outfile,"static int _number_of_symbols_  =      %d;\n", num_symbols);
  
  fprintf(outfile,"static int _degree_of_jet_vars_ =      %d;\n", djets);
  fprintf(outfile,"static int _monomial_counts_[]  =      {1");
  for(i=1; i<= djets; i++) {
    int k=num_monomials(num_symbols, i);
    jsize +=k;
    fprintf(outfile, ",%d", k);
  }
  fprintf(outfile, "};\n");
  fprintf(outfile,"static int _monomial_offsets_[] =      {0");
  n=0;
  for(i=0;i<= djets; i++) {
    // 20220608r
    //n += num_monomials(njets, i);
    n += num_monomials(num_symbols, i);    
    fprintf(outfile, ",%d", n);
  }
  fprintf(outfile, "};\n");
  fprintf(outfile, "static int _size_of_jet_variable_ =    %d;\n", jsize);

  fprintf(outfile,"#define _SIZE_OF_JET_VAR_              %d\n", jsize);    
  fprintf(outfile,"#endif\n\n");


  fprintf(outfile, "#ifndef _TAYLOR_H_\n");
  fprintf(outfile, "#define _TAYLOR_H_\n");
  if (qd4 | qd2 ) { /* use library qd */
    fprintf(outfile, "#include <iostream>\n");
    fprintf(outfile, "#include <qd/qd_real.h>\n");
    fprintf(outfile, "#include <qd/fpu.h>\n");
    fprintf(outfile, "using namespace std;\n");    
    if (qd4) {  /* use quad double */
      fprintf(outfile,"typedef qd_real MY_FLOAT;\n");      
    } else if (qd2) { /* use double double */
      fprintf(outfile,"typedef dd_real MY_FLOAT;\n");      
    }
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");          
    fprintf(outfile, "%s\n",qd_header);

    fprintf(outfile, "#endif\n");    
//    fprintf(outfile, "%s\n",qd_header_JET);
//    jetHeader = strlen(qd_header_JET);
  } else if (mpfr) {
    fprintf(outfile,"#include<math.h>\n");      
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");              
    fprintf(outfile,"#include<mpfr.h>\n");      
    fprintf(outfile, "%s\n", mpfr_header);

    fprintf(outfile, "#endif\n");        
//    fprintf(outfile, "%s\n", mpfr_header_JET);
//    jetHeader = strlen(mpfr_header_JET);
  } else if (gmp) {
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");              
    fprintf(outfile,"#include<gmp.h>\n");      
    fprintf(outfile,"#include<math.h>\n");      
    fprintf(outfile, "%s\n",gmp_header);

    fprintf(outfile, "#endif\n");    
//    fprintf(outfile, "%s\n",gmp_header_JET);
//    jetHeader = strlen(gmp_header_JET);
  } else if (ldouble) {
    fprintf(outfile,"typedef long double MY_FLOAT;\n\n");
    fprintf(outfile,"#include<math.h>\n");      
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");              
    fprintf(outfile, "%s\n",longdouble_header);
    fprintf(outfile, "#endif\n");    
  } else if (float128) {
    fprintf(outfile,"#include<math.h>\n");      
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");
    fprintf(outfile,"#include<quadmath.h>\n");
    fprintf(outfile,"typedef __float128 MY_FLOAT;\n\n");        
    fprintf(outfile, "%s\n",_float128_header);
    fprintf(outfile, "#endif\n");    
  } else if (complexx) {
    fprintf(outfile,"#include<complex.h>\n");      
    fprintf(outfile,"typedef double complex MY_FLOAT;\n\n");
    fprintf(outfile,"#include<math.h>\n");      
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");              
    fprintf(outfile, "%s\n",complex_header);

    fprintf(outfile, "#endif\n");    
//    fprintf(outfile, "%s\n",sample_header_JET_long_double);
//    jetHeader = strlen(sample_header_JET_long_double);
  } else {
    fprintf(outfile,"typedef double MY_FLOAT;\n\n");
    fprintf(outfile,"#include<math.h>\n");      
    fprintf(outfile,"#include<stdio.h>\n");  
    fprintf(outfile,"#include<stdlib.h>\n");  
    fprintf(outfile,"#include<string.h>\n");
    fprintf(outfile,"#include<ctype.h>\n");              
    fprintf(outfile, "%s\n",sample_header);
    fprintf(outfile, "#endif\n");
    
//    fprintf(outfile, "%s\n",sample_header_JET);
//    jetHeader = strlen(sample_header_JET);
  }

  genMyJetHeader();

  /*  function prototypes */
  {
    int i;
    for(i = 0; i < num_names; i++) {
      fprintf(outfile,"\nMY_FLOAT **%s(MY_FLOAT t, MY_FLOAT *x, int order);\n", outNames[i]);
      fprintf(outfile,"\nMY_FLOAT **%sA(MY_FLOAT t, MY_FLOAT *x, int order, int reuse_last_computation);\n", outNames[i]);
      fprintf(outfile,"\nMY_FLOAT **%s_A(MY_FLOAT t, MY_FLOAT *x, int order, int reuse_last_computation, MY_JET *jetIn, MY_JET ***jetOut);\n", outNames[i]);
      fprintf(outfile, "int       taylor_step_%s(MY_FLOAT *ti, MY_FLOAT *x, int dir, int step_ctl,\n", suffixes[i]);
      fprintf(outfile, "                          double log10abserr, double log10relerr, \n");
      fprintf(outfile, "                          MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetInOut);\n\n");
    }
  }
  fprintf(outfile, "/************************************************************************/\n\n");
  // 20220614
  if( jsize > 1)      
  {
    fprintf(outfile,"\n/* JET IO Helpers */\n\n");

    fprintf(outfile,"\n/* Set Jet values  */\n");    
    fprintf(outfile,"int taylor_make_jet(MY_JET a, MY_FLOAT *myfloats, double *values);\n");
    fprintf(outfile,"int taylor_make_identity_jets(MY_JET *inOut, MY_FLOAT *myfloats, double *values);\n");
    fprintf(outfile,"int taylor_make_unit_jet(MY_JET a, int idx, MY_FLOAT *myfloat, double *value);\n");
    fprintf(outfile,"int taylor_set_jet(MY_JET a, MY_FLOAT *myfloats, double *values, int include_state);\n");

    fprintf(outfile,"\n/* Convert Jet to array */\n");    
    fprintf(outfile,"MY_FLOAT *taylor_convert_jet_to_array(MY_JET a, int include_state);\n");

    fprintf(outfile,"\n/* Input/Output Jet */\n");
    fprintf(outfile,"int taylor_input_jet_from_stdin(MY_JET a, int idx) ;\n");
    fprintf(outfile,"int taylor_input_jet_from_string(MY_JET a, const char *str);\n");
    fprintf(outfile,"int taylor_output_jet(FILE *file, char *fmt, MY_JET a);\n");

    fprintf(outfile,"\n/* Init/Cleanup Library/Jet */\n");
    fprintf(outfile,"void taylor_initialize_jet_library();\n");    
    fprintf(outfile,"void taylor_initialize_jet_library2(int nvars, int degree);\n");
    fprintf(outfile,"void taylor_initialize_jet_variable(MY_JET *jet);\n");
    fprintf(outfile,"void taylor_clear_jet_variable(MY_JET *jet);\n");
    fprintf(outfile,"void taylor_clear_up_jet_library();\n");
    fprintf(outfile,"int taylor_set_jet_variable_number_of_symbols(int);\n");
    fprintf(outfile,"int taylor_set_jet_variable_degree(int);\n\n");
    fprintf(outfile,"void taylor_jet_reduce(MY_JET, double *);\n");
    fprintf(outfile,"MY_FLOAT *taylor_get_jet_data_array(MY_JET);\n");
    fprintf(outfile,"const char **taylor_get_variable_names();\n");                
    fprintf(outfile,"const char **taylor_get_jet_monomials();\n");            
    
  }
  // 202206014
  fprintf(outfile, "#ifdef __cplusplus\n}\n#endif\n");
}
/********************************************************/
void genJetHelpers(int add_header_h) {
  int i, jsize=1, djets =deg_jet_vars;
  
  for(i=1; i<= djets; i++) {
    int k=num_monomials(num_symbols, i);
    jsize +=k;
  }

  if( jsize > 1)  {          
    fprintf(outfile, "#ifndef JET_IO_HELPERS\n#define JET_IO_HELPERS\n\n");

    if(add_header_h)
      fprintf(outfile, "#include \"%s\"\n\n", header_name);

    //fprintf(outfile, "#ifndef JET_IO_NAMES\n#define JET_IO_NAMES\n");
    
    int i;
    fprintf(outfile,"\nstatic const char *ode_variable_names[]={ ");
    for(i = 0; i <neqns; i++) {
      fprintf(outfile, "\"%s\", ",NODE_GIVEN_NAME(equations[i].name));
    }
    fprintf(outfile, "NULL };\n");
    int djets =deg_jet_vars;

    fprintf(outfile,"\nstatic const char *jet_variable_monomials[]={\n");    
    for(i = 1; i <= djets; i++)
      list_monomials2(i, outfile);
    fprintf(outfile,"\n\nNULL\n\n};\n\n");
    
    //fprintf(outfile, "#endif\n");

    fprintf(outfile, "%s\n", JetVarIOHelpers);
    
    output_taylor_jet_reducer(outfile);
    
    fprintf(outfile, "#endif\n\n");
  }
}
  
 

/* number of degree deg monomials in nvars variables */

int num_monomials(int nvars0, int deg0) {
  long int nvars, deg;
  nvars = (long int)nvars0;
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

int exponent_to_rank ( int m, int *x ) {
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

int *rank_to_exponent ( int m, int rank ) {
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
  fprintf(outfile, "\tstatic myscal_t tmp1, tmp2;\n");
  fprintf(outfile, "\tstatic int inited = 0;\n");
  fprintf(outfile,"#pragma omp threadprivate(tmp1, tmp2, inited)\n");
  fprintf(outfile,"\t if(inited == 0) {\n");
  fprintf(outfile,"\t\tmyscal_init(tmp1); myscal_init(tmp2);\n");
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
	    fprintf(outfile, "\t\tmyscal_mul2(c[%d], a[%d], b[%d]);\n\n", IDX(i-1), IDX(term->i), IDX(term->j));
	  else
	    fprintf(outfile, "\tmyscal_mul2(c[%d], a[%d], b[%d]);\n\n", IDX(i-1), IDX(term->i), IDX(term->j));	      	    
	} else {
	  //fprintf(outfile, "\tMakeMyFloatA(tmp2, 0);\n");
	  while(term) {
	    fprintf(outfile, "\t\tmyscal_mul2(tmp1, a[%d], b[%d]);\n", IDX(term->i), IDX(term->j));
	    if(term->next != NULL) {
	      if(out == 0) {
		fprintf(outfile, "\t\tmyscal_set(tmp2, tmp1);\n");
		out = 1;
	      } else {
		fprintf(outfile, "\t\tmyscal_add2(tmp2, tmp1, tmp2);\n");
	      }
	    } else {
	      fprintf(outfile, "\t\tmyscal_add2(c[%d], tmp1, tmp2);\n", IDX(i-1));
	    }
	    term = term->next;
	  }
	  fprintf(outfile,"\n");
	  //fprintf(outfile, "\tAssignMyFloat(c[%d], tmp2);\n", IDX(i-1));	      	  
	}
      } else {
	fprintf(outfile, "\tmyscal_set_d(c[%d], 0);\n", IDX(i-1)); // shouldn't be here
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


void genlexia (int *c, int n) {
  int ordp, i, inz;
  for (inz=0; inz<n; inz++) if (c[inz]!=0) break;
  if (inz==n) return;
  if (inz==n-1) { c[0]=c[n-1]; c[n-1]=0; return; }
  ordp=c[inz]+c[inz+1];
  c[inz+1]++;
  for (i=1; i<=inz; i++) c[i]=0;
  c[0]=ordp-c[inz+1];
}

int *tree_monomial_order_map() {
  int dim=num_symbols, kmax=deg_jet_vars, i, j, k, m, r, order,*im, *indicies, *map;

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

void print_jet_tree_monomial_index_mapper() {
  int j=0, *p, *mapper;  
  fprintf(outfile, "static int _tr_idx_map_[] = {\n");

  mapper = tree_monomial_order_map();
  p = mapper;
  while(*p >= 0) {
    fprintf(outfile, "%3d,", *p);
    p++; j++;
    if(j == 20) {
      fprintf(outfile, "\n");
      j=0;
    }
  }
  free(mapper);
  fprintf(outfile, " -1};\n");
}

void output_jet_multiplication_code(void) {
  if (outfile == NULL) return;  
  
  if(index_my_jet_code == CODE_JET_m) {
    fprintf(outfile, "/*Multiplication table for degree %d in %d symbols.*/\n", deg_jet_vars,num_symbols);
    mn_multiply_jets( num_symbols, deg_jet_vars,0);
  }
}


int list_monomials2(int degree, FILE *file ) {
  int i, j, k, d, order, *f1;
  int *indicies;
  int m = num_symbols;
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
  int m = num_symbols;
  int n = deg_jet_vars;

  order = 1; // one xtra, o count starts with 1.
  for(d = 0; d <= n; d++) {
    order += num_monomials(m, d);
  }
  
  indicies = mn_rearrange_indicies(m, n);

  fprintf(outfile, "static MY_FLOAT *taylor_jet_monomial_values(double *params) {\n");
  fprintf(outfile, "\tMY_FLOAT *monomial_values = (MY_FLOAT *)malloc( (%d) * sizeof(MY_FLOAT));\n", order);
  fprintf(outfile, "\tMY_FLOAT ftmp, ptmp, qtmp, one;\n");    
  fprintf(outfile, "\tint i;\n\n");


  fprintf(outfile, "\tfor(i=0;i<%d;i++) {;\n",order);
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
	f1 = rank_to_exponent ( m, indicies[i-1]+1 );

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
    fprintf(outfile, "\tMY_FLOAT *monomials = taylor_jet_monomial_values(params);\n");
    fprintf(outfile, "\tstatic int inited=0;\n");
    fprintf(outfile, "\tstatic MY_FLOAT ftmp;\n");
    fprintf(outfile, "#pragma omp threadprivate(inited,ftmp)\n");
    fprintf(outfile, "\tint i;\n\n");
    fprintf(outfile, "\tif(inited==0){ inited=1;InitMyFloat(ftmp); }\n");    

    //fprintf(outfile, "\tfor(i=1; i< %d; i++) {\n", order-1); fprintf(outfile, "fprintf(stderr, \"%%f \",MY_JET_DATA((a),i);}\nfprintf(stderr,\"\\n\");\n"); 
    
    fprintf(outfile, "\tfor(i=1; i< %d; i++) {\n", order-1);
    fprintf(outfile, "\t\tMultiplyMyFloatA(ftmp, MY_JET_DATA((a),i),monomials[i]);\n");
    fprintf(outfile, "\t\tAddMyFloatA(MY_JET_DATA((a),0), MY_JET_DATA((a),0), ftmp);\n");
    fprintf(outfile, "\t};\n");
    fprintf(outfile, "\tfor(i=0; i< %d; i++) {ClearMyFloat(monomials[i]);}\n", order);    
    fprintf(outfile, "\tfree(monomials);\n");
    fprintf(outfile, "}\n");
  }
  free( indicies );
  return 0;    
}
