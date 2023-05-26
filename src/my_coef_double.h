#ifndef MY_COEF_DOUBLE_H
#define MY_COEF_DOUBLE_H

/* *******  *********************  ******* */
/* *******  MY_COEF_DOUBLE_HEADER  ******* */
/* *******  *********************  ******* */

#define MY_COEF_DOUBLE_FIXES MY_COEF_SPACE

#define MY_COEF_DOUBLE_PREHEADER(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
#define " PREFIX_COEF(t) "          double\n\
" \

#define MY_COEF_DOUBLE_HEADER(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
#define " PREFIX_COEF(initup2) "(s,d)   {}\n\
#define " PREFIX_COEF(init) "(s)        {}\n\
#define " PREFIX_COEF(clean) "(s)       {}\n\
#define " PREFIX_COEF(cleanup) "()      {}\n\
\n\
/* assign */\n\
#define " PREFIX_COEF(set) "(a,b)       ((a)=(b))\n\
#define " PREFIX_COEF(set_zero) "(r)    " PREFIX_COEF(set) "(r,0.0e0)\n\
#define " PREFIX_COEF(set_d) "(a,b)     " PREFIX_COEF(set) "(a,b)\n\
#define " PREFIX_COEF(set_si) "(a,b)    " PREFIX_COEF(set) "(a,b)\n\
#define " PREFIX_COEF(set_str) "(a,b)   " PREFIX_COEF(set) "(a,atof(b))\n\
#define " PREFIX_COEF(set_myfloat) "(a,b) undefined\n\
\n\
/* elemental operations */\n\
#define " PREFIX_COEF(add2) "(a,b,c)    ((a)=(b)+(c))\n\
#define " PREFIX_COEF(sub2) "(a,b,c)    ((a)=(b)-(c))\n\
#define " PREFIX_COEF(mul2) "(a,b,c)    ((a)=(b)*(c))\n\
#define " PREFIX_COEF(div2) "(a,b,c)    ((a)=(b)/(c))\n\
\n\
#define " PREFIX_COEF(add2_myfloat) "(a,b,c)  undefined\n\
#define " PREFIX_COEF(sub2_myfloat) "(a,b,c)  undefined\n\
#define " PREFIX_COEF(myfloat_sub2) "(a,b,c)  undefined\n\
#define " PREFIX_COEF(mul2_myfloat) "(a,b,c)  undefined\n\
#define " PREFIX_COEF(div2_myfloat) "(a,b,c)  undefined\n\
#define " PREFIX_COEF(myfloat_div2) "(a,b,c)  undefined\n\
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
#define " PREFIX_COEF(set_sqrt) "(a,b)  ((a)=sqrt(b))\n\
#define " PREFIX_COEF(set_pow) "(a,b,c) ((a)=pow(b,c))\n\
#define " PREFIX_COEF(set_pow_myfloat) " undefined\n\
#define " PREFIX_COEF(set_pow_si) "     " PREFIX_COEF(set_pow) "\n\
\n\
#define " PREFIX_COEF(set_sin) "(a,b)   ((a)=sin(b))\n\
#define " PREFIX_COEF(set_cos) "(a,b)   ((a)=cos(b))\n\
#define " PREFIX_COEF(set_tan) "(a,b)   ((a)=tan(b))\n\
#define " PREFIX_COEF(set_atan) "(a,b)  ((a)=atan(b))\n\
#define " PREFIX_COEF(set_exp) "(a,b)   ((a)=exp(b))\n\
#define " PREFIX_COEF(set_log) "(a,b)   ((a)=log(b))\n\
#define " PREFIX_COEF(set_sinh) "(a,b)  ((a)=sinh(b))\n\
#define " PREFIX_COEF(set_cosh) "(a,b)  ((a)=cosh(b))\n\
#define " PREFIX_COEF(set_tanh) "(a,b)  ((a)=tanh(b))\n\
#define " PREFIX_COEF(set_log10) "(a,b) ((a)=log10(b))\n\
#define " PREFIX_COEF(set_fabs) "(a,b)  ((a)=fabs(b))\n\
\n\
/* castings */\n\
#define " PREFIX_COEF(to_myfloat) "(a,b) undefined\n\
#define " PREFIX_COEF(to_si) "(a,b)     ((a)=(int)(b))\n\
#define " PREFIX_COEF(to_d) "(a,b)      ((a)=(double)(b))\n\
#define " PREFIX_COEF(to_str) "(a,b)    (sprintf(a,\"% .15e\",b))\n\
\n\
/* boolean operations */\n\
#define " PREFIX_COEF(ge) "(a,b)        ((a)>=(b))\n\
#define " PREFIX_COEF(gt) "(a,b)        ((a)> (b))\n\
#define " PREFIX_COEF(le) "(a,b)        ((a)<= (b))\n\
#define " PREFIX_COEF(lt) "(a,b)        ((a)< (b))\n\
#define " PREFIX_COEF(eq) "(a,b)        ((a)==(b))\n\
#define " PREFIX_COEF(neq) "(a,b)       ((a)!=(b))\n\
\n\
/* output format */\n\
#define " PREFIX_COEF(fprintf) "(file,fmt,a) fprintf(file,fmt,(a))\n\
" \

#define MY_COEF_DOUBLE_POSTHEADER(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
#define MY_COEF " PREFIX_COEF(t) "\n\
" \

/* *******  *******************  ******* */
/* *******  MY_COEF_DOUBLE_CODE  ******* */
/* *******  *******************  ******* */
#define MY_COEF_DOUBLE_PRECODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" \

#define MY_COEF_DOUBLE_CODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" \

#define MY_COEF_DOUBLE_POSTCODE(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_COEF_MACROS(PREFIX_COEF,MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME) " \n\
" \

#endif /* MY_COEF_DOUBLE_H */
