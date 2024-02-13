#!/usr/bin/python3

import re
import os
import sys
import math
import subprocess
import argparse
import pathlib
import numpy as np
import pprint

psection_usage = """

#====================================================================================
[;36mThis script generates a python script that uses taylor to compute Poincare sections
interactively.  A sample input file (henon.eq) looks like
[;33m#========= Henon Heiles ==========
x'= xp;
y'= yp;
xp'= -x -2*x*y;
yp'= -y -x^2 + y^2;

expr Poincare_section = x ;

# The following comments configures UI
# Poincare_section_slider1 = "Hamiltonian", 0.0, 0.16, 0.01, 0.1, HH;
# How to compute initial values: given H, x=0,y and yp, solve for xp.
# poincare_section_initials = 0, y, 1./3.*sqrt(6.*y**3-9.*y**2-9*yp**2+18.*HH), yp;
# poincare_section_xlimit = -0.6,0.6;
# Poincare_section_ylimit = -0.7,0.7;
# poincare_section_mouse = y,yp;
# poincare_section_columns = 1,3;
#========= Henon Heiles ==========
[;36mThe #poincare_section_** comments are used to configure the UI.
The slider line defines a slider, fields are
[;33m   NAME, left_value, right_value, increment, default_value, variable_bind_to[;36m
You can have up to 3 sliders. External parameter can be bound to a slider.
The x(y)limit lines defines the ploting grid.
The mouse line defines the variables mouse pointer coordinates are bound to.
The columns line tells the columns in the output to plot.[m
#====================================================================================

"""

c_wrapper_code="""
#include "tps_{0[model_name]}.h"

MY_FLOAT {0[extern_vars]};
{0[typed_extern_vars]};

int poincare_set_extern_vars_{0[model_name]}({0[double_slider_vars]}) {{
  static int inited=0;
  {0[mpfr_init]}  
  if(!inited) {{
   {0[init_myfloat_slider_vars]}
   inited=1;
  }}
  {0[myfloat_slider_vars]}

  return 0;
}}


"""

