
#include "tp_prep.h"

#define RANDOM 59           /* for function rand() which need no argument */
#define SRANDOM 60          /* set random seed */
#define dummy (struct value *) 0

/************************************************************/

extern BOOLEAN              undefined;
extern int                  num_tokens,c_token;
extern struct lexical_unit  token[];
extern char                 c_dummy_var[];
extern char                 c_dummy_var1[];
extern char                 c_dummy_var2[];
extern char                 c_dummy_var3[];
extern struct udft_entry    *dummy_func;       /* pointer to dummy variable's func */

struct at_type     at;

/************************************************************/
void express(),xterm(),aterm(),bterm(),cterm(),dterm(),eterm(),fterm(),gterm(),hterm(),factor(),xterms(),
     aterms(),bterms(),cterms(),dterms(),eterms(),fterms(),gterms(),hterms(),iterms(),unary();
/************************************************************/
extern void reset_stack(),  check_stack();
extern void execute_at(struct at_type *at_ptr);
extern int standard(int);
/************************************************************/

void evaluate_at( struct at_type *at_ptr,  struct value *val_ptr)
{
  undefined = FALSE;
  errno = 0;
  reset_stack();

  execute_at(at_ptr);

  if (errno == EDOM || errno == ERANGE) 
    {
      undefined = TRUE;
    } 
  else 
    {
      (void) pop(val_ptr);
      check_stack();
    }
}

/************************************************************/

struct value *const_express(     struct value *valptr)
{
  register int tkn = c_token;
  if (END_OF_COMMAND)
    int_error("constant expression required",c_token);
  evaluate_at(temp_at(),valptr);	/* run it and send answer back */
  if (undefined) 
    int_error("undefined value",tkn);

  return(valptr);
}

/************************************************************/

struct at_type *temp_at()	/* build a static action table and return its pointer */

{
  at.a_count = 0;		/* reset action table !!! */
  express();
  return(&at);
}

/************************************************************/

/* build an action table, put it in dynamic memory, and return its pointer */

struct at_type *
  perm_at()
{
  register struct at_type *at_ptr;
  register unsigned int len;

  (void) temp_at();
  len = sizeof(struct at_type) -
    (MAX_AT_LEN - at.a_count)*sizeof(struct at_entry);
  if ((at_ptr = (struct at_type *) malloc(len)))
    (void) memcpy( (char *)at_ptr,(char *)&at,(int)len);
  return(at_ptr);
}

/************************************************************/

void express()  /* full expressions */
{
  xterm();
  xterms();
}

void xterm()  /* ? : expressions */
{
  aterm();
  aterms();
}


void aterm()
{
  bterm();
  bterms();
}


void bterm()
{
  cterm();
  cterms();
}


void cterm()
{
  dterm();
  dterms();
}


void dterm()
{	
  eterm();
  eterms();
}


void eterm()
{
  fterm();
  fterms();
}


void fterm()
{
  gterm();
  gterms();
}


void gterm()
{
  hterm();
  hterms();
}


void hterm()
{
  unary(); /* - things */
  iterms(); /* * / % */
}


void factor()
{
  register int value;

  if (equals(c_token,"(")) 
    {
      c_token++;
      express();
      if (!equals(c_token,")")) 
	int_error("')' expected",c_token);
      c_token++;
    }
  else if (equals(c_token,"{")) 
    {
      c_token++;
      express();
      if (!equals(c_token,"}")) 
	int_error("'}' expected",c_token);
      c_token++;
    }
  else if (equals(c_token,"[")) 
    {
      c_token++;
      express();
      if (!equals(c_token,"]")) 
	int_error("']' expected",c_token);
      c_token++;
    }
  else if (is_number(c_token)) 
    {
      convert(&(add_action(PUSHC)->v_arg),c_token);
      c_token++;
    }
  else if (isletter(c_token)) 
    {
      if ((c_token+1 < num_tokens)  && (equals(c_token+1,"(") ||
					equals(c_token+1,"[") ||
					equals(c_token+1,"{")))
	  {
	  value = standard(c_token);
	  if (value)     /* it's a standard function */
	    {
	      c_token += 2;
	      if(value != RANDOM)  express();
	      if (! (equals(c_token,")") || equals(c_token,"]") ||
		     equals(c_token,"}") ))
		int_error("')' or ']' or '}' expected",c_token);
	      c_token++;
	      (void) add_action( (enum operators) value);
	    }
	  else 
	    {
	      value = c_token;
	      c_token += 2;
	      express();
	      if(equals(c_token,","))
		{
		  c_token++;
		  express(); 
		}
	      if(equals(c_token,","))
		{
		  c_token++;
		  express(); 
		}
	      if(equals(c_token,",")) 
		{
		  c_token++;
		  express(); 
		}
	      if (! (equals(c_token,")") || equals(c_token,"]") ||
		     equals(c_token,"}") ))
		int_error("')' or ']' or '}' expected",c_token);        
	      c_token++;
	      add_action(CALL)->udf_arg = add_udf(value);
	    }
	}
      else 
	{
	  if (equals(c_token,c_dummy_var)) 
	    {
	      c_token++;
	      add_action(PUSHD)->udf_arg = dummy_func;
	    }
	  else if (equals(c_token,c_dummy_var1)) 
	    {
	      c_token++;
	      add_action(PUSHD1)->udf_arg = dummy_func;
	    }
	  else if (equals(c_token,c_dummy_var2)) 
	    {
	      c_token++;
	      add_action(PUSHD2)->udf_arg = dummy_func;
	    }
	  else if (equals(c_token,c_dummy_var3)) 
	    {
	      c_token++;
	      add_action(PUSHD3)->udf_arg = dummy_func;
	    }
	  else 
	    {
	      add_action(PUSH)->udv_arg = add_udv(c_token);
	      c_token++;
	    }
	}
    }          /* end if letter */
  else
    int_error("invalid expression ",c_token);

              /* add action code for ! (factorial) operator */
  while(equals(c_token,"!")) 
    {
      c_token++;
      (void) add_action(FACTORIAL);
    }
  if (equals(c_token,"**") || equals(c_token,"^") )
    {
      c_token++;
      unary();
      (void) add_action(POWER);
    }
}

