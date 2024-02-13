x'= xp;
y'= yp;
xp'= -x -2*x*y;
yp'= -y -x**2 + y**2;

initialValues=-0.86659,-0.4999,0.00017,0.00099;
absoluteErrorTolerance = 1.0E-16;
relativeErrorTolerance = 1.0E-16;
stopTime = 14;
startTime = 0.0;

jet all symbols 2 degree 1;

jetInitialValues x ="(-0.86659   1 0  )";
jetInitialValues y ="(-0.49990   0 1  )";
jetInitialValues xp ="(0.00017   0 0  )";
jetInitialValues yp ="(0.00099   0 0  )";


# ../../../python/ptaylor.py -i henon2.eq -o ex4.py -m ex4
