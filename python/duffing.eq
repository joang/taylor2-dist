
//k=0.3;
//gamma = 0.4;
//omega = 1.2;

extern omega;

k = 0.1;
gamma=0.38;
#omega=1.4;

x'=y;
y'=-k*y +x -x^3 + gamma *cos(omega *t);

expr poincare_section = cos(omega*t);



initialValues=0.1,1.0,0.0;
absoluteErrorTolerance = 1.0E-16;      /* error tolerance for step control */
relativeErrorTolerance = 1.0E-16;      /* error tolerance for step control */
stopTime = 1000000;                    /* stop time */
startTime = 0.0;                       /* start time */
number_of_steps=12345678;
number_of_intersections=20000;
crossing=0;


# poincare_section_initials = x, y
# poincare_section_xlimit = -2.0,2.0;
# Poincare_section_ylimit = -2.0,2.0;
# Poincare_section_slider1 = "Omega", 1.0, 2.0, 0.01, 1.2, omega;

# poincare_section_mouse = x, y;
# poincare_section_columns=0,1

