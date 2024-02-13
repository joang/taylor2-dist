/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
 *    Copyright (C) 2022  Joan Gimeno, Angel Jorba, Maorong Zou
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

#define RK45_C
#include "Header.h"

void genGeneralRK45Code() {

  int m = neqns - nonautonomous, n= state_jet_vars;

   fprintf(outfile, "\n\
#ifndef MY_FLOAT_IS_DOUBLE\n\
  static int rk45_constants_initialized;\n\
 \n\
  static MY_FLOAT xE1,xE2,xE3,xE4,xE5,xRK4_1,xRK4_2,xRK4_3,xRK4_4,xRK5_1,xRK5_2,xRK5_3,xRK5_4,xRK5_5,xC1,xC2,xC3,xC4,xC5,xC6,xC7,xC8,xC9,xC10,rk45_tmp1;\n\
  static MY_FLOAT xeight,xeight45,xfive,xfive0,xfive13,xfive5,xfive6430,xfour104,xfour275,xfour39,xnine,xone,xone2,xone28,xone2825,xone3,\n\
    xone35,xone408,xone6,xone859,xone932,xseven200,xseven296,xseven5240,xsix656,xthree544,xthree60,xthree680,xtwo,xtwo16,xtwo187,xtwo197,xtwo5,xtwo565,xtwo7,xtwo8561;\n\
  static MY_FLOAT xp25,xp5,xp09375,xp28125,xp275,xthree2,xfour,xfour0,xthree,xone1;\n\
  \n\
  static void rk45_init_constants() {			\n\
    if(! rk45_constants_initialized) {\n\
      InitMyFloat(xp25);\n\
      InitMyFloat(xp5);\n\
      InitMyFloat(xp09375);\n\
      InitMyFloat(xp28125);\n\
      InitMyFloat(xp275);\n\
      InitMyFloat(xtwo);\n\
      InitMyFloat(xfour);            \n\
      InitMyFloat(xfour0);\n\
      InitMyFloat(xthree);\n\
      InitMyFloat(xthree2);      \n\
      InitMyFloat(xone1);                  \n\
      InitMyFloat(rk45_tmp1);\n\
      InitMyFloat(xeight);\n\
      InitMyFloat(xeight45);\n\
      InitMyFloat(xfive);\n\
      InitMyFloat(xfive0);\n\
      InitMyFloat(xfive13);\n\
      InitMyFloat(xfive5);\n\
      InitMyFloat(xfive6430);\n\
      InitMyFloat(xfour104);\n\
      InitMyFloat(xfour275);\n\
      InitMyFloat(xfour39);\n\
      InitMyFloat(xnine);\n\
      InitMyFloat(xone);\n\
      InitMyFloat(xone2);\n\
      InitMyFloat(xone28);\n\
      InitMyFloat(xone2825);\n\
      InitMyFloat(xone3);\n\
      InitMyFloat(xone35);\n\
      InitMyFloat(xone408);\n\
      InitMyFloat(xone6);\n\
      InitMyFloat(xone859);\n\
      InitMyFloat(xone932);\n\
      InitMyFloat(xseven200);\n\
      InitMyFloat(xseven296);\n\
      InitMyFloat(xseven5240);\n\
      InitMyFloat(xsix656);\n\
      InitMyFloat(xthree544);\n\
      InitMyFloat(xthree60);\n\
      InitMyFloat(xthree680);\n\
      InitMyFloat(xtwo);\n\
      InitMyFloat(xtwo16);\n\
      InitMyFloat(xtwo187);\n\
      InitMyFloat(xtwo197);\n\
      InitMyFloat(xtwo5);\n\
      InitMyFloat(xtwo565);\n\
      InitMyFloat(xtwo7);\n\
      InitMyFloat(xtwo8561);\n\
      InitMyFloat(xE1);\n\
      InitMyFloat(xE2);\n\
      InitMyFloat(xE3);\n\
      InitMyFloat(xE4);\n\
      InitMyFloat(xE5);\n\
      InitMyFloat(xRK4_1);\n\
      InitMyFloat(xRK4_2);\n\
      InitMyFloat(xRK4_3);\n\
      InitMyFloat(xRK4_4);\n\
      InitMyFloat(xRK5_1);\n\
      InitMyFloat(xRK5_2);\n\
      InitMyFloat(xRK5_3);\n\
      InitMyFloat(xRK5_4);\n\
      InitMyFloat(xRK5_5);\n\
      InitMyFloat(xC1);\n\
      InitMyFloat(xC2);\n\
      InitMyFloat(xC3);\n\
      InitMyFloat(xC4);\n\
      InitMyFloat(xC5);\n\
      InitMyFloat(xC6);\n\
      InitMyFloat(xC7);\n\
      InitMyFloat(xC8);\n\
      InitMyFloat(xC9);\n\
      InitMyFloat(xC10);\n\
\n\
      MakeMyFloatC(xeight       ,\"8\",(double)8);	   \n\
      MakeMyFloatC(xeight45	 ,\"845\",(double)845);	   \n\
      MakeMyFloatC(xfive	 ,\"5\",(double)5);	   \n\
      MakeMyFloatC(xfive0	 ,\"50\",(double)50);	   \n\
      MakeMyFloatC(xfive13	 ,\"513\",(double)513);	   \n\
      MakeMyFloatC(xfive5	 ,\"55\",(double)55);	   \n\
      MakeMyFloatC(xfive6430	 ,\"56430\",(double)56430);   \n\
      MakeMyFloatC(xfour104	 ,\"4104\",(double)4104);	   \n\
      MakeMyFloatC(xfour275	 ,\"4275\",(double)4275);	   \n\
      MakeMyFloatC(xfour39	 ,\"439\",(double)439);	   \n\
      MakeMyFloatC(xnine	 ,\"9\",(double)9);	   \n\
      MakeMyFloatC(xone	 ,\"1\",(double)1);	   \n\
      MakeMyFloatC(xone2	 ,\"12\",(double)12);	   \n\
      MakeMyFloatC(xone28	 ,\"128\",(double)128);	   \n\
      MakeMyFloatC(xone2825	 ,\"12825\",(double)12825);   \n\
      MakeMyFloatC(xone3	 ,\"13\",(double)13);	   \n\
      MakeMyFloatC(xone35	 ,\"135\",(double)135);	   \n\
      MakeMyFloatC(xone408	 ,\"1408\",(double)1408);	   \n\
      MakeMyFloatC(xone6	 ,\"16\",(double)16);	   \n\
      MakeMyFloatC(xone859	 ,\"1859\",(double)1859);	   \n\
      MakeMyFloatC(xone932	 ,\"1932\",(double)1932);	   \n\
      MakeMyFloatC(xseven200	 ,\"7200\",(double)7200);	   \n\
      MakeMyFloatC(xseven296	 ,\"7296\",(double)7296);	   \n\
      MakeMyFloatC(xseven5240	 ,\"75240\",(double)75240);   \n\
      MakeMyFloatC(xsix656	 ,\"6656\",(double)6656);	   \n\
      MakeMyFloatC(xthree544	 ,\"3544\",(double)3544);	   \n\
      MakeMyFloatC(xthree60	 ,\"360\",(double)360);	   \n\
      MakeMyFloatC(xthree680	 ,\"3680\",(double)3680);	   \n\
      MakeMyFloatC(xtwo	 ,\"2\",(double)2);	   \n\
      MakeMyFloatC(xtwo16	 ,\"216\",(double)216);	   \n\
      MakeMyFloatC(xtwo187	 ,\"2187\",(double)2187);	   \n\
      MakeMyFloatC(xtwo197	 ,\"2197\",(double)2197);	   \n\
      MakeMyFloatC(xtwo5	 ,\"25\",(double)25);	   \n\
      MakeMyFloatC(xtwo565	 ,\"2565\",(double)2565);	   \n\
      MakeMyFloatC(xtwo7	 ,\"27\",(double)27);	   \n\
      MakeMyFloatC(xtwo8561	 ,\"28561\",(double)28561);\n\
\n\
      MakeMyFloatC(xfour	 ,\"4\",(double)4);               \n\
      MakeMyFloatC(xtwo	         ,\"2\",(double)2);               \n\
      MakeMyFloatC(xfour0	 ,\"40\",(double)40);         \n\
      MakeMyFloatC(xthree	 ,\"3\",(double)3);\n\
      MakeMyFloatC(xthree2	 ,\"32\",(double)32);\n\
      MakeMyFloatC(xone1	 ,\"11\",(double)11);         \n\
\n\
      InitMyFloat(xp25);\n\
      InitMyFloat(xp5);\n\
      InitMyFloat(xp09375);\n\
      InitMyFloat(xp28125);\n\
      InitMyFloat(xp275);\n\
      DivideMyFloatA(xp25,xone,xfour);\n\
      DivideMyFloatA(xp5,xone,xtwo);       \n\
      DivideMyFloatA(xp09375,xthree,xthree2);\n\
      DivideMyFloatA(xp28125,xnine,xthree2);\n\
      DivideMyFloatA(xp275,xone1,xfour0);                   \n\
\n\
      //#define E1 (x1.e0/360.e0)\n\
      DivideMyFloatA(xE1,xone,xthree60); \n\
\n\
      //#define E2 (x-128.e0/4275.e0)\n\
      DivideMyFloatA(rk45_tmp1,xone28,xfour275);\n\
      NegateMyFloatA(xE2,rk45_tmp1);\n\
\n\
      //#define E3 (x-2197.e0/75240.e0)  \n\
      DivideMyFloatA(rk45_tmp1,xtwo197,xseven5240);\n\
      NegateMyFloatA(xE3,rk45_tmp1);      \n\
\n\
      //#define E4 (x1.e0/50.e0)\n\
      DivideMyFloatA(xE4,xone,xfive0);\n\
\n\
      //#define E5 (x2.e0/55.e0)\n\
      DivideMyFloatA(xE5,xtwo,xfive5);\n\
\n\
      //#define RK4_1 (x25.e0/216.e0)\n\
      DivideMyFloatA(xRK4_1,xtwo5,xtwo16);\n\
      \n\
      //#define RK4_2 (x1408.e0/2565.e0)\n\
      DivideMyFloatA(xRK4_2,xone408,xtwo565);\n\
      \n\
      //#define RK4_3 (x2197.e0/4104.e0)\n\
      DivideMyFloatA(xRK4_3,xtwo197,xfour104);\n\
      \n\
      //#define RK4_4 (x-1.e0/5.e0)\n\
      DivideMyFloatA(rk45_tmp1,xone,xfive);\n\
      NegateMyFloatA(xRK4_4,rk45_tmp1);      \n\
      \n\
\n\
      //#define RK5_1 (x16.e0/135.e0)\n\
      DivideMyFloatA(xRK5_1,xone6,xone35);\n\
      \n\
      //#define RK5_2 (x6656.e0/12825.e0)\n\
      DivideMyFloatA(xRK5_2,xsix656,xone2825);\n\
      \n\
      //#define RK5_3 (x28561.e0/56430.e0)\n\
      DivideMyFloatA(xRK5_3,xtwo8561,xfive6430);\n\
      \n\
      //#define RK5_4 (x-9.e0/50.e0)\n\
      DivideMyFloatA(rk45_tmp1,xnine,xfive0);\n\
      NegateMyFloatA(xRK5_4,rk45_tmp1);      \n\
      \n\
      //#define RK5_5 (x2.e0/55.e0)\n\
      DivideMyFloatA(xRK5_5,xtwo,xfive5);      \n\
\n\
      //#define C1 (x12.e0/13.e0)\n\
      DivideMyFloatA(xC1,xone2,xone3);            \n\
\n\
      //#define C2 (x1932.e0/2197.e0)\n\
      DivideMyFloatA(xC2,xone932,xtwo197);\n\
      \n\
      //#define C3 (x-7200.e0/2197.e0)\n\
      DivideMyFloatA(rk45_tmp1,xseven200,xtwo197);\n\
      NegateMyFloatA(xC3,rk45_tmp1);            \n\
      \n\
      //#define C4 (x7296.e0/2197.e0)\n\
      DivideMyFloatA(xC4,xseven296,xtwo197);\n\
      \n\
      //#define C5 (x439.e0/216.e0)\n\
      DivideMyFloatA(xC5,xfour39,xtwo16);\n\
      \n\
      //#define C6 (x3680.e0/513.e0)\n\
      DivideMyFloatA(xC6,xthree680,xfive13);\n\
      \n\
      //#define C7 (x-845.e0/4104.e0)\n\
      DivideMyFloatA(rk45_tmp1,xeight45,xfour104);\n\
      NegateMyFloatA(xC7,rk45_tmp1);            \n\
      \n\
      //#define C8 (x-8.e0/27.e0)\n\
      DivideMyFloatA(rk45_tmp1,xeight,xtwo7);\n\
      NegateMyFloatA(xC8,rk45_tmp1);            \n\
      \n\
      //#define C9 (x-3544.e0/2565.e0)\n\
      DivideMyFloatA(rk45_tmp1,xthree544,xtwo565);\n\
      NegateMyFloatA(xC9,rk45_tmp1);            \n\
      \n\
      //#define C10 (x1859.e0/4104.e0)\n\
      DivideMyFloatA(xC10,xone859,xfour104);      \n\
\n\
      rk45_constants_initialized = 1;\n\
    }\n\
  }\n\
\n\
#else\n\
  static void rk45_init_constants() {	}		\n\
#define xE1 (1.e0/360.e0)\n\
#define xE2 (-128.e0/4275.e0)\n\
#define xE3 (-2197.e0/75240.e0)  \n\
#define xE4 (1.e0/50.e0)\n\
#define xE5 (2.e0/55.e0)\n\
#define xRK4_1 (25.e0/216.e0)\n\
#define xRK4_2 (1408.e0/2565.e0)\n\
#define xRK4_3 (2197.e0/4104.e0)\n\
#define xRK4_4 (-1.e0/5.e0)\n\
#define xRK5_1 (16.e0/135.e0)\n\
#define xRK5_2 (6656.e0/12825.e0)\n\
#define xRK5_3 (28561.e0/56430.e0)\n\
#define xRK5_4 (-9.e0/50.e0)\n\
#define xRK5_5 (2.e0/55.e0)\n\
#define xC1 (12.e0/13.e0)\n\
#define xC2 (1932.e0/2197.e0)\n\
#define xC3 (-7200.e0/2197.e0)\n\
#define xC4 (7296.e0/2197.e0)\n\
#define xC5 (439.e0/216.e0)\n\
#define xC6 (3680.e0/513.e0)\n\
#define xC7 (-845.e0/4104.e0)\n\
#define xC8 (-8.e0/27.e0)\n\
#define xC9 (-3544.e0/2565.e0)\n\
#define xC10 (1859.e0/4104.e0)\n\
#define xeight (8.) \n\
#define xtwo (2.) \n\
#define xp25 (1./4.) \n\
#define xp5  (1./5.)\n\
#define xp09375 (3./32.)\n\
#define xp28125 (9./32.)\n\
#define xp275 (11./40.)\n\
#endif\n\
\n\
");

   fprintf(outfile, "\n\
  static void compute_ks45_%s(MY_FLOAT t, MY_FLOAT *xx_in, MY_FLOAT step,  MY_FLOAT **k,\n\
			      MY_JET *jetIn, MY_JET **kjet ) {\n\
    // double ta;\n\
    static int inited;\n\
    static MY_FLOAT tmp1,tmp2,tmp3,tmp4,tmp5,ta,aux[%d]; //m\n\
\n\
#pragma omp threadprivate(inited, tmp1,tmp2,tmp3,tmp4,tmp5,ta,aux,inited)\n\
\n\
",suffix,m);
   if(state_jet_vars > 0) {
   fprintf(outfile, "\n\
    static MY_JET jtmp1,jtmp2,jtmp3,jtmp4,jtmp5,jaux[%d]; //n\n\
#pragma omp threadprivate(jtmp1,jtmp2,jtmp3,jtmp4,jtmp5,jaux)\n\
    MY_JET **jetOut;	 //n\n\
",n);
   }
   fprintf(outfile, "\n\
    MY_FLOAT **fptr;\n\
    int i, j;    \n\
\n\
    if(!inited) {\n\
      InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);InitMyFloat(tmp4);InitMyFloat(tmp5);InitMyFloat(ta); \n\
      for(i=0;i<%d;i++) InitMyFloat(aux[i]); //m\n\
      \n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
      InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);InitJet(jtmp5);\n\
      for(i=0;i<%d;i++) InitJet(jaux[i]); //n\n\
						    \n\
",n);
   }
   fprintf(outfile, "\n\
      inited = 1;\n\
    }\n\
\n\
    // (*ode)(t,x,n,k[0]);\n\
    // for (i=0; i<n; i++)\n\
    // {\n\
    //  k[0][i] *= h;\n\
    //  aux[i]=x[i]+0.25e0*k[0][i];\n\
    // }\n\
\n\
");
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, jetIn, &jetOut);      \n\
    for(i=0;i<%d;i++){\n\
	AssignJetToJet(kjet[0][i], jetOut[i][1]);\n\
	MultiplyJetFloatA(jtmp1,kjet[0][i], step);\n\
	AssignJetToJet(kjet[0][i], jtmp1);\n\
    	MultiplyJetFloatA(jtmp1,kjet[0][i], xp25);\n\
	AddJetJetA(jaux[i], jetIn[i], jtmp1);\n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, NULL, NULL);      \n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[0][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[0][i], step);\n\
      AssignMyFloat(k[0][i],tmp1);\n\
      MultiplyMyFloatA(tmp1,k[0][i], xp25);\n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    // (*ode)(t+0.25e0*h,aux,n,k[1]);\n\
    // for (i=0; i<n; i++)\n\
    // {\n\
    // k[1][i] *= h;\n\
    // aux[i]=x[i]+0.09375e0*k[0][i]+0.28125e0*k[1][i];\n\
    // }    \n\
