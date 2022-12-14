/* No jet */
#include "taylor.h"

/***********************************************************************
 *
 * Code generated by the TAYLOR translator.
 */

#define _N_DIM_ 2
#define _J_DIM_ 0


#define xInitUpJet() /* */
#define xInitJet(x_) /* */
#define xNormJet(a_,x_) /* */
#define xMultiplyFloatJetA(t_, f_, a_) /* */
#define xAddJetJetA(t_,a_, b_) /* */
#define xAssignJetToJet(t_,s_) /* */
#define xAssignJetToFloat(t_,s_) /* */





/*
next line defines the largest power of 2 such that 2^(LEXP2) and
2^(-LEXP2) do not overflow/underflow the double arithmetic of your
computer.
*/
#define LEXP2 1023 

#define DEBUG_LEVEL 0 /* to print some internal information */

int taylor_step_perturbation(MY_FLOAT *ti,
                 MY_FLOAT *x,
                 int      dir,
                 int      step_ctl,
                 double   log10abserr,
                 double   log10relerr,
                 MY_FLOAT *endtime,
                 MY_FLOAT *ht,
                 int      *order,
                 MY_JET   *jetInOut)
/*
 * single integration step with taylor method. the parameters are:
 *
 * ti: on input:  time of the initial condition
 *     on output: new time
 *
 * x:  on input:  initial condition
 *     on output: new condition, corresponding to the (output) time ti
 *
 * dir: flag to integrate forward or backwards.
 *     1: forward
 *    -1: backwards
 *     WARNING: this flag is ignored if step_ctl (see below) is set to 0.
 *
 * step_ctl: flag for the step size control. the possible values are:
 *     0: no step size control, so the step and order are provided by
 *        the user. the parameter ht is used as step, and the parameter
 *        order (see below) is used as the order.
 *     1: standard stepsize control. it uses an approximation to the
 *        optimal order and to the radius of convergence of the series
 *        to approximate the 'optimal' step size. It tries to keep
 *        either the absolute or the relative errors below the given
 *        values. See the paper for more details.
 *     2: as 1, but adding an extra condition on the stepsize h: the
 *        terms of the series --after being multiplied by the suitable
 *        power of h-- cannot grow.
 *    -1: user defined stepsize control. the code has to be included
 *        in the routine compute_timestep_user_defined (you can find
 *        this routine below). The user should also provide code for
 *        the selection of degree (see the function
 *        compute_order_user_defined below).
 *
 * log10abserr: decimal log of the absolute accuracy. the routine
 *     tries to keep either the absolute or the relative errors below
 *     the given thresholds.
 *
 * log10relerr: decimal log of the relative accuracy. the routine
 *     tries to keep either the absolute or the relative errors below
 *     the given thresholds.
 *
 * endtime: if NULL, it is ignored. if step_ctl (see above) is set
 *     to 0, it is also ignored. otherwise, if the step size is too
 *     large, it is reduced so that the new time ti is exactly endtime
 *     (in that case, the function returns 1).
 *
 * ht: on input:  ignored/used as a time step (see parameter step_ctl)
 *     on output: time step used
 *
 * order: degree of the taylor expansion.
 *        input: this parameter is only used if step_ctl is 0,
 *               or if you add the code for the case step_ctl=3.
 *               its possible values are:
 *               < 2: the program will select degree 2 (if step_ctl is 0).
 *               >=2: the program will use this degree (if step_ctl is 0).
 *        ouput: degree used.
 *
 * jetInOut: on input: the value of all declared jet variables
 *           on output: new value of the jet variable, corresponding to the new time
 *
 * return value:
 * 0: ok.
 * 1: ok, and ti=endtime.  */
{
  MY_FLOAT **taylor_coefficients_perturbation(MY_FLOAT, MY_FLOAT*, int);
  MY_FLOAT **taylor_coefficients_perturbationA(MY_FLOAT, MY_FLOAT*, int, int);
  int compute_order_1_perturbation(double, double, double, int*);
  int comp_order_other_perturbation(double, double, double);
  double compute_stepsize_1_perturbation(MY_FLOAT**, MY_JET**, int, double, int);
  double compute_stepsize_2_perturbation(MY_FLOAT**, MY_JET**, int, double, int);
  double comp_stepsize_other_perturbation(MY_FLOAT**, MY_JET**, int, int, double, double, double);

  static MY_FLOAT **s,h,mtmp;
  static MY_JET jtmp,jtmp1, **jetJetOut;
  double xi,xnorm,dh;
  int i,j,k,nt,flag_endtime,flag_err;
  static int init=0;
#pragma omp threadprivate(s,h,mtmp,init)
#pragma omp threadprivate(jtmp,jtmp1,jetJetOut)

  if (init == 0) /* initialization of MY_FLOAT and MY_JET variables */
    {
      init=1;
      InitMyFloat(h);
      InitMyFloat(mtmp);
      xInitJet(jtmp);
      xInitJet(jtmp1);
      xInitUpJet();
    }
/*
  sup norm of the initial condition (jet included)
*/
  xnorm=0;
  if (step_ctl != 0)
    {
#if _J_DIM_ != 0
    for (i=0; i<_J_DIM_; i++)
      {
        xNormJet(mtmp,jetInOut[i]);
        MyFloatToDouble(xi,mtmp);
        xi=fabs(xi);
        if (xi > xnorm) xnorm=xi;
      }
#endif
      for (i=_J_DIM_; i<_N_DIM_; i++)
      {
	MyFloatToDouble(xi,x[i]);
	xi=fabs(xi);
	if (xi > xnorm) xnorm=xi;
      }
    }
/*
  we determine the degree of the taylor expansion.
  this value will be stored in the variable nt.
  the flag flag_err returns a value indicating if we are using an
  absolute error estimate (value 1) or a relative one (value 2).
*/
  switch(step_ctl)
    {
    case 0: /* no step size control, fixed degree; both given by the user */
      nt=(*order<2)? 2: *order; /* 2 is the minimum order allowed */
      break;
    case 1:
      nt=compute_order_1_perturbation(xnorm,log10abserr,log10relerr,&flag_err);
      break;
    case 2:
      nt=compute_order_1_perturbation(xnorm,log10abserr,log10relerr,&flag_err);
      break;
    case -1:
      nt=comp_order_other_perturbation(xnorm,log10abserr,log10relerr);
      break;
    default:
      fprintf(stderr, "taylor_step: undefined step size control.\n");
      fprintf(stderr, "you must choose an existing step size control\n");
      fprintf(stderr, "method or supply a step size control procedure.\n");
      exit(0);
    }
  *order=nt;
/*
  computation of the jet of derivatives up to order nt
*/
  if(step_ctl != 0) {
    s=taylor_coefficients_perturbation_A(*ti,x,nt,1, jetInOut, &jetJetOut);
  } else {
    s=taylor_coefficients_perturbation_A(*ti,x,nt,0, jetInOut, &jetJetOut); 
 }

/*
  selection of the routine to compute the time step. the value
  step_ctl=3 has been reserved for the user, in case she/he wants to
  code a different method.
*/
  switch(step_ctl)
    {
    case 0: /* no step size control (fixed step size, given by the user) */
      AssignMyFloat(h,*ht);
      break;
    case 1:
      dh=compute_stepsize_1_perturbation(s,jetJetOut,nt,xnorm,flag_err);
      MakeMyFloatA(h, dh);
      break;
    case 2:
      dh=compute_stepsize_2_perturbation(s,jetJetOut,nt,xnorm,flag_err);
      MakeMyFloatA(h, dh);
      break;
    case -1:
      dh=comp_stepsize_other_perturbation(s,jetJetOut,_N_DIM_,nt,xnorm,log10abserr,log10relerr);
      MakeMyFloatA(h, dh);
      break;
    default:
      fprintf(stderr, "taylor_step: undefined step size control.\n");
      fprintf(stderr, "You must choose an existing step size control\n");
      fprintf(stderr, "method or supply a step size control procedure.\n");
      exit(0);
    }
/*
  if step_ctl != 0, we adjust the sign of the computed stepsize.
*/
  flag_endtime=0;
  if (step_ctl != 0)
    {
      if (dir == -1) { NegateMyFloatA(mtmp,h); AssignMyFloat(h, mtmp);} 
/*
      we compare *ti+h with endtime. we modify h if necessary.
*/
      if (endtime != NULL)
        {
          AddMyFloatA(mtmp,h,*ti);
          if (dir == 1) /* time goes forward */
            {
              if (MyFloatA_GE_B(mtmp,*endtime))
                {
                  SubtractMyFloatA(h,*endtime,*ti);
                  flag_endtime=1;
                }
            }
            else /* time goes backwards */
            {
              if (MyFloatA_GE_B(*endtime,mtmp))
                {
                  SubtractMyFloatA(h,*endtime,*ti);
                  flag_endtime=1;
                }
            }
        }
    }
/*
  next lines are the summation of the taylor series (horner's method)
*/
  j=nt-1;
#if _J_DIM_ != 0
  for(i=0; i<_J_DIM_; i++)
  {
    xAssignJetToJet(jetInOut[i],jetJetOut[i][nt]);
    for(k=j; k>=0; k--)
    {
      xMultiplyFloatJetA(jtmp, h, jetInOut[i]);
      xAddJetJetA(jetInOut[i], jtmp, jetJetOut[i][k]);
    }
    xAssignJetToFloat(x[i],jetInOut[i]);
  }
#endif
  for(i=_J_DIM_; i<_N_DIM_; i++)
  {
    AssignMyFloat(x[i],s[i][nt]);
    for(k=j; k>=0; k--)
    {
      MultiplyMyFloatA(mtmp, h, x[i]);
      AddMyFloatA(x[i], mtmp, s[i][k]);
    }
  }
/*
  finally, we set the values of the parameters *ht and *ti.
*/
  AssignMyFloat(*ht,h);
  if (flag_endtime == 0)
    {
      AssignMyFloat(mtmp, *ti);
      AddMyFloatA(*ti, mtmp, h);
    }
    else
    {
      AssignMyFloat(*ti,*endtime);
    }
  return(flag_endtime);
}
int compute_order_1_perturbation(double xnorm, double log10abserr, double log10relerr, int* flag_err)
/*
 * this is to determine the 'optimal' degree of the taylor expansion.
 *
 * parameters:
 * xnorm: norm of the initial condition
 * log10abserr: base-10 log of the absolute error required
 * log10relerr: base-10 log of the relative error required
 * flag_err:    flag. returns 1 if absolute error is used
 *                    returns 2 if relative error is used
 *
 * returned value: 'optimal' degree.
*/
{
  double log10eps,tmp;
  int nt;

  log10eps=log10abserr;
  *flag_err=1;
  if (xnorm != (double)0.0)
    {
      tmp=log10relerr+log10(xnorm);
      if (tmp > log10abserr) {log10eps=log10relerr; *flag_err=2;}
    }
/*
  we use 1.16 for the value 0.5*log(10)=1.151292546497...
*/
  nt=(int)(1.5-1.16*log10eps);
  if (nt < 2) nt=2; /* this is the minimum order accepted */

#if DEBUG_LEVEL > 0
      fprintf(stderr, "taylor_step: order is %d\n",nt);
#endif

  return(nt);
}
double compute_stepsize_1_perturbation(MY_FLOAT **s, MY_JET **jet, int nt, double xnorm, int flag_err)
/*
 * it looks for a step size for an expansion up to order nt. this
 * function requires that nt is the value computed by
 * compute_order_1_
 */
{
  double double_log_MyFloat_perturbation(MY_FLOAT x);
  static MY_FLOAT z,v1,v2;
  static MY_FLOAT of,uf;
  double lnv1,lnv2,r,lnro1,lnro2,lnro;
  int i;
  static int init=0;
#pragma omp threadprivate(z,v1,v2,of,uf,init)

  if (init == 0)
    {
      init=1;
      InitMyFloat(z);
      InitMyFloat(v1);
      InitMyFloat(v2);
      InitMyFloat(of);
      InitMyFloat(uf);

      r=pow((double)2,(double)LEXP2);
      MakeMyFloatA(of,r);
      r=pow((double)2,(double)(-LEXP2));
      MakeMyFloatA(uf,r);
    }
/*
  we compute the sup norm of the last two coefficients of the taylor
  series, and we store them into v1 and v2.
*/
  MakeMyFloatA(v1,0);
  MakeMyFloatA(v2,0);
#if _J_DIM_ != 0
  for (i=0; i<_J_DIM_; i++)
  {
    xNormJet(z,jet[i][nt-1]);
    if (MyFloatA_GT_B(z,v1)) AssignMyFloat(v1,z);
    xNormJet(z,jet[i][nt]);
    if (MyFloatA_GT_B(z,v2)) AssignMyFloat(v2,z);
  }
#endif
  for(i=_J_DIM_; i<_N_DIM_; i++)
  {
    fabsMyFloatA(z,s[i][nt-1]);
    if (MyFloatA_GT_B(z,v1)) AssignMyFloat(v1,z);
    fabsMyFloatA(z,s[i][nt]);
    if (MyFloatA_GT_B(z,v2)) AssignMyFloat(v2,z);
  }
/*
  computation of the step size. we need the logs of v1 and v2, in
  double precision (there is no need for extended precision). the idea
  is to assign these variables to double variables and then to use the
  standard log function. before doing that, we have to be sure that v1
  can be assigned to a double without under or overflow. for this
  reason we will check for this condition and, if it fails, we will
  call an specific function for this case.
*/
  if (MyFloatA_LE_B(v1,of) && MyFloatA_GE_B(v1,uf))
    {
      MyFloatToDouble(r,v1);
      lnv1=log(r);
    }
    else
    {
      lnv1=double_log_MyFloat_perturbation(v1);
    }
  if (MyFloatA_LE_B(v2,of) && MyFloatA_GE_B(v2,uf))
    {
      MyFloatToDouble(r,v2);
      lnv2=log(r);
    }
    else
    {
      lnv2=double_log_MyFloat_perturbation(v2);
    }
/*
  if flag_err == 2, this means that we are using a relative error control.
*/
  if (flag_err == 2)
    {
      r = -log10(xnorm);
      lnv1 += r;
      lnv2 += r;
    }
 lnro1= -lnv1/(nt-1);
 lnro2= -lnv2/nt;
 lnro=(lnro1 < lnro2)? lnro1: lnro2;

 r=exp(lnro-2-0.7/(nt-1)); /* exp(-0.7/(nt-1)) is a security factor */
  return(r);
}
double compute_stepsize_2_perturbation(MY_FLOAT **s, MY_JET **jet, int nt, double xnorm, int flag_err)
/*
 * it looks for a step size for an expansion up to order nt. this
 * function requires that nt is the value computed by
 * compute_order_1_. it also tries to reduce cancellations of big
 * terms in the summation of the taylor series.
 */
{
  double compute_stepsize_1_perturbation(MY_FLOAT**, MY_JET**, int, double, int);
  static MY_FLOAT h,hj,r,z,a,normj;
  double c,rtmp,dh;
  int i,j;
  static int init=0;
#pragma omp threadprivate(h,hj,r,z,a,normj,init)

  if (init == 0)
    {
      init=1;
      InitMyFloat(h); 
      InitMyFloat(hj);
      InitMyFloat(r);
      InitMyFloat(z);
      InitMyFloat(a);
      InitMyFloat(normj);
    }
/*
  we compute the step size according to the first algorithm
*/
  dh=compute_stepsize_1_perturbation(s,jet,nt,xnorm,flag_err);
  MakeMyFloatA(h,dh);
/*
  next lines select a value (z), that will be used to control the size
  of the terms of the Taylor series.
*/
  if (flag_err == 1) {
     MakeMyFloatA(z, 1.0);
  } else if (flag_err == 2) {
    MakeMyFloatA(z,xnorm);
  } else
    {
      printf("compute_stepsize_2 internal error. flag_err: %d\n",flag_err);
      exit(1);
    }
/*
  next loop checks if the sup norm of the terms in the Taylor series are
  lower than z. if a term is greater than z, the step size h is reduced.
*/
  MakeMyFloatA(hj,(double)1.0);

  for(j=1; j<=nt; j++)
  {
    MultiplyMyFloatA(r,h,hj);
    AssignMyFloat(hj,r);

    MakeMyFloatC(normj,"0", (double)0);
#if _J_DIM_ != 0
    for (i=0; i<_J_DIM_; i++)
    {
      xNormJet(a,jet[i][j]);
      if (MyFloatA_GT_B(a,normj)) AssignMyFloat(normj,a);
    }
#endif
    for (i=_J_DIM_; i<_N_DIM_; i++)
    {
      fabsMyFloatA(a,s[i][j]);
      if (MyFloatA_GT_B(a,normj)) AssignMyFloat(normj,a);
    }

    MultiplyMyFloatA(r,normj,hj);
    if (MyFloatA_LE_B(r,z)) continue;
/*
    we reduce h (and hj)
*/
    DivideMyFloatA(hj,z,normj);

    DivideMyFloatA(a,r,z);
    MyFloatToDouble(c,a);
    c=pow(c,(double)1.e0/(double)j);
    MakeMyFloatA(a,c);
    DivideMyFloatA(r,h,a);
    AssignMyFloat(h,r);

#if DEBUG_LEVEL > 1
    fprintf(stderr, "order %2d. reducing h from %14.6e to %14.6e\n",j,c*h,h);
#endif
  }

  MyFloatToDouble(rtmp,h);
  return(rtmp);
}

