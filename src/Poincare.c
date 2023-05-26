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

void  genGeneralRK4Code();

#define POINCARE_C
#include "Header.h"

static char *poincare_doc =
"\
/* Generate sample/simple code to compute poincare sections.\n\
 * \n\
 *  Parameters:\n\
 *  MY_FLOAT *xx,     initial value of state variables.\n\
 *  int n_steps_in    control parameter, will perform at most this many steps. \n\
 *                    To disable it, use -1  \n\
 *  int n_instersections_in  control, find at most this many intersections\n\
 *  int step_ctl_in   step_ctl.   See stepper for details \n\
 *  double startT_in  starting time. \n\
 *  double stopT_in   control, stop when time reaches this value\n\
 *  double pEpsilon_in  control variable, when _poincare_section_f(xx) is about pEpsilon close in magnitude to \n\
 *                     the section, it is regarded as on the section plane. Should be significantly\n\
 *                     larger than the error tolerances. Please also note RK4 is a 4th order algorithm, \n\
 *                     it will miss some intersections.\n\
 *  double tolerance_in, rtolerance_in  control used for step size control, see stepper\n\
 *                      documentation for details.  \n\
 *\n\
 *   _poincare_section_f, expression to evaluate the expression of poincare the section. The poincare\n\
 *                        section is defined by    expression=0  \n\
 *   _poincare_section_devirative_f, expression to evaluate the time derivative of the poincare section expression.\n\
 *   int crossing  control which crossing to record, values are: 1 -- from - to +, -1 -- from + to -, 0 -- both \n\
 *   int method:   bit 0 specifies method to compute intersection point. 0: use RungeKutta 4 to intetrate\n\
 *                             backward. 1 use Newton descend, equivalent to Taylor method of degree 1, to integrate backward.\n\
 *                 bit 1 select RK4 (fixed step size) as the main integrator\n\
 *                 bit 2 select RK4QC adaptive RK4 as the main integrator\n\
 */\n\
";