\n\
    MultiplyMyFloatA(tmp1, xp25, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[1][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[1][i], step);\n\
      AssignJetToJet(kjet[1][i], jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], xp09375);\n\
      MultiplyJetFloatA(jtmp2,kjet[1][i], xp28125);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp1);      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp3);      \n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
\n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[1][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[1][i], step);\n\
      AssignMyFloat(k[1][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], xp09375);\n\
      MultiplyMyFloatA(tmp2,k[1][i], xp28125);\n\
      AddMyFloatA(tmp3, tmp2, tmp1);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp3);            \n\
    }\n\
\n\
    // (*ode)(t+0.375e0*h,aux,n,k[2]);\n\
    // for (i=0; i<n; i++)\n\
    // {\n\
    // k[2][i] *= h;\n\
    // aux[i]=x[i]+C2*k[0][i]+C3*k[1][i]+C4*k[2][i];\n\
    // }\n\
\n\
    MultiplyMyFloatA(tmp1, xp275, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[2][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[2][i], step);\n\
      AssignJetToJet(kjet[2][i], jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], xC2);\n\
      MultiplyJetFloatA(jtmp2,kjet[1][i], xC3);\n\
      MultiplyJetFloatA(jtmp3,kjet[2][i], xC4);      \n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp4); //1           \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp1);      \n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
