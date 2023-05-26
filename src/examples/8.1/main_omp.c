#include <omp.h>
#include "lorenz4.h"

#define NJ _NUMBER_OF_JET_VARS_
#define NS _NUMBER_OF_STATE_VARS_

MY_FLOAT RR, SS;

#pragma omp threadprivate(RR)
int main(int argc, char *argv[])
{
  int k, j, ord;
  const int np = omp_get_max_threads();
  MY_FLOAT x[NS*np], te;
  MY_JET xjets[NJ*np];

  printf("np=%d threads\n", np);

  taylor_initialize_jet_library();
  
#pragma omp parallel

  /* memory allocation */
  InitMyFloat(te); InitMyFloat(RR); InitMyFloat(SS);
  for (k = 0; k < NS*np; k++) {InitMyFloat(x[k]);}
  for (k = 0; k < NJ*np; k++) {InitJet(xjets[k]);}

  /* some initializations */
  MakeMyFloatC(SS,"10",10);
  MakeMyFloatC(te,"1",1);

  /* jets with identity matrix at first order */
  for (j = 0; j < np; j++) {taylor_make_identity_jets(xjets+NJ*j,x+NS*j,NULL,NULL);}
#pragma omp parallel private(ord,k)
  {
    MY_FLOAT t,ht;
    InitMyFloat(t); InitMyFloat(ht);
    int tid = omp_get_thread_num();
    printf("tid=%d\n", tid);
    MakeMyFloatA(RR,tid+1);
    for (k = 0; k < NS; k++) {MakeMyFloatA(x[NS*tid+k],1/(tid+1));}
    MakeMyFloatC(t,"0",0);

    while (taylor_step_lorenz4_eq(&t, x+NS*tid, +1, 2, -16, -16,
				    &te, &ht, &ord, xjets+NJ*tid) != 1) {}
    ClearMyFloat(ht); ClearMyFloat(t);
  } /* end parallel region */

  /* free memory */
  for (k = 0; k < NJ*np; k++) {ClearJet(xjets[k]);}
  for (k = 0; k < NS*np; k++) {ClearMyFloat(x[k]);}
  ClearMyFloat(SS); ClearMyFloat(RR); ClearMyFloat(te);
  #pragma omp parallel
  ClearUpJet();
  return 0;
}
