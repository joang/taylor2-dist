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

#define RK78_C
#include "Header.h"


void genGeneralRK78Code() {

  int m = neqns - nonautonomous, n= state_jet_vars;
  fprintf(outfile, "\n\
#ifndef MY_FLOAT_IS_DOUBLE\n\
\n\
  static int rk78_constants_initialized;\n\
\n\
  static MY_FLOAT yeight,yeight2,yeight40,yfive,yfive1,yfive3,yfive4,yfour,yfour1,yfour100,yfour496,yfour5,ynine,ynine0,ynine00,ynine1,ynine76,yone,yone025,yone05,yone07,\n\
    yone08,yone2,yone25,yone3,yone35,yone6,yone64,yone7,yone777,yone8,yone9,yseven04,ysix,ysix0,ysix1,ysix5,ysix7,ythree,ythree00,ythree01,ythree1,ythree11,ythree3,ythree4,ythree41,ythree5,\n\
    ythree6,ytwo,ytwo4,ytwo0,ytwo05,ytwo133,ytwo193,ytwo25,ytwo3,ytwo383,ytwo5,ytwo7,ytwo80,ytwo89;\n\
\n\
  static MY_FLOAT yA1,yA2,yA3,yA4,yA5,yA6,yA7,yA8,yA9,yB10,yB20,yB21,yB30,yB32,yB40,yB42,yB43,yB50,yB53,yB54,yB60,yB63,yB64,yB65,yB70,yB74,yB75,yB76,yB80,yB83,yB84,yB85,yB86,yB87,yB90,\n\
    yB93,yB94,yB95,yB96,yB97,yB98,yBA0,yBA3,yBA4,yBA5,yBA6,yBA7,yBA8,yBA9,yBB0,yBB5,yBB6,yBB7,yBB8,yBB9,yBC0,yBC3,yBC4,yBC5,yBC6,yBC7,yBC8,yBC9,yC70,yC75,yC76,yC78,yC7A,yC85,yC86,yC88,yC8B,rk78_tmp1;\n\
  static void rk78_init_constants() {			\n\
    if(! rk78_constants_initialized) {\n\
      InitMyFloat(rk78_tmp1);      \n\
      InitMyFloat(yA1);\n\
      InitMyFloat(yA2);\n\
      InitMyFloat(yA3);\n\
      InitMyFloat(yA4);\n\
      InitMyFloat(yA5);\n\
      InitMyFloat(yA6);\n\
      InitMyFloat(yA7);\n\
      InitMyFloat(yA8);\n\
      InitMyFloat(yA9);\n\
      InitMyFloat(yB10);\n\
      InitMyFloat(yB20);\n\
      InitMyFloat(yB21);\n\
      InitMyFloat(yB30);\n\
      InitMyFloat(yB32);\n\
      InitMyFloat(yB40);\n\
      InitMyFloat(yB42);\n\
      InitMyFloat(yB43);\n\
      InitMyFloat(yB50);\n\
      InitMyFloat(yB53);\n\
      InitMyFloat(yB54);\n\
      InitMyFloat(yB60);\n\
      InitMyFloat(yB63);\n\
      InitMyFloat(yB64);\n\
      InitMyFloat(yB65);\n\
      InitMyFloat(yB70);\n\
      InitMyFloat(yB74);\n\
      InitMyFloat(yB75);\n\
      InitMyFloat(yB76);\n\
      InitMyFloat(yB80);\n\
      InitMyFloat(yB83);\n\
      InitMyFloat(yB84);\n\
      InitMyFloat(yB85);\n\
      InitMyFloat(yB86);\n\
      InitMyFloat(yB87);\n\
      InitMyFloat(yB90);\n\
      InitMyFloat(yB93);\n\
      InitMyFloat(yB94);\n\
      InitMyFloat(yB95);\n\
      InitMyFloat(yB96);\n\
      InitMyFloat(yB97);\n\
      InitMyFloat(yB98);\n\
      InitMyFloat(yBA0);\n\
      InitMyFloat(yBA3);\n\
      InitMyFloat(yBA4);\n\
      InitMyFloat(yBA5);\n\
      InitMyFloat(yBA6);\n\
      InitMyFloat(yBA7);\n\
      InitMyFloat(yBA8);\n\
      InitMyFloat(yBA9);\n\
      InitMyFloat(yBB0);\n\
      InitMyFloat(yBB5);\n\
      InitMyFloat(yBB6);\n\
      InitMyFloat(yBB7);\n\
      InitMyFloat(yBB8);\n\
      InitMyFloat(yBB9);\n\
      InitMyFloat(yBC0);\n\
      InitMyFloat(yBC3);\n\
      InitMyFloat(yBC4);\n\
      InitMyFloat(yBC5);\n\
      InitMyFloat(yBC6);\n\
      InitMyFloat(yBC7);\n\
      InitMyFloat(yBC8);\n\
      InitMyFloat(yBC9);\n\
      InitMyFloat(yC70);\n\
      InitMyFloat(yC75);\n\
      InitMyFloat(yC76);\n\
      InitMyFloat(yC78);\n\
      InitMyFloat(yC7A);\n\
      InitMyFloat(yC85);\n\
      InitMyFloat(yC86);\n\
      InitMyFloat(yC88);\n\
      InitMyFloat(yC8B);\n\
      \n\
      InitMyFloat(yeight);\n\
      InitMyFloat(yeight2);\n\
      InitMyFloat(yeight40);\n\
      InitMyFloat(yfive);\n\
      InitMyFloat(yfive1);\n\
      InitMyFloat(yfive3);\n\
      InitMyFloat(yfive4);\n\
      InitMyFloat(yfour);\n\
      InitMyFloat(yfour1);\n\
      InitMyFloat(yfour100);\n\
      InitMyFloat(yfour496);\n\
      InitMyFloat(yfour5);\n\
      InitMyFloat(ynine);\n\
      InitMyFloat(ynine0);\n\
      InitMyFloat(ynine00);\n\
      InitMyFloat(ynine1);\n\
      InitMyFloat(ynine76);\n\
      InitMyFloat(yone);\n\
      InitMyFloat(yone025);\n\
      InitMyFloat(yone05);\n\
      InitMyFloat(yone07);\n\
      InitMyFloat(yone08);\n\
      InitMyFloat(yone2);\n\
      InitMyFloat(yone25);\n\
      InitMyFloat(yone3);\n\
      InitMyFloat(yone35);\n\
      InitMyFloat(yone6);\n\
      InitMyFloat(yone64);\n\
      InitMyFloat(yone7);\n\
      InitMyFloat(yone777);\n\
      InitMyFloat(yone8);\n\
      InitMyFloat(yone9);\n\
      InitMyFloat(yseven04);\n\
      InitMyFloat(ysix);\n\
      InitMyFloat(ysix0);\n\
      InitMyFloat(ysix1);\n\
      InitMyFloat(ysix5);      \n\
      InitMyFloat(ysix7);\n\
      InitMyFloat(ythree);      \n\
      InitMyFloat(ythree00);\n\
      InitMyFloat(ythree01);\n\
      InitMyFloat(ythree1);\n\
      InitMyFloat(ythree11);\n\
      InitMyFloat(ythree3);\n\
      InitMyFloat(ythree4);\n\
      InitMyFloat(ythree41);\n\
      InitMyFloat(ythree5);\n\
      InitMyFloat(ythree6);\n\
      InitMyFloat(ytwo);\n\
      InitMyFloat(ytwo4);      \n\
      InitMyFloat(ytwo0);\n\
      InitMyFloat(ytwo05);\n\
      InitMyFloat(ytwo133);\n\
      InitMyFloat(ytwo193);\n\
      InitMyFloat(ytwo25);\n\
      InitMyFloat(ytwo3);\n\
      InitMyFloat(ytwo383);\n\
      InitMyFloat(ytwo5);\n\
      InitMyFloat(ytwo7);\n\
      InitMyFloat(ytwo80);\n\
      InitMyFloat(ytwo89);\n\
\n\
\n\
      MakeMyFloatC(yeight,          \"8\",(double)8);		 \n\
      MakeMyFloatC(yeight2,		\"82\",(double)82);	 \n\
      MakeMyFloatC(yeight40,	\"840\",(double)840);	 \n\
      MakeMyFloatC(yfive,		\"5\",(double)5);		 \n\
      MakeMyFloatC(yfive1,		\"51\",(double)51);	 \n\
      MakeMyFloatC(yfive3,		\"53\",(double)53);	 \n\
      MakeMyFloatC(yfive4,		\"54\",(double)54);	 \n\
      MakeMyFloatC(yfour,		\"4\",(double)4);		 \n\
      MakeMyFloatC(yfour1,		\"41\",(double)41);	 \n\
      MakeMyFloatC(yfour100,	\"4100\",(double)4100);	 \n\
      MakeMyFloatC(yfour496,	\"4496\",(double)4496);	 \n\
      MakeMyFloatC(yfour5,		\"45\",(double)45);	 \n\
      MakeMyFloatC(ynine,		\"9\",(double)9);		 \n\
      MakeMyFloatC(ynine0,		\"90\",(double)90);	 \n\
      MakeMyFloatC(ynine00,		\"900\",(double)900);	 \n\
      MakeMyFloatC(ynine1,		\"91\",(double)91);	 \n\
      MakeMyFloatC(ynine76,		\"976\",(double)976);	 \n\
      MakeMyFloatC(yone,		\"1\",(double)1);		 \n\
      MakeMyFloatC(yone025,		\"1025\",(double)1025);	 \n\
      MakeMyFloatC(yone05,		\"105\",(double)105);	 \n\
      MakeMyFloatC(yone07,		\"107\",(double)107);	 \n\
      MakeMyFloatC(yone08,		\"108\",(double)108);	 \n\
      MakeMyFloatC(yone2,		\"12\",(double)12);	 \n\
      MakeMyFloatC(yone25,		\"125\",(double)125);	 \n\
      MakeMyFloatC(yone3,		\"13\",(double)13);	 \n\
      MakeMyFloatC(yone35,		\"135\",(double)135);	 \n\
      MakeMyFloatC(yone6,		\"16\",(double)16);	 \n\
      MakeMyFloatC(yone64,		\"164\",(double)164);	 \n\
      MakeMyFloatC(yone7,		\"17\",(double)17);	 \n\
      MakeMyFloatC(yone777,		\"1777\",(double)1777);	 \n\
      MakeMyFloatC(yone8,		\"18\",(double)18);	 \n\
      MakeMyFloatC(yone9,		\"19\",(double)19);	 \n\
      MakeMyFloatC(yseven04,	\"704\",(double)704);	 \n\
      MakeMyFloatC(ysix,		\"6\",(double)6);		 \n\
      MakeMyFloatC(ysix0,		\"60\",(double)60);	 \n\
      MakeMyFloatC(ysix1,		\"61\",(double)61);\n\
      MakeMyFloatC(ysix5,		\"65\",(double)65);	       \n\
      MakeMyFloatC(ysix7,		\"67\",(double)67);\n\
      MakeMyFloatC(ythree,	\"3\",(double)3);	       \n\
      MakeMyFloatC(ythree00,	\"300\",(double)300);	 \n\
      MakeMyFloatC(ythree01,	\"301\",(double)301);	 \n\
      MakeMyFloatC(ythree1,		\"31\",(double)31);	 \n\
      MakeMyFloatC(ythree11,	\"311\",(double)311);	 \n\
      MakeMyFloatC(ythree3,		\"33\",(double)33);	 \n\
      MakeMyFloatC(ythree4,		\"34\",(double)34);	 \n\
      MakeMyFloatC(ythree41,	\"341\",(double)341);	 \n\
      MakeMyFloatC(ythree5,		\"35\",(double)35);	 \n\
      MakeMyFloatC(ythree6,		\"36\",(double)36);	 \n\
      MakeMyFloatC(ytwo,		\"2\",(double)2);\n\
      MakeMyFloatC(ytwo4,		\"24\",(double)24);		       \n\
      MakeMyFloatC(ytwo0,		\"20\",(double)20);	 \n\
      MakeMyFloatC(ytwo05,		\"205\",(double)205);	 \n\
      MakeMyFloatC(ytwo133,		\"2133\",(double)2133);	 \n\
      MakeMyFloatC(ytwo193,		\"2193\",(double)2193);	 \n\
      MakeMyFloatC(ytwo25,		\"225\",(double)225);	 \n\
      MakeMyFloatC(ytwo3,		\"23\",(double)23);	 \n\
      MakeMyFloatC(ytwo383,		\"2383\",(double)2383);	 \n\
      MakeMyFloatC(ytwo5,		\"25\",(double)25);	 \n\
      MakeMyFloatC(ytwo7,		\"27\",(double)27);	 \n\
      MakeMyFloatC(ytwo80,		\"280\",(double)280);	 \n\
      MakeMyFloatC(ytwo89,		\"289\",(double)289);	 \n\
\n\
      // #define A1 2.e0/27.e0\n\
      DivideMyFloatA(yA1,ytwo,ytwo7);\n\
      \n\
      // #define A2 1.e0/9.e0\n\
      DivideMyFloatA(yA2,yone,ynine);\n\
      \n\
      // #define A3 1.e0/6.e0\n\
      DivideMyFloatA(yA3,yone,ysix);\n\
      \n\
      // #define A4 5.e0/12.e0\n\
      DivideMyFloatA(yA4,yfive,yone2);\n\
      \n\
      // #define A5 0.5e0\n\
      DivideMyFloatA(yA5,yone,ytwo);\n\
      \n\
      // #define A6 5.e0/6.e0\n\
      DivideMyFloatA(yA6,yfive,ysix);\n\
      \n\
      // #define A7 1.e0/6.e0\n\
      DivideMyFloatA(yA7,yone,ysix);\n\
      \n\
      // #define A8 2.e0/3.e0\n\
      DivideMyFloatA(yA8,ytwo,ythree);\n\
      \n\
      // #define A9 1.e0/3.e0\n\
      DivideMyFloatA(yA9,yone,ythree);\n\
      \n\
      // #define B10 2.e0/27.e0\n\
      DivideMyFloatA(yB10,ytwo,ytwo7);\n\
      \n\
      // #define B20 1.e0/36.e0\n\
      DivideMyFloatA(yB20,yone,ythree6);\n\
      \n\
      // #define B21 1.e0/12.e0\n\
      DivideMyFloatA(yB21,yone,yone2);\n\
      \n\
      // #define B30 1.e0/24.e0\n\
      DivideMyFloatA(yB30,yone,ytwo4);\n\
      \n\
      // #define B32 0.125\n\
      DivideMyFloatA(yB32,yone,yeight);\n\
      \n\
      // #define B40 5.e0/12.e0\n\
      DivideMyFloatA(yB40,yfive,yone2);\n\
      \n\
      // #define B42 -1.5625  =25/16\n\
      DivideMyFloatA(rk78_tmp1,ytwo5,yone6);\n\
      NegateMyFloatA(yB42,rk78_tmp1);\n\
      \n\
      // #define B43 1.5625\n\
      NegateMyFloatA(yB43,yB42);\n\
      \n\
      // #define B50 1.e0/20.e0\n\
      DivideMyFloatA(yB50,yone,ytwo0);\n\
      \n\
      // #define B53 0.25\n\
      DivideMyFloatA(yB53,yone,yfour);\n\
      \n\
      // #define B54 1.e0/5.e0\n\
      DivideMyFloatA(yB54,yone,yfive);\n\
      \n\
      // #define B60 -25.e0/108.e0\n\
      DivideMyFloatA(rk78_tmp1,ytwo5,yone08);\n\
      NegateMyFloatA(yB60,rk78_tmp1);\n\
      \n\
      // #define B63 125.e0/108.e0\n\
      DivideMyFloatA(yB63,yone25,yone08);  \n\
      \n\
      // #define B64 -65.e0/27.e0\n\
      DivideMyFloatA(rk78_tmp1,ysix5,ytwo7);\n\
      NegateMyFloatA(yB64,rk78_tmp1);\n\
      \n\
      // #define B65 125.e0/54.e0\n\
      DivideMyFloatA(yB65,yone25,yfive4);\n\
	    \n\
      // #define B70 31.e0/300.e0\n\
      DivideMyFloatA(yB70,ythree1,ythree00);\n\
      \n\
      // #define B74 61.e0/225.e0\n\
      DivideMyFloatA(yB74,ysix1,ytwo25);\n\
      \n\
      // #define B75 -2.e0/9.e0\n\
      DivideMyFloatA(rk78_tmp1,ytwo,ynine);\n\
      NegateMyFloatA(yB75,rk78_tmp1);\n\
      \n\
      // #define B76 13.e0/900.e0\n\
      DivideMyFloatA(yB76,yone3,ynine00);\n\
      \n\
      // #define B80 2\n\
      AssignMyFloat(yB80,ytwo);\n\
      \n\
      // #define B83 -53.e0/6.e0\n\
      DivideMyFloatA(rk78_tmp1,yfive3,ysix);\n\
      NegateMyFloatA(yB83,rk78_tmp1);\n\
      \n\
      // #define B84 704.e0/45.e0\n\
      DivideMyFloatA(yB84,yseven04,yfour5);\n\
      \n\
      // #define B85 -107.e0/9.e0\n\
      DivideMyFloatA(rk78_tmp1,yone07,ynine);\n\
      NegateMyFloatA(yB85,rk78_tmp1);\n\
      \n\
      // #define B86 67.e0/90.e0\n\
      DivideMyFloatA(yB86,ysix7,ynine0);      \n\
      // #define B87 3\n\
      AssignMyFloat(yB87, ythree);\n\
      \n\
      // #define B90 -91.e0/108.e0\n\
      DivideMyFloatA(rk78_tmp1,ynine1,yone08);\n\
      NegateMyFloatA(yB90,rk78_tmp1);\n\
      \n\
      // #define B93 23.e0/108.e0\n\
      DivideMyFloatA(yB93,ytwo3,yone08);\n\
      \n\
      // #define B94 -976.e0/135.e0\n\
      DivideMyFloatA(rk78_tmp1,ynine76,yone35);\n\
      NegateMyFloatA(yB94,rk78_tmp1);\n\
      \n\
      // #define B95 311.e0/54.e0\n\
      DivideMyFloatA(yB95,ythree11,yfive4);\n\
      \n\
      // #define B96 -19.e0/60.e0\n\
      DivideMyFloatA(rk78_tmp1,yone9,ysix0);\n\
      NegateMyFloatA(yB96,rk78_tmp1);\n\
      \n\
      // #define B97 17.e0/6.e0\n\
      DivideMyFloatA(yB97,yone7,ysix);\n\
      \n\
      // #define B98 -1.e0/12.e0\n\
      DivideMyFloatA(rk78_tmp1,yone,yone2);\n\
      NegateMyFloatA(yB98,rk78_tmp1);\n\
      \n\
      // #define BA0 2383.e0/4100.e0\n\
      DivideMyFloatA(yBA0,ytwo383,yfour100);\n\
      \n\
      // #define BA3 -341.e0/164.e0\n\
      DivideMyFloatA(rk78_tmp1,ythree41,yone64);\n\
      NegateMyFloatA(yBA3,rk78_tmp1);\n\
      \n\
      // #define BA4 4496.e0/1025.e0\n\
      DivideMyFloatA(yBA4,yfour496,yone025);\n\
      \n\
      // #define BA5 -301.e0/82.e0\n\
      DivideMyFloatA(rk78_tmp1,ythree01,yeight2);\n\
      NegateMyFloatA(yBA5,rk78_tmp1);\n\
      \n\
      // #define BA6 2133.e0/4100.e0\n\
      DivideMyFloatA(yBA6,ytwo133,yfour100);\n\
      \n\
      // #define BA7 45.e0/82.e0\n\
      DivideMyFloatA(yBA7,yfour5,yeight2);\n\
      \n\
      // #define BA8 45.e0/164.e0\n\
      DivideMyFloatA(yBA8,yfour5,yone64);\n\
      \n\
      // #define BA9 18.e0/41.e0\n\
      DivideMyFloatA(yBA9,yone8,yfour1);\n\
      \n\
      // #define BB0 3.e0/205.e0\n\
      DivideMyFloatA(yBB0,ythree,ytwo05);\n\
      \n\
      // #define BB5 -6.e0/41.e0\n\
      DivideMyFloatA(rk78_tmp1,ysix,yfour1);\n\
      NegateMyFloatA(yBB5,rk78_tmp1);\n\
\n\
      // #define BB6 -3.e0/205.e0\n\
      DivideMyFloatA(rk78_tmp1,ythree,ytwo05);\n\
      NegateMyFloatA(yBB6,rk78_tmp1);\n\
\n\
      // #define BB7 -3.e0/41.e0\n\
      DivideMyFloatA(rk78_tmp1,ythree,yfour1);\n\
      NegateMyFloatA(yBB7,rk78_tmp1);\n\
      \n\
      // #define BB8 3.e0/41.e0\n\
      DivideMyFloatA(yBB8,ythree,yfour1);\n\
      \n\
      // #define BB9 6.e0/41.e0\n\
      DivideMyFloatA(yBB9,ysix,yfour1);\n\
      \n\
      // #define BC0 -1777.e0/4100.e0\n\
      DivideMyFloatA(rk78_tmp1,yone777,yfour100);\n\
      NegateMyFloatA(yBC0,rk78_tmp1);\n\
      \n\
      // #define BC3 -341.e0/164.e0\n\
      DivideMyFloatA(rk78_tmp1,ythree41,yone64);\n\
      NegateMyFloatA(yBC3,rk78_tmp1);\n\
      \n\
      // #define BC4 4496.e0/1025.e0\n\
      DivideMyFloatA(yBC4,yfour496,yone025);\n\
      \n\
      // #define BC5 -289.e0/82.e0\n\
      DivideMyFloatA(rk78_tmp1,ytwo89,yeight2);\n\
      NegateMyFloatA(yBC5,rk78_tmp1);\n\
      \n\
      // #define BC6 2193.e0/4100.e0\n\
      DivideMyFloatA(yBC6,ytwo193,yfour100);\n\
      \n\
      // #define BC7 51.e0/82.e0\n\
      DivideMyFloatA(yBC7,yfive1,yeight2);\n\
      \n\
      // #define BC8 33.e0/164.e0\n\
      DivideMyFloatA(yBC8,ythree3,yone64);\n\
      \n\
      // #define BC9 12.e0/41.e0\n\
      DivideMyFloatA(yBC9,yone2,yfour1);\n\
      \n\
      // #define C70 41.e0/840.e0\n\
      DivideMyFloatA(yC70,yfour1,yeight40);\n\
      \n\
      // #define C75 34.e0/105.e0\n\
      DivideMyFloatA(yC75,ythree4,yone05);\n\
      \n\
      // #define C76 9.e0/35.e0\n\
      DivideMyFloatA(yC76,ynine,ythree5);\n\
      \n\
      // #define C78 9.e0/280.e0\n\
      DivideMyFloatA(yC78,ynine,ytwo80);\n\
      \n\
      // #define C7A 41.e0/840.e0\n\
      DivideMyFloatA(yC7A,yfour1,yeight40);\n\
      \n\
      // #define C85 34.e0/105.e0\n\
      DivideMyFloatA(yC85,ythree4,yone05);\n\
      \n\
      // #define C86 9.e0/35.e0\n\
      DivideMyFloatA(yC86,ynine,ythree5);\n\
      \n\
      // #define C88 9.e0/280.e0\n\
      DivideMyFloatA(yC88,ynine,ytwo80);\n\
      \n\
      // #define C8B 41.e0/840.e0\n\
      DivideMyFloatA(yC8B,yfour1,yeight40);\n\
\n\
      rk78_constants_initialized = 1;\n\
    }\n\
  }\n\
#else\n\
  static void rk78_init_constants() {}	\n\
#define yA1 (2.e0/27.e0)\n\
#define yA2 (1.e0/9.e0)\n\
#define yA3 (1.e0/6.e0)\n\
#define yA4 (5.e0/12.e0)\n\
#define yA5 (0.5e0)\n\
#define yA6 (5.e0/6.e0)\n\
#define yA7 (1.e0/6.e0)\n\
#define yA8 (2.e0/3.e0)\n\
#define yA9 (1.e0/3.e0)\n\
#define yB10 (2.e0/27.e0)\n\
#define yB20 (1.e0/36.e0)\n\
#define yB21 (1.e0/12.e0)\n\
#define yB30 (1.e0/24.e0)\n\
#define yB32 (0.125)\n\
#define yB40 (5.e0/12.e0)\n\
#define yB42 (-1.5625)\n\
#define yB43 (1.5625)\n\
#define yB50 (1.e0/20.e0)\n\
#define yB53 (0.25)\n\
#define yB54 (1.e0/5.e0)\n\
#define yB60 (-25.e0/108.e0)\n\
#define yB63 (125.e0/108.e0)\n\
#define yB64 (-65.e0/27.e0)\n\
#define yB65 (125.e0/54.e0)\n\
#define yB70 (31.e0/300.e0)\n\
#define yB74 (61.e0/225.e0)\n\
#define yB75 (-2.e0/9.e0)\n\
#define yB76 (13.e0/900.e0)\n\
#define yB80 (2)\n\
#define yB83 (-53.e0/6.e0)\n\
#define yB84 (704.e0/45.e0)\n\
#define yB85 (-107.e0/9.e0)\n\
#define yB86 (67.e0/90.e0)\n\
#define yB87 (3)\n\
#define yB90 (-91.e0/108.e0)\n\
#define yB93 (23.e0/108.e0)\n\
#define yB94 (-976.e0/135.e0)\n\
#define yB95 (311.e0/54.e0)\n\
#define yB96 (-19.e0/60.e0)\n\
#define yB97 (17.e0/6.e0)\n\
#define yB98 (-1.e0/12.e0)\n\
#define yBA0 (2383.e0/4100.e0)\n\
#define yBA3 (-341.e0/164.e0)\n\
#define yBA4 (4496.e0/1025.e0)\n\
#define yBA5 (-301.e0/82.e0)\n\
#define yBA6 (2133.e0/4100.e0)\n\
#define yBA7 (45.e0/82.e0)\n\
#define yBA8 (45.e0/164.e0)\n\
#define yBA9 (18.e0/41.e0)\n\
#define yBB0 (3.e0/205.e0)\n\
#define yBB5 (-6.e0/41.e0)\n\
#define yBB6 (-3.e0/205.e0)\n\
#define yBB7 (-3.e0/41.e0)\n\
#define yBB8 (3.e0/41.e0)\n\
#define yBB9 (6.e0/41.e0)\n\
#define yBC0 (-1777.e0/4100.e0)\n\
#define yBC3 (-341.e0/164.e0)\n\
#define yBC4 (4496.e0/1025.e0)\n\
#define yBC5 (-289.e0/82.e0)\n\
#define yBC6 (2193.e0/4100.e0)\n\
#define yBC7 (51.e0/82.e0)\n\
#define yBC8 (33.e0/164.e0)\n\
#define yBC9 (12.e0/41.e0)\n\
#define yC70 (41.e0/840.e0)\n\
#define yC75 (34.e0/105.e0)\n\
#define yC76 (9.e0/35.e0)\n\
#define yC78 (9.e0/280.e0)\n\
#define yC7A (41.e0/840.e0)\n\
#define yC85 (34.e0/105.e0)\n\
#define yC86 (9.e0/35.e0)\n\
#define yC88 (9.e0/280.e0)\n\
#define yC8B (41.e0/840.e0)\n\
#endif\n\
\n\
");
  
  fprintf(outfile, "\n\
  static void compute_ks78_%s(MY_FLOAT t, MY_FLOAT *xx_in, MY_FLOAT step,  MY_FLOAT **k,\n\
			      MY_JET *jetIn, MY_JET **kjet ) {\n\
    // double ta;\n\
    static int inited;\n\
    static MY_FLOAT tmp1,tmp2,tmp3,tmp4,tmp5,ta,aux[%d]; //m\n\
\n\
#pragma omp threadprivate(inited, tmp1,tmp2,tmp3,tmp4,tmp5,ta,aux,inited)\n\
",suffix,m);
  
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    static MY_JET jtmp1,jtmp2,jtmp3,jtmp4,jtmp5,jaux[%d]; //n\n\
#pragma omp threadprivate(jtmp1,jtmp2,jtmp3,jtmp4,jtmp5,jaux)\n\
    MY_JET **jetOut;	 //n\n\
\n\
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
",n);
    }
    
      fprintf(outfile, "\n\
   \n\
      inited = 1;\n\
    }\n\
\n\
");
      fprintf(outfile, "\n\
\n\
    //(*ode)(t,x,n,k[0]);\n\
    //  for (j=0; j<n; j++)\n\
    //  {\n\
    //    k[0][j] *= h;\n\
    //    aux[j]=x[j]+B10*k[0][j];\n\
    //  }    \n\
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
    	MultiplyJetFloatA(jtmp1,kjet[0][i], yB10);\n\
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
								   \n\
      MultiplyMyFloatA(tmp1,k[0][i], yB10);\n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);      \n\
    }\n\