\n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n			\
      AssignMyFloat(k[2][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[2][i], step);\n\
      AssignMyFloat(k[2][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], xC2);\n\
      MultiplyMyFloatA(tmp2,k[1][i], xC3);\n\
      MultiplyMyFloatA(tmp3,k[2][i], xC4);      \n\
      AddMyFloatA(tmp4, tmp2, tmp1);\n\
      AddMyFloatA(tmp1, tmp3, tmp4); //1           \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    //(*ode)(t+C1*h,aux,n,k[3]);\n\
    //for (i=0; i<n; i++)\n\
    //{\n\
    // k[3][i] *= h;\n\
    // aux[i]=x[i]+C5*k[0][i]-8.e0*k[1][i]+C6*k[2][i]+C7*k[3][i];\n\
    // }\n\
\n\
    MultiplyMyFloatA(tmp1, xC1, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[3][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[3][i], step);\n\
      AssignJetToJet(kjet[3][i], jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], xC5);\n\
      MultiplyJetFloatA(jtmp2,kjet[1][i], xeight);\n\
      MultiplyJetFloatA(jtmp3,kjet[2][i], xC6);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      SubtractJetJetA(jtmp3, jtmp4, jtmp2); //3\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], xC7);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp2);      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp1);      \n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
\n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[3][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[3][i], step);\n\
      AssignMyFloat(k[3][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], xC5);\n\
      MultiplyMyFloatA(tmp2,k[1][i], xeight);\n\
      MultiplyMyFloatA(tmp3,k[2][i], xC6);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      SubtractMyFloatA(tmp3, tmp4, tmp2); //3\n\
      MultiplyMyFloatA(tmp2,k[3][i], xC7);\n\
      AddMyFloatA(tmp1, tmp3, tmp2);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    //  (*ode)(t+h,aux,n,k[4]);\n\
    //  for (i=0; i<n; i++)\n\
    //  {\n\
    //    k[4][i] *= h;\n\
    //    aux[i]=x[i]+C8*k[0][i]+2*k[1][i]+C9*k[2][i]+C10*k[3][i]-0.275e0*k[4][i];\n\
    //  }\n\
\n\
    AddMyFloatA(ta, t, step);\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[4][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[4][i], step);\n\
      AssignJetToJet(kjet[4][i], jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], xC8);\n\
      MultiplyJetFloatA(jtmp2,kjet[1][i], xtwo);\n\
      MultiplyJetFloatA(jtmp3,kjet[2][i], xC9);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp4, jtmp2); //3\n\
      MultiplyJetFloatA(jtmp1,kjet[3][i], xC10);\n\
      MultiplyJetFloatA(jtmp2,kjet[4][i], xp275);\n\
      SubtractJetJetA(jtmp4,jtmp1,jtmp2); //4\n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp1);      \n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
