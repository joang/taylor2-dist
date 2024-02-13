/*********************************************************************
 *
 *       Taylor  
 *
 *    Copyright (C) 1999  Maorong Zou, Angel Jorba
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

#define RK67_C
#include "Header.h"

typedef double MY_FLOAT;
typedef double MY_JET;
void genGeneralRK67Code() {
  int m = neqns - nonautonomous, n= state_jet_vars;
  fprintf(outfile, "\n\
#ifndef MY_FLOAT_IS_DOUBLE\n\
  static int rk67_constants_initialized;\n\
  \n\
static MY_FLOAT zeight,zeight64,zfive12,zfive1237,zfive4,zfive445,zfive632,zfive643,zfour,zfour3,zfour617,zfour6592,zfour692,zfour802,zfour86,znine152,zone,zone0077,zone029,zone05,zone053,zone067,zone1,zone16259,zone215,zone408,zone41,zone440,zone5,zone65,zone6807,zone701,zone76,zone771561,zone8,zone8304,zone833,zseven,zseven203,zseven29,zseven33,zseven4880,zseven7,zsix,zsix289920,zsix4,zsix240,zsix86,zthree,zthree2,zthree3,zthree35763,zthree52,zthree584,ztwo,ztwo16,ztwo176,ztwo2,ztwo3296,ztwo383,ztwo401,ztwo43,ztwo560,ztwo6312,ztwo70,ztwo816,rk67_tmp1;\n\
  static MY_FLOAT zA1,zA2,zA3,zA4,zA5,zA6,zB10,zB21,zB30,zB32,zB40,zB42,zB43,zB50,zB52,zB53,zB54,zB60,zB62,zB63,zB64,zB65,zB70,zB72,zB73,zB74,zB75,zB76,zB80,zB83,zB84,zB85,zB86,zB90,zB92,zB93,zB94,zB95,zB96,zC60,zC63,zC64,zC65,zC66,zC67,zC70;\n\
\n\
  static void rk67_init_constants() {\n\
    if(! rk67_constants_initialized) {\n\
      InitMyFloat(rk67_tmp1);\n\
      InitMyFloat(zA1);\n\
      InitMyFloat(zA2);\n\
      InitMyFloat(zA3);\n\
      InitMyFloat(zA4);\n\
      InitMyFloat(zA5);\n\
      InitMyFloat(zA6);\n\
      InitMyFloat(zB10);\n\
      InitMyFloat(zB21);\n\
      InitMyFloat(zB30);\n\
      InitMyFloat(zB32);\n\
      InitMyFloat(zB40);\n\
      InitMyFloat(zB42);\n\
      InitMyFloat(zB43);\n\
      InitMyFloat(zB50);\n\
      InitMyFloat(zB52);\n\
      InitMyFloat(zB53);\n\
      InitMyFloat(zB54);\n\
      InitMyFloat(zB60);\n\
      InitMyFloat(zB62);\n\
      InitMyFloat(zB63);\n\
      InitMyFloat(zB64);\n\
      InitMyFloat(zB65);\n\
      InitMyFloat(zB70);\n\
      InitMyFloat(zB72);\n\
      InitMyFloat(zB73);\n\
      InitMyFloat(zB74);\n\
      InitMyFloat(zB75);\n\
      InitMyFloat(zB76);\n\
      InitMyFloat(zB80);\n\
      InitMyFloat(zB83);\n\
      InitMyFloat(zB84);\n\
      InitMyFloat(zB85);\n\
      InitMyFloat(zB86);\n\
      InitMyFloat(zB90);\n\
      InitMyFloat(zB92);\n\
      InitMyFloat(zB93);\n\
      InitMyFloat(zB94);\n\
      InitMyFloat(zB95);\n\
      InitMyFloat(zB96);\n\
      InitMyFloat(zC60);\n\
      InitMyFloat(zC63);\n\
      InitMyFloat(zC64);\n\
      InitMyFloat(zC65);\n\
      InitMyFloat(zC66);\n\
      InitMyFloat(zC67);\n\
      InitMyFloat(zC70);\n\
      InitMyFloat(zeight);\n\
      InitMyFloat(zeight64);\n\
      InitMyFloat(zfive12);\n\
      InitMyFloat(zfive1237);\n\
      InitMyFloat(zfive4);\n\
      InitMyFloat(zfive445);\n\
      InitMyFloat(zfive632);\n\
      InitMyFloat(zfive643);\n\
      InitMyFloat(zfour);\n\
      InitMyFloat(zfour3);\n\
      InitMyFloat(zfour617);\n\
      InitMyFloat(zfour6592);      \n\
      InitMyFloat(zfour692);\n\
      InitMyFloat(zfour802);\n\
      InitMyFloat(zfour86);\n\
      InitMyFloat(znine152);\n\
      InitMyFloat(zone);\n\
      InitMyFloat(zone0077);\n\
      InitMyFloat(zone029);\n\
      InitMyFloat(zone05);\n\
      InitMyFloat(zone053);\n\
      InitMyFloat(zone067);\n\
      InitMyFloat(zone1);\n\
      InitMyFloat(zone16259);\n\
      InitMyFloat(zone215);\n\
      InitMyFloat(zone408);\n\
      InitMyFloat(zone41);\n\
      InitMyFloat(zone440);\n\
      InitMyFloat(zone5);\n\
      InitMyFloat(zone65);\n\
      InitMyFloat(zone6807);\n\
      InitMyFloat(zone701);\n\
      InitMyFloat(zone76);\n\
      InitMyFloat(zone771561);\n\
      InitMyFloat(zone8);\n\
      InitMyFloat(zone8304);\n\
      InitMyFloat(zone833);\n\
      InitMyFloat(zseven);\n\
      InitMyFloat(zseven203);\n\
      InitMyFloat(zseven29);\n\
      InitMyFloat(zseven33);\n\
      InitMyFloat(zseven4880);\n\
      InitMyFloat(zseven7);\n\
      InitMyFloat(zsix);\n\
      InitMyFloat(zsix289920);\n\
      InitMyFloat(zsix4);\n\
      InitMyFloat(zsix240);      \n\
      InitMyFloat(zsix86);\n\
      InitMyFloat(zthree);      \n\
      InitMyFloat(zthree2);\n\
      InitMyFloat(zthree3);\n\
      InitMyFloat(zthree35763);\n\
      InitMyFloat(zthree52);\n\
      InitMyFloat(zthree584);\n\
      InitMyFloat(ztwo);\n\
      InitMyFloat(ztwo16);\n\
      InitMyFloat(ztwo176);\n\
      InitMyFloat(ztwo2);\n\
      InitMyFloat(ztwo3296);\n\
      InitMyFloat(ztwo383);\n\
      InitMyFloat(ztwo401);\n\
      InitMyFloat(ztwo43);\n\
      InitMyFloat(ztwo560);\n\
      InitMyFloat(ztwo6312);\n\
      InitMyFloat(ztwo70);\n\
      InitMyFloat(ztwo816);\n\
      MakeMyFloatC(zeight             ,\"8\",(double)8);	       \n\
      MakeMyFloatC(zeight64           ,\"864\",(double)864);	       \n\
      MakeMyFloatC(zfive12      	   ,\"512\",(double)512);	       \n\
      MakeMyFloatC(zfive1237    	   ,\"51237\",(double)51237);       \n\
      MakeMyFloatC(zfive4		   ,\"54\",(double)54);	       \n\
      MakeMyFloatC(zfive445	   ,\"5445\",(double)5445);	       \n\
      MakeMyFloatC(zfive632	   ,\"5632\",(double)5632);	       \n\
      MakeMyFloatC(zfive643	   ,\"5643\",(double)5643);	       \n\
      MakeMyFloatC(zfour		   ,\"4\",(double)4);	       \n\
      MakeMyFloatC(zfour3		   ,\"43\",(double)43);	       \n\
      MakeMyFloatC(zfour617	   ,\"4617\",(double)4617);\n\
      MakeMyFloatC(zfour6592	   ,\"46592\",(double)46592);      \n\
      MakeMyFloatC(zfour692	   ,\"4692\",(double)4692);\n\
      MakeMyFloatC(zfour802	   ,\"4802\",(double)4802);	       \n\
      MakeMyFloatC(zfour86		   ,\"486\",(double)486);	       \n\
      MakeMyFloatC(znine152	   ,\"9152\",(double)9152);	       \n\
      MakeMyFloatC(zone		   ,\"1\",(double)1);	       \n\
      MakeMyFloatC(zone0077	   ,\"10077\",(double)10077);       \n\
      MakeMyFloatC(zone029		   ,\"1029\",(double)1029);	       \n\
      MakeMyFloatC(zone05		   ,\"105\",(double)105);	       \n\
      MakeMyFloatC(zone053		   ,\"1053\",(double)1053);	       \n\
      MakeMyFloatC(zone067		   ,\"1067\",(double)1067);	       \n\
      MakeMyFloatC(zone1		   ,\"11\",(double)11);	       \n\
      MakeMyFloatC(zone16259	   ,\"116259\",(double)116259);     \n\
      MakeMyFloatC(zone215		   ,\"1215\",(double)1215);	       \n\
      MakeMyFloatC(zone408		   ,\"1408\",(double)1408);	       \n\
      MakeMyFloatC(zone41		   ,\"141\",(double)141);	       \n\
      MakeMyFloatC(zone440		   ,\"1440\",(double)1440);	       \n\
      MakeMyFloatC(zone5		   ,\"15\",(double)15);	       \n\
      MakeMyFloatC(zone65		   ,\"165\",(double)165);	       \n\
      MakeMyFloatC(zone6807	   ,\"16807\",(double)16807);       \n\
      MakeMyFloatC(zone701		   ,\"1701\",(double)1701);	       \n\
      MakeMyFloatC(zone76		   ,\"176\",(double)176);	       \n\
      MakeMyFloatC(zone771561	   ,\"1771561\",(double)1771561);   \n\
      MakeMyFloatC(zone8		   ,\"18\",(double)18);	       \n\
      MakeMyFloatC(zone8304	   ,\"18304\",(double)18304);       \n\
      MakeMyFloatC(zone833		   ,\"1833\",(double)1833);	       \n\
      MakeMyFloatC(zseven		   ,\"7\",(double)7);	       \n\
      MakeMyFloatC(zseven203	   ,\"7203\",(double)7203);	       \n\
      MakeMyFloatC(zseven29	   ,\"729\",(double)729);	       \n\
      MakeMyFloatC(zseven33	   ,\"733\",(double)733);	       \n\
      MakeMyFloatC(zseven4880	   ,\"74880\",(double)74880);       \n\
      MakeMyFloatC(zseven7		   ,\"77\",(double)77);	       \n\
      MakeMyFloatC(zsix		   ,\"6\",(double)6);	       \n\
      MakeMyFloatC(zsix289920	   ,\"6289920\",(double)6289920);   \n\
      MakeMyFloatC(zsix4		   ,\"64\",(double)64);\n\
      MakeMyFloatC(zsix240		   ,\"6240\",(double)6240);      \n\
      MakeMyFloatC(zsix86		   ,\"686\",(double)686);\n\
      MakeMyFloatC(zthree		   ,\"3\",(double)3);	             \n\
      MakeMyFloatC(zthree2		   ,\"32\",(double)32);	       \n\
      MakeMyFloatC(zthree3		   ,\"33\",(double)33);	       \n\
      MakeMyFloatC(zthree35763	   ,\"335763\",(double)335763);     \n\
      MakeMyFloatC(zthree52	   ,\"352\",(double)352);	       \n\
      MakeMyFloatC(zthree584	   ,\"3584\",(double)3584);	       \n\
      MakeMyFloatC(ztwo		   ,\"2\",(double)2);	       \n\
      MakeMyFloatC(ztwo16		   ,\"216\",(double)216);	       \n\
      MakeMyFloatC(ztwo176		   ,\"2176\",(double)2176);	       \n\
      MakeMyFloatC(ztwo2		   ,\"22\",(double)22);	       \n\
      MakeMyFloatC(ztwo3296	   ,\"23296\",(double)23296);       \n\
      MakeMyFloatC(ztwo383		   ,\"2383\",(double)2383);	       \n\
      MakeMyFloatC(ztwo401		   ,\"2401\",(double)2401);	       \n\
      MakeMyFloatC(ztwo43		   ,\"243\",(double)243);	       \n\
      MakeMyFloatC(ztwo560		   ,\"2560\",(double)2560);	       \n\
      MakeMyFloatC(ztwo6312	   ,\"26312\",(double)26312);       \n\
      MakeMyFloatC(ztwo70		   ,\"270\",(double)270);	       \n\
      MakeMyFloatC(ztwo816            ,\"2816\",(double)2816);\n\
\n\
      //#define A1 2.e0/33.e0\n\
      DivideMyFloatA(zA1,ztwo,zthree3);\n\
      \n\
      //#define A2 4.e0/33.e0\n\
      DivideMyFloatA(zA2,zfour,zthree3);\n\
      \n\
      // #define A3 2.e0/11.e0\n\
      DivideMyFloatA(zA3,ztwo,zone1);\n\
      \n\
      //#define A4 1.e0/2.e0\n\
      DivideMyFloatA(zA4,zone,ztwo);\n\
      \n\
      //#define A5 2.e0/3.e0\n\
      DivideMyFloatA(zA5,ztwo,zthree);\n\
      \n\
      //#define A6 6.e0/7.e0\n\
      DivideMyFloatA(zA6,zsix,zseven);      \n\
\n\
      //#define B10 2.e0/33.e0\n\
      DivideMyFloatA(zB10,ztwo,zthree3);      \n\
\n\
      //#define B21 4.e0/33.e0\n\
      DivideMyFloatA(zB21,zfour,zthree3);            \n\
\n\
      //#define B30 1.e0/22.e0\n\
      DivideMyFloatA(zB30,zone,ztwo2);\n\
      \n\
      //#define B32 3.e0/22.e0\n\
      DivideMyFloatA(zB32,zthree,ztwo2);\n\
      \n\
      //#define B40 43.e0/64.e0\n\
      DivideMyFloatA(zB40,zfour3,zsix4);            \n\
\n\
      //#define B42 -165.0/64.0\n\
      DivideMyFloatA(rk67_tmp1,zone65,zsix4);\n\
      NegateMyFloatA(zB42,rk67_tmp1);\n\
      \n\
      //#define B43 77.e0/32.e0\n\
      DivideMyFloatA(zB43,zseven7,zthree2);\n\
      \n\
      //#define B50 -2383.e0/486.e0\n\
      DivideMyFloatA(rk67_tmp1,ztwo383,zfour86);\n\
      NegateMyFloatA(zB50,rk67_tmp1);\n\
      \n\
      //#define B52 1067.e0/54.e0\n\
      DivideMyFloatA(zB52,zone067,zfive4);\n\
      \n\
      //#define B53 -26312.e0/1701.e0\n\
      DivideMyFloatA(rk67_tmp1,ztwo6312,zone701);\n\
      NegateMyFloatA(zB53,rk67_tmp1);\n\
\n\
      //#define B54 2176.e0/1701.e0\n\
      DivideMyFloatA(zB54,ztwo176,zone701);      \n\
\n\
      //#define B60 10077.e0/4802.e0\n\
      DivideMyFloatA(zB60,zone0077,zfour802);\n\
      \n\
      //#define B62 -5643.e0/686.e0\n\
      DivideMyFloatA(rk67_tmp1,zfive643,zsix86);\n\
      NegateMyFloatA(zB62,rk67_tmp1);\n\
\n\
      //#define B63 116259.e0/16807.e0\n\
      DivideMyFloatA(zB63,zone16259,zone6807);      \n\
\n\
      //#define B64 -6240.e0/16807.e0\n\
      DivideMyFloatA(rk67_tmp1,zsix240,zone6807);\n\
      NegateMyFloatA(zB64,rk67_tmp1);\n\
      \n\
      //#define B65 1053.e0/2401.e0\n\
      DivideMyFloatA(zB65,zone053,ztwo401);\n\
      \n\
      //#define B70 -733.e0/176.e0\n\
      DivideMyFloatA(rk67_tmp1,zseven33,zone76);\n\
      NegateMyFloatA(zB70,rk67_tmp1);\n\
      \n\
      //#define B72 141.e0/8.e0\n\
      DivideMyFloatA(zB72,zone41,zeight);\n\
      \n\
      //#define B73 -335763.e0/23296.e0\n\
      DivideMyFloatA(rk67_tmp1,zthree35763,ztwo3296);\n\
      NegateMyFloatA(zB73,rk67_tmp1);\n\
      \n\
      //#define B74 216.e0/77.e0\n\
      DivideMyFloatA(zB74,ztwo16,zseven7);\n\
      \n\
      //#define B75 -4617.e0/2816.e0\n\
      DivideMyFloatA(rk67_tmp1,zfour617,ztwo816);\n\
      NegateMyFloatA(zB75,rk67_tmp1);\n\
      \n\
      //#define B76 7203.e0/9152.e0\n\
      DivideMyFloatA(zB76,zseven203,znine152);\n\
      \n\
      //#define B80 15.e0/352.e0\n\
      DivideMyFloatA(zB80,zone5,zthree52);\n\
      \n\
      //#define B83 -5445.e0/46592.e0\n\
      DivideMyFloatA(rk67_tmp1,zfive445,zfour6592);\n\
      NegateMyFloatA(zB83,rk67_tmp1);\n\
      \n\
      //#define B84 18.e0/77.e0\n\
      DivideMyFloatA(zB84,zone8,zseven7);\n\
      \n\
      //#define B85 -1215.e0/5632.e0\n\
      DivideMyFloatA(rk67_tmp1,zone215,zfive632);\n\
      NegateMyFloatA(zB85,rk67_tmp1);\n\
      \n\
      //#define B86 1029.e0/18304.e0\n\
      DivideMyFloatA(zB86,zone029,zone8304);\n\
      \n\
      //#define B90 -1833./352.e0\n\
      DivideMyFloatA(rk67_tmp1,zone833,zthree52);\n\
      NegateMyFloatA(zB90,rk67_tmp1);\n\
      \n\
      //#define B92 141.e0/8.e0\n\
      DivideMyFloatA(zB92,zone41,zeight);\n\
      \n\
      //#define B93 -51237.e0/3584.e0\n\
      DivideMyFloatA(rk67_tmp1,zfive1237,zthree584);\n\
      NegateMyFloatA(zB93,rk67_tmp1);\n\
      \n\
      //#define B94 18.e0/7.e0\n\
      DivideMyFloatA(zB94,zone8,zseven);\n\
      \n\
      //#define B95 -729.e0/512.e0\n\
      DivideMyFloatA(rk67_tmp1,zseven29,zfive12);\n\
      NegateMyFloatA(zB95,rk67_tmp1);\n\
      \n\
      //#define B96 1029.e0/1408.e0\n\
      DivideMyFloatA(zB96,zone029,zone408);      \n\
\n\
      //#define C60 77.e0/1440.e0\n\
      DivideMyFloatA(zC60,zseven7,zone440);\n\
      \n\
      //#define C63 1771561.e0/6289920.e0\n\
      DivideMyFloatA(zC63,zone771561,zsix289920);\n\
      \n\
      //#define C64 32.e0/105.e0\n\
      DivideMyFloatA(zC64,zthree2,zone05);\n\
      \n\
      //#define C65 243.e0/2560.e0\n\
      DivideMyFloatA(zC65,ztwo43,ztwo560);\n\
      \n\
      //#define C66 16807.e0/74880.e0\n\
      DivideMyFloatA(zC66,zone6807,zseven4880);      \n\
\n\
      //#define C67 11.e0/270.e0\n\
      DivideMyFloatA(zC67,zone1,ztwo70);      \n\
\n\
      //#define C70 11.e0/864.e0\n\
      DivideMyFloatA(zC70,zone1,zeight64);            \n\
      \n\
      rk67_constants_initialized = 1;\n\
    }\n\
  }\n\
\n\
#else\n\
  static void rk67_init_constants() {	}		\n\
#define zA1 (2.e0/33.e0)\n\
#define zA2 (4.e0/33.e0)\n\
#define zA3 (2.e0/11.e0)\n\
#define zA4 (1.e0/2.e0)\n\
#define zA5 (2.e0/3.e0)\n\
#define zA6 (6.e0/7.e0)\n\
#define zB10 (2.e0/33.e0)\n\
#define zB21 (4.e0/33.e0)\n\
#define zB30 (1.e0/22.e0)\n\
#define zB32 (3.e0/22.e0)\n\
#define zB40 (43.e0/64.e0)\n\
#define zB42 (-165.0/64.0)\n\
#define zB43 (77.e0/32.e0)\n\
#define zB50 (-2383.e0/486.e0)\n\
#define zB52 (1067.e0/54.e0)\n\
#define zB53 (-26312.e0/1701.e0)\n\
#define zB54 (2176.e0/1701.e0)\n\
#define zB60 (10077.e0/4802.e0)\n\
#define zB62 (-5643.e0/686.e0)\n\
#define zB63 (116259.e0/16807.e0)\n\
#define zB64 (-6240.e0/16807.e0)\n\
#define zB65 (1053.e0/2401.e0)\n\
#define zB70 (-733.e0/176.e0)\n\
#define zB72 (141.e0/8.e0)\n\
#define zB73 (-335763.e0/23296.e0)\n\
#define zB74 (216.e0/77.e0)\n\
#define zB75 (-4617.e0/2816.e0)\n\
#define zB76 (7203.e0/9152.e0)\n\
#define zB80 (15.e0/352.e0)\n\
#define zB83 (-5445.e0/46592.e0)\n\
#define zB84 (18.e0/77.e0)\n\
#define zB85 (-1215.e0/5632.e0)\n\
#define zB86 (1029.e0/18304.e0)\n\
#define zB90 (-1833./352.e0)\n\
#define zB92 (141.e0/8.e0)\n\
#define zB93 (-51237.e0/3584.e0)\n\
#define zB94 (18.e0/7.e0)\n\
#define zB95 (-729.e0/512.e0)\n\
#define zB96 (1029.e0/1408.e0)\n\
#define zC60 (77.e0/1440.e0)\n\
#define zC63 (1771561.e0/6289920.e0)\n\
#define zC64 (32.e0/105.e0)\n\
#define zC65 (243.e0/2560.e0)\n\
#define zC66 (16807.e0/74880.e0)\n\
#define zC67 (11.e0/270.e0)\n\
#define zC70 (11.e0/864.e0)\n\
#endif\n\
\n\
");
  fprintf(outfile, "\n\
  static void compute_ks67_%s(MY_FLOAT t, MY_FLOAT *xx_in, MY_FLOAT step,  MY_FLOAT **k,\n\
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
  \n\
    static MY_JET jtmp1,jtmp2,jtmp3,jtmp4,jtmp5,jaux[%d]; //n\n\
#pragma omp threadprivate(jtmp1,jtmp2,jtmp3,jtmp4,jtmp5,jaux)\n\
    MY_JET **jetOut;	 //n\n\
\n\
\n\
",n);
    }
    fprintf(outfile, "\n\
\n\
    MY_FLOAT **fptr;\n\
    int i, j;    \n\
\n\
    if(!inited) {\n\
      InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);InitMyFloat(tmp4);InitMyFloat(tmp5);InitMyFloat(ta); \n\
      for(i=0;i<%d;i++) InitMyFloat(aux[i]); //m\n\
      \n\
",n);
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
");
      fprintf(outfile, "\n\
    //(*ode)(t,x,n,k[0]);\n\
    //  for (j=0; j<n; j++)\n\
    //  {\n\
    //    k[0][j] *= h;\n\
    //    aux[j]=x[j]+B10*k[0][j];\n\
    //  }    \n\
\n\
\n\
");

    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, jetIn, &jetOut);      \n\
    for(i=0;i<%d;i++){\n\
	AssignJetToJet(kjet[0][i], jetOut[i][1]);\n\
	MultiplyJetFloatA(jtmp1,kjet[0][i], step);\n\
	AssignJetToJet(kjet[0][i], jtmp1);\n\
	\n\
    	MultiplyJetFloatA(jtmp1,kjet[0][i], zB10);\n\
	AddJetJetA(jaux[i], jetIn[i], jtmp1);\n\
    }\n\
\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, NULL, NULL);      \n\
\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[0][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[0][i], step);\n\
      AssignMyFloat(k[0][i],tmp1);\n\
								   \n\
      MultiplyMyFloatA(tmp1,k[0][i], zB10);\n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    // ta=t+A1*h;\n\
    // (*ode)(ta,aux,n,k[1]);\n\
    // for (j=0; j<n; j++)\n\
    // {\n\
    // k[1][j] *= h;\n\
    // aux[j]=x[j]+B21*k[1][j];\n\
    // }    \n\
\n\
    MultiplyMyFloatA(tmp1, zA1, step);  \n\
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
      MultiplyJetFloatA(jtmp3,kjet[1][i], zB21);\n\
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
      MultiplyMyFloatA(tmp3,k[1][i], zB21);\n\
      AddMyFloatA(aux[i], xx_in[i], tmp3);            \n\
    }\n\
\n\
    //ta=t+A2*h;\n\
    //(*ode)(ta,aux,n,k[2]);\n\
    //  for (j=0; j<n; j++)\n\
    //{\n\
    //k[2][j] *= h;\n\
    //aux[j]=x[j]+B30*k[0][j]+B32*k[2][j];\n\
    //}\n\
    MultiplyMyFloatA(tmp1, zA2, step);  \n\
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
      MultiplyJetFloatA(jtmp3,kjet[0][i], zB30);\n\
      MultiplyJetFloatA(jtmp4,kjet[2][i], zB32);      \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4); //1           \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp1);      \n\
    }\n\
\n\
",suffix, n);
    } else {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[2][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[2][i], step);\n\
      AssignMyFloat(k[2][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp3,k[0][i], zB30);\n\
      MultiplyMyFloatA(tmp4,k[2][i], zB32);      \n\
      AddMyFloatA(tmp1, tmp3, tmp4); //1           \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    // ta=t+A3*h;\n\
    // (*ode)(ta,aux,n,k[3]);\n\
    // for (j=0; j<n; j++)\n\
    // {\n\
    // k[3][j] *= h;\n\
    // aux[j]=x[j]+B40*k[0][j]+B42*k[2][j]+B43*k[3][j];\n\
    // }\n\
    MultiplyMyFloatA(tmp1, zA3, step);  \n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], zB40);\n\
      MultiplyJetFloatA(jtmp2,kjet[2][i], zB42);\n\
      MultiplyJetFloatA(jtmp3,kjet[3][i], zB43);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp1, jtmp4, jtmp2);      \n\
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
      MultiplyMyFloatA(tmp1,k[0][i], zB40);\n\
      MultiplyMyFloatA(tmp2,k[2][i], zB42);\n\
      MultiplyMyFloatA(tmp3,k[3][i], zB43);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp1, tmp4, tmp2);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    //ta=t+A4*h;\n\
    //(*ode)(ta,aux,n,k[4]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[4][j] *= h;\n\
    //aux[j]=x[j]+B50*k[0][j]+B53*k[3][j]+B54*k[4][j]+B52*k[2][j];\n\
    //}    \n\
\n\
    MultiplyMyFloatA(tmp1, zA4, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], zB50);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], zB53);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], zB54);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp4);\n\
      MultiplyJetFloatA(jtmp3,kjet[2][i], zB52);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp1);      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp2);      \n\
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
      MultiplyMyFloatA(tmp1,k[0][i], zB50);\n\
      MultiplyMyFloatA(tmp2,k[3][i], zB53);\n\
      MultiplyMyFloatA(tmp3,k[4][i], zB54);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp1, tmp2, tmp4);\n\
      MultiplyMyFloatA(tmp3,k[2][i], zB52);\n\
      AddMyFloatA(tmp2, tmp3, tmp1);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp2);            \n\
    }\n\
\n\
    //ta=t+A5*h;\n\
    //(*ode)(ta,aux,n,k[5]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[5][j] *= h;\n\
    //aux[j]=x[j]+B60*k[0][j]+B63*k[3][j]+B64*k[4][j]+B65*k[5][j]+B62*k[2][j];\n\
    //}\n\
\n\
    MultiplyMyFloatA(tmp1, zA5, step);  \n\
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
\n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], zB60);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], zB63);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], zB64);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//\n\
      MultiplyJetFloatA(jtmp4,kjet[5][i], zB65);      \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);//\n\
      MultiplyJetFloatA(jtmp4,kjet[2][i], zB62);\n\
      AddJetJetA(jtmp2, jtmp4, jtmp1);//      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp2);      \n\
    }\n\
\n\
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
      AssignMyFloat(k[5][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[5][i], step);\n\
      AssignMyFloat(k[5][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], zB60);\n\
      MultiplyMyFloatA(tmp2,k[3][i], zB63);\n\
      MultiplyMyFloatA(tmp3,k[4][i], zB64);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4);\n\
      MultiplyMyFloatA(tmp4,k[5][i], zB65);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);\n\
      MultiplyMyFloatA(tmp4,k[2][i], zB62);\n\
      AddMyFloatA(tmp2, tmp4, tmp1);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp2);            \n\
    }\n\
\n\
    //ta=t+A6*h;\n\
    //(*ode)(ta,aux,n,k[6]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[6][j] *= h;\n\
    //aux[j]=x[j]+B70*k[0][j]+B74*k[4][j]+B75*k[5][j]+B76*k[6][j]++B72*k[2][j]+B73*k[3][j];\n\
    //}\n\
    MultiplyMyFloatA(tmp1, zA6, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[6][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[6][i], step);\n\
      AssignJetToJet(kjet[6][i], jtmp1);\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], zB70);\n\
      MultiplyJetFloatA(jtmp2,kjet[4][i], zB74);\n\
      MultiplyJetFloatA(jtmp3,kjet[5][i], zB75);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//\n\
      MultiplyJetFloatA(jtmp4,kjet[6][i], zB76);      \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);//1\n\
      MultiplyJetFloatA(jtmp2,kjet[2][i], zB72);\n\
      MultiplyJetFloatA(jtmp3,kjet[3][i], zB73);            \n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp1, jtmp4);//1      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp2);      \n\
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
      AssignMyFloat(k[6][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[6][i], step);\n\
      AssignMyFloat(k[6][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], zB70);\n\
      MultiplyMyFloatA(tmp2,k[4][i], zB74);\n\
      MultiplyMyFloatA(tmp3,k[5][i], zB75);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4);\n\
      MultiplyMyFloatA(tmp4,k[6][i], zB76);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);  //1\n\
      MultiplyMyFloatA(tmp2,k[2][i], zB72);\n\
      MultiplyMyFloatA(tmp3,k[3][i], zB73);\n\
      AddMyFloatA(tmp4, tmp3, tmp2);  //1\n\
      AddMyFloatA(tmp2, tmp1, tmp4);  //1      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp2);            \n\
    }\n\
\n\
    //ta=t+h;\n\
    //(*ode)(ta,aux,n,k[7]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[7][j] *= h;\n\
    //aux[j]=x[j]+B80*k[0][j]+B83*k[3][j]+B84*k[4][j]+B85*k[5][j]+B86*k[6][j];\n\
    //}    \n\
\n\
    AddMyFloatA(ta, t, step);\n\
\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[7][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[7][i], step);\n\
      AssignJetToJet(kjet[7][i], jtmp1);\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], zB80);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], zB83);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], zB84);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//3\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], zB85);\n\
      MultiplyJetFloatA(jtmp2,kjet[6][i], zB86);\n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      AddJetJetA(jaux[i], jetIn[i], jtmp2);      \n\
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
      AssignMyFloat(k[7][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[7][i], step);\n\
      AssignMyFloat(k[7][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], zB80);\n\
      MultiplyMyFloatA(tmp2,k[3][i], zB83);\n\
      MultiplyMyFloatA(tmp3,k[4][i], zB84);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4); //3\n\
      MultiplyMyFloatA(tmp1,k[5][i], zB85);\n\
      MultiplyMyFloatA(tmp2,k[6][i], zB86);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);            \n\
      AddMyFloatA(tmp2, tmp3, tmp4);//2\n\
      AddMyFloatA(aux[i], xx_in[i], tmp2);            \n\
    }\n\
\n\
    //ta=t;\n\
    //(*ode)(ta,aux,n,k[8]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[8][j] *= h;\n\
    //aux[j]=x[j]+B90*k[0][j]+B93*k[3][j]+B94*k[4][j]+B95*k[5][j]+B96*k[6][j]+k[8][j]+B92*k[2][j];\n\
    //}\n\
\n\
    AssignMyFloat(ta, t);\n\
\n\
",m);

   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[8][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[8][i], step);\n\
      AssignJetToJet(kjet[8][i], jtmp1);\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], zB90);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], zB93);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], zB94);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//3\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], zB95);\n\
      MultiplyJetFloatA(jtmp2,kjet[6][i], zB96);\n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp3,kjet[2][i], zB92);\n\
      AddJetJetA(jtmp4, jtmp3, kjet[8][i]); //4           \n\
      AddJetJetA(jtmp1, jtmp4, jtmp2);      \n\
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
      AssignMyFloat(k[8][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[8][i], step);\n\
      AssignMyFloat(k[8][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], zB90);\n\
      MultiplyMyFloatA(tmp2,k[3][i], zB93);\n\
      MultiplyMyFloatA(tmp3,k[4][i], zB94);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4); //3\n\
      MultiplyMyFloatA(tmp1,k[5][i], zB95);\n\
      MultiplyMyFloatA(tmp2,k[6][i], zB96);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);            \n\
      AddMyFloatA(tmp2, tmp3, tmp4);//2\n\
      MultiplyMyFloatA(tmp3,k[2][i], zB92);\n\
      AddMyFloatA(tmp4, tmp3, k[8][i]);            \n\
      AddMyFloatA(tmp1, tmp4, tmp2);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
    // ta=t+h;\n\
    // (*ode)(ta,aux,n,k[9]);\n\
    // for (j=0; j<n; j++)\n\
    // {\n\
    // k[9][j] *= h;\n\
    // }    \n\
\n\
    AddMyFloatA(ta, t, step);\n\
\n\
",m);
   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[9][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[9][i], step);\n\
      AssignJetToJet(kjet[9][i], jtmp1);\n\
    }\n\
\n\
",suffix,n);
   } else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n \n\
\n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[9][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[9][i], step);\n\
      AssignMyFloat(k[9][i], tmp1);\n\
    }\n\
  }\n\
\n\
",m);

   fprintf(outfile, "\n\
  void step_rk6_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n	\n\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4,initialized)\n\
\n\
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
\n\
",m,m);
   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
\n\
    InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);\n\
\n\
");
   }
   fprintf(outfile, "\n\
\n\
    initialized=1;\n\
  }\n\
\n\
");
   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
\n\
  MakeMyFloatA(jnr,(double)0);\n\
\n\
");
   }
fprintf(outfile, "\n\
\n\
  MakeMyFloatA(nr,(double)0);\n\
\n\
  //x[j] += C60*k[0][j]+C63*k[3][j]+C64*k[4][j]+C65*k[5][j]+C66*k[6][j]+C67*k[7][j];\n\
  // nr += x[j]*x[j];  \n\
\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], zC60);\n\
      MultiplyMyFloatA(tmp2, k[3][j], zC63);\n\
      MultiplyMyFloatA(tmp3, k[4][j], zC64);      \n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp4, tmp3);       //1\n\
      MultiplyMyFloatA(tmp2, k[5][j], zC65);\n\
      MultiplyMyFloatA(tmp3, k[6][j], zC66);      \n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      AddMyFloatA(tmp2, tmp1, tmp4);      //2\n\
      MultiplyMyFloatA(tmp3, k[7][j], zC67);            \n\
      AddMyFloatA(tmp1, tmp2, tmp3);      //1\n\
      \n\
      AddMyFloatA(tmp2, tmp1, xx_in[j]);\n\
      AssignMyFloat(xx_in[j], tmp2);\n\
\n\
      MultiplyMyFloatA(tmp3, tmp2, tmp2);      \n\
      AddMyFloatA(tmp1, nr, tmp3);\n\
      AssignMyFloat(nr, tmp1);      \n\
  }\n\
\n\
",m);
   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
  if(kjet)\n\
    for(j = 0; j < %d; ++j) { //n\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], zC60);\n\
      MultiplyJetFloatA(jtmp2, kjet[3][j], zC63);\n\
      MultiplyJetFloatA(jtmp3, kjet[4][j], zC64);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp1, jtmp4, jtmp3);      //1\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], zC65);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], zC66);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);  \n\
      AddJetJetA(jtmp2, jtmp1, jtmp4);      //2      \n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], zC67);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp2);\n\
\n\
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
");
   
   
   fprintf(outfile, "\n\
  void step_rk7_67_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n	\n\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4,initialized)\n\
\n\
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
\n\
    InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);\n\
\n\
");
   }
   fprintf(outfile, "\n\
    initialized=1;\n\
  }\n\
\n\
  MakeMyFloatA(jnr,(double)0);\n\
\n\
");

   fprintf(outfile, "\n\
\n\
  MakeMyFloatA(nr,(double)0);\n\
\n\
  // x[j] += C70*k[0][j]+C63*k[3][j]+C64*k[4][j]+C65*k[5][j]+C66*k[6][j]+C67*(k[8][j]+k[9][j]);\n\
  // nr += x[j]*x[j];  \n\
\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], zC70);\n\
      MultiplyMyFloatA(tmp2, k[3][j], zC63);\n\
      MultiplyMyFloatA(tmp3, k[4][j], zC64);            \n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);//1\n\
      MultiplyMyFloatA(tmp2, k[5][j], zC65);\n\
      MultiplyMyFloatA(tmp3, k[6][j], zC66);            \n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      AddMyFloatA(tmp2, tmp4, tmp1);  //2    \n\
      AddMyFloatA(tmp1, k[8][j], k[9][j]);//1\n\
      MultiplyMyFloatA(tmp3, tmp1, zC67);\n\
      AddMyFloatA(tmp1, tmp3, tmp2);  //1\n\
      \n\
      AddMyFloatA(tmp2, tmp1, xx_in[j]);\n\
      \n\
      AssignMyFloat(xx_in[j], tmp2);\n\
\n\
      MultiplyMyFloatA(tmp3, tmp2, tmp2);      \n\
      AddMyFloatA(tmp1, nr, tmp3);\n\
      AssignMyFloat(nr, tmp1);      \n\
  }\n\
\n\
",m);

   if(state_jet_vars > 0) {
     fprintf(outfile, "\n\
  if(kjet)\n\
    for(j = 0; j < %d; ++j) { //n\n\
\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], zC70);\n\
      MultiplyJetFloatA(jtmp2, kjet[3][j], zC63);\n\
      MultiplyJetFloatA(jtmp3, kjet[4][j], zC64);            \n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);       //1\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], zC65);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], zC66);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp4, jtmp1);       //2\n\
      \n\
      AddJetJetA(jtmp1, kjet[8][j], kjet[9][j]);\n\
      MultiplyJetFloatA(jtmp3, jtmp1, zC67);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp2);\n\
\n\
      AddJetJetA(jtmp2, jtmp1, jetIn[j]);\n\
      \n\
      AssignJetToJet(jetIn[j], jtmp2);\n\
      \n\
      xNormJet(tmp1,jetIn[j]);\n\
      AddMyFloatA(jnr, tmp1, jnr);\n\
    }\n\
  \n\
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
");

   fprintf(outfile, "\n\
 static void error_rk67_%s(MY_FLOAT **k, MY_JET **kjet, MY_FLOAT *er_ret) {\n\
   if(er_ret == NULL) return;\n\
   static MY_FLOAT s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero;\n\
   static int inited;\n\
#pragma omp threadprivate(inited,s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero,inited)\n\
   int j;\n\
\n\
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
     InitMyFloat(s); InitMyFloat(je);  InitMyFloat(e); InitMyFloat(NN); InitMyFloat(zero);\n\
     InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
\n\
\n\
");
   if(state_jet_vars > 0) {
   fprintf(outfile, "\n\
     InitJet(js); InitJet(jtmp1); InitJet(jtmp2); InitJet(jtmp3); InitJet(jtmp4);\n\
  \n\
");
   }
   fprintf(outfile, "\n\
\n\
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
\n\
   AssignMyFloat(je, zero);\n\
\n\
");
   }
   fprintf(outfile, "\n\
   //     s=k[0][j]+k[7][j]-k[8][j]-k[9][j];\n\
   //\n\
   //     e += s*s;\n\
   for(j = _J_DIM_; j < %d; ++j) { //m\n\
\n\
     AddMyFloatA(tmp4, k[0][j], k[7][j]);\n\
     AddMyFloatA(tmp3, k[8][j], k[9][j]);\n\
\n\
     SubtractMyFloatA(s, tmp4, tmp3);\n\
     MultiplyMyFloatA(tmp1, s, s);     \n\
\n\
     AddMyFloatA(e, e, tmp1);     \n\
   }\n\
\n\
",m);

   if(state_jet_vars > 0) {
   fprintf(outfile, "\n\
if(kjet)  \n\
   for(j = 0; j < %d; ++j) { //n\n\
     \n\
     AddJetJetA(jtmp4, kjet[0][j], kjet[7][j]);\n\
     AddJetJetA(jtmp3, kjet[8][j], kjet[9][j]);\n\
\n\
     SubtractJetJetA(js, jtmp4, jtmp3);\n\
\n\
     xNormJet(tmp2, js);     \n\
     MultiplyMyFloatA(tmp1, tmp2, tmp2);\n\
\n\
     AddMyFloatA(je, je, tmp1);     \n\
\n\
     AddMyFloatA(e, je, e);     \n\
}\n\
\n\
",n);
   }
   fprintf(outfile, "\n\
\n\
   DivideMyFloatA(tmp1, e, NN);\n\
\n\
   sqrtMyFloatA(tmp2, tmp1);\n\
   MultiplyMyFloatA(*er_ret, tmp2, zC67);\n\
\n\
}\n\
");
}

