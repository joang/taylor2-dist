#include "tp_prep.h"

extern BOOLEAN undefined;
extern int     errno;
extern struct  value stack[STACK_DEPTH];
extern int     s_p;

/***************************************************************************/
void f_real(), f_imag(), f_arg(), f_conjg(), f_sin(), f_cos(), f_tan(), f_asin(), f_acos(), f_atan(),
     f_sinh(), f_cosh(), f_tanh(), f_int(), f_abs(), f_sgn(), f_sqrt(), f_exp(), f_log10(), f_log(),
     f_besj0(), f_besj1(), f_besy0(), f_besy1(), f_floor(), f_rand(), f_srand(), f_ceil();
/***************************************************************************/
extern void push(struct value *), f_push(union argument *), f_pushc(union argument *), f_pushd0(union argument *),
     f_pushd1(union argument *), f_pushd2(union argument *), f_pushd3(union argument *), f_call(union argument *) ,
     reset_stack(), check_stack(), f_lnot(), f_bnot(), f_bool(), f_lor(), f_land(), f_bor(), f_xor(),
     f_band(), f_uminus(), f_eq(), f_ne(), f_gt(), f_lt(), f_ge(), f_le(), f_plus(), f_minus(), f_mult(),
     f_div(), f_mod(), f_power(), f_factorial();
/***************************************************************************/
void f_real()
{
struct value a;
	push( complex(&a,real(pop(&a)), 0.0) );
}

void f_imag()
{
struct value a;
	push( complex(&a,imag(pop(&a)), 0.0) );
}

void f_arg()
{
struct value a;
	push( complex(&a,angle(pop(&a)), 0.0) );
}

void f_conjg()
{
struct value a;
	(void) pop(&a);
	push( complex(&a,real(&a),-imag(&a) ));
}

void f_sin()
{
struct value a;
	(void) pop(&a);
	push( complex(&a,sin(real(&a))*cosh(imag(&a)), cos(real(&a))*sinh(imag(&a))) );
}

void f_cos()
{
struct value a;
	(void) pop(&a);
	push( complex(&a,cos(real(&a))*cosh(imag(&a)), -sin(real(&a))*sinh(imag(&a))));
}

void f_tan()
{
struct value a;
register double den;
	(void) pop(&a);
	if (imag(&a) == 0.0)
		push( complex(&a,tan(real(&a)),0.0) );
	else {
		den = cos(2*real(&a))+cosh(2*imag(&a));
		if (den == 0.0) {
			undefined = TRUE;
			push( &a );
		}
		else
			push( complex(&a,sin(2*real(&a))/den, sinh(2*imag(&a))/den) );
	}
}

void f_asin()
{
struct value a;
register double alpha, beta, x, y;
	(void) pop(&a);
	x = real(&a); y = imag(&a);
	if (y == 0.0) {
		if (fabs(x) > 1.0) {
			undefined = TRUE;
			push(complex(&a,0.0, 0.0));
		} else
			push( complex(&a,asin(x),0.0) );
	} else {
		beta  = sqrt((x + 1)*(x + 1) + y*y)/2 - sqrt((x - 1)*(x - 1) + y*y)/2;
		alpha = sqrt((x + 1)*(x + 1) + y*y)/2 + sqrt((x - 1)*(x - 1) + y*y)/2;
		push( complex(&a,asin(beta), log(alpha + sqrt(alpha*alpha-1))) );
	}
}

void f_acos()
{
struct value a;
register double alpha, beta, x, y;
	(void) pop(&a);
	x = real(&a); y = imag(&a);
	if (y == 0.0) {
		if (fabs(x) > 1.0) {
			undefined = TRUE;
			push(complex(&a,0.0, 0.0));
		} else
			push( complex(&a,acos(x),0.0) );
	} else {
		alpha = sqrt((x + 1)*(x + 1) + y*y)/2 + sqrt((x - 1)*(x - 1) + y*y)/2;
		beta  = sqrt((x + 1)*(x + 1) + y*y)/2 - sqrt((x - 1)*(x - 1) + y*y)/2;
		push( complex(&a,acos(beta), log(alpha + sqrt(alpha*alpha-1))) );
	}
}

void f_atan()
{
struct value a;
register double x, y;
	(void) pop(&a);
	x = real(&a); y = imag(&a);
	if (y == 0.0)
		push( complex(&a,atan(x), 0.0) );
	else if (x == 0.0 && fabs(y) == 1.0) {
		undefined = TRUE;
		push(complex(&a,0.0, 0.0));
	} else
		push( complex(&a,atan(2*x/(1-x*x-y*y)),
	    		log((x*x+(y+1)*(y+1))/(x*x+(y-1)*(y-1)))/4) );
}

