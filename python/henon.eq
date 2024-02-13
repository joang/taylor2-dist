//extern mu, nu;

x'= xp;
y'= yp;
xp'= -x -2*x*y;
yp'= -y -x^2 + y^2;

number_of_intersections=2000;
initialValues=0.0,0.1,0.39,0.2 ;
startTime = 0.0;
stopTime = 1.0e6;
crossing=1;
number_of_steps=12345678;
absoluteErrorTolerance = 1.0E-16;      /* error tolerance for step control */
relativeErrorTolerance = 1.0E-16;      /* error tolerance for step control */


expr Poincare_section = x ;

# poincare_section_initials = 0, y,  1./3.*sqrt(6.*y*y*y-9.*y*y-9*yp*yp+18.*HH), yp;
# poincare_section_xlimit = -0.6,0.6;
# Poincare_section_ylimit = -0.7,0.7;
# Poincare_section_slider1 = "Hamiltonian", 0.0, 0.16, 0.01, 0.1, HH;
//Poincare_section_slider2 = "Mu", 1.0, 10.0, 0.5, 5, mu;
//Poincare_section_slider3 = "XY", 1.0, 10.0, 0.5, 5, nu;
# poincare_section_mouse = y, yp;


