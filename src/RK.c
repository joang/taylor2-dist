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

#define RK_C
#include "Header.h"

void genRungeKuttaMacros(){
  fprintf(outfile, "\n\n/**************************** start RungeKutta steppers *********************/\n");  
  fprintf(outfile, "#ifndef _N_DIM_\n");
  fprintf(outfile, "#define _N_DIM_ %d\n", neqns - nonautonomous);
  if (state_jet_vars > 0) fprintf(outfile, "#define _J_DIM_ %d\n", state_jet_vars);
  else if (num_cloud_vars > 0) fprintf(outfile, "#define _J_DIM_ %d\n", num_cloud_vars);
  else fprintf(outfile, "#define _J_DIM_ 0\n");
  fprintf(outfile, "#endif\n\n");
}

void genRungeKuttaWrapper(){ 

  int m = neqns - nonautonomous, n= state_jet_vars;
  
  fprintf(outfile, "\n\
/*\n\
 * Stepper for Runge-Kutta-Fehlberg methods. One can choose the method\n\
 * by specifying the *order parameter. Implemented methods are: 4,5,6,7,8 and 9.\n\
 *\n\
 * The initial condition (t_in,xx_in) is changed when the stepper returns.\n\
 *\n\
 *\n\
 *  Parameters:\n\
 *\n\
 *  MY_FLOAT *t_in,   value of time.\n\
 * \n\
 *  MY_FLOAT *xx_in,  value of state variables.\n\
 *\n\
 *  int dir           direction of time, 1 or -1.  \n\
 *\n\
 * int step_ctl, step size control. 1 for adaptive step size, 0 for fixed step size.\n\
 *  \n\
 *  double log10abserr, error tolerance. When step_ctl !=0, it is used to select the step size. \n\
 *  double log10relerr, not used. \n\
 *\n\
 * MY_FLOAT *endtime, integration stops when t reaches *endtime.\n\
 * \n\
 * MY_FLOAT *ht, step size. If step_ctl !=0, it returns the new step size.\n\
 *\n\
 * int *order,  select RK method. Valid values are: 4,5,6,7,8,9, 76,87,89.\n\
 *\n\
 * MY_JET *jetIn, value of jet vars. Used only when jet is requested.\n\
 *\n\
 * double *err_ret,  if not NULL, return the estimated abs error of the stepper.\n\
*/\n									\
\n\
int RungeKutta_step_%s(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl,\n\
		       double log10abserr, double log10relerr,\n\
		       MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn, double *err_ret) {\n\
\n\
  static int initialized=0;\n\
  static  MY_FLOAT step, t1, t2, t, **k, kx[13*%d], x0[%d], nr_ret, er_ret, ns, hn ; //m\n\
#pragma omp threadprivate(initialized,step, t1, t2, t, k, kx, nr_ret, er_ret, ns, hn, initialized)\n\
\n\
", suffix, m, m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  static MY_JET **kjet, kjetx[13*%d], j0[%d];  //n\n\
#pragma omp threadprivate(kjet, kjetx)\n\
\n\
",n,n);
   }
   fprintf(outfile, "\n\
  MY_JET  **jet_arg = NULL, *jet_in_arg = NULL;\n\
  int i,j,flag=0;\n\
  double nr, ee, ea, es, s, tol, FC, LOG10, t_double, h_double;\n\
  int IS, rk = *order;\n\
\n\
  LOG10=2.3025850929940457;  \n\
  FC=0.9;\n\
  IS=3;\n\
  tol=exp(log10abserr * LOG10);\n\
\n\
  if(rk != 4 && rk != 5 && rk != 6 && rk != 7 && rk != 8 && rk != 9 && rk != 76 && rk != 87 && rk != 89 ) {\n\
    fprintf(stderr, \"RK=%%d is invalid. Valid values are: 4,5,6,7,8,9, 76, 87 and 89\\n\", rk);\n\
    exit(1);\n\
  }\n\
  \n\
  switch(rk) {\n\
  case 4:\n\
  case 5:\n\
    rk45_init_constants();\n\
    break;\n\
  case 6:\n\
  case 76:\n\
    rk67_init_constants();\n\
    break;\n\
  case 7:\n\
  case 87:\n\
    rk78_init_constants();\n\
    break;\n\
  case 8:\n\
  case 89:    \n\
  case 9:\n\
    rk89_init_constants();\n\
    break;\n\
  default:\n\
    break;\n\
  }\n\
\n\
");
   
   fprintf(outfile, "\n\
  if(!initialized){\n\
    InitMyFloat(step); InitMyFloat(t);InitMyFloat(t1);InitMyFloat(t2);InitMyFloat(nr_ret); InitMyFloat(er_ret); InitMyFloat(ns); InitMyFloat(hn);\n\
    for(j = 0; j< %d; j++) {InitMyFloat(x0[j]);} //m    \n\
    for(j = 0; j< 13*%d; j++) {InitMyFloat(kx[j]);} //m\n\
    k=(MY_FLOAT **)malloc(13 * sizeof(MY_FLOAT *));\n\
   if(k == NULL) {fprintf(stderr, \"Out of memory allocating k.\\n\"); exit(1);}\n\
    for(i=0; i< 13; i++) k[i]=&(kx[i*%d]);\n\
\n\
",m,m,m);
   
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    for(j = 0; j< %d; j++) {InitJet(j0[j]);}  //n    \n\
    for(j = 0; j< 13* %d; j++) {InitJet(kjetx[j]);}  //n\n\
    kjet=(MY_JET **)malloc(13 * sizeof(MY_JET *));\n\
    if(kjet == NULL) {fprintf(stderr, \"Out of memory allocating kjet.\\n\"); exit(1);}\n\
    for(i=0; i<13; i++) kjet[i]=&(kjetx[i*%d]);\n\
\n\
",n,n,n);
   }
   
   fprintf(outfile, "\n\
    initialized=1;\n\
  }\n\
\n\
");
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  jet_arg = kjet;\n\
  jet_in_arg = jetIn; \n\
\n\
");
   }
   fprintf(outfile, "\n\
  \n\
  \n\
  //t=*t_in;\n\
  //h=*ht;\n\
  AssignMyFloat(t, *t_in);\n\
  AssignMyFloat(step, *ht);\n\
\n\
  //  if(h>0) {\n\
  //    if (t+h > *endtime) {\n\
  //      h=*endtime-t;\n\
  //      flag=1;\n\
  //    }\n\
  //  } else {\n\
  //    if (t+h < *endtime) {\n\
  //      h=*endtime-t;\n\
  //      flag=1;\n\
  //    }\n\
  //  }\n\
  if(dir > 0) { //forward\n\
    AddMyFloatA(t1, t, step);\n\
    if(MyFloatA_GE_B(t1,*endtime)) {\n\
      flag=1;\n\
      SubtractMyFloatA(t2,*endtime, t);\n\
      AssignMyFloat(step, t2);\n\
      AssignMyFloat(*ht, step);\n\
    }\n\
  } else { //backword\n\
    SubtractMyFloatA(t1, t, step);\n\
    if(MyFloatA_LE_B(t1,*endtime)) {\n\
      flag=1;\n\
      SubtractMyFloatA(t2, t, *endtime);\n\
      AssignMyFloat(step, t2);\n\
      AssignMyFloat(*ht, step);\n\
    }\n\
    NegateMyFloatA(step, step); // make it forward with negative step\n\
  }\n\
\n\
  // compute_ks(t,xx_in,step,k,jetIn, kjet);  \n\
");

   fprintf(outfile, "\n\
  switch(rk) {\n\
  case 4:\n\
  case 5:\n\
    compute_ks45_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
    break;\n\
  case 6:\n\
  case 76:\n\
    compute_ks67_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
    break;\n\
  case 7:\n\
  case 87:    \n\
    compute_ks78_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
    break;\n\
  case 8:\n\
  case 89:\n\
  case 9:\n\
    compute_ks89_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
    break;\n\
  default:\n\
    break;\n\
  }\n\
