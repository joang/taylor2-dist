#include "lorenz2.h"

int main(int argc, char **argv)
{
  int     i, j, order=20, itmp=0, direction = 1, nsteps = -1, counter=0;
  double  dstep, rtolerance, log10abs=-16, log10rel=-16;
  double  startT, stopT, nextT;
  double  xx[4], yy[4], zz[4];
  double  **jet;
  double  lyap=0.0, norm;
	 
  MY_JET    *jetOut;
  MY_JET    jetIn[4];

  taylor_initialize_jet_library();

  for(i=0; i < 3; i++) InitJet(jetIn[i]);
  
  /* initialize jet vars --start */
  InputJetFromString(jetIn[0],"(0.03  1 )");
  InputJetFromString(jetIn[1],"(-0.02 0 )");
  InputJetFromString(jetIn[2],"(0.15  0 )");
  /* initialize jet vars --end */

  stopT = 100000;
  startT = 0;
  dstep=0.001;
	 
  while(1)  {
    if(itmp != 0) {break;} 
    if(startT >= stopT) { break;}
    itmp = taylor_step_lorenz2_eq( &startT, xx, direction, 1, log10abs, log10rel, &stopT, &nextT, &order, jetIn);
		 
    if(++counter >= 1000) {  // we normalize the jet every 1000 steps 
      norm = 0;
     for(i=0; i < 3; i++)  norm += MY_JET_DATA(jetIn[i],1) * MY_JET_DATA(jetIn[i],1);

      norm = sqrt(norm);		   
      lyap += log(norm);

      for(i=0; i < 3; i++) MY_JET_DATA(jetIn[i],1)  /= norm;
      counter = 0;
    }
  }  /* while */
  if(counter > 0) {
    for(i=0; i < 3; i++) norm += MY_JET_DATA(jetIn[i],1) * MY_JET_DATA(jetIn[i],1);
    norm = sqrt(norm);		   	   
    lyap += log(norm);
  }
	 
  lyap /= 10000;	 
  fprintf(stdout, "The estimated Lyapunov exponent is: %f\n", lyap);
	 
  exit(0); 
} 