\n\
    // ta=t+A1*h;\n\
    // (*ode)(ta,aux,n,k[1]);\n\
    // for (j=0; j<n; j++)\n\
    // {\n\
    // k[1][j] *= h;\n\
    // aux[j]=x[j]+B20*k[0][j]+B21*k[1][j];\n\
    // }    \n\
\n\
    MultiplyMyFloatA(tmp1, yA1, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
",m);

    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[1][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[1][i], step);\n\
      AssignJetToJet(kjet[1][i], jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB20);\n\
      MultiplyJetFloatA(jtmp2,kjet[1][i], yB21);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp1);      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp3);      \n\
    }\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[1][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[1][i], step);\n\
      AssignMyFloat(k[1][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yB20);\n\
      MultiplyMyFloatA(tmp2,k[1][i], yB21);\n\
      AddMyFloatA(tmp3, tmp2, tmp1);      \n\
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
    MultiplyMyFloatA(tmp1, yA2, step);  \n\
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
      MultiplyJetFloatA(jtmp3,kjet[0][i], yB30);\n\
      MultiplyJetFloatA(jtmp4,kjet[2][i], yB32);      \n\
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
      MultiplyMyFloatA(tmp3,k[0][i], yB30);\n\
      MultiplyMyFloatA(tmp4,k[2][i], yB32);      \n\
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
    MultiplyMyFloatA(tmp1, yA3, step);  \n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB40);\n\
      MultiplyJetFloatA(jtmp2,kjet[2][i], yB42);\n\
      MultiplyJetFloatA(jtmp3,kjet[3][i], yB43);      \n\
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
      MultiplyMyFloatA(tmp1,k[0][i], yB40);\n\
      MultiplyMyFloatA(tmp2,k[2][i], yB42);\n\
      MultiplyMyFloatA(tmp3,k[3][i], yB43);      \n\
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
    //aux[j]=x[j]+B50*k[0][j]+B53*k[3][j]+B54*k[4][j];\n\
    //}    \n\