\n\
", suffix,suffix,suffix,suffix);

   fprintf(outfile, "\n\
  //  if(step_ctl == 0)\n\
  //    {\n\
  //      if (aer != NULL) *aer=error_rk(n,k);\n\
  //      step_rk(xx_in,n,k);\n\
  //      *t_in=t+h;\n\
  //      return flag;\n\
  //    }\n\
\n\
  for (j = 0; j < %d; ++j) { // m\n\
    AssignMyFloat(x0[j], xx_in[j]);    \n\
  }\n\
  \n\
",m);

   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  for (j = 0; j < %d; ++j) { // n\n\
    AssignJetToJet(j0[j], jetIn[j]);    \n\
  }\n\
  \n\
",n);
   }
   fprintf(outfile, "\n\
\n\
  switch(rk) {\n\
  case 4:\n\
    step_rk4_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);\n\
    break;\n\
  case 5:\n\
    step_rk5_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
    break;\n\
  case 6:\n\
    step_rk6_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
    break;\n\
  case 76:\n\
    step_rk7_67_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
    break;\n\
  case 7:\n\
    step_rk7_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);\n\
    break;\n\
  case 87:\n\
    step_rk8_78_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
    break;\n\
  case 8:\n\
  case 89:\n\
    step_rk8_89_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);\n\
    break;\n\
  case 9:\n\
    step_rk9_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
    break;\n\
  default:\n\
    break;\n\
  }\n\
  \n\
", suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix);

   fprintf(outfile, "\n\
  if(step_ctl == 0) {\n\
    AddMyFloatA(t1,t, step);\n\
    AssignMyFloat(*t_in, t1);    \n\
    return flag;\n\
  }\n\
\n\
  \n\
  MyFloatToDouble(nr,nr_ret);\n\
  ee=(1.0+nr)*tol/2.0;\n\
\n\
");

   fprintf(outfile, "\n\
  switch(rk) {\n\
  case 4:\n\
  case 5:\n\
    error_rk45_%s(k, jet_arg, &er_ret);        \n\
    break;\n\
  case 6:\n\
  case 76:\n\
    error_rk67_%s(k, jet_arg, &er_ret);        \n\
    break;\n\
  case 7:\n\
  case 87:\n\
    error_rk78_%s(k, jet_arg, &er_ret);        \n\
    break;\n\
  case 8:\n\
  case 89:\n\
  case 9:\n\
    error_rk89_%s(k, jet_arg, &er_ret);    \n\
    break;\n\
  default:\n\
    break;\n\
  }\n\
  \n\
",suffix,suffix,suffix,suffix);

   fprintf(outfile, "\n\
  MyFloatToDouble(es,er_ret);  \n\
\n\
  ea=es+1.e-16*nr;\n\
  switch(rk) {\n\
  case 4:\n\
  case 5:\n\
    s=FC*pow(ee/ea,0.2);\n\
    break;\n\
  case 6:\n\
  case 76:\n\
    s=FC*exp(log(ee/ea)/7.e0);\n\
    break;\n\
  case 7:\n\
  case 87:\n\
    s=FC*exp(log(ee/ea)/8.e0);\n\
    break;\n\
  case 8:\n\
  case 89:\n\
  case 9:\n\
    s=FC*exp(log(ee/ea)/9.e0);\n\
    break;\n\
  default:\n\
    break;\n\
  }\n\
  if (2.0 < s) s=2.0;\n\
\n\
  // hn=h*s;  \n\
  MakeMyFloatA(ns,s);\n\
  MultiplyMyFloatA(hn, step, ns);\n\
\n\
  if(ea < ee) {\n\
    //if (aer != NULL) *aer=ea;\n\
    //*t_in=t+h;\n\
    //*ht=hn;\n\
    if(err_ret != NULL) *err_ret=ea;\n\
    AddMyFloatA(*t_in, t, step);\n\
    AssignMyFloat(*ht, hn);\n\
    return flag;\n\
  }\n\
\n\
  // seek an appropriate step size \n\
");

   fprintf(outfile, "\n\
  for(i=0; i<IS; i++) {\n\
    for(j = 0; j < %d; ++j) { //m\n\
      AssignMyFloat(xx_in[j], x0[j]);\n\
    }\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    for(j = 0; j < %d; ++j) { //n\n\
      AssignJetToJet(jetIn[j], j0[j]);\n\
    }\n\
",n);
   }
   
   fprintf(outfile, "\n\
    //h=hn;    \n\
    AssignMyFloat(step, hn);\n\
\n\
    switch(rk) {\n\
    case 4:\n\
    case 5:\n\
      compute_ks45_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
      break;\n\
    case 6:\n\
    case 76:\n\
      compute_ks67_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
      break;\n\
    case 7:\n\
    case 87:    \n\
      compute_ks78_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
      break;\n\
    case 8:\n\
    case 89:\n\
    case 9:\n\
      compute_ks89_%s(t,xx_in, step, k, jet_in_arg, jet_arg);\n\
      break;\n\
    default:\n\
      break;\n\
    }\n\