python_sample_code="""#!/usr/bin/python3

from tkinter import *
from math import *
from matplotlib.figure import Figure
from matplotlib import pyplot as plt
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg,NavigationToolbar2Tk)
import numpy as np
import random
import argparse
import ctypes
import time

notused=0
xlimit=[{0[xlimit]}]
ylimit=[{0[ylimit]}]
xxlimit=[{0[xlimit]}]
yylimit=[{0[ylimit]}]
xslimit=[{0[xlimit]}]
yslimit=[{0[ylimit]}]
last_xlimit=0
last_ylimit=0

columns=[{0[columns]}]        

win_xmin=str(xlimit[0])
win_xmax=str(xlimit[1])
win_ymin=str(ylimit[0]) + " "
win_ymax=str(ylimit[1]) + " "

slider1_setup=None
slider2_setup=None
slider3_setup=None

{0[slider1_setup_var]}
{0[slider1_setup]}
{0[slider2_setup_var]}
{0[slider2_setup]}
{0[slider3_setup_var]}
{0[slider3_setup]}

npoints={0[npoints]}
{0[slidern_setup]}



ps_initial_values = {0[init_expr]} 
orbits = []
points = []
the_color = 0
work_in_progress=0

#colors = ['red','green','blue','orange','purple','brown','pink','gray','olive','cyan']
colors=[
"#D86666","#E77299","#B472B4","#8D66D8","#668DB4","#668DD8","#5AC0B4","#66AE8A","#4BB151","#8DC03F","#C0C04E","#E1C33F","#F3A53F","#E7814E","#BD9393","#A893A8","#8A96A5","#93A2BD","#84ABA5","#A8A87E","#C3A884","#CCCCCC",

"#CC3333","#DE4578","#994599","#6633CC","#336699","#3366CC","#21AB99","#339363","#0F9618","#66AB00","#ABAB12","#D5AE00","#ED8700","#DE5412","#A86F6F","#8D6C8D","#637587","#6F84A8","#5D8D87","#8A8A51","#B18A5A","#BBBBBB",

"#A22A2A","#B13660","#7B367B","#512AA2","#2A517B","#2A51A2","#1B877B","#27754E","#0C7812","#518700","#87870F","#AB8A00","#BD6C00","#B1450F","#875A5A","#6F576F","#4E5D6C","#5A6987","#4B726C","#6F6F42","#8D6F48","#AAAAAA",

"#E79999","#EDA2BA","#CCA2CC","#B499E7","#99B4CC","#99B4E7","#90D5CC","#99C9B1","#87CC8D","#B4D581","#D5D587","#EAD881","#F6C381","#EDAB87","#D5B7B7","#C6B7C6","#B1BAC3","#B7C0D5","#AEC6C3","#C3C3A8","#D8C6AB","#DDDDDD",

"#F0C3C3","#F6C6D5","#E1C6E1","#D2C3F0","#C3D2E1","#C3D2F0","#BDE7E1","#C3DECF","#B7E1BA","#D2E7B4","#E7E7B7","#F3E7B4","#F9DBB4","#F6CCB7","#E4D5D5","#DED2DE","#CFD5DB","#D5DBE4","#CFDEDB","#DBDBCC","#E7DBCF","#EEEEEF"
]


bgcolor="#111111"
fig = Figure(figsize = (12, 8), dpi = 100)
plot1 = fig.add_subplot(111)
plot1.set_facecolor(bgcolor)
plot1.set_autoscaley_on(False)
plot1.set_xlim(xlimit)
plot1.set_ylim(ylimit)
fig.subplots_adjust(top=1, bottom=0, right=1,left=0,  hspace = 0, wspace = 0)

def taylor_data_info():
    return {{
        'row_size': {0[row_size]},
        'num_eqns': {0[num_eqns]},
        'jet_var_count': {0[jet_var_count]},
        'jet_var_size': {0[jet_var_size]},
        'jet_degree': {0[jet_degree]},
        'jet_symbols': {0[jet_symbols]},
       }}

def poincare_section(pname, nsteps, nxins, crossing, method, step_ctl, abs_err, rel_err, start_time, end_time, step, x_in, epsilon, outfile, *slider_vars ):

    {0[mpfr_lib_loader]}
    {0[f128_lib_loader]}
    mylib = ctypes.CDLL( './{0[shared_lib_name]}.so')
    mylib.poincare_section_{0[model_name]}_python.argtypes= [
                               ctypes.c_char_p,                                  # poincare section function name
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
    mylib.poincare_set_extern_vars_{0[model_name]}.argtypes= [ {0[ctype_args]}];    
    xp=np.array(x_in,dtype=np.float64)

    mylib.poincare_set_extern_vars_{0[model_name]}(*slider_vars)
    mylib.poincare_section_{0[model_name]}_python(pname.encode(), xp, nsteps, nxins, crossing, method, step_ctl, abs_err, rel_err, start_time, end_time, step, epsilon,outfile.encode())



g_start_t = {0[start_t]}
g_stop_t = {0[stop_t]}
g_step_t = {0[step_t]}
g_num_steps = {0[num_steps]}
g_crossing = {0[crossing]}
g_method = {0[method]}
g_step_ctl =  {0[step_ctl]}
g_abs_err  = {0[abs_err]}
g_rel_err  = {0[rel_err]}
g_epsilon = {0[epsilon]}
g_outfile="tmp-psection.data"
g_pname="{0[pname]}"
{0[taylor_vars]}

def set_ps_method():
    global g_method
    v=integrator.get()
    if(v==2):
        g_method = g_method & ~4
        g_method = g_method | 2
        print("RK4")
    elif (v == 4):
        g_method = g_method & ~2
        g_method = g_method | 4
        print("RK4 with adaptive step")
    elif (v == 8):
        g_method = g_method & ~6
        print("Taylor")
    elif(v == 1):
        g_method = g_method | 1
    else:
        g_method = g_method & ~1
        

def add_orbit(mx,my):

    global g_pname,g_outfile,g_columns,g_start_t,g_stop_t,g_epsilon,g_abs_err,g_rel_err,g_num_steps,npoints,g_crossing,g_method,g_step_ctl,g_step_t

    {0[mouse_x]}=mx
    {0[mouse_y]}=my
    
    info =taylor_data_info()
    num_eqns = info['num_eqns']
    initial_values = [0]*num_eqns    
    for i in range(num_eqns):
        try:
            initial_values[i] = eval(ps_initial_values[i])
        except(ValueError):
            print("Bad value");
            return 0

    start_t = g_start_t
    stop_t = g_stop_t 
    step_t = g_step_t
    num_steps = g_num_steps
    crossing = g_crossing
    method = g_method
    step_ctl = g_step_ctl
    abs_err  = g_abs_err
    rel_err  = g_rel_err
    epsilon = g_epsilon
    outfile = g_outfile
    pname = g_pname
    num_intersections = npoints

    time_start = time.time()
    poincare_section(pname,num_steps,num_intersections, crossing, method, step_ctl,abs_err,rel_err,start_t,stop_t,step_t,initial_values,epsilon,outfile,{0[slider_vars]})
    time_end = time.time()

    total_time = time_end - time_start
    print("Time="+str(total_time))

    return 1

def set_npoints(v):
    global npoints
    npoints = int(v)
    #print(npoints)

def warn_change(v):
    hamilButton["fg"] = "red"

def toggle_bg_color():
    global plot1,bgcolor
    if(bgcolor == "#111111"):
        bgcolor = "#ffffff"
    else:
        bgcolor = "#111111"
    plot1.set_facecolor(bgcolor)
    fig.canvas.draw()    

def set_values():
    global {0[slider_vars]}
    changed = 0
    if( slider1_setup is not None):
        newH=slider1.get()
        if(newH != {0[slider1_var]}):
            {0[slider1_var]} = newH
            changed = 1
    if( slider2_setup is not None ):
        newH=slider2.get()
        if(newH != {0[slider2_var]}):
            {0[slider2_var]} = newH
            changed = 1
    if( slider3_setup is not None ):
        newH=slider3.get()
        if(newH != {0[slider3_var]}):
            {0[slider3_var]} = newH
            changed = 1
    if(changed):
        plot1.cla()
        plot1.set_autoscaley_on(False)
        plot1.set_ylim(ylimit)
        plot1.set_xlim(xlimit)
        fig.canvas.draw()
    hamilButton["fg"] = "black"        


def remove_last():
    global txtLabel
    if(len(orbits) > 0):
        tmp = orbits.pop()
        tmp.remove()
        if(len(points)>0):
            tmp = points.pop()
            tmp.remove()                
        fig.canvas.draw()
        txtLabel["text"] = " "

def set_range():
    global entry_xmin,entry_xmax,entry_ymin,entry_ymax, plot1,xlimit,ylimit;
    xmin =float(entry_xmin.get())
    xmax =float(entry_xmax.get()) 
    ymin =float(entry_ymin.get())
    ymax =float(entry_ymax.get())
    if(xmin != xlimit[0] or xmax != xlimit[1] or ymin != ylimit[0] or ymax != ylimit[1]):
        xlimit=[xmin, xmax]
        ylimit=[ymin, ymax]
        plot1.set_ylim(ylimit)
        plot1.set_xlim(xlimit)
        fig.canvas.draw()    
    else:
        print("Range is current.")


def update_xrange_entries(v):
    v1=v[0]
    v2=v[1]
    entry_xmin.delete(0, END)
    entry_xmin.insert(0, '{{:.6f}}'.format(v[0]))
    entry_xmax.delete(0, END)        
    entry_xmax.insert(0, '{{:.6f}}'.format(v[1]))

def update_yrange_entries(v):
    v1=v[0]
    v2=v[1]
    entry_ymin.delete(0, END)
    entry_ymin.insert(0, '{{:.6f}}'.format(v[0]))
    entry_ymax.delete(0, END)        
    entry_ymax.insert(0, '{{:.6f}}'.format(v[1]))
    
def move_xlimit(z):
    global xpslider, plot1,xlimit,ylimit, xslimit,yslimit,last_xlimit;    
    v=float(xpslider.get())
    if(last_xlimit == 2): # scaled
        xlimit[0]=xslimit[0]
        xlimit[1]=xslimit[1]
        xsslider.set(1)        
    r=xlimit[0]-xlimit[1]
    d= v * r
    if(d != 0):
        xxlimit[0]= xlimit[0]+d
        xxlimit[1]= xlimit[1]+d
        plot1.set_xlim(xxlimit)
        fig.canvas.draw()
        last_xlimit=1
        update_xrange_entries(xxlimit)

def move_ylimit(z):
    global ypslider, plot1,xlimit,ylimit, xslimit,yslimit, last_ylimit;    
    v=float(ypslider.get())
    if(last_ylimit == 2):  # was scaled
        ylimit[0] = yslimit[0]
        ylimit[1] = yslimit[1]
        ysslider.set(1)
    r=ylimit[1]-ylimit[0]
    d= v * r
    if(d != 0):
        yylimit[0]= ylimit[0]+d
        yylimit[1]= ylimit[1]+d
        plot1.set_ylim(yylimit)
        fig.canvas.draw()
        last_ylimit=1
        update_yrange_entries(yylimit)

def scale_xlimit(z):
    global ysslider, plot1,xlimit,ylimit,yylimit,xxlimit,last_xlimit;    
    v= 2.0 - float(xsslider.get())
    if(last_xlimit == 1):  # was moved
        xlimit[0] = xxlimit[0]
        xlimit[1] = xxlimit[1]
        xpslider.set(0)                
    cx = 0.5*(xlimit[0]+xlimit[1])
    d = 0.5*v * (xlimit[1]-xlimit[0])
    if(d != 0):
        xslimit[0]= cx - d
        xslimit[1]= cx + d
        plot1.set_xlim(xslimit)
        fig.canvas.draw()            
        last_xlimit=2
        update_xrange_entries(xslimit)
        
def scale_ylimit(z):
    global ysslider, plot1,xlimit,ylimit,yylimit,xxlimit,last_ylimit;    
    v= 2.0-float(ysslider.get())
    if(last_ylimit == 1):  # was moved
        ylimit[0] = yylimit[0]
        ylimit[1] = yylimit[1]
        ypslider.set(0)                        
    cy = 0.5*(ylimit[0]+ylimit[1])
    d = 0.5*v * (ylimit[1]-ylimit[0])
    if(d != 0):
        yslimit[0]= cy - d
        yslimit[1]= cy + d
        plot1.set_ylim(yslimit)
        fig.canvas.draw()            
        last_ylimit=2
        update_yrange_entries(yslimit)


class Orbiter:
    def __init__(self, artist):
        self.artist = artist
        self.cid = artist.figure.canvas.mpl_connect('button_press_event', self)

    def __call__(self, event):
        global work_in_progress, the_color, orbits, columns, plot1,fig,txtLabel
        if work_in_progress: return
        if event.inaxes!=self.artist.axes: return
        x=event.xdata
        y=event.ydata
        the_color = the_color + 1
        the_color = the_color % len(colors)
        clr= colors[the_color]        
        xy=self.artist.scatter([x],[y],c=clr)
        self.artist.figure.canvas.draw()
        txtLabel["text"] = "Point: ( "+ '{{:.6f}}'.format(x) +" , " + '{{:.6f}}'.format(y) +" )"
        txtLabel["fg"] = clr
        work_in_progress = 1
        window.config(cursor="watch") 
        window.update()       
        status = add_orbit(x,y)
        if(status == 0):
            xy.remove()
            txtLabel["text"] = "Bad Point: ( "+ '{{:.6f}}'.format(x) +" , " + '{{:.6f}}'.format(y) +" )"            
            work_in_progress = 0        
            window.config(cursor="") 
            return
        window.config(cursor="") 
        outfile="tmp-psection.data"        
        res = np.loadtxt(outfile)
        res = list(res.transpose())        
        #sizes = [1]*len(res[columns[0]])
        #siz=(9./fig.dpi)**2
        #print("data-size=",len(res[columns[0]]))         
        siz=0.01
        tmp=plot1.scatter(res[columns[0]],res[columns[1]],c=clr,s=siz)
        orbits.append(tmp);
        if(len(points)>0):
            last_point = points.pop()
            last_point.remove()
        points.append(xy);        
        fig.canvas.draw()
        work_in_progress = 0        


# build UI
window = Tk()
frame=Frame(window)
window.rowconfigure(tuple(range(8)), weight=1)
window.columnconfigure(tuple(range(16)), weight=1)
#frame.rowconfigure(7, weight=1)
#frame.columnconfigure(0, weight=1)
frame.columnconfigure(tuple(range(16)), weight=1)
frame.rowconfigure(tuple(range(8)), weight=1)

window.title('Poincare section example')
#window.geometry("1024x640")
#window.resizable(0,0)

has_slider=0
# slider 1
if( slider1_setup is not None):
    if(len(slider1_setup) and len(slider1_setup[0])):
        Label(window, text=slider1_setup[0]).grid(row=0,column=1,columnspan=3,pady=4,padx=4,sticky=S)
        slider1 = Scale(window, from_=slider1_setup[1],to=slider1_setup[2], orient=HORIZONTAL, resolution=slider1_setup[3], length=400, command=warn_change)
        slider1.grid(row=0,column=4,columnspan=8,padx=4,pady=4,sticky='NEWS')
        slider1.set({0[slider1_var]})
        has_slider=1

# slider 2
if( slider2_setup is not None):
    if(len(slider2_setup) and len(slider2_setup[0])):
        Label(window, text=slider2_setup[0]).grid(row=1,column=1,columnspan=3,pady=4,padx=4,sticky=S)
        slider2 = Scale(window, from_=slider2_setup[1],to=slider2_setup[2], orient=HORIZONTAL, resolution=slider2_setup[3], length=400, command=warn_change)
        slider2.grid(row=1,column=4,columnspan=8,padx=4,pady=4,sticky='NEWS')
        slider2.set({0[slider2_var]})
        has_slider=1        

# slider 3
if( slider3_setup is not None ):
    if(len(slider3_setup) and len(slider3_setup[0])):
        Label(window, text=slider3_setup[0]).grid(row=2,column=1,columnspan=3,pady=4,padx=4,sticky=S)
        slider3 = Scale(window, from_=slider3_setup[1],to=slider3_setup[2], orient=HORIZONTAL, resolution=slider3_setup[3], length=400, command=warn_change)
        slider3.grid(row=2,column=4,columnspan=8,padx=4,pady=4,sticky='NEWS')
        slider3.set({0[slider3_var]})
        has_slider=1        

#set hamiltonian button        
if( has_slider):        
    hamilButton=Button(window,  width=12, text='Set and Clear', command=set_values);
    hamilButton.grid(row=0,column=13,columnspan=3,sticky='WS',padx=4,pady=4)

#change bg button
    bgButton=Button(window,  width=12, text='Toggle BG Color', command=toggle_bg_color);
    bgButton.grid(row=6,column=13,columnspan=2,sticky='WS',padx=4,pady=10)

#slider n, on row 5 
Label(window, text=slidern_setup[0]).grid(row=5,column=1,columnspan=3,pady=4,padx=4,sticky=S)
slidern = Scale(window, from_=slidern_setup[1], to=slidern_setup[2], orient=HORIZONTAL, resolution=slidern_setup[3], length=400, command=set_npoints)
slidern.grid(row=5,column=4,columnspan=8,padx=4,pady=4,sticky='NEWS')
slidern.set(npoints)


#lable to display picked point 
txtLabel=Label(window,text="");
txtLabel.grid(row=6,column=4,columnspan=6,sticky='NEWS',pady=10)

#remove last orbit
Button(window,command = remove_last, width=10, text = "Remove Last").grid(row=6,column=10,columnspan=2,sticky='WENS',pady=10)

xlabel=Label(window,text="X Range");
ylabel=Label(window,text="Y Range");
entry_xmin=Entry(window,textvariable=win_xmin);
entry_xmax=Entry(window,textvariable=win_xmax);
entry_ymin=Entry(window,textvariable=win_ymin);
entry_ymax=Entry(window,textvariable=win_ymax);
range_btn=Button(window, command= set_range, width=12, text="Set XY ranges");

entry_xmin.insert(0,win_xmin)
entry_xmax.insert(0,win_xmax)
entry_ymin.insert(0,win_ymin)
entry_ymax.insert(0,win_ymax)

xlabel.grid(row=7,column=1,columnspan=1,sticky='E')
ylabel.grid(row=7,column=7,columnspan=1,sticky='E')
entry_xmin.grid(row=7,column=2,columnspan=2);
entry_xmax.grid(row=7,column=4,columnspan=2);
entry_ymin.grid(row=7,column=8,columnspan=2);
entry_ymax.grid(row=7,column=10,columnspan=2);
range_btn.grid(row=7,column=13,columnspan=2,pady=10,sticky="WS")

# canvas
canvas = FigureCanvasTkAgg(fig, master = window)
canvas.get_tk_widget().grid(row=9,column=0,rowspan=21,columnspan=15)

window.grid_columnconfigure(14,weight=0);
# scale canvas height
Label(window, text="S\\nc\\na\\nl\\ne\\n \\ny\\nl\\ni\\nm\\ni\\nt", fg="#0000cc").grid(row=9,column=16,columnspan=1,rowspan=1,pady=4,padx=0,sticky="N")
ysslider= Scale(window, from_=0, to=2, orient=VERTICAL, resolution=0.02, length=400, command=scale_ylimit,background="#ffffcc",showvalue=0)
ysslider.grid(row=10,column=16,columnspan=1,rowspan=20,sticky='NS',pady=10,padx=0)
ysslider.set(1)

Label(window, text="M\\no\\nv\\ne\\n \\ny\\nl\\ni\\nm\\ni\\nt").grid(row=9,column=15,columnspan=1,rowspan=1,pady=4,padx=0,sticky="N")
ypslider= Scale(window, from_=-1, to=1, orient=VERTICAL, resolution=0.02, length=400, command=move_ylimit, background="#eeeeff",showvalue=0)
ypslider.grid(row=10,column=15,columnspan=1,rowspan=20,sticky='NS',pady=10,padx=0)
ypslider.set(0)



Label(window, text="Scale xlimit").grid(row=31,column=1,columnspan=1,pady=4,padx=4,sticky=E)
xsslider= Scale(window, from_=0, to=2, orient=HORIZONTAL, resolution=0.02, length=400, command=scale_xlimit, background="#ffffcc",showvalue=0)
xsslider.grid(row=31,column=2,columnspan=13,sticky='NEWS',pady=4)
xsslider.set(1)

Label(window, text="Move xlimit").grid(row=30,column=1,columnspan=1,pady=4,padx=4,sticky=E)
xpslider= Scale(window, from_=-1, to=1, orient=HORIZONTAL, resolution=0.02, length=400, command=move_xlimit, background="#eeeeff",showvalue=0)
xpslider.grid(row=30,column=2,columnspan=13,sticky='NEWS',pady=10)
xpslider.set(0)

Label(window, text="Integrator: ").grid(row=6,column=0,columnspan=1,pady=4,padx=4,sticky=E)
integrator=IntVar(None,8)
R1 = Radiobutton(window, text="Taylor", variable=integrator, value=8,command=set_ps_method)
R2 = Radiobutton(window, text="RK4", variable=integrator, value=2,command=set_ps_method)
R3 = Radiobutton(window, text="RK4 With Adaptive Step", variable=integrator, value=4,command=set_ps_method)
R1.grid(row=6,column=1,columnspan=1,sticky=W)
R2.grid(row=6,column=2,columnspan=1,sticky=W)
R3.grid(row=6,column=3,columnspan=1,sticky=W)


def main():

    parser = argparse.ArgumentParser(epilog="Compute Poincare sections interactively for {0[model_name]}.")
    parser.add_argument('-f', '--function_name', type=str, dest='pname', help='set poincare section function name' )
    parser.add_argument('-o', '--output_file', type=str, dest='output_file', help='file to output data' )
    parser.add_argument('-p', '--plot', type=str, dest='columns', help='select two columns to plot')        
    parser.add_argument('-t0', '--start_time', type=str, dest='start_t')    
    parser.add_argument('-t1', '--stop_time', type=str, dest='stop_t')
    parser.add_argument('-h0', '--step_size', type=str, dest='step_size', help="initial step size, only used when step size control is 0")
    parser.add_argument('-nsteps', '--num_steps', type=str, dest='num_steps')
    parser.add_argument('-nintersections', '--num_intersections', type=str, dest='num_intersections', help="stop_time, nsteps and nintersections are stop conditions, integration stops when one of the limit is reached")
    parser.add_argument('-xing', '--crossing', type=str, dest='crossing', help="crossing, -1,1 or 0; cross the plane from left, right or both" )
    parser.add_argument('-method', '--method', type=str, dest='method', help="bit 0 select method to compute intersection: 0 RK4 integration backwards, 1 for using 6th degree fixed step taylor. bit 1 select fixed step RK4 as the main integrator. bit 2 select adaptive RK4 as the main integrator.")            
    parser.add_argument('-epsilon', '--epsilon', type=str, dest='epsilon', help="Error tolerance when compute the intersection. If abs(value) is smaller than epsilon, it is considered on the plane,")            
    parser.add_argument('-sc', '--stepsize_control', type=str, choices=['0','1','2'], dest='step_ctl', help="Only 1 is availale when jet var is present")    
    parser.add_argument('-abs_err', '--absolute_error_tolerance', type=str, dest='abs_err')
    parser.add_argument('-rel_err', '--relative_error_tolerance', type=str, dest='rel_err')    

    args = parser.parse_args()    

    global g_outfile,g_columns,g_start_t,g_stop_t,g_epsilon,g_abs_err,g_rel_err,g_num_steps,npoints,g_crossing,g_method,g_step_ctl,g_step_t

    if(args.output_file):
        g_outfile = args.output_file        
    if(args.pname):
        g_pname = args.pname
    if(args.columns):
        tmp= args.columns.split(',')
        g_columns=[ int(x)-1 for x in tmp]
    if(args.start_t):    
        g_start_t= float(args.start_t)
    if(args.stop_t):    
        g_stop_t= float(args.stop_t)
    if(args.epsilon):    
        g_epsilon= float(args.epsilon)
    if(args.abs_err):    
        g_abs_err= math.log10(float(args.abs_err))
    if(args.rel_err):    
        g_rel_err= math.log10(float(args.rel_err))
    if(args.num_steps):    
        g_num_steps= int(args.num_steps)
    if(args.num_intersections):    
        npoints= int(args.num_intersections)
    if(args.crossing):    
        g_crossing= int(args.crossing)
    if(args.method):    
        g_method= int(args.method)
    if(args.step_ctl):    
        g_step_ctl= int(args.step_ctl)
    if(args.step_size):    
        g_step_t= float(args.step_size)

    if(g_step_t < 0 or g_stop_t < g_start_t):
        print("\\nThis program can only integrate forward. You have step_size={{}}, start_t={{}}, stop_t={{}}\\n".format(g_step_t, g_start_t, g_stop_t));
        exit(1)


    canvas.draw()        
    Orbiter(plot1)

    window.mainloop()

if __name__ == "__main__":
    main()

"""




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