\n\
    MultiplyMyFloatA(tmp1, yA4, step);  \n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB50);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], yB53);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], yB54);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp4);      \n\
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
      MultiplyMyFloatA(tmp1,k[0][i], yB50);\n\
      MultiplyMyFloatA(tmp2,k[3][i], yB53);\n\
      MultiplyMyFloatA(tmp3,k[4][i], yB54);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp1, tmp2, tmp4);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
\n\
    //ta=t+A5*h;\n\
    //(*ode)(ta,aux,n,k[5]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[5][j] *= h;\n\
    //aux[j]=x[j]+B60*k[0][j]+B63*k[3][j]+B64*k[4][j]+B65*k[5][j];\n\
    //}\n\
\n\
    MultiplyMyFloatA(tmp1, yA5, step);  \n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB60);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], yB63);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], yB64);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//\n\
      MultiplyJetFloatA(jtmp4,kjet[5][i], yB65);      \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);//      \n\
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
      AssignMyFloat(k[5][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[5][i], step);\n\
      AssignMyFloat(k[5][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yB60);\n\
      MultiplyMyFloatA(tmp2,k[3][i], yB63);\n\
      MultiplyMyFloatA(tmp3,k[4][i], yB64);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4);\n\
      MultiplyMyFloatA(tmp4,k[5][i], yB65);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
\n\
    //ta=t+A6*h;\n\
    //(*ode)(ta,aux,n,k[6]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[6][j] *= h;\n\
    //aux[j]=x[j]+B70*k[0][j]+B74*k[4][j]+B75*k[5][j]+B76*k[6][j];\n\
    //}\n\
    MultiplyMyFloatA(tmp1, yA6, step);  \n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB70);\n\
      MultiplyJetFloatA(jtmp2,kjet[4][i], yB74);\n\
      MultiplyJetFloatA(jtmp3,kjet[5][i], yB75);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//\n\
      MultiplyJetFloatA(jtmp4,kjet[6][i], yB76);      \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);//      \n\
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
      AssignMyFloat(k[6][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[6][i], step);\n\
      AssignMyFloat(k[6][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yB70);\n\
      MultiplyMyFloatA(tmp2,k[4][i], yB74);\n\
      MultiplyMyFloatA(tmp3,k[5][i], yB75);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4);\n\
      MultiplyMyFloatA(tmp4,k[6][i], yB76);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
    //ta=t+A7*h;\n\
    //(*ode)(ta,aux,n,k[7]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[7][j] *= h;\n\
    //aux[j]=x[j]+B80*k[0][j]+B83*k[3][j]+B84*k[4][j]+B85*k[5][j]+B86*k[6][j]+B87*k[7][j];\n\
    //}    \n\
\n\
    MultiplyMyFloatA(tmp1, yA7, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB80);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], yB83);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], yB84);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//3\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], yB85);\n\
      MultiplyJetFloatA(jtmp2,kjet[6][i], yB86);\n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp3,kjet[7][i], yB87);      \n\
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
      AssignMyFloat(k[7][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[7][i], step);\n\
      AssignMyFloat(k[7][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yB80);\n\
      MultiplyMyFloatA(tmp2,k[3][i], yB83);\n\
      MultiplyMyFloatA(tmp3,k[4][i], yB84);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4); //3\n\
      MultiplyMyFloatA(tmp1,k[5][i], yB85);\n\
      MultiplyMyFloatA(tmp2,k[6][i], yB86);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);            \n\
      AddMyFloatA(tmp2, tmp3, tmp4);//2\n\
      MultiplyMyFloatA(tmp3,k[7][i], yB87);\n\
      AddMyFloatA(tmp1, tmp3, tmp2);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
    //ta=t+A8*h;\n\
    //(*ode)(ta,aux,n,k[8]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[8][j] *= h;\n\
    //aux[j]=x[j]+B90*k[0][j]+B93*k[3][j]+B94*k[4][j]+B95*k[5][j]+B96*k[6][j]+B97*k[7][j]+B98*k[8][j];\n\
    //}\n\
