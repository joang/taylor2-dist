
#include "tp_prep.h"

extern BOOLEAN   undefined;

struct value     stack[STACK_DEPTH];
int               s_p = -1;   /* stack pointer */

/*************************************************************/      
void push(struct value *), f_push(union argument *), f_pushc(union argument *), f_pushd0(union argument *),
     f_pushd1(union argument *), f_pushd2(union argument *), f_pushd3(union argument *), f_call(union argument *) ,
     reset_stack(), check_stack(), f_lnot(), f_bnot(), f_bool(), f_lor(), f_land(), f_bor(), f_xor(),
     f_band(), f_uminus(), f_eq(), f_ne(), f_gt(), f_lt(), f_ge(), f_le(), f_plus(), f_minus(), f_mult(),
     f_div(), f_mod(), f_power(), f_factorial();
/*************************************************************/      
extern void execute_at(struct at_type *at_ptr);
/*************************************************************/      

void reset_stack()
{
  s_p = -1;
}

/*************************************************************/      

void check_stack()	/* make sure stack's empty */
{
  if (s_p != -1)
    (void)fprintf(STDERRR,"\nwarning:  internal error--stack not empty!\n");
}

/*************************************************************/      

struct value *pop( struct value *x)
{
#ifdef DEBUG
  if (s_p  < 0 )
    int_error("stack underflow",NO_CARET);
#endif
  *x = stack[s_p--];
  return(x);
}

/*************************************************************/      

void push(  struct value *x)
{
#ifdef DEBUG
  if (s_p == STACK_DEPTH - 1)
    int_error("stack overflow",NO_CARET);
#endif
  stack[++s_p] = *x;
}

/*************************************************************/      

#define ERR_VAR "undefined variable: "
void f_push( union argument *x)		/* contains pointer to value to push; */
{
  static char err_str[sizeof(ERR_VAR) + MAX_ID_LEN] = ERR_VAR;
  struct udvt_entry *udv;

  udv = x->udv_arg;
  if (udv->udv_undef)  /* undefined */
    {	 
      (void) strcpy(&err_str[sizeof(ERR_VAR) - 1], udv->udv_name);
      int_error(err_str,NO_CARET);
    }
  push(&(udv->udv_value));
}

/*************************************************************/      

void f_pushc(  union argument *x)
{
  push(&(x->v_arg));
}

/*************************************************************/      

void f_pushd0(    union argument *x)
{
  push(&((x->udf_arg->dummy_value)));
}

/*************************************************************/      

void f_pushd1(     union argument *x)
{
  push(&((x->udf_arg->dummy_value1)));
}

/*************************************************************/      

void f_pushd2(     union argument *x)
{
  push(&((x->udf_arg->dummy_value2)));
}

/*************************************************************/      

void f_pushd3(     union argument *x)
{
  push(&((x->udf_arg->dummy_value3)));
}

/*************************************************************/      

#define ERR_FUN "undefined function: "
void f_call(union argument *x)  /* execute a udf */
{
  static   char err_str[sizeof(ERR_FUN) + MAX_ID_LEN] = ERR_FUN;
  register struct udft_entry *udf;

  udf = x->udf_arg;
  if (!udf->at)  /* undefined */
    {
      (void) strcpy(&err_str[sizeof(ERR_FUN) - 1],
		    udf->udf_name);
      int_error(err_str,NO_CARET);
    }
  if(udf->n_arg == 4)
    (void) pop(&((udf->dummy_value3)));
  if(udf->n_arg >= 3)
    (void) pop(&((udf->dummy_value2)));
  if(udf->n_arg >= 2)
    (void) pop(&((udf->dummy_value1)));
  (void) pop(&((udf->dummy_value)));
  execute_at(udf->at);
}

/*************************************************************/      

static void int_check(    struct value *v)
{
  if (v->type != INT)
    int_error("non-integer passed to boolean operator",NO_CARET);
}

/*************************************************************/      

void f_lnot()
{
  struct value a;
  int_check(pop(&a));
  push(integer(&a,!a.v.int_val) );
}

/*************************************************************/      

void f_bnot()
{
  struct value a;
  int_check(pop(&a));
  push( integer(&a,~a.v.int_val) );
}

/*************************************************************/      

void f_bool()
{			/* converts top-of-stack to boolean */
  int_check(&top_of_stack);
  top_of_stack.v.int_val = !!top_of_stack.v.int_val;
}

