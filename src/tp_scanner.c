
#include "tp_prep.h"

#define MIN(x,y) ( (x) < (y) ? (x) : (y) )
#define isident(c) (isalnum(c) || (c) == '_')

#ifndef STDOUT
#define STDOUT 1
#endif

#define APPEND_TOKEN      {token[t_num].length++; current++;}
#define SCAN_IDENTIFIER   while (isident(expression[current + 1]))\
  APPEND_TOKEN

/***************************************************************************/

extern int            num_tokens,c_token;
extern char           input_line[MAX_LINE_LEN+1];
extern struct lexical_unit  token[MAX_TOKENS];
int              number_of_inputs=1;

static int       t_num;

/***************************************************************************/
void substitute(char *, int);
int get_num(char *);
void done(int);
void myexit(int);
/***************************************************************************/

int scanner(     char expression[])
{
  register int current;
  register int quote;

  for (current = t_num = 0;
       t_num < MAX_TOKENS && expression[current] != '\0'; current++) 
    {
    again:
      if (isspace(expression[current]))
	continue;	                     /* skip the whitespace */
      token[t_num].start_index = current;
      token[t_num].length = 1;
      token[t_num].is_token = TRUE;	/* to start with...*/
      
      if (expression[current] == '`') 
	{
	  substitute(&expression[current],MAX_LINE_LEN - current);
	  goto again;
	}
      if (isalpha(expression[current])) 
	{
	  SCAN_IDENTIFIER;
	} else if (isdigit(expression[current]) || expression[current] == '.')
	  {
	    token[t_num].is_token = FALSE;
	    token[t_num].length = get_num(&expression[current]);
	    current += (token[t_num].length - 1);
	    /*
	     * The code for complex checking is taken off
	     */
	  }
	else if (expression[current] == '\'' || expression[current] == '\"')
	  {
	    token[t_num].length++;
	    quote = expression[current];
	    while (expression[++current] != quote) 
	      {
		if (!expression[current]) 
		  {
		    expression[current] = quote;
		    expression[current+1] = '\0';
		    break;
		  }
		else
		  token[t_num].length++;
	      }
	  }
	else switch (expression[current]) 
	  {
	  case '+':
	  case '-':
	  case '^':
	  case '/':
	  case '%':
	  case '~':
	  case '{':
	  case '}':
	  case '(':
	  case ')':
	  case '[':
	  case ']':
	  case ';':
	  case ':':
	  case '?':
	  case ',':
          case '#':
	  case '$':
	    break;
	    
	  case '&':
	  case '|':
	  case '=':
	  case '*':
	    if (expression[current] == expression[current + 1])
	      APPEND_TOKEN;
	    break;
	  case '!':
	  case '<':
	  case '>':
	    if (expression[current + 1] == '=')
	      APPEND_TOKEN;
	    break;
	  default:
	    int_error("invalid character",t_num);
	  }
      ++t_num;	/* next token if not white space */
    }
  
  /*
   * Now kludge an extra token which points to '\0' at end of expression[].
   * This is useful so printerror() looks nice even if we've fallen off the
   * line.
   */
  token[t_num].start_index = current;
  token[t_num].length = 0;
  return(t_num);
}

/***************************************************************************/

int get_num(    char str[])
{
  double atof(const char *);
  register int count = 0;
  long atol(const char *);
  register long lval;

  token[t_num].is_token = FALSE;
  token[t_num].l_val.type = INT;   /* CMPLX; do not handle integer math */
  while (isdigit(str[count]))
    count++;
  if (str[count] == '.')
    {
      token[t_num].l_val.type = CMPLX;
      while (isdigit(str[++count])) /* swallow up digits until non-digit */
	;
                                    /* now str[count] is other than a digit */
    }
  if (str[count] == 'e' || str[count] == 'E') 
    {
      token[t_num].l_val.type = CMPLX;
      if (str[++count] == '-')
	count++;
      if (!isdigit(str[count])) 
	{
	  token[t_num].start_index += count;
	  int_error("expecting exponent",t_num);
	}
      while (isdigit(str[++count]))
	;
    }
  if (token[t_num].l_val.type == INT) 
    {
      lval = atol(str);
      if ((token[t_num].l_val.v.int_val = lval) != lval)
	int_error("integer overflow; change to floating point",t_num);
    }
  else 
    {
      token[t_num].l_val.v.cmplx_val.imag = 0.0;
      token[t_num].l_val.v.cmplx_val.real = atof(str);
    }
  return(count);
}

/***************************************************************************/

void substitute(char *str,int max)			/* substitute output from ` ` */
{
  register char *last;
  register int i,c;
  register FILE *f;
  //FILE *popen();
  static char pgm[MAX_LINE_LEN+1],output[MAX_LINE_LEN+1];

  i = 0;
  last = str;
  while (*(++last) != '`') 
    {
      if (*last == '\0')
	int_error("unmatched `",t_num);
      pgm[i++] = *last;
    }
  pgm[i] = '\0';		/* end with null */
  max -= strlen(last);          /* max is now the max length of output sub. */
  
  if ((f = popen(pgm,"r")) == NULL)
    os_error("popen failed",NO_CARET);
  
  i = 0;
  while ((c = getc(f)) != EOF) 
    {
      output[i++] = ((c == '\n') ? ' ' : c);	/* newlines become blanks*/
      if (i == max) {
	(void) pclose(f);
	int_error("substitution overflow", t_num);
      }
    }
  (void) pclose(f);
  if (i + strlen(last) > max)
    int_error("substitution overflowed rest of line", t_num);
  (void) strncpy(output+i,last+1,MAX_LINE_LEN-i);
                                          /* tack on rest of line to output */
  (void) strcpy(str,output);	          /* now replace ` ` with output */
}
/****************************************************************************/
void done(int status)
{
  exit(status);
}
/******************************************************************/
void myexit(int status)
{
  exit(status);
}
/******************************************************************/