\n\
    MultiplyMyFloatA(tmp1, yA8, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
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
      MultiplyJetFloatA(jtmp1,kjet[0][i], yB90);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], yB93);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], yB94);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//3\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], yB95);\n\
      MultiplyJetFloatA(jtmp2,kjet[6][i], yB96);\n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp3,kjet[7][i], yB97);\n\
      MultiplyJetFloatA(jtmp1,kjet[8][i], yB98);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp1); //4           \n\
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
     for(i=0;i<%d;i++){ //m\n	\
      AssignMyFloat(k[8][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[8][i], step);\n\
      AssignMyFloat(k[8][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yB90);\n\
      MultiplyMyFloatA(tmp2,k[3][i], yB93);\n\
      MultiplyMyFloatA(tmp3,k[4][i], yB94);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4); //3\n\
      MultiplyMyFloatA(tmp1,k[5][i], yB95);\n\
      MultiplyMyFloatA(tmp2,k[6][i], yB96);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);            \n\
      AddMyFloatA(tmp2, tmp3, tmp4);//2\n\
      MultiplyMyFloatA(tmp3,k[7][i], yB97);\n\
      MultiplyMyFloatA(tmp1,k[8][i], yB98);\n\
      AddMyFloatA(tmp4, tmp3, tmp1);            \n\
      AddMyFloatA(tmp1, tmp4, tmp2);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
    // ta=t+A9*h;\n\
    // (*ode)(ta,aux,n,k[9]);\n\
    // for (j=0; j<n; j++)\n\
    // {\n\
    // k[9][j] *= h;\n\
    // aux[j]=x[j]+BA0*k[0][j]+BA3*k[3][j]+BA4*k[4][j]+BA5*k[5][j]+BA6*k[6][j]+BA7*k[7][j]+BA8*k[8][j]+BA9*k[9][j];\n\
    // }    \n\
\n\
    MultiplyMyFloatA(tmp1, yA9, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[9][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[9][i], step);\n\
      AssignJetToJet(kjet[9][i], jtmp1);\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], yBA0);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], yBA3);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], yBA4);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//3\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], yBA5);\n\
      MultiplyJetFloatA(jtmp2,kjet[6][i], yBA6);\n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp3,kjet[7][i], yBA7);\n\
      MultiplyJetFloatA(jtmp1,kjet[8][i], yBA8);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp1); //4\n\
      MultiplyJetFloatA(jtmp1,kjet[9][i], yBA9);\n\
      AddJetJetA(jtmp3, jtmp1, jtmp4);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp2);      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp1);      \n\
    }\n\
