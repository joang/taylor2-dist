extern MY_FLOAT RR;

x' = 10.0* (y - x);
y' = RR* x - x*z - y;
z' = x* y - 8.0* z /3.0;

jet x,y,z symbols 3 deg 1;

/*
taylor -o lorenz5.h -header lorenz5.eq
taylor -o lorenz5.c -jet -step -headername lorenz5.h lorenz5.eq -jethelper
*/
