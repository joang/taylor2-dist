
#include "tp_prep.h"

extern int                   number_of_inputs;
extern char                  input_line[];
extern struct lexical_unit   token[];
//extern int                   errno,sys_nerr;
//extern char                  *sys_errlist[];
/***************************************************************************/
static char                  ErrorMessage[1024];
MSG_FUNC_PTR                 CALC_ErrorMessageHandle = NULL;
/***************************************************************************/

/*
 * equals() compares string value of token number t_num with str[], and
 *   returns TRUE if they are identical.
 */
int equals(int t_num, char *str)
{
  register int i;

  if (!token[t_num].is_token)
    return(FALSE);				/* must be a value--can't be equal */
  for (i = 0; i < token[t_num].length; i++) 
    {
      if (input_line[token[t_num].start_index+i] != *(str+i))
	return(FALSE);
    }
  /* now return TRUE if at end of str[], FALSE if not */
  return(str[i] == '\0');
}

/***************************************************************************/

/*
 * almost_equals() compares string value of token number t_num with str[], and
 *   returns TRUE if they are identical up to the first $ in str[].
 */
int almost_equals(int t_num, char *str)
{
  register int i;
  register int after = 0;
  register int start = token[t_num].start_index;
  register int length = token[t_num].length;

  if (!token[t_num].is_token)
    return(FALSE);				/* must be a value--can't be equal */
  for (i = 0; i < length + after; i++) 
    {
      if (str[i] != input_line[start + i]) 
	{
	  if (str[i] != '$')
	    return(FALSE);
	  else 
	    {
	      after = 1;
	      start--;	/* back up token ptr */
	    }
	}
    }
  /* i now beyond end of token string */
  return(after || str[i] == '$' || str[i] == '\0');
}

/***************************************************************************/

int is_number(int t_num)
{
  return(!token[t_num].is_token);
}


int isletter(int t_num)
{
  return(token[t_num].is_token &&
	 (isalpha(input_line[token[t_num].start_index])));
}


/*
 * is_definition() returns TRUE if the next tokens are of the form
 *   identifier =
 *		-or-
 *   identifier ( identifer ) =
 */
int is_definition(int t_num)
{
  if( isletter(t_num) &&  equals(t_num+1,"="))     /* variable */
    return(1);
  /* function */
  if(isletter(t_num) &&  equals(t_num+1,"(") && isletter(t_num+2))
    {
      t_num += 3;
      
      while((equals(t_num,",")))
	{
	  if(!isletter(++t_num))  return(0);
	   t_num++;
	}
      return(equals(t_num,")") && equals(t_num+1,"="));
    }
  return(0);
}

/***************************************************************************/

/*
 * copy_str() copies the string in token number t_num into str, appending
 *   a null.  No more than MAX_ID_LEN chars are copied.
 */
void copy_str(char str[], int t_num)
{
  register int i = 0;
  register int start = token[t_num].start_index;
  register int count;

  if ((count = token[t_num].length) > MAX_ID_LEN)
    count = MAX_ID_LEN;
  do {
    str[i++] = input_line[start++];
  } while (i != count);
  str[i] = '\0';
}


/*
 * quote_str() does the same thing as copy_str, except it ignores the
 *   quotes at both ends.  This seems redundant, but is done for
 *   efficency.
 */
void quote_str(char str[], int t_num)
{
  register int i = 0;
  register int start = token[t_num].start_index + 1;
  register int count;
  
  if ((count = token[t_num].length - 2) > MAX_ID_LEN)
    count = MAX_ID_LEN;
  do {
    str[i++] = input_line[start++];
  } while (i != count);
  str[i] = '\0';
}

void quote_str1(char str[], int t_num)
{
  register int i = 0;
  register int start = token[t_num].start_index + 1;
  register int count;
  
  count = token[t_num].length -2;
  do {
    str[i++] = input_line[start++];
  } while (i != count);
  str[i] = '\0';
}

/*
 *	capture() copies into str[] the part of input_line[] which lies between
 *	the begining of token[start] and end of token[end].
 */
void capture(char str[],int start,int end)
{
register int i,e;

	e = token[end].start_index + token[end].length;
	for (i = token[start].start_index; i < e && input_line[i] != '\0'; i++)
		*str++ = input_line[i];
	*str = '\0';
}


/*
 *	m_capture() is similar to capture(), but it mallocs storage for the
 *  string.
 */
void m_capture(char **str,int start,int end)
{
  register int i,e;
  register char *s;

  if (*str)		/* previous pointer to malloc'd memory there */
    free(*str);
  if(end >= start)
    {
      e = token[end].start_index + token[end].length;
      if ((*str =(char*) malloc((unsigned int)(e - token[start].start_index + 1)))) 
	{
	  s = *str;
	  for (i=token[start].start_index; i< e && input_line[i] != '\0'; i++)
	    *s++ = input_line[i];
	  *s = '\0';
	}
      else
	int_error("out of memory",-1);
    }
  else
    {
      if(( *str = (char *)malloc((unsigned int)  2*sizeof(char))))
	{
	  s = *str; *s = '\0';
	}
      else
	int_error("out of memory", -1);
    }
}

/***************************************************************************/

void convert( struct value *val_ptr,int t_num)
{
  *val_ptr = token[t_num].l_val;
}



