extern MY_FLOAT RR,SS;

x' = SS* (y - x);
y' = RR* x - x*z - y;
z' = x* y - 8.0* z /3.0;

jet x,y,z symbols 3 deg 5;

/*
taylor -o lorenz4.h -header lorenz4.eq
taylor -o lorenz4.c -jet -step -headername lorenz4.h lorenz4.eq -jethelper
*/
