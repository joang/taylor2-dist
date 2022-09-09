/* control parameters */
initialValues = 0.03,-0.02,0.15,0,0; /* initial values */
absoluteErrorTolerance = 1.0E-16;      /* error tolerance for step control */
relativeErrorTolerance = 1.0E-16;      /* error tolerance for step control */
stopTime = 0.3;                        /* stop time */
startTime = 0.0;                       /* start time */


/* ODE specification:  lorenz */
RR = 28.0;
x' = 10.0* (y - x);
y' = RR* x - x*z - y;
z' = x* y - 8.0* z /3.0;