double double_log_MyFloat_perturbation(MY_FLOAT x)
/*
 * natural log, in double precision, of a MY_FLOAT positive number.
 */
{
  static MY_FLOAT a,tmp;
  static MY_FLOAT z,of,uf;
  double b,lx;
  int k;
  static int init=0;
#pragma omp threadprivate(a,tmp,z,of,uf,init)

  if (init == 0)
    {
      init=1;
      InitMyFloat(a);
      InitMyFloat(z);
      InitMyFloat(of);
      InitMyFloat(uf);
      InitMyFloat(tmp);

      b=0;
      MakeMyFloatA(z,b);
      b=pow((double)2,(double)LEXP2);
      MakeMyFloatA(of,b);
      b=pow((double)2,(double)(-LEXP2));
      MakeMyFloatA(uf,b);
    }

  if (MyFloatA_EQ_B(x,z))
    {
      puts("double_log_MyFloat error: zero argument");
      puts("(this is because one of the last two terms of your taylor");
      puts(" expansion is exactly zero)");
      exit(1);
    }

  AssignMyFloat(a,x);

  k=0;
  while(MyFloatA_LT_B(a,uf))
  {
    ++k;
    if(k>3000){fprintf(stderr,"double_log_MyFloat overflow: %d\n", k); exit(1);}
    MultiplyMyFloatA(tmp,a,of);
    AssignMyFloat(a,tmp);
  }
  while(MyFloatA_GT_B(a,of))
  {
    --k;
    if(k<-3000){fprintf(stderr,"double_log_MyFloat underflow: %d\n", k); exit(1);}
    MultiplyMyFloatA(tmp,a,uf);
    AssignMyFloat(a,tmp);
  }

  MyFloatToDouble(b,a);
/*
  lx stands for log(x)
*/
  lx=log(b)-(LEXP2*0.69314718055994530942)*k;

  return(lx);
}


