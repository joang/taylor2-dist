x'= xp;
y'= yp;
xp'= -x -2*x*y;
yp'= -y -x*x + y*y;

expr Hamiltonian= 0.5*(xp*xp+yp*yp) + 0.5*(x*x+y*y+2*x*x*y -2./3. * y*y*y);

initialValues=0.0,0.1,0.39,0.2 ;
absoluteErrorTolerance = 1.0E-16;
relativeErrorTolerance = 1.0E-16;
stopTime = 100;
startTime = 0.0; 


# ../../../python/ptaylor.py -mpfr 512 -abs_err 1E-150 -rel_err 1E-150 -i henon.eq -o ex2.py -m ex2