\n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[4][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[4][i], step);\n\
      AssignMyFloat(k[4][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], xC8);\n\
      MultiplyMyFloatA(tmp2,k[1][i], xtwo);\n\
      MultiplyMyFloatA(tmp3,k[2][i], xC9);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp4, tmp2); //3\n\
      MultiplyMyFloatA(tmp1,k[3][i], xC10);\n\
      MultiplyMyFloatA(tmp2,k[4][i], xp275);\n\
      SubtractMyFloatA(tmp4,tmp1,tmp2); //4\n\
      AddMyFloatA(tmp1, tmp3, tmp4);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
    // (*ode)(t+0.5e0*h,aux,n,k[5]);\n\
    // for (i=0; i<n; i++) k[5][i] *= h;\n\
    MultiplyMyFloatA(tmp1, xp5, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[5][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], step);\n\
      AssignJetToJet(kjet[5][i], jtmp1);\n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
\n\
", suffix);
   }
	     
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[5][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[5][i], step);\n\
      AssignMyFloat(k[5][i], tmp1);\n\
    }\n\
  }\n\
\n\
\n\
",m);

   fprintf(outfile, "\n\
  void step_rk4_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n	\n\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4,initialized)\n\
\n\
",suffix);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  static MY_JET jtmp1, jtmp2, jtmp3, jtmp4;\n\
#pragma omp threadprivate(jtmp1, jtmp2, jtmp3, jtmp4)\n\
\n\
");
   }
   fprintf(outfile, "\n\
  int j;\n\
\n\
  if(initialized == 0) {\n\
    InitMyFloat(nr); InitMyFloat(jnr); InitMyFloat(NN);\n\
    InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
    MakeMyFloatC(NN, \"%d\", (double)%d); //m    \n\
\n\
",m,m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);\n\
");
   }
 fprintf(outfile, "\n\
    initialized=1;\n\
  }\n\
\n\
  \n\
");
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  MakeMyFloatA(jnr,(double)0);\n\
");
   }
   fprintf(outfile, "\n\
\n\
  MakeMyFloatA(nr,(double)0);\n\
  \n\
  // x[i] += RK4_1*k[0][i]+RK4_2*k[2][i]+RK4_3*k[3][i]+RK4_4*k[4][i];\n\
  // nr += x[i]*x[i];  \n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], xRK4_1);\n\
      MultiplyMyFloatA(tmp2, k[2][j], xRK4_2);\n\
      MultiplyMyFloatA(tmp3, k[3][j], xRK4_3);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp2, tmp3, tmp4); //2\n\
      MultiplyMyFloatA(tmp4, k[4][j], xRK4_4);\n\
      AddMyFloatA(tmp1, tmp2, tmp4); //1 \n\
      AddMyFloatA(tmp2, tmp1, xx_in[j]);\n\
      AssignMyFloat(xx_in[j], tmp2);\n\
\n\
      MultiplyMyFloatA(tmp3, tmp2, tmp2);\n\
      AddMyFloatA(tmp1, nr, tmp3);\n\
      AssignMyFloat(nr, tmp1);      \n\
  }\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  if(kjet)\n\
    for(j = 0; j < %d; ++j) { //n\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], xRK4_1);\n\
      MultiplyJetFloatA(jtmp2, kjet[2][j], xRK4_2);\n\
      MultiplyJetFloatA(jtmp3, kjet[3][j], xRK4_3);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp4, kjet[4][j], xRK4_4);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp4);\n\
      AddJetJetA(jtmp2, jtmp1, jetIn[j]);\n\
      AssignJetToJet(jetIn[j], jtmp2);\n\
\n\
      xNormJet(tmp1,jetIn[j]);\n\
      AddMyFloatA(jnr, tmp1, jnr);\n\
    }\n\
  \n\
\n\
  if(MyFloatA_GE_B(jnr,nr)) { AssignMyFloat(nr, jnr);}\n\
\n\
",n);
   }
   fprintf(outfile, "\n\
  DivideMyFloatA(tmp1, nr, NN);\n\
  sqrtMyFloatA( *nr_ret, tmp1);\n\
}\n\
\n\
\n\
");
   fprintf(outfile, "\n\
  void step_rk5_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n	\n\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4,initialized)\n\
\n\
",suffix);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  static MY_JET jtmp1, jtmp2, jtmp3, jtmp4;\n\
#pragma omp threadprivate(jtmp1, jtmp2, jtmp3, jtmp4)\n\
\n\
");
   }
   fprintf(outfile, "\n\
  int j;\n\
\n\
  if(initialized == 0) {\n\
    InitMyFloat(nr); InitMyFloat(jnr); InitMyFloat(NN);\n\
    InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
    MakeMyFloatC(NN, \"%d\", (double)%d); //m    \n\
\n\
",m,m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
    InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);\n\
");
   }
   fprintf(outfile, "\n\
    initialized=1;\n\
  }\n\
\n\
  \n\
");
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  MakeMyFloatA(jnr,(double)0);\n\
\n\
");
   }
   fprintf(outfile, "\n\
  MakeMyFloatA(nr,(double)0);\n\
\n\
  //    x[i] += RK5_1*k[0][i]+RK5_2*k[2][i]+RK5_3*k[3][i]+RK5_4*k[4][i]+RK5_5*k[5][i];\n\
  //    nr += x[i]*x[i];  \n\
\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], xRK5_1);\n\
      MultiplyMyFloatA(tmp2, k[2][j], xRK5_2);\n\
      MultiplyMyFloatA(tmp3, k[3][j], xRK5_3);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp2, tmp3, tmp4); //2\n\
      MultiplyMyFloatA(tmp1, k[4][j], xRK5_4);\n\
      MultiplyMyFloatA(tmp4, k[5][j], xRK5_5);      \n\
      AddMyFloatA(tmp3, tmp1, tmp4); \n\
      AddMyFloatA(tmp1, tmp2, tmp3);  //1\n\
      AddMyFloatA(tmp2, tmp1, xx_in[j]);\n\
      AssignMyFloat(xx_in[j], tmp2);\n\
\n\
      MultiplyMyFloatA(tmp3, tmp2, tmp2);\n\
      AddMyFloatA(tmp1, nr, tmp3);\n\
      AssignMyFloat(nr, tmp1);      \n\
  }\n\
\n\
",m);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  if(kjet)\n\
    for(j = 0; j < %d; ++j) { //n\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], xRK5_1);\n\
      MultiplyJetFloatA(jtmp2, kjet[2][j], xRK5_2);\n\
      MultiplyJetFloatA(jtmp3, kjet[3][j], xRK5_3);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp1, kjet[4][j], xRK5_4);\n\
      MultiplyJetFloatA(jtmp4, kjet[5][j], xRK5_5);      \n\
      AddJetJetA(jtmp3, jtmp1, jtmp4);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);      \n\
      AddJetJetA(jtmp2, jtmp1, jetIn[j]);\n\
      AssignJetToJet(jetIn[j], jtmp2);\n\
