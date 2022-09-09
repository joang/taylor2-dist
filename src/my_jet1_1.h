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

#ifndef MY_JET1_1_H
#define MY_JET1_1_H

/* *******  ****************  ******* */
/* *******  MY_JET1_1_HEADER  ******* */
/* *******  ****************  ******* */
#define MY_JET1_1_FIXES MY_JET_SPACE

#define MY_JET1_1_TYPE(PREFIX_JET1_1,SCAL,I) "\
typedef " SCAL " " PREFIX_JET1_1(t) "[2];\n\
typedef " PREFIX_JET1_1(t) "* " PREFIX_JET1_1(ptr) ";\n" \

#define MY_JET1_1_API(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET1_1_TYPE(PREFIX_JET1_1,SCAL,I) "\n\
\n\
#define " PREFIX_JET1_1(init) "(x) {\\\n\
\t" PREFIX_SCAL(init) "((*x)[0]);\\\n\
\t" PREFIX_SCAL(init) "((*x)[1]);\
}\n\
#define " PREFIX_JET1_1(initup) "(a,b) {}\n\
#define " PREFIX_JET1_1(clean) "(x) {\\\n\
\t" PREFIX_SCAL(clean) "((*x)[1]);\\\n\
\t" PREFIX_SCAL(clean) "((*x)[0]);\
}\n\
#define " PREFIX_JET1_1(cleanup) "(x) {}\n\
#define " PREFIX_JET1_1(set_deg) "(x) (1)\n\
#define " PREFIX_JET1_1(set_num_symbs) "(x) (1)\n\
\n\
/* y' = x' */\n\
#define " PREFIX_JET1_1(set) "(y,x) {\\\n\
\t" PREFIX_SCAL(set) "(y[0],x[0]);\\\n\
\t" PREFIX_SCAL(set) "(y[1],x[1]);\
}\n\
\n\
/* y' = (x[0],x[1]) */\n\
#define " PREFIX_JET1_1(set_scal_array) "(y,x) {\\\n\
\t" PREFIX_SCAL(set) "(y[0],x[0]);\\\n\
\t" PREFIX_SCAL(set) "(y[1],x[1]);\
}\n\
/* (y[0],y[1])=x */\n\
#define " PREFIX_JET1_1(set_jet_2_scal_array) "(y,x) {\\\n\
\t" PREFIX_SCAL(set) "(y[0],x[0]);\\\n\
\t" PREFIX_SCAL(set) "(y[1],x[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_scal) "(y,x) {\\\n\
\t" PREFIX_SCAL(set) "(y[0],x);\\\n\
\t" PREFIX_SCAL(set_zero) "(y[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_d) "(y,x) {\\\n\
\t" PREFIX_SCAL(set_d) "(y[0],x);\\\n\
\t" PREFIX_SCAL(set_zero) "(y[1]);\
}\n\
/* y' = 0 */\n\
#define " PREFIX_JET1_1(set_si) "(y,x) {\\\n\
\t" PREFIX_SCAL(set_si) "(y[0],x);\\\n\
\t" PREFIX_SCAL(set_zero) "(y[1]);\
}\
\n\
\n\
#define " PREFIX_JET1_1(to_scal) "(t,x) " PREFIX_SCAL(set) "(*t,x[0])\n\
\n\
/* y' = -x' */\n\
#define " PREFIX_JET1_1(neg) "(y,x) {\\\n\
\t" PREFIX_SCAL(neg) "(y[0],x[0]);\\\n\
\t" PREFIX_SCAL(neg) "(y[1],x[1]);\
}\n\
\n\
/* a = max(|x|,|x'|) */\n\
#define " PREFIX_JET1_1(nrminf) "(a,x) {\\\n\
\t" PREFIX_SCAL(t) " _aux_;\\\n\
\t" PREFIX_SCAL(init) "(_aux_);\\\n\
\t" PREFIX_SCAL(set_fabs) "(*a,x[0]);\\\n\
\t" PREFIX_SCAL(set_fabs) "(_aux_,x[1]);\\\n\
\tif (" PREFIX_SCAL(lt) "(*a,_aux_)) {" PREFIX_SCAL(set) "(*a,_aux_);}\\\n\
\t" PREFIX_SCAL(clean) "(_aux_);\
}\n\
/* a = (x^2+x'^2)^.5 */\n\
#define " PREFIX_JET1_1(nrm2) "(a,x) {\\\n\
\t" PREFIX_SCAL(t) " _aux_;\\\n\
\t" PREFIX_SCAL(init) "(_aux_);\\\n\
\t" PREFIX_SCAL(mul2) "(*a,x[0],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(_aux_,x[1],x[1]);\\\n\
\t" PREFIX_SCAL(add2) "(_aux_,_aux_,*a);\\\n\
\t" PREFIX_SCAL(set_sqrt) "(*a,_aux_);\\\n\
\t" PREFIX_SCAL(clean) "(_aux_);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(add2) "(z[0],x[0],y[0]);\\\n\
\t" PREFIX_SCAL(add2) "(z[1],x[1],y[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2) "(z[0],x[0],y[0]);\\\n\
\t" PREFIX_SCAL(sub2) "(z[1],x[1],y[1]);\
}\n\
\n\
/* z' = x'*y + y'*x\n\
 * Assump. z[1]!=x[1] && z[0]!=y[0] in memory \n\
 */\n\
