/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 2022 Joan Gimeno, Angel Jorba, Maorong Zou
 *
 *
 *  This file is part of taylor.
 *
 * Taylor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * Taylor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taylor; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 *************************************************************************/

/*
fortran users should call this routine instead of using the main call.

WARNING: in some systems (like Linux) you have to add a '_' after
the name of the called routine in your fortran program:

      call taylor_f77_NAME_(ti,x,d,ssc,l10ae,l10re,et,ht,ord)
*/


/* typedef double MY_FLOAT;*/

static char *OneStepCodeF77 = 
"\
\n\
void taylor_f77__ODE_NAME____(MY_FLOAT *ti,\n\
                           MY_FLOAT *x,\n\
                           int      *dir,\n\
                           int      *step_ctl,\n\
                           double   *log10abserr,\n\
                           double   *log10relerr,\n\
                           MY_FLOAT *endtime,\n\
                           MY_FLOAT *ht,\n\
                           int      *order,\n\
                           int      *flag)\n\
/*\n\
 * This is a wrapping routine to call the taylor integrator from a\n\
 * fortran 77 program. this routine should be called as\n\
 *\n\
 *       call taylor_f77_ODE_NAME(...)\n\
 *\n\
 * Note that, in the call, we have removed the string \"__\" at the\n\
 * end of the name. The reason is that the standard GNU compiler (g77)\n\
 * adds \"__\" at the end of the name of the procedures and the C compiler\n\
 * (gcc) does not.\n\
 *\n\
 * WARNING: different compilers could use different alterations of these\n\
 * names. In that case, you should modify the name of this routine\n\
 * accordingly.\n\
 *\n\
 * The meaning of the parameters is the same as in the C main call:\n\
 *\n\
 * ti: on input:  time of the initial condition\n\
 *     on output: new time\n\
 *\n\
 * x:  on input:  initial condition\n\
 *     on output: new condition, corresponding to the (output) time ti\n\
 *\n\
 * dir: flag to integrate forward or backwards.\n\
 *     1: forward\n\
 *    -1: backwards\n\
 *     WARNING: this flag is ignored if step_ctl (see below) is set to 0.\n\
 *\n\
 * step_ctl: flag for the step size control. the possible values are:\n\
 *     0: no step size control, so the step and order are provided by\n\
 *        the user. the parameter ht is used as step, and the parameter\n\
 *        order (see below) is used as the order.\n\
 *     1: standard stepsize control. it uses an approximation to the\n\
 *        optimal order and to the radius of convergence of the series\n\
 *        to approximate the 'optimal' step size. It tries to keep the\n\
 *        absolute and relative errors below the given values. See the\n\
 *        paper for more details.\n\
 *     2: as 1, but adding an extra condition on the stepsize h: the\n\
 *        terms of the series --after being multiplied by the suitable\n\
 *        power of h-- cannot grow.\n\
 *     3: user defined stepsize control. the code has to be included\n\
 *        in the routine compute_timestep_user_defined (you can find\n\
 *        this routine below). The user should also provide code for\n\
 *        the selection of degree (see the function\n\
 *        compute_order_user_defined below).\n\
 *\n\
 * log10abserr: decimal log of the absolute accuracy required.\n\
 *\n\
 * log10relerr: decimal log of the relative accuracy required.\n\
 *\n\
 * endtime: if NULL, it does nothing. if step_ctl (see above) is set\n\
 *     to 0, it is also ignored. otherwise, if next step is outside\n\
 *     endtime, reduce the step size so that the new time ti is\n\
 *     exactly endtime (in that case, the function returns 1).\n\
 *\n\
 * ht: on input:  ignored/used as a time step (see parameter step_ctl)\n\
 *     on output: time step used\n\
 *\n\
 * order: degree of the taylor expansion.\n\
 *        input: this parameter is only used if step_ctl is 0,\n\
 *               or if you add the code for the case step_ctl=3.\n\
 *               its possible values are:\n\
 *               < 2: the program will select degree 2 (if step_ctl is 0).\n\
 *               >=2: the program will use this degree (if step_ctl is 0).\n\
 *        ouput: degree used.\n\
 *\n\
 * flag: on input: ignored.\n\
 *       on output: flag; its values can be:\n\
 *                  0: ok.\n\
 *                  1: ok, and ti=endtime.\n\
*/\n\
{\n\
  *flag=taylor_step__ODE_NAME__(ti,x,*dir,*step_ctl,*log10abserr,*log10relerr,\n\
                             endtime,ht,order,NULL);\n\
  return;\n\
}\n\
\n\
";
