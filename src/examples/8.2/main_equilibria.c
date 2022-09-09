#include "lorenz5.h"
#define NJ _NUMBER_OF_JET_VARS_
#define NS _NUMBER_OF_STATE_VARS_
MY_FLOAT RR;

int main(int argc, char *argv[])
{
  int k, j;
  MY_FLOAT x[NS], t, A[NS*NS], *dtmp;
  MY_JET jetIn[NJ], **jetOut;
  taylor_initialize_jet_library();
  /* memory allocation */
  InitMyFloat(t);
  InitMyFloat(RR);
  for (k = 0; k < NS; k++) {InitMyFloat(x[k]);}
  for (k = 0; k < NJ; k++) {InitJet(jetIn[k]);}
  for (k = 0; k < NS*NS; k++) {InitMyFloat(A[k]);}
  /* equilibrium point */
  MakeMyFloatC(t,"0",0);
  MakeMyFloatC(RR,"28",28);
  for (k = 0; k < NS; k++) {MakeMyFloatC(x[k],"0",0);}
  /* jets with identity matrix at first order */
  taylor_make_identity_jets(jetIn,x,NULL);
  /* vector field evaluation */
  taylor_coefficients_lorenz5_eq_A(t, x, 1, 0, jetIn, &jetOut);
  for (k = 0; k < NS; k++)
  {
    /* coefficients without the state variable */
    dtmp = taylor_convert_jet_to_array(jetOut[k][1],0);
    /* save the value in the matrix A*/
    for (j = 0; j < NS; j++) A[k*NS + j] = dtmp[j];
  }
  /* print the differential at the equilibrium point */
  for (k = 0; k < NS*NS; k++)
  {
    OutputMyFloat3(stdout,"% .5e ", A[k]);
    if ((k+1) % NS==0) printf("\n");
  }
  /* free memory */
  for (k = 0; k < NS*NS; k++) {ClearMyFloat(A[k]);}
  for (k = 0; k < NJ; k++) {ClearJet(jetIn[k]);}
  for (k = 0; k < NS; k++) {ClearMyFloat(x[k]);}
  ClearMyFloat(RR); ClearMyFloat(t);
  ClearUpJet();
  return 0;
}
