
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

//======================================================
#define TRUE             1
#define FALSE            0
#define Pi               3.141592653589793
#define MAX_LINE_LEN     2048
#define MAX_TOKENS       1024
#define MAX_ID_LEN       20
#define MAX_AT_LEN       600		
#define STACK_DEPTH      600
#define NO_CARET         (-1)
#define	IO_SUCCESS	 0
#define	IO_ERROR	 1

enum DATA_TYPES
{
  INT, CMPLX
};

struct cmplx 
{
  double real, imag;
};

struct value 
{
  enum DATA_TYPES type;
  union
    {
      int int_val;
      struct cmplx cmplx_val;
    } v;
};

struct lexical_unit
{	
  int    is_token; 
  struct value l_val;
  int start_index; 
  int length;      
};

#define putcc            putc
#define END_OF_COMMAND   (c_token >= num_tokens || equals(c_token,";"))
#define NOMEMORY  int_error("not enough memory",NO_CARET)
#ifndef memcpy
#define memcpy(d,s,l)    bcopy(s,d,l)
#endif
#define top_of_stack     stack[s_p]


typedef int              BOOLEAN;

enum operators
{
  PUSH, PUSHC, PUSHD,PUSHD1,PUSHD2,PUSHD3,CALL,LNOT, BNOT, UMINUS, LOR, LAND, 
  BOR, XOR, BAND, EQ, NE, GT, LT, GE, LE, PLUS, MINUS, MULT, DIV, MOD, POWER,
  FACTORIAL,BOOL, JUMP, JUMPZ, JUMPNZ, JTERN, SF_START
};

#define is_jump(operator) ((operator) >=(int)JUMP && (operator) <(int)SF_START)


struct udft_entry 
{                                  
  struct udft_entry     *next_udf; 
  char                  udf_name[MAX_ID_LEN+1]; 
  struct at_type        *at;                    
  char                  *definition;            
  int                   n_arg;                  
  struct value          dummy_value;            
  struct value          dummy_value1;
  struct value          dummy_value2;
  struct value          dummy_value3;
};

struct udvt_entry 
{               		
  struct udvt_entry *next_udv;  
  char udv_name[MAX_ID_LEN+1];  
  BOOLEAN udv_undef;            
  struct value udv_value;       
};

union argument 
{	                	
  int j_arg;			
  struct value v_arg;		
  struct udvt_entry *udv_arg;   
  struct udft_entry *udf_arg;   
};

struct at_entry 
{                        
  enum operators index;	 
  union argument arg;
};

struct at_type
{
  int a_count;		
  struct at_entry actions[MAX_AT_LEN];
};

typedef int              (*FUNC_PTR)(union argument *);
typedef void             (*MSG_FUNC_PTR)(char *);

struct ft_entry 
{		               
  char *f_name;                
  FUNC_PTR func;               
};


#define STDERRR stderr

/****************************************************************************/
int equals(int, char *);
int almost_equals(int, char *);
int is_number(int);
int isletter(int);
int is_definition(int);
void copy_str(char *, int);
void quote_str(char *, int);
void quote_str1(char *, int);
void m_capture(char**, int, int);
void convert(struct value *, int);
void disp_value(FILE *, struct value *);
double real(struct value *);
double imag(struct value *);
double maginatude(struct value *);
double angle(struct value *);
void int_error(char *,int);
void os_error(char *,int);
void do_initialization();
int scanner(char *);
void define(char **, int*);
void check_func_def_error();
struct value *const_express(struct value *);
void evaluate_at(struct at_type *,struct value *);
int get_var_int_value(char *);

struct value *complex(struct value *a, double realpart, double imagpart);
double magnitude(struct value *val);
struct value *pop(struct value *x);
void push(struct value *x);
struct value *integer(struct value *a, int i);
double angle(struct value *val);

int equals(int t_num, char *str);


union argument *add_action(enum operators sf_index);
struct udft_entry *add_udf(int t_num);
struct udvt_entry *add_udv(  int t_num);
struct at_type   *temp_at(), *perm_at();