/*************************************************************/      

void f_lor()
{
  struct value a,b;
  int_check(pop(&b));
  int_check(pop(&a));
  push( integer(&a,a.v.int_val || b.v.int_val) );
}

/*************************************************************/      

void f_land()
{
  struct value a,b;
  int_check(pop(&b));
  int_check(pop(&a));
  push( integer(&a,a.v.int_val && b.v.int_val) );
}

/*************************************************************/      

void f_bor()
{
  struct value a,b;
  int_check(pop(&b));
  int_check(pop(&a));
  push( integer(&a,a.v.int_val | b.v.int_val) );
}

/*************************************************************/      

void f_xor()
{
  struct value a,b;
  int_check(pop(&b));
  int_check(pop(&a));
  push( integer(&a,a.v.int_val ^ b.v.int_val) );
}

/*************************************************************/      

void f_band()
{
  struct value a,b;
  int_check(pop(&b));
  int_check(pop(&a));
  push( integer(&a,a.v.int_val & b.v.int_val) );
}

/*************************************************************/      

void f_uminus()
{
  struct value a;
  (void) pop(&a);
  switch(a.type) {
  case INT:
    a.v.int_val = -a.v.int_val;
    break;
  case CMPLX:
    a.v.cmplx_val.real =
      -a.v.cmplx_val.real;
    a.v.cmplx_val.imag =
      -a.v.cmplx_val.imag;
  }
  push(&a);
}

/*************************************************************/      

void f_eq() /* note: floating point equality is rare because of roundoff error! */
{
  struct value a, b;
  register int result=0;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) {
      case INT:
	result = (a.v.int_val ==
		  b.v.int_val);
	break;
      case CMPLX:
	result = (a.v.int_val ==
		  b.v.cmplx_val.real &&
		  b.v.cmplx_val.imag == 0.0);
      }
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  result = (b.v.int_val == a.v.cmplx_val.real &&
		    a.v.cmplx_val.imag == 0.0);
	  break;
	case CMPLX:
	  result = (a.v.cmplx_val.real==
		    b.v.cmplx_val.real &&
		    a.v.cmplx_val.imag==
		    b.v.cmplx_val.imag);
	}
    }
  push(integer(&a,result));
}

/*************************************************************/      

void f_ne()
{
  struct value a, b;
  register int result=0;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.int_val !=
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.int_val !=
		    b.v.cmplx_val.real ||
		    b.v.cmplx_val.imag != 0.0);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  result = (b.v.int_val !=
		    a.v.cmplx_val.real ||
		    a.v.cmplx_val.imag != 0.0);
	  break;
	case CMPLX:
	  result = (a.v.cmplx_val.real !=
		    b.v.cmplx_val.real ||
		    a.v.cmplx_val.imag !=
		    b.v.cmplx_val.imag);
	}
    }
  push(integer(&a,result));
}

/*************************************************************/      

void f_gt()
{
  struct value a, b;
  register int result=0;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.int_val >
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.int_val >
		    b.v.cmplx_val.real);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.cmplx_val.real >
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.cmplx_val.real >
		    b.v.cmplx_val.real);
	}
    }
  push(integer(&a,result));
}

/*************************************************************/      

void f_lt()
{
  struct value a, b;
  register int result=0;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.int_val <
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.int_val <
		    b.v.cmplx_val.real);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.cmplx_val.real <
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.cmplx_val.real <
		    b.v.cmplx_val.real);
	}
    }
  push(integer(&a,result));
}

/*************************************************************/      

void f_ge()
{
  struct value a, b;
  register int result=0;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.int_val >=
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.int_val >=
		    b.v.cmplx_val.real);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.cmplx_val.real >=
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.cmplx_val.real >=
		    b.v.cmplx_val.real);
	}
    }
  push(integer(&a,result));
}

/*************************************************************/      

void f_le()
{
  struct value a, b;
  register int result=0;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.int_val <=
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.int_val <=
		    b.v.cmplx_val.real);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  result = (a.v.cmplx_val.real <=
		    b.v.int_val);
	  break;
	case CMPLX:
	  result = (a.v.cmplx_val.real <=
		    b.v.cmplx_val.real);
	}
    }
  push(integer(&a,result));
}

/*************************************************************/      

