
#include "tp_prep.h"

#ifndef STDOUT
#define STDOUT 1
#endif

/***************************************************************************/

extern char   input_line[];

extern int f_push(),f_pushc(),f_pushd0(),f_pushd1(),f_pushd2(),f_pushd3(),
  f_call(),f_lnot(),f_bnot(),f_uminus(),
  f_lor(),f_land(),f_bor(),f_xor(),f_band(),f_eq(),f_ne(),f_gt(),f_lt(),
  f_ge(),f_le(),f_plus(),f_minus(),f_mult(),f_div(),f_mod(),f_power(),
  f_factorial(),f_bool(),f_jump(),f_jumpz(),f_jumpnz(),f_jtern();

extern  int f_real(),f_imag(),f_arg(),f_conjg(),f_sin(),f_cos(),f_tan(),f_asin(),
  f_acos(),f_atan(),f_sinh(),f_cosh(),f_tanh(),f_int(),f_abs(),f_sgn(),
  f_sqrt(),f_exp(),f_log10(),f_log(),f_besj0(),f_besj1(),f_besy0(),f_besy1(),
  f_gamma(),f_floor(),f_ceil(),f_rand(),f_srand();
  

struct ft_entry ft[] = {	/* built-in function table */

  /* internal functions: */
	{"push", f_push},	{"pushc", f_pushc},	{"pushd0", f_pushd0},
	{"pushd1", f_pushd1},   {"pushd2",f_pushd2},    {"pushd3",f_pushd3},
	{"call", f_call},	{"lnot", f_lnot},
	{"bnot", f_bnot},       {"uminus", f_uminus},	{"lor", f_lor},
	{"land", f_land},	{"bor", f_bor},		{"xor", f_xor},
	{"band", f_band},	{"eq", f_eq},		{"ne", f_ne},
	{"gt", f_gt},		{"lt", f_lt},		{"ge", f_ge},
	{"le", f_le},		{"plus", f_plus},	{"minus", f_minus},
	{"mult", f_mult},	{"div", f_div},		{"mod", f_mod},
	{"power", f_power},     {"factorial", f_factorial},
	{"bool", f_bool},	{"jump", f_jump},	{"jumpz", f_jumpz},
	{"jumpnz",f_jumpnz},    {"jtern", f_jtern},

  /* standard functions: */
	{"real", f_real},	{"imag", f_imag},	{"arg", f_arg},
	{"conjg", f_conjg},     {"sin", f_sin},         {"cos", f_cos},
	{"tan", f_tan},		{"asin", f_asin},	{"acos", f_acos},
	{"atan", f_atan},	{"sinh", f_sinh},	{"cosh", f_cosh},
	{"tanh", f_tanh},	{"int", f_int},		{"abs", f_abs},
 	{"sgn", f_sgn},		{"sqrt", f_sqrt},	{"exp", f_exp},
	{"log10", f_log10},	{"log", f_log},		{"besj0", f_besj0},
	{"besj1", f_besj1},	{"besy0", f_besy0},	{"besy1", f_besy1},
	{"floor", f_floor},	{"ceil", f_ceil},       {"rand",f_rand},
        {"srand", f_srand},     
	{NULL, NULL}
      };

//struct value                 *integer(),*complex();
struct udvt_entry            *first_udv = NULL;
struct udft_entry            *first_udf = NULL;

/***************************************************************************/

extern int num_tokens, c_token;

int built_in_func = 0;
int built_in_vari = 0;

void do_initialization()
{
  static char *init_args[] =
    {
      "pi = 3.14159265358979323",
      "i=sqrt(-1.0)",
      "pow(x,y) = (x)**(y)",
    };

  register int i;
  int  junki;
  char *junkc;
  for(i = 0; i < 3; i++)
    {
      (void) strcpy(input_line,init_args[i]);
      num_tokens = scanner(input_line);
      c_token = 0; 
      define(&junkc, &junki);
    }
  srand48((long) getpid());
}
/****************************************************************************/