#define " PREFIX_JET1_1(mul2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(mul2) "(z[1],x[0],y[1]);\\\n\
\t" PREFIX_SCAL(mul2) "(z[0],x[1],y[0]);\\\n\
\t" PREFIX_SCAL(add2) "(z[1],z[0],z[1]);\\\n\
\t" PREFIX_SCAL(mul2) "(z[0],x[0],y[0]);\
}\n\
\n\
/* z' = (x'*y-x*y')/y^2 = (x' - z*y')/y\n\
 * Assump. y[0]!=0, z[1]!=x[1] && z[0]!=y[0] in memory \n\
 */\n\
#define " PREFIX_JET1_1(div2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2) "(z[0],x[0],y[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(z[1],z[0],y[1]);\\\n\
\t" PREFIX_SCAL(sub2) "(z[1],x[1],z[1]);\\\n\
\t" PREFIX_SCAL(div2) "(z[1],z[1],y[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_scal) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(add2) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_scal) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(scal_sub2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_SCAL(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_scal) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(mul2) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(mul2) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0  */\n\
#define " PREFIX_JET1_1(div2_scal) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(div2) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = -x*y'/y^2 = -z*y'/y. Assump. y!=0  */\n\
#define " PREFIX_JET1_1(scal_div2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_SCAL(neg) "(z[1],z[1]);\\\n\
\t" PREFIX_SCAL(div2) "(z[0],x,y[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(z[1],z[1],z[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_d) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(add2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(set) "(z[1],x[1]));\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_d) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(set) "(z[1],x[1]);\
}\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(d_sub2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2_d) "(z[0],x,y[0]);\\\n\
\t" PREFIX_SCAL(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_d) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(mul2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(mul2_d) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0 */\n\
#define " PREFIX_JET1_1(div2_d) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2_d) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(div2_d) "(z[1],x[1],y);\
}\n\
/* z' = -z*y'/y. Assump. y!=0 and z[0]!=y[0] in memory */\n\
#define " PREFIX_JET1_1(d_div2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_SCAL(neg) "(z[1],z[1]);\\\n\
\t" PREFIX_SCAL(set_d) "(z[0],x);\\\n\
\t" PREFIX_SCAL(div2) "(z[0],z[0],y[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(z[1],z[1],z[0]);\
}\n\
\n\
/* z' = x'+y' */\n\
#define " PREFIX_JET1_1(add2_si) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(add2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(set) "(z[1],x[1]);\
}\n\
\n\
/* z' = x'-y' */\n\
#define " PREFIX_JET1_1(sub2_si) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(set) "(z[1],x[1]);\
}\n\
/* z' = (x-y)' = -y' */\n\
#define " PREFIX_JET1_1(si_sub2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(sub2_si) "(z[0],x,y[0]);\\\n\
\t" PREFIX_SCAL(neg) "(z[1],y[1]);\
}\n\
\n\
/* z' = x'*y */\n\
#define " PREFIX_JET1_1(mul2_si) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(mul2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(mul2_si) "(z[1],x[1],y);\
}\n\
\n\
/* z' = (x/y)' = x'/y. Assump. y!=0 */\n\
#define " PREFIX_JET1_1(div2_si) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2_si) "(z[0],x[0],y);\\\n\
\t" PREFIX_SCAL(div2_si) "(z[1],x[1],y);\
}\n\
/* z' = -z*y'/y. Assump. z[0]!=y[0] in memory */\n\
#define " PREFIX_JET1_1(si_div2) "(z,x,y) {\\\n\
\t" PREFIX_SCAL(div2) "(z[1],y[1],y[0]);\\\n\
\t" PREFIX_SCAL(neg) "(z[1],z[1]);\\\n\
\t" PREFIX_SCAL(set_si) "(z[0],x);\\\n\
\t" PREFIX_SCAL(div2) "(z[0],z[0],y[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(z[1],z[1],z[0]);\
}\n\
\n\
/* s' = x'/(2s). Assump. s!=0 */\n\
#define " PREFIX_JET1_1(set_sqrt) "(s,x) {\\\n\
\t" PREFIX_SCAL(set_sqrt) "(s[0],x[0]);\\\n\
\t" PREFIX_SCAL(div2) "(s[1],x[1],s[0]);\\\n\
\t" PREFIX_SCAL(div2_si) "(s[1],s[1],2);\
}\
\n\
/* p' = e*p*x'/x. Assump. x[0] != 0 */\n\
#define " PREFIX_JET1_1(set_pow_scal) "(p,x,e) {\\\n\
\t" PREFIX_SCAL(div2) "(p[1],x[1],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(p[1],p[1],e);\\\n\
\t" PREFIX_SCAL(set_pow) "(p[0],x[0],e);\\\n\
\t" PREFIX_SCAL(mul2) "(p[1],p[1],p[0]);\
}\
\n\
/* e' = e*x' */\n\
#define " PREFIX_JET1_1(set_exp) "(e,x) {\\\n\
\t" PREFIX_SCAL(set_exp) "(e[0],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(e[1],e[0],x[1]);\
}\
\n\
/* l' = x'/x */\n\
#define " PREFIX_JET1_1(set_log) "(l,x) {\\\n\
\t" PREFIX_SCAL(div2) "(l[1],x[1],x[0]);\\\n\
\t" PREFIX_SCAL(set_log) "(l[0],x[0]);\
}\
\n\
/* c' =-sin(x)*x'. Assump. c'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_cos) "(c,x) {\\\n\
\t" PREFIX_SCAL(set_sin) "(c[1],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(c[1],c[1],x[1]);\\\n\
\t" PREFIX_SCAL(neg) "(c[1],c[1]);\\\n\
\t" PREFIX_SCAL(set_cos) "(c[0],x[0]);\
}\
\n\
/* s' = cos(x)*x'. Assump. s'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_sin) "(s,x) {\\\n\
\t" PREFIX_SCAL(set_cos) "(s[1],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(s[1],s[1],x[1]);\\\n\
\t" PREFIX_SCAL(set_sin) "(s[0],x[0]);\
}\
\n\
/* t' = (t^2+1)*x'. Assump. t'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_tan) "(t,x) {\\\n\
\t" PREFIX_SCAL(set_tan) "(t[0],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(t[1],t[0],t[0]);\\\n\
\t" PREFIX_SCAL(add2_si) "(t[1],t[1],1);\\\n\
\t" PREFIX_SCAL(mul2) "(t[1],t[1],x[1]);\
}\
\n\
/* t' = x'/(x^2+1). Assump. t'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_atan) "(t,x) {\\\n\
\t" PREFIX_SCAL(mul2) "(t[1],x[0],x[0]);\\\n\
\t" PREFIX_SCAL(add2_si) "(t[1],t[1],1);\\\n\
\t" PREFIX_SCAL(div2) "(t[1],x[1],t[1]);\
\t" PREFIX_SCAL(set_atan) "(t[0],x[0]);\\\n\
}\
\n\
/* c' =sinh(x)*x'. Assump. c'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_cosh) "(c,x) {\\\n\
\t" PREFIX_SCAL(set_sinh) "(c[1],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(c[1],c[1],x[1]);\\\n\
\t" PREFIX_SCAL(set_cosh) "(c[0],x[0]);\
}\
\n\
/* s' = cosh(x)*x'. Assump. s'!=x' in memory */\n\
#define " PREFIX_JET1_1(set_sinh) "(s,x) {\\\n\
\t" PREFIX_SCAL(set_cosh) "(s[1],x[0]);\\\n\
\t" PREFIX_SCAL(mul2) "(s[1],s[1],x[1]);\\\n\
\t" PREFIX_SCAL(set_sinh) "(s[0],x[0]);\
}\
\n\
#define " PREFIX_JET1_1(fprintf) "(file,fmt,s) {\\\n\
\t" PREFIX_SCAL(fprintf) "(file,fmt,s[0]);\\\n\
\t" PREFIX_SCAL(fprintf) "(file,fmt,s[1]);\\\n\
}\
\n" \

#define MY_JET1_1_PREHEADER(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET1_1(t) " */\n"\

#define MY_JET1_1_HEADER(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
" MY_JET1_1_API(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_SCAL,SCAL,I) "\n" \

#define MY_JET1_1_POSTHEADER(PREFIX_JET1_1,MY_JET1_1_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET1_1(t) " */\n" \

/* *******  **************  ******* */
/* *******  MY_JET1_1_CODE  ******* */
/* *******  **************  ******* */
#define MY_JET1_1_PRECODE(PREFIX_JET1_1,PREFIX_SCAL,I) "\
/* CODE FOR " PREFIX_JET1_1(t) " */\n"\

#define MY_JET1_1_CODE(PREFIX_JET1_1,PREFIX_SCAL,I) "\
/* none */\n" \

#define MY_JET1_1_POSTCODE(PREFIX_JET1_1,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET1_1(t) " */\n" \

#endif /* MY_JET1_1_H */
