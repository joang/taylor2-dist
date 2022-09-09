x' = 10.0* (y - x);
y' = RR* x - x*z - y;
z' = x* y - 8.0* z /3.0;
RR'= 0;

jet x,y,z,RR symbols 4 deg 1;

/*
taylor -o lorenz3.h -header lorenz3.eq -mpfr
taylor -o lorenz3.c -jet -step -headername lorenz3.h lorenz3.eq -jethelper -mpfr
*/

