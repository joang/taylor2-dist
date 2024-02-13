#!/usr/bin/python3

import re
import os
import sys
import math
import subprocess
import argparse
import numpy as np

ptaylor_usage="""
#========================================================================================================
[;36mThis script generates a python wrapper for taylor. Given a taylor input file (pendulum.eq) like
[;33m#========== pendulum ===============
x' = y;  y' = -sin(x);
expr energy=0.5*y^2-cos(x);
initialValues = 0.6,0.3;
startTime = 0.0 ; stopTime = 100.0;
#========== pendulum ===============[;36m
The command 
[;31m   ./ptaylor.py  -i pendulum.eq -o sample.py[;36m
generates a script 'sample.py' that uses taylor to numerically solve the specified ode
and save the result to a data file, each row contains x,y, energy and time. A sample main is included 
in 'sample.py' that can be used to plot the orbit. E.g
[;31m ./sample.py -p 1,4
[;36mVarious options can be supplied on the command line. For example,
[;31m  ./ptaylor.py  -i sample.eq -o sample.py -t0 0 -t1 500 -iv 0.1,0.6
[;36moverrides the parameters in 'pendulum.eq'. In this example, it sets to integrate from t=0 to t=500
and sets the initial values to 0.1,0.6.

The multiprecision library mpfr is supported with the '-mpfr precision_in_bits' option.
Jet transport is also supported. Familarity with jet transport is needed to make sense of the output data.
Options are provided in the generated script to set/alter the default parameters.  

The output script can be imported into other scripts as a data source. For example, 
[;31m  ./ptaylor.py  -i pendulum.eq -o pendulum.py -mpfr 256 -abs_err 1E-64 -rel_err 1E-64[;36m
generates a high precision solver for the pendulum.  The script can be imported as in the following example
[;33m#!/usr/bin/python3
import mpmath
import pendulum

def main():
    res,count = pendulum.sample_main(1)
    emax= mpmath.mpf(-123456789)
    emin= mpmath.mpf(123456789)    
    for i in range(count):
        a=mpmath.fmul(0.5,mpmath.fmul( res[1][i], res[1][i]));
        b=mpmath.fsub(a, mpmath.cos(res[0][i]))
        if(b > emax):
            emax = b
        if(b < emin):
            emin = b
    print("EnergyMax",emax, "EnergyMin", emin)
    print("EnergyRange:", mpmath.fsub(emax,emin))

if __name__ == "__main__":
    main()
[;36mIf this stript is invoked with the '-mp' option, it will display an output like
[;32mEnergyMax -0.7803356149096783131092030657201... EnergyMin -0.7803356149096783131092030657201...
EnergyRange: 6.099849733485738119757742540037443381538148456242757261892482194488628627748e-66
[;m#========================================================================================================
"""

# C wrapper that use taylor to solve an ode
# code is compiled along with a generated taylor stepper
# to a dll to be loaded by python ctypes

