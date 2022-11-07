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

static char *oneStepCode =
"\
\n\
/*\n\
next line defines the largest power of 2 such that 2^(LEXP2) and\n\
2^(-LEXP2) do not overflow/underflow the double arithmetic of your\n\
computer.\n\
*/\n\
#define LEXP2 1023 \n\
\n\
#define DEBUG_LEVEL 0 /* to print some internal information */\n\
\n\
int taylor_step__ODE_NAME__(MY_FLOAT *ti,\n\
                 MY_FLOAT *x,\n\
                 int      dir,\n\
                 int      step_ctl,\n\
                 double   log10abserr,\n\
                 double   log10relerr,\n\
                 MY_FLOAT *endtime,\n\
                 MY_FLOAT *ht,\n\
                 int      *order,\n\
                 MY_JET   *jetInOut)\n\
/*\n\
 * single integration step with taylor method. the parameters are:\n\
 *\n\
 * ti: on input:  time of the initial condition\n\
 *     on output: new time\n\
 *\n\
 * x:  on input:  initial condition\n\
 *     on output: new condition, corresponding to the (output) time ti\n\
 *\n\
 * dir: flag to integrate forward or backwards.\n\
 *     1: forward\n\
 *    -1: backwards\n\
 *     WARNING: this flag is ignored if step_ctl (see below) is set to 0.\n\
 *\n\
 * step_ctl: flag for the step size control. the possible values are:\n\
 *     0: no step size control, so the step and order are provided by\n\
 *        the user. the parameter ht is used as step, and the parameter\n\
 *        order (see below) is used as the order.\n\
 *     1: standard stepsize control. it uses an approximation to the\n\
 *        optimal order and to the radius of convergence of the series\n\
 *        to approximate the 'optimal' step size. It tries to keep\n\
 *        either the absolute or the relative errors below the given\n\
 *        values. See the paper for more details.\n\
 *     2: as 1, but adding an extra condition on the stepsize h: the\n\
 *        terms of the series --after being multiplied by the suitable\n\
 *        power of h-- cannot grow.\n\
 *    -1: user defined stepsize control. the code has to be included\n\
 *        in the routine compute_timestep_user_defined (you can find\n\
 *        this routine below). The user should also provide code for\n\
 *        the selection of degree (see the function\n\
 *        compute_order_user_defined below).\n\
 *\n\
 * log10abserr: decimal log of the absolute accuracy. the routine\n\
 *     tries to keep either the absolute or the relative errors below\n\
 *     the given thresholds.\n\
 *\n\
 * log10relerr: decimal log of the relative accuracy. the routine\n\
 *     tries to keep either the absolute or the relative errors below\n\
 *     the given thresholds.\n\
 *\n\
 * endtime: if NULL, it is ignored. if step_ctl (see above) is set\n\
 *     to 0, it is also ignored. otherwise, if the step size is too\n\
 *     large, it is reduced so that the new time ti is exactly endtime\n\
 *     (in that case, the function returns 1).\n\
 *\n\
 * ht: on input:  ignored/used as a time step (see parameter step_ctl)\n\
 *     on output: time step used if the pointer is not NULL\n\
 *\n\
 * order: degree of the taylor expansion.\n\
 *        input: this parameter is only used if step_ctl is 0,\n\
 *               or if you add the code for the case step_ctl=3.\n\
 *               its possible values are:\n\
 *               < 2: the program will select degree 2 (if step_ctl is 0).\n\
 *               >=2: the program will use this degree (if step_ctl is 0).\n\
 *        ouput: degree used if the pointer is not NULL\n\
 *\n\
 * jetInOut: on input: the value of all declared jet variables\n\
 *           on output: new value of the jet variable, corresponding to the new time\n\
 *\n\
 * return value:\n\
 * 0: ok.\n\
 * 1: ok, and ti=endtime.  */\n\
{\n\
  MY_FLOAT **taylor_coefficients__ODE_NAME__(MY_FLOAT, MY_FLOAT*, int);\n\
  MY_FLOAT **taylor_coefficients__ODE_NAME__A(MY_FLOAT, MY_FLOAT*, int, int);\n\
  int compute_order_1__ODE_NAME__(double, double, double, int*);\n\
  int comp_order_other__ODE_NAME__(double, double, double);\n\
  double compute_stepsize_1__ODE_NAME__(MY_FLOAT**, MY_JET**, int, double, int);\n\
  double compute_stepsize_2__ODE_NAME__(MY_FLOAT**, MY_JET**, int, double, int);\n\
  double comp_stepsize_other__ODE_NAME__(MY_FLOAT**, MY_JET**, int, int, double, double, double);\n\
\n\
  static MY_FLOAT **s,h,mtmp;\n\
  static MY_JET jtmp,jtmp1, **jetJetOut;\n\
  double xi,xnorm,dh;\n\
  int i,j,k,nt,flag_endtime,flag_err;\n\
  static int init=0;\n\
#pragma omp threadprivate(s,h,mtmp,init)\n\
#pragma omp threadprivate(jtmp,jtmp1,jetJetOut)\n\
\n\
  if (init == 0) /* initialization of MY_FLOAT and MY_JET variables */\n\
    {\n\
      init=1;\n\
      InitMyFloat(h);\n\
      InitMyFloat(mtmp);\n\
      xInitUpJet();\n\
      xInitJet(jtmp);\n\
      xInitJet(jtmp1);\n\
    }\n\
/*\n\
  sup norm of the initial condition (jet included)\n\
*/\n\
  xnorm=0;\n\
  if (step_ctl != 0)\n\
    {\n\
#if _J_DIM_ != 0\n\
    for (i=0; i<_J_DIM_; i++)\n\
      {\n\
        xNormJet(mtmp,jetInOut[i]);\n\
        MyFloatToDouble(xi,mtmp);\n\
        xi=fabs(xi);\n\
        if (xi > xnorm) xnorm=xi;\n\
      }\n\
#endif\n\
      for (i=_J_DIM_; i<_N_DIM_; i++)\n\
      {\n\
	MyFloatToDouble(xi,x[i]);\n\
	xi=fabs(xi);\n\
	if (xi > xnorm) xnorm=xi;\n\
      }\n\
    }\n\
/*\n\
  we determine the degree of the taylor expansion.\n\
  this value will be stored in the variable nt.\n\
  the flag flag_err returns a value indicating if we are using an\n\
  absolute error estimate (value 1) or a relative one (value 2).\n\
*/\n\
  switch(step_ctl)\n\
    {\n\
    case 0: /* no step size control, fixed degree; both given by the user */\n\
      if (order==NULL)\n\
      {\n\
        fprintf(stderr,\"step_ctl=%d needs an input order\\n\",step_ctl);\n\
        exit(0);\n\
      }\n\
      nt=(*order<2)? 2: *order; /* 2 is the minimum order allowed */\n\
      break;\n\
    case 1:\n\
      nt=compute_order_1__ODE_NAME__(xnorm,log10abserr,log10relerr,&flag_err);\n\
      break;\n\
    case 2:\n\
      nt=compute_order_1__ODE_NAME__(xnorm,log10abserr,log10relerr,&flag_err);\n\
      break;\n\
    case -1:\n\
      nt=comp_order_other__ODE_NAME__(xnorm,log10abserr,log10relerr);\n\
      break;\n\
    default:\n\
      fprintf(stderr, \"taylor_step: undefined step size control.\\n\");\n\
      fprintf(stderr, \"you must choose an existing step size control\\n\");\n\
      fprintf(stderr, \"method or supply a step size control procedure.\\n\");\n\
      exit(0);\n\
    }\n\
  if (order!=NULL) *order=nt;\n\
/*\n\
  computation of the jet of derivatives up to order nt\n\
*/\n\
  if(step_ctl != 0) {\n\
    s=taylor_coefficients__ODE_NAME___A(*ti,x,nt,1, jetInOut, &jetJetOut);\n\
  } else {\n\
    s=taylor_coefficients__ODE_NAME___A(*ti,x,nt,0, jetInOut, &jetJetOut); \n\
 }\n\
\n\
/*\n\
  selection of the routine to compute the time step. the value\n\
  step_ctl=3 has been reserved for the user, in case she/he wants to\n\
  code a different method.\n\
*/\n\
  switch(step_ctl)\n\
    {\n\
    case 0: /* no step size control (fixed step size, given by the user) */\n\
      if (ht==NULL)\n\
      {\n\
        fprintf(stderr,\"step_ctl=%d needs an input stepsize\\n\",step_ctl);\n\
        exit(0);\n\
      }\n\
      AssignMyFloat(h,*ht);\n\
      break;\n\
    case 1:\n\
      dh=compute_stepsize_1__ODE_NAME__(s,jetJetOut,nt,xnorm,flag_err);\n\
      MakeMyFloatA(h, dh);\n\
      break;\n\
    case 2:\n\
      dh=compute_stepsize_2__ODE_NAME__(s,jetJetOut,nt,xnorm,flag_err);\n\
      MakeMyFloatA(h, dh);\n\
      break;\n\
    case -1:\n\
      dh=comp_stepsize_other__ODE_NAME__(s,jetJetOut,_N_DIM_,nt,xnorm,log10abserr,log10relerr);\n\
      MakeMyFloatA(h, dh);\n\
      break;\n\
    default:\n\
      fprintf(stderr, \"taylor_step: undefined step size control.\\n\");\n\
      fprintf(stderr, \"You must choose an existing step size control\\n\");\n\
      fprintf(stderr, \"method or supply a step size control procedure.\\n\");\n\
      exit(0);\n\
    }\n\
/*\n\
  if step_ctl != 0, we adjust the sign of the computed stepsize.\n\
*/\n\
  flag_endtime=0;\n\
  if (step_ctl != 0)\n\
    {\n\
      if (dir == -1) { NegateMyFloatA(mtmp,h); AssignMyFloat(h, mtmp);} \n\
/*\n\
      we compare *ti+h with endtime. we modify h if necessary.\n\
*/\n\
      if (endtime != NULL)\n\
        {\n\
          AddMyFloatA(mtmp,h,*ti);\n\
          if (dir == 1) /* time goes forward */\n\
            {\n\
              if (MyFloatA_GE_B(mtmp,*endtime))\n\
                {\n\
                  SubtractMyFloatA(h,*endtime,*ti);\n\
                  flag_endtime=1;\n\
                }\n\
            }\n\
            else /* time goes backwards */\n\
            {\n\
              if (MyFloatA_GE_B(*endtime,mtmp))\n\
                {\n\
                  SubtractMyFloatA(h,*endtime,*ti);\n\
                  flag_endtime=1;\n\
                }\n\
            }\n\
        }\n\
    }\n\
/*\n\
  next lines are the summation of the taylor series (horner's method)\n\
*/\n\
  j=nt-1;\n\
#if _J_DIM_ != 0\n\
  for(i=0; i<_J_DIM_; i++)\n\
  {\n\
    xAssignJetToJet(jetInOut[i],jetJetOut[i][nt]);\n\
    for(k=j; k>=0; k--)\n\
    {\n\
      xMultiplyFloatJetA(jtmp, h, jetInOut[i]);\n\
      xAddJetJetA(jetInOut[i], jtmp, jetJetOut[i][k]);\n\
    }\n\
    xAssignJetToFloat(x[i],jetInOut[i]);\n\
  }\n\
#endif\n\
  for(i=_J_DIM_; i<_N_DIM_; i++)\n\
  {\n\
    AssignMyFloat(x[i],s[i][nt]);\n\
    for(k=j; k>=0; k--)\n\
    {\n\
      MultiplyMyFloatA(mtmp, h, x[i]);\n\
      AddMyFloatA(x[i], mtmp, s[i][k]);\n\
    }\n\
  }\n\
/*\n\
  finally, we set the values of the parameters *ht and *ti.\n\
*/\n\
  if (ht!=NULL) AssignMyFloat(*ht,h);\n\
  if (flag_endtime == 0)\n\
    {\n\
      AssignMyFloat(mtmp, *ti);\n\
      AddMyFloatA(*ti, mtmp, h);\n\
    }\n\
    else\n\
    {\n\
      AssignMyFloat(*ti,*endtime);\n\
    }\n\
  return(flag_endtime);\n\
}\n\
int compute_order_1__ODE_NAME__(double xnorm, double log10abserr, double log10relerr, int* flag_err)\n\
/*\n\
 * this is to determine the 'optimal' degree of the taylor expansion.\n\
 *\n\
 * parameters:\n\
 * xnorm: norm of the initial condition\n\
 * log10abserr: base-10 log of the absolute error required\n\
 * log10relerr: base-10 log of the relative error required\n\
 * flag_err:    flag. returns 1 if absolute error is used\n\
 *                    returns 2 if relative error is used\n\
 *\n\
 * returned value: 'optimal' degree.\n\
*/\n\
{\n\
  double log10eps,tmp;\n\
  int nt;\n\
\n\
  log10eps=log10abserr;\n\
  *flag_err=1;\n\
  if (xnorm != (double)0.0)\n\
    {\n\
      tmp=log10relerr+log10(xnorm);\n\
      if (tmp > log10abserr) {log10eps=log10relerr; *flag_err=2;}\n\
    }\n\
/*\n\
  we use 1.16 for the value 0.5*log(10)=1.151292546497...\n\
*/\n\
  nt=(int)(1.5-1.16*log10eps);\n\
  if (nt < 2) nt=2; /* this is the minimum order accepted */\n\
\n\
#if DEBUG_LEVEL > 0\n\
      fprintf(stderr, \"taylor_step: order is %d\\n\",nt);\n\
#endif\n\
\n\
  return(nt);\n\
}\n\
double compute_stepsize_1__ODE_NAME__(MY_FLOAT **s, MY_JET **jet, int nt, double xnorm, int flag_err)\n\
/*\n\
 * it looks for a step size for an expansion up to order nt. this\n\
 * function requires that nt is the value computed by\n\
 * compute_order_1_\n\
 */\n\
{\n\
  double double_log_MyFloat__ODE_NAME__(MY_FLOAT x);\n\
  static MY_FLOAT z,v1,v2;\n\
  static MY_FLOAT of,uf;\n\
  double lnv1,lnv2,r,lnro1,lnro2,lnro;\n\
  int i;\n\
  static int init=0;\n\
#pragma omp threadprivate(z,v1,v2,of,uf,init)\n\
\n\
  if (init == 0)\n\
    {\n\
      init=1;\n\
      InitMyFloat(z);\n\
      InitMyFloat(v1);\n\
      InitMyFloat(v2);\n\
      InitMyFloat(of);\n\
      InitMyFloat(uf);\n\
\n\
      r=pow((double)2,(double)LEXP2);\n\
      MakeMyFloatA(of,r);\n\
      r=pow((double)2,(double)(-LEXP2));\n\
      MakeMyFloatA(uf,r);\n\
    }\n\
/*\n\
  we compute the sup norm of the last two coefficients of the taylor\n\
  series, and we store them into v1 and v2.\n\
*/\n\
  MakeMyFloatA(v1,0);\n\
  MakeMyFloatA(v2,0);\n\
#if _J_DIM_ != 0\n\
  for (i=0; i<_J_DIM_; i++)\n\
  {\n\
    xNormJet(z,jet[i][nt-1]);\n\
    if (MyFloatA_GT_B(z,v1)) AssignMyFloat(v1,z);\n\
    xNormJet(z,jet[i][nt]);\n\
    if (MyFloatA_GT_B(z,v2)) AssignMyFloat(v2,z);\n\
  }\n\
#endif\n\
  for(i=_J_DIM_; i<_N_DIM_; i++)\n\
  {\n\
    fabsMyFloatA(z,s[i][nt-1]);\n\
    if (MyFloatA_GT_B(z,v1)) AssignMyFloat(v1,z);\n\
    fabsMyFloatA(z,s[i][nt]);\n\
    if (MyFloatA_GT_B(z,v2)) AssignMyFloat(v2,z);\n\
  }\n\
/*\n\
  computation of the step size. we need the logs of v1 and v2, in\n\
  double precision (there is no need for extended precision). the idea\n\
  is to assign these variables to double variables and then to use the\n\
  standard log function. before doing that, we have to be sure that v1\n\
  can be assigned to a double without under or overflow. for this\n\
  reason we will check for this condition and, if it fails, we will\n\
  call an specific function for this case.\n\
*/\n\
  if (MyFloatA_LE_B(v1,of) && MyFloatA_GE_B(v1,uf))\n\
    {\n\
      MyFloatToDouble(r,v1);\n\
      lnv1=log(r);\n\
    }\n\
    else\n\
    {\n\
      lnv1=double_log_MyFloat__ODE_NAME__(v1);\n\
    }\n\
  if (MyFloatA_LE_B(v2,of) && MyFloatA_GE_B(v2,uf))\n\
    {\n\
      MyFloatToDouble(r,v2);\n\
      lnv2=log(r);\n\
    }\n\
    else\n\
    {\n\
      lnv2=double_log_MyFloat__ODE_NAME__(v2);\n\
    }\n\
/*\n\
  if flag_err == 2, this means that we are using a relative error control.\n\
*/\n\
  if (flag_err == 2)\n\
    {\n\
      r = -log10(xnorm);\n\
      lnv1 += r;\n\
      lnv2 += r;\n\
    }\n\
 lnro1= -lnv1/(nt-1);\n\
 lnro2= -lnv2/nt;\n\
 lnro=(lnro1 < lnro2)? lnro1: lnro2;\n\
\n\
 r=exp(lnro-2-0.7/(nt-1)); /* exp(-0.7/(nt-1)) is a security factor */\n\
  return(r);\n\
}\n\
double compute_stepsize_2__ODE_NAME__(MY_FLOAT **s, MY_JET **jet, int nt, double xnorm, int flag_err)\n\
/*\n\
 * it looks for a step size for an expansion up to order nt. this\n\
 * function requires that nt is the value computed by\n\
 * compute_order_1_. it also tries to reduce cancellations of big\n\
 * terms in the summation of the taylor series.\n\
 */\n\
{\n\
  double compute_stepsize_1__ODE_NAME__(MY_FLOAT**, MY_JET**, int, double, int);\n\
  static MY_FLOAT h,hj,r,z,a,normj;\n\
  double c,rtmp,dh;\n\
  int i,j;\n\
  static int init=0;\n\
#pragma omp threadprivate(h,hj,r,z,a,normj,init)\n\
\n\
  if (init == 0)\n\
    {\n\
      init=1;\n\
      InitMyFloat(h); \n\
      InitMyFloat(hj);\n\
      InitMyFloat(r);\n\
      InitMyFloat(z);\n\
      InitMyFloat(a);\n\
      InitMyFloat(normj);\n\
    }\n\
/*\n\
  we compute the step size according to the first algorithm\n\
*/\n\
  dh=compute_stepsize_1__ODE_NAME__(s,jet,nt,xnorm,flag_err);\n\
  MakeMyFloatA(h,dh);\n\
/*\n\
  next lines select a value (z), that will be used to control the size\n\
  of the terms of the Taylor series.\n\
*/\n\
  if (flag_err == 1) {\n\
     MakeMyFloatA(z, 1.0);\n\
  } else if (flag_err == 2) {\n\
    MakeMyFloatA(z,xnorm);\n\
  } else\n\
    {\n\
      printf(\"compute_stepsize_2 internal error. flag_err: %d\\n\",flag_err);\n\
      exit(1);\n\
    }\n\
/*\n\
  next loop checks if the sup norm of the terms in the Taylor series are\n\
  lower than z. if a term is greater than z, the step size h is reduced.\n\
*/\n\
  MakeMyFloatA(hj,(double)1.0);\n\
\n\
  for(j=1; j<=nt; j++)\n\
  {\n\
    MultiplyMyFloatA(r,h,hj);\n\
    AssignMyFloat(hj,r);\n\
\n\
    MakeMyFloatC(normj,\"0\", (double)0);\n\
#if _J_DIM_ != 0\n\
    for (i=0; i<_J_DIM_; i++)\n\
    {\n\
      xNormJet(a,jet[i][j]);\n\
      if (MyFloatA_GT_B(a,normj)) AssignMyFloat(normj,a);\n\
    }\n\
#endif\n\
    for (i=_J_DIM_; i<_N_DIM_; i++)\n\
    {\n\
      fabsMyFloatA(a,s[i][j]);\n\
      if (MyFloatA_GT_B(a,normj)) AssignMyFloat(normj,a);\n\
    }\n\
\n\
    MultiplyMyFloatA(r,normj,hj);\n\
    if (MyFloatA_LE_B(r,z)) continue;\n\
/*\n\
    we reduce h (and hj)\n\
*/\n\
    DivideMyFloatA(hj,z,normj);\n\
\n\
    DivideMyFloatA(a,r,z);\n\
    MyFloatToDouble(c,a);\n\
    c=pow(c,(double)1.e0/(double)j);\n\
    MakeMyFloatA(a,c);\n\
    DivideMyFloatA(r,h,a);\n\
    AssignMyFloat(h,r);\n\
\n\
#if DEBUG_LEVEL > 1\n\
    fprintf(stderr, \"order %2d. reducing h from %14.6e to %14.6e\\n\",j,c*h,h);\n\
#endif\n\
  }\n\
\n\
  MyFloatToDouble(rtmp,h);\n\
  return(rtmp);\n\
}\n\
\n\
double double_log_MyFloat__ODE_NAME__(MY_FLOAT x)\n\
/*\n\
 * natural log, in double precision, of a MY_FLOAT positive number.\n\
 */\n\
{\n\
  static MY_FLOAT a,tmp;\n\
  static MY_FLOAT z,of,uf;\n\
  double b,lx;\n\
  int k;\n\
  static int init=0;\n\
#pragma omp threadprivate(a,tmp,z,of,uf,init)\n\
\n\
  if (init == 0)\n\
    {\n\
      init=1;\n\
      InitMyFloat(a);\n\
      InitMyFloat(z);\n\
      InitMyFloat(of);\n\
      InitMyFloat(uf);\n\
      InitMyFloat(tmp);\n\
\n\
      b=0;\n\
      MakeMyFloatA(z,b);\n\
      b=pow((double)2,(double)LEXP2);\n\
      MakeMyFloatA(of,b);\n\
      b=pow((double)2,(double)(-LEXP2));\n\
      MakeMyFloatA(uf,b);\n\
    }\n\
\n\
  if (MyFloatA_EQ_B(x,z))\n\
    {\n\
      puts(\"double_log_MyFloat error: zero argument\");\n\
      puts(\"(this is because one of the last two terms of your taylor\");\n\
      puts(\" expansion is exactly zero)\");\n\
      exit(1);\n\
    }\n\
\n\
  AssignMyFloat(a,x);\n\
\n\
  k=0;\n\
  while(MyFloatA_LT_B(a,uf))\n\
  {\n\
    ++k;\n\
    if(k>3000){fprintf(stderr,\"double_log_MyFloat overflow: %d\\n\", k); exit(1);}\n\
    MultiplyMyFloatA(tmp,a,of);\n\
    AssignMyFloat(a,tmp);\n\
  }\n\
  while(MyFloatA_GT_B(a,of))\n\
  {\n\
    --k;\n\
    if(k<-3000){fprintf(stderr,\"double_log_MyFloat underflow: %d\\n\", k); exit(1);}\n\
    MultiplyMyFloatA(tmp,a,uf);\n\
    AssignMyFloat(a,tmp);\n\
  }\n\
\n\
  MyFloatToDouble(b,a);\n\
/*\n\
  lx stands for log(x)\n\
*/\n\
  lx=log(b)-(LEXP2*0.69314718055994530942)*k;\n\
\n\
  return(lx);\n\
}\n\
\n\
";