\n\
",suffix,n);
    } else {
       fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, NULL, NULL);\n \
\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[9][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[9][i], step);\n\
      AssignMyFloat(k[9][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yBA0);\n\
      MultiplyMyFloatA(tmp2,k[3][i], yBA3);\n\
      MultiplyMyFloatA(tmp3,k[4][i], yBA4);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4); //3\n\
      MultiplyMyFloatA(tmp1,k[5][i], yBA5);\n\
      MultiplyMyFloatA(tmp2,k[6][i], yBA6);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);            \n\
      AddMyFloatA(tmp2, tmp3, tmp4);//2\n\
      MultiplyMyFloatA(tmp3,k[7][i], yBA7);\n\
      MultiplyMyFloatA(tmp1,k[8][i], yBA8);\n\
      AddMyFloatA(tmp4, tmp3, tmp1); //4\n\
      MultiplyMyFloatA(tmp1,k[9][i], yBA9);\n\
      AddMyFloatA(tmp3, tmp1, tmp4);            \n\
      AddMyFloatA(tmp1, tmp3, tmp2);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp1);            \n\
    }\n\
\n\
    //ta=t+h;\n\
    //(*ode)(ta,aux,n,k[10]);\n\
    // for (j=0; j<n; j++)\n\
    //{\n\
    //    k[10][j] *= h;\n\
    //aux[j]=x[j]+BB0*(k[0][j]-k[6][j])+BB9*(k[9][j]-k[5][j])+BB8*(k[8][j]-k[7][j]);\n\
    //}    \n\
    AddMyFloatA(ta, t, step);\n\