/************************************************************/

void xterms()
{  /* create action code for ? : expressions */

  if (equals(c_token,"?"))
    {
      register int savepc1, savepc2;
      register union argument *argptr1,*argptr2;
      c_token++;
      savepc1 = at.a_count;
      argptr1 = add_action(JTERN);
      express();
      if (!equals(c_token,":"))
	int_error("expecting ':'",c_token);
      c_token++;
      savepc2 = at.a_count;
      argptr2 = add_action(JUMP);
      argptr1->j_arg = at.a_count - savepc1;
      express();
      argptr2->j_arg = at.a_count - savepc2;
    }
}

/************************************************************/

void aterms()
{  /* create action codes for || operator */

  while (equals(c_token,"||")) 
    {
      register int savepc;
      register union argument *argptr;
      c_token++;
      savepc = at.a_count;
      argptr = add_action(JUMPNZ);	/* short-circuit if already TRUE */
      aterm();
      argptr->j_arg = at.a_count - savepc;/* offset for jump */
      (void) add_action(BOOL);
    }
}

/************************************************************/

void bterms()
{ /* create action code for && operator */

  while (equals(c_token,"&&")) 
    {
      register int savepc;
      register union argument *argptr;
      c_token++;
      savepc = at.a_count;
      argptr = add_action(JUMPZ);	/* short-circuit if already FALSE */
      bterm();
      argptr->j_arg = at.a_count - savepc;/* offset for jump */
      (void) add_action(BOOL);
    }
}

/************************************************************/

void cterms()
{ /* create action code for | operator */

  while (equals(c_token,"|")) 
    {
      c_token++;
      cterm();
      (void) add_action(BOR);
    }
}

/************************************************************/

void dterms()
{ /* create action code for ^ operator */
/*
  while (equals(c_token,"^")) 
    {
      c_token++;
      dterm();
      (void) add_action(XOR);
    }
*/
}

/************************************************************/

void eterms()
{ /* create action code for & operator */

  while (equals(c_token,"&")) 
    {
      c_token++;
      eterm();
      (void) add_action(BAND);
    }
}

/************************************************************/

void fterms()
{ /* create action codes for == and != operators */

  while (TRUE) 
    {
      if (equals(c_token,"==")) 
	{
	  c_token++;
	  fterm();
	  (void) add_action(EQ);
	}
      else if (equals(c_token,"!=")) 
	{
	  c_token++;
	  fterm();
	  (void) add_action(NE);
	}
      else break;
    }
}

/************************************************************/

void gterms()
{ /* create action code for < > >= or <= operators */
	
  while (TRUE) 
    {
      /* I hate "else if" statements */
      if (equals(c_token,">")) 
	{
	  c_token++;
	  gterm();
	  (void) add_action(GT);
	}
      else if (equals(c_token,"<")) 
	{
	  c_token++;
	  gterm();
	  (void) add_action(LT);
	}		
      else if (equals(c_token,">=")) 
	{
	  c_token++;
	  gterm();
	  (void) add_action(GE);
	}
      else if (equals(c_token,"<=")) 
	{
	  c_token++;
	  gterm();
	  (void) add_action(LE);
	}
      else
	break;
    }
}

/************************************************************/

void hterms()
{ /* create action codes for + and - operators */

  while (TRUE) 
    {
      if (equals(c_token,"+")) 
	{
	  c_token++;
	  hterm();
	  (void) add_action(PLUS);
	}
      else if (equals(c_token,"-")) 
	{
	  c_token++;
	  hterm();
	  (void) add_action(MINUS);
	}
      else break;
    }
}

/************************************************************/

void iterms()
{ /* add action code for * / and % operators */

  while (TRUE) 
    {
      if (equals(c_token,"*")) 
	{
	  c_token++;
	  unary();
	  (void) add_action(MULT);
	}
      else if (equals(c_token,"/")) 
	{
	  c_token++;
	  unary();
	  (void) add_action(DIV);
	}
      else if (equals(c_token,"%")) 
	{
	  c_token++;
	  unary();
	  (void) add_action(MOD);
	}
      else 
	break;
    }
}

/************************************************************/

void unary()
{ /* add code for unary operators */
  if (equals(c_token,"!")) 
    {
      c_token++;
      unary();
      (void) add_action(LNOT);
    }
  else if (equals(c_token,"~")) 
    {
      c_token++;
      unary();
      (void) add_action(BNOT);
    }
  else if (equals(c_token,"-")) 
    {
      c_token++;
      unary();
      (void) add_action(UMINUS);
    }
  else
    factor();
}

/****************************************************************************/
