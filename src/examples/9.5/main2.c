#include <stdio.h>
#include <math.h>
#include "taylor.h"

MY_FLOAT **taylor_coefficients_lorenz(MY_FLOAT, MY_FLOAT *, int);

int main(int argc, char *argv[])
{
   MY_FLOAT  xx[3], tmp[3], t, **coef;
   int       j, order=20, nsteps = 100;
   double    step_size;
   /* set initiaial conditions */
   xx[0] = 0.1;
   xx[1] = 0.2;
   xx[2] = 0.3;
   t     = 0.0;
   /* control parameters       */
   step_size= 0.1;

   /* integrate 100 steps */
   while( -- nsteps > 0) {
      /* do something with xx and t. We just print it */
      printf("%f %f %f %f\n", xx[0], xx[1], xx[2], t);

      /* compute the taylor coefficients */
      coef = taylor_coefficients_lorenz(t, xx, order);

      /* now we have the taylor coefficients in coef,
       * we can analyze them and choose a best step size.
       * Here we just integrate use the given stepsize.
       */
       tmp[0] = tmp[1] = tmp[2] = 0.0;
       for(j=order; j>0; j--) /* sum up the taylor polynomial */
         { 
           tmp[0] = (tmp[0] + coef[0][j])* step_size;
           tmp[1] = (tmp[1] + coef[1][j])* step_size;
           tmp[2] = (tmp[2] + coef[2][j])* step_size;
         }
        /* advance one step */
        xx[0] = xx[0] + tmp[0];
        xx[1] = xx[1] + tmp[1];
        xx[2] = xx[2] + tmp[2];
        t += step_size; /* advance time */
     }
  return 0;
}