\n\
",m);

   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[10][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[10][i], step);\n\
      AssignJetToJet(kjet[10][i], jtmp1);\n\
\n\
      SubtractJetJetA(jtmp1,kjet[0][i], kjet[6][i]);\n\
      MultiplyJetFloatA(jtmp2,jtmp1, yBB0);//2\n\
      SubtractJetJetA(jtmp1,kjet[9][i], kjet[5][i]);\n\
      MultiplyJetFloatA(jtmp3,jtmp1, yBB9);//3\n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);     //1\n\
      SubtractJetJetA(jtmp2,kjet[8][i], kjet[7][i]);\n\
      MultiplyJetFloatA(jtmp3,jtmp2, yBB8);\n\
\n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
\n\
      AddJetJetA(jaux[i], jetIn[i], jtmp4);      \n\
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
      AssignMyFloat(k[10][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[10][i], step);\n\
      AssignMyFloat(k[10][i], tmp1);\n\
\n\
      SubtractMyFloatA(tmp1,k[0][i], k[6][i]);\n\
      MultiplyMyFloatA(tmp2,tmp1, yBB0);//2\n\
      SubtractMyFloatA(tmp1,k[9][i], k[5][i]);\n\
      MultiplyMyFloatA(tmp3,tmp1, yBB9);//3\n\
      AddMyFloatA(tmp1, tmp2, tmp3);     //1\n\
      SubtractMyFloatA(tmp2,k[8][i], k[7][i]);\n\
      MultiplyMyFloatA(tmp3,tmp2, yBB8);\n\
\n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp4);            \n\
    }\n\