void genPoincareSectionCode() {
  int i,j;

  void genRK4Code();

  fprintf(outfile,"\n/* functions to evaluate poincare section and its derivative for %s */\n", suffix);  
  fprintf(outfile,"static MY_FLOAT *(*%s_ps_section_function)(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *xOut, MY_JET *sIn, MY_JET ***sOut); \n", suffix);
  fprintf(outfile,"static MY_FLOAT *(*%s_ps_section_function_derivative)(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *xOut, MY_JET *sIn, MY_JET ***sOut); \n\n", suffix);  
  
  genRK4Code();
  
  fprintf(outfile,"%s\n",poincare_doc);  
  
  fprintf(outfile,"void poincare_section_%s(MY_FLOAT *xx_in, \n",suffix);
  fprintf(outfile,"\t\t int nsteps_in, int nintersections_in, int step_ctl_in,\n");
  fprintf(outfile,"\t\t double startT_in, double stopT_in, double step_size_in,\n");
  fprintf(outfile,"\t\t double pEpsilon_in, double tolerance_in, double rtolerance_in, \n");
  fprintf(outfile,"\t\t MY_FLOAT *(*_poincare_section_f)(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *v, MY_JET *sIn, MY_JET ***sOut), \n");
  fprintf(outfile,"\t\t MY_FLOAT *(*_poincare_section_derivative_f)(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *v, MY_JET *sIn, MY_JET ***sOut), \n");
  fprintf(outfile,"\t\t int crossing, int method, char *output_file, MY_JET *sIn \n");    
  fprintf(outfile,") {\n");

  fprintf(outfile,"\t int       i, j, order=20, order_save, itmp=0, direction = 1, nsteps = -1, integrator=0;\n");
  fprintf(outfile,"\t double    ftmp, dstep, zstep, tolerance, rtolerance, log10tolerance, log10rtolerance;\n");
  fprintf(outfile,"\t MY_FLOAT  startT, stopT, nextT, startT_save, nextT_save, xstep;\n");
  fprintf(outfile,"\t MY_FLOAT  myFloatZero;\n");

  fprintf(outfile,"\t int       stop_nsteps, stop_nintersections, sign=0, last_sign=0, last_sign2=0, nsections = 0, step_ctl=0, saved=0;\n");  
  fprintf(outfile,"\t MY_FLOAT  psection_current[1], derivative_of_p[1], backward_step_size[1], dsmall,pEpsilon;\n");
  fprintf(outfile,"\t MY_FLOAT  ptmp1, ptmp2, ptmp3, pstep;\n");
  fprintf(outfile,"\t MY_FLOAT  **derivatives;\n");

  fprintf(outfile,"\t MY_JET    **sOut;\n");
  fprintf(outfile,"\t MY_JET    jetIn[%d], jetSave[%d], jetOut[%d];\n",   1+state_jet_vars,1+state_jet_vars,1+state_jet_vars);
  
  fprintf(outfile,"\t char  format_string[32];\n");  
  fprintf(outfile,"\t FILE  *fp_out = NULL;\n");
  
  j = neqns + 1;
  fprintf(outfile,"\t MY_FLOAT  xx[%d], yy[%d], zz[%d];\n", j,j,j);


  fprintf(outfile,"\n\t if(_poincare_section_f == NULL || _poincare_section_derivative_f == NULL) {\n");
  fprintf(outfile,"\t   fprintf(stderr, \"Poincare section evaluator or its derivative function is NULL.\\n\");\n");
  fprintf(outfile,"\t   exit(1);\n");
  fprintf(outfile,"\t  }\n\n");

  fprintf(outfile, "\t if(method&2) integrator=1; else if(method&4) integrator=2;\n\n");
  
  fprintf(outfile,"\t  %s_ps_section_function = _poincare_section_f;\n", suffix );
  fprintf(outfile,"\t  %s_ps_section_function_derivative = _poincare_section_derivative_f;\n\n",suffix);

  fprintf(outfile,"\t if(output_file != NULL && strlen(output_file)>0) {;\n");
  fprintf(outfile,"\t   if((fp_out = fopen(output_file,\"w\")) == NULL) {;\n");
  fprintf(outfile,"\t      fprintf(stderr,\"Unable to open '%%s'\", output_file); exit(2);\n");
  fprintf(outfile,"\t  }\n");
  fprintf(outfile,"\t }\n");  
  fprintf(outfile,"\n\t if(fp_out == NULL) fp_out=stdout;\n\n");  

  if(my_float_arith==ARITH_GMP){    
    if( !gmp_precision ) gmp_precision = 64;
    fprintf(outfile, "\t if(mpf_get_default_prec() != %d) ", gmp_precision);
    fprintf(outfile, "\t   mpf_set_default_prec(%d);\n\n", gmp_precision);
  } else if( my_float_arith==ARITH_MPFR) {    
    if( !mpfr_precision ) mpfr_precision = 64;
    fprintf(outfile, "\t if(mpfr_get_default_prec() != %d)\n", mpfr_precision);    
    fprintf(outfile, "\t   mpfr_set_default_prec(%d);\n\n", mpfr_precision);
  }
  
  fprintf(outfile, "\t InitMyFloat(myFloatZero);\n");
  fprintf(outfile, "\t InitMyFloat(startT); InitMyFloat(startT_save);\n");
  fprintf(outfile, "\t InitMyFloat(stopT); InitMyFloat(xstep);\n");
  fprintf(outfile, "\t InitMyFloat(nextT);InitMyFloat(nextT_save);\n");

  fprintf(outfile, "\t InitMyFloat(psection_current[0]);\n");
  fprintf(outfile, "\t InitMyFloat(derivative_of_p[0]);\n");
  fprintf(outfile, "\t InitMyFloat(backward_step_size[0]);\n");
  fprintf(outfile, "\t InitMyFloat(dsmall); InitMyFloat(ptmp1); InitMyFloat(ptmp2);InitMyFloat(ptmp3); InitMyFloat(pstep); InitMyFloat(pEpsilon); \n");  

  fprintf(outfile, "\t for(i=0; i<%d; i++){InitMyFloat(xx[i]); InitMyFloat(yy[i]);InitMyFloat(zz[i]);}\n",j);
  fprintf(outfile,"\n\t MakeMyFloatA(myFloatZero, 0);\n");

  fprintf(outfile,"\n\t /* initialize state vars --start */\n");  
  for(i = 0; i < neqns-nonautonomous; i++) {
    fprintf(outfile,"\t AssignMyFloat(xx[%d], xx_in[%d]);\n", i,i);
  }
  fprintf(outfile,"\t /* initialize state vars --end */\n");

  for(i = 0; i < neqns-nonautonomous; i++) {
    fprintf(outfile,"\t AssignMyFloat(yy[%d], xx[%d]);\n", i,i);
  }

  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\t InitUpJet2(%d, %d);\n", num_jet_symbols, deg_jet_vars);
    fprintf(outfile, "\t for(i=0; i<%d; i++) {InitJet(jetIn[i]);InitJet(jetSave[i]);InitJet(jetOut[i]);}\n",state_jet_vars+1);
    fprintf(outfile, "\t if(sIn) {\n\t\t for(i=0; i<%d; i++) {AssignJetToJet(jetIn[i], sIn[i]); AssignJetToJet(jetSave[i], sIn[i]);AssignJetToJet(jetOut[i], sIn[i]);}\n",state_jet_vars+1);
    fprintf(outfile, "\t } else  {\n\t\t for(i=0; i<%d; i++) {AssignFloatToJet(jetIn[i], xx[i]); AssignFloatToJet(jetSave[i], xx[i]);AssignFloatToJet(jetSave[i], xx[i]);}\n",state_jet_vars+1);
    fprintf(outfile, "\t }\n");        
  }
  
  
  fprintf(outfile,"\n\t /* initialize control parameters --start */\n");
  fprintf(outfile,"\n\t  step_ctl = step_ctl_in;\n");
  fprintf(outfile,"\n\t  stop_nsteps = nsteps_in;\n");
  fprintf(outfile,"\n\t  stop_nintersections = nintersections_in;\n");
  fprintf(outfile,"\n\t  zstep = pow(pEpsilon_in, 0.25); if(zstep < 0.05) zstep=0.05;\n");
  fprintf(outfile,"\n\t  MakeMyFloatA(startT, startT_in);\n");
  fprintf(outfile,"\n\t  MakeMyFloatA(stopT, stopT_in);\n");

  fprintf(outfile,"\t MakeMyFloatC(dsmall, \"1.0E-4\", 1.0E-4);\n");  
  //fprintf(outfile, "\t dstep=0.001; /* only nedeed when step_ctrl_method==0 (see manual) */\n");
  //fprintf(outfile,"\t MakeMyFloatC(nextT, \"0.001\", (double)dstep);\n");

  fprintf(outfile, "\t dstep=step_size_in; /* only nedeed when step_ctrl_method==0 (see manual) */\n");
  fprintf(outfile, "\t MakeMyFloatA(nextT, dstep);\n");
  
  fprintf(outfile,"\t MakeMyFloatA(pEpsilon, (double)(10*pEpsilon_in));\n");  
  fprintf(outfile, "\t log10tolerance  = log10(tolerance_in);\n");
  fprintf(outfile, "\t log10rtolerance = log10(rtolerance_in);\n");
  
  fprintf(outfile,"\n\t /* initialize control parameters --end */\n");    

  if(my_float_arith==ARITH_GMP){
    fprintf(outfile, "\t int prec = (int) mpf_get_prec(myFloatZero);\n");
    fprintf(outfile, "\t int digits = (8 * prec / 32 );\n");
    fprintf(outfile, "\t sprintf(format_string, \"%%%%.%%dFg \",digits);\n");    
  } else if( my_float_arith==ARITH_MPFR) {
    fprintf(outfile, "\t int prec = (int) mpfr_get_prec(myFloatZero);\n");
    fprintf(outfile, "\t int digits = (8 * prec / 32 );\n");
    fprintf(outfile, "\t sprintf(format_string, \"%%%%.%%dRg \",digits);\n");
  } else if(my_float_arith==ARITH_FLOAT128) {
    fprintf(outfile, "\t (void)strcpy(format_string, \"%%.33Qe\");\n");    
  } else {
    fprintf(outfile, "\t (void)strcpy(format_string, \"%%.16g \");\n");    
  }  


  fprintf(outfile, "\t (void) _poincare_section_f(startT, xx, psection_current, sIn, &sOut);\n");
  fprintf(outfile, "\t fabsMyFloatA(ptmp1,psection_current[0]);\n");
  fprintf(outfile, "\t if(MyFloatA_LE_B(ptmp1, pEpsilon)) {\n");
  fprintf(outfile, "\t\t last_sign = 0;\n");
  fprintf(outfile, "\t } else if(MyFloatA_GT_B(psection_current[0], myFloatZero)) { \n");
  fprintf(outfile, "\t\t last_sign = 1;\n");
  fprintf(outfile, "\t } else { \n");    
  fprintf(outfile, "\t\t last_sign = -1;\n");
  fprintf(outfile, "\t }\n");
  
  
  fprintf(outfile, "\t while(1)  {\n");
  fprintf(outfile, "\t\t if(last_sign == 0) {\n");
  for(i=0; i < neqns-nonautonomous; i++) 
    { fprintf(outfile, "\t\t   OutputMyFloat3(fp_out, format_string, yy[%d]);\n", i); }
  fprintf(outfile, "\t\t   OutputMyFloat3(fp_out, format_string, startT);\n");

  if(state_jet_vars>0) {
    for(i=0; i < state_jet_vars; i++) {
      fprintf(outfile, "\t\t   OutputJet2File(fp_out, format_string, jetOut[%d]);\n",i);
      fprintf(outfile, "\t\t   fprintf(fp_out, \"  \");\n");
    }
  }

  fprintf(outfile, "\t\t   _poincare_section_f(startT, yy, psection_current, jetIn, &sOut);\n");		     
  fprintf(outfile, "\t\t   OutputMyFloat3(fp_out, format_string, psection_current[0]);\n");      
  fprintf(outfile, "\t\t   fprintf(fp_out, \"\\n\");\n");      
  fprintf(outfile, "\t\t   nsections++;\n");
  fprintf(outfile, "\t\t   if(nsections >= stop_nintersections) break;\n");
  fprintf(outfile, "\t\t   if(saved) {\n");
  fprintf(outfile, "\t\t      order=order_save;\n");
  fprintf(outfile, "\t\t      AssignMyFloat(startT,startT_save);\n");
  fprintf(outfile, "\t\t      AssignMyFloat(nextT,nextT_save);\n");
  fprintf(outfile, "\t\t      for(i=0; i< %d; i++){\n",neqns-nonautonomous);  
  fprintf(outfile, "\t\t         AssignMyFloat(xx[i],zz[i]);\n"); 
  fprintf(outfile, "\t\t      }\n");

  if(state_jet_vars>0) {
      fprintf(outfile, "\t\t      for(i=0; i< %d; i++){\n",state_jet_vars);  
      fprintf(outfile, "\t\t         AssignJetToJet(jetIn[i], jetSave[i]);\n");
      fprintf(outfile, "\t\t      }\n");
  }

  fprintf(outfile, "\t\t   }\n");
  fprintf(outfile, "\t\t   saved=0;\n");  
  fprintf(outfile, "\t\t }\n");
  fprintf(outfile, "\t\t  last_sign2 = last_sign;\n");      
  fprintf(outfile, "\t\t  last_sign = sign;\n");    
  fprintf(outfile, "\t\t if(itmp != 0) break;\n");
  fprintf(outfile, "\t\t if(MyFloatA_GE_B(startT,stopT)) { break;}\n");
  fprintf(outfile, "\t\t if(--stop_nsteps == -1) { break;}\n\n");
  fprintf(outfile, "\t\t if(integrator==0)\n");
  fprintf(outfile, "\t\t    itmp = taylor_step_%s( &startT, xx, direction, step_ctl, log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", suffix);
  fprintf(outfile, "\t\t else if(integrator==1)\n");
  fprintf(outfile, "\t\t    itmp = RK4_step_%s( &startT, xx, direction, step_ctl, log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", suffix);
  fprintf(outfile,"\t\t else\n");
  fprintf(outfile, "\t\t    itmp = RK4QC_step_%s( &startT, xx, direction, step_ctl, 6+log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", suffix);  
  fprintf(outfile, "\t\t if(itmp == 0) {\n");
  fprintf(outfile, "\t\t   _poincare_section_f(startT, xx, psection_current, jetIn, &sOut);\n");
  fprintf(outfile, "\t\t   fabsMyFloatA(ptmp1,psection_current[0]);\n");
  fprintf(outfile, "\t\t   if(MyFloatA_LE_B(ptmp1, pEpsilon)) {\n");

  fprintf(outfile, "\t\t     if(crossing == 0 || last_sign2 == -crossing) {\n");  
  fprintf(outfile, "\t\t\t     last_sign = 0;\n");
  fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",neqns-nonautonomous);  
  fprintf(outfile, "\t\t\t         AssignMyFloat(yy[i], xx[i]);\n");
  fprintf(outfile, "\t\t\t      }\n");

  if(state_jet_vars>0) {
    fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",state_jet_vars);  
    fprintf(outfile, "\t\t\t         AssignJetToJet(jetOut[i], jetIn[i]);\n");
    fprintf(outfile, "\t\t\t      }\n");
  }
  
  fprintf(outfile, "\t\t      }\n");  

  fprintf(outfile, "\t\t   } else { \n");
  fprintf(outfile, "\t\t      if(MyFloatA_LE_B(psection_current[0], myFloatZero)) {\n");
  fprintf(outfile, "\t\t\t      sign = -1;\n");
  fprintf(outfile, "\t\t      } else { \n");    
  fprintf(outfile, "\t\t\t      sign = 1;\n");
  fprintf(outfile, "\t\t      }\n");

  fprintf(outfile, "\t\t      if( (crossing == 0 && (sign  != last_sign)) || (last_sign == -crossing && sign == crossing )) {\n");
  fprintf(outfile, "\t\t\t int found = 0, count=0;\n")  ;

  fprintf(outfile, "\t\t\t saved = 1;\n")  ;
  fprintf(outfile, "\t\t\t AssignMyFloat(startT_save,startT);\n");
  fprintf(outfile, "\t\t\t AssignMyFloat(nextT_save,nextT);\n");
  fprintf(outfile, "\t\t\t order_save = order;\n");  

  fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",neqns-nonautonomous);
  fprintf(outfile, "\t\t\t        AssignMyFloat(zz[i],xx[i]);\n");
  fprintf(outfile, "\t\t\t      }\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",state_jet_vars);  
    fprintf(outfile, "\t\t\t         AssignJetToJet(jetSave[i], jetIn[i]);\n");
    fprintf(outfile, "\t\t\t      }\n");
  }

  fprintf(outfile, "\t\t   if((method & 1) == 0) { //-----RK4 backwards to find T when intersection occured \n");
  fprintf(outfile, "\t\t\t _poincare_section_derivative_f(startT, xx, derivative_of_p, jetIn, &sOut);\n");
  fprintf(outfile, "\t\t\t fabsMyFloatA(ptmp1,derivative_of_p[0]);\n");
  fprintf(outfile, "\t\t\t if(MyFloatA_GE_B(ptmp1, dsmall)) {\n");  
  fprintf(outfile, "\t\t\t   int k, n; double dtmp;\n");
  fprintf(outfile, "\t\t\t   MyFloatToDouble(dtmp,psection_current[0]);\n");  
  fprintf(outfile, "\t\t\t   n = 1+(int)fabs(dtmp/zstep);\n");
  fprintf(outfile, "\t\t\t   MakeMyFloatA(ptmp1, (double)n);\n");
  fprintf(outfile, "\t\t\t   DivideMyFloatA(ptmp2,psection_current[0], ptmp1);\n");
  fprintf(outfile, "\t\t\t   NegateMyFloatA(xstep,ptmp2);\n");  
  if(1 || nonautonomous ) {
    fprintf(outfile, "\t\t\t   /*  switch independent to P,  need to solve t as well, crucial for nonautonomous systems */;\n");      
    fprintf(outfile, "\t\t\t   AssignMyFloat(xx[%d],startT);\n", neqns-nonautonomous);  
  }
  fprintf(outfile, "\t\t\t   for(k=0;k<n;k++) {\n");    
  fprintf(outfile, "\t\t\t      RK4_%s_poincare(startT,xstep,xx,NULL,0);\n",suffix);
  if(1 || nonautonomous) {
    fprintf(outfile, "\t\t\t     /* get the new t from RK4, crucial for nonautonomous systems */;\n");          
    fprintf(outfile, "\t\t\t      AssignMyFloat(startT,xx[%d]);\n", neqns-nonautonomous);
  } else  {
    fprintf(outfile, "\t\t\t      AddMyFloatA(ptmp2, startT,  xstep);  \n");
    fprintf(outfile, "\t\t\t      AssignMyFloat(startT,ptmp2);  \n");
  }
  fprintf(outfile, "\t\t\t  }\n");
  fprintf(outfile, "\t\t\t   _poincare_section_f(startT, xx, psection_current,jetIn,&sOut);\n");
  fprintf(outfile, "\t\t\t   fabsMyFloatA(ptmp1,psection_current[0]);\n");    
  fprintf(outfile, "\t\t\t   if(MyFloatA_LE_B(ptmp1, pEpsilon)) {\n");
  fprintf(outfile, "\t\t\t     found = 1;\n");

  fprintf(outfile, "\t\t\t     /* do a one step taylor backward with time step startT - startT_save */\n");
  fprintf(outfile, "\t\t\t     SubtractMyFloatA(nextT, startT,startT_save);\n");
  fprintf(outfile, "\t\t\t     for(i = 0; i < %d; i++) {\n",neqns-nonautonomous);
  fprintf(outfile, "\t\t\t             AssignMyFloat(xx[i],zz[i]);\n");
  fprintf(outfile, "\t\t\t       }\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",state_jet_vars);  
    fprintf(outfile, "\t\t\t         AssignJetToJet(jetIn[i], jetSave[i]);\n");
    fprintf(outfile, "\t\t\t      }\n");
  }
  fprintf(outfile, "\t\t\t     AssignMyFloat(startT,startT_save);  \n");
  fprintf(outfile, "\t\t\t     order = 6; /* no step size control */\n");  
  fprintf(outfile, "\t\t\t     itmp = taylor_step_%s( &startT, xx, direction, 0, log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);\n", suffix);
  
  fprintf(outfile, "\t\t\t     for(i = 0; i < %d; i++) {\n",neqns-nonautonomous);
  fprintf(outfile, "\t\t\t             AssignMyFloat(yy[i],xx[i]);\n");
  fprintf(outfile, "\t\t\t       }\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",state_jet_vars);  
    fprintf(outfile, "\t\t\t         AssignJetToJet(jetOut[i], jetIn[i]);\n");
    fprintf(outfile, "\t\t\t      }\n");
  }

  fprintf(outfile, "\t\t\t  } else { \n");
  fprintf(outfile, "\t\t\t    fprintf(stderr,\"RK4 failed to find intersection.\"); OutputMyFloat3(stderr, format_string, ptmp1);fprintf(stderr,\"\\n\");\n");
  fprintf(outfile, "\t\t\t    found=-1;\n");
  fprintf(outfile, "\t\t\t  }\n");
  fprintf(outfile, "\t\t\t } else {\n");
  fprintf(outfile, "\t\t\t    fprintf(stderr, \"RK4 failed to find intersection.\"); OutputMyFloat3(stderr, format_string, derivative_of_p[0]);fprintf(stderr,\"\\n\");\n");
  fprintf(outfile, "\t\t\t    found=-1;\n");
  fprintf(outfile, "\t\t\t }\n");  
  
  fprintf(outfile, "\t\t  } else {//================ Taylor backwards\n");            
  fprintf(outfile, "\t\t\t do {\n");
  fprintf(outfile, "\t\t\t   _poincare_section_derivative_f(startT, xx, derivative_of_p,jetIn,&sOut);\n");
  fprintf(outfile, "\t\t\t   fabsMyFloatA(ptmp2,derivative_of_p[0]);\n");
  fprintf(outfile, "\t\t\t   if(MyFloatA_GE_B(ptmp2, dsmall) && ++count < 6) {\n");
  fprintf(outfile, "\t\t\t      int k, n; double dtmp;\n");
  fprintf(outfile, "\t\t\t      DivideMyFloatA(ptmp3, psection_current[0],derivative_of_p[0]);\n");
  fprintf(outfile, "\t\t\t      MyFloatToDouble(dtmp,ptmp3);\n");
  fprintf(outfile, "\t\t\t      n = 1+(int)fabs(dtmp/zstep);\n");
  fprintf(outfile, "\t\t\t      MakeMyFloatA(ptmp1, (double)n);\n");
  fprintf(outfile, "\t\t\t      DivideMyFloatA(ptmp2, ptmp3, ptmp1);\n");
  fprintf(outfile, "\t\t\t      NegateMyFloatA(backward_step_size[0], ptmp2);\n");
  fprintf(outfile, "\t\t\t      order=6; /* no step control! */\n");    
  fprintf(outfile, "\t\t\t      for(k=0;k<n;k++) {\n");    
  fprintf(outfile, "\t\t\t         itmp = taylor_step_%s( &startT, xx, -direction, 0, log10tolerance, log10rtolerance, &stopT, backward_step_size, &order, jetIn);\n", suffix);
  fprintf(outfile, "\t\t\t      }\n");        
  fprintf(outfile, "\t\t\t   } else {\n");
  fprintf(outfile, "\t\t\t      fprintf(stderr, \"Taylor failed to find intersection.\"); OutputMyFloat3(stderr, format_string, derivative_of_p[0]);fprintf(stderr,\"\\n\");\n");
  fprintf(outfile, "\t\t\t      found=-1;\n");
  fprintf(outfile, "\t\t\t   }\n");  
  fprintf(outfile, "\t\t\t   _poincare_section_f(startT, xx, psection_current,jetIn, &sOut);\n");
  fprintf(outfile, "\t\t\t   fabsMyFloatA(ptmp1,psection_current[0]);\n");
  fprintf(outfile, "\t\t\t   if(MyFloatA_LE_B(ptmp1, pEpsilon)) {\n");
  fprintf(outfile, "\t\t\t     found = 1;\n");
  fprintf(outfile, "\t\t\t     for(i = 0; i < %d; i++) {\n",neqns-nonautonomous);
  fprintf(outfile, "\t\t\t             AssignMyFloat(yy[i],xx[i]);\n");
  fprintf(outfile, "\t\t\t       }\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "\t\t\t     for(i=0; i< %d; i++){\n",state_jet_vars);  
    fprintf(outfile, "\t\t\t         AssignJetToJet(jetOut[i], jetIn[i]);\n");
    fprintf(outfile, "\t\t\t      }\n");
  }  
  fprintf(outfile, "\t\t\t   }\n");  
  fprintf(outfile, "\t\t\t } while(!found && itmp == 0);\n");
  fprintf(outfile, "\t\t\t}//=======================\n");
  fprintf(outfile, "\t\t\t if(found>0) last_sign = 0;\n");
  fprintf(outfile, "\t\t\t if(itmp != 0) break;\n");    
  fprintf(outfile, "\t\t  }\n");
  fprintf(outfile, "\t\t} \n");        
  fprintf(outfile, "\t   }\n");      
  fprintf(outfile, "\t }  /* while */ \n");
  fprintf(outfile, "\t if(fp_out != stdout) fclose(fp_out); \n");          
  fprintf(outfile, "} \n");

  // a samile main
  if(num_expr_vars > 0) {
    fprintf(outfile, "\n/* list defined expressions */\n");
    fprintf(outfile, "typedef struct {\n\
  char name[128];\n\
  MY_FLOAT *(*pf)(MY_FLOAT,MY_FLOAT *,MY_FLOAT *, MY_JET *sIn, MY_JET ***sOut);\n \
 MY_FLOAT *(*pfd)(MY_FLOAT,MY_FLOAT *,MY_FLOAT *, MY_JET *sIn, MY_JET ***sOut);\n \
} Expression_record;\n\
\n");

    fprintf(outfile, "static Expression_record defined_expressions[%d] = {\n", num_expr_vars);
    {
      Expression expr = expression_list;
      while(expr) {    
        Node name = EXPRESSION_NAME(expr);
        fprintf(outfile,"\t { \"%s\", %s, %s_derivative }, \n",NODE_NAME(name),NODE_NAME(name),NODE_NAME(name));
        expr =  EXPRESSION_NEXT(expr);
      }
   }
  fprintf(outfile, "};\n");
  }


  fprintf(outfile,"\n\n/* A sample poincare section main */\n");
  fprintf(outfile,"void poincare_section_%s_main(char *pfname){\n",suffix);
  fprintf(outfile,"\t MY_FLOAT xx[%d];\n", neqns+1);  
  fprintf(outfile,"\t int max_nsteps    = 1234567;\n");
  fprintf(outfile,"\t int max_points    = 1000;\n");
  fprintf(outfile,"\t int step_ctl      = 1;\n");
  fprintf(outfile,"\t double start_t    = 0.0;\n");
  fprintf(outfile,"\t double stop_t     = 1.0e6;\n");
  fprintf(outfile,"\t double step_t     = 0.001;\n");  
  fprintf(outfile,"\t double epsilon    = 1.0e-5;\n");
  fprintf(outfile,"\t double abs_err    = 1.0e-16;\n");
  fprintf(outfile,"\t double rel_err    = 1.0e-16;\n");
  fprintf(outfile,"\t int crossing      = 0;\n");
  fprintf(outfile,"\t int method        = 1;\n\n");
  fprintf(outfile,"\t char *out_file    = NULL;\n\n");

  fprintf(outfile,"\t MY_JET    *jetOut;\n");
  fprintf(outfile,"\t MY_JET    jetIn[%d];\n",   1+state_jet_vars);

  
  fprintf(outfile,"\t MY_FLOAT *(*pf)(MY_FLOAT,MY_FLOAT *,MY_FLOAT *, MY_JET *sIn, MY_JET ***sOut);\n");
  fprintf(outfile,"\t MY_FLOAT *(*pfd)(MY_FLOAT,MY_FLOAT *,MY_FLOAT *, MY_JET *sIn, MY_JET ***sOut);\n\n");    

  fprintf(outfile,"\t int i;\n\n");        
  fprintf(outfile,"\t pf  = NULL; pfd= NULL;\n\n");

  // guess poincare section names
  {
    Expression expr = expression_list;
    while(expr) {    
      Node name = EXPRESSION_NAME(expr);
      if(!strcasecmp(NODE_NAME(name), "psecf") || !strcasecmp(NODE_NAME(name), "poincare_section")) {
	fprintf(outfile,"\t pf  = %s; \n",NODE_NAME(name));
	fprintf(outfile,"\t pfd = %s_derivative; \n\n",NODE_NAME(name));
	break;
      }
      expr =  EXPRESSION_NEXT(expr);      
    }
  }
  
  // set poincare section function if pfname is given
  if(num_expr_vars > 0) {
    fprintf(outfile,"\t for(i=0;i<%d;i++){\n",   num_expr_vars);
    fprintf(outfile,"\t\t  if(pfname && !strcmp(pfname, defined_expressions[i].name)) {\n");
    fprintf(outfile,"\t\t      pf  = defined_expressions[i].pf;\n");
    fprintf(outfile,"\t\t      pfd = defined_expressions[i].pfd;\n");
    fprintf(outfile,"\t\t      break;\n");
    fprintf(outfile,"\t\t   }\n");
    fprintf(outfile,"\t }\n\n");                
  }    

  fprintf(outfile,"\t if(pf == NULL) {fprintf(stderr, \"Poincare Section expression is undefined.\"); exit(1);}\n\n");
  
  if(state_jet_vars > 0) {
    fprintf(outfile, "\t InitUpJet2(%d, %d);\n", num_jet_symbols, deg_jet_vars);
    fprintf(outfile, "\t for(i=0; i<%d; i++) {InitJet(jetIn[i]);}\n",state_jet_vars+param_jet_vars);
  }

  if(state_jet_vars>0) {
    Node var; char *p;


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
  
  
  fprintf(outfile,"\t// pf  = psecf; \n");
  fprintf(outfile,"\t// pfd = psecf_derivative; \n\n");          
  // guess poincare section names
  {
    Expression expr = expression_list;
    while(expr) {    
      Node name = EXPRESSION_NAME(expr);
      if(!strcasecmp(NODE_NAME(name), "psecf") || !strcasecmp(NODE_NAME(name), "poincare_section")) {
	fprintf(outfile,"\t pf  = %s; \n",NODE_NAME(name));
	fprintf(outfile,"\t pfd = %s_derivative; \n\n",NODE_NAME(name));
	break;
      }
      expr =  EXPRESSION_NEXT(expr);      
    }
  }
  
  if(my_float_arith==ARITH_GMP) {
    fprintf(outfile, "\t mpf_set_default_prec(%d);\n\n", gmp_precision);
  } else if( my_float_arith==ARITH_MPFR)  {
    fprintf(outfile, "\t mpfr_set_default_prec(%d);\n\n", mpfr_precision);
  }
  fprintf(outfile,"\t for(i=0;i<%d;i++) {;\n",neqns+1);
  fprintf(outfile,"\t\t InitMyFloat(xx[i]);\n");  
  fprintf(outfile,"\t}\n\n");

  fprintf(outfile, "\n\t /* assign initials */\n");
  if(controlParams.ninitials > 0)
    {
      int i, cnt; char sbuf[256];
      if(controlParams.ninitials > neqns - nonautonomous)
        cnt = neqns - nonautonomous;
      else cnt = controlParams.ninitials;
      for(i = 0; i < cnt; i++)
        {
          double dtmp = cstNodeValue(controlParams.initials[i]);
          fprintf(outfile,"\t MakeMyFloatC(xx[%d],\"%s\", (double)%.16g);\n", i, cstNodeStringValue(controlParams.initials[i],sbuf), dtmp);
        }
    }
  fprintf(outfile,"\n");

  Node pnode;
  double dtmp;
  char ibuf[256];

  if(getControlParameterValue("numberOfIntersections",&dtmp, &pnode) || getControlParameterValue("number_of_intersections",&dtmp, &pnode)||
     getControlParameterValue("NumberOfIntersections",&dtmp, &pnode) || getControlParameterValue("Number_Of_Intersections",&dtmp, &pnode))
    {
      fprintf(outfile,"\t max_points = %d;\n", (int)dtmp);
    }
  if(getControlParameterValue("crossingDirection",&dtmp, &pnode) || getControlParameterValue("crossing_direction",&dtmp, &pnode)||
     getControlParameterValue("CrossingDirection",&dtmp, &pnode) || getControlParameterValue("Crossing_Direction",&dtmp, &pnode))
    {
      fprintf(outfile,"\t crossing = %d;\n", (int)dtmp);
    }
  if(getControlParameterValue("crossingMethod",&dtmp, &pnode) || getControlParameterValue("crossing_method",&dtmp, &pnode)||
     getControlParameterValue("CrossingMethod",&dtmp, &pnode) || getControlParameterValue("Crossing_Method",&dtmp, &pnode))
    {
      fprintf(outfile,"\t method = %d;\n", (int)dtmp);
    }
  if(getControlParameterValue("crossingEpsilon",&dtmp, &pnode) || getControlParameterValue("crossing_epsilon",&dtmp, &pnode)||
     getControlParameterValue("CrossingEpsilon",&dtmp, &pnode) || getControlParameterValue("Crossing_Epsilon",&dtmp, &pnode))
    {
      fprintf(outfile,"\t epsilon = (double)%s;\n", cstNodeStringValue(pnode,ibuf)); 
    }
  
  if(getControlParameterValue("numberOfSteps",&dtmp, &pnode) || getControlParameterValue("number_of_steps",&dtmp, &pnode)||
     getControlParameterValue("NumberOfSteps",&dtmp, &pnode) || getControlParameterValue("Number_Of_Steps",&dtmp, &pnode))
    {
      fprintf(outfile,"\t max_nsteps = %d;\n", (int)dtmp);
    }
  if(getControlParameterValue("startTime",&dtmp, &pnode) || getControlParameterValue("start_time",&dtmp, &pnode)||
     getControlParameterValue("StartTime",&dtmp, &pnode) || getControlParameterValue("Start_Time",&dtmp, &pnode))
    {
      fprintf(outfile,"\t start_t =(double)%s;\n",cstNodeStringValue(pnode,ibuf));      
    }
  if(getControlParameterValue("stopTime",&dtmp, &pnode) || getControlParameterValue("stop_time",&dtmp, &pnode)||
     getControlParameterValue("StopTime",&dtmp, &pnode) || getControlParameterValue("Stop_Time",&dtmp, &pnode))
    {
      fprintf(outfile,"\t stop_t = (double)%s;\n", cstNodeStringValue(pnode,ibuf));      
    }  
  if(getControlParameterValue("minimalStep",&dtmp, &pnode) || getControlParameterValue("minimal_step",&dtmp, &pnode)||
     getControlParameterValue("MinimalStep",&dtmp, &pnode) || getControlParameterValue("Minimal_Step",&dtmp, &pnode)||
     getControlParameterValue("maximalStep",&dtmp, &pnode) || getControlParameterValue("maximal_step",&dtmp, &pnode)||
     getControlParameterValue("MaximalStep",&dtmp, &pnode) || getControlParameterValue("Maximal_Step",&dtmp, &pnode)||
     getControlParameterValue("step",&dtmp, &pnode) || getControlParameterValue("Step",&dtmp, &pnode)||
     getControlParameterValue("StepSize",&dtmp, &pnode) || getControlParameterValue("step_size",&dtmp, &pnode)||
     getControlParameterValue("stepSize",&dtmp, &pnode) || getControlParameterValue("Step_Size",&dtmp, &pnode))
    {
      fprintf(outfile,"\t step_t = (double)%.16g;\n", dtmp);      
    }
  if(getControlParameterValue("absoluteErrorTolerance",&dtmp, &pnode) || getControlParameterValue("absolute_error_tolerance",&dtmp, &pnode)||
     getControlParameterValue("AbsoluteErrorTolerance",&dtmp, &pnode) || getControlParameterValue("Absolute_Error_Tolerance",&dtmp, &pnode))
    {
      fprintf(outfile,"\t abs_err = (double)(%.16g);\n",dtmp);      
    }
  if(getControlParameterValue("relativeErrorTolerance",&dtmp, &pnode) || getControlParameterValue("relative_error_tolerance",&dtmp, &pnode)||
     getControlParameterValue("RelativeErrorTolerance",&dtmp, &pnode) || getControlParameterValue("Relative_Error_Tolerance",&dtmp, &pnode))
    {
      fprintf(outfile,"\t rel_err = (double)(%.16g);\n",dtmp);      
    }

  fprintf(outfile,"\n\t poincare_section_%s(\n", suffix);
  fprintf(outfile,"\t\t xx,          /* initial values                         */\n");
  fprintf(outfile,"\t\t max_nsteps,  /* limit, max number of steps             */  \n");
  fprintf(outfile,"\t\t max_points,  /* limit, max number of intersections     */\n");
  fprintf(outfile,"\t\t step_ctl,    /* step size control method               */\n");
  fprintf(outfile,"\t\t start_t,     /* start time                             */ \n");
  fprintf(outfile,"\t\t stop_t,      /* limit, end time                        */\n");
  fprintf(outfile,"\t\t step_t,     /* initial step size                       */\n");  
  fprintf(outfile,"\t\t epsilon,     /* intersection epsilon                   */\n");  
  fprintf(outfile,"\t\t abs_err,     /* absolute error tolerance               */\n");
  fprintf(outfile,"\t\t rel_err,     /* relative error tolerance               */\n");
  fprintf(outfile,"\t\t pf,          /* pf=0 is the eq of poincare section     */\n");
  fprintf(outfile,"\t\t pfd,         /* time derivative of pf                  */\n");
  fprintf(outfile,"\t\t crossing,    /* crossing: -1,0,1. -1 means from - to + */\n");
  fprintf(outfile,"\t\t method,      /* method to comp intersection at xing.   */\n");
  fprintf(outfile,"\t\t              /* 0 or 1: RK4 or Taylor integrate back   */\n");  
  fprintf(outfile,"\t\t out_file,    /* file to save output data               */\n");
  fprintf(outfile,"\t\t jetIn        /*       */\n");
  fprintf(outfile,"\t\t );\n\n");        

  fprintf(outfile,"}\n\n");  

  fprintf(outfile,"//int main(int ac, char **av){ poincare_section_%s_main();}\n\n",suffix);  


  fprintf(outfile,"/* A wrapper for Python */\n");  
  fprintf(outfile,"void poincare_section_%s_python(\n",suffix);
  fprintf(outfile,"	  char *pfname,\n");  
  fprintf(outfile,"	  double *x_in,\n");  
  fprintf(outfile,"	  int nsteps,\n");
  fprintf(outfile,"	  int nintersections,\n");
  fprintf(outfile,"	  int crossing_in,\n");
  fprintf(outfile,"	  int method_in,\n");  
  fprintf(outfile,"	  int step_ctl_in,\n");
  fprintf(outfile,"	  int abs_error_log10,\n");
  fprintf(outfile,"	  int rel_error_log10,\n");
  fprintf(outfile,"	  double start_time,\n");
  fprintf(outfile,"	  double end_time,\n");
  fprintf(outfile,"	  double step_size_in,\n");  
  fprintf(outfile,"	  double epsilon_in,\n");
  fprintf(outfile,"	  char *outfile,\n");
  fprintf(outfile,"	  MY_JET *jetIn\n");  
  fprintf(outfile,") {\n\n");  
  fprintf(outfile,"\t MY_FLOAT xx[%d];\n", neqns+1);  
  fprintf(outfile,"\t int max_nsteps    = nsteps;\n");
  fprintf(outfile,"\t int max_points    = nintersections;\n");
  fprintf(outfile,"\t int crossing      = crossing_in;\n");
  fprintf(outfile,"\t int method        = method_in;\n\n");    
  fprintf(outfile,"\t int step_ctl      = step_ctl_in;\n");
  fprintf(outfile,"\t double epsilon    = epsilon_in;\n");
  fprintf(outfile,"\t double start_t    = start_time;\n");
  fprintf(outfile,"\t double stop_t     = end_time;\n");
  fprintf(outfile,"\t double step_t     = step_size_in;\n");  
  fprintf(outfile,"\t double abs_err    = pow(10.0,abs_error_log10);\n");
  fprintf(outfile,"\t double rel_err    = pow(10.0,rel_error_log10);\n");
  fprintf(outfile,"\t char *out_file    = outfile;\n\n");
  
  fprintf(outfile,"\t MY_FLOAT *(*pf)(MY_FLOAT,MY_FLOAT *,MY_FLOAT *, MY_JET *sIn, MY_JET ***sOut);\n");
  fprintf(outfile,"\t MY_FLOAT *(*pfd)(MY_FLOAT,MY_FLOAT *,MY_FLOAT *, MY_JET *sIn, MY_JET ***sOut);\n\n");    

  fprintf(outfile,"\t int i;\n\n");        
  fprintf(outfile,"\t pf  = NULL; \n");
  fprintf(outfile,"\t pfd = NULL; \n\n");          

  // guess poincare section names
  {
    Expression expr = expression_list;
    while(expr) {    
      Node name = EXPRESSION_NAME(expr);
      if(!strcasecmp(NODE_NAME(name), "psecf") || !strcasecmp(NODE_NAME(name), "poincare_section")) {
	fprintf(outfile,"\t pf  = %s; \n",NODE_NAME(name));
	fprintf(outfile,"\t pfd = %s_derivative; \n\n",NODE_NAME(name));
	break;
      }
      expr =  EXPRESSION_NEXT(expr);      
    }
  }

  if(num_expr_vars > 0) {
    fprintf(outfile,"\t for(i=0;i<%d;i++){\n",   num_expr_vars);
    fprintf(outfile,"\t\t  if(pfname && !strcmp(pfname, defined_expressions[i].name)) {\n");
    fprintf(outfile,"\t\t      pf  = defined_expressions[i].pf;\n");
    fprintf(outfile,"\t\t      pfd = defined_expressions[i].pfd;\n");
    fprintf(outfile,"\t\t      break;\n");
    fprintf(outfile,"\t\t   }\n");
    fprintf(outfile,"\t }\n\n");                
  }    

  fprintf(outfile,"\t if(pf == NULL) {fprintf(stderr, \"Poincare Section expression is undefined.\"); exit(1);}\n\n");

  if(my_float_arith==ARITH_GMP) {
    fprintf(outfile, "\t mpf_set_default_prec(%d);\n\n", gmp_precision);
  } else if( my_float_arith==ARITH_MPFR)  {
    fprintf(outfile, "\t mpfr_set_default_prec(%d);\n\n", mpfr_precision);
  }
  
  fprintf(outfile,"\t for(i=0;i<%d;i++) {;\n",neqns+1);
  fprintf(outfile,"\t\t InitMyFloat(xx[i]);\n");  
  fprintf(outfile,"\t}\n\n");
  fprintf(outfile,"\t for(i=0;i<%d;i++) {;\n",neqns);
  fprintf(outfile,"\t\t MakeMyFloatA(xx[i], x_in[i]);\n");  
  fprintf(outfile,"\t}\n\n");

  
  fprintf(outfile,"\t poincare_section_%s(\n", suffix);
  fprintf(outfile,"\t\t xx,          /* initial values                         */\n");
  fprintf(outfile,"\t\t max_nsteps,  /* limit, max number of steps             */  \n");
  fprintf(outfile,"\t\t max_points,  /* limit, max number of intersections     */\n");
  fprintf(outfile,"\t\t step_ctl,    /* step size control method               */\n");
  fprintf(outfile,"\t\t start_t,     /* start time                             */ \n");
  fprintf(outfile,"\t\t stop_t,      /* limit, end time                        */\n");
  fprintf(outfile,"\t\t step_t,      /* initial step size                      */\n");  
  fprintf(outfile,"\t\t epsilon,     /* intersection epsilon                   */\n");  
  fprintf(outfile,"\t\t abs_err,     /* absolute error tolerance               */\n");
  fprintf(outfile,"\t\t rel_err,     /* relative error tolerance               */\n");
  fprintf(outfile,"\t\t pf,          /* pf=0 is the eq of poincare section     */\n");
  fprintf(outfile,"\t\t pfd,         /* time derivative of pf                  */\n");
  fprintf(outfile,"\t\t crossing,    /* crossing: -1,0,1. -1 means from - to + */\n");
  fprintf(outfile,"\t\t method,      /* method to comp intersection at xing.   */\n");
  fprintf(outfile,"\t\t              /* 0 or 1: RK4 or Taylor integrate back   */\n");  
  fprintf(outfile,"\t\t out_file,    /* file to save output data               */\n");
  fprintf(outfile,"\t\t jetIn        /*       */\n");
  fprintf(outfile,"\t\t );\n\n");        
  fprintf(outfile,"}\n\n");  


  genGeneralRK4Code();
}


static char *RK4_doc =
"\
/* Generate a special RK4 integrator for Poincare section calculations.\n\
 * \n\
 *  Integrate ODE one step using RungeKutta 4th order algorithm, using \n\
 *  Poincare section function as the new time variable. Jet is ignored. \n\
 *  The process will locate the exact time when the orbit intersects the \n\
 *  section surface.  Once the time is found, a taylor integrator is \n\
 *  used to find the intersection, including jet variables.\n\
 *\n\
 *  Parameters:\n\
 *  MY_FLOAT t,       time\n\
 *  MY_FLOAT step,    step size\n\
 *  MY_FLOAT *xx_in,  value of state variables.\n\
 *  MY_FLOAT *xx_out, to output new values of state variables. If NULL, xx_in will be updated instead\n\
 *\n\
 *  Important: the dimension of xx_in and xx_out must be 1+nequations. I.e, state + time. Caller is \n\
 *             responsible to provide storate and initialize xx_in. \n\
 *\n\
 *  int flag          flag, not used.\n\
 *\n\
 */\n\
";


void genRK4Code() {
  int j;

  fprintf(outfile,"%s\n",RK4_doc);
  
  j = neqns + 1;  

 if(0) {
  fprintf(outfile,"void RK4_%s_deri_normal(MY_FLOAT t, MY_FLOAT *xx_in, MY_FLOAT *xx_out, MY_JET *sIn, MY_JET ***sOut) {\n",suffix);
  fprintf(outfile,"\t static int initialized=0;\n");
  fprintf(outfile,"\t static MY_FLOAT myFloatOne;\n");
  fprintf(outfile, "#pragma omp threadprivate(initialized, myFloatOne)\n");
  fprintf(outfile,"\t MY_FLOAT **ptr;\n");
  fprintf(outfile,"\t int i;\n");  
  fprintf(outfile, "\t if(!initialized) {\n");
  fprintf(outfile, "\t\t initialized = 1;\n");
  fprintf(outfile, "\t\t InitMyFloat(myFloatOne);\n");
  fprintf(outfile, "\t\t MakeMyFloatC(myFloatOne, \"1.0\",(double)1.0);\n");          
  fprintf(outfile, "\t }\n\n");
  fprintf(outfile,"\t// ptr = taylor_coefficients_%s(t, xx_in, 1) ;\n", suffix);

  fprintf(outfile,"\t ptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, sIn, sOut) ;\n", suffix);  

  
  fprintf(outfile,"\t for(i=0; i < %d; i++){\n",neqns-nonautonomous);
  fprintf(outfile, "\t\t AssignMyFloat(xx_out[i],ptr[i][1]);\n");  
  fprintf(outfile,"\t }\n");
  if(1||nonautonomous ) {
    fprintf(outfile, "\t  /* time, curcial for  nonautonomous systems */\n");    
    fprintf(outfile, "\t  AssignMyFloat(xx_out[%d],myFloatOne);\n",neqns-nonautonomous);
  }  
  fprintf(outfile,"}\n\n");
 }
 
  fprintf(outfile,"/* After orbit crosses p section, we switch to use P as the new time, and \n");
  fprintf(outfile," * use RK4 to integrate backwards with exactly the P value as step. This is \n");
  fprintf(outfile," * routine compute the deriv w r t P, incuding t.  Note caller is responsibleis for /\n");
  fprintf(outfile," * pass over t as the last entry in xx[]. Note also we need to supply a routine \n");
  fprintf(outfile," * to evaluate the old time deriv of P. \n");  
  fprintf(outfile," */\n");          
  fprintf(outfile,"void RK4_%s_deri_for_poincare(MY_FLOAT t, MY_FLOAT *xx_in, MY_FLOAT *xx_out) {\n",suffix);
  fprintf(outfile,"\t int i;\n");
  fprintf(outfile,"\t static int initialized=0;\n");      
  fprintf(outfile,"\t static MY_FLOAT myFloatOne, psmall, ptmp,  pderi[1], deri[%d]; \n",j);
  fprintf(outfile,"\t static MY_JET    jetIn[%d];\n",   1+state_jet_vars);  
  fprintf(outfile, "#pragma omp threadprivate(initialized, myFloatOne, psmall, ptmp,  pderi[1], deri[%d], jetIn)\n",j);  
  fprintf(outfile,"\t MY_FLOAT **ptr; \n");

  
  fprintf(outfile, "\t if(!initialized) {\n");
  fprintf(outfile, "\t\t initialized = 1;\n");
  fprintf(outfile, "\t\t InitMyFloat(myFloatOne);\n");
  fprintf(outfile, "\t\t InitMyFloat(ptmp);\n");  
  fprintf(outfile, "\t\t InitMyFloat(psmall);\n");      
  fprintf(outfile, "\t\t InitMyFloat(pderi[0]);\n");
  fprintf(outfile, "\t\t MakeMyFloatC(myFloatOne, \"1.0\",(double)1.0);\n");          
  fprintf(outfile, "\t\t MakeMyFloatC(psmall, \"1E-10\",(double)1E-10);\n");        
  fprintf(outfile, "\t\t for(i=0; i<%d; i++){\n", j);
  fprintf(outfile, "\t\t    InitMyFloat(deri[i]);\n");
  fprintf(outfile, "\t\t }\n");
  if(state_jet_vars>0) {  
    fprintf(outfile, "\t\t for(i=0; i<%d; i++){\n", 1+state_jet_vars);
    fprintf(outfile, "\t\t    InitJet(jetIn[i]);\n");
    fprintf(outfile, "\t\t }\n");
  }
  fprintf(outfile, "\t }\n\n");
  
  if(1||nonautonomous ) {
    fprintf(outfile, "\t  /* time depends on p. RK4 changes it! crucial for nonautonomous systems  */\n");
    fprintf(outfile, "\t  /* i.e, it is not the original t.                     */\n");    
    fprintf(outfile, "\t  AssignMyFloat(t,xx_in[%d]);\n",neqns-nonautonomous);
  }

  if(state_jet_vars > 0) {
    fprintf(outfile, "\t for(i=0; i<%d; i++) {AssignFloatToJet(jetIn[i], xx_in[i]);}\n",state_jet_vars+1);
  }
  
  fprintf(outfile, "\t %s_ps_section_function_derivative(t,xx_in,pderi,jetIn,NULL);\n",suffix);
  fprintf(outfile, "\t fabsMyFloatA(ptmp, pderi[0]);\n");
  fprintf(outfile, "\t if(MyFloatA_LT_B(ptmp,psmall)) {\n");  
  fprintf(outfile,"\t\t    fprintf(stderr,\"orbit almost tangent to section.\\n\");\n");
  fprintf(outfile,"\t }\n");        
  fprintf(outfile,"\t ptr = taylor_coefficients_%s_A(t, xx_in, 1,0,jetIn,NULL);\n", suffix);
  fprintf(outfile,"\t for(i=0; i < %d; i++){\n",neqns-nonautonomous);
  fprintf(outfile, "\t   DivideMyFloatA(ptmp, ptr[i][1],pderi[0]);\n");    
  fprintf(outfile, "\t   AssignMyFloat(xx_out[i],ptmp);\n");
  fprintf(outfile,"\t  }\n");
  if(1 || nonautonomous ) {
    fprintf(outfile, "\t  /* time, crucial for nonautonomous systems */\n");
    fprintf(outfile, "\t  DivideMyFloatA(ptmp, myFloatOne,pderi[0]);\n");        
    fprintf(outfile, "\t  AssignMyFloat(xx_out[%d],ptmp);\n",neqns-nonautonomous);
  }
  fprintf(outfile,"}\n\n");
  
  fprintf(outfile,"void RK4_%s_poincare(MY_FLOAT t, MY_FLOAT step, MY_FLOAT *xx_in, MY_FLOAT *xx_out, int flag ){\n",suffix);

  fprintf(outfile,"\t int i, j;\n");  
  fprintf(outfile,"\t static int initialized=0;\n");  
  fprintf(outfile,"\t static MY_FLOAT  oneHalf, oneSixth, myFloatZero, myFloatOne;\n");

  fprintf(outfile,"\t static MY_FLOAT  t1, t2, ptmp1, ptmp2, ptmp3, half_step;\n");
  j = neqns + 1;
  fprintf(outfile,"\t static MY_FLOAT  k0[%d], k1[%d], k2[%d], k3[%d], k4[%d], xx[%d];\n", j,j,j,j,j,j);

  fprintf(outfile, "#pragma omp threadprivate(initialized,oneHalf, oneSixth, myFloatZero, myFloatOne,t1, t2, ptmp1, ptmp2, ptmp3, half_step)\n");  
  fprintf(outfile, "#pragma omp threadprivate(k0[%d], k1[%d], k2[%d], k3[%d], k4[%d], xx[%d])\n", j,j,j,j,j,j);
  fprintf(outfile, "\t if(!initialized) {\n");
  fprintf(outfile, "\t\t initialized = 1;\n");
  fprintf(outfile, "\t\t InitMyFloat(myFloatZero); InitMyFloat(myFloatOne); InitMyFloat(oneHalf); InitMyFloat(oneSixth);\n");
  fprintf(outfile, "\t\t InitMyFloat(half_step); InitMyFloat(t1); InitMyFloat(t2); InitMyFloat(ptmp1); InitMyFloat(ptmp2); InitMyFloat(ptmp3);\n");  
  fprintf(outfile, "\t\t for(i=0; i<%d; i++){\n", j);
  fprintf(outfile, "\t\t    InitMyFloat(xx[i]);\n");
  fprintf(outfile, "\t\t    InitMyFloat(k0[i]);InitMyFloat(k1[i]);InitMyFloat(k2[i]);InitMyFloat(k3[i]); InitMyFloat(k4[i]);\n");
  fprintf(outfile, "\t\t }\n");  
  fprintf(outfile, "\t\t MakeMyFloatC(myFloatZero,\"0.0\", (double)0.0);\n");  
  fprintf(outfile, "\t\t MakeMyFloatC(myFloatOne, \"1.0\", (double)1.0);\n");
  fprintf(outfile, "\t\t MakeMyFloatC(t1, \"2.0\", (double)2.0);\n");
  fprintf(outfile, "\t\t MakeMyFloatC(t2, \"6.0\", (double)6.0);\n");
  fprintf(outfile, "\t\t DivideMyFloatA(oneHalf,  myFloatOne, t1);\n");
  fprintf(outfile, "\t\t DivideMyFloatA(oneSixth,  myFloatOne, t2);\n");    
  fprintf(outfile, "\t }\n");
  fprintf(outfile, "\t MultiplyMyFloatA(half_step,  step, oneHalf);\n");
  fprintf(outfile, "\t AddMyFloatA(t1, half_step, t);\n");
  fprintf(outfile, "\t AddMyFloatA(t2, step, t);\n");
  
  fprintf(outfile, "\t RK4_%s_deri_for_poincare(t, xx_in, k0);\n", suffix);
  fprintf(outfile, "\t for(i=0;i<%d;i++){\n", neqns+1-nonautonomous);  
  fprintf(outfile, "\t   MultiplyMyFloatA(k1[i], k0[i], step);\n");    
  fprintf(outfile, "\t   MultiplyMyFloatA(ptmp1, k1[i], oneHalf);\n");    
  fprintf(outfile, "\t   AddMyFloatA(xx[i], xx_in[i], ptmp1);\n");
  fprintf(outfile, "\t }\n");   

  fprintf(outfile, "\t RK4_%s_deri_for_poincare(t1, xx, k0);\n", suffix);
  fprintf(outfile, "\t for(i=0;i<%d;i++){\n", neqns+1-nonautonomous);    
  fprintf(outfile, "\t   MultiplyMyFloatA(k2[i], k0[i], step);\n");    
  fprintf(outfile, "\t   MultiplyMyFloatA(ptmp1, k2[i], oneHalf);\n");    
  fprintf(outfile, "\t   AddMyFloatA(xx[i], xx_in[i], ptmp1);\n");
  fprintf(outfile, "\t }\n");     

  fprintf(outfile, "\t RK4_%s_deri_for_poincare(t1, xx, k0);\n", suffix);
  fprintf(outfile, "\t for(i=0;i<%d;i++){\n", neqns+1-nonautonomous);      
  fprintf(outfile, "\t   MultiplyMyFloatA(k3[i], k0[i], step);\n");    
  fprintf(outfile, "\t   AddMyFloatA(xx[i], xx_in[i], k3[i]);\n");
  fprintf(outfile, "\t }\n");     

  fprintf(outfile, "\t RK4_%s_deri_for_poincare(t2, xx, k0);\n", suffix);
  fprintf(outfile, "\t for(i=0;i<%d;i++){\n", neqns+1-nonautonomous);      
  fprintf(outfile, "\t   MultiplyMyFloatA(k4[i], k0[i], step);\n");
  fprintf(outfile, "\t }\n");
  
  fprintf(outfile, "\t for(i=0;i<%d;i++){\n", neqns+1-nonautonomous);        
  fprintf(outfile, "\t   AddMyFloatA(ptmp1, k2[i], k3[i]);\n");
  fprintf(outfile, "\t   AddMyFloatA(ptmp2, ptmp1, ptmp1);\n");
  fprintf(outfile, "\t   AddMyFloatA(ptmp3, k1[i], k4[i]);\n");
  fprintf(outfile, "\t   AddMyFloatA(ptmp1, ptmp2,ptmp3);\n");
  fprintf(outfile, "\t   MultiplyMyFloatA(ptmp2, ptmp1,oneSixth);\n");
  fprintf(outfile, "\t   if(xx_out) {\n");
  fprintf(outfile, "\t\t   AddMyFloatA(ptmp1, ptmp2, xx_in[i]);\n");
  fprintf(outfile, "\t\t   AssignMyFloat(xx_out[i], ptmp1);\n");              
  fprintf(outfile, "\t   } else  {\n");
  fprintf(outfile, "\t\t   AddMyFloatA(ptmp1, ptmp2,xx_in[i]);\n");              
  fprintf(outfile, "\t\t   AssignMyFloat(xx_in[i], ptmp1);\n");            
  fprintf(outfile, "\t   }\n");
  fprintf(outfile, "\t }\n");                 

  fprintf(outfile, "} \n");      
}

//==== toy with RK4

void genGeneralRK4Code() {
  int m = neqns, k= m + 1, n= state_jet_vars, j=n+1;
  fprintf(outfile,"\n\
/*\n\
 * single integration step with RK4. the parameters are:\n\
 *\n\
 * t_in: on input:  time of the initial condition\n\
 *     on output: new time\n\
 *\n\
 * x_in:  on input:  initial condition\n\
 *     on output: new condition, corresponding to the (output) time ti\n\
 *\n\
 * dir: flag to integrate forward or backwards.\n\
 *     1: forward\n\
 *    -1: backwards\n\
 *\n\
 * step_ctl: not used\n\
 *\n\
 * log10abserr:  not used\n\
 *\n\
 * log10relerr:  not ussed\n\
 *\n\
 * endtime: if NULL, it is ignored. Otherwise, it is used to make sure\n\
 *          next step will overtake it,\n\
 *\n\
 * ht: on input:  step size\n\
 *     on output: step size used. There may be a difference only t reaches endtime\n\
 *\n\
 * order:  ignored\n\
 *\n\
 * jetInOut: on input: the value of all declared jet variables\n\
 *           on output: new value of the jet variable, corresponding to the new time\n\
 *\n\
 * return value:\n\
 * 0: ok.\n\
 * 1: ok, and ti=endtime.  */\n\
");

  fprintf(outfile, "int RK4_step_%s(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl, double log10abserr, double log10relerr, MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn) { //out to jetIn on return\n",suffix);
  fprintf(outfile, "    int i, j, flag = 0;\n");
  fprintf(outfile, "    static int initialized=0;\n");
  fprintf(outfile, "    static MY_FLOAT  oneHalf, oneSixth, myFloatZero, myFloatOne,step,t;\n");
  fprintf(outfile, "    static MY_FLOAT  t1, t2, ptmp1, ptmp2, ptmp3, half_step;\n");
  fprintf(outfile, "    static MY_FLOAT  k0[%d], k1[%d], k2[%d], k3[%d], k4[%d], xx[%d];\n", k,k,k,k,k,k);
  fprintf(outfile, "    static MY_JET **jetOut;\n");
  fprintf(outfile, "    static MY_JET  jtmp1, jtmp2, jtmp3;\n");
  fprintf(outfile, "    static MY_JET  jet0[%d], jet1[%d], jet2[%d], jet3[%d], jet4[%d], jet[%d];\n",j,j,j,j,j,j);
  fprintf(outfile, "    MY_FLOAT **fptr;\n");
	 
  fprintf(outfile, "    #pragma omp threadprivate(jetOut) \n");
  fprintf(outfile, "    #pragma omp threadprivate(initialized,oneHalf, oneSixth, myFloatZero, myFloatOne,t1, t2, ptmp1, ptmp2, ptmp3, half_step,step,t)\n");
  fprintf(outfile, "    #pragma omp threadprivate(k0, k1, k2, k3, k4, xx)\n");
  fprintf(outfile, "    #pragma omp threadprivate(jet, jet1, jet2, jet3, jet4)\n");
  fprintf(outfile, "    #pragma omp threadprivate(jtmp1,jtmp2,jtmp3)\n\n");
  fprintf(outfile, "    if(!initialized) {\n");
  fprintf(outfile, "      initialized = 1;\n");
  fprintf(outfile, "      InitMyFloat(myFloatZero); InitMyFloat(myFloatOne); InitMyFloat(oneHalf); InitMyFloat(oneSixth); InitMyFloat(step); InitMyFloat(t);\n");
  fprintf(outfile, "      InitMyFloat(half_step); InitMyFloat(t1); InitMyFloat(t2); InitMyFloat(ptmp1); InitMyFloat(ptmp2); InitMyFloat(ptmp3);\n");
  fprintf(outfile, "      for(i=0; i<%d; i++){\n",k);
  fprintf(outfile, "        InitMyFloat(xx[i]);\n");
  fprintf(outfile, "        InitMyFloat(k0[i]);InitMyFloat(k1[i]);InitMyFloat(k2[i]);InitMyFloat(k3[i]); InitMyFloat(k4[i]);\n");
  fprintf(outfile, "      }\n");
  if(state_jet_vars > 0) {
    fprintf(outfile, "      for(i=0; i<%d; i++){\n", j);
    fprintf(outfile, "        InitJet(jet0[i]);InitJet(jet1[i]);InitJet(jet2[i]);InitJet(jet3[i]);InitJet(jet4[i]);InitJet(jet[i]);\n");
    fprintf(outfile, "      }\n");    
    fprintf(outfile, "      InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);\n");
  }		 
  fprintf(outfile, "      MakeMyFloatC(myFloatZero,\"0.0\", (double)0.0);\n");
  fprintf(outfile, "      MakeMyFloatC(myFloatOne, \"1.0\", (double)1.0);\n");
  fprintf(outfile, "      MakeMyFloatC(t1, \"2.0\", (double)2.0);\n");
  fprintf(outfile, "      MakeMyFloatC(t2, \"6.0\", (double)6.0);\n");
  fprintf(outfile, "      DivideMyFloatA(oneHalf,  myFloatOne, t1);\n");
  
  fprintf(outfile, "      DivideMyFloatA(oneSixth,  myFloatOne, t2);\n");
  fprintf(outfile, "      }\n\n");

  fprintf(outfile, "     AssignMyFloat(t, *t_in); \n");
  fprintf(outfile, "     AssignMyFloat(step, *ht); \n");  
  fprintf(outfile, "    \n// first check if endtime is reached\n");
  fprintf(outfile, "     if(dir >0 ) { // forward\n");
  fprintf(outfile, "       AddMyFloatA(t1, t, step);\n");
  fprintf(outfile, "       if(MyFloatA_GE_B(t1,*endtime)) {\n");
  fprintf(outfile, "         flag=1;\n");
  fprintf(outfile, "         SubtractMyFloatA(t2,*endtime, t);\n");
  fprintf(outfile, "         AssignMyFloat(step, t2);\n");
  fprintf(outfile, "         AssignMyFloat(*ht, step);\n");
  fprintf(outfile, "        }\n");
  fprintf(outfile, "     } else { // backward\n");
  fprintf(outfile, "       SubtractMyFloatA(t1, t, step);\n");
  fprintf(outfile, "       if(MyFloatA_LE_B(t1,*endtime)) {\n");
  fprintf(outfile, "        flag=1;\n");
  fprintf(outfile, "        SubtractMyFloatA(t2, t, *endtime);\n");
  fprintf(outfile, "        AssignMyFloat(step, t2);\n");
  fprintf(outfile, "        AssignMyFloat(*ht, step);\n");
  fprintf(outfile, "     }\n");
  fprintf(outfile, "     NegateMyFloatA(step, step); // make it forward with negative step\n");
  fprintf(outfile, "     }\n\n");
      

  fprintf(outfile, "     MultiplyMyFloatA(half_step,  step, oneHalf);\n");
  fprintf(outfile, "     AddMyFloatA(t1, half_step, t);\n");
  fprintf(outfile, "     AddMyFloatA(t2, step, t);\n");

  fprintf(outfile, "    \n// k0 = f(t0,x0); xx = x0+h/2 k0; k1= h k0 =>  k1 = h f(t0,x0)\n");
  if(state_jet_vars > 0)   
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, jetIn, &jetOut); \n", suffix);
  else
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, NULL, NULL); \n", suffix);    
  fprintf(outfile, "     for(i=0;i<%d;i++){\n", m);
  fprintf(outfile, "       AssignMyFloat(k0[i], fptr[i][1]); \n");
  fprintf(outfile, "     }\n");
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       MultiplyMyFloatA(k1[i], k0[i], step);\n");
  fprintf(outfile, "       MultiplyMyFloatA(ptmp1, k1[i], oneHalf);\n");
  fprintf(outfile, "       AddMyFloatA(xx[i], xx_in[i], ptmp1);\n");
  fprintf(outfile, "     }\n");
  if(state_jet_vars > 0) {
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "        AssignJetToJet(jet0[i], jetOut[i][1]);\n");
    fprintf(outfile, "     }\n");
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "       MultiplyJetFloatA(jet1[i], jet0[i], step);\n");
    fprintf(outfile, "       MultiplyJetFloatA(jtmp1, jet1[i], oneHalf);\n");
    fprintf(outfile, "       AddJetJetA(jet[i], jetIn[i], jtmp1);\n");
    fprintf(outfile, "      }\n");
  }
  fprintf(outfile, "     // k0 = f(t0+h/2,xx); xx=x0+h/2 k0; k2= h k0 =>  k2 = h f(t0+h/2,x0+1/2 k1)  	 \n");
  if(state_jet_vars > 0)   
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t1, xx, 1, 0, jet, &jetOut);\n", suffix);
  else
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t1, xx, 1, 0, NULL, NULL);\n", suffix);  
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       AssignMyFloat(k0[i], fptr[i][1]);\n");
  fprintf(outfile, "     }	 \n");
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       MultiplyMyFloatA(k2[i], k0[i], step);\n");
  fprintf(outfile, "       MultiplyMyFloatA(ptmp1, k2[i], oneHalf);\n");
  fprintf(outfile, "       AddMyFloatA(xx[i], xx_in[i], ptmp1);\n");
  fprintf(outfile, "     }\n");
  if(state_jet_vars > 0) {
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "       AssignJetToJet(jet0[i], jetOut[i][1]);\n");
    fprintf(outfile, "     }\n");
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n); 
    fprintf(outfile, "       MultiplyJetFloatA(jet2[i], jet0[i], step);\n");
    fprintf(outfile, "       MultiplyJetFloatA(jtmp1, jet2[i], oneHalf);\n");
    fprintf(outfile, "       AddJetJetA(jet[i], jetIn[i], jtmp1);\n");
    fprintf(outfile, "     }\n");
  }
  fprintf(outfile, "    \n// k0 = f(t0+h/2,xx); xx= x0+ h*k0; k3 = h k0 => k2 h f(t0+h/2,x0+1/2 k2)\n");
  if(state_jet_vars > 0)     
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t1, xx, 1, 0, jet, &jetOut);\n",suffix);
  else
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t1, xx, 1, 0, NULL, NULL);\n",suffix);
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       AssignMyFloat(k0[i], fptr[i][1]);\n");
  fprintf(outfile, "     }\n");
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       MultiplyMyFloatA(k3[i], k0[i], step);\n");
  fprintf(outfile, "       AddMyFloatA(xx[i], xx_in[i], k3[i]);\n");
  fprintf(outfile, "     }\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "        AssignJetToJet(jet0[i], jetOut[i][1]);\n");
    fprintf(outfile, "     }\n");
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "        MultiplyJetFloatA(jet3[i], jet0[i], step);\n");
    fprintf(outfile, "        AddJetJetA(jet[i], jetIn[i], jet3[i]);\n");
    fprintf(outfile, "     }\n");
  }
  fprintf(outfile, "    \n// k0 = f(t0+h, xx);  k4= h*k0 => k4 = h f(t0+h, x0+k3)\n");
  if(state_jet_vars > 0)       
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t2, xx, 1, 0, jet, &jetOut);\n", suffix);
  else
    fprintf(outfile, "     fptr = taylor_coefficients_%s_A(t2, xx, 1, 0, NULL, NULL);\n", suffix);    
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "     AssignMyFloat(k0[i], fptr[i][1]);\n");
  fprintf(outfile, "     } \n");
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       MultiplyMyFloatA(k4[i], k0[i], step);\n");
  fprintf(outfile, "     }\n");

  if(state_jet_vars>0) {
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "        AssignJetToJet(jet0[i], jetOut[i][1]);\n");
    fprintf(outfile, "     }\n");    
    fprintf(outfile, "     for(i=0;i<%d;i++){\n",n);
    fprintf(outfile, "         MultiplyJetFloatA(jet4[i], jet0[i], step);\n");
    fprintf(outfile, "     }\n");
  }

  fprintf(outfile, "    \n// xx_out=xx_in + 1/6(k1+ 2*(k2+k3)+k4)\n");
  
  fprintf(outfile, "     for(i=0;i<%d;i++){\n",m);
  fprintf(outfile, "       AddMyFloatA(ptmp1, k2[i], k3[i]);\n");
  fprintf(outfile, "       AddMyFloatA(ptmp2, ptmp1, ptmp1);\n");
  fprintf(outfile, "       AddMyFloatA(ptmp3, k1[i], k4[i]);\n");
  fprintf(outfile, "       AddMyFloatA(ptmp1, ptmp2,ptmp3);\n");
  fprintf(outfile, "       MultiplyMyFloatA(ptmp2, ptmp1,oneSixth);\n");
  fprintf(outfile, "       AddMyFloatA(ptmp1, ptmp2,xx_in[i]);\n");
  fprintf(outfile, "       AssignMyFloat(xx_in[i], ptmp1);\n");
  fprintf(outfile, "       }\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "       for(i=0;i<%d;i++){\n",m);
    fprintf(outfile, "         AddJetJetA(jtmp1, jet2[i], jet3[i]);\n");
    fprintf(outfile, "         AddJetJetA(jtmp2, jtmp1, jtmp1);\n");
    fprintf(outfile, "         AddJetJetA(jtmp3, jet1[i], jet4[i]);\n");
    fprintf(outfile, "         AddJetJetA(jtmp1, jtmp2,jtmp3);\n");
    fprintf(outfile, "         MultiplyJetFloatA(jtmp2,jtmp1,oneSixth);\n");

    fprintf(outfile, "       AddJetJetA(jtmp1, jtmp2,jetIn[i]);\n");
    fprintf(outfile, "       AssignJetToJet(jetIn[i], jtmp1);\n");
    fprintf(outfile, "     } \n");
  }
  fprintf(outfile, "       AddMyFloatA(*t_in, t, step);\n");  
  fprintf(outfile, "     return flag;\n");
  fprintf(outfile, "}\n\n");  

  
  fprintf(outfile,"\n\
/*\n\
 * single integration step with RK4 with adaptive step size. the parameters are:\n\
 *\n\
 * t_in: on input:  time of the initial condition\n\
 *       on output: new time\n\
 *\n\
 * x_in:  on input:  initial condition\n\
 *     on output: new condition, corresponding to the (output) time ti\n\
 *\n\
 * dir: flag to integrate forward or backwards.\n\
 *     1: forward\n\
 *    -1: backwards\n\
 *\n\
 * step_ctl: not used\n\
 *\n\
 * log10abserr:  log10(epsilon), where epsilon is used for error control. if Err/epsilon > 1,\n\
 *               then shrink step size t0 0.9*h/(Err/epsilon)^0.25. If Err/epsilon <1, then \n\
 *               grow the step size to  0.9*h/(Err/epsilon)^0.2.\n\
 *\n\
 * log10relerr:  not ussed\n\
 *\n\
 * endtime: if NULL, it is ignored. Otherwise, it is used to make sure\n\
 *          next step will not overtake it.\n\
 *\n\
 * ht: on input:  step size\n\
 *     on output: next step size to use\n\
 *\n\
 * order:  ignored\n\
 *\n\
 * jetInOut: on input: the value of all declared jet variables\n\
 *           on output: new value of the jet variable, corresponding to the new time\n\
 *\n\
 * return value:\n\
 * 0: ok.\n\
 * 1: ok, and ti=endtime.  */\n\
");

  
  fprintf(outfile, "int RK4QC_step_%s(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl, double log10abserr, double log10relerr, MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn) { //out to jetIn on return\n",suffix);

  fprintf(outfile, "\t static int initialized =0;\n");
  fprintf(outfile, "\t static MY_FLOAT PGROW, PSHRNK, FCOR, ONE, TWO, SAFETY, ERRCON, ERROR_SCALE, ZERO, EPSILON;\n");
  fprintf(outfile, "\t static MY_FLOAT  ptmp1, ptmp2, ptmp3, h, h1, hh, hh0,hh1, hnext, oneHalf, ttmp;\n");
  fprintf(outfile, "\t static MY_FLOAT  xtmp[%d], xtmp2[%d], xtmp3[%d], errmax, jerrmax, TOOSMALL;\n",k,k,k);
  fprintf(outfile, "\t static MY_JET  jtmp1, jtmp2, jtmp3;\n");
  fprintf(outfile, "\t static MY_JET  jet[%d], jet1[%d], jet2[%d], jet3[%d];\n",j,j,j,j);

  fprintf(outfile, "   #pragma omp threadprivate(initialized,PGROW, PSHRNK, FCOR, ONE, TWO, SAFETY, ERRCON, ERROR_SCALE, ZERO, EPSILON)\n");
  fprintf(outfile, "   #pragma omp threadprivate ptmp1, ptmp2, ptmp3, h, h1, hh, hh0,hh1, hnext, oneHalf, ttmp)\n");
  fprintf(outfile, "   #pragma omp threadprivate(xtmp, xtmp2, xtmp3, errmax, jerrmax, TOOSMALL)\n");
  fprintf(outfile, "   #pragma omp threadprivate(jet,jet1,jet2,het3)\n");      
  fprintf(outfile, "   #pragma omp threadprivate(jtmp1, jtmp2, jtmp3)\n");  
  
  fprintf(outfile, "   int i,flag;\n");
  fprintf(outfile, "   int ended = 0, done = 0;\n");
  
  fprintf(outfile, "   if(!initialized) {\n");
  fprintf(outfile, "       double abs_error = pow(10,log10abserr);\n");
  fprintf(outfile, "       initialized = 1;\n");
  fprintf(outfile, "       InitMyFloat(PGROW);InitMyFloat(PSHRNK); InitMyFloat(FCOR); InitMyFloat(ONE); InitMyFloat(SAFETY); InitMyFloat(ERRCON);InitMyFloat(ZERO); InitMyFloat(EPSILON);\n");
  fprintf(outfile, "       InitMyFloat(ttmp); InitMyFloat(ptmp1); InitMyFloat(ptmp2); InitMyFloat(ptmp3);InitMyFloat(TOOSMALL);InitMyFloat(ERROR_SCALE);\n");
  fprintf(outfile, "       InitMyFloat(h); InitMyFloat(h1); InitMyFloat(hh); InitMyFloat(hh0); InitMyFloat(hh1); InitMyFloat(hnext);\n");
  fprintf(outfile, "       InitMyFloat(oneHalf); InitMyFloat(errmax); InitMyFloat(jerrmax);\n");
  fprintf(outfile, "       InitMyFloat(TWO);\n");
  fprintf(outfile, "       for(i=0; i<%d; i++) {\n", k);
  fprintf(outfile, "           InitMyFloat(xtmp[i]); InitMyFloat(xtmp2[i]);  InitMyFloat(xtmp3[i]);\n");
  fprintf(outfile, "       }\n");
  if(state_jet_vars >0) {
    
    fprintf(outfile, "       for(i=0; i<%d; i++){\n", j);
    fprintf(outfile, "         InitJet(jet1[i]);InitJet(jet2[i]);InitJet(jet3[i]);InitJet(jet[i]);\n");
    fprintf(outfile, "       }\n");
    fprintf(outfile, "       InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);\n");
  }
  
  fprintf(outfile, "   MakeMyFloatA(EPSILON, abs_error);\n");
  fprintf(outfile, "   MakeMyFloatC(PGROW,   \"-0.2\", (double)-0.2);\n");
  fprintf(outfile, "   MakeMyFloatC(PSHRNK,  \"-0.25\", (double)-0.25);\n");
  fprintf(outfile, "   MakeMyFloatC(SAFETY,  \"0.9\", (double)0.9);\n");
  fprintf(outfile, "   MakeMyFloatC(ERRCON,   \"0.0006\", (double)0.0006);\n");
  fprintf(outfile, "   MakeMyFloatC(TOOSMALL, \"0.00000001\", (double)0.00000001);\n");
  fprintf(outfile, "   MakeMyFloatC(ERROR_SCALE, \"2.0\", (double)2.0);            \n");
  fprintf(outfile, "   MakeMyFloatC(ONE,  \"1.0\", 1.0);\n");
  fprintf(outfile, "   MakeMyFloatC(ZERO,  \"0\", (double)0);    \n");
  fprintf(outfile, "   MakeMyFloatC(TWO, \"2.0\", (double)2.0);\n");
  fprintf(outfile, "   MakeMyFloatC(ptmp2, \"15.0\", (double)15.0);\n");
  fprintf(outfile, "   DivideMyFloatA(oneHalf,  ONE, TWO);\n");
  fprintf(outfile, "   DivideMyFloatA(FCOR,  ONE, ptmp2);    \n");
  fprintf(outfile, "   }\n");

  fprintf(outfile, "   AssignMyFloat(h,    *ht);\n");
  fprintf(outfile, "   AssignMyFloat(ttmp, *t_in);\n");

 
  fprintf(outfile, "   do {\n");
  fprintf(outfile, "     MultiplyMyFloatA(hh, h, oneHalf);\n");
  fprintf(outfile, "     AssignMyFloat(hh0,hh);\n");
  fprintf(outfile, "     AssignMyFloat(hh1,hh);        \n");

  fprintf(outfile, "     for(i=0; i<%d;  i++) {\n", m);
  fprintf(outfile, "         AssignMyFloat(xtmp[i], xx_in[i]);\n");
  fprintf(outfile, "     }\n");
  fprintf(outfile, "     AssignMyFloat(ttmp,*t_in);\n");
  if(state_jet_vars>0) {
    fprintf(outfile, "     for(i=0; i< %d; i++) {\n",n);
    fprintf(outfile, "        AssignJetToJet(jet[i], jetIn[i]);\n");
    fprintf(outfile, "        AssignJetToJet(jet1[i], jetIn[i]);    \n");
    fprintf(outfile, "     }\n");
  }
  fprintf(outfile, "     // two half step rk4\n");
  if(state_jet_vars>0)
    fprintf(outfile, "     flag = RK4_step_%s(&ttmp, xtmp, dir, 0, log10abserr, log10relerr, endtime,  &hh0,  NULL, jet);\n",suffix);
  else
    fprintf(outfile, "     flag = RK4_step_%s(&ttmp, xtmp, dir, 0, log10abserr, log10relerr, endtime,  &hh0,  NULL, NULL);\n",suffix);    

  fprintf(outfile, "     if(flag){\n");
  fprintf(outfile, "         ended = done=1;\n");
  fprintf(outfile, "     } else {\n");
  if(state_jet_vars>0)  
    fprintf(outfile, "        flag = RK4_step_%s(&ttmp,  xtmp, dir, 0, log10abserr, log10relerr, endtime,  &hh1,  NULL, jet);\n",suffix);
  else
    fprintf(outfile, "        flag = RK4_step_%s(&ttmp,  xtmp, dir, 0, log10abserr, log10relerr, endtime,  &hh1,  NULL, NULL);\n",suffix);  

  fprintf(outfile, "         if(flag) {\n");
  fprintf(outfile, "           ended = done=1;\n");
  fprintf(outfile, "         }\n");
  fprintf(outfile, "    }\n");
  fprintf(outfile, "    if(MyFloatA_LE_B(h,TOOSMALL)) {\n");
  fprintf(outfile, "        fprintf(stderr, \"Step size is too small, giving up.\\n\");\n");
  fprintf(outfile, "        ended = done = 1;\n");
  fprintf(outfile, "    }\n");

  fprintf(outfile, "    if(ended) break;\n");
    
  fprintf(outfile, "    for(i=0; i<%d; i++) {\n",m);
  fprintf(outfile, "       AssignMyFloat(xtmp2[i],xx_in[i]);\n");
  fprintf(outfile, "    }\n");
  fprintf(outfile, "    AssignMyFloat(ttmp,*t_in);\n");
  fprintf(outfile, "    AssignMyFloat(h1,h);\n");
  fprintf(outfile, "    // one full step rk4\n");  
  if(state_jet_vars>0)
    fprintf(outfile, "    flag = RK4_step_%s(&ttmp, xtmp2, dir, 0, log10abserr, log10relerr, endtime,  &h1,  NULL, jet1);\n",suffix);
  else
    fprintf(outfile, "    flag = RK4_step_%s(&ttmp, xtmp2, dir, 0, log10abserr, log10relerr, endtime,  &h1,  NULL, NULL);\n",suffix);

  if(state_jet_vars>0) {  
    fprintf(outfile, "    AssignMyFloat(jerrmax, ZERO);        \n");
    fprintf(outfile, "    for(i=0; i< %d; i++) {\n", n);
    fprintf(outfile, "       SubtractJetJetA(jet3[i], jet1[i], jet[i]);\n");
    fprintf(outfile, "       xNormJet(ptmp1,jet3[i]);\n");
    fprintf(outfile, "       fabsMyFloatA(ptmp3, ptmp1);\n");
    fprintf(outfile, "       if(MyFloatA_GT_B(ptmp3, jerrmax)) {\n");
    fprintf(outfile, "         AssignMyFloat(jerrmax, ptmp3);\n");
    fprintf(outfile, "       }\n");
    fprintf(outfile, "    }\n");
    fprintf(outfile, "    AssignMyFloat(errmax, jerrmax);\n");
  } else {
    fprintf(outfile, "    AssignMyFloat(errmax, ZERO);        \n");
  }
  fprintf(outfile, "    for(i=0; i<%d; i++) {\n",m);
  fprintf(outfile, "       SubtractMyFloatA(xtmp3[i], xtmp2[i],xtmp[i]);\n");
  fprintf(outfile, "       fabsMyFloatA(ptmp2, xtmp3[i]);\n");
  fprintf(outfile, "       DivideMyFloatA(ptmp3 ,ptmp2, ERROR_SCALE);\n");
  fprintf(outfile, "       if(MyFloatA_GE_B(ptmp3, errmax)) {\n");
  fprintf(outfile, "           AssignMyFloat(errmax, ptmp3);\n");
  fprintf(outfile, "       }\n");
  fprintf(outfile, "    }\n");
  fprintf(outfile, "    DivideMyFloatA(errmax, errmax, EPSILON);\n");
  fprintf(outfile, "    if(MyFloatA_GE_B(errmax,ONE)) {\n");
  fprintf(outfile, "       ExponentiateMyFloatA(ptmp1,errmax,PSHRNK);\n");
  fprintf(outfile, "       MultiplyMyFloatA(ptmp2, ptmp1, h);\n");
  fprintf(outfile, "       MultiplyMyFloatA(ptmp3, ptmp2, SAFETY);\n");
  fprintf(outfile, "       AssignMyFloat(h, ptmp3);\n");
  fprintf(outfile, "    } else {\n");
  fprintf(outfile, "       if(MyFloatA_GE_B(errmax, ERRCON)) {\n");
  fprintf(outfile, "          ExponentiateMyFloatA(ptmp1,errmax,PGROW);\n");
  fprintf(outfile, "          MultiplyMyFloatA(ptmp2, ptmp1, h);\n");
  fprintf(outfile, "          MultiplyMyFloatA(hnext, ptmp2, SAFETY);\n");
  fprintf(outfile, "       } else {\n");
  fprintf(outfile, "          AddMyFloatA(ptmp1, h, h);\n");
  fprintf(outfile, "          AddMyFloatA(hnext, ptmp1, ptmp1);\n");  
  fprintf(outfile, "       }\n");
  fprintf(outfile, "       AssignMyFloat(*ht, hnext);;      \n");
  fprintf(outfile, "       done = 1;\n");
  fprintf(outfile, "   }\n");
  fprintf(outfile, "  } while(!done);\n\n");
  
  fprintf(outfile, "  if(ended) {\n");
  fprintf(outfile, "     for(i = 0; i< %d;i++) {\n", m);
  fprintf(outfile, "        AssignMyFloat(xx_in[i], xtmp[i]);;\n");
  fprintf(outfile, "     }\n");
  if(state_jet_vars >0) {
    fprintf(outfile, "     for(i = 0; i< %d;i++) {\n",n);
    fprintf(outfile, "        AssignJetToJet(jetIn[i], jet[i]);      \n");
    fprintf(outfile, "     }\n");
  }
  fprintf(outfile, "  } else {\n");
  fprintf(outfile, "     for(i = 0; i< %d;i++) {\n",m);
  fprintf(outfile, "        MultiplyMyFloatA(ptmp1, xtmp3[i] , FCOR);\n");
  fprintf(outfile, "        AddMyFloatA(xx_in[i], xtmp[i], ptmp1);\n");
  fprintf(outfile, "     }\n");
  if(state_jet_vars>0){
    fprintf(outfile, "     for(i=0;i<4;i++){\n");
    fprintf(outfile, "        MultiplyJetFloatA(jtmp2,jet3[i], FCOR);      \n");
    fprintf(outfile, "        AddJetJetA(jetIn[i], jet[i], jtmp2);\n");
    fprintf(outfile, "     }\n");
  }
  fprintf(outfile, "  }\n");  
  fprintf(outfile, "  AssignMyFloat(*t_in, ttmp);\n");
  fprintf(outfile, "  return ended;\n");
  fprintf(outfile, "}\n");
}
