
x' = -y + x *( 1-x*x -y*y);
y' =  x + y *( 1-x*x -y*y);
r' = r*(1-r*r);

jet x,y  symbols 2 deg 3;

/*
taylor -header -name jdata -o jdata.h model.eq
taylor -jet -jhelper -name jdata -step -header_name jdata.h model.eq -o jdata.c
*/