\n\
    //(*ode)(t,aux,n,k[11]);\n\
    //for (j=0; j<n; j++)\n\
    //{\n\
    //k[11][j] *= h;\n\
    //aux[j]=x[j]+BC0*k[0][j]+BC3*k[3][j]+BC4*k[4][j]+BC5*k[5][j]+BC6*k[6][j]+BC7*k[7][j]+BC8*k[8][j]+BC9*k[9][j]+k[11][j];\n\
    //}\n\
\n\
\n\
",m);

   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[11][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[11][i], step);\n\
      AssignJetToJet(kjet[11][i], jtmp1);\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[0][i], yBC0);\n\
      MultiplyJetFloatA(jtmp2,kjet[3][i], yBC3);\n\
      MultiplyJetFloatA(jtmp3,kjet[4][i], yBC4);      \n\
      AddJetJetA(jtmp4, jtmp3, jtmp1);\n\
      AddJetJetA(jtmp3, jtmp2, jtmp4);//3\n\
      \n\
      MultiplyJetFloatA(jtmp1,kjet[5][i], yBC5);\n\
      MultiplyJetFloatA(jtmp2,kjet[6][i], yBC6);\n\
      AddJetJetA(jtmp4, jtmp2, jtmp1);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4); //2\n\
      MultiplyJetFloatA(jtmp3,kjet[7][i], yBC7);\n\
      MultiplyJetFloatA(jtmp1,kjet[8][i], yBC8);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp1); //4\n\
      MultiplyJetFloatA(jtmp1,kjet[9][i], yBC9);\n\
      AddJetJetA(jtmp3, jtmp1, jtmp4);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp2); //1\n\
\n\
      AddJetJetA(jtmp2, jtmp1, kjet[11][i]); //1\n\
      \n\
      AddJetJetA(jaux[i], jetIn[i], jtmp2);      \n\
    }\n\
\n\
",suffix,n);
   } else {
     fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, aux, 1, 0, NULL, NULL);\n\
\n\
",suffix);
   }
   fprintf(outfile, "\n\
    for(i=0;i<%d;i++){ //m\n\
      AssignMyFloat(k[11][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[11][i], step);\n\
      AssignMyFloat(k[11][i], tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1,k[0][i], yBC0);\n\
      MultiplyMyFloatA(tmp2,k[3][i], yBC3);\n\
      MultiplyMyFloatA(tmp3,k[4][i], yBC4);      \n\
      AddMyFloatA(tmp4, tmp3, tmp1);\n\
      AddMyFloatA(tmp3, tmp2, tmp4); //3\n\
      MultiplyMyFloatA(tmp1,k[5][i], yBC5);\n\
      MultiplyMyFloatA(tmp2,k[6][i], yBC6);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);            \n\
      AddMyFloatA(tmp2, tmp3, tmp4);//2\n\
      MultiplyMyFloatA(tmp3,k[7][i], yBC7);\n\
      MultiplyMyFloatA(tmp1,k[8][i], yBC8);\n\
      AddMyFloatA(tmp4, tmp3, tmp1); //4\n\
      MultiplyMyFloatA(tmp1,k[9][i], yBC9);\n\
      AddMyFloatA(tmp3, tmp1, tmp4);            \n\
      AddMyFloatA(tmp1, tmp3, tmp2);\n\
\n\
      AddMyFloatA(tmp2, tmp1, k[11][i]);      \n\
      AddMyFloatA(aux[i], xx_in[i], tmp2);            \n\
    }\n\
\n\
    //(*ode)(ta,aux,n,k[12]);\n\
    //for (j=0; j<n; j++) k[12][j] *= h;\n\
\n\
",m);
   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, aux, 1, 0, jaux, &jetOut);      \n\
    for(i=0;i<%d;i++){ //n\n\
      AssignJetToJet(kjet[12][i], jetOut[i][1]);\n\
      MultiplyJetFloatA(jtmp1,kjet[12][i], step);\n\
      AssignJetToJet(kjet[12][i], jtmp1);\n\
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
      AssignMyFloat(k[12][i], fptr[i][1]);\n\
      MultiplyMyFloatA(tmp1,k[12][i], step);\n\
      AssignMyFloat(k[12][i], tmp1);\n\
    }\n\
