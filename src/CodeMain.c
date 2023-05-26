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
#include "Header.h"
#include "OneStep.h"
#include "OneStepF77.h"
/************************************************************************
 *
 * search for word in buf. Return 1 if found.
 * if pos_ret is not NULL, it returns the starting
 * location of the match.
 */
int matchWord(buf, word, buflen, pos_ret) 
     const char *buf; char *word; int *pos_ret, buflen;
{
  int blen, wlen;
  int pfxS[256], *pfx = pfxS;

  blen = buflen;
  wlen = strlen(word);
  if(blen < wlen) return(0);
  if(wlen > 250)  pfx = (int *) malloc( (wlen + 4) * sizeof(int));

  /* compute the prefix function */
  {
    int m = wlen;
    int i,j;
    pfx[0] = -2;
    pfx[1] = -1;
    for(i = 2; i < m; i++)
      {
	j = pfx[i-1] + 1;
	while(word[i-1] != word[j] && j >= 0) j = pfx[j]+1;
	pfx[i] = j;
      }
  }

  /* match */
  {
    int n = blen;
    int m = wlen;
    int i=0, j=0;

    while(i < n)
      {
	if(word[j] == buf[i]) {j++; i++;}
	else 
	  {
	    j = pfx[j] + 1;
	    if(j < 0) {j = 0; i++;}
	  }
	if(j == m)
	  {
	    if(pos_ret) *pos_ret = i-m;
	    if(pfx != pfxS) free(pfx);
	    return(1);
	  }
      }
  }
  if(pfx != pfxS) free(pfx);
  return(0);
}
/***************************************************************************/


