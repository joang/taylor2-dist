/* ODE specification:  lorenz */
RR = 28.0;
x' = 10.0* (y - x);
y' = RR* x - x*z - y;
z' = x* y - 8.0* z /3.0;

/*
taylor -o lorenz1.c lorenz1.eq
*/