",m);

   fprintf(outfile, "\n\
  }\n\
\n\
\n\
");

   fprintf(outfile, "\n\
  void step_rk7_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
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
");

   if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
  MakeMyFloatA(jnr,(double)0);\n\
");
   }
   fprintf(outfile, "\n\
  MakeMyFloatA(nr,(double)0);\n\
\n\
  //x[j] += C70*k[0][j]+C75*k[5][j]+ C76* (k[6][j]+k[7][j]) + C78*(k[8][j]+k[9][j])+ C7A*k[10][j];\n\
  // nr += x[j]*x[j];  \n\
\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], yC70);\n\
      MultiplyMyFloatA(tmp2, k[5][j], yC75);\n\
      AddMyFloatA(tmp4, k[6][j], k[7][j]);\n\
      MultiplyMyFloatA(tmp3, tmp4, yC76);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);      //1\n\
\n\
      AddMyFloatA(tmp4, k[8][j], k[9][j]);\n\
      MultiplyMyFloatA(tmp3, tmp4, yC78);\n\
      MultiplyMyFloatA(tmp2, k[10][j], yC7A);\n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      \n\
      AddMyFloatA(tmp3, tmp1, tmp4);\n\
\n\
      AddMyFloatA(tmp2, tmp3, xx_in[j]);\n\
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
      MultiplyJetFloatA(jtmp1, kjet[0][j], yC70);\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], yC75);\n\
      AddJetJetA(jtmp4, kjet[6][j], kjet[7][j]);\n\
      MultiplyJetFloatA(jtmp3, jtmp4, yC76);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);      //1\n\
\n\
      AddJetJetA(jtmp4, kjet[8][j], kjet[9][j]);\n\
      MultiplyJetFloatA(jtmp3, jtmp4, yC78);\n\
      MultiplyJetFloatA(jtmp2, kjet[10][j], yC7A);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);\n\
      \n\
      AddJetJetA(jtmp3, jtmp1, jtmp4);\n\
\n\
      AddJetJetA(jtmp2, jtmp3, jetIn[j]);\n\
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
");
   
   
   fprintf(outfile, "\n\
  void step_rk8_78_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n	\n\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4, initlalized)\n\
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
  // x[j] += C85*k[5][j]+  C86* (k[6][j]+k[7][j]) +C88* (k[8][j]+k[9][j]) + C8B* (k[11][j]+k[12][j]);  \n\
  // nr += x[j]*x[j];  \n\
\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[5][j], yC85);\n\
\n\
      AddMyFloatA(tmp4, k[6][j], k[7][j]);\n\
      MultiplyMyFloatA(tmp3, tmp4, yC86);\n\
\n\
      AddMyFloatA(tmp4, k[8][j], k[9][j]);\n\
      MultiplyMyFloatA(tmp2, tmp4, yC88);\n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      AddMyFloatA(tmp2, tmp1, tmp4);//2\n\
\n\
      AddMyFloatA(tmp4, k[11][j], k[12][j]);      \n\
      MultiplyMyFloatA(tmp3, tmp4, yC8B);\n\
\n\
      AddMyFloatA(tmp1, tmp3, tmp2);      \n\
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
      MultiplyJetFloatA(jtmp1, kjet[5][j], yC85);\n\
\n\
      AddJetJetA(jtmp4, kjet[6][j], kjet[7][j]);\n\
      MultiplyJetFloatA(jtmp3, jtmp4, yC86);\n\
\n\
      AddJetJetA(jtmp4, kjet[8][j], kjet[9][j]);\n\
      MultiplyJetFloatA(jtmp2, jtmp4, yC88);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp1, jtmp4);//2\n\
\n\
      AddJetJetA(jtmp4, kjet[11][j], kjet[12][j]);      \n\
      MultiplyJetFloatA(jtmp3, jtmp4, yC8B);\n\
\n\
      AddJetJetA(jtmp1, jtmp3, jtmp2);      \n\
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
\n\
");

   fprintf(outfile, "\n\
 static void error_rk78_%s(MY_FLOAT **k, MY_JET **kjet, MY_FLOAT *er_ret) {\n\
   if(er_ret == NULL) return;\n\
   static MY_FLOAT s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero;\n\
   static int inited;\n\
#pragma omp threadprivate(inited,s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero, inited)\n\
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
     InitMyFloat(s); InitMyFloat(je);  InitMyFloat(e); InitMyFloat(NN); InitMyFloat(zero);\n\
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
",m,m);

   if(state_jet_vars > 0) {
   fprintf(outfile, "\n\
\n\
   AssignMyFloat(je, zero);\n\
");
   }
   fprintf(outfile, "\n\
   //     s=k[0][j]+k[10][j]-k[11][j]-k[12][j]; // * 41/840 = yC70\n\
   //     e += s*s;\n\
   for(j = _J_DIM_; j < %d; ++j) { //m\n\
\n\
     AddMyFloatA(tmp4, k[0][j], k[10][j]);\n\
     AddMyFloatA(tmp3, k[11][j], k[12][j]);\n\
\n\
     SubtractMyFloatA(s, tmp4, tmp3);\n\
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
     AddJetJetA(jtmp4, kjet[0][j], kjet[10][j]);\n\
     AddJetJetA(jtmp3, kjet[11][j], kjet[12][j]);\n\
\n\
     SubtractJetJetA(js, jtmp4, jtmp3);\n\
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
   sqrtMyFloatA(tmp2, tmp1);\n\
   MultiplyMyFloatA(*er_ret, tmp2, yC70);\n\
\n\
}\n\
\n\
");
}