c_wrapper_code="""

#include "tp_{0[model_name]}.h"

int taylor_{0[model_name]}(
	      int nsteps,
	      int step_ctl,
	      int abs_error_log10,
	      int rel_error_log10,
	      double start_time,
	      double end_time,
	      double step_in,
	      double *x_in,
	      char *jet_init_vs,
	      char *outfile,
              int tag,
              int rk
              )
{{
  int i, j, k, status , order, direction ;
  MY_FLOAT step , t_start , t_end ;
  MY_FLOAT x_in_out[1+{0[num_eqns]}];
  char *jet_iv, *p, *q, *jips[100];
  MY_JET    jetIn[1+{0[jet_var_count]}];
  MY_FLOAT  **evar_out;
  MY_JET   **evar_jet_out;
  int jet_var_count={0[jet_var_count]};
  int jet_symbols = {0[jet_symbols]};
  int jet_degree = {0[jet_degree]};
  FILE *file_output = (FILE *)NULL;
  static char *default_jet_init_v="(0.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)";
  static char *format_string = "{0[format_string]} ";
  static char *format_stringE = "{0[format_string]}\\n";
  double rk_err;

  {0[mpfr_init]}  
  InitMyFloat(step);InitMyFloat(t_start); InitMyFloat(t_end);
  for(i =0; i< {0[num_eqns]}; i++) {{
    InitMyFloat(x_in_out[i]);
    MakeMyFloatA(x_in_out[i], x_in[i]);
  }}  
 #if {0[jet_var_count]} > 0 
  // picking jet init values
  for(i=0; i<100; i++) jips[i]=(char *) default_jet_init_v;
  jet_iv=(char *) malloc((1+strlen(jet_init_vs))*sizeof(char));
  strcpy(jet_iv, jet_init_vs);
  i=0;
  jips[i]=jet_iv;
  p = jet_iv;
  while((q=strchr(p, '|'))) {{
    *q=0;
    p=q+1;
    if(*p) jips[++i] =p;
  }}

  InitUpJet2({0[jet_symbols]}, {0[jet_degree]});  
  for(j = 0; j < {0[jet_var_count]}; j++) {{
    InitJet(jetIn[j]);
  }}

  for(j=0; j < {0[jet_var_count]};  j++) {{
    InputJetFromString(jetIn[j],jips[j]);
    AssignMyFloat(x_in_out[j], MY_JET_DATA(jetIn[j],0));
  }}
#endif

  direction = (step_in > 0 ? 1: -1);
  order = 21;
  MakeMyFloatA(t_start, start_time);
  MakeMyFloatA(t_end, end_time);
  MakeMyFloatA(step, step_in);


  if(outfile != NULL && strlen(outfile)>0) 
    if(! (file_output = fopen(outfile, "w"))) {{
	fprintf(stderr, "Unable to open '%s'.", outfile);
	exit(2);
  }}    

  if(! file_output) file_output = stdout;

#if {0[evar_count]} > 0
     evar_out={0[evar_name]}_taylor_coefficients(t_start,x_in_out,1,jetIn,&evar_jet_out);
#endif

  for(j = 0; j < {0[num_eqns]}; j++)
    OutputMyFloat3(file_output, format_string, x_in_out[j]);

#if {0[jet_var_count]} > 0
    for(j=0; j < {0[jet_var_count]}; j++) 
       OutputJet2File(file_output, format_string, jetIn[j]);
#endif

#if {0[evar_count]} > 0
    for(j = 0; j < {0[evar_count]}; j++)
       OutputMyFloat3(file_output, format_string, evar_out[0][0]);      
#if {0[jet_var_count]} > 0
    for(j = 0; j < {0[evar_count]}; j++)
         OutputJet2File(file_output, format_string, evar_jet_out[j][0]);
#endif
#endif

  OutputMyFloat3(file_output, format_stringE, t_start);    

  nsteps--;
  i = 1;
  if(rk) {{
      order=rk;
      if(strlen("{0[uniform]}") > 1) step_ctl = 0;
   }} 

  while(1) {{

  if(rk)  {{
      k = RungeKutta_step_{0[model_name]}(&t_start , x_in_out , direction , step_ctl ,
			   abs_error_log10, rel_error_log10,
			   &t_end , &step , &order , jetIn,  &rk_err ) ;

  }} else {{
      k = taylor_{0[uniform]}step_{0[model_name]}{0[uniform_tag0]}(&t_start , x_in_out , direction , step_ctl ,
			   abs_error_log10, rel_error_log10,
			   &t_end , &step , &order , jetIn  {0[uniform_tag1]} ) ;
 }}

#if {0[evar_count]} > 0
     evar_out={0[evar_name]}_taylor_coefficients(t_start,x_in_out,1,jetIn,&evar_jet_out);
#endif

    for(j = 0; j < {0[num_eqns]}; j++)
       OutputMyFloat3(file_output, format_string, x_in_out[j]);      

#if {0[jet_var_count]} > 0
      for(j=0; j < {0[jet_var_count]}; j++) 
         OutputJet2File(file_output, format_string, jetIn[j]);
#endif

#if {0[evar_count]} > 0
    for(j = 0; j < {0[evar_count]}; j++)
       OutputMyFloat3(file_output, format_string, evar_out[0][0]);      
#if {0[jet_var_count]} > 0
    for(j = 0; j < {0[evar_count]}; j++)
         OutputJet2File(file_output, format_string, evar_jet_out[j][0]);
#endif
#endif

    OutputMyFloat3(file_output, format_stringE, t_start);    

    nsteps--;
    i++;

    if(k != 0 || nsteps == 0)  break;
    if( (direction>0 && MyFloatA_GE_B(t_start,t_end)) ||  (direction<0 && MyFloatA_LE_B(t_start,t_end)) )    break;    

  }}

  if(file_output && file_output != stdout) fclose(file_output);

  return i;

}}

"""



