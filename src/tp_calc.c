
#include "tp_prep.h"

/******************************************************************/
char	dummy_var[MAX_ID_LEN+1]         = "x";
char	dummy_var1[MAX_ID_LEN+1]         = "y";
char	dummy_var2[MAX_ID_LEN+1]         = "z";
char	dummy_var3[MAX_ID_LEN+1]         = "w";
char    c_dummy_var[MAX_ID_LEN+1] = "x";
char    c_dummy_var1[MAX_ID_LEN+1] = "y";   
char    c_dummy_var2[MAX_ID_LEN+1] = "z";   
char    c_dummy_var3[MAX_ID_LEN+1] = "w";   
BOOLEAN undefined;
struct lexical_unit   token[MAX_TOKENS];
char input_line[MAX_LINE_LEN+1];
int  num_tokens, c_token;
struct udft_entry     *dummy_func;

/*************************************************************/
static int initialized = 0;
/*************************************************************/

void CALC_DefineFunctionOrVariable(char *input, char **result_return, int *status_return )
//     char *input;
//     char **result_return;  /* return the udf entry or udv entry */
//     int *status_return;    /* 1 for variable, 2 for function    */
{
  int finished = 0;
  if(initialized == 0)
    {do_initialization(); initialized = 1;}
  (void)strcpy(input_line, input);

  *result_return = (char *)NULL;
  *status_return = 0;
  num_tokens = scanner(input_line); 
  c_token = 0; 
  while(finished == 0)
    {
      if(is_definition(c_token))
	define( result_return, status_return);
      if(c_token < num_tokens && 
	 (equals(c_token,";") || equals(c_token,",")))
	c_token++;
      else  finished = 1;
    }
}

void define(char **result_return, int *status_return)
//     char **result_return; int *status_return;
{
  register int  start_token,temp_token,n_args;
  register struct udvt_entry *udv;
  register struct udft_entry *udf;

  if (equals(c_token+1,"("))    /* function ! */
    { 
      start_token = c_token; n_args = 0;
      temp_token  = c_token;
      copy_str(c_dummy_var, c_token + 2);
      if(equals(temp_token+3,")"))
	{
	  c_token += 5;                /* skip (, dummy, ) and = */
	  check_func_def_error();
	  n_args = 1;
	}
      else if(equals(temp_token+5,")")) 
	{    
	  copy_str(c_dummy_var1, c_token + 4);
	  c_token += 7;                /* skip ( dummy, dummy1 ) and = */
	  check_func_def_error();
	  n_args = 2;
	}
      else if(equals(temp_token+7,")")) 
	{    
	  copy_str(c_dummy_var1, c_token + 4);
	  copy_str(c_dummy_var2, c_token + 6);
	  c_token += 9;     /* skip ( dummy, dummy1,dummy2 ) and = */
	  check_func_def_error();
	  n_args = 3;
	}
      else if(equals(temp_token+9,")")) 
	{    
	  copy_str(c_dummy_var1, c_token + 4);
	  copy_str(c_dummy_var2, c_token + 6);
	  copy_str(c_dummy_var3, c_token + 8);
	  c_token += 11;       /* skip ( dummy,dummy1,dummy2,dummy3) and = */
	  check_func_def_error();
	  n_args = 4;
	}
      udf = dummy_func = add_udf(start_token);
      if (udf->at)	        /* already a dynamic a.t. there */
	free((char *)udf->at);	/* so free it first */
      if (!(udf->at = perm_at()))
	int_error("not enough memory for function",start_token);
      udf->n_arg = n_args;
      m_capture(&(udf->definition),start_token,c_token-1);
      *result_return = (char *)udf;
      *status_return = 2;
    }
  else             /* variable ! */
    {  
      start_token = c_token;
      c_token +=2;
      udv = add_udv(start_token);
      (void) const_express(&(udv->udv_value));
      udv->udv_undef = FALSE;
      *result_return = (char *)udv;
      *status_return = 1;
    }
}

void check_func_def_error()
{
  if (END_OF_COMMAND)
    int_error("function definition expected",c_token);
}
/*************************************************************/

void CALC_EvaluateFunction(char *func, double *arguments, double *value_return)
//     char   *func;
//     double *arguments;
//     double *value_return;
{
  struct udft_entry  *tmp_func = (struct udft_entry *)func;
  struct value the_v;

  if(tmp_func)
    {
      switch(tmp_func->n_arg)
	{
	case 1:
	  complex(&(tmp_func->dummy_value), arguments[0], 0.0);
	  break;
	case 2:
	  complex(&(tmp_func->dummy_value), arguments[0], 0.0);
	  complex(&(tmp_func->dummy_value1), arguments[1], 0.0);
	  break;
	case 3:
	  complex(&(tmp_func->dummy_value), arguments[0], 0.0);
	  complex(&(tmp_func->dummy_value1), arguments[1], 0.0);
	  complex(&(tmp_func->dummy_value2), arguments[2], 0.0);
	  break;
	case 4:
	  complex(&(tmp_func->dummy_value), arguments[0], 0.0);
	  complex(&(tmp_func->dummy_value1), arguments[1], 0.0);
	  complex(&(tmp_func->dummy_value2), arguments[2], 0.0);
	  complex(&(tmp_func->dummy_value3), arguments[3], 0.0);
	  break;
	default:
	  *value_return = 0.0;
	  return;
	  break;
	}
      evaluate_at(tmp_func->at,&the_v);
      *value_return = real(&the_v);
    }
  else  *value_return = 0.0;
}

void CALC_EvaluateComplexFunction(char *func, double *arguments[2], double *real_return, double *imag_return)
//     char   *func;
//     double *arguments[2];
//     double *real_return, *imag_return;
{
  struct udft_entry  *tmp_func = (struct udft_entry *)func;
  struct value the_v;

  if(tmp_func)
    {
      switch(tmp_func->n_arg)
	{
	case 1:
	  complex(&(tmp_func->dummy_value), arguments[0][0], arguments[0][1]);
	  break;
	case 2:
	  complex(&(tmp_func->dummy_value), arguments[0][0], arguments[0][1]);
	  complex(&(tmp_func->dummy_value1),arguments[1][0], arguments[1][1]);
	  break;
	case 3:
	  complex(&(tmp_func->dummy_value), arguments[0][0], arguments[0][1]);
	  complex(&(tmp_func->dummy_value1),arguments[1][0], arguments[1][1]);
	  complex(&(tmp_func->dummy_value2),arguments[2][0], arguments[2][1]);
	  break;
	case 4:
	  complex(&(tmp_func->dummy_value), arguments[0][0], arguments[0][1]);
	  complex(&(tmp_func->dummy_value1),arguments[1][0], arguments[1][1]);
	  complex(&(tmp_func->dummy_value2),arguments[2][0], arguments[2][1]);
	  complex(&(tmp_func->dummy_value3),arguments[3][0], arguments[3][1]);
	  break;
	default:
	  *real_return = 0.0;
	  *imag_return = 0.0;
	  return;
	  break;
	}
      evaluate_at(tmp_func->at,&the_v);
      *real_return = real(&the_v);
      *imag_return = imag(&the_v);
    }
  else 
    {
      *real_return = 0.0;
      *imag_return = 0.0;
    }
}
  