void genMainCode()
{
  int i,j, nsteps=0, stoptime=0; 
  double dtmp;

  fprintf(outfile,"/***********************************************************************\n");
  fprintf(outfile," *\n");
  fprintf(outfile," * Code generated by the TAYLOR translator.\n");
  fprintf(outfile," */\n\n");

  fprintf(outfile,"char *get_a_number_stdin(char *buf) {\n");
  fprintf(outfile,"    int i=0, k=0; char p = getc(stdin);\n");
  fprintf(outfile,"    while(p && (!isdigit(p)) && p != '.' && p != '-') { p = getc(stdin);  }\n");
  fprintf(outfile,"    while(p && (isdigit(p) || p == '.' || p == '-')) { buf[i++] = p; p = getc(stdin);}\n");
  fprintf(outfile,"    buf[i++] = '\\0'; return buf;\n");
  fprintf(outfile,"}\n\n");
  

  fprintf(outfile,"int main(int argc, char **argv)\n{\n");

  fprintf(outfile,"\t int       i, j, order=20, itmp=0, direction = 1, nsteps = -1;\n");
  fprintf(outfile,"\t double    ftmp, dstep, tolerance, rtolerance, log10tolerance, log10rtolerance;\n");
  fprintf(outfile,"\t MY_FLOAT  startT, stopT, nextT;\n");
  fprintf(outfile,"\t MY_FLOAT  myFloatZero;\n");
  fprintf(outfile,"\t char  format_string[32];\n");  
  
  j = neqns + 1;
  fprintf(outfile,"\t MY_FLOAT  xx[%d], yy[%d], zz[%d];\n", j,j,j);
  fprintf(outfile,"\t MY_FLOAT  **jet;\n");
  fprintf(outfile,"\t MY_JET    *jetOut;\n");
  fprintf(outfile,"\t MY_JET    jetIn[%d];\n",   1+state_jet_vars);

  fprintf(outfile,"\t char *q, ibuf[256];\n");  
  
  if(qd2 | qd4 ) {
    fprintf(outfile,"\t unsigned int old_cw;;\n");    
    fprintf(outfile,"\t fpu_fix_start(&old_cw);\n");    
  } else if(my_float_arith == ARITH_GMP) {
    if( gmp_precision ) {
      fprintf(outfile, "\t mpf_set_default_prec(%d);\n", gmp_precision);
    } else {
      fprintf(outfile, "{ int gmppre;\n");
      fprintf(outfile, "  fprintf(stderr, \"Enter gmp precision in number of bits: \");\n");
      fprintf(outfile, "  itmp=scanf(\"%%d\", &gmppre);\n");      
      fprintf(outfile, "\t mpf_set_default_prec(gmppre);\n"); 
      fprintf(outfile, "};\n"); 
    }
  } else if(my_float_arith == ARITH_MPFR) {
    if( mpfr_precision ) {
      fprintf(outfile, "\t mpfr_set_default_prec(%d);\n", mpfr_precision);
    } else {
      fprintf(outfile, "{ int gmppre;\n");
      fprintf(outfile, "  fprintf(stderr, \"Enter mpfr precision in number of bits: \");\n");
      fprintf(outfile, "  itmp=scanf(\"%%d\", &gmppre);\n");      
      fprintf(outfile, "\t mpfr_set_default_prec(gmppre);\n"); 
      fprintf(outfile, "};\n"); 
    }
  } else if(ddouble) {fprintf(outfile, "\t cout <<setprecision(31);\n");}

  
  fprintf(outfile, "\t InitMyFloat(myFloatZero);\n");
  fprintf(outfile, "\t InitMyFloat(startT);\n");
  fprintf(outfile, "\t InitMyFloat(stopT);\n");
  fprintf(outfile, "\t InitMyFloat(nextT);\n");

  fprintf(outfile, "\t for(i=0; i<%d; i++){InitMyFloat(xx[i]); InitMyFloat(yy[i]);InitMyFloat(zz[i]);}\n",j);
  fprintf(outfile,"\n\t MakeMyFloatA(myFloatZero, 0);\n");

  if(state_jet_vars > 0) {
//     fprintf(outfile, "\t InitUpJet(%d, %d);\n", num_jet_symbols, deg_jet_vars);
    fprintf(outfile, "\t InitUpJet2(%d, %d);\n", num_jet_symbols, deg_jet_vars);
    fprintf(outfile, "\t for(i=0; i<%d; i++) {InitJet(jetIn[i]);}\n",state_jet_vars+param_jet_vars);
  }

if(1 || state_jet_vars==0) {  
  fprintf(outfile, "\n\t /* assign initials */\n");
  if(controlParams.ninitials > 0)
    {
      int cnt; char sbuf[256];
      if(controlParams.ninitials != neqns - nonautonomous)
        fprintf(stderr, "Warning: number of initial values does not match number of equations!\n");
      if(controlParams.ninitials > neqns - nonautonomous)
        cnt = neqns - nonautonomous;
      else cnt = controlParams.ninitials;
      for(i = 0; i < cnt; i++)
        {
          double dtmp = cstNodeValue(controlParams.initials[i]);
          fprintf(outfile,"\t MakeMyFloatC(xx[%d],\"%s\", (double)%.16g);\n", i, cstNodeStringValue(controlParams.initials[i],sbuf), dtmp);
        }
    }
  else
    {
      for(i = 0; i < neqns-nonautonomous; i++)
        {
          fprintf(outfile,"\t fprintf(stderr,\"Enter Initial xx[%d]: \"); \n",i);
	  fprintf(outfile,"\t q = get_a_number_stdin(ibuf);\n");
          fprintf(outfile,"\t ftmp = atof(q);\n");
          fprintf(outfile,"\t MakeMyFloatC(xx[%d], q, (double)ftmp);\n", i);
        }
    }
}
  /* get init values for jet vars */
  if(state_jet_vars>0) {
    Node var; char *p;

    /*
    if(param_jet_vars > 0) {
      for(i=0; i<param_jet_vars; i++) {
	Node var = jetParamsList[i];
	int j = ID_PINDEX(var);
 	int k = ID_JINDEX(var);
	fprintf(outfile, "\n    \t InitJetParameter(jetIn[%d], %d); // %s /\n", k, j-1, ID_GIVEN_NAME(var));	
      }
    }
    */

    fprintf(outfile,"\n\t /* initialize jet vars --start */\n");
    for(i=0; i< state_jet_vars+param_jet_vars; i++) {
      var =jetVarsList[i];
      p = NODE_JINITV(var);
      if(p) {
	fprintf(outfile, "\t InputJetFromString(jetIn[%d],\"%s\");\n",i,p);
      } else {
	if(i < state_jet_vars) {
	  fprintf(outfile, "\t InputJet(jetIn[%d],\"Enter init value for Jet Var %d:\",%d);\n",i,i+1,i);
	}
      }
      // make sure the constant term match xx.
      fprintf(outfile, "\t AssignMyFloat(xx[%d], MY_JET_DATA(jetIn[%d],0));\n", i, i);
    }
    fprintf(outfile,"\t /* initialize jet vars --end */\n\n");
  }

  Node pnode;
  char ibuf[256];
  
  if(getControlParameterValue("numberOfSteps",&dtmp, &pnode) || getControlParameterValue("number_of_steps",&dtmp, &pnode)||
     getControlParameterValue("NumberOfSteps",&dtmp, &pnode) || getControlParameterValue("Number_Of_Steps",&dtmp, &pnode))
    {
      nsteps = 1;
      fprintf(outfile,"\t nsteps = %d;\n", (int)dtmp);
      fprintf(outfile,"\t if(nsteps <0) nsteps = -nsteps;;\n");
    }
  if(getControlParameterValue("stopTime",&dtmp, &pnode) || getControlParameterValue("stop_time",&dtmp, &pnode)||
     getControlParameterValue("StopTime",&dtmp, &pnode) || getControlParameterValue("Stop_Time",&dtmp, &pnode))
    {
      stoptime=1;
      fprintf(outfile,"\t MakeMyFloatC(stopT, \"%s\", (double)(%.16g));\n", cstNodeStringValue(pnode,ibuf), dtmp);      
    }
  /* start time */
  if(getControlParameterValue("startTime",&dtmp, &pnode) || getControlParameterValue("start_time",&dtmp, &pnode)||
     getControlParameterValue("StartTime",&dtmp, &pnode) || getControlParameterValue("Start_Time",&dtmp, &pnode))
    {
      fprintf(outfile,"\t MakeMyFloatC(startT, \"%s\", (double)(%.16g));\n",cstNodeStringValue(pnode,ibuf), dtmp);      
    }
  else
    {
      fprintf(outfile,"\t fprintf(stderr,\"Enter start time: \"); \n");
      fprintf(outfile,"\t q = get_a_number_stdin(ibuf);\n");
      fprintf(outfile,"\t dstep = atof(q);\n");      
      fprintf(outfile,"\t MakeMyFloatC(startT, q, dstep);\n");
    }
  /* make sure we have a stop condition */
  if(nsteps <= 0 && stoptime == 0) 
    {
      stoptime = 1;
      fprintf(outfile,"\t fprintf(stderr,\"Enter stop time: \"); \n");
      fprintf(outfile,"\t q = get_a_number_stdin(ibuf);\n");
      fprintf(outfile,"\t dstep = atof(q);\n");            
      fprintf(outfile,"\t MakeMyFloatC(stopT, q, dstep);\n");
    }
  if(getControlParameterValue("minimalStep",&dtmp, &pnode) || getControlParameterValue("minimal_step",&dtmp, &pnode)||
     getControlParameterValue("MinimalStep",&dtmp, &pnode) || getControlParameterValue("Minimal_Step",&dtmp, &pnode)||
     getControlParameterValue("maximalStep",&dtmp, &pnode) || getControlParameterValue("maximal_step",&dtmp, &pnode)||
     getControlParameterValue("MaximalStep",&dtmp, &pnode) || getControlParameterValue("Maximal_Step",&dtmp, &pnode)||
     getControlParameterValue("step",&dtmp, &pnode) || getControlParameterValue("Step",&dtmp, &pnode)||
     getControlParameterValue("StepSize",&dtmp, &pnode) || getControlParameterValue("step_size",&dtmp, &pnode)||
     getControlParameterValue("stepSize",&dtmp, &pnode) || getControlParameterValue("Step_Size",&dtmp, &pnode))
    {
      fprintf(outfile,"\t MakeMyFloatC(nextT, \"%s\", (double)(%.16g) );\n",cstNodeStringValue(pnode,ibuf), dtmp);
      
      fprintf(outfile,"\t dstep = (double)%.16g;\n", dtmp);      
    }
  else 
    {
      fprintf(outfile, "\t dstep=0.001; /* only nedeed when step_ctrl_method==0 (see manual) */\n");
      fprintf(outfile,"\t MakeMyFloatC(nextT, \"0.001\", (double)dstep);\n");
    }
  /* get max error tolerance */
  if(getControlParameterValue("absoluteErrorTolerance",&dtmp, &pnode) || getControlParameterValue("absolute_error_tolerance",&dtmp, &pnode)||
     getControlParameterValue("AbsoluteErrorTolerance",&dtmp, &pnode) || getControlParameterValue("Absolute_Error_Tolerance",&dtmp, &pnode))
    {
      fprintf(outfile,"\t tolerance = (double)(%.16g);\n",dtmp);      
    }
  else 
    {
      fprintf(outfile,"\t fprintf(stderr,\"Enter absolute error tolerance: \"); \n");
      fprintf(outfile,"\t itmp=scanf(\"%%le\", &tolerance);");
    }
  if(getControlParameterValue("relativeErrorTolerance",&dtmp, &pnode) || getControlParameterValue("relative_error_tolerance",&dtmp, &pnode)||
     getControlParameterValue("RelativeErrorTolerance",&dtmp, &pnode) || getControlParameterValue("Relative_Error_Tolerance",&dtmp, &pnode))
    {
      fprintf(outfile,"\t rtolerance = (double)(%.16g);\n",dtmp);      
    }
  else 
    {
      fprintf(outfile,"\n\t fprintf(stderr,\"Enter relative error tolerance: \"); \n");
      fprintf(outfile,"\t itmp=scanf(\"%%le\", &rtolerance);");
    }

  fprintf(outfile, "\n");  
  fprintf(outfile, "\t log10tolerance  = log10(tolerance);\n");
  fprintf(outfile, "\t log10rtolerance = log10(rtolerance);\n");

  fprintf(outfile, "\n\t /* the main loop */\n");  
  fprintf(outfile, "\n\t if(dstep < (double)0.0) { direction = -1;}\n");


  if(my_float_arith == ARITH_GMP) {
    fprintf(outfile, "\t int prec = (int) mpf_get_prec(myFloatZero);\n");
    fprintf(outfile, "\t int digits = (8 * prec / 32 );\n");
    fprintf(outfile, "\t sprintf(format_string, \"%%%%.%%dFg \",digits);\n");    
  } else if(my_float_arith == ARITH_MPFR) {
    fprintf(outfile, "\t int prec = (int) mpfr_get_prec(myFloatZero);\n");
    fprintf(outfile, "\t int digits = (8 * prec / 32 );\n");
    fprintf(outfile, "\t sprintf(format_string, \"%%%%.%%dRg \",digits);\n");
  } else if(my_float_arith == ARITH_LONG_DOUBLE) {
    fprintf(outfile, "\t (void)strcpy(format_string, \"%%.20Lg \");\n");
  } else if(my_float_arith == ARITH_FLOAT128) {
    fprintf(outfile, "\t (void)strcpy(format_string, \"%%.33Qe\");\n");    
  } else {
    fprintf(outfile, "\t (void)strcpy(format_string, \"%%.16g \");\n");    
  }  
  fprintf(outfile, "\t itmp = 0; \n");      
  fprintf(outfile, "\t while(1)  {\n");
  /* output current value first */
  
  for(i=0; i < neqns-nonautonomous; i++) 
    { fprintf(outfile, "\t\t OutputMyFloat3(stdout, format_string, xx[%d]);\n", i); }
  fprintf(outfile, "\t\t OutputMyFloat3(stdout, format_string, startT);\n");
  if(state_jet_vars>0) {
    for(i=0; i < state_jet_vars; i++) {
      fprintf(outfile, "\t\t OutputJet2File(stdout, format_string, jetIn[%d]);\n",i);
      fprintf(outfile, "\t\t printf(\"  \");\n");
    }
  }
  fprintf(outfile, "\t\t printf(\"\\n\");");

  if(nsteps > 0)
    fprintf(outfile, "\t\t if(!(itmp == 0 && nsteps != 0)){break;}\n");      
  else
    fprintf(outfile, "\t\t if(itmp != 0) {break;} \n");      
  /* if stopTime is reached, break */
  fprintf(outfile, "\t\t if(MyFloatA_GE_B(startT,stopT)) { break;}\n");

  /* integrate one step */
  fprintf(outfile, "\n\t\t itmp = taylor_step_%s( &startT, xx, direction, %d, log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", 
          suffix, step_ctrl);
  fprintf(outfile, "\n\t\t //itmp = RK4_step_%s( &startT, xx, direction, %d, log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", 
          suffix, step_ctrl);
  fprintf(outfile, "\n\t\t //itmp = RK4QC_step_%s( &startT, xx, direction, %d, 6+log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", 
          suffix, step_ctrl);


  /* check stop condition */
  if(nsteps > 0) { 
    fprintf(outfile, "\t\t nsteps--;\n");
  }

  fprintf(outfile, "\t     }  /* while */ \n");      
  if(qd2 | qd4 ) {
    fprintf(outfile,"\t fpu_fix_end(&old_cw);\n");    
  }
  fprintf(outfile, "\t exit(0); \n");      
  fprintf(outfile, " } \n");      
}