void f_plus()
{
  struct value a, b, result;
  (void) pop(&b);
  (void) pop(&a);
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  (void) integer(&result,a.v.int_val +
			 b.v.int_val);
	  break;
	case CMPLX:
	  (void) complex(&result,a.v.int_val +
			 b.v.cmplx_val.real,
			 b.v.cmplx_val.imag);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  (void) complex(&result,b.v.int_val +
			 a.v.cmplx_val.real,
			 a.v.cmplx_val.imag);
	  break;
	case CMPLX:
	  (void) complex(&result,a.v.cmplx_val.real+
			 b.v.cmplx_val.real,
			 a.v.cmplx_val.imag+
			 b.v.cmplx_val.imag);
	}
    }
  push(&result);
}

/*************************************************************/      

void f_minus()
{
  struct value a, b, result;
  (void) pop(&b);
  (void) pop(&a);		/* now do a - b */
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  (void) integer(&result,a.v.int_val -
			 b.v.int_val);
	  break;
	case CMPLX:
	  (void) complex(&result,a.v.int_val -
			 b.v.cmplx_val.real,
			 -b.v.cmplx_val.imag);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  (void) complex(&result,a.v.cmplx_val.real -
			 b.v.int_val,
			 a.v.cmplx_val.imag);
	  break;
	case CMPLX:
	  (void) complex(&result,a.v.cmplx_val.real-
			 b.v.cmplx_val.real,
			 a.v.cmplx_val.imag-
			 b.v.cmplx_val.imag);
	}
    }
  push(&result);
}

/************************************************************/

void f_mult()
{
  struct value a, b, result;
  (void) pop(&b);
  (void) pop(&a);	/* now do a*b */
  
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  (void) integer(&result,a.v.int_val *
			 b.v.int_val);
	  break;
	case CMPLX:
	  (void) complex(&result,a.v.int_val *
			 b.v.cmplx_val.real,
			 a.v.int_val *
			 b.v.cmplx_val.imag);
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  (void) complex(&result,b.v.int_val *
			 a.v.cmplx_val.real,
			 b.v.int_val *
			 a.v.cmplx_val.imag);
	  break;
	case CMPLX:
	  (void) complex(&result,a.v.cmplx_val.real*
			 b.v.cmplx_val.real-
			 a.v.cmplx_val.imag*
			 b.v.cmplx_val.imag,
			 a.v.cmplx_val.real*
			 b.v.cmplx_val.imag+
			 a.v.cmplx_val.imag*
			 b.v.cmplx_val.real);
	}
    }
  push(&result);
}

/************************************************************/

void f_div()
{
  struct value a, b, result;
  register double square;
  (void) pop(&b);
  (void) pop(&a);	/* now do a/b */
  
  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  if (b.v.int_val)
	    (void) integer(&result,a.v.int_val /
			   b.v.int_val);
	  else 
	    {
	      (void) integer(&result,0);
	      undefined = TRUE;
	    }
	  break;
	case CMPLX:
	  square = b.v.cmplx_val.real*
	    b.v.cmplx_val.real +
	      b.v.cmplx_val.imag*
		b.v.cmplx_val.imag;
	  if (square)
	    (void) complex(&result,a.v.int_val*
			   b.v.cmplx_val.real/square,
			   -a.v.int_val*
			   b.v.cmplx_val.imag/square);
	  else 
	    {
	      (void) complex(&result,0.0,0.0);
	      undefined = TRUE;
	    }
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  if (b.v.int_val)
	
	    (void) complex(&result,a.v.cmplx_val.real/
			   b.v.int_val,
			   a.v.cmplx_val.imag/
			   b.v.int_val);
	  else 
	    {
	      (void) complex(&result,0.0,0.0);
	      undefined = TRUE;
	    }
	  break;
	case CMPLX:
	  square = b.v.cmplx_val.real*
	    b.v.cmplx_val.real +
	      b.v.cmplx_val.imag*
		b.v.cmplx_val.imag;
	  if (square)
	    (void) complex(&result,(a.v.cmplx_val.real*
				    b.v.cmplx_val.real+
				    a.v.cmplx_val.imag*
				    b.v.cmplx_val.imag)/square,
			   (a.v.cmplx_val.imag*
			    b.v.cmplx_val.real-
			    a.v.cmplx_val.real*
			    b.v.cmplx_val.imag)/
			   square);
	  else 
	    {
	      (void) complex(&result,0.0,0.0);
	      undefined = TRUE;
	    }
	}
    }
  push(&result);
}