\n\
    switch(rk) {\n\
    case 4:\n\
      step_rk4_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);\n\
      break;\n\
    case 5:\n\
      step_rk5_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
      break;\n\
    case 6:\n\
      step_rk6_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
      break;\n\
    case 76:\n\
      step_rk7_67_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
      break;\n\
    case 7:\n\
      step_rk7_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);\n\
      break;\n\
    case 87:\n\
      step_rk8_78_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
      break;\n\
    case 8:\n\
    case 89:\n\
      step_rk8_89_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);\n\
      break;\n\
    case 9:\n\
      step_rk9_%s(xx_in,k, jet_in_arg, jet_arg, &nr_ret);      \n\
      break;\n\
    default:\n\
      break;\n\
    }\n\
    \n\
    MyFloatToDouble(nr, nr_ret);\n\
    ee=(1.0+nr)*tol/2.0;\n\
\n\
    switch(rk) {\n\
    case 4:\n\
    case 5:\n\
      error_rk45_%s(k, jet_arg, &er_ret);        \n\
      break;\n\
    case 6:\n\
    case 76:\n\
      error_rk67_%s(k, jet_arg, &er_ret);        \n\
      break;\n\
    case 7:\n\
    case 87:\n\
      error_rk78_%s(k, jet_arg, &er_ret);        \n\
      break;\n\
    case 8:\n\
    case 89:\n\
    case 9:\n\
      error_rk89_%s(k, jet_arg, &er_ret);    \n\
      break;\n\
    default:\n\
      break;\n\
    }\n\
\n\
",suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix,suffix);

   fprintf(outfile, "\n\
    MyFloatToDouble(es,er_ret);      \n\
    ea=es+1.e-16*nr;\n\
    switch(rk) {\n\
    case 4:\n\
    case 5:\n\
      s=FC*pow(ee/ea,0.2);\n\
      break;\n\
    case 6:\n\
    case 76:\n\
      s=FC*exp(log(ee/ea)/7.e0);\n\
      break;\n\
    case 7:\n\
    case 87:\n\
      s=FC*exp(log(ee/ea)/8.e0);\n\
      break;\n\
    case 8:\n\
    case 89:\n\
    case 9:\n\
      s=FC*exp(log(ee/ea)/9.e0);\n\
      break;\n\
    default:\n\
      break;\n\
    }\n\
    if(1.0 < s) s=1.0;\n\
    //hn=h*s;\n\
    MakeMyFloatA(ns,s);\n\
    MultiplyMyFloatA(hn, step, ns);\n\
    \n\
    if(ea < ee) {\n\
      if(err_ret != NULL) *err_ret = ea;\n\
      //*t_in=t+h;\n\
      //*ht=hn;\n\
      AddMyFloatA(*t_in, t, step);\n\
      AssignMyFloat(*ht, hn);      \n\
      return 0;\n\
    }\n\
  }\n\
\n\
  if(err_ret == NULL) {\n\
    fprintf(stderr, \"RungeKutta%%d error: unable to choose a step size to satisfy the required error tolerance.\\n\", rk);\n\
    MyFloatToDouble(t_double,t);\n\
    MyFloatToDouble(h_double,step);\n\
    fprintf(stderr, \"Time=%%f. Last step=%%f, estimated error=%%e, actural threshold=%%e\\n\",t_double,h_double,ea,ee);\n\
    exit(1);\n\
  }\n\
  //*at =t+h;\n\
  //*ht=hn;\n\
  //*aer=ea;\n\
  MyFloatToDouble(t_double,t);\n\
  MyFloatToDouble(h_double,hn);\n\
  fprintf(stderr, \"RungeKutta%%d: Unable to choose step to satisfy the required error tolerance.  Time=%%e, step=%%e.  Estimated abs_error=%%e rel_error=%%e, threshold=%%e.\\n\",rk,t_double,h_double,ea,ea/nr,ee);\n\
  AddMyFloatA(*t_in, t, step);\n\
  AssignMyFloat(*ht, hn);\n\
  if(err_ret) *err_ret = ea;\n\
  return 0;\n\
}\n\
\n\
");
   fprintf(outfile, "\n\n/**************************** end RungeKutta steppers *********************/\n");  
}
