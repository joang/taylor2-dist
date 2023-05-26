#include "jdata.h"

/*************************************************************************
 *
 *  This example demostrates how to access MY_JET storage array directly.
 *
 *  taylor -header -name jdata -o jdata.h model.eq
 *  taylor -jet -jet_helper -name jdata -step -header_name jdata.h model.eq -o jdata.c
 *  cc -g jdata.c jdata_main.c -lm 
 *
 *  ./a.out
 */

#define NN  _NUMBER_OF_STATE_VARS_
#define JJ  _NUMBER_OF_JET_VARS_ 

double initial_values[] = { 0.03, -0.02, 0.15, 0.1, -0.1, 0.1, 0.2, -0.2, 0.2, 0.1};
double T0 = 0.0, T1 = 1.0;

 
int main(int argc, char **argv)
{
	 int       i, j, order=20, itmp=0, direction = 1, nsteps = -1;
	 double    dstep, tolerance, rtolerance, log10tolerance, log10rtolerance;
	 MY_FLOAT  startT, stopT, nextT;
	 MY_FLOAT  xx[NN];
	 MY_JET    jetIn[NN];
	 
#ifdef _USE_MPFR_	 
	 mpfr_set_default_prec(256);
#endif
#ifdef _USE_GMP_	 
	 mpf_set_default_prec(256);
#endif	 
	 taylor_initialize_jet_library();
	 
	 InitMyFloat(startT);
	 InitMyFloat(stopT);
	 InitMyFloat(nextT);
	 
	 for(i = 0; i<NN; i++) {
	   InitMyFloat(xx[i]);
	 }
	 
	 for(i=0; i<NN; i++) {
	   InitJet(jetIn[i]);
	 }

	 
	 
	 dstep=0.001; /* only nedeed when step_ctrl_method==0 (see manual) */
	 tolerance = (double)(1e-16);
	 rtolerance = (double)(1e-16);
	 log10tolerance  = log10(tolerance);
	 log10rtolerance = log10(rtolerance);

	 taylor_make_identity_jets(jetIn, NULL, NULL,NULL);

	 MakeMyFloatA(stopT, T1);
	 MakeMyFloatA(startT, T0);	 
	 order=20;
	 itmp = 0;

	 /* 
	  * The MY_FLOAT storage array for MY_JET can be accessed using the MY_JETDATA(jet,idx) macro
	  */
	 for(i =0; i< NN; i++) {
	   MakeMyFloatA(xx[i],  initial_values[i]);
	   AssignMyFloat(MY_JET_DATA(jetIn[i],0),  xx[i]);	   	   
	 }

	 // print monomial names
	 {
	   const char **monomials = taylor_get_jet_monomials();
	   fprintf(stdout,"%-18s", "     1");
	   i = 0;
	   while(monomials[i] != NULL) {
	     fprintf(stdout, "%-13s", monomials[i]);
	     i++;
	   }
	   fprintf(stdout,"\n");
	 }
	 
	 while(1)  {

	   for(i = 0; i < JJ; i++) {
	     const char **var_names = taylor_get_variable_names();
	     fprintf(stdout, "%s: ", var_names[i]);
	     for(j = 0; j < _MAX_SIZE_OF_JET_VAR_; j++) {
#ifdef _USE_MPFR_	 	       
	       fprintf(stdout, "%12.8f ", mpfr_get_d( MY_JET_DATA(jetIn[i],j),GMP_RNDN));
#else
#ifdef _USE_GMP_
	       fprintf(stdout, "%12.8f ", mpf_get_d(MY_JET_DATA(jetIn[i],j),GMP_RNDN));	       
#else
	       fprintf(stdout, "%12.8f ", MY_JET_DATA(jetIn[i],j));	       
#endif
#endif	       
	     }
	     fprintf(stdout, "\n");	     
	   }
	   fprintf(stdout, "\n");	     	   
	   
	   if(itmp != 0) {break;} 
	   if(MyFloatA_GE_B(startT,stopT)) { break;}
	   
	   itmp = taylor_step_jdata( &startT, xx, direction, 1, log10tolerance, log10rtolerance, &stopT, &nextT, &order, jetIn);

	 }  /* while */
	 
	 exit(0); 
 } 