void f_sinh()
{
struct value a;
	(void) pop(&a);
	push( complex(&a,sinh(real(&a))*cos(imag(&a)), cosh(real(&a))*sin(imag(&a))) );
}

void f_cosh()
{
struct value a;
	(void) pop(&a);
	push( complex(&a,cosh(real(&a))*cos(imag(&a)), sinh(real(&a))*sin(imag(&a))) );
}

void f_tanh()
{
struct value a;
register double den;
	(void) pop(&a);
	den = cosh(2*real(&a)) + cos(2*imag(&a));
	push( complex(&a,sinh(2*real(&a))/den, sin(2*imag(&a))/den) );
}

void f_int()
{
struct value a;
	push( integer(&a,(int)real(pop(&a))) );
}


void f_abs()
{
struct value a;
	(void) pop(&a);
	switch (a.type) {
		case INT:
			push( integer(&a,abs(a.v.int_val)) );			
			break;
		case CMPLX:
			push( complex(&a,magnitude(&a), 0.0) );
	}
}

void f_sgn()
{
struct value a;
	(void) pop(&a);
	switch(a.type) {
		case INT:
			push( integer(&a,(a.v.int_val > 0) ? 1 : 
					(a.v.int_val < 0) ? -1 : 0) );
			break;
		case CMPLX:
			push( integer(&a,(a.v.cmplx_val.real > 0.0) ? 1 : 
					(a.v.cmplx_val.real < 0.0) ? -1 : 0) );
			break;
	}
}


void f_sqrt()
{
struct value a;
register double mag, ang;
	(void) pop(&a);
	mag = sqrt(magnitude(&a));
	if (imag(&a) == 0.0 && real(&a) < 0.0)
		push( complex(&a,0.0,mag) );
	else
	{
		if ( (ang = angle(&a)) < 0.0)
			ang += 2*Pi;
		ang /= 2;
		push( complex(&a,mag*cos(ang), mag*sin(ang)) );
	}
}


void f_exp()
{
struct value a;
register double mag, ang;
	(void) pop(&a);
	mag = exp(real(&a));
	ang = imag(&a);
	push( complex(&a,mag*cos(ang), mag*sin(ang)) );
}


void f_log10()
{
struct value a;
register double l10;;
	(void) pop(&a);
	l10 = log(10.0);	/***** replace with a constant! ******/
	push( complex(&a,log(magnitude(&a))/l10, angle(&a)/l10) );
}


void f_log()
{
struct value a;
	(void) pop(&a);
	push( complex(&a,log(magnitude(&a)), angle(&a)) );
}


void f_besj0()	/* j0(a) = sin(a)/a */
{
struct value a;
	a = top_of_stack;
	f_sin();
	push(&a);
	f_div();
}


void f_besj1()	/* j1(a) = sin(a)/(a**2) - cos(a)/a */
{
struct value a;
	a = top_of_stack;
	f_sin();
	push(&a);
	push(&a);
	f_mult();
	f_div();
	push(&a);
	f_cos();
	push(&a);
	f_div();
	f_minus();
}


void f_besy0()	/* y0(a) = -cos(a)/a */
{
struct value a;
	a = top_of_stack;
	f_cos();
	push(&a);
	f_div();
	f_uminus();
}


void f_besy1()	/* y1(a) = -cos(a)/(a**2) - sin(a)/a */
{
struct value a;

	a = top_of_stack;
	f_cos();
	push(&a);
	push(&a);
	f_mult();
	f_div();
	push(&a);
	f_sin();
	push(&a);
	f_div();
	f_plus();
	f_uminus();
}


void f_floor()
{
struct value a;

	(void) pop(&a);
	switch (a.type) {
		case INT:
			push( integer(&a,(int)floor((double)a.v.int_val)));			
			break;
		case CMPLX:
			push( complex(&a,floor(a.v.cmplx_val.real),
				floor(a.v.cmplx_val.imag)) );
	}
}

void f_rand()
{
  struct value a;
  push( integer(&a,(int)lrand48()));
}

void f_srand()
{
  struct value a;
  (void) pop(&a);
  srand48( (long) a.v.int_val);
  push( &a);
}


void f_ceil()
{
struct value a;

	(void) pop(&a);
	switch (a.type) {
		case INT:
			push( integer(&a,(int)ceil((double)a.v.int_val)));			
			break;
		case CMPLX:
			push( complex(&a,ceil(a.v.cmplx_val.real), ceil(a.v.cmplx_val.imag)) );
	}
}


/****************************************************************************/
