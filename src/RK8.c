int RK4QC_step_d_eq(MY_FLOAT *t_in, MY_FLOAT *xx_in, int dir, int step_ctl, double log10abserr, double log10relerr,
		    MY_FLOAT *endtime, MY_FLOAT *ht, int *order, MY_JET *jetIn) { //out to jetIn on return
  static int initialized =0;
  static MY_FLOAT




#define RT6   2.449489742783178098197

#define A1    (1.0 / 12.0)
#define A2    (1.0 / 9.0)
#define A3    (1.0 / 6.0)
#define A4    ((2.0 + 2.0 * RT6) / 15.0)
#define A5    ((6.0 + RT6) / 15.0)
#define A6    ((6.0 - RT6) / 15.0)
#define A7    (2.0 / 3.0)
#define A8    (1.0 / 2.0)
#define A9    (1.0 / 3.0)
#define A10   (1.0 / 4.0)
#define A11   (4.0 / 3.0)
#define A12   (5.0 / 6.0)
#define A14   (1.0 / 6.0)

#define B10   (1.0 / 12.0)

#define B20   (1.0 / 27.0)
#define B21   (2.0 / 27.0)

#define B30   (1.0 / 24.0)
#define B32   (1.0 / 8.0)

#define B40   ((4.0 + 94.0 * RT6) / 375.0)
#define B42   ((-94.0 - 84.0 * RT6) / 125.0)
#define B43   ((328.0 + 208.0 * RT6) / 375.0)

#define B50   ((9.0 - RT6) / 150.0)
#define B53   ((312.0 + 32.0 * RT6) / 1425.0)
#define B54   ((69.0 + 29.0 * RT6) / 570.0)

#define B60   ((927.0 - 347.0 * RT6) / 1250.0)
#define B63   ((-16248.0 + 7328.0 * RT6) / 9375.0)
#define B64   ((-489.0 + 179.0 * RT6) / 3750.0)
#define B65   ((14268.0 - 5798.0 * RT6) / 9375.0)
#define B70   (2.0 / 27.0)
#define B75   ((16.0 - RT6) / 54.0)
#define B76   ((16.0 + RT6) / 54.0)

#define B80   (19.0 /256.0)
#define B85   ((118.0 - 23.0 * RT6) / 512.0)
#define B86   ((118.0 + 23.0 * RT6) / 512.0)
#define B87   (-9.0 / 256.0)

#define B90   (11.0 / 144.0)
#define B95   ((266.0 - RT6) / 864.0)
#define B96   ((266.0 + RT6) / 864.0)
#define B97   (-1.0 / 16.0)
#define B98   (-8.0 / 27.0)

#define B100  ((5034.0 - 271.0 * RT6) / 61440.0)
#define B106  ((7859.0 - 1626.0 * RT6) / 10240.0)
#define B107  ((-2232.0 + 813.0 * RT6) / 20480.0)
#define B108  ((-594.0 + 271.0 * RT6) / 960.0)
#define B109  ((657.0 - 813.0 * RT6) / 5120.0)
#define B110  ((5996.0 - 3794.0 * RT6) / 405.0)
#define B115  ((-4342.0 - 338.0 * RT6) / 9.0)
#define B116  ((154922.0 - 40458.0 * RT6) / 135.0)
#define B117  ((-4176.0 + 3794.0 * RT6) / 45.0)
#define B118  ((-340864.0 + 242816.0 * RT6) / 405.0)
#define B119  ((26304.0 - 15176.0 * RT6) / 45.0)
#define B1110 (-26624.0 / 81.0)



#define B120  ((3793.0 + 2168.0 * RT6) / 103680.0)
#define B125  ((4042.0 + 2263.0 * RT6) / 13824.0)
#define B126  ((-231278.0 + 40717.0 * RT6) / 69120.0)
#define B127  ((7947.0 - 2168.0 * RT6) / 11520.0)
#define B128  ((1048.0 - 542.0 * RT6) / 405.0)
#define B129  ((-1383.0 + 542.0 * RT6) / 720.0)
#define B1210 (2624.0 / 1053.0)
#define B1211 (3.0 / 1664.0)
#define B130  (-137.0 / 1296.0)
#define B135  ((5642.0 - 337.0 * RT6) / 864.0)
#define B136  ((5642.0 + 337.0 * RT6) / 864.0)
#define B137  (-299.0 / 48.0)
#define B138  (184.0 / 81.0)
#define B139  (-44.0 / 9.0)
#define B1310 (-5120.0 / 1053.0)
#define B1311 (-11.0 / 468.0)
#define B1312 (16.0 / 9.0)

#define B140  ((33617.0 - 2168.0 * RT6) / 518400.0)
#define B145  ((-3846.0 + 31.0 * RT6) / 13824.0)
#define B146  ((155338.0 - 52807.0 * RT6) / 345600.0)
#define B147  ((-12537.0 + 2168.0 * RT6) / 57600.0)
#define B148  ((92.0 + 542.0 * RT6) / 2025.0)
#define B149  ((-1797.0 - 542.0 * RT6) / 3600.0)
#define B1410 (320.0 / 567.0)
#define B1411 (-1.0 / 1920.0)
#define B1412 (4.0 / 105.0)
#define B150  ((-36487.0 - 30352.0 * RT6) / 279600.0)
#define B155  ((-29666.0 - 4499.0 * RT6) / 7456.0)
#define B156  ((2779182.0 - 615973.0 * RT6) / 186400.0)
#define B157  ((-94329.0 + 91056.0 * RT6) / 93200.0)
#define B158  ((-232192.0 + 121408.0 * RT6) / 17475.0)
#define B159  ((101226.0 - 22764.0 * RT6) / 5825.0)
#define B1510 (-169984.0 / 9087.0)
#define B1511 (-87.0 / 30290.0)
#define B1512 (492.0 / 1165.0)
#define B1514 (1260.0 / 233.0)


#define C80   (103.0 / 1680.0)
#define C87   (-27.0 / 140.0)
#define C88   (76.0 / 105.0)
#define C89   (-201.0 / 280.0)
#define C810  (1024.0 / 1365.0)
#define C811  (3.0 / 7280.0)
#define C812  (12.0 / 35.0)
#define C813  (9.0 / 280.0)

#define C90   (23.0 / 525.0)
#define C97   (171.0 / 1400.0)
#define C98   (86.0 / 525.0)
#define C99   (93.0 / 280.0)
#define C910  (-2048.0 / 6825.0)
#define C911  (-3.0 / 18200.0)
#define C912  (39.0 / 175.0)
#define C914  (9.0 / 25.0)
#define C915  (233.0 / 4200.0)

#define E0    (-7.0 / 400.0)
#define E7    (63.0 / 200.0)
#define E8    (-14.0 / 25.0)
#define E9    (21.0 / 20.0)
#define E10   (-1024.0 / 975.0)
#define E11   (-21.0 / 36400.0)
#define E12   (-3.0 / 25.0)
#define E13   (-9.0 / 280.0)
#define E14   (9.0 / 25.0)
#define E15   (233.0 / 4200.0)
    
    
#define EW 1    /* if 1, prints a message when the estimated error for */
                /* the computed point is larger than tol */
#define IS 3    /* number of iterates for the stepsize prediction */
#define FC 0.90 /* safety factor for the stepsize prediction */
#define RK 9    /* Runge-Kutta used to compute the new point */

static inline double step_rk(double *x, int n, double k[13][n]);
static inline double error_rk(int n, double k[13][n]);
static inline void compute_ks(double t, double *x, int n, double h,
              double k[13][n], void (*ode)(double,double*,int,double*));
/**
 * @brief rkv89
 *    this routine performs one step of Runge-Kutta-Verner 8(9).
 *    the initial condition (at,x) is changed by a new one corresponding
 *     to the same orbit. the error is controlled by the threshold tol.
 *
 * @param at
 *    time. input: time corresponding to the actual initial condition.
 *          output: new value corresponding to the new initial condition.
 * @param x
 *    position. input: actual initial condition.
 *              output: new position at time *at.
 * @param n
 *    dimension of the system of odes.
 * @param ah
 *    time step (it can be modified by the routine according to the
 *    given threshold). if negative, the integration goes backwards.
 *    on exit, it will contain the time step for the next rkv89 call
 * @param sc
 *    stepsize control.
 *          0: no stepsize control, the step *ah is used
 *          !=0: stepsize control according to the threshold tol
 * @param tol
 *    threshold to control the integration error
 * @param atf
 *    final integration time. if NULL, it is ignored. Otherwise, if the
 *    stepsize is too large (*at+*ah>*atf), it is reduced so that the new
 *    time at is exactly atf (in that case, the function returns 1) *
 * @param aer
 *    if NULL, the routine stops if the estimated error is larger than tol.
 *    if not NULL, the integration returns the estimated absolute error of
 *    the performed step. this allows, for instance, to integrate with
 *    constant stepsize (sc == 0) and to know an estimate of the error. *
 * @param ode
 *    pointer to the the vectorfield. The parameters of the function are
 *    are: (t,x,n,f), where t is the time, x the position vector, n
 *    the dimension and f the value of the vectorfield at (t,x)
 * @param k
 * @return
 *    0: ok
 *    1: ok, and *at = *tf
 */
int rkv89(double *at, double *x, int n, double *ah, int sc, double tol,
          double *atf, double *aer, void (*ode)(double,double*,int,double*))
{
  double k[13][n],ea,ee,nr,s,t,h,hn;
  int i,j,flag=0;

  t=*at;
  h=*ah;
  if (atf != NULL)
    {
       if (h > 0) {if (t+h > *atf) {h=*atf-t; flag=1;}}
       else {if (t+h < *atf) {h=*atf-t; flag=1;}}
    }
  compute_ks(t,x,n,h,k,ode);
  if (sc == 0)
    {
      if (aer != NULL) *aer=error_rk(n,k);
      step_rk(x,n,k);
      *at=t+h;
      return flag;
    }
  for (j = 0; j < n; ++j) k[1][j]=x[j];
  nr=step_rk(x,n,k);
  ee=(1+nr)*tol/2;
  ea=error_rk(n,k)+1.e-16*nr;
  s=FC*exp(log(ee/ea)/9.e0);
  if (2.0 < s) s=2.0;
  hn=h*s;
  if (ea < ee)
    {
      if (aer != NULL) *aer=ea;
      *at=t+h;
      *ah=hn;
      return flag;
    }
  for (i=0; i<IS; i++)
  {
    for (j = 0; j < n; ++j) x[j]=k[1][j];
    h=hn;
    compute_ks(t,x,n,h,k,ode);
    nr=step_rk(x,n,k);
    ee=(1+nr)*tol/2;
    ea=error_rk(n,k)+1.e-16*nr;
    s=FC*exp(log(ee/ea)/9.e0);
    if (1.0 < s) s=1.0;
    hn=h*s;
    if (ea < ee)
      {
        if (aer != NULL) *aer=ea;
        *at=t+h;
        *ah=hn;
        return 0;
      }
  }
  if (aer == NULL)
    {
      puts("rkv89 error 1.");
      puts("this message appears because aer is NULL.");
      puts("It means that the stepsize cannot be adjusted to");
      puts("match the required accuracy. To (try to) solve it:");
      puts("* check that the initial stepsize is reasonable (and != 0)");
      puts("* reduce your accuracy requirement");
      printf("time: %g\n",t);
      printf("rk estimated error: %e  actual threshold: %e\n",ea,ee);
      printf("last stepsize: %g\n",h);
      exit(1);
    }
#if EW == 1
  printf("rkv89: t=%e  est. abserr=%e  est. relerr=%e\n",t,ea,ea/nr);
#endif
  *at=t+h;
  *ah=hn;
  *aer=ea;
  return 0;
}
static inline double step_rk(double *x, int n, double k[13][n])
{
  double nr;
  int j;

  nr = 0;
  for (j = 0; j < n; ++j)
  {
#if RK==8
    x[j]+= C80*k[0][j] + C813*k[3][j] + C87*k[7][j] + C88*k[8][j] + C89*k[9][j] + C810*k[10][j] + C811*k[11][j] + C812*k[12][j];
#elif RK==9
    x[j]+= C90*k[0][j] + C914*k[4][j] + C915*k[6][j] + C97*k[7][j] + C98*k[8][j] + C99*k[9][j] + C910*k[10][j] + C911*k[11][j] + C912*k[12][j];
#else
    puts("rkv89 error. the value of RK '#defined' in the rkv89.c file");
    puts("has to be either 8 or 9.");
    exit(1);
#endif
    nr+= x[j]*x[j];
  }

  return sqrt(nr/n);
}
static inline double error_rk(int n, double k[13][n])
{
  double s,e;
  int j;

  e = 0.0;
  for (j = 0; j < n; ++j)
    {
      s = E0*k[0][j] + E13*k[3][j] + E14*k[4][j] + E15*k[6][j] + E7*k[7][j] + E8*k[8][j] + E9*k[9][j] + E10*k[10][j] + E11*k[11][j] + E12*k[12][j];
      e+= s*s;
    }

  return sqrt(e/n);
}
static inline void compute_ks(double t, double *x, int n, double h,
              double k[13][n], void (*ode)(double,double*,int,double*))
{
  double ta;
  int j;

//k0:
  (*ode)(t,x,n,k[0]);
//k1:
  for (j = 0; j < n; ++j)
    {
      k[0][j]*= h;
      k[3][j] = x[j] + B10*k[0][j];
    }
  ta=t+A1*h;
  (*ode)(ta,k[3],n,k[2]);
//k2:
  for (j = 0; j < n; ++j)
    {
      k[2][j]*= h;
      k[3][j] = x[j] + B20*k[0][j] + B21*k[2][j];
    }
  ta=t+A2*h;
  (*ode)(ta,k[3],n,k[2]);
//k3:
  for (j = 0; j < n; ++j)
    {
      k[2][j]*= h;
      k[4][j] = x[j] + B30*k[0][j] + B32*k[2][j];
    }
  ta=t+A3*h;
  (*ode)(ta,k[4],n,k[3]);
//k4:
  for (j = 0; j < n; ++j)
    {
      k[3][j]*= h;
      k[5][j] = x[j] + B40*k[0][j] + B42*k[2][j] + B43*k[3][j];
    }
  ta=t+A4*h;
  (*ode)(ta,k[5],n,k[4]);
//k5:
  for (j = 0; j < n; ++j)
    {
      k[4][j]*= h;
      k[6][j] = x[j] + B50*k[0][j] + B53*k[3][j] + B54*k[4][j];
    }
  ta=t+A5*h;
  (*ode)(ta,k[6],n,k[5]);
//k6:
  for (j = 0; j < n; ++j)
    {
      k[5][j]*= h;
      k[7][j] = x[j] + B60*k[0][j] + B63*k[3][j] + B64*k[4][j] + B65*k[5][j];
    }
  ta=t+A6*h;
  (*ode)(ta,k[7],n,k[6]);
//k7:
  for (j = 0; j < n; ++j)
    {
      k[6][j]*= h;
      k[8][j] = x[j] + B70*k[0][j] + B75*k[5][j] + B76*k[6][j];
    }
  ta=t+A7*h;
  (*ode)(ta,k[8],n,k[7]);
//k8:
  for (j = 0; j < n; ++j)
    {
      k[7][j]*= h;
      k[9][j] = x[j] + B80*k[0][j] + B85*k[5][j] + B86*k[6][j] + B87*k[7][j];
    }
  ta=t+A8*h;
  (*ode)(ta,k[9],n,k[8]);
//k9:
  for (j = 0; j < n; ++j)
    {
      k[8][j]*= h;
      k[10][j] = x[j] + B90*k[0][j] + B95*k[5][j] + B96*k[6][j] + B97*k[7][j] + B98*k[8][j];
    }
  ta=t+A9*h;
  (*ode)(ta,k[10],n,k[9]);
//k10:
  for (j = 0; j < n; ++j)
    {
      k[9][j]*= h;
      k[11][j] = x[j] + B100*k[0][j] + B106*k[6][j] + B107*k[7][j] + B108*k[8][j] + B109*k[9][j];
    }
  ta=t+A10*h;
  (*ode)(ta,k[11],n,k[10]);
//k11:
  for (j = 0; j < n; ++j)
    {
      k[10][j]*= h;
      k[12][j] = x[j] + B110*k[0][j] + B115*k[5][j] + B116*k[6][j] + B117*k[7][j] + B118*k[8][j] + B119*k[9][j] + B1110*k[10][j];
    }
  ta=t+A11*h;
  (*ode)(ta,k[12],n,k[11]);
//k12:
  for (j = 0; j < n; ++j)
    {
      k[11][j]*= h;
      k[3][j] = x[j] + B120*k[0][j] + B125*k[5][j] + B126*k[6][j] + B127*k[7][j] + B128*k[8][j] + B129*k[9][j] + B1210*k[10][j] + B1211*k[11][j];
    }
  ta=t+A12*h;
  (*ode)(ta,k[3],n,k[12]);
//k13:
  for (j = 0; j < n; ++j)
    {
      k[12][j]*= h;
      k[2][j] = x[j] + B130*k[0][j] + B135*k[5][j] + B136*k[6][j] + B137*k[7][j] + B138*k[8][j] + B139*k[9][j] + B1310*k[10][j] + B1311*k[11][j] + B1312*k[12][j];
    }
  ta=t+h;
  (*ode)(ta,k[2],n,k[3]);
//k14:
  for (j = 0; j < n; ++j)
    {
      k[3][j]*= h;
      k[2][j] = x[j] + B140*k[0][j] + B145*k[5][j] + B146*k[6][j] + B147*k[7][j] + B148*k[8][j] + B149*k[9][j] + B1410*k[10][j] + B1411*k[11][j] + B1412*k[12][j];
    }
  ta=t+A14*h;
  (*ode)(ta,k[2],n,k[4]);
//k15:
  for (j = 0; j < n; ++j)
    {
      k[4][j]*= h;
      k[5][j] = x[j] + B150*k[0][j] + B1514*k[4][j] + B155*k[5][j] + B156*k[6][j] + B157*k[7][j] + B158*k[8][j] + B159*k[9][j] + B1510*k[10][j] + B1511*k[11][j] + B1512*k[12][j];
    }
  ta=t+h;
  (*ode)(ta,k[5],n,k[6]);
//k16:
  for (j = 0; j < n; ++j) k[6][j]*= h;

  return;
}

  