void disp_value(FILE *fp,     struct value *val)
{
  switch(val->type) 
    {
    case INT:
      (void)fprintf(fp,"%d",val->v.int_val);
      break;
    case CMPLX:
      if (val->v.cmplx_val.imag != 0.0 )
	{
	  if (val->v.cmplx_val.imag > 0.0 )	
	    (void)fprintf(fp,"%.16f + %.16f*sqrt(-1.0)",
			  val->v.cmplx_val.real,val->v.cmplx_val.imag);
	  else
	    (void)fprintf(fp,"%.16f - %.16f*sqrt(-1.0)",
			  val->v.cmplx_val.real,- val->v.cmplx_val.imag);
	}
      else
	(void)fprintf(fp,"%.16f", val->v.cmplx_val.real);
      break;
    default:
      int_error("unknown type in disp_value()",NO_CARET);
    }
}

/***************************************************************************/

double  real(struct value *val)		/* returns the real part of val */
{
  switch(val->type) 
    {
    case INT:
      return((double) val->v.int_val);
      break;
    case CMPLX:
      return(val->v.cmplx_val.real);
    }
  int_error("unknown type in real()",NO_CARET);
  /* NOTREACHED */
  return 0;
}


double imag(struct value *val)		/* returns the imag part of val */
{
  switch(val->type) 
    {
    case INT:
      return(0.0);
      break;
    case CMPLX:
      return(val->v.cmplx_val.imag);
    }
  int_error("unknown type in imag()",NO_CARET);
  /* NOTREACHED */
  return 0;  
}



double magnitude(struct value *val)		/* returns the magnitude of val */
{
  //  double sqrt();
  
  switch(val->type) 
    {
    case INT:
      return((double) abs(val->v.int_val));
      break;
    case CMPLX:
      return(sqrt(val->v.cmplx_val.real*
		  val->v.cmplx_val.real +
		  val->v.cmplx_val.imag*
		  val->v.cmplx_val.imag));
      break;
    }
  int_error("unknown type in magnitude()",NO_CARET);
  /* NOTREACHED */
  return 0;
}



double angle(struct value *val)		/* returns the angle of val */
{
  //double atan2();

  switch(val->type) 
    {
    case INT:
      return((val->v.int_val > 0) ? 0.0 : Pi);
      break;
    case CMPLX:
      if (val->v.cmplx_val.imag == 0.0) {
	if (val->v.cmplx_val.real >= 0.0)
	  return(0.0);
	else
	  return(Pi);
      }
      return(atan2(val->v.cmplx_val.imag,
		   val->v.cmplx_val.real));
    }
  int_error("unknown type in angle()",NO_CARET);
  /* NOTREACHED */
  return 0;  
}

/***************************************************************************/

struct value *complex(struct value *a, double realpart, double imagpart)
{
  a->type = CMPLX;
  a->v.cmplx_val.real = realpart;
  a->v.cmplx_val.imag = imagpart;
  return(a);
}


struct value *integer( struct value *a,     int i)
{
  a->type = INT;
  a->v.int_val = i;
  return(a);
}


/***************************************************************************/
void os_error(char str[],int t_num)
{
  register int i,j, k;

  if (t_num != NO_CARET)		/* put caret under error */
    {
      if(t_num >= 10) j = t_num -10;
      else  j = 0 ;

      k = 0;
      for (i = token[j].start_index;
	   i <= token[t_num].start_index+token[t_num].length; i++) 
	ErrorMessage[k++] = input_line[i];
      ErrorMessage[k++]= '\n'; ErrorMessage[k++]= '\0';
    }
  else 	ErrorMessage[0] = '\0';
  (void)strcat(ErrorMessage, str);
  (void)strcat(ErrorMessage, "\n");

  //  if(errno >= sys_nerr)
  //    (void)strcat(ErrorMessage, "unknown error");
  //  else
    (void)strcat(ErrorMessage, strerror(errno));

  if(CALC_ErrorMessageHandle) CALC_ErrorMessageHandle(ErrorMessage);
  else fprintf(STDERRR, "%s", ErrorMessage);
  exit(1);  
}
/**********************************************************/

void int_error(char str[], int t_num)
{
  register int i,j, k;


  if (t_num != NO_CARET)   
    {
      if(t_num >= 10  ) j = t_num -10;
      else j = 0;
      (void)strcpy(ErrorMessage, "Syntax error near\n   ");
      k = strlen(ErrorMessage);
      for(i = token[j].start_index; i<token[t_num].start_index+token[t_num].length; i++) {
	ErrorMessage[k++] = input_line[i];
      }
      ErrorMessage[k++] = '\0';
      (void)strcat(ErrorMessage, " <---\n");
    }

  else   ErrorMessage[0] = 0;

  fprintf(STDERRR, "%s\n", input_line);
  
  (void)strcat(ErrorMessage, "(");
  (void)strcat(ErrorMessage, str);
  (void)strcat(ErrorMessage, ")");
  if(CALC_ErrorMessageHandle) CALC_ErrorMessageHandle(ErrorMessage);
  else fprintf(STDERRR, "%s", ErrorMessage);

  
  exit(2);
}
/*********************************************************/
int get_var_int_value(char *ptr)
{
  struct udvt_entry *udv = (struct udvt_entry *)ptr;
  if(udv) {
    struct value *val = &(udv->udv_value);
      switch(val->type) 
	{
	case INT:
	return (val->v.int_val);
	break;
	case CMPLX:
	return (int)(val->v.cmplx_val.real);
	break;
	default:
	int_error("unknown type in get_var_int_value",NO_CARET);
	}
  } else {
    int_error("Cannot get value of NULL variable.",NO_CARET);
  }
  // not reached
  return 0;
}
/*********************************************************/
