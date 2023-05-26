#ifndef MY_COEF_COMPLEX_H
#define MY_COEF_COMPLEX_H

#define MY_COEF_COMPLEX_FIXES MY_COEF_SPACE

/* *******  **********************  ******* */
/* *******  MY_COEF_COMPLEX_HEADER  ******* */
/* *******  **********************  ******* */
#define MY_COEF_COMPLEX_PREHEADER(PREFIX_COEF) "\
#include <complex.h>\n\
typedef double complex MY_COEF;\n\
\n\
" \

#define MY_COEF_COMPLEX_HEADER(PREFIX_COEF) "\
#define " PREFIX_COEF(t) "          MY_COEF\n\
#define " PREFIX_COEF(init) "       {}\n\
#define " PREFIX_COEF(clean) "      {}\n\
\n\
/* assign */\n\
#define " PREFIX_COEF(set) "(a,b)       ((a)=(b))\n\
#define " PREFIX_COEF(set_zero) "(r)    " PREFIX_COEF(set) "(r,0.0e0)\n\
#define " PREFIX_COEF(set_d) "(a,b)     " PREFIX_COEF(set) "(a,b)\n\
#define " PREFIX_COEF(set_si) "(a,b)    " PREFIX_COEF(set) "(a,b)\n\
#define " PREFIX_COEF(set_str) "(a,b)   {double r__,i__; sscanf(b, \"%lf %lf\", &r__, &i__); a=r__+_Complex_I*i__;}\n\
#define " PREFIX_COEF(set_myfloat) "(a,b) " PREFIX_COEF(set_str) "(a,MyFloatToString(b))\n\
\n\
/* elemental operations */\n\
#define " PREFIX_COEF(add2) "(a,b,c)    ((a)=(b)+(c))\n\
#define " PREFIX_COEF(sub2) "(a,b,c)    ((a)=(b)-(c))\n\
#define " PREFIX_COEF(mul2) "(a,b,c)    ((a)=(b)*(c))\n\
#define " PREFIX_COEF(div2) "(a,b,c)    ((a)=(b)/(c))\n\
\n\
#define " PREFIX_COEF(add2_d) "(a,b,c)  " PREFIX_COEF(add2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(sub2_d) "(a,b,c)  " PREFIX_COEF(sub2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(d_sub2) "(a,b,c)  " PREFIX_COEF(sub2) "(a,(double)(b),c)\n\
#define " PREFIX_COEF(mul2_d) "(a,b,c)  " PREFIX_COEF(mul2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(div2_d) "(a,b,c)  " PREFIX_COEF(div2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(d_div2) "(a,b,c)  " PREFIX_COEF(div2) "(a,(double)(b),c)\n\
\n\
#define " PREFIX_COEF(add2_si) "(a,b,c) " PREFIX_COEF(add2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(sub2_si) "(a,b,c) " PREFIX_COEF(sub2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(si_sub2) "(a,b,c) " PREFIX_COEF(sub2) "(a,(double)(b),c)\n\
#define " PREFIX_COEF(mul2_si) "(a,b,c) " PREFIX_COEF(mul2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(div2_si) "(a,b,c) " PREFIX_COEF(div2) "(a,b,(double)(c))\n\
#define " PREFIX_COEF(si_div2) "(a,b,c) " PREFIX_COEF(div2) "(a,(double)(b),c)\n\
\n\
#define " PREFIX_COEF(neg) "(a,b)       ((a)=-(b))\n\
\n\
/* built-in functions */\n\
#define " PREFIX_COEF(set_sqrt) "(a,b)  ((a)=csqrt(b))\n\
#define " PREFIX_COEF(set_pow) "(a,b,c) ((a)=cpow(b,c))\n\
#define " PREFIX_COEF(set_pow_si) "     " PREFIX_COEF(set_pow) "\n\
\n\
#define " PREFIX_COEF(set_sin) "(a,b)   ((a)=csin(b))\n\
#define " PREFIX_COEF(set_cos) "(a,b)   ((a)=ccos(b))\n\
#define " PREFIX_COEF(set_tan) "(a,b)   ((a)=ctan(b))\n\
#define " PREFIX_COEF(set_atan) "(a,b)  ((a)=catan(b))\n\
#define " PREFIX_COEF(set_exp) "(a,b)   ((a)=cexp(b))\n\
#define " PREFIX_COEF(set_log) "(a,b)   ((a)=clog(b))\n\
#define " PREFIX_COEF(set_sinh) "(a,b)  ((a)=csinh(b))\n\
#define " PREFIX_COEF(set_cosh) "(a,b)  ((a)=ccosh(b))\n\
#define " PREFIX_COEF(set_tanh) "(a,b)  ((a)=ctanh(b))\n\
#define " PREFIX_COEF(set_log10) "(a,b) ((a)=clog10(b))\n\
#define " PREFIX_COEF(set_fabs) "(a,b)  ((a)=cabs(b))\n\
\n\
/* castings */\n\
#define " PREFIX_COEF(to_si) "(a,b)     ((a)=(int)creal(b))\n\
#define " PREFIX_COEF(to_d) "(a,b)      ((a)=(double)creal(b))\n\
#define " PREFIX_COEF(to_str) "(a,b)    sprintf(a,\"% .15e % .15e\",creal(b),cimag(b))\n\
#define " PREFIX_COEF(to_myfloat) "(a,b){char s__[64];" PREFIX_COEF(to_str) "(s__,b);MakeMyFloatC(a,s__,b);}\n\
\n\
/* boolean operations */\n\
#define " PREFIX_COEF(ge) "(a,b)        (creal(a)>=creal(b))\n\
#define " PREFIX_COEF(gt) "(a,b)        (creal(a)> creal(b))\n\
#define " PREFIX_COEF(le) "(a,b)        (creal(a)<=creal(b))\n\
#define " PREFIX_COEF(lt) "(a,b)        (creal(a)< creal(b))\n\
#define " PREFIX_COEF(eq) "(a,b)        (creal(a)==creal(b))\n\
#define " PREFIX_COEF(neq) "(a,b)       (creal(a)!=creal(b))\n\
\n\
/* output format */\n\
#define " PREFIX_COEF(fprintf) "(file,fmt,a) fprintf(file,fmt,creal(a),cimag(a))\n\
" \

#define MY_COEF_COMPLEX_POSTHEADER(PREFIX_COEF) "\
" \

/* *******  ********************  ******* */
/* *******  MY_COEF_COMPLEX_CODE  ******* */
/* *******  ********************  ******* */
#define MY_COEF_COMPLEX_PRECODE(PREFIX_COEF) "\
" \

#define MY_COEF_COMPLEX_CODE(PREFIX_COEF) "\
" \

#define MY_COEF_COMPLEX_POSTCODE(PREFIX_COEF) "\
" \

#endif /* MY_COEF_COMPLEX_H */