/************************************************************/

void f_mod()
{
  struct value a, b;
  (void) pop(&b);
  (void) pop(&a);	/* now do a%b */
  
  if (a.type != INT || b.type != INT) 
    int_error("can only mod ints",NO_CARET);
  if (b.v.int_val)
    push(integer(&a,a.v.int_val % b.v.int_val));
  else {
    push(integer(&a,0));
    undefined = TRUE;
  }
}

/************************************************************/

void f_power()
{
  struct value a, b, result;
  register int i, t, count;
  register double mag, ang;
  (void) pop(&b);
  (void) pop(&a);	/* now find a**b */

  switch(a.type) 
    {
    case INT:
      switch (b.type) 
	{
	case INT:
	  count = abs(b.v.int_val);
	  t = 1;
	  for(i = 0; i < count; i++)
	    t *= a.v.int_val;
	  if (b.v.int_val >= 0)
	    (void) integer(&result,t);
	  else
	    (void) complex(&result,1.0/t,0.0);
	  break;
	case CMPLX:
	  mag =
	    pow(magnitude(&a),fabs(b.v.cmplx_val.real));
	  if (b.v.cmplx_val.real < 0.0)
	    mag = 1.0/mag;
	  ang = angle(&a)*b.v.cmplx_val.real+
	    b.v.cmplx_val.imag;
	  (void) complex(&result,mag*cos(ang),
			 mag*sin(ang));
	}
      break;
    case CMPLX:
      switch (b.type) 
	{
	case INT:
	  if (a.v.cmplx_val.imag == 0.0) 
	    {
	      mag = pow(a.v.cmplx_val.real,(double)abs(b.v.int_val));
	      if (b.v.int_val < 0)
		mag = 1.0/mag;
	      (void) complex(&result,mag,0.0);
	    }
	  else 
	    {
	      /* not so good, but...! */
	      mag = pow(magnitude(&a),(double)abs(b.v.int_val));
	      if (b.v.int_val < 0)
		mag = 1.0/mag;
	      ang = angle(&a)*b.v.int_val;
	      (void) complex(&result,mag*cos(ang),
			     mag*sin(ang));
	    }
	  break;
	case CMPLX:
	  mag = pow(magnitude(&a),fabs(b.v.cmplx_val.real));
	  if (b.v.cmplx_val.real < 0.0)
	    mag = 1.0/mag;
	  ang = angle(&a)*b.v.cmplx_val.real+ b.v.cmplx_val.imag;
	  (void) complex(&result,mag*cos(ang),
			 mag*sin(ang));
	}
    }
  push(&result);
}

/************************************************************/

void f_factorial()
{
  struct value a;
  register int i;
  register double val=0;
  
  (void) pop(&a);	/* find a! (factorial) */
  
  switch (a.type) 
    {
    case INT:
      val = 1.0;
      for (i = a.v.int_val; i > 1; i--)  /*fpe's should catch overflows*/
	val *= i;
      break;
    default:
      int_error("factorial (!) argument must be an integer",
		NO_CARET);
    }
  
  push(complex(&a,val,0.0));
}

/************************************************************/

int   f_jump(     union argument *x)
{
  return(x->j_arg);
}

/************************************************************/

int  f_jumpz(     union argument *x)
{
  struct value a;
  int_check(&top_of_stack);
  if (top_of_stack.v.int_val) 	/* non-zero */
    {
      (void) pop(&a);
      return 1;				/* no jump */
    }
  else
    return(x->j_arg);		/* leave the argument on TOS */
}

/************************************************************/

int f_jumpnz(     union argument *x)
{
  struct value a;
  int_check(&top_of_stack);
  if (top_of_stack.v.int_val)	/* non-zero */
    return(x->j_arg);		/* leave the argument on TOS */
  else
    {
      (void) pop(&a);
      return 1;				/* no jump */
    }
}

/************************************************************/

int f_jtern(     union argument *x)
{
  struct value a;

  int_check(pop(&a));
  if (a.v.int_val)
    return(1);				/* no jump; fall through to TRUE code */
  else
    return(x->j_arg);		/* go jump to FALSE code */
}
/****************************************************************************/