# python script to load the DLL and use it to solve an ode
# sample
python_sample_code="""#!/usr/bin/python3

import argparse
import ctypes
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import re
import os
import math
from math import *
{0[mp_math_module]}

{0[taylor_vars]}

def taylor(nsteps, step_ctl, abs_err, rel_err, start_time, end_time, step, x, jet_init_vals, outfile, tag, rk ):

    {0[mpfr_lib_loader]}
    {0[f128_lib_loader]}
    mylib = ctypes.CDLL( './{0[shared_lib_name]}.so')
    mylib.taylor_{0[model_name]}.argtypes= [
                               ctypes.c_int ,                                    # nsteps
                               ctypes.c_int ,                                    # step ctl
                               ctypes.c_int ,                                    # abs_error_log10
                               ctypes.c_int ,                                    # rel_error_log10
                               ctypes.c_double ,                                 # start time
                               ctypes.c_double ,                                 # end time 
                               ctypes.c_double ,                                 # initial_step_size
                               np.ctypeslib.ndpointer( dtype=np.float64,ndim=1), # x_in_and_out
                               ctypes.c_char_p,                                  # jet_init_values
                               ctypes.c_char_p,                                  # output_file 
                               ctypes.c_int,                                     # tag
                               ctypes.c_int                                      # rk 
    ]
    mylib.taylor_{0[model_name]}.restype = ctypes.c_int


    xp=np.array(x,dtype=np.float64)

    count = mylib.taylor_{0[model_name]}(nsteps, step_ctl, abs_err, rel_err, start_time, end_time, step, xp, jet_init_vals.encode(), outfile.encode(), tag, rk)

    return count

def taylor_data_info():
    return {{
        'row_size': {0[row_size]},
        'num_eqns': {0[num_eqns]},
        'jet_var_count': {0[jet_var_count]},
        'jet_var_size': {0[jet_var_size]},
        'jet_degree': {0[jet_degree]},
        'jet_symbols': {0[jet_symbols]},
       }}


def sample_main(return_data_to_caller):

    num_steps = {0[num_steps]}
    step_ctl = {0[step_ctl]}
    abs_err  = {0[abs_err]}
    rel_err = {0[rel_err]}
    start_t = {0[start_t]}
    stop_t = {0[stop_t]}
    initial_step = {0[initial_step]}
    ndigits = {0[ndigits]}
    return_data = 0
    if(return_data_to_caller):
        return_data = 1
    rk=0

    parser = argparse.ArgumentParser(epilog="Integrate one orbit for {0[model_name]}.")
    parser.add_argument('-o', '--output_file', type=str, dest='output_file')
    parser.add_argument('-p', '--plot', type=str, dest='columns', help='select columns to plot')        
    parser.add_argument('-s', '--style', type=str, dest='style', choices=['dot','solid'], help='set plot style')        
    parser.add_argument('-t0', '--start_time', type=str, dest='start_t')    
    parser.add_argument('-t1', '--stop_time', type=str, dest='stop_t')
    parser.add_argument('-nsteps', '--num_steps', type=str, dest='num_steps')
    parser.add_argument('-step', '--initial_step', type=str, dest='initial_step')
    parser.add_argument('-iv', '--initial_values', type=str, dest='init_v')
    parser.add_argument('-sc', '--stepsize_control', choices=['0','1','2'], dest='step_ctl', help="Only 1 is availale when jet var is present")    
    parser.add_argument('-abs_err', '--absolute_error_tolerance', type=float, dest='abs_err')
    parser.add_argument('-rel_err', '--relative_error_tolerance', type=float, dest='rel_err')    
    parser.add_argument('-d', '--data', dest='return_data', action='store_true', help="return data in an array [data_array, count]")
    parser.add_argument('-rk', '--rungekutta', dest='rk', type=int, choices=[4,5,6,7,8,9], help="select RungeKutta Method, possible values: 5,6,7,8,9")
    if(ndigits):
        parser.add_argument('-mp', '--mpmath', dest='use_mpmath', action='store_true', help="return data in an mpmath array [ data_array, count]")    

    args = parser.parse_args()    
    use_mpmath = 0    
    outfile=""
    plot=0
    columns=[0,1]
    style="-"
    plot_styles = {{'solid': '-', 'dot': ':'}}
    initial_values = []
    initial_values_in = {0[init_v]}

    for i in range(len(initial_values_in)):
        try:
            initial_values.append(eval(initial_values_in[i]))
        except(ValueError):
            print("Bad value: Unable to evaluate "+ initial_values_in[i]);
            return 0

    jet_init_vars="{0[jet_init_vals]}"

    if(args.output_file):
        outfile = args.output_file        
    if(args.columns):
        plot=1
        tmp= re.split(',|:',args.columns)
        columns=[ int(x)-1 for x in tmp]
    if(args.style):
       if args.style in plot_styles:     
           style=plot_styles[args.style]

    if(args.start_t):    
        start_t= float(args.start_t)
    if(args.stop_t):    
        stop_t= float(args.stop_t)
    if(args.abs_err):    
        abs_err= int(math.log10(float(args.abs_err)))
    if(args.rel_err):    
        rel_err= int(math.log10(float(args.rel_err)))
    if(args.num_steps):    
        num_steps= int(args.num_steps)
    if(args.step_ctl):    
        step_ctl= int(args.step_ctl)
    if(args.initial_step):    
        initial_step= float(args.initial_step)
    if(args.init_v):    
        tmp= args.init_v.split(',')
        initial_values= [ float(x) for x in tmp]
    if(args.return_data):
        return_data = 1
    if(args.rk):
        rk = args.rk
    if(ndigits):
       if(args.use_mpmath):
           use_mpmath = 1    
           return_data = 1

    if((plot or return_data) and not outfile):
        outfile="tmp-ptaylor.data"


    count = taylor(num_steps,step_ctl,abs_err,rel_err,start_t,stop_t,initial_step,initial_values,jet_init_vars,outfile, 0, rk)

    if(plot):
        res = np.loadtxt(outfile)
        res = list(res.transpose())        
        vsize = len(columns)

        if(vsize > 2):
            fig = plt.figure()
            ax = plt.axes(projection='3d')
            ax.plot3D (res[columns[0]], res[columns[1]], res[columns[2]], linestyle=style)
            plt.show()
        else:
            if(vsize == 1):
                data_info = taylor_data_info()
                t = int(data_info['row_size']) -1
                plt.plot(res[t] , res[columns[0]], linestyle=style)
            else:
                plt.plot(res[columns[0]] , res[columns[1]], linestyle=style)                
            plt.show()

    if(return_data):
        if(use_mpmath):
            mpmath.mp.dps={0[ndigits]}
            def load_data_mpmath(file_path):
                import csv
                
                if(not os.path.isfile(file_path)):
                    print("file '{{}}' does not exist.".format(file_path))
                    exit(1)    
                data=[]
                count=0
                with open(file_path) as csvfile:
                    sreader = csv.reader(csvfile, delimiter=' ')
                    length=0
                    for row in sreader:
                        if length == 0:
                            length = len(row)
                            for i in range(length):
                                data.append([])
                        for i in range(length):
                            data[i].append(mpmath.mpf(row[i]))
                        count += 1

                return [data, count]
            
            return load_data_mpmath(outfile)
        else:
            res = np.loadtxt(outfile)
            data = list(res.transpose())        
            count = len(data[0])
            return [data, count] 


if __name__ == "__main__":
    sample_main(0)

"""


