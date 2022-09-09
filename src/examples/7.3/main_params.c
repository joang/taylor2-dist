#include "taylor.h"

/* these are the variables the vector fields depends on */
MY_FLOAT e1, e2, coef[10], freq[10];
int main(void)
{
  MY_FLOAT xx[2], t;
  double h, abs_err, rel_err, h_return;
  double log10abs_err, log10rel_err, endtime;
  int i, nsteps = 1000, order=10, direction=1;
  int step_ctrl_method=2;
  /* read in e1, e2, coef[] and freq[]
   * here, we just assign them to some
   * values
   */
  e1 = e2 = 1.0;
  for(i = 0; i < 10; i++) {
    coef[i] = 1.0;
    freq[i] = 0.1*(double) i;
  }
  /* set initial condition */
  xx[0] = 0.1;
  xx[1] = 0.2;
  t = 0.0;
  /* control parameters */
  h= 0.001;
  abs_err = 1.0e-16;
  rel_err = 1.0e-16;
  log10abs_err = log10(abs_err);
  log10rel_err = log10(rel_err);
  endtime = 10.0;
  /* integrate 100 steps */
  h_return = h; /* thanks to Jason James */
  while( -- nsteps > 0 && h_return != 0.0 ) {
    /* do something with xx and t. We just print it */
    printf("%f %f %f\n", xx[0],xx[1],t);
    taylor_step_perturbation(&t, &xx[0], direction,
			     step_ctrl_method,log10abs_err, log10rel_err,
			     &endtime, &h_return, &order,NULL);
  }
  return 0;
}
    
