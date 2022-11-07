
#include "lorenz3.h"

#define N _NUMBER_OF_STATE_VARS_
#define NJ _NUMBER_OF_JET_VARS_

#ifdef _USE_MPFR_
#define DIGITS_PRECISION 35
#define STR(x) #x
#define STR1(x) STR(x)
#define JFMT "% ." STR1(DIGITS_PRECISION) "RNE"
#else
#define DIGITS_PRECISION 16
#define JFMT "% .14e"
#endif

int main(void)
{
  int i, direction=+1, step_cntrl=2;
  double log10abs=-DIGITS_PRECISION, log10rel=-DIGITS_PRECISION;
  MY_FLOAT startT, nextT, stopT, x[N];
  MY_JET xjet[NJ];

      #ifdef _USE_MPFR_
  mpfr_set_default_prec((int)(DIGITS_PRECISION*log2(10))+1);
      #endif

  taylor_initialize_jet_library();

  InitMyFloat(startT); InitMyFloat(nextT); InitMyFloat(stopT);
  for (i = 0; i < N; i++)  {InitMyFloat(x[i]);}
  for (i = 0; i < NJ; i++) {taylor_initialize_jet_variable(&xjet[i]);}

  MakeMyFloatC(x[0],"0",0);
  MakeMyFloatC(x[1],"1",1);
  MakeMyFloatC(x[2],"0",0);
  MakeMyFloatC(x[3],"28",28); /* parameter RR */

  taylor_make_identity_jets(xjet,x,NULL);

  MakeMyFloatC(startT,"0",0);
  MakeMyFloatC(stopT,"1",1);
  while (taylor_step_lorenz3_eq(&startT, x, direction,
				step_cntrl, log10abs, log10rel,
				&stopT, &nextT, NULL, xjet) != 1) {}

  for (i = 0; i < NJ; i++) {
    printf("x%d=\n",i);
    taylor_output_jet(stdout, JFMT "\n", xjet[i]);
  }

  /* cleaning memory */
  for (i = 0; i < NJ; i++) {taylor_clear_jet_variable(&xjet[i]);}
  for (i = 0; i < N; i++)  {ClearMyFloat(x[i]);}
  ClearMyFloat(stopT); ClearMyFloat(nextT); ClearMyFloat(startT);
  taylor_clear_up_jet_library();
  return 0;
} 