\n\
      xNormJet(tmp1,jetIn[j]);\n\
      AddMyFloatA(jnr, tmp1, jnr);\n\
    }\n\
  \n\
\n\
  if(MyFloatA_GE_B(jnr,nr)) { AssignMyFloat(nr, jnr);}\n\
\n\
",n);
   }
   fprintf(outfile, "\n\
  DivideMyFloatA(tmp1, nr, NN);\n\
  sqrtMyFloatA( *nr_ret, tmp1);\n\
}\n\
\n\
\n\
");
   
   fprintf(outfile, "\n\
 static void error_rk45_%s(MY_FLOAT **k, MY_JET **kjet, MY_FLOAT *er_ret) {\n\
   if(er_ret == NULL) return;\n\
   static MY_FLOAT s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero;\n\
   static int inited;\n\
#pragma omp threadprivate(inited,s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero,inited)\n\
   int j;\n\
\n\
",suffix);
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
   static MY_JET js, jtmp1,jtmp2,jtmp3,jtmp4;\n\
#pragma omp threadprivate(js, jtmp1, jtmp2, jtmp3, jtmp4)\n\
\n\
");
   }
   fprintf(outfile, "\n\
   if(!inited) {\n\
     InitMyFloat(s); InitMyFloat(je);  InitMyFloat(e); InitMyFloat(NN); InitMyFloat(zero);\n \n\
     InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
\n\
");
   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
     InitJet(js); InitJet(jtmp1); InitJet(jtmp2); InitJet(jtmp3); InitJet(jtmp4);\n\
  \n\
");
   }
   fprintf(outfile, "\n\
     MakeMyFloatC(zero, \"0\", (double)0);\n\
     MakeMyFloatC(NN, \"%d\", (double)%d); //m\n\
     inited = 1;\n\
   }\n\
\n\
   AssignMyFloat(e, zero);\n\
\n\
",m,m);
     if(state_jet_vars > 0) {
       fprintf(outfile, "\n\
   AssignMyFloat(je, zero);\n\
\n\
");
     }
     fprintf(outfile, "\n\
   //    s=E1*k[0][i]+E2*k[2][i]+E3*k[3][i]+E4*k[4][i]+E5*k[5][i];\n\
   //    e += s*s;\n\
   for(j = _J_DIM_; j < %d; ++j) { //m\n\
\n\
     MultiplyMyFloatA(tmp1, k[0][j], xE1);\n\
     MultiplyMyFloatA(tmp2, k[2][j], xE2);\n\
     MultiplyMyFloatA(tmp3, k[3][j], xE3);\n\
     AddMyFloatA(tmp4, tmp1, tmp2);\n\
     AddMyFloatA(s, tmp3, tmp4);     \n\
\n\
     MultiplyMyFloatA(tmp1, k[4][j], xE4);\n\
     MultiplyMyFloatA(tmp2, k[5][j], xE5);\n\
     AddMyFloatA(tmp3, tmp1, tmp2);\n\
\n\
     AddMyFloatA(tmp2, tmp3, s);\n\
     AssignMyFloat(s, tmp2);\n\
\n\
     MultiplyMyFloatA(tmp1, s, s);     \n\
\n\
     AddMyFloatA(e, e, tmp1);     \n\
   }\n\
",m);

     if(state_jet_vars > 0) {
       fprintf(outfile, "\n\
 if(kjet)  \n\
   for(j = 0; j < %d; ++j) { //n\n\
\n\
     MultiplyJetFloatA(jtmp1, kjet[0][j], xE1);\n\
     MultiplyJetFloatA(jtmp2, kjet[2][j], xE2);\n\
     MultiplyJetFloatA(jtmp3, kjet[3][j], xE3);\n\
     AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
     AddJetJetA(js, jtmp3, jtmp4);     \n\
\n\
     MultiplyJetFloatA(jtmp1, kjet[4][j], xE4);\n\
     MultiplyJetFloatA(jtmp2, kjet[5][j], xE5);\n\
     AddJetJetA(jtmp3, jtmp1, jtmp2);\n\
     AddJetJetA(jtmp2, jtmp3, js);\n\
     AssignJetToJet(js, jtmp2);\n\
\n\
     xNormJet(tmp2, js);     \n\
     MultiplyMyFloatA(tmp1, tmp2, tmp2);\n\
\n\
     AddMyFloatA(je, je, tmp1);     \n\
   }\n\
\n\
     AddMyFloatA(e, je, e);     \n\
\n\
",n);
     }
     fprintf(outfile, "\n\
   DivideMyFloatA(tmp1, e, NN);\n\
\n\
   sqrtMyFloatA( *er_ret, tmp1);\n\
\n\
}\n\
\n\
");
}
