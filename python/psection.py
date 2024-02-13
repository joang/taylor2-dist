#!/usr/bin/python3

import re
import os
import sys
import math
import subprocess
import argparse
import numpy as np

psection_usage="""

#====================================================================================
[;36mThis script generates a python script that uses taylor to compute the Poincare section
of one orbit defined by the input file.  A sample input file (henon.eq) looks like
[;33m#========= Henon Heiles ==========
x'= xp;
y'= yp;
xp'= -x -2*x*y;
yp'= -y -x^2 + y^2;

expr Poincare_section = x ;

number_of_intersections=2000;
initialValues=0.0,0.1,0.39,0.3 ;
startTime = 0.0;
stopTime = 1.0e10;
crossing=1;
number_of_steps=12345678;
#========= Henon Heiles ==========
[;36mThe command
[;31m  psection.py -i henon.eq -o test.py
[;36mwill generate a python script test.py that compute
the Poincare section of the given orbit with the plane x=0.
[;31m  test.py -p 2,4
[;36mwill plot the Poincare section.[m
#====================================================================================
"""

c_wrapper_code="""

#include "tp_{0[model_name]}.h"

int poincare_section_{0[model_name]}_init() {{
  {0[mpfr_init]}  
  return 0;
}}

"""


# python script to load the DLL and use it to solve an ode
# sample
python_sample_code="""#!/usr/bin/python3

import argparse
import ctypes
import math
import numpy as np
import matplotlib.pyplot as plt
import re

{0[taylor_vars]}

def poincare_section(pname, nsteps, nxins, crossing, method, step_ctl, abs_err, rel_err, start_time, end_time, step, x_in, epsilon, outfile ):

    {0[mpfr_lib_loader]}
    {0[f128_lib_loader]}
    mylib = ctypes.CDLL( './{0[shared_lib_name]}.so')
    mylib.poincare_section_{0[model_name]}_python.argtypes= [
                               ctypes.c_char_p,                                  # psection function name
                               np.ctypeslib.ndpointer( dtype=np.float64,ndim=1), # x_in        
                               ctypes.c_int ,                                    # nsteps
                               ctypes.c_int ,                                    # nxins
                               ctypes.c_int ,                                    # xing
                               ctypes.c_int ,                                    # method        
                               ctypes.c_int ,                                    # step ctl
                               ctypes.c_int ,                                    # abs_error_log10
                               ctypes.c_int ,                                    # rel_error_log10
                               ctypes.c_double ,                                 # start time
                               ctypes.c_double ,                                 # end time
                               ctypes.c_double ,                                 # step size
                               ctypes.c_double ,                                 # epsilon        
                               ctypes.c_char_p                                   # output_file
    ]
    
    xp=np.array(x_in,dtype=np.float64)
    mylib. poincare_section_{0[model_name]}_init()
    mylib.poincare_section_{0[model_name]}_python(pname.encode(), xp, nsteps, nxins, crossing, method, step_ctl, abs_err, rel_err, start_time, end_time, step, epsilon,outfile.encode())


def taylor_data_info():
    return {{
        'row_size': {0[row_size]},
        'num_eqns': {0[num_eqns]},
        'jet_var_count': {0[jet_var_count]},
        'jet_var_size': {0[jet_var_size]},
        'jet_degree': {0[jet_degree]},
        'jet_symbols': {0[jet_symbols]},
       }}

def sample_main():

    initial_values = []
    initial_values_in = {0[init_v]}
    start_t = {0[start_t]}
    stop_t = {0[stop_t]}
    step_t = {0[step_t]}
    num_steps = {0[num_steps]}
    num_intersections = {0[num_intersections]}
    crossing = {0[crossing]}
    method = {0[method]}
    step_ctl =  {0[step_ctl]}
    abs_err  = {0[abs_err]}
    rel_err  = {0[rel_err]}
    epsilon = {0[epsilon]}
    pname="{0[pname]}"

    for i in range(len(initial_values_in)):
        try:
            initial_values.append(eval(initial_values_in[i]))
        except(ValueError):
            print("Bad value: Unable to evaluate "+ initial_values_in[i]);
            return 0

    parser = argparse.ArgumentParser(epilog="Compute Poincare Section of 1 orbit for {0[model_name]}.")
    parser.add_argument('-f', '--function_name', type=str, dest='pname', help='set poincare section function name' )
    parser.add_argument('-o', '--output_file', type=str, dest='output_file', help='file to output data' )
    parser.add_argument('-p', '--plot', type=str, dest='columns', help='select two columns to plot')        
    parser.add_argument('-t0', '--start_time', type=str, dest='start_t')    
    parser.add_argument('-t1', '--stop_time', type=str, dest='stop_t')
    parser.add_argument('-h0', '--step_size', type=str, dest='step_size', help="initial step size, only used when step size control is 0")
    parser.add_argument('-nsteps', '--num_steps', type=str, dest='num_steps')
    parser.add_argument('-nintersections', '--num_intersections', type=str, dest='num_intersections', help="stop_time, nsteps and nintersections are stop conditions, program stops when one of the limit is reached")
    parser.add_argument('-xing', '--crossing', type=str, dest='crossing', help="crossing, -1,1 or 0; cross the plane from left, right or both" )
    parser.add_argument('-method', '--method', type=str, dest='method', help="bit 0 select method to compute intersection: 0 RK4 integration backwards, 1 for using 6th degree fixed step taylor. bit 1 select fixed step RK4 as the main integrator. bit 2 select adaptive RK4 as the main integrator.")            
    parser.add_argument('-epsilon', '--epsilon', type=str, dest='epsilon', help="Error tolerance when compute the intersection. If abs(value) is smaller than epsilon, it is considered on the plane,")            
    parser.add_argument('-iv', '--initial_values', type=str, dest='init_v', help="comma separated set if values")
    parser.add_argument('-sc', '--stepsize_control', type=str, choices=['0','1','2'], dest='step_ctl', help="Only 1 is availale when jet var is present")    
    parser.add_argument('-abs_err', '--absolute_error_tolerance', type=str, dest='abs_err')
    parser.add_argument('-rel_err', '--relative_error_tolerance', type=str, dest='rel_err')    

    args = parser.parse_args()    
    outfile=""
    plot=0
    columns=[0,1]

    if(args.output_file):
        outfile = args.output_file        
    if(args.pname):
        pname = args.pname
    if(args.columns):
        plot=1
        tmp= re.split(',|:',args.columns)
        columns=[ int(x)-1 for x in tmp]
    if(args.start_t):    
        start_t= float(args.start_t)
    if(args.stop_t):    
        stop_t= float(args.stop_t)
    if(args.epsilon):    
        epsilon= float(args.epsilon)
    if(args.abs_err):    
        abs_err= math.log10(float(args.abs_err))
    if(args.rel_err):    
        rel_err= math.log10(float(args.rel_err))
    if(args.num_steps):    
        num_steps= int(args.num_steps)
    if(args.num_intersections):    
        num_intersections= int(args.num_intersections)
    if(args.crossing):    
        crossing= int(args.crossing)
    if(args.method):    
        method= int(args.method)
    if(args.step_ctl):    
        step_ctl= int(args.step_ctl)
    if(args.step_size):    
        step_t= float(args.step_size)
    if(args.init_v):    
        tmp= args.init_v.split(',')
        initial_values= [ float(x) for x in tmp]

    if(plot and not outfile):
        outfile="tmp-psection.data"

    if(step_t < 0 or stop_t < start_t):
        print("\\nThis program can only integrate forward. You have step_size={{}}, start_t={{}}, stop_t={{}}\\n".format(step_t, start_t, stop_t));
        exit(1)


    poincare_section(pname,num_steps,num_intersections, crossing, method, step_ctl,abs_err,rel_err,start_t,stop_t,step_t,initial_values,epsilon,outfile)

    if(plot):
        res = np.loadtxt(outfile)
        res = list(res.transpose())        
        sizes = [1]*len(res[columns[0]])
        data_info = taylor_data_info()
        clr="red"
        fig = plt.figure(figsize=(8,6))
        ax = fig.add_subplot(111)
        ax.set_facecolor("black")        
        #ax.scatter(res[columns[0]],res[columns[1]],c=clr,s=(72./fig.dpi)**2)
        ax.scatter(res[columns[0]],res[columns[1]],c=clr,s=sizes)                        
        plt.show()

if __name__ == "__main__":
    sample_main()

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


def size_of_jet(num_symbols, degree):
    i=1
    jsize=1
    while(i <= degree):
        k=num_monomials(num_symbols, i)
        jsize += k
        i += 1
    return int(jsize)



def main():

    RE_INIT_V=re.compile(r'initial_?values\s*=(.+);',re.IGNORECASE)
    RE_START_TIME=re.compile(r'start_?time\s*=(.+);',re.IGNORECASE)
    RE_STOP_TIME=re.compile(r'stop_?time\s*=(.+);',re.IGNORECASE)
    RE_NUM_STEPS=re.compile(r'number_?of_?steps\s*=(.+);',re.IGNORECASE)
    RE_ABS_ERROR=re.compile(r'absolute_?error_?tolerance\s*=(.+);',re.IGNORECASE)
    RE_REL_ERROR=re.compile(r'relative_?error_?tolerance\s*=(.+);',re.IGNORECASE)
    RE_DIFF_EQ=re.compile(r'\s*(\w+)\'|\s*diff\s*\(\s*(\w+)\s*,\s*\w+\s*\)\s*=(.+);',re.IGNORECASE)
    RE_JET_DECLARE=re.compile(r'jet\s+(\w+((,\w+)*))\s+symbols\s+(\d+)\s+(deg|degree)\s+(\d+);',re.IGNORECASE)
    RE_JET_INIT_V=re.compile(r'jetInitialValues?\s+(\w+)\s*=\s*"\s*(.+)\s*";',re.IGNORECASE)
    RE_NON_WORD_CHARS= re.compile(r'\W', re.IGNORECASE)
    RE_DEFINE=re.compile(r'\s*(\w+)\s*=([^,]+)\s*;',re.IGNORECASE)
    
    RE_STEP_SIZE=re.compile(r'step_?size\s*=(.+);',re.IGNORECASE)        
    RE_NUM_XINS=re.compile(r'number_?of_?intersections\s*=(.+);',re.IGNORECASE)
    RE_XING=re.compile(r'crossing\s*=(.+);',re.IGNORECASE)
    RE_METHOD=re.compile(r'method\s*=(.+);',re.IGNORECASE)        

    taylor_defs = {'_not_used_': '_xx_',};
    ps_state_vars=[]
    
    data=""
    init_v = None
    start_t = 0
    stop_t = 1.0e9
    num_steps = 123456789
    num_intersections = 1000
    crossing = 0
    method = 0
    epsilon = 0.00001
    abs_err = None
    rel_err = None
    num_eqns = 0
    debug = 0
    step_ctl = 1
    step_size = 0.001    
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
    f128=0
    f128_lib_loader=""    
    mpfr=0
    format_string="%.17f"
    mpfr_lib_loader=""
    mpfr_init=""
    pname=""
    
    parser = argparse.ArgumentParser(epilog=psection_usage,formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-f', '--function_name', type=str, dest='pname', help='set poincare section function name' )    
    parser.add_argument('-m', '--model_name', type=str, dest='model_name')
    parser.add_argument('-f128', '--float128',  dest='f128', action='store_true')    
    parser.add_argument('-mpfr', '--mpfr_precision', type=str, dest='mpfr')
    parser.add_argument('-t0', '--start_time', type=str, dest='start_t')    
    parser.add_argument('-t1', '--stop_time', type=str, dest='stop_t')
    parser.add_argument('-nsteps', '--num_steps', type=str, dest='num_steps')
    parser.add_argument('-nintersections', '--num_intersections', type=str, dest='num_intersections')
    parser.add_argument('-xing', '--crossing', type=str, dest='crossing')
    parser.add_argument('-method', '--method', type=str, dest='method')            
    parser.add_argument('-iv', '--initial_values', type=str, dest='init_v')
    parser.add_argument('-sc', '--stepsize_control', choices=[0,1,2], dest='step_ctl', help="Only 1 is availale when jet var is present")    
    parser.add_argument('-h0', '--step_size', type=str, dest='step_size')
    parser.add_argument('-abs_err', '--absolute_error_tolerance', type=str, dest='abs_err')
    parser.add_argument('-rel_err', '--relative_error_tolerance', type=str, dest='rel_err')    
    parser.add_argument('-lib', '--lib_name', type=str, dest='shared_lib_name')
    parser.add_argument('-o', '--output', type=str, dest='output_file')    

    parser.add_argument('-jlib', '--jet_library', choices=['1_1','1_n','n_1','2_n','n_2','m_n','tree'], type=str, dest='jlib')    
    parser.add_argument('-d', '--debug', dest='debug', action='store_true')                
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


    matched=0
    for line in res:
        match = RE_DIFF_EQ.match(line)
        if(match):
            num_eqns += 1
            matched=1
            if(match.group(1) is None):
                ps_state_vars.append(match.group(2))
            else:
                ps_state_vars.append(match.group(1))
                
        match = RE_INIT_V.match(line)
        if(match):
            matched=1
            temp = match.group(1)            
            init_v = temp.split(",")            
        match = RE_START_TIME.match(line)
        if(match):
            matched=1            
            start_t = match.group(1)
        match = RE_STOP_TIME.match(line)
        if(match):
            matched=1            
            stop_t = match.group(1)
        match = RE_NUM_STEPS.match(line)
        if(match):
            matched=1            
            num_steps = match.group(1)
        match = RE_ABS_ERROR.match(line)
        if(match):
            matched=1            
            abs_err = match.group(1)
        match = RE_REL_ERROR.match(line)
        if(match):
            matched=1            
            rel_err = match.group(1)
        match = RE_STEP_SIZE.match(line)
        if(match):
            matched=1            
            step_size = match.group(1)            
        match = RE_JET_DECLARE.match(line)
        if(match):
            matched=1            
            jet_vars = match.group(1)
            jet_symbols = match.group(4)
            jet_degree = match.group(6)                
            jet_var_count = len(jet_vars.split(','))
            if(jet_vars == 'all'):
                jet_var_count=num_eqns

        match = RE_JET_INIT_V.match(line)
        if(match):
            matched=1            
            var_nam = match.group(1)
            var_val = match.group(2)
            jet_init_vs_dict[var_nam] = var_val
            jet_init_vs_list.append(var_val)

        match = RE_NUM_XINS.match(line)                
        if(match):
            matched=1            
            num_intersections = match.group(1)
        match = RE_XING.match(line)                
        if(match):
            matched=1            
            crossing = match.group(1)
        match = RE_METHOD.match(line)                
        if(match):
            matched=1            
            method = match.group(1)
            
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
        tmp_list.append(key +"="+val)
    taylor_vars = "\n".join(tmp_list)        

            
    if(args.f128):
        f128 = 1
    if(args.pname):
        pname = args.pname            
    if(args.model_name):
        model_name = args.model_name
    if(args.debug):
        debug = 1
    if(args.crossing):
        crossing = int(args.crossing)        
    if(args.num_intersections):
        num_intersections = int(args.num_intersections)
        ps_npoints = num_intersections
    if(args.method):
        method= int(args.method)        
    if(args.start_t):
        start_t = args.start_t
    if(args.stop_t):
        stop_t = args.stop_t
    if(args.num_steps):
        num_steps = args.num_steps
    if(args.step_ctl):
        step_ctl = args.step_ctl
    if(args.init_v):
        init_v = args.init_v
    if(args.abs_err):
        abs_err = args.abs_err
    if(args.rel_err):
        rel_err = args.rel_err
    if(args.step_size):
        step_size = args.step_size
    if(args.shared_lib_name):
        shared_lib_name = args.shared_lib_name
    if(args.output_file):
        output_file = args.output_file
    if(args.mpfr):
        if(not f128):
            mpfr=args.mpfr
            ndigits= int(int(args.mpfr)/3.33)
            format_string="%.{0}Rg".format(ndigits)
            mpfr_init="mpfr_set_default_prec({0});".format(mpfr)
            mpfr_lib_loader="ctypes.CDLL( 'libmpfr.so' , mode = ctypes.RTLD_GLOBAL)"
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
            init_v = ",".join( create_list(1,num_eqns))
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
                 'pname': pname,
                 'start_t': start_t, 'stop_t': stop_t,
                 'num_steps': num_steps,
                 'num_eqns': num_eqns,
                 'abs_err': abs_err, 'rel_err': rel_err,
                 'model_name': model_name,
                 'shared_lib_name': shared_lib_name,
                 'step_ctl': step_ctl,
                 'jet_var_count': jet_var_count,
                 'jet_symbols': jet_symbols,
                 'jet_degree': jet_degree,
                 'jet_var_size': jet_size,
                 'row_size': row_size,                 
                 'jet_init_vals': "|".join(jet_init_vs_list),
                 'format_string': format_string,
                 'mpfr_init': mpfr_init,
                 'mpfr_lib_loader': mpfr_lib_loader,
                 'jet_lib': jet_lib,
                 'step_size': step_size,
                 'step_t': step_size,                        
                 'num_intersections': num_intersections,
                 'crossing' : crossing,
                 'method' : method,
                 'epsilon' : epsilon,
                 'f128_lib_loader': f128_lib_loader,
                 'taylor_vars': taylor_vars,                        
                 }


        # output C wrapper
        with open(C_wrapper_file,"w") as f:
            c_code = c_wrapper_code.format(data)
            f.write(c_code)

        taylor_error = ""

        # generate taylor header file
        cmd_h = ['taylor', '-header', '-expression', '-poincare', '-name', model_name ];
        if(f128):
            cmd_h.extend(['-f128'])
            print("F128 is true")            
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
        cmd_s = ['taylor', '-step', '-jet','-expression', '-poincare', '-name', model_name,  '-headername', C_header_file] 
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
        process = subprocess.Popen(['cc', '-w', '-fPIC', '-shared', '-o', C_shared_lib_file,  C_stepper_file, C_wrapper_file],
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
