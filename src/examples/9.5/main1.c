/* save as main1.c */

#include <stdio.h>
#include <math.h>
#include "taylor.h"
int main(int argc, char *argv[])
{
  MY_FLOAT  xx[3], t;
  double    h, h_return, log10abs_err, log10rel_err, endtime;
  int       nsteps = 100, step_ctrl_method = 2, direction = 1;
  int       order = 10;
  /* set initial conditions */
  xx[0] = 0.1;
  xx[1] = 0.2;
  xx[2] = 0.3;
  t     = 0.0;
  /* control parameters       */
  h= 0.001;
  log10abs_err = -16; /* i.e. 1.0e-16 absolute error */
  log10rel_err = -16; /* i.e. 1.0e-16 relative error */
  endtime = 10.0;

  /* integrate 100 steps */
  while( -- nsteps > 0 && h_return != 0) {
    /* do something with xx and t. We just print it */
    printf("%f %f %f %f\n", xx[0],xx[1],xx[2],t);
    taylor_step_lorenz(&t, &xx[0], direction,
		       step_ctrl_method,log10abs_err, log10rel_err,
		       &endtime, &h_return, &order, NULL);
  }
  return 0;
}