def create_list(r1, r2):
    """ create a list of sample initial values """
    return list(map(lambda x: str(0.1*x), range(r1, r2+1)))

# remove C comments from input, see
# https://codereview.stackexchange.com/questions/148305/remove-comments-from-c-like-source-code
def source_code(char):
    if char == '/':
        return comment_begin, ''
    if char == '#':
        return sh_comment, ''
    return source_code, char

def sh_comment(char):
    if char == '\n':
         return source_code, char
    return sh_comment, ''

def comment_begin(char):
    if char == '/':
        return inline_comment, ''
    if char == '*':
        return block_comment, ''
    return source_code, '/'+char

def inline_comment(char):
    if char == '\n':
         return source_code, char
    return inline_comment, ''

def block_comment(char):
    if char == '*':
        return end_block_comment, ''
    return block_comment, ''

def end_block_comment(char):
    if char == '/':
        return source_code, ''
    return block_comment, ''

def remove_comments(content):
    """ remove comments from taylor input """
    def gen_content():
        parser = source_code
        for character in content:
            parser, text = parser(character)
            yield text

    return ''.join(gen_content())


def canonical_input(content):
    """ strip out empty lines, leading and traling spaces from content """        
    lines = content.split(';')
    NEWLINE_RE = re.compile(r'(^\s+|\s*\r?\n)')
    EMPTY_LINE_RE = re.compile(r'^\s*$')
    code=[]
    for line in lines:
        line1 = NEWLINE_RE.sub('',line)
        if(EMPTY_LINE_RE.match(line1)):
            continue
        else:
            code.append(line1 + ';')
    return code