void genStepCode() 
{
  fprintf(outfile,"/***********************************************************************\n");
  fprintf(outfile," *\n");
  fprintf(outfile," * Code generated by the TAYLOR translator.\n");
  fprintf(outfile," */\n\n");

  fprintf(outfile, "#define _N_DIM_ %d\n", neqns - nonautonomous);
  fprintf(outfile, "#define _J_DIM_ %d\n", state_jet_vars);
  fprintf(outfile, "\n\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "#define xInitUpJet InitUpJet\n");
    fprintf(outfile, "#define xInitJet InitJet\n");
    fprintf(outfile, "#define xNormJet NormJet\n");
    fprintf(outfile, "#define xMultiplyFloatJetA MultiplyFloatJetA\n");
    fprintf(outfile, "#define xAddJetJetA AddJetJetA \n");
    fprintf(outfile, "#define xAssignJetToJet AssignJetToJet\n");
    fprintf(outfile, "#define xAssignJetToFloat AssignJetToFloat\n");
  } else {
    fprintf(outfile, "#define xInitUpJet() /* */\n");
    fprintf(outfile, "#define xInitJet(x_) /* */\n");
    fprintf(outfile, "#define xNormJet(a_,x_) /* */\n");
    fprintf(outfile, "#define xMultiplyFloatJetA(t_, f_, a_) /* */\n"); 
    fprintf(outfile, "#define xAddJetJetA(t_,a_, b_) /* */\n");
    fprintf(outfile, "#define xAssignJetToJet(t_,s_) /* */\n");
    fprintf(outfile, "#define xAssignJetToFloat(t_,s_) /* */\n");
  }



  fprintf(outfile, "\n\n");
  /*
  fprintf(outfile, "static int (*user_defined_order_function)(double, double, double);\n");
  fprintf(outfile, "static double (*user_defined_stepsize_function)(MY_FLOAT **, int, int, double, double, double);\n");
  */
  fprintf(outfile, "\n\n");  

  /** code for step control etc **/
  {
    static char *word = "_ODE_NAME__";
    char *str = oneStepCode;
    int  length = strlen(oneStepCode);
    int  wlen = strlen(word);
    int  i, location = 0;
    while(matchWord(str, word, length, &location)) {
      for(i = 0; i < location; i++) {fputc(str[i], outfile);}
      str += location + wlen;
      length -= (location + wlen);
      fprintf(outfile, "%s", suffix);
    }
    fprintf(outfile,"%s\n", str);
  }
  /** code for use defined step control etc **/
  {
    fprintf(outfile,"int comp_order_other_%s(double lnxnorm, double log10abserr, double log10relerr){\n", suffix);
    if(uso_name) {
      fprintf(outfile, "  extern int %s(double, double, double);\n", uso_name);      
      fprintf(outfile, "  return %s(lnxnorm,log10abserr,log10relerr);\n", uso_name);      
    }
    fprintf(outfile, "\
  puts(\"---\");\n\
  puts(\"compute_order_user_defined:\");\n\
  puts(\"you have to code this routine\");\n\
  puts(\"or select a different value for the step_ctl parameter\");\n\
  puts(\"---\");\n\
  exit(1);\n\
\n\
  return(0);\n\
}\n\
");
    fprintf(outfile, "double comp_stepsize_other_%s(MY_FLOAT **s, MY_JET **jet, int nd, int nt, double xnorm, double log10abserr, double log10relerr) {\n\n", suffix);
    if(uss_name) {
      fprintf(outfile, "  extern double %s(MY_FLOAT **,int,int,double,double,double);\n", uss_name);      
      fprintf(outfile, "  return %s(s,nd,nt,xnorm,log10abserr,log10relerr);\n", uss_name);
    }
    fprintf(outfile, 
"\
  puts(\"---\");\n\
  puts(\"compute_timestep_user_defined:\");\n\
  puts(\"you have to code this routine\");\n\
  puts(\"or select a different value for the step_ctl parameter\");\n\
  puts(\"---\");\n\
  exit(1);\n\
  return((double)0.00001);\n\
}\n");
  }
  if(f77hook) {
    static char *word = "_ODE_NAME__";
    char *str = OneStepCodeF77;
    int  length = strlen(OneStepCodeF77);
    int  wlen = strlen(word);
    int  i, location = 0;
    while(matchWord(str, word, length, &location)) {
      for(i = 0; i < location; i++) {fputc(str[i], outfile);}
      str += location + wlen;
      length -= (location + wlen);
      fprintf(outfile, "%s", suffix);
    }
    fprintf(outfile,"%s\n", str);
  }
}
/***************************************************************************/