def create_list(r1, r2):
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
    def gen_content():
        parser = source_code
        for character in content:
            parser, text = parser(character)
            yield text

    return ''.join(gen_content())

#========================================================
def shell_comment(char):
    if char == '/':
        return comment_begin1, ''
    if char == '#':
        return sh_comment1, ''
    return shell_comment, ''

def sh_comment1(char):
    if char == '\n':
        return shell_comment, ';\n'
    return sh_comment1, char

def comment_begin1(char):
    if char == '/':
        return inline_comment1, ''
    if char == '*':
        return block_comment1, ''
    return shell_comment, ''

def inline_comment1(char):
    if char == '\n':
         return shell_comment, ''
    return inline_comment1, ''

def block_comment1(char):
    if char == '*':
        return end_block_comment1, ''
    return block_comment1, ''

def end_block_comment1(char):
    if char == '/':
        return shell_comment, ''
    return block_comment1, ''

def extract_sh_comments(content):
    def gen_content1():
        parser = shell_comment
        for character in content:
            parser, text = parser(character)
            yield text
    return ''.join(gen_content1())
#========================================================

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
#========================================================

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

    RE_EXTERN_int=re.compile(r'extern\s+int\s+(.+);',re.IGNORECASE)    
    RE_EXTERN_double=re.compile(r'extern\s+double\s+(.+);',re.IGNORECASE)
    RE_EXTERN_myfloat=re.compile(r'extern\s+MY_FLOAT\s+(.+);',re.IGNORECASE)            
    RE_EXTERN=re.compile(r'extern\s+(.+);',re.IGNORECASE)    

    P_INIT_RE = re.compile(r'^poincare_section_initials?\s*=\s*([^;]+)\s*;?',re.IGNORECASE)
    P_XLIMIT_RE = re.compile(r'^poincare_section_xlimit?\s*=\s*([^;]+)\s*;?',re.IGNORECASE)
    P_YLIMIT_RE = re.compile(r'^poincare_section_ylimit?\s*=\s*([^;]+)\s*;?',re.IGNORECASE)
    P_SLIDER1_RE = re.compile(r'^poincare_section_slider1?\s*=\s*([^;]+)\s*;?',re.IGNORECASE)
    P_SLIDER2_RE = re.compile(r'^poincare_section_slider2\s*=\s*([^;]+)\s*;?',re.IGNORECASE)                
    P_SLIDER3_RE = re.compile(r'^poincare_section_slider3\s*=\s*([^;]+)\s*;?',re.IGNORECASE)
    P_SLIDERn_RE = re.compile(r'^poincare_section_slidern\s*=\s*([^;]+)\s*;?',re.IGNORECASE)                
    P_COLUMNS_RE = re.compile(r'^poincare_section_columns?\s*=\s*([^;]+)\s*;?',re.IGNORECASE)
    P_MOUSE_RE = re.compile(r'^poincare_section_mouse?\s*=\s*([^;]+)\s*;?',re.IGNORECASE)        

    taylor_defs = {'_not_used_': '_xx_',};
    
    ps_state_vars=[]
    ps_init=[0]*10
    ps_xlimit="-1,1"
    ps_ylimit="-1,1"
    ps_columns=None
    ps_columns_computed=None    
    ps_mouse_x="x"
    ps_mouse_y="y"        
    ps_npoints=1000
    ps_slider1_var='notused'
    ps_slider1=""
    ps_slider2_var='notused'
    ps_slider2=""
    ps_slider3_var='notused'
    ps_slider3=""
    ps_slider_var_list=[]
    ps_slidern_var='npoints'
    ps_slidern="'Number of Intersections',1000,50000,1000,2000,npoints"
    slidern_setup="slidern_setup=["+ps_slidern+"]"

    slider1_setup=""
    slider1_setup_var=""
    slider2_setup=""
    slider2_setup_var=""
    slider3_setup=""
    slider3_setup_var=""        
    
    data=""
    extern="notused "
    iextern=""
    dextern=""
    mextern=""    
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
    mpfr=0
    f128=0    
    format_string="%.17f"
    mpfr_lib_loader=""
    f128_lib_loader=""    
    mpfr_init=""    
    pname=""
    
    parser = argparse.ArgumentParser(epilog=psection_usage,formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('-f', '--function_name', type=str, dest='pname', help='set poincare section function name' )
    parser.add_argument('-m', '--model_name', type=str, dest='model_name')
    parser.add_argument('-mpfr', '--mpfr_precision', type=str, dest='mpfr')
    parser.add_argument('-f128', '--float128', dest='f128', action='store_true')    
    parser.add_argument('-t0', '--start_time', type=str, dest='start_t')    
    parser.add_argument('-t1', '--stop_time', type=str, dest='stop_t')
    parser.add_argument('-nsteps', '--num_steps', type=str, dest='num_steps')
    parser.add_argument('-nintersections', '--num_intersections', type=str, dest='num_intersections')
    parser.add_argument('-xing', '--crossing', type=str, dest='crossing')
    parser.add_argument('-method', '--method', type=str, dest='method')            
    parser.add_argument('-sc', '--stepsize_control', choices=[0,1,2], dest='step_ctl', help="Only 1 is availale when jet var is present")    
    parser.add_argument('-step', '--step_size', type=str, dest='step_size')
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
        file_path = 'tmp-psection.eq'
        with open(file_path,"w") as f:
            f.write(data)
            
    clean = remove_comments(data)
    source_lines = canonical_input(clean)    
    comments = extract_sh_comments(data)
    comment_lines = canonical_input(comments)

    # processing source code
    for line in source_lines:
        match = RE_DIFF_EQ.match(line)
        matched=0
        if(match):
            num_eqns += 1
            matched=1
            if(match.group(1) is None):
                ps_state_vars.append(match.group(2))
            else:
                ps_state_vars.append(match.group(1))                
        match = RE_INIT_V.match(line)
        if(match):
            init_v = match.group(1)
            matched=1            
        match = RE_START_TIME.match(line)
        if(match):
            start_t = match.group(1)
            matched=1
        match = RE_STOP_TIME.match(line)
        if(match):
            stop_t = match.group(1)
            matched=1            
        match = RE_NUM_STEPS.match(line)
        if(match):
            num_steps = match.group(1)
            matched=1            
        match = RE_ABS_ERROR.match(line)
        if(match):
            abs_err = match.group(1)
            matched=1            
        match = RE_REL_ERROR.match(line)
        if(match):
            rel_err = match.group(1)
            matched=1            
        match = RE_STEP_SIZE.match(line)
        if(match):
            step_size = match.group(1)
            matched=1            
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
            num_intersections = match.group(1)
            matched=1            
        match = RE_XING.match(line)                
        if(match):
            crossing = match.group(1)
            matched=1
        match = RE_METHOD.match(line)                
        if(match):
            method = match.group(1)
            matched=1
            
        if(matched == 0):
            match = RE_DEFINE.match(line)                
            if(match):
                taylor_defs[match.group(1)] = match.group(2);
                                   
        matched=0
        match = RE_EXTERN_int.match(line)
        if(match):
            iextern = iextern + line;
            matched = 1
        match = RE_EXTERN_double.match(line)
        if(match):
            dextern = dextern + line;
            matched = 1
        match = RE_EXTERN_myfloat.match(line)
        if(match):
            mextern = mextern + line;
            matched = 1
        if(matched == 0):
            match = RE_EXTERN.match(line)                
            if(match):
                extern = match.group(1)

                    
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
   
    # processing psection command lines, they are shell comments
    for line in comment_lines:
        match= P_INIT_RE.match(line)
        if(match):
                tmp = match.group(1).split(',')
                ps_init=[x.strip() for x in tmp]
        match= P_XLIMIT_RE.match(line)
        if(match):
                ps_xlimit=match.group(1)
        match= P_YLIMIT_RE.match(line)
        if(match):
                ps_ylimit=match.group(1)
        match= P_COLUMNS_RE.match(line)
        if(match):
                ps_columns=match.group(1)                        
        match= P_SLIDER1_RE.match(line)
        if(match):
                ps_slider1=match.group(1)
                tmp=ps_slider1.split(",")
                ps_slider1_var = tmp[-1].strip()
                ps_slider_var_list.append(ps_slider1_var)
                slider1_setup="slider1_setup=["+ps_slider1+"]"
                slider1_setup_var=ps_slider1_var +"="+tmp[-2].strip()                
        match= P_SLIDER2_RE.match(line)
        if(match):
                ps_slider2=match.group(1)
                tmp=ps_slider2.split(",")
                ps_slider2_var=tmp[-1].strip()
                ps_slider_var_list.append(ps_slider2_var)
                slider2_setup="slider2_setup=["+ps_slider2+"]"
                slider2_setup_var=ps_slider2_var +"="+tmp[-2].strip()                                        
        match= P_SLIDER3_RE.match(line)
        if(match):
                ps_slider3=match.group(1)
                tmp=ps_slider3.split(",")
                ps_slider3_var=tmp[-1].strip()
                ps_slider_var_list.append(ps_slider3_var)
                slider3_setup="slider3_setup=["+ps_slider3+"]"
                slider3_setup_var=ps_slider3_var +"="+tmp[-2].strip()                                                        
        match= P_SLIDERn_RE.match(line)
        if(match):
                ps_slidern=match.group(1)
                slidern_setup="slidern_setup=["+ps_slidern+"]"                
                
        match= P_MOUSE_RE.match(line)
        if(match):
                smp = match.group(1)
                tmp=smp.split(",")
                ps_mouse_x=tmp[0].strip()
                ps_mouse_y=tmp[1].strip()
                i=ps_state_vars.index(ps_mouse_x)
                j=ps_state_vars.index(ps_mouse_y)                
                ps_columns_computed="{},{}".format(i,j)

    if(ps_columns is None):
        ps_columns = ps_columns_computed
    # process command line arguments
    if(args.f128):
        f128 = 1    
    if(args.model_name):
        model_name = args.model_name
    if(args.pname):
        pname = args.pname        
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

    if(model_name == 'ode_name'):
        if(args.input_file):
            tmp = args.input_file
            model_name = 'ode_name_'+ RE_NON_WORD_CHARS.sub('_',tmp)

    if(shared_lib_name == 'tp_temp_lib'):
        shared_lib_name = 'lib_' + model_name

    if(f128):
        f128_lib_loader="ctypes.CDLL( 'libquadmath.so.0' , mode = ctypes.RTLD_GLOBAL)"
        
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

        if(len(ps_slider_var_list)):
                slider_vars = ",".join(ps_slider_var_list)
        else:
                slider_vars = "not_used"

        # extern var + slider var list
        l1 = slider_vars.split(",")
        l2 = extern.split(",")
        l3 = l1 + l2
        l4 = [ x.strip() for x in l3 ]
        l5 = [ *set(l4) ]
        extern_vars = ",".join(l5)

        extern_vars = extern_vars;
        typed_extern_vars = iextern + dextern + mextern
        
        # slider var list, ordered
        l4 = [ x.strip() for x in l1 ]
        l5 = [ *set(l4) ]
        l6 = [ 'double_'+x for x in l5 ]
        double_slider_vars = "double " + ", double ".join(l6)        
        l7 = [ 'MakeMyFloatA('+x+','+'double_'+x+');' for x in l5 ]
        myfloat_slider_vars= "\n  ".join(l7)
        l7 = [ 'InitMyFloat('+x+');' for x in l5 ]        
        init_myfloat_slider_vars= "\n  ".join(l7)        
        myfloat_vars_count = len(l7)
        tmp = ['ctypes.c_double'] * myfloat_vars_count
        ctype_args = ",".join(tmp)

        C_header_file = "tps_" + model_name + ".h"
        C_wrapper_file = "tps_"+ model_name + ".c"
        C_stepper_file = "tps_"+ model_name + "_step.c"        
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
                'f128_lib_loader': f128_lib_loader,
                'jet_lib': jet_lib,
                'step_size': step_size,
                'step_t': step_size,                        
                'num_intersections': num_intersections,
                'crossing' : crossing,
                'method' : method,
                'epsilon' : epsilon,
                'extern' : extern,

                'taylor_vars': taylor_vars,
            
                'init_expr': ps_init,
                'xlimit': ps_xlimit,
                'ylimit': ps_ylimit,
                'columns': ps_columns,
                'slider1': ps_slider1,
                'slidern': ps_slidern,
                'npoints': ps_npoints,
                'slider1_var': ps_slider1_var,
                'slider2_var': ps_slider2_var,
                'slider3_var': ps_slider3_var,
                'slider_var_list': slider_vars,
                'slider_vars': slider_vars,                
                'double_slider_vars': double_slider_vars,
                'myfloat_slider_vars': myfloat_slider_vars,
                'init_myfloat_slider_vars': init_myfloat_slider_vars,            
                'myfloat_vars_count': myfloat_vars_count,
                'mouse_x': ps_mouse_x,
                'mouse_y': ps_mouse_y,
                'extern_vars': extern_vars,
                'typed_extern_vars' : typed_extern_vars,                        
                'ctype_args': ctype_args,
                'slider1_setup': slider1_setup,
                'slider1_setup_var':slider1_setup_var,
                'slider2_setup': slider2_setup,
                'slider2_setup_var':slider2_setup_var,                
                'slider3_setup': slider3_setup,
                'slider3_setup_var':slider3_setup_var,
                'slidern_setup': slidern_setup,
            
        }

        
#        pp = pprint.PrettyPrinter(indent=4)
#        pp.pprint(data)
        with open(C_wrapper_file,"w") as f:
            c_code = c_wrapper_code.format(data)
            f.write(c_code)                

        taylor_error = ""
        
        # generate taylor header file
        cmd_h = ['taylor', '-header', '-expression', '-poincare', '-name', model_name ];
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
        process = subprocess.Popen(['cc', '-w', '-O3', '-fPIC', '-shared', '-o', C_shared_lib_file,  C_stepper_file, C_wrapper_file],
                                   stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()

        if(stderr):
            taylor_error = taylor_error  + "Taylor was unable to compile a shared library: "+ stderr.decode() + "\n"                                

        if(taylor_error):
            print(taylor_error)

            
        p_code = python_sample_code.format(data)
        P_sample_code_file = None
        if(output_file):
            P_sample_code_file = output_file
            with open(P_sample_code_file,"w") as f:
                f.write(p_code)
            os.chmod(P_sample_code_file, 0o755)
        else:
            print(p_code)        
                
        
if __name__ == "__main__":
    main()