def num_monomials(nvars, deg):
    """ return the number of monomials of degree 'deg' in 'nvars' variables """
    t=nvars+deg-1
    d=0
    n=1
    k = nvars - 1
    if(deg > k):
        c= k
    else:
        c= deg
    if (c == 0):
        return 1
  
    while(d<c):
        n *= (t-d)
        n /= (1+d)
        d += 1

    return n;


def size_of_jet(num_symbols, degree):
    """ return the size of jet variable in 'num_symbols' of degree 'degree' """
    i=1
    jsize=1
    
    if num_symbols == 0:
        return 1
    
    while(i <= degree):
        k=num_monomials(num_symbols, i)
        jsize += k
        i += 1
        
    return int(jsize)


def main():

    RE_INIT_V=re.compile(r'initial_?values\s*=(.+);',re.IGNORECASE)
    RE_START_TIME=re.compile(r'start_?time\s*=(.+);',re.IGNORECASE)
    RE_STOP_TIME=re.compile(r'stop_?time\s*=(.+);',re.IGNORECASE)
    RE_STEP_SIZE=re.compile(r'step(_?size)?\s*=\s*(.+)\s*;',re.IGNORECASE)    
    RE_NUM_STEPS=re.compile(r'number_?of_?steps\s*=(.+);',re.IGNORECASE)
    RE_ABS_ERROR=re.compile(r'absolute_?error_?tolerance\s*=(.+);',re.IGNORECASE)
    RE_REL_ERROR=re.compile(r'relative_?error_?tolerance\s*=(.+);',re.IGNORECASE)
    RE_DIFF_EQ=re.compile(r'\s*(\w+)\'|\s*diff\s*\(\s*(\w+)\s*,\s*\w+\s*\)\s*=(.+);',re.IGNORECASE)
    RE_JET_DECLARE=re.compile(r'jet\s+(\w+((,\w+)*))\s+symbols\s+(\d+)\s+(deg|degree)\s+(\d+);',re.IGNORECASE)
    RE_JET_INIT_V=re.compile(r'jetInitialValues?\s+(\w+)\s*=\s*"\s*(.+)\s*";',re.IGNORECASE)
    RE_EXPR=re.compile(r'expr(ession)?\s+([^=\s]+)\s*=(.+);',re.IGNORECASE);
    RE_NON_WORD_CHARS= re.compile(r'\W', re.IGNORECASE)
    RE_DEFINE=re.compile(r'\s*(\w+)\s*=([^,]+)\s*;',re.IGNORECASE)

    taylor_defs = {'_not_used_': '_xx_',};
    ps_state_vars=[]
    
    evar_name=None
    evar_count=0
    data=""
    uniform=""
    uniform_tag0=""
    uniform_tag1=""                
    init_v = None
    start_t = None
    stop_t = None
    num_steps = None
    abs_err = None
    rel_err = None
    num_eqns = 0
    debug = 0
    init_step = 0.015625
    step_ctl = 1
    jet_vars=None
    jet_var_count=0 
    jet_symbols=0
    jet_degree=0
    jet_size=0
    jet_lib=""
    row_size=0
    jet_init_vs_dict={}
    jet_init_vs_list=[]    
    model_name='ode_name'
    shared_lib_name='tp_temp_lib'
    output_file = None
    mpfr=0
    f128=0
    f128_lib_loader=""        
    format_string="%.17f"
    mpfr_lib_loader=""
    mpfr_init=""
    ndigits=0
    mp_math_module=""
        
    parser = argparse.ArgumentParser(epilog=ptaylor_usage,formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-m', '--model_name', type=str, dest='model_name')
    parser.add_argument('-mpfr', '--mpfr_precision', type=str, dest='mpfr')
    parser.add_argument('-f128', '--float128', dest='f128',action='store_true')    
    parser.add_argument('-t0', '--start_time', type=str, dest='start_t')    
    parser.add_argument('-t1', '--stop_time', type=str, dest='stop_t')
    parser.add_argument('-nsteps', '--num_steps', type=str, dest='num_steps')
    parser.add_argument('-step', '--step_size', type=str, dest='step_size')
    parser.add_argument('-iv', '--initial_values', type=str, dest='init_v')
    parser.add_argument('-sc', '--stepsize_control', choices=['0','1','2'], dest='step_ctl', help="Only 1 is availale when jet var is present")    
    parser.add_argument('-abs_err', '--absolute_error_tolerance', type=str, dest='abs_err')
    parser.add_argument('-rel_err', '--relative_error_tolerance', type=str, dest='rel_err')    
    parser.add_argument('-lib', '--lib_name', type=str, dest='shared_lib_name')
    parser.add_argument('-o', '--output', type=str, dest='output_file')    

    parser.add_argument('-jlib', '--jet_library', choices=['1_1','1_n','n_1','2_n','n_2','m_n','tree'], type=str, dest='jlib')    
    parser.add_argument('-d', '--debug', dest='debug', action='store_true')
    parser.add_argument('-u', '--uniform', dest='uniform', action='store_true', help="use the uniform stepper")                    
    parser.add_argument('-i', '--input', dest='input_file', type=str, help="Taylor input file")
    
    args = parser.parse_args()

    if args.input_file:
         file_path = args.input_file
         if(os.path.isfile(file_path)):
             text_file = open(file_path, "r")
             data = text_file.read()
             text_file.close()
         else:
             print("file '{}' does not exist.".format(file_path))
             exit(1)
    else:
        data=sys.stdin.read()
        file_path = 'tmp-ptylor.eq'
        with open(file_path,"w") as f:
            f.write(data)
            
    clean = remove_comments(data)
    res = canonical_input(clean)    
    #print("\n".join(res)) 


    for line in res:
        matched=0        
        match = RE_EXPR.match(line)
        if(match):
            matched=1
            evar_name = match.group(2)
            exprs = match.group(3)
            tmp = exprs.split(",")
            evar_count=len(tmp);
        
        match = RE_DIFF_EQ.match(line)
        if(match):
            num_eqns += 1
            matched=2
            if(match.group(1) is None):
                ps_state_vars.append(match.group(2))
            else:
                ps_state_vars.append(match.group(1))                

        match = RE_STEP_SIZE.match(line)
        if(match):
            init_step = match.group(2)
            matched=3
        match = RE_INIT_V.match(line)
        if(match):
            temp = match.group(1)
            init_v = temp.split(",")
            matched=4            
        match = RE_START_TIME.match(line)
        if(match):
            start_t = match.group(1)
            matched=5            
        match = RE_STOP_TIME.match(line)
        if(match):
            stop_t = match.group(1)
            matched=6            
        match = RE_NUM_STEPS.match(line)
        if(match):
            num_steps = match.group(1)
            matched=7            
        match = RE_ABS_ERROR.match(line)
        if(match):
            abs_err = match.group(1)
            matched=8            
        match = RE_REL_ERROR.match(line)
        if(match):
            rel_err = match.group(1)
            matched=9            
        match = RE_JET_DECLARE.match(line)
        if(match):
            matched=10            
            jet_vars = match.group(1)
            jet_symbols = match.group(4)
            jet_degree = match.group(6)                
            jet_var_count = len(jet_vars.split(','))
            if(jet_vars == 'all'):
                jet_var_count=num_eqns

        match = RE_JET_INIT_V.match(line)
        if(match):
            matched=11            
            var_nam = match.group(1)
            var_val = match.group(2)
            jet_init_vs_dict[var_nam] = var_val
            jet_init_vs_list.append(var_val)

        if(matched == 0):
            match = RE_DEFINE.match(line)                
            if(match):
                taylor_defs[match.group(1)] = match.group(2);
                                   

    state_vars = '|'.join(ps_state_vars)
    tmp='({})'.format(state_vars)
    reg_state_vars = re.compile(r'\b%s\b' % tmp)

    removed_list=['_not_used_']
    changed = 1;
    while changed:
        changed=0;
        removed_vars = '|'.join(removed_list)        
        tmp='({})'.format(removed_vars)
        reg_removed_vars = re.compile(r'\b%s\b' % tmp)            
        for key in taylor_defs:
            val = taylor_defs[key]
            if(re.search(reg_state_vars,val)):
                if(not key in removed_list):
                    removed_list.append(key)
                    changed=1
            elif(re.search(reg_removed_vars,val)):
                if(not key in removed_list):
                    removed_list.append(key)
                    changed=1
    for key in removed_list:
        del taylor_defs[key]

    taylor_vars = ""
    tmp_list=[]
    for key in taylor_defs:
        val = taylor_defs[key]
        pval = val.replace("^", "**");
        tmp_list.append(key +"="+pval)
    taylor_vars = "\n".join(tmp_list)        

                

    if(args.f128):
        f128 = 1
    if(args.model_name):
        model_name = args.model_name
    if(args.debug):
        debug = 1
    if(args.uniform):
        uniform = 'uniform_'
        uniform_tag0 = '_tag'
        uniform_tag1 = ', tag'                                
    if(args.start_t):
        start_t = args.start_t
    if(args.stop_t):
        stop_t = args.stop_t
    if(args.num_steps):
        num_steps = args.num_steps
    if(args.step_size):
        init_step = args.step_size
    if(args.step_ctl):
        step_ctl = args.step_ctl
    if(args.init_v):
        init_v = args.init_v
    if(args.abs_err):
        abs_err = args.abs_err
    if(args.rel_err):
        rel_err = args.rel_err
    if(args.shared_lib_name):
        shared_lib_name = args.shared_lib_name
    if(args.output_file):
        output_file = args.output_file
    if(args.mpfr):
        if(not f128 ):
            mpfr=args.mpfr
            ndigits= int(int(args.mpfr)/3.33)
            format_string="%.{0}Rg".format(ndigits)
            mpfr_init="mpfr_set_default_prec({0});".format(mpfr)
            mpfr_lib_loader="ctypes.CDLL( 'libmpfr.so' , mode = ctypes.RTLD_GLOBAL)"
            mp_math_module="import mpmath"

    if(args.jlib):
        jet_lib = args.jlib

    if(f128):
        f128_lib_loader="ctypes.CDLL( 'libquadmath.so.0' , mode = ctypes.RTLD_GLOBAL)"

    if(model_name == 'ode_name'):
        if(args.input_file):
            tmp = args.input_file
            model_name = 'ode_name_'+ RE_NON_WORD_CHARS.sub('_',tmp)

    if(shared_lib_name == 'tp_temp_lib'):
        shared_lib_name = 'lib_' + model_name
            
    if(num_eqns > 0):
        if(init_v is None):
            tmp = ",".join( create_list(1,num_eqns))
            init_v = tmp.split(",");
        if(start_t is None):
            start_t = 0
        if(stop_t is None):
            stop_t = 10
        if(num_steps is None):
            num_steps = 1000
        if(abs_err is None):
            abs_err = -16
        else:
            abs_err =  int( math.log10(float(abs_err)))
        if(rel_err is None):
            rel_err = -16
        else:
            rel_err =  int( math.log10(float(rel_err)))

        if(jet_vars):
            jet_size = size_of_jet(int(jet_symbols), int(jet_degree))
            step_ctl = 1
            
        # size of output row
        row_size = int(num_eqns) + 1 + jet_size * jet_var_count
            
        if(debug):
            print("init_v={}".format(init_v) )
            print("start_time={}".format(start_t))    
            print("stop_time={}".format(stop_t))
            print("num_steps={}".format(num_steps))
            print("abs_error={}".format(abs_err))
            print("rel_error={}".format(rel_err))
            print("num_eqns={}".format(num_eqns))
            print("model_name={}".format(model_name))
            print("format_str={}".format(format_string))
            if(evar_name):
                print("Expression:", evar_name, evar_count);
            if(mpfr):
                print("mpfr_precision={}".format(mpfr))
            if(jet_vars):
                if(jet_lib):
                    print("jet_library={}".format(jet_lib))
                print("jet_vars={}".format(jet_vars))
                print("jet_symbols={}".format(jet_symbols))
                print("jet_degree={}".format(jet_degree))
                print("jet_var_count={}".format(jet_var_count))                                        
                print("jet_var_size={}".format(jet_size))
                print("jet_init_vals:")
                print("  "+"\n  ".join(jet_init_vs_list))
            print("output row size={}+1+{}*{} = {}".format(num_eqns,jet_size,jet_var_count,row_size))                    

        C_header_file = "tp_" + model_name + ".h"
        C_wrapper_file = "tp_"+ model_name + ".c"
        C_stepper_file = "tp_"+ model_name + "_step.c"        
        C_shared_lib_file = shared_lib_name + ".so"

        data = { 
                 'init_v': init_v,
                 'start_t': start_t, 'stop_t': stop_t,
                 'num_steps': num_steps,
                 'num_eqns': num_eqns,
                 'abs_err': abs_err, 'rel_err': rel_err,
                 'model_name': model_name,
                 'shared_lib_name': shared_lib_name,
                 'step_ctl': step_ctl,
                 'initial_step': init_step,
                 'jet_var_count': jet_var_count,
                 'jet_symbols': jet_symbols,
                 'jet_degree': jet_degree,
                 'jet_var_size': jet_size,
                 'row_size': row_size,                 
                 'jet_init_vals': "|".join(jet_init_vs_list),
                 'format_string': format_string,
                 'mpfr_init': mpfr_init,
                 'mpfr_lib_loader': mpfr_lib_loader,
                 'f128_lib_loader': f128_lib_loader,            
                 'jet_lib': jet_lib,
                 'evar_name': evar_name,
                 'evar_count': evar_count,
                 'ndigits': ndigits,
                 'mp_math_module': mp_math_module,
                 'taylor_vars': taylor_vars,
                 'uniform' : uniform,
                 'uniform_tag0' : uniform_tag0,
                 'uniform_tag1' : uniform_tag1,
                 }


        # output C wrapper
        with open(C_wrapper_file,"w") as f:
            c_code = c_wrapper_code.format(data)
            f.write(c_code)

        taylor_error = ""

        # generate taylor header file
        cmd_h = ['taylor', '-header', '-rk', '-poincare', '-name', model_name ];

        if(f128):
            cmd_h.extend(['-f128'])                   
        elif(mpfr):
            cmd_h.extend(['-mpfr_precision',mpfr])
        if(jet_lib):
            cmd_h.extend(['-jet_lib',jet_lib])            

        cmd_h.append(file_path)
        
        process = subprocess.Popen(cmd_h,
                                   stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        if(stdout is not None):
            with open(C_header_file, "wb") as f:
                f.write(stdout)
        if(stderr):
            taylor_error = taylor_error  + "Taylor is unable to generate a header file: "+ stderr.decode() + "\n"

        # generate stepper
        cmd_s = ['taylor', '-step','-rk', '-poincare', '-jet','-name', model_name,  '-headername', C_header_file] 
        if(jet_vars):
            cmd_s.append('-jet_helper')
        if(f128):
            cmd_s.extend(['-f128'])            
        elif(mpfr):
            cmd_s.extend(['-mpfr_precision',mpfr])
        if(jet_lib):
            cmd_s.extend(['-jet_lib',jet_lib])            

        cmd_s.append(file_path)            
        process = subprocess.Popen(cmd_s,
                                   stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        if(stdout is not None):        
            with open(C_stepper_file, "wb") as f:
                    f.write(stdout)
        if(stderr):
            taylor_error = taylor_error  + "Taylor was unable to generate a stepper file: "+ stderr.decode() + "\n"                

                
        # compile a shared library
        process = subprocess.Popen(['cc','-w', '-fPIC', '-shared', '-o', C_shared_lib_file,  C_stepper_file, C_wrapper_file],
                                   stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()

        if(stderr):
            taylor_error = taylor_error  + "Taylor was unable to compile a shared library: "+ stderr.decode() + "\n"                                

        if(taylor_error):
            print(taylor_error)

        # generate and output sample python code
        p_code = python_sample_code.format(data)

        P_sample_code_file = None
        if(output_file):
            P_sample_code_file = output_file
            with open(P_sample_code_file,"w") as f:
                f.write(p_code)
            os.chmod(P_sample_code_file, 0o755)
                
        else:
            print(p_code)
            
        #taylor -header -name xxx pen.eq
        c_code = c_wrapper_code.format(data)
        #print(c_code)
        p_code = python_sample_code.format(data)
        #print(p_code)
        
        
if __name__ == "__main__":
    main()