int comp_order_other_perturbation(double lnxnorm, double log10abserr, double log10relerr){
  puts("---");
  puts("compute_order_user_defined:");
  puts("you have to code this routine");
  puts("or select a different value for the step_ctl parameter");
  puts("---");
  exit(1);

  return(0);
}
double comp_stepsize_other_perturbation(MY_FLOAT **s, MY_JET **jet, int nd, int nt, double xnorm, double log10abserr, double log10relerr) {

  puts("---");
  puts("compute_timestep_user_defined:");
  puts("you have to code this routine");
  puts("or select a different value for the step_ctl parameter");
  puts("---");
  exit(1);
  return((double)0.00001);
}
/***********************************************************************
 *
 * Procedure generated by the TAYLOR translator. Do not edit!
 *
 * It needs the header file 'taylor.h' to compile.
 * Run taylor with the -header -o taylor.h option to generate a sample 'taylor.h'

 * Translation info is at the end of this file.
 * Version 2.0.0 , Sept 8, 2022
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
MY_FLOAT **taylor_coefficients_perturbation_A(MY_FLOAT t, MY_FLOAT *x, int order, int rflag, MY_JET *sIn, MY_JET ***sOut)
{
   /* input: 
      t:     current value of the time variable 
      x:     array represent values of the state variables
      order: order of the taylor coefficients sought
      rflag: recompute flag. If you call this routine with one order 
             first, but then decided that you need a higher order of the
             taylor polynomial, you can pass 0 to rflag. This routine 
             will try to use the values already computed. Provided that 
             both x and t have not been changed, and you did not modify 
             the jet derivatives from the previous call.
      sIn:   array representing the input jet variables
             If NULL, the x values are assigned.
      sOut:  Pointer output jet variables
             If NULL, the pointer is not modified.
      Return Value:
            Two D Array, rows are the taylor coefficients of the
            state variables

     */
    extern MY_FLOAT e1;
    extern MY_FLOAT e2;
    extern MY_FLOAT coef[10];
    extern MY_FLOAT freq[10];

    static int          _jz_ivars[10];
    #pragma omp threadprivate( _jz_ivars)
    static MY_FLOAT     *_jz_jet[56],  *_jz_save = NULL, *_jz_oneOverN=NULL,*_jz_theNs=NULL;
    static MY_FLOAT     _jz_tvar1, _jz_tvar2, _jz_tvar3, _jz_tvar4; /* tmp vars */
    static MY_FLOAT     _jz_uvar1, _jz_uvar2; /* tmp vars */
    static MY_FLOAT     _jz_svar1, _jz_svar2, _jz_svar3, _jz_svar4, _jz_svar5; /* tmp vars */
    static MY_FLOAT     _jz_wvar3, _jz_wvar4; /* tmp vars */
    static MY_FLOAT     _jz_zvar1, _jz_zvar2; /* tmp vars */
    static MY_FLOAT     _jz_MyFloatZERO, _kthtmp;
    #pragma omp threadprivate(_jz_jet, _jz_save, _jz_oneOverN, _jz_theNs, _jz_tvar1, _jz_tvar2, _jz_tvar3, _jz_tvar4, _jz_uvar1, _jz_uvar2, _jz_svar1, _jz_svar2, _jz_svar3, _jz_svar4, _jz_svar5, _jz_wvar3, _jz_wvar4, _jz_zvar1, _jz_zvar2, _jz_MyFloatZERO, _kthtmp)
    static int          _jz_maxOrderUsed  = -1;
    static int          _jz_lastOrder = 0, _jz_initialized=0, _jz_ginitialized=0;
    int                 _jz_i, _jz_j, _jz_k, _jz_l, _jz_m, _jz_n, _jz_oorder ;
    #pragma omp threadprivate(_jz_maxOrderUsed,_jz_lastOrder, _jz_initialized, _jz_ginitialized)
    /* allocating memory if needed */
    if (_jz_maxOrderUsed < order )  { 
    	 if (_jz_ginitialized == 0) { 
    	   InitMyFloat(_jz_tvar1); InitMyFloat(_jz_tvar2);InitMyFloat(_jz_tvar3);InitMyFloat(_jz_tvar4);
    	   InitMyFloat(_jz_svar1); InitMyFloat(_jz_svar2);InitMyFloat(_jz_svar3);InitMyFloat(_jz_svar4);
    	   InitMyFloat(_jz_svar5); InitMyFloat(_jz_zvar1);InitMyFloat(_jz_zvar2);
    	   InitMyFloat(_jz_uvar1); InitMyFloat(_jz_uvar2);
    	   InitMyFloat(_jz_wvar3);InitMyFloat(_jz_wvar4);
    	   InitMyFloat(_jz_MyFloatZERO); InitMyFloat(_kthtmp);
    	   MakeMyFloatC(_jz_MyFloatZERO, "0", (double)0);
    	 }
    	 if (rflag > 0) rflag = 0; /* have to recompute everything */
    	 _jz_oorder=_jz_maxOrderUsed;
    	 _jz_maxOrderUsed  = order;
    	 if (_jz_ginitialized) { 
    	   for(_jz_i=0; _jz_i< _jz_oorder+1; _jz_i++) {ClearMyFloat(_jz_oneOverN[_jz_i]); ClearMyFloat(_jz_theNs[_jz_i]);}
    	   free(_jz_oneOverN); free(_jz_theNs);
    	 }
    	 _jz_theNs = (MY_FLOAT *)malloc((order+1) * sizeof(MY_FLOAT));
    	 _jz_oneOverN = (MY_FLOAT *)malloc((order+1) * sizeof(MY_FLOAT));
    	 for(_jz_i=0; _jz_i<order+1; _jz_i++) {InitMyFloat(_jz_oneOverN[_jz_i]);InitMyFloat(_jz_theNs[_jz_i]);}
    	 MakeMyFloatC(_jz_theNs[0],"0.0", (double)0.0);
    	 MakeMyFloatC(_jz_uvar1,"1.0", (double)1.0);
    	 for(_jz_i = 1; _jz_i <= order; _jz_i++) {
    		 AssignMyFloat(_jz_tvar2, _jz_theNs[_jz_i-1]);
    		 AddMyFloatA(_jz_theNs[_jz_i], _jz_tvar2, _jz_uvar1);
	}
    	 AssignMyFloat(_jz_oneOverN[0],_jz_uvar1);
    	 AssignMyFloat(_jz_oneOverN[1],_jz_uvar1);
    	 for(_jz_i = 2; _jz_i <= order; _jz_i++) {
    		 DivideMyFloatA(_jz_oneOverN[_jz_i], _jz_uvar1,_jz_theNs[_jz_i]);
	}
    	 if (_jz_ginitialized) {
    	    for(_jz_i=0; _jz_i<(_jz_oorder+1)*(56); _jz_i++) { ClearMyFloat(_jz_save[_jz_i]);} free(_jz_save);
    	 }
    	 _jz_save = (MY_FLOAT *)malloc((order+1)* 56 *sizeof(MY_FLOAT));
    	 for(_jz_i=0; _jz_i<(order+1)*(56); _jz_i++) { InitMyFloat(_jz_save[_jz_i]);}
    	 for(_jz_j = 0, _jz_k = 0; _jz_j < 56 ;  _jz_j++, _jz_k += order+1) { _jz_jet[_jz_j] =& (_jz_save[_jz_k]); }
    	 /* nonautonomous, derivatves of time */
    	 MakeMyFloatA(_jz_jet[2][1], (double)1.0);
    	 for(_jz_i = 2; _jz_i <= order; _jz_i++) { MakeMyFloatA(_jz_jet[2][_jz_i], (double)0.0);}

    	 /* True constants, initialized only once. */
	 /* const: i_019=0 */
	 _jz_ivars[0]=0;
	 /* const: i_026=1 */
	 _jz_ivars[1]=1;
	 /* const: i_027=2 */
	 _jz_ivars[2]=2;
	 /* const: i_028=3 */
	 _jz_ivars[3]=3;
	 /* const: i_029=4 */
	 _jz_ivars[4]=4;
	 /* const: i_030=5 */
	 _jz_ivars[5]=5;
	 /* const: i_031=6 */
	 _jz_ivars[6]=6;
	 /* const: i_032=7 */
	 _jz_ivars[7]=7;
	 /* const: i_033=8 */
	 _jz_ivars[8]=8;
	 /* const: i_024=9 */
	 _jz_ivars[9]=9;
    }

    if (rflag) {
    	 if (rflag < 0 ) return(NULL);
    	 if (MyFloatA_NEQ_B(t, _jz_jet[2][0])) rflag = 0;
    	 for(_jz_i = 0; rflag != 0 && _jz_i < 2; _jz_i++) {
    		 if (MyFloatA_NEQ_B(_jz_jet[_jz_i][0], x[_jz_i])) rflag = 0;
    	 }
    }

    if (rflag == 0) {
	 /* initialize all constant vars and state variables */
	 _jz_lastOrder = 1;
		 /* init state variable: */
	 AssignMyFloat(_jz_jet[0][0], x[0]);
		 /* init state variable: */
	 AssignMyFloat(_jz_jet[1][0], x[1]);
	 AssignMyFloat(_jz_jet[2][0], t); /* nonautonomous, time */
	 /* mult: v_035=(e1*v_017) */
	 MultiplyMyFloatA(_jz_jet[3][0], e1, _jz_jet[1][0]);
	 /* mult: v_036=(freq[i_019]*v_018) */
	 MultiplyMyFloatA(_jz_jet[4][0], freq[_jz_ivars[0]], _jz_jet[2][0]);
	 /* call: v_037=sin(v_036) */
	 sinMyFloatA(_jz_jet[5][0], _jz_jet[4][0]);
	 /* call: v_078=cos(v_036) */
	 cosMyFloatA(_jz_jet[6][0], _jz_jet[4][0]);
	 /* mult: v_038=(coef[i_019]*v_037) */
	 MultiplyMyFloatA(_jz_jet[7][0], coef[_jz_ivars[0]], _jz_jet[5][0]);
	 /* mult: v_039=(freq[i_026]*v_018) */
	 MultiplyMyFloatA(_jz_jet[8][0], freq[_jz_ivars[1]], _jz_jet[2][0]);
	 /* call: v_040=sin(v_039) */
	 sinMyFloatA(_jz_jet[9][0], _jz_jet[8][0]);
	 /* call: v_079=cos(v_039) */
	 cosMyFloatA(_jz_jet[10][0], _jz_jet[8][0]);
	 /* mult: v_041=(coef[i_026]*v_040) */
	 MultiplyMyFloatA(_jz_jet[11][0], coef[_jz_ivars[1]], _jz_jet[9][0]);
	 /* plus: v_042=(v_038+v_041) */
	 AddMyFloatA(_jz_jet[12][0], _jz_jet[7][0], _jz_jet[11][0]);
	 /* mult: v_043=(freq[i_027]*v_018) */
	 MultiplyMyFloatA(_jz_jet[13][0], freq[_jz_ivars[2]], _jz_jet[2][0]);
	 /* call: v_044=sin(v_043) */
	 sinMyFloatA(_jz_jet[14][0], _jz_jet[13][0]);
	 /* call: v_080=cos(v_043) */
	 cosMyFloatA(_jz_jet[15][0], _jz_jet[13][0]);
	 /* mult: v_045=(coef[i_027]*v_044) */
	 MultiplyMyFloatA(_jz_jet[16][0], coef[_jz_ivars[2]], _jz_jet[14][0]);
	 /* plus: v_046=(v_042+v_045) */
	 AddMyFloatA(_jz_jet[17][0], _jz_jet[12][0], _jz_jet[16][0]);
	 /* mult: v_047=(freq[i_028]*v_018) */
	 MultiplyMyFloatA(_jz_jet[18][0], freq[_jz_ivars[3]], _jz_jet[2][0]);
	 /* call: v_048=sin(v_047) */
	 sinMyFloatA(_jz_jet[19][0], _jz_jet[18][0]);
	 /* call: v_081=cos(v_047) */
	 cosMyFloatA(_jz_jet[20][0], _jz_jet[18][0]);
	 /* mult: v_049=(coef[i_028]*v_048) */
	 MultiplyMyFloatA(_jz_jet[21][0], coef[_jz_ivars[3]], _jz_jet[19][0]);
	 /* plus: v_050=(v_046+v_049) */
	 AddMyFloatA(_jz_jet[22][0], _jz_jet[17][0], _jz_jet[21][0]);
	 /* mult: v_051=(freq[i_029]*v_018) */
	 MultiplyMyFloatA(_jz_jet[23][0], freq[_jz_ivars[4]], _jz_jet[2][0]);
	 /* call: v_052=sin(v_051) */
	 sinMyFloatA(_jz_jet[24][0], _jz_jet[23][0]);
	 /* call: v_082=cos(v_051) */
	 cosMyFloatA(_jz_jet[25][0], _jz_jet[23][0]);
	 /* mult: v_053=(coef[i_029]*v_052) */
	 MultiplyMyFloatA(_jz_jet[26][0], coef[_jz_ivars[4]], _jz_jet[24][0]);
	 /* plus: v_054=(v_050+v_053) */
	 AddMyFloatA(_jz_jet[27][0], _jz_jet[22][0], _jz_jet[26][0]);
	 /* mult: v_055=(freq[i_030]*v_018) */
	 MultiplyMyFloatA(_jz_jet[28][0], freq[_jz_ivars[5]], _jz_jet[2][0]);
	 /* call: v_056=sin(v_055) */
	 sinMyFloatA(_jz_jet[29][0], _jz_jet[28][0]);
	 /* call: v_083=cos(v_055) */
	 cosMyFloatA(_jz_jet[30][0], _jz_jet[28][0]);
	 /* mult: v_057=(coef[i_030]*v_056) */
	 MultiplyMyFloatA(_jz_jet[31][0], coef[_jz_ivars[5]], _jz_jet[29][0]);
	 /* plus: v_058=(v_054+v_057) */
	 AddMyFloatA(_jz_jet[32][0], _jz_jet[27][0], _jz_jet[31][0]);
	 /* mult: v_059=(freq[i_031]*v_018) */
	 MultiplyMyFloatA(_jz_jet[33][0], freq[_jz_ivars[6]], _jz_jet[2][0]);
	 /* call: v_060=sin(v_059) */
	 sinMyFloatA(_jz_jet[34][0], _jz_jet[33][0]);
	 /* call: v_084=cos(v_059) */
	 cosMyFloatA(_jz_jet[35][0], _jz_jet[33][0]);
	 /* mult: v_061=(coef[i_031]*v_060) */
	 MultiplyMyFloatA(_jz_jet[36][0], coef[_jz_ivars[6]], _jz_jet[34][0]);
	 /* plus: v_062=(v_058+v_061) */
	 AddMyFloatA(_jz_jet[37][0], _jz_jet[32][0], _jz_jet[36][0]);
	 /* mult: v_063=(freq[i_032]*v_018) */
	 MultiplyMyFloatA(_jz_jet[38][0], freq[_jz_ivars[7]], _jz_jet[2][0]);
	 /* call: v_064=sin(v_063) */
	 sinMyFloatA(_jz_jet[39][0], _jz_jet[38][0]);
	 /* call: v_085=cos(v_063) */
	 cosMyFloatA(_jz_jet[40][0], _jz_jet[38][0]);
	 /* mult: v_065=(coef[i_032]*v_064) */
	 MultiplyMyFloatA(_jz_jet[41][0], coef[_jz_ivars[7]], _jz_jet[39][0]);
	 /* plus: v_066=(v_062+v_065) */
	 AddMyFloatA(_jz_jet[42][0], _jz_jet[37][0], _jz_jet[41][0]);
	 /* mult: v_067=(freq[i_033]*v_018) */
	 MultiplyMyFloatA(_jz_jet[43][0], freq[_jz_ivars[8]], _jz_jet[2][0]);
	 /* call: v_068=sin(v_067) */
	 sinMyFloatA(_jz_jet[44][0], _jz_jet[43][0]);
	 /* call: v_086=cos(v_067) */
	 cosMyFloatA(_jz_jet[45][0], _jz_jet[43][0]);
	 /* mult: v_069=(coef[i_033]*v_068) */
	 MultiplyMyFloatA(_jz_jet[46][0], coef[_jz_ivars[8]], _jz_jet[44][0]);
	 /* plus: v_070=(v_066+v_069) */
	 AddMyFloatA(_jz_jet[47][0], _jz_jet[42][0], _jz_jet[46][0]);
	 /* mult: v_071=(freq[i_024]*v_018) */
	 MultiplyMyFloatA(_jz_jet[48][0], freq[_jz_ivars[9]], _jz_jet[2][0]);
	 /* call: v_072=sin(v_071) */
	 sinMyFloatA(_jz_jet[49][0], _jz_jet[48][0]);
	 /* call: v_087=cos(v_071) */
	 cosMyFloatA(_jz_jet[50][0], _jz_jet[48][0]);
	 /* mult: v_073=(coef[i_024]*v_072) */
	 MultiplyMyFloatA(_jz_jet[51][0], coef[_jz_ivars[9]], _jz_jet[49][0]);
	 /* plus: v_074=(v_070+v_073) */
	 AddMyFloatA(_jz_jet[52][0], _jz_jet[47][0], _jz_jet[51][0]);
	 /* mult: v_075=(e2*v_074) */
	 MultiplyMyFloatA(_jz_jet[53][0], e2, _jz_jet[52][0]);
	 /* negate: v_076=(-v_016) */
	 NegateMyFloatA(_jz_jet[54][0],_jz_jet[0][0]);
	 /* plus: v_077=(v_075+v_076) */
	 AddMyFloatA(_jz_jet[55][0], _jz_jet[53][0], _jz_jet[54][0]);

	 /* the first derivative of state variables */
	 /* state variable 0: */
	 AssignMyFloat(_jz_jet[0][1], _jz_jet[3][0]);
	 /* state variable 1: */
	 AssignMyFloat(_jz_jet[1][1], _jz_jet[55][0]);
	 /* nonautonomous, time'=1 has been initialized already */
	}

	 /* compute the kth order derivatives of all vars */
	 for(_jz_k = _jz_lastOrder; _jz_k < order; _jz_k++) {
		 /* derivative for tmp variables */
		 /* mult: v_035=(e1*v_017) */
		 MultiplyMyFloatA(_jz_jet[3][_jz_k], e1, _jz_jet[1][_jz_k]);
		 /* mult: v_036=(freq[i_019]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[4][_jz_k], freq[_jz_ivars[0]]);}
		 else {AssignMyFloat(_jz_jet[4][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_037=sin(v_036) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[0]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[5][_jz_k], _jz_jet[6][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[5][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[6][_jz_k], tmp1);
		 }
		 /* call: v_078=cos(v_036) */
			 /* computed already */
		 /* mult: v_038=(coef[i_019]*v_037) */
		 MultiplyMyFloatA(_jz_jet[7][_jz_k], coef[_jz_ivars[0]], _jz_jet[5][_jz_k]);
		 /* mult: v_039=(freq[i_026]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[8][_jz_k], freq[_jz_ivars[1]]);}
		 else {AssignMyFloat(_jz_jet[8][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_040=sin(v_039) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[1]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[9][_jz_k], _jz_jet[10][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[9][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[10][_jz_k], tmp1);
		 }
		 /* call: v_079=cos(v_039) */
			 /* computed already */
		 /* mult: v_041=(coef[i_026]*v_040) */
		 MultiplyMyFloatA(_jz_jet[11][_jz_k], coef[_jz_ivars[1]], _jz_jet[9][_jz_k]);
		 /* plus: v_042=(v_038+v_041) */
		 AddMyFloatA(_jz_jet[12][_jz_k], _jz_jet[7][_jz_k],_jz_jet[11][_jz_k]);
		 /* mult: v_043=(freq[i_027]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[13][_jz_k], freq[_jz_ivars[2]]);}
		 else {AssignMyFloat(_jz_jet[13][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_044=sin(v_043) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[2]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[14][_jz_k], _jz_jet[15][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[14][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[15][_jz_k], tmp1);
		 }
		 /* call: v_080=cos(v_043) */
			 /* computed already */
		 /* mult: v_045=(coef[i_027]*v_044) */
		 MultiplyMyFloatA(_jz_jet[16][_jz_k], coef[_jz_ivars[2]], _jz_jet[14][_jz_k]);
		 /* plus: v_046=(v_042+v_045) */
		 AddMyFloatA(_jz_jet[17][_jz_k], _jz_jet[12][_jz_k],_jz_jet[16][_jz_k]);
		 /* mult: v_047=(freq[i_028]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[18][_jz_k], freq[_jz_ivars[3]]);}
		 else {AssignMyFloat(_jz_jet[18][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_048=sin(v_047) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[3]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[19][_jz_k], _jz_jet[20][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[19][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[20][_jz_k], tmp1);
		 }
		 /* call: v_081=cos(v_047) */
			 /* computed already */
		 /* mult: v_049=(coef[i_028]*v_048) */
		 MultiplyMyFloatA(_jz_jet[21][_jz_k], coef[_jz_ivars[3]], _jz_jet[19][_jz_k]);
		 /* plus: v_050=(v_046+v_049) */
		 AddMyFloatA(_jz_jet[22][_jz_k], _jz_jet[17][_jz_k],_jz_jet[21][_jz_k]);
		 /* mult: v_051=(freq[i_029]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[23][_jz_k], freq[_jz_ivars[4]]);}
		 else {AssignMyFloat(_jz_jet[23][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_052=sin(v_051) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[4]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[24][_jz_k], _jz_jet[25][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[24][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[25][_jz_k], tmp1);
		 }
		 /* call: v_082=cos(v_051) */
			 /* computed already */
		 /* mult: v_053=(coef[i_029]*v_052) */
		 MultiplyMyFloatA(_jz_jet[26][_jz_k], coef[_jz_ivars[4]], _jz_jet[24][_jz_k]);
		 /* plus: v_054=(v_050+v_053) */
		 AddMyFloatA(_jz_jet[27][_jz_k], _jz_jet[22][_jz_k],_jz_jet[26][_jz_k]);
		 /* mult: v_055=(freq[i_030]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[28][_jz_k], freq[_jz_ivars[5]]);}
		 else {AssignMyFloat(_jz_jet[28][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_056=sin(v_055) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[5]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[29][_jz_k], _jz_jet[30][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[29][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[30][_jz_k], tmp1);
		 }
		 /* call: v_083=cos(v_055) */
			 /* computed already */
		 /* mult: v_057=(coef[i_030]*v_056) */
		 MultiplyMyFloatA(_jz_jet[31][_jz_k], coef[_jz_ivars[5]], _jz_jet[29][_jz_k]);
		 /* plus: v_058=(v_054+v_057) */
		 AddMyFloatA(_jz_jet[32][_jz_k], _jz_jet[27][_jz_k],_jz_jet[31][_jz_k]);
		 /* mult: v_059=(freq[i_031]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[33][_jz_k], freq[_jz_ivars[6]]);}
		 else {AssignMyFloat(_jz_jet[33][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_060=sin(v_059) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[6]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[34][_jz_k], _jz_jet[35][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[34][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[35][_jz_k], tmp1);
		 }
		 /* call: v_084=cos(v_059) */
			 /* computed already */
		 /* mult: v_061=(coef[i_031]*v_060) */
		 MultiplyMyFloatA(_jz_jet[36][_jz_k], coef[_jz_ivars[6]], _jz_jet[34][_jz_k]);
		 /* plus: v_062=(v_058+v_061) */
		 AddMyFloatA(_jz_jet[37][_jz_k], _jz_jet[32][_jz_k],_jz_jet[36][_jz_k]);
		 /* mult: v_063=(freq[i_032]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[38][_jz_k], freq[_jz_ivars[7]]);}
		 else {AssignMyFloat(_jz_jet[38][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_064=sin(v_063) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[7]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[39][_jz_k], _jz_jet[40][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[39][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[40][_jz_k], tmp1);
		 }
		 /* call: v_085=cos(v_063) */
			 /* computed already */
		 /* mult: v_065=(coef[i_032]*v_064) */
		 MultiplyMyFloatA(_jz_jet[41][_jz_k], coef[_jz_ivars[7]], _jz_jet[39][_jz_k]);
		 /* plus: v_066=(v_062+v_065) */
		 AddMyFloatA(_jz_jet[42][_jz_k], _jz_jet[37][_jz_k],_jz_jet[41][_jz_k]);
		 /* mult: v_067=(freq[i_033]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[43][_jz_k], freq[_jz_ivars[8]]);}
		 else {AssignMyFloat(_jz_jet[43][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_068=sin(v_067) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[8]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[44][_jz_k], _jz_jet[45][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[44][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[45][_jz_k], tmp1);
		 }
		 /* call: v_086=cos(v_067) */
			 /* computed already */
		 /* mult: v_069=(coef[i_033]*v_068) */
		 MultiplyMyFloatA(_jz_jet[46][_jz_k], coef[_jz_ivars[8]], _jz_jet[44][_jz_k]);
		 /* plus: v_070=(v_066+v_069) */
		 AddMyFloatA(_jz_jet[47][_jz_k], _jz_jet[42][_jz_k],_jz_jet[46][_jz_k]);
		 /* mult: v_071=(freq[i_024]*v_018) */
		 if(_jz_k==1) {AssignMyFloat(_jz_jet[48][_jz_k], freq[_jz_ivars[9]]);}
		 else {AssignMyFloat(_jz_jet[48][_jz_k], _jz_MyFloatZERO);}
		 /* call: v_072=sin(v_071) */
		 { /* call sin cos */
			 static MY_FLOAT tmp, tmp1, tmp2, tmp3;
			 #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)
			 if(_jz_initialized==0) {
			  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);
			 }
			 DivideMyFloatByInt(tmp, freq[_jz_ivars[9]], _jz_k);
			 MultiplyMyFloatA(_jz_jet[49][_jz_k], _jz_jet[50][_jz_k-1], tmp);
			 MultiplyMyFloatA(tmp2, tmp, _jz_jet[49][_jz_k-1]);
			 NegateMyFloatA(tmp1, tmp2);
			 AssignMyFloat(_jz_jet[50][_jz_k], tmp1);
		 }
		 /* call: v_087=cos(v_071) */
			 /* computed already */
		 /* mult: v_073=(coef[i_024]*v_072) */
		 MultiplyMyFloatA(_jz_jet[51][_jz_k], coef[_jz_ivars[9]], _jz_jet[49][_jz_k]);
		 /* plus: v_074=(v_070+v_073) */
		 AddMyFloatA(_jz_jet[52][_jz_k], _jz_jet[47][_jz_k],_jz_jet[51][_jz_k]);
		 /* mult: v_075=(e2*v_074) */
		 MultiplyMyFloatA(_jz_jet[53][_jz_k], e2, _jz_jet[52][_jz_k]);
		 /* negation: v_076=(-v_016) */
		 NegateMyFloatA(_jz_jet[54][_jz_k], _jz_jet[0][_jz_k]);
		 /* plus: v_077=(v_075+v_076) */
		 AddMyFloatA(_jz_jet[55][_jz_k], _jz_jet[53][_jz_k],_jz_jet[54][_jz_k]);
		 /* derivative of state variables */
		 _jz_m = _jz_k+1;
		 /* state variable 0: */
		 DivideMyFloatByInt(_jz_jet[0][_jz_m], _jz_jet[3][_jz_k], _jz_m);
		 /* state variable 1: */
		 DivideMyFloatByInt(_jz_jet[1][_jz_m], _jz_jet[55][_jz_k], _jz_m);
		 _jz_initialized=1;
	 }
    _jz_lastOrder = order;
    _jz_ginitialized=1;
    if (sOut) {
     *sOut = NULL;
    }
    return(_jz_jet);
}

MY_FLOAT **taylor_coefficients_perturbationA(MY_FLOAT t, MY_FLOAT *x, int order, int rflag)
{
    return(taylor_coefficients_perturbation_A(t,x,order,rflag, NULL, NULL));
}

MY_FLOAT **taylor_coefficients_perturbation(MY_FLOAT t, MY_FLOAT *x, int order)
{
    return(taylor_coefficients_perturbationA(t,x,order,0));
}

