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

#define RK89_C
#include "Header.h"

void genGeneralRK89Code() {

  int m = neqns - nonautonomous, n= state_jet_vars; // int m1=m+1, n1=n+1;

  fprintf(outfile, "\n\
#ifndef MY_FLOAT_IS_DOUBLE\n\
  static int rk89_constants_initialized;\n\
 \n\
  static MY_FLOAT RT6,eight,eight1,eight13,eight4,eight6,eight64,eight7,five798,five996,five,five034,five12,five120,five18400,five25,five2807,five4,five42,five642,five67,five70,five7600,five825,five94,four,four00,four042,four0458,four05,four0717,four176,four200,four342,four4,four499,four5,four68,four8,four89,four92,nine,nine087,nine1056,nine2,nine27,nine3,nine3200,nine375,nine4,nine4329,nine60,nine75,one,one296,one0240,one626,one1,one44,one01226,one024,one03,one03680,one048,one05,one053,one1296,one1520,one165,one18,one2,one21408,one25,one250,one2537,one260,one35,one365,one37,one3824,one383,one4,one40,one400,one425,one4268,one5,one50,one5176,one54922,one55338,one6,one6248,one664,one680,one69984,one71,one7475,one75,one79,one797,one8200,one84,one86400,one9,one920,seven,seven20,seven859,seven280,seven328,seven456,seven6,seven70,seven947,six,six1440,six15973,six3,six57,six825,six9,six9120,thirty5,three,three5,three0290,three0352,three1,three12,three2,three20,three28,three3617,three37,three38,three40864,three45600,three47,three600,three6400,three6487,three75,three750,three793,three794,three846,three9,two,two2764,two232,two0,two00,two01,two025,two048,two0480,two08,two1,two168,two2232,two263,two2754,two3,two31278,two32192,two33,two4,two42816,two5,two56,two624,two6304,two66,two6624,two7,two71,two779182,two79600,two80,two9,two9666,two99,rk89_tmp1,rk89_tmp2,rk89_tmp3;\n\
  static MY_FLOAT A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A14,B10,B20,B21,B30,B32,B40,B42,B43,B50,B53,B54,B60,B63,B64,B65,B70,B75,B76,B80,B85,B86,B87,B90,B95,B96,B97,B98,B100,B106,B107,B108,B109,B110,B115,B116,B117,B118,B119,B1110,B120,B125,B126,B127,B128,B129,B1210,B1211,B130,B135,B136,B137,B138,B139,B1310,B1311,B1312,B140,B145,B146,B147,B148,B149,B1410,B1411,B1412,B150,B155,B156,B157,B158,B159,B1510,B1511,B1512,B1514,C80,C87,C88,C89,C810,C811,C812,C813,C90,C97,C98,C99,C910,C911,C912,C914,C915,E0,E7,E8,E9,E10,E11,E12,E13,E14,E15;\n\
\n\
  static void rk89_init_constants() {			\n\
    if(! rk89_constants_initialized) {\n\
      InitMyFloat(RT6);\n\
      InitMyFloat(eight);\n\
      InitMyFloat(eight1);\n\
      InitMyFloat(eight13);\n\
      InitMyFloat(eight4);\n\
      InitMyFloat(eight6);\n\
      InitMyFloat(eight64);\n\
      InitMyFloat(eight7);\n\
      InitMyFloat(five798);\n\
      InitMyFloat(five996);\n\
      InitMyFloat(five);\n\
      InitMyFloat(five034);\n\
      InitMyFloat(five12);\n\
      InitMyFloat(five120);\n\
      InitMyFloat(five18400);\n\
      InitMyFloat(five25);\n\
      InitMyFloat(five2807);\n\
      InitMyFloat(five4);\n\
      InitMyFloat(five42);\n\
      InitMyFloat(five642);\n\
      InitMyFloat(five67);\n\
      InitMyFloat(five70);\n\
      InitMyFloat(five7600);\n\
      InitMyFloat(five825);\n\
      InitMyFloat(five94);\n\
      InitMyFloat(four);\n\
      InitMyFloat(four00);\n\
      InitMyFloat(four042);\n\
      InitMyFloat(four0458);\n\
      InitMyFloat(four05);\n\
      InitMyFloat(four0717);\n\
      InitMyFloat(four176);\n\
      InitMyFloat(four200);\n\
      InitMyFloat(four342);\n\
      InitMyFloat(four4);\n\
      InitMyFloat(four499);\n\
      InitMyFloat(four5);\n\
      InitMyFloat(four68);\n\
      InitMyFloat(four8);\n\
      InitMyFloat(four89);\n\
      InitMyFloat(four92);\n\
      InitMyFloat(nine);\n\
      InitMyFloat(nine087);\n\
      InitMyFloat(nine1056);\n\
      InitMyFloat(nine2);\n\
      InitMyFloat(nine27);\n\
      InitMyFloat(nine3);\n\
      InitMyFloat(nine3200);\n\
      InitMyFloat(nine375);\n\
      InitMyFloat(nine4);\n\
      InitMyFloat(nine4329);\n\
      InitMyFloat(nine60);\n\
      InitMyFloat(nine75);\n\
      InitMyFloat(one);\n\
      InitMyFloat(one296);\n\
      InitMyFloat(one0240);\n\
      InitMyFloat(one626);\n\
      InitMyFloat(one1);\n\
      InitMyFloat(one44);\n\
      InitMyFloat(one01226);\n\
      InitMyFloat(one024);\n\
      InitMyFloat(one03);\n\
      InitMyFloat(one03680);\n\
      InitMyFloat(one048);\n\
      InitMyFloat(one05);\n\
      InitMyFloat(one053);\n\
      InitMyFloat(one1296);\n\
      InitMyFloat(one1520);\n\
      InitMyFloat(one165);\n\
      InitMyFloat(one18);\n\
      InitMyFloat(one2);\n\
      InitMyFloat(one21408);\n\
      InitMyFloat(one25);\n\
      InitMyFloat(one250);\n\
      InitMyFloat(one2537);\n\
      InitMyFloat(one260);\n\
      InitMyFloat(one35);\n\
      InitMyFloat(one365);\n\
      InitMyFloat(one37);\n\
      InitMyFloat(one3824);\n\
      InitMyFloat(one383);\n\
      InitMyFloat(one4);\n\
      InitMyFloat(one40);\n\
      InitMyFloat(one400);\n\
      InitMyFloat(one425);\n\
      InitMyFloat(one4268);\n\
      InitMyFloat(one5);\n\
      InitMyFloat(one50);\n\
      InitMyFloat(one5176);\n\
      InitMyFloat(one54922);\n\
      InitMyFloat(one55338);\n\
      InitMyFloat(one6);\n\
      InitMyFloat(one6248);\n\
      InitMyFloat(one664);\n\
      InitMyFloat(one680);\n\
      InitMyFloat(one69984);\n\
      InitMyFloat(one71);\n\
      InitMyFloat(one7475);\n\
      InitMyFloat(one75);\n\
      InitMyFloat(one79);\n\
      InitMyFloat(one797);\n\
      InitMyFloat(one8200);\n\
      InitMyFloat(one84);\n\
      InitMyFloat(one86400);\n\
      InitMyFloat(one9);\n\
      InitMyFloat(one920);\n\
      InitMyFloat(seven);\n\
      InitMyFloat(seven20);\n\
      InitMyFloat(seven859);\n\
      InitMyFloat(seven280);\n\
      InitMyFloat(seven328);\n\
      InitMyFloat(seven456);\n\
      InitMyFloat(seven6);\n\
      InitMyFloat(seven70);\n\
      InitMyFloat(seven947);\n\
      InitMyFloat(six);\n\
      InitMyFloat(six1440);\n\
      InitMyFloat(six15973);\n\
      InitMyFloat(six3);\n\
      InitMyFloat(six57);\n\
      InitMyFloat(six825);\n\
      InitMyFloat(six9);\n\
      InitMyFloat(six9120);\n\
      InitMyFloat(thirty5);\n\
      InitMyFloat(three);\n\
      InitMyFloat(three5);\n\
      InitMyFloat(three0290);\n\
      InitMyFloat(three0352);\n\
      InitMyFloat(three1);\n\
      InitMyFloat(three12);\n\
      InitMyFloat(three2);\n\
      InitMyFloat(three20);\n\
      InitMyFloat(three28);\n\
      InitMyFloat(three3617);\n\
      InitMyFloat(three37);\n\
      InitMyFloat(three38);\n\
      InitMyFloat(three40864);\n\
      InitMyFloat(three45600);\n\
      InitMyFloat(three47);\n\
      InitMyFloat(three600);\n\
      InitMyFloat(three6400);\n\
      InitMyFloat(three6487);\n\
      InitMyFloat(three75);\n\
      InitMyFloat(three750);\n\
      InitMyFloat(three793);\n\
      InitMyFloat(three794);\n\
      InitMyFloat(three846);\n\
      InitMyFloat(three9);\n\
      InitMyFloat(two);\n\
      InitMyFloat(two2764);\n\
      InitMyFloat(two232);\n\
      InitMyFloat(two0);\n\
      InitMyFloat(two00);\n\
      InitMyFloat(two01);\n\
      InitMyFloat(two025);\n\
      InitMyFloat(two048);\n\
      InitMyFloat(two0480);\n\
      InitMyFloat(two08);\n\
      InitMyFloat(two1);\n\
      InitMyFloat(two168);\n\
      InitMyFloat(two2232);\n\
      InitMyFloat(two263);\n\
      InitMyFloat(two2754);\n\
      InitMyFloat(two3);\n\
      InitMyFloat(two31278);\n\
      InitMyFloat(two32192);\n\
      InitMyFloat(two33);\n\
      InitMyFloat(two4);\n\
      InitMyFloat(two42816);\n\
      InitMyFloat(two5);\n\
      InitMyFloat(two56);\n\
      InitMyFloat(two624);\n\
      InitMyFloat(two6304);\n\
      InitMyFloat(two66);\n\
      InitMyFloat(two6624);\n\
      InitMyFloat(two7);\n\
      InitMyFloat(two71);\n\
      InitMyFloat(two779182);\n\
      InitMyFloat(two79600);\n\
      InitMyFloat(two80);\n\
      InitMyFloat(two9);\n\
      InitMyFloat(two9666);\n\
      InitMyFloat(two99);\n\
      InitMyFloat(rk89_tmp1);\n\
      InitMyFloat(rk89_tmp2);\n\
      InitMyFloat(rk89_tmp3);\n\
      InitMyFloat(A1);\n\
      InitMyFloat(A2);\n\
      InitMyFloat(A3);\n\
      InitMyFloat(A4);\n\
      InitMyFloat(A5);\n\
      InitMyFloat(A6);\n\
      InitMyFloat(A7);\n\
      InitMyFloat(A8);\n\
      InitMyFloat(A9);\n\
      InitMyFloat(A10);\n\
      InitMyFloat(A11);\n\
      InitMyFloat(A12);\n\
      InitMyFloat(A14);\n\
      InitMyFloat(B10);\n\
      InitMyFloat(B20);\n\
      InitMyFloat(B21);\n\
      InitMyFloat(B30);\n\
      InitMyFloat(B32);\n\
      InitMyFloat(B40);\n\
      InitMyFloat(B42);\n\
      InitMyFloat(B43);\n\
      InitMyFloat(B50);\n\
      InitMyFloat(B53);\n\
      InitMyFloat(B54);\n\
      InitMyFloat(B60);\n\
      InitMyFloat(B63);\n\
      InitMyFloat(B64);\n\
      InitMyFloat(B65);\n\
      InitMyFloat(B70);\n\
      InitMyFloat(B75);\n\
      InitMyFloat(B76);\n\
      InitMyFloat(B80);\n\
      InitMyFloat(B85);\n\
      InitMyFloat(B86);\n\
      InitMyFloat(B87);\n\
      InitMyFloat(B90);\n\
      InitMyFloat(B95);\n\
      InitMyFloat(B96);\n\
      InitMyFloat(B97);\n\
      InitMyFloat(B98);\n\
      InitMyFloat(B100);\n\
      InitMyFloat(B106);\n\
      InitMyFloat(B107);\n\
      InitMyFloat(B108);\n\
      InitMyFloat(B109);\n\
      InitMyFloat(B110);\n\
      InitMyFloat(B115);\n\
      InitMyFloat(B116);\n\
      InitMyFloat(B117);\n\
      InitMyFloat(B118);\n\
      InitMyFloat(B119);\n\
      InitMyFloat(B1110);\n\
      InitMyFloat(B120);\n\
      InitMyFloat(B125);\n\
      InitMyFloat(B126);\n\
      InitMyFloat(B127);\n\
      InitMyFloat(B128);\n\
      InitMyFloat(B129);\n\
      InitMyFloat(B1210);\n\
      InitMyFloat(B1211);\n\
      InitMyFloat(B130);\n\
      InitMyFloat(B135);\n\
      InitMyFloat(B136);\n\
      InitMyFloat(B137);\n\
      InitMyFloat(B138);\n\
      InitMyFloat(B139);\n\
      InitMyFloat(B1310);\n\
      InitMyFloat(B1311);\n\
      InitMyFloat(B1312);\n\
      InitMyFloat(B140);\n\
      InitMyFloat(B145);\n\
      InitMyFloat(B146);\n\
      InitMyFloat(B147);\n\
      InitMyFloat(B148);\n\
      InitMyFloat(B149);\n\
      InitMyFloat(B1410);\n\
      InitMyFloat(B1411);\n\
      InitMyFloat(B1412);\n\
      InitMyFloat(B150);\n\
      InitMyFloat(B155);\n\
      InitMyFloat(B156);\n\
      InitMyFloat(B157);\n\
      InitMyFloat(B158);\n\
      InitMyFloat(B159);\n\
      InitMyFloat(B1510);\n\
      InitMyFloat(B1511);\n\
      InitMyFloat(B1512);\n\
      InitMyFloat(B1514);\n\
      InitMyFloat(C80);\n\
      InitMyFloat(C87);\n\
      InitMyFloat(C88);\n\
      InitMyFloat(C89);\n\
      InitMyFloat(C810);\n\
      InitMyFloat(C811);\n\
      InitMyFloat(C812);\n\
      InitMyFloat(C813);\n\
      InitMyFloat(C90);\n\
      InitMyFloat(C97);\n\
      InitMyFloat(C98);\n\
      InitMyFloat(C99);\n\
      InitMyFloat(C910);\n\
      InitMyFloat(C911);\n\
      InitMyFloat(C912);\n\
      InitMyFloat(C914);\n\
      InitMyFloat(C915);\n\
      InitMyFloat(E0);\n\
      InitMyFloat(E7);\n\
      InitMyFloat(E8);\n\
      InitMyFloat(E9);\n\
      InitMyFloat(E10);\n\
      InitMyFloat(E11);\n\
      InitMyFloat(E12);\n\
      InitMyFloat(E13);\n\
      InitMyFloat(E14);\n\
      InitMyFloat(E15);\n\
\n\
      MakeMyFloatC(eight,\"8\",(double)8);\n\
      MakeMyFloatC(eight1,\"81\",(double)81);\n\
      MakeMyFloatC(eight13,\"813\",(double)813);\n\
      MakeMyFloatC(eight4,\"84\",(double)84);\n\
      MakeMyFloatC(eight6,\"86\",(double)86);\n\
      MakeMyFloatC(eight64,\"864\",(double)864);\n\
      MakeMyFloatC(eight7,\"87\",(double)87);\n\
      MakeMyFloatC(five798,\"5798\",(double)5798);\n\
      MakeMyFloatC(five996,\"5996\",(double)5996);\n\
      MakeMyFloatC(five,\"5\",(double)5);\n\
      MakeMyFloatC(five034,\"5034\",(double)5034);\n\
      MakeMyFloatC(five12,\"512\",(double)512);\n\
      MakeMyFloatC(five120,\"5120\",(double)5120);\n\
      MakeMyFloatC(five18400,\"518400\",(double)518400);\n\
      MakeMyFloatC(five25,\"525\",(double)525);\n\
      MakeMyFloatC(five2807,\"52807\",(double)52807);\n\
      MakeMyFloatC(five4,\"54\",(double)54);\n\
      MakeMyFloatC(five42,\"542\",(double)542);\n\
      MakeMyFloatC(five642,\"5642\",(double)5642);\n\
      MakeMyFloatC(five67,\"567\",(double)567);\n\
      MakeMyFloatC(five70,\"570\",(double)570);\n\
      MakeMyFloatC(five7600,\"57600\",(double)57600);\n\
      MakeMyFloatC(five825,\"5825\",(double)5825);\n\
      MakeMyFloatC(five94,\"594\",(double)594);\n\
      MakeMyFloatC(four,\"4\",(double)4);\n\
      MakeMyFloatC(four00,\"400\",(double)400);\n\
      MakeMyFloatC(four042,\"4042\",(double)4042);\n\
      MakeMyFloatC(four0458,\"40458\",(double)40458);\n\
      MakeMyFloatC(four05,\"405\",(double)405);\n\
      MakeMyFloatC(four0717,\"40717\",(double)40717);\n\
      MakeMyFloatC(four176,\"4176\",(double)4176);\n\
      MakeMyFloatC(four200,\"4200\",(double)4200);\n\
      MakeMyFloatC(four342,\"4342\",(double)4342);\n\
      MakeMyFloatC(four4,\"44\",(double)44);\n\
      MakeMyFloatC(four499,\"4499\",(double)4499);\n\
      MakeMyFloatC(four5,\"45\",(double)45);\n\
      MakeMyFloatC(four68,\"468\",(double)468);\n\
      MakeMyFloatC(four8,\"48\",(double)48);\n\
      MakeMyFloatC(four89,\"489\",(double)489);\n\
      MakeMyFloatC(four92,\"492\",(double)492);\n\
      MakeMyFloatC(nine,\"9\",(double)9);\n\
      MakeMyFloatC(nine087,\"9087\",(double)9087);\n\
      MakeMyFloatC(nine1056,\"91056\",(double)91056);\n\
      MakeMyFloatC(nine2,\"92\",(double)92);\n\
      MakeMyFloatC(nine27,\"927\",(double)927);\n\
      MakeMyFloatC(nine3,\"93\",(double)93);\n\
      MakeMyFloatC(nine3200,\"93200\",(double)93200);\n\
      MakeMyFloatC(nine375,\"9375\",(double)9375);\n\
      MakeMyFloatC(nine4,\"94\",(double)94);\n\
      MakeMyFloatC(nine4329,\"94329\",(double)94329);\n\
      MakeMyFloatC(nine60,\"960\",(double)960);\n\
      MakeMyFloatC(nine75,\"975\",(double)975);\n\
      MakeMyFloatC(one,\"1\",(double)1);\n\
      MakeMyFloatC(one296,\"1296\",(double)1296);\n\
      MakeMyFloatC(one0240,\"10240\",(double)10240);\n\
      MakeMyFloatC(one626,\"1626\",(double)1626);\n\
      MakeMyFloatC(one01226,\"101226\",(double)101226);\n\
      MakeMyFloatC(one024,\"1024\",(double)1024);\n\
      MakeMyFloatC(one03,\"103\",(double)103);\n\
      MakeMyFloatC(one03680,\"103680\",(double)103680);\n\
      MakeMyFloatC(one048,\"1048\",(double)1048);\n\
      MakeMyFloatC(one05,\"105\",(double)105);\n\
      MakeMyFloatC(one053,\"1053\",(double)1053);\n\
      MakeMyFloatC(one1296,\"11296\",(double)11296);\n\
      MakeMyFloatC(one1520,\"11520\",(double)11520);\n\
      MakeMyFloatC(one165,\"1165\",(double)1165);\n\
      MakeMyFloatC(one18,\"118\",(double)118);\n\
      MakeMyFloatC(one1,\"11\",(double)11);\n\
      MakeMyFloatC(one44,\"144\",(double)144);\n\
      MakeMyFloatC(one2,\"12\",(double)12);\n\
      MakeMyFloatC(one21408,\"121408\",(double)121408);\n\
      MakeMyFloatC(one25,\"125\",(double)125);\n\
      MakeMyFloatC(one250,\"1250\",(double)1250);\n\
      MakeMyFloatC(one2537,\"12537\",(double)12537);\n\
      MakeMyFloatC(one260,\"1260\",(double)1260);\n\
      MakeMyFloatC(one35,\"135\",(double)135);\n\
      MakeMyFloatC(one365,\"1365\",(double)1365);\n\
      MakeMyFloatC(one37,\"137\",(double)137);\n\
      MakeMyFloatC(one3824,\"13824\",(double)13824);\n\
      MakeMyFloatC(one383,\"1383\",(double)1383);\n\
      MakeMyFloatC(one4,\"14\",(double)14);\n\
      MakeMyFloatC(one40,\"140\",(double)140);\n\
      MakeMyFloatC(one400,\"1400\",(double)1400);\n\
      MakeMyFloatC(one425,\"1425\",(double)1425);\n\
      MakeMyFloatC(one4268,\"14268\",(double)14268);\n\
      MakeMyFloatC(one5,\"15\",(double)15);\n\
      MakeMyFloatC(one50,\"150\",(double)150);\n\
      MakeMyFloatC(one5176,\"15176\",(double)15176);\n\
      MakeMyFloatC(one54922,\"154922\",(double)154922);\n\
      MakeMyFloatC(one55338,\"155338\",(double)155338);\n\
      MakeMyFloatC(one6,\"16\",(double)16);\n\
      MakeMyFloatC(one6248,\"16248\",(double)16248);\n\
      MakeMyFloatC(one664,\"1664\",(double)1664);\n\
      MakeMyFloatC(one680,\"1680\",(double)1680);\n\
      MakeMyFloatC(one69984,\"169984\",(double)169984);\n\
      MakeMyFloatC(one71,\"171\",(double)171);\n\
      MakeMyFloatC(one7475,\"17475\",(double)17475);\n\
      MakeMyFloatC(one75,\"175\",(double)175);\n\
      MakeMyFloatC(one79,\"179\",(double)179);\n\
      MakeMyFloatC(one797,\"1797\",(double)1797);\n\
      MakeMyFloatC(one8200,\"18200\",(double)18200);\n\
      MakeMyFloatC(one84,\"184\",(double)184);\n\
      MakeMyFloatC(one86400,\"186400\",(double)186400);\n\
      MakeMyFloatC(one9,\"19\",(double)19);\n\
      MakeMyFloatC(one920,\"1920\",(double)1920);\n\
      MakeMyFloatC(seven,\"7\",(double)7);\n\
      MakeMyFloatC(seven20,\"720\",(double)720);\n\
      MakeMyFloatC(seven859,\"7859\",(double)7859);\n\
      MakeMyFloatC(seven280,\"7280\",(double)7280);\n\
      MakeMyFloatC(seven328,\"7328\",(double)7328);\n\
      MakeMyFloatC(seven456,\"7456\",(double)7456);\n\
      MakeMyFloatC(seven6,\"76\",(double)76);\n\
      MakeMyFloatC(seven70,\"770\",(double)770);\n\
      MakeMyFloatC(seven947,\"7947\",(double)7947);\n\
      MakeMyFloatC(six,\"6\",(double)6);\n\
      MakeMyFloatC(six1440,\"61440\",(double)61440);\n\
      MakeMyFloatC(six15973,\"615973\",(double)615973);\n\
      MakeMyFloatC(six3,\"63\",(double)63);\n\
      MakeMyFloatC(six57,\"657\",(double)657);\n\
      MakeMyFloatC(six825,\"6825\",(double)6825);\n\
      MakeMyFloatC(six9,\"69\",(double)69);\n\
      MakeMyFloatC(six9120,\"69120\",(double)69120);\n\
      MakeMyFloatC(thirty5,\"thirty5\",(double)thirty5);\n\
      MakeMyFloatC(three,\"3\",(double)3);\n\
      MakeMyFloatC(three5,\"35\",(double)35);\n\
      MakeMyFloatC(three0290,\"30290\",(double)30290);\n\
      MakeMyFloatC(three0352,\"30352\",(double)30352);\n\
      MakeMyFloatC(three1,\"31\",(double)31);\n\
      MakeMyFloatC(three12,\"312\",(double)312);\n\
      MakeMyFloatC(three2,\"32\",(double)32);\n\
      MakeMyFloatC(three20,\"320\",(double)320);\n\
      MakeMyFloatC(three28,\"328\",(double)328);\n\
      MakeMyFloatC(three3617,\"33617\",(double)33617);\n\
      MakeMyFloatC(three37,\"337\",(double)337);\n\
      MakeMyFloatC(three38,\"338\",(double)338);\n\
      MakeMyFloatC(three40864,\"340864\",(double)340864);\n\
      MakeMyFloatC(three45600,\"345600\",(double)345600);\n\
      MakeMyFloatC(three47,\"347\",(double)347);\n\
      MakeMyFloatC(three600,\"3600\",(double)3600);\n\
      MakeMyFloatC(three6400,\"36400\",(double)36400);\n\
      MakeMyFloatC(three6487,\"36487\",(double)36487);\n\
      MakeMyFloatC(three75,\"375\",(double)375);\n\
      MakeMyFloatC(three750,\"3750\",(double)3750);\n\
      MakeMyFloatC(three793,\"3793\",(double)3793);\n\
      MakeMyFloatC(three794,\"3794\",(double)3794);\n\
      MakeMyFloatC(three846,\"3846\",(double)3846);\n\
      MakeMyFloatC(three9,\"39\",(double)39);\n\
      MakeMyFloatC(two,\"2\",(double)2);\n\
      MakeMyFloatC(two2764,\"22764\",(double)22764);\n\
      MakeMyFloatC(two232,\"2232\",(double)2232);\n\
      MakeMyFloatC(two0,\"20\",(double)20);\n\
      MakeMyFloatC(two00,\"200\",(double)200);\n\
      MakeMyFloatC(two01,\"201\",(double)201);\n\
      MakeMyFloatC(two025,\"2025\",(double)2025);\n\
      MakeMyFloatC(two048,\"2048\",(double)2048);\n\
      MakeMyFloatC(two0480,\"20480\",(double)20480);\n\
      MakeMyFloatC(two08,\"208\",(double)208);\n\
      MakeMyFloatC(two1,\"21\",(double)21);\n\
      MakeMyFloatC(two168,\"2168\",(double)2168);\n\
      MakeMyFloatC(two2232,\"22232\",(double)22232);\n\
      MakeMyFloatC(two263,\"2263\",(double)2263);\n\
      MakeMyFloatC(two2754,\"22754\",(double)22754);\n\
      MakeMyFloatC(two3,\"23\",(double)23);\n\
      MakeMyFloatC(two31278,\"231278\",(double)231278);\n\
      MakeMyFloatC(two32192,\"232192\",(double)232192);\n\
      MakeMyFloatC(two33,\"233\",(double)233);\n\
      MakeMyFloatC(two4,\"24\",(double)24);\n\
      MakeMyFloatC(two42816,\"242816\",(double)242816);\n\
      MakeMyFloatC(two5,\"25\",(double)25);\n\
      MakeMyFloatC(two56,\"256\",(double)256);\n\
      MakeMyFloatC(two624,\"2624\",(double)2624);\n\
      MakeMyFloatC(two6304,\"26304\",(double)26304);\n\
      MakeMyFloatC(two66,\"266\",(double)266);\n\
      MakeMyFloatC(two6624,\"26624\",(double)26624);\n\
      MakeMyFloatC(two7,\"27\",(double)27);\n\
      MakeMyFloatC(two71,\"271\",(double)271);\n\
      MakeMyFloatC(two779182,\"2779182\",(double)2779182);\n\
      MakeMyFloatC(two79600,\"279600\",(double)279600);\n\
      MakeMyFloatC(two80,\"280\",(double)280);\n\
      MakeMyFloatC(two9,\"29\",(double)29);\n\
      MakeMyFloatC(two9666,\"29666\",(double)29666);\n\
      MakeMyFloatC(two99,\"299\",(double)299);\n\
\n\
      //#define RT6   2.449489742783178098197;\n\
      sqrtMyFloatA(RT6,six);\n\
\n\
      //#define A1    (1.0 / 12.0);\n\
      DivideMyFloatA(A1,one,one2);\n\
\n\
      //#define A2    (1.0 / 9.0);\n\
      DivideMyFloatA(A2,one,nine);\n\
\n\
      //#define A3    (1.0 / 6.0);\n\
      DivideMyFloatA(A3,one,six);\n\
\n\
      //#define A4    ((2.0 + 2.0 * RT6) / 15.0);\n\
      MultiplyMyFloatA(rk89_tmp1, two, RT6);\n\
      AddMyFloatA(rk89_tmp2, rk89_tmp1, two);\n\
      DivideMyFloatA(A4,rk89_tmp2,one5);\n\
\n\
      //#define A5    ((6.0 + RT6) / 15.0);\n\
      AddMyFloatA(rk89_tmp1, six, RT6);\n\
      DivideMyFloatA(A5,rk89_tmp1,one5);\n\
    \n\
      //#define A6    ((6.0 - RT6) / 15.0);\n\
      SubtractMyFloatA(rk89_tmp1, six, RT6);\n\
      DivideMyFloatA(A6,rk89_tmp1,one5);\n\
\n\
      //#define A7    (2.0 / 3.0);\n\
      DivideMyFloatA(A7,two,three);\n\
\n\
      //#define A8    (1.0 / 2.0);\n\
      DivideMyFloatA(A8,one,two);\n\
\n\
      //#define A9    (1.0 / 3.0)\n\
      DivideMyFloatA(A9,one,three);\n\
\n\
      //#define A10   (1.0 / 4.0)\n\
      DivideMyFloatA(A10,one,four);\n\
\n\
      //#define A11   (4.0 / 3.0);\n\
      DivideMyFloatA(A11,four,three);\n\
\n\
      //#define A12   (5.0 / 6.0);\n\
      DivideMyFloatA(A12,five,six);\n\
\n\
      //#define A14   (1.0 / 6.0);\n\
      DivideMyFloatA(A14,one,six);\n\
\n\
\n\
      //#define B10   (1.0 / 12.0);\n\
      DivideMyFloatA(B10,one,one2);\n\
\n\
      //#define B20   (1.0 / 27.0);\n\
      DivideMyFloatA(B20,one,two7);\n\
\n\
      //#define B21   (2.0 / 27.0);\n\
      DivideMyFloatA(B21,two,two7);\n\
\n\
      //#define B30   (1.0 / 24.0);\n\
      DivideMyFloatA(B30,one,two4);\n\
\n\
      //#define B32   (1.0 / 8.0);\n\
      DivideMyFloatA(B32,one,eight);\n\
\n\
      //#define B40   ((4.0 + 94.0 * RT6); / 375.0)\n\
      MultiplyMyFloatA(rk89_tmp1, nine4, RT6);\n\
      AddMyFloatA(rk89_tmp2, four, rk89_tmp1);\n\
      DivideMyFloatA(B40,rk89_tmp2,three75);\n\
\n\
      //#define B42   ((-94.0 - 84.0 * RT6); / 125.0)\n\
      MultiplyMyFloatA(rk89_tmp1, eight4, RT6);\n\
      AddMyFloatA(rk89_tmp2, nine4, rk89_tmp1);\n\
      DivideMyFloatA(rk89_tmp3,rk89_tmp2,one25);\n\
      NegateMyFloatA(B42,rk89_tmp3);\n\
\n\
      //#define B43   ((328.0 + 208.0 * RT6); / 375.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two08, RT6);\n\
      AddMyFloatA(rk89_tmp2, three28, rk89_tmp1);\n\
      DivideMyFloatA(B43,rk89_tmp2,three75);\n\
\n\
\n\
      //#define B50   ((9.0 - RT6); / 150.0)\n\
      SubtractMyFloatA(rk89_tmp1, nine, RT6);\n\
      DivideMyFloatA(B50,rk89_tmp1,one50);\n\
\n\
      //#define B53   ((312.0 + 32.0 * RT6); / 1425.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three2, RT6);\n\
      AddMyFloatA(rk89_tmp2, three12, rk89_tmp1);\n\
      DivideMyFloatA(B53,rk89_tmp2,one425);\n\
\n\
      //#define B54   ((69.0 + 29.0 * RT6); / 570.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two9, RT6);\n\
      AddMyFloatA(rk89_tmp2, six9, rk89_tmp1);\n\
      DivideMyFloatA(B54,rk89_tmp2,five70);\n\
\n\
\n\
      //#define B60   ((927.0 - 347.0 * RT6); / 1250.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three47, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, nine27, rk89_tmp1);\n\
      DivideMyFloatA(B60,rk89_tmp2,one250);\n\
\n\
\n\
      //#define B63   ((-16248.0 + 7328.0 * RT6); / 9375.0)\n\
      MultiplyMyFloatA(rk89_tmp1, seven328, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, one6248);\n\
      DivideMyFloatA(B63,rk89_tmp2,nine375);\n\
\n\
      //#define B64   ((-489.0 + 179.0 * RT6); / 3750.0)\n\
      MultiplyMyFloatA(rk89_tmp1, one79, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, four89);\n\
      DivideMyFloatA(B64,rk89_tmp2,three750);\n\
\n\
      //#define B65   ((14268.0 - 5798.0 * RT6); / 9375.0)\n\
      MultiplyMyFloatA(rk89_tmp1, five798, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, one4268, rk89_tmp1);\n\
      DivideMyFloatA(B65,rk89_tmp2,nine375);\n\
\n\
      //#define B70   (2.0 / 27.0);\n\
      DivideMyFloatA(B70,two,two7);\n\
\n\
      //#define B75   ((16.0 - RT6); / 54.0)\n\
      SubtractMyFloatA(rk89_tmp1, one6, RT6);\n\
      DivideMyFloatA(B75,rk89_tmp1,five4);\n\
\n\
      //#define B76   ((16.0 + RT6); / 54.0)\n\
      AddMyFloatA(rk89_tmp1, one6, RT6);\n\
      DivideMyFloatA(B76,rk89_tmp1,five4);\n\
\n\
      //#define B80   (19.0 /256.0);\n\
      DivideMyFloatA(B80,one9,two56);\n\
\n\
      //#define B85   ((118.0 - 23.0 * RT6); / 512.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two3, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, one18, rk89_tmp1);\n\
      DivideMyFloatA(B85,rk89_tmp2,five12);\n\
\n\
      //#define B86   ((118.0 + 23.0 * RT6); / 512.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two3, RT6);\n\
      AddMyFloatA(rk89_tmp2, one18, rk89_tmp1);\n\
      DivideMyFloatA(B86,rk89_tmp2,five12);\n\
\n\
      //#define B87   (-9.0 / 256.0);\n\
      DivideMyFloatA(rk89_tmp1,nine,two56);\n\
      NegateMyFloatA(B87, rk89_tmp1);\n\
\n\
      //#define B90   (11.0 / 144.0);\n\
      DivideMyFloatA(B90,one1,one44);\n\
\n\
      //#define B95   ((266.0 - RT6); / 864.0)\n\
      SubtractMyFloatA(rk89_tmp1, two66, RT6);\n\
      DivideMyFloatA(B95,rk89_tmp1,eight64);\n\
\n\
      //#define B96   ((266.0 + RT6); / 864.0)\n\
      AddMyFloatA(rk89_tmp1, two66, RT6);\n\
      DivideMyFloatA(B96,rk89_tmp1,eight64);\n\
\n\
      //#define B97   (-1.0 / 16.0);\n\
      DivideMyFloatA(rk89_tmp1,one, one6);\n\
      NegateMyFloatA(B97, rk89_tmp1);\n\
\n\
      //#define B98   (-8.0 / 27.0);\n\
      DivideMyFloatA(rk89_tmp1,eight, two7);\n\
      NegateMyFloatA(B98, rk89_tmp1);\n\
\n\
      //#define B100  ((5034.0 - 271.0 * RT6); / 61440.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two71, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, five034, rk89_tmp1);\n\
      DivideMyFloatA(B100,rk89_tmp2,six1440);\n\
\n\
      //#define B106  ((7859.0 - 1626.0 * RT6); / 10240.0)\n\
      MultiplyMyFloatA(rk89_tmp1, one626, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, seven859, rk89_tmp1);\n\
      DivideMyFloatA(B106,rk89_tmp2,one0240);\n\
\n\
\n\
      //#define B107  ((-2232.0 + 813.0 * RT6); / 20480.0)\n\
      MultiplyMyFloatA(rk89_tmp1, eight13, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1,two232);\n\
      DivideMyFloatA(B107,rk89_tmp2,two0480);\n\
\n\
      //#define B108  ((-594.0 + 271.0 * RT6); / 960.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two71, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, five94);\n\
      DivideMyFloatA(B108,rk89_tmp2, nine60);\n\
\n\
      //#define B109  ((657.0 - 813.0 * RT6); / 5120.0)\n\
      MultiplyMyFloatA(rk89_tmp1, eight13, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, six57, rk89_tmp1);\n\
      DivideMyFloatA(B109,rk89_tmp2, five120);\n\
\n\
      //#define B110  ((5996.0 - 3794.0 * RT6); / 405.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three794, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, five996, rk89_tmp1);\n\
      DivideMyFloatA(B110,rk89_tmp2, four05);\n\
\n\
      //#define B115  ((-4342.0 - 338.0 * RT6); / 9.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three38, RT6);\n\
      AddMyFloatA(rk89_tmp2, four342, rk89_tmp1);\n\
      DivideMyFloatA(rk89_tmp3,rk89_tmp2, nine);\n\
      NegateMyFloatA(B115, rk89_tmp3);\n\
\n\
      //#define B116  ((154922.0 - 40458.0 * RT6); / 135.0)\n\
      MultiplyMyFloatA(rk89_tmp1, four0458, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, one54922, rk89_tmp1);\n\
      DivideMyFloatA(B116,rk89_tmp2, one35);\n\
\n\
      //#define B117  ((-4176.0 + 3794.0 * RT6); / 45.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three794, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, four176);\n\
      DivideMyFloatA(B117,rk89_tmp2, four5);\n\
\n\
      //#define B118  ((-340864.0 + 242816.0 * RT6); / 405.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two42816, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, three40864);\n\
      DivideMyFloatA(B118,rk89_tmp2, four05);\n\
\n\
      //#define B119  ((26304.0 - 15176.0 * RT6); / 45.0)\n\
      MultiplyMyFloatA(rk89_tmp1, one5176, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, two6304, rk89_tmp1);\n\
      DivideMyFloatA(B119,rk89_tmp2, four5);\n\
\n\
      //#define B1110 (-26624.0 / 81.0);\n\
      DivideMyFloatA(rk89_tmp1, two6624, eight1);\n\
      NegateMyFloatA(B1110, rk89_tmp1);\n\
\n\
      //#define B120  ((3793.0 + 2168.0 * RT6); / 103680.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two168, RT6);\n\
      AddMyFloatA(rk89_tmp2, rk89_tmp1, three793);\n\
      DivideMyFloatA(B120,rk89_tmp2, one03680);\n\
\n\
      //#define B125  ((4042.0 + 2263.0 * RT6); / 13824.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two263, RT6);\n\
      AddMyFloatA(rk89_tmp2, rk89_tmp1, four042);\n\
      DivideMyFloatA(B125,rk89_tmp2, one3824);\n\
\n\
      //#define B126  ((-231278.0 + 40717.0 * RT6); / 69120.0)\n\
      MultiplyMyFloatA(rk89_tmp1, four0717, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, two31278);\n\
      DivideMyFloatA(B126,rk89_tmp2, six9120);\n\
\n\
      //#define B127  ((7947.0 - 2168.0 * RT6); / 11520.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two168, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, seven947,rk89_tmp1);\n\
      DivideMyFloatA(B127,rk89_tmp2, one1520);\n\
\n\
      //#define B128  ((1048.0 - 542.0 * RT6); / 405.0)\n\
      MultiplyMyFloatA(rk89_tmp1, five42, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, one048,rk89_tmp1);\n\
      DivideMyFloatA(B128,rk89_tmp2, four05);\n\
\n\
      //#define B129  ((-1383.0 + 542.0 * RT6); / 720.0)\n\
      MultiplyMyFloatA(rk89_tmp1, five42, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, one383);\n\
      DivideMyFloatA(B129,rk89_tmp2, seven20);\n\
\n\
      //#define B1210 (2624.0 / 1053.0);\n\
      DivideMyFloatA(B1210,two624, one053);\n\
\n\
      //#define B1211 (3.0 / 1664.0);\n\
      DivideMyFloatA(B1211,three, one664);\n\
\n\
      //#define B130  (-137.0 / 1296.0);\n\
      DivideMyFloatA(rk89_tmp1,one37, one296);\n\
      NegateMyFloatA(B130,rk89_tmp1);\n\
\n\
      //#define B135  ((5642.0 - 337.0 * RT6); / 864.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three37, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, five642,rk89_tmp1);\n\
      DivideMyFloatA(B135,rk89_tmp2, eight64);\n\
\n\
      // #define B136  ((5642.0 + 337.0 * RT6); / 864.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three37, RT6);\n\
      AddMyFloatA(rk89_tmp2, five642,rk89_tmp1);\n\
      DivideMyFloatA(B136,rk89_tmp2, eight64);\n\
\n\
      //#define B137  (-299.0 / 48.0);\n\
      DivideMyFloatA(rk89_tmp1, two99, four8);\n\
      NegateMyFloatA(B137,rk89_tmp1);\n\
\n\
      //#define B138  (184.0 / 81.0);\n\
      DivideMyFloatA(B138,one84, eight1);\n\
\n\
      //#define B139  (-44.0 / 9.0);\n\
      DivideMyFloatA(rk89_tmp1,four4, nine);\n\
      NegateMyFloatA(B139,rk89_tmp1);\n\
\n\
      //#define B1310 (-5120.0 / 1053.0);\n\
      DivideMyFloatA(rk89_tmp1,five120, one053);\n\
      NegateMyFloatA(B1310,rk89_tmp1);\n\
\n\
      //#define B1311 (-11.0 / 468.0);\n\
      DivideMyFloatA(rk89_tmp1,one1, four68);\n\
      NegateMyFloatA(B1311,rk89_tmp1);\n\
\n\
\n\
      //#define B1312 (16.0 / 9.0);\n\
      DivideMyFloatA(B1312,one6, nine);\n\
\n\
\n\
      //#define B140  ((33617.0 - 2168.0 * RT6); / 518400.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two168, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, three3617,rk89_tmp1);\n\
      DivideMyFloatA(B140,rk89_tmp2, five18400);\n\
\n\
      //#define B145  ((-3846.0 + 31.0 * RT6); / 13824.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three1, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, three846);\n	\
      DivideMyFloatA(B145,rk89_tmp2, one3824);\n\
\n\
      //#define B146  ((155338.0 - 52807.0 * RT6); / 345600.0)\n\
      MultiplyMyFloatA(rk89_tmp1, five2807, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, one55338,rk89_tmp1);\n\
      DivideMyFloatA(B146,rk89_tmp2, three45600);\n\
\n\
      //#define B147  ((-12537.0 + 2168.0 * RT6); / 57600.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two168, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1, one2537);\n\
      DivideMyFloatA(B147,rk89_tmp2, five7600);\n\
\n\
      //#define B148  ((92.0 + 542.0 * RT6); / 2025.0)\n\
      MultiplyMyFloatA(rk89_tmp1, five42, RT6);\n\
      AddMyFloatA(rk89_tmp2, nine2,rk89_tmp1);\n\
      DivideMyFloatA(B148,rk89_tmp2, two025);\n\
\n\
      //#define B149  ((-1797.0 - 542.0 * RT6); / 3600.0)\n\
      MultiplyMyFloatA(rk89_tmp1, five42, RT6);\n\
      AddMyFloatA(rk89_tmp2, one797,rk89_tmp1);\n\
      DivideMyFloatA(rk89_tmp3,rk89_tmp2,three600);\n\
      NegateMyFloatA(B149,rk89_tmp3);\n\
\n\
      //#define B1410 (320.0 / 567.0);\n\
      DivideMyFloatA(B1410,three20,five67);\n\
\n\
      //#define B1411 (-1.0 / 1920.0);\n\
      DivideMyFloatA(rk89_tmp1,one,one920);\n\
      NegateMyFloatA(B1411, rk89_tmp1);\n\
\n\
      //#define B1412 (4.0 / 105.0);\n\
      DivideMyFloatA(B1412,four,one05);\n\
\n\
      //#define B150  ((-36487.0 - 30352.0 * RT6); / 279600.0)\n\
      MultiplyMyFloatA(rk89_tmp1, three0352, RT6);\n\
      AddMyFloatA(rk89_tmp2, three6487,rk89_tmp1);\n\
      DivideMyFloatA(rk89_tmp3,rk89_tmp2,two79600);\n\
      NegateMyFloatA(B150,rk89_tmp3);\n\
\n\
      //#define B155  ((-29666.0 - 4499.0 * RT6); / 7456.0)\n\
      MultiplyMyFloatA(rk89_tmp1, four499, RT6);\n\
      AddMyFloatA(rk89_tmp2, two9666,rk89_tmp1);\n\
      DivideMyFloatA(rk89_tmp3,rk89_tmp2,seven456);\n\
      NegateMyFloatA(B155,rk89_tmp3);\n\
\n\
      //#define B156  ((2779182.0 - 615973.0 * RT6); / 186400.0)\n\
      MultiplyMyFloatA(rk89_tmp1, six15973, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, two779182,rk89_tmp1);\n\
      DivideMyFloatA(B156,rk89_tmp2,one86400);\n\
\n\
      //#define B157  ((-94329.0 + 91056.0 * RT6); / 93200.0)\n\
      MultiplyMyFloatA(rk89_tmp1, nine1056, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1,nine4329);\n\
      DivideMyFloatA(B157,rk89_tmp2,nine3200);\n\
\n\
      //#define B158  ((-232192.0 + 121408.0 * RT6); / 17475.0)\n\
      MultiplyMyFloatA(rk89_tmp1, one21408, RT6);\n\
      SubtractMyFloatA(rk89_tmp2, rk89_tmp1,two32192);\n\
      DivideMyFloatA(B158,rk89_tmp2,one7475);\n\
\n\
      //#define B159  ((101226.0 - 22764.0 * RT6); / 5825.0)\n\
      MultiplyMyFloatA(rk89_tmp1, two2764, RT6);\n\
      SubtractMyFloatA(rk89_tmp2,one01226,rk89_tmp1);\n\
      DivideMyFloatA(B159,rk89_tmp2,five825);\n\
\n\
      //#define B1510 (-169984.0 / 9087.0);\n\
      DivideMyFloatA(rk89_tmp1,one69984,nine087);\n\
      NegateMyFloatA(B1510,rk89_tmp1);\n\
\n\
      //#define B1511 (-87.0 / 30290.0);\n\
      DivideMyFloatA(rk89_tmp1,eight7,three0290);\n\
      NegateMyFloatA(B1511,rk89_tmp1);\n\
\n\
      //#define B1512 (492.0 / 1165.0);\n\
      DivideMyFloatA(B1512,four92,one165);\n\
\n\
      //#define B1514 (1260.0 / 233.0);\n\
      DivideMyFloatA(B1514,one260,two33);\n\
\n\
\n\
      //#define C80   (103.0 / 1680.0);\n\
      DivideMyFloatA(C80,one03,one680);\n\
\n\
      //#define C87   (-27.0 / 140.0);\n\
      DivideMyFloatA(rk89_tmp1,two7,one40);\n\
      NegateMyFloatA(C87, rk89_tmp1);\n\
\n\
      //#define C88   (76.0 / 105.0);\n\
      DivideMyFloatA(C88,seven6,one05);\n\
\n\
      //#define C89   (-201.0 / 280.0);\n\
      DivideMyFloatA(rk89_tmp1,two01,two80);\n\
      NegateMyFloatA(C89,rk89_tmp1);\n\
\n\
      //#define C810  (1024.0 / 1365.0);\n\
      DivideMyFloatA(C810,one024,one365);\n\
\n\
      //#define C811  (3.0 / 7280.0);\n\
      DivideMyFloatA(C811,three,seven280);\n\
\n\
      //#define C812  (12.0 / 35.0);\n\
      DivideMyFloatA(C812,one2,three5);\n\
\n\
      //#define C813  (9.0 / 280.0);\n\
      DivideMyFloatA(C813,nine,two80);\n\
\n\
      //#define C90   (23.0 / 525.0);\n\
      DivideMyFloatA(C90,two3,five25);\n\
\n\
      //#define C97   (171.0 / 1400.0);\n\
      DivideMyFloatA(C97,one71,one400);\n\
\n\
      //#define C98   (86.0 / 525.0);\n\
      DivideMyFloatA(C98,eight6,five25);\n\
\n\
      //#define C99   (93.0 / 280.0);\n\
      DivideMyFloatA(C99,nine3,two80);\n\
\n\
      //#define C910  (-2048.0 / 6825.0);\n\
      DivideMyFloatA(rk89_tmp1,two048,six825);\n\
      NegateMyFloatA(C910,rk89_tmp1);\n\
\n\
      //#define C911  (-3.0 / 18200.0);\n\
      DivideMyFloatA(rk89_tmp1,three,one8200);\n\
      NegateMyFloatA(C911, rk89_tmp1);\n\
\n\
      //#define C912  (39.0 / 175.0);\n\
      DivideMyFloatA(C912,three9,one75);\n\
\n\
      //#define C914  (9.0 / 25.0);\n\
      DivideMyFloatA(C914,nine,two5);\n\
\n\
      //#define C915  (233.0 / 4200.0);\n\
      DivideMyFloatA(C915,two33,four200);\n\
\n\
\n\
      //#define E0    (-7.0 / 400.0);\n\
      DivideMyFloatA(rk89_tmp1,seven,four00);\n\
      NegateMyFloatA(E0,rk89_tmp1);\n\
\n\
      //#define E7    (63.0 / 200.0);\n\
      DivideMyFloatA(E7,six3,two00);\n\
\n\
      //#define E8    (-14.0 / 25.0);\n\
      DivideMyFloatA(rk89_tmp1,one4,two5);\n\
      NegateMyFloatA(E8,rk89_tmp1);\n\
\n\
      //#define E9    (21.0 / 20.0);\n\
      DivideMyFloatA(E9,two1,two0);\n\
\n\
      //#define E10   (-1024.0 / 975.0);\n\
      DivideMyFloatA(rk89_tmp1,one024,nine75);\n\
      NegateMyFloatA(E10,rk89_tmp1);\n\
\n\
      //#define E11   (-21.0 / 36400.0);\n\
      DivideMyFloatA(rk89_tmp1,two1,three6400);\n\
      NegateMyFloatA(E11,rk89_tmp1);\n\
\n\
      //#define E12   (-3.0 / 25.0);\n\
      DivideMyFloatA(rk89_tmp1,three,two5);\n\
      NegateMyFloatA(E12,rk89_tmp1);\n\
\n\
      //#define E13   (-9.0 / 280.0);\n\
      DivideMyFloatA(rk89_tmp1,nine,two80);\n\
      NegateMyFloatA(E13,rk89_tmp1);\n\
\n\
      //#define E14   (9.0 / 25.0);\n\
      DivideMyFloatA(E14,nine,two5);\n\
\n\
      //#define E15   (233.0 / 4200.0);\n\
      DivideMyFloatA(E15,two33,four200);\n\
\n\
      rk89_constants_initialized = 1;\n\
    }\n\
  }\n\
#else\n\
  static void rk89_init_constants() {}	\n\
#define RT6   2.449489742783178098197\n\
#define A1    (1.0 / 12.0)\n\
#define A2    (1.0 / 9.0)\n\
#define A3    (1.0 / 6.0)\n\
#define A4    ((2.0 + 2.0 * RT6) / 15.0)\n\
#define A5    ((6.0 + RT6) / 15.0)\n\
#define A6    ((6.0 - RT6) / 15.0)\n\
#define A7    (2.0 / 3.0)\n\
#define A8    (1.0 / 2.0)\n\
#define A9    (1.0 / 3.0)\n\
#define A10   (1.0 / 4.0)\n\
#define A11   (4.0 / 3.0)\n\
#define A12   (5.0 / 6.0)\n\
#define A14   (1.0 / 6.0)\n\
#define B10   (1.0 / 12.0)\n\
#define B20   (1.0 / 27.0)\n\
#define B21   (2.0 / 27.0)\n\
#define B30   (1.0 / 24.0)\n\
#define B32   (1.0 / 8.0)\n\
#define B40   ((4.0 + 94.0 * RT6) / 375.0)\n\
#define B42   ((-94.0 - 84.0 * RT6) / 125.0)\n\
#define B43   ((328.0 + 208.0 * RT6) / 375.0)\n\
#define B50   ((9.0 - RT6) / 150.0)\n\
#define B53   ((312.0 + 32.0 * RT6) / 1425.0)\n\
#define B54   ((69.0 + 29.0 * RT6) / 570.0)\n\
#define B60   ((927.0 - 347.0 * RT6) / 1250.0)\n\
#define B63   ((-16248.0 + 7328.0 * RT6) / 9375.0)\n\
#define B64   ((-489.0 + 179.0 * RT6) / 3750.0)\n\
#define B65   ((14268.0 - 5798.0 * RT6) / 9375.0)\n\
#define B70   (2.0 / 27.0)\n\
#define B75   ((16.0 - RT6) / 54.0)\n\
#define B76   ((16.0 + RT6) / 54.0)\n\
#define B80   (19.0 /256.0)\n\
#define B85   ((118.0 - 23.0 * RT6) / 512.0)\n\
#define B86   ((118.0 + 23.0 * RT6) / 512.0)\n\
#define B87   (-9.0 / 256.0)\n\
#define B90   (11.0 / 144.0)\n\
#define B95   ((266.0 - RT6) / 864.0)\n\
#define B96   ((266.0 + RT6) / 864.0)\n\
#define B97   (-1.0 / 16.0)\n\
#define B98   (-8.0 / 27.0)\n\
#define B100  ((5034.0 - 271.0 * RT6) / 61440.0)\n\
#define B106  ((7859.0 - 1626.0 * RT6) / 10240.0)\n\
#define B107  ((-2232.0 + 813.0 * RT6) / 20480.0)\n\
#define B108  ((-594.0 + 271.0 * RT6) / 960.0)\n\
#define B109  ((657.0 - 813.0 * RT6) / 5120.0)\n\
#define B110  ((5996.0 - 3794.0 * RT6) / 405.0)\n\
#define B115  ((-4342.0 - 338.0 * RT6) / 9.0)\n\
#define B116  ((154922.0 - 40458.0 * RT6) / 135.0)\n\
#define B117  ((-4176.0 + 3794.0 * RT6) / 45.0)\n\
#define B118  ((-340864.0 + 242816.0 * RT6) / 405.0)\n\
#define B119  ((26304.0 - 15176.0 * RT6) / 45.0)\n\
#define B1110 (-26624.0 / 81.0)\n\
#define B120  ((3793.0 + 2168.0 * RT6) / 103680.0)\n\
#define B125  ((4042.0 + 2263.0 * RT6) / 13824.0)\n\
#define B126  ((-231278.0 + 40717.0 * RT6) / 69120.0)\n\
#define B127  ((7947.0 - 2168.0 * RT6) / 11520.0)\n\
#define B128  ((1048.0 - 542.0 * RT6) / 405.0)\n\
#define B129  ((-1383.0 + 542.0 * RT6) / 720.0)\n\
#define B1210 (2624.0 / 1053.0)\n\
#define B1211 (3.0 / 1664.0)\n\
#define B130  (-137.0 / 1296.0)\n\
#define B135  ((5642.0 - 337.0 * RT6) / 864.0)\n\
 #define B136  ((5642.0 + 337.0 * RT6) / 864.0)\n\
#define B137  (-299.0 / 48.0)\n\
#define B138  (184.0 / 81.0)\n\
#define B139  (-44.0 / 9.0)\n\
#define B1310 (-5120.0 / 1053.0)\n\
#define B1311 (-11.0 / 468.0)\n\
#define B1312 (16.0 / 9.0)\n\
#define B140  ((33617.0 - 2168.0 * RT6) / 518400.0)\n\
#define B145  ((-3846.0 + 31.0 * RT6) / 13824.0)\n\
#define B146  ((155338.0 - 52807.0 * RT6) / 345600.0)\n\
#define B147  ((-12537.0 + 2168.0 * RT6) / 57600.0)\n\
#define B148  ((92.0 + 542.0 * RT6) / 2025.0)\n\
#define B149  ((-1797.0 - 542.0 * RT6) / 3600.0)\n\
#define B1410 (320.0 / 567.0)\n\
#define B1411 (-1.0 / 1920.0)\n\
#define B1412 (4.0 / 105.0)\n\
#define B150  ((-36487.0 - 30352.0 * RT6) / 279600.0)\n\
#define B155  ((-29666.0 - 4499.0 * RT6) / 7456.0)\n\
#define B156  ((2779182.0 - 615973.0 * RT6) / 186400.0)\n\
#define B157  ((-94329.0 + 91056.0 * RT6) / 93200.0)\n\
#define B158  ((-232192.0 + 121408.0 * RT6) / 17475.0)\n\
#define B159  ((101226.0 - 22764.0 * RT6) / 5825.0)\n\
#define B1510 (-169984.0 / 9087.0)\n\
#define B1511 (-87.0 / 30290.0)\n\
#define B1512 (492.0 / 1165.0)\n\
#define B1514 (1260.0 / 233.0)\n\
#define C80   (103.0 / 1680.0)\n\
#define C87   (-27.0 / 140.0)\n\
#define C88   (76.0 / 105.0)\n\
#define C89   (-201.0 / 280.0)\n\
#define C810  (1024.0 / 1365.0)\n\
#define C811  (3.0 / 7280.0)\n\
#define C812  (12.0 / 35.0)\n\
#define C813  (9.0 / 280.0)\n\
#define C90   (23.0 / 525.0)\n\
#define C97   (171.0 / 1400.0)\n\
#define C98   (86.0 / 525.0)\n\
#define C99   (93.0 / 280.0)\n\
#define C910  (-2048.0 / 6825.0)\n\
#define C911  (-3.0 / 18200.0)\n\
#define C912  (39.0 / 175.0)\n\
#define C914  (9.0 / 25.0)\n\
#define C915  (233.0 / 4200.0)\n\
#define E0    (-7.0 / 400.0)\n\
#define E7    (63.0 / 200.0)\n\
#define E8    (-14.0 / 25.0)\n\
#define E9    (21.0 / 20.0)\n\
#define E10   (-1024.0 / 975.0)\n\
#define E11   (-21.0 / 36400.0)\n\
#define E12   (-3.0 / 25.0)\n\
#define E13   (-9.0 / 280.0)\n\
#define E14   (9.0 / 25.0)\n\
#define E15   (233.0 / 4200.0)\n\
#endif\n\
\n\
");


  
  // static inline void compute_ks(double t, double *xx_in, int n, double h,
  // double k[13][n], void (*ode)(double,double*,int,double*))  

  fprintf(outfile, "\n\
  static void compute_ks89_%s(MY_FLOAT t, MY_FLOAT *xx_in, MY_FLOAT step,  MY_FLOAT **k,\n\
			 MY_JET *jetIn, MY_JET **kjet )\n\
  {\n\
    // double ta;\n\
    static int inited;\n\
    static MY_FLOAT tmp1,tmp2,tmp3,tmp4,tmp5,ta;\n\
#pragma omp threadprivate(inited, tmp1,tmp2,tmp3,tmp4,tmp5,ta,inited)\n\
",suffix);

    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
    static MY_JET jtmp1,jtmp2,jtmp3,jtmp4,jtmp5;\n\
#pragma omp threadprivate(jtmp1,jtmp2,jtmp3,jtmp4,jtmp5)\n\
    MY_JET **jetOut;	 \n\
");
    }

    fprintf(outfile, "\n\
    MY_FLOAT **fptr;\n\
    int i, j;    \n\
\n\
    if(!inited) {\n\
      InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);InitMyFloat(tmp4);InitMyFloat(tmp5);InitMyFloat(ta);\n\
");
      if(state_jet_vars > 0) {
	fprintf(outfile, "\n\
	InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);InitJet(jtmp5);\n\
");
      }
      fprintf(outfile,"\n\
      inited = 1;\n\
    }\n\
");
    fprintf(outfile, "\n\
    //k0:\n\
    //  (*ode)(t,xx_in,n,k[0]);\n\
    ");

    if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, jetIn, &jetOut);      \n\
    for(i=0;i<%d;i++){\n\
	AssignJetToJet(kjet[0][i], jetOut[i][1]); \n\
    }\n\
", suffix,n);
    } else {
      fprintf(outfile,"\n\
    fptr = taylor_coefficients_%s_A(t, xx_in, 1, 0, NULL, NULL);      \n\
", suffix);
    }
    
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[0][i], fptr[i][1]); \n\
    }\n\
", m);

    

    fprintf(outfile, "\n\
    //k1:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[0][j]*= h;\n\
    //      k[3][j] = xx_in[j] + B10*k[0][j];\n\
    //    }\n\
    //  ta=t+A1*h;\n\
    //  (*ode)(ta,k[3],n,k[2]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[0][j], step);\n\
      AssignMyFloat(k[0][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B10);\n\
      AddMyFloatA(k[3][j], xx_in[j], tmp1);\n\
    }\n\
",m);

    if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], step);\n\
	AssignJetToJet(kjet[0][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B10);\n\
	AddJetJetA(kjet[3][j], jetIn[j], jtmp1);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A1, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
");
    if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[3], 1, 0, kjet[3], &jetOut);      \n\
      for(i=0;i<%d;i++){\n\
	AssignJetToJet(kjet[2][i], jetOut[i][1]); \n\
      }\n\
",suffix,n);
    } else {
    fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[3], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[2][i], fptr[i][1]);\n\
    }\n\
",m);

    fprintf(outfile, "\n\
    //k2:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[2][j]*= h;\n\
    //      k[3][j] = xx_in[j] + B20*k[0][j] + B21*k[2][j];\n\
    //    }\n\
    //  ta=t+A2*h;\n\
    //  (*ode)(ta,k[3],n,k[2]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[2][j], step);\n\
      AssignMyFloat(k[2][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B20);\n\
      MultiplyMyFloatA(tmp2, k[2][j], B21);\n\
      AddMyFloatA(tmp3, tmp1, tmp2);    \n\
      AddMyFloatA(k[3][j], xx_in[j], tmp3);\n\
    }\n\
",m);

    if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[2][j], step);\n\
	AssignJetToJet(kjet[2][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B20);\n\
	MultiplyJetFloatA(jtmp2, kjet[2][j], B21);\n\
	AddJetJetA(jtmp3, jtmp1, jtmp2);    \n\
	AddJetJetA(kjet[3][j], jetIn[j], jtmp3);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A2, step);  \n\
    AddMyFloatA(ta, t, tmp1);\n\
");
    if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[3], 1, 0, kjet[3], &jetOut);      \n\
      for(i=0;i<%d;i++){\n\
	AssignJetToJet(kjet[2][i], jetOut[i][1]); \n\
      }\n\
",suffix,n);
    } else {
    fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[3], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[2][i], fptr[i][1]);\n\
    }\n\
",m);
    

    fprintf(outfile, "\n\
    //k3:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[2][j]*= h;\n\
    //      k[4][j] = xx_in[j] + B30*k[0][j] + B32*k[2][j];\n\
    //    }\n\
    //  ta=t+A3*h;\n\
    //  (*ode)(ta,k[4],n,k[3]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[2][j], step);\n\
      AssignMyFloat(k[2][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B30);\n\
      MultiplyMyFloatA(tmp2, k[2][j], B32);\n\
      AddMyFloatA(tmp3, tmp1, tmp2);    \n\
      AddMyFloatA(k[4][j], xx_in[j], tmp3);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[2][j], step);\n\
	AssignJetToJet(kjet[2][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B30);\n\
	MultiplyJetFloatA(jtmp2, kjet[2][j], B32);\n\
	AddJetJetA(jtmp3, jtmp1, jtmp2);    \n\
	AddJetJetA(kjet[4][j], jetIn[j], jtmp3);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A3, step);  \n\
    AddMyFloatA(ta, t, tmp1);    \n\
");
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[4], 1, 0, kjet[4], &jetOut);      \n\
      for(i=0;i<%d;i++){      \n\
	AssignJetToJet(kjet[3][i], jetOut[i][1]);\n\
      }\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[4], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[3][i], fptr[i][1]);\n\
    }\n\
",m);
	    
  

    fprintf(outfile, "\n\
    //k4:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[3][j]*= h;\n\
    //      k[5][j] = xx_in[j] + B40*k[0][j] + B42*k[2][j] + B43*k[3][j];\n\
    //    }\n\
    //  ta=t+A4*h;\n\
    //  (*ode)(ta,k[5],n,k[4]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[3][j], step);\n\
      AssignMyFloat(k[3][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B40);\n\
      MultiplyMyFloatA(tmp2, k[2][j], B42);\n\
      MultiplyMyFloatA(tmp3, k[3][j], B43);    \n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);        \n\
      AddMyFloatA(k[5][j], xx_in[j], tmp1);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[3][j], step);\n\
	AssignJetToJet(kjet[3][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B40);\n\
	MultiplyJetFloatA(jtmp2, kjet[2][j], B42);\n\
	MultiplyJetFloatA(jtmp3, kjet[3][j], B43);    \n\
	AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
	AddJetJetA(jtmp1, jtmp3, jtmp4);        \n\
	AddJetJetA(kjet[5][j], jetIn[j], jtmp1);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A4, step);  \n\
    AddMyFloatA(ta, t, tmp1);    \n\
");
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[5], 1, 0, kjet[5], &jetOut);      \n\
      for(i=0;i<%d;i++){      \n\
	AssignJetToJet(kjet[4][i], jetOut[i][1]);\n\
      }\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[5], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[4][i], fptr[i][1]);\n\
    }  \n\
",m);
    

    fprintf(outfile, "\n\
    //k5:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[4][j]*= h;\n\
    //      k[6][j] = xx_in[j] + B50*k[0][j] + B53*k[3][j] + B54*k[4][j];\n\
    //    }\n\
    //  ta=t+A5*h;\n\
    //  (*ode)(ta,k[6],n,k[5]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[4][j], step);\n\
      AssignMyFloat(k[4][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B50);\n\
      MultiplyMyFloatA(tmp2, k[3][j], B53);\n\
      MultiplyMyFloatA(tmp3, k[4][j], B54);    \n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);        \n\
      AddMyFloatA(k[6][j], xx_in[j], tmp1);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[4][j], step);\n\
	AssignJetToJet(kjet[4][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B50);\n\
	MultiplyJetFloatA(jtmp2, kjet[3][j], B53);\n\
	MultiplyJetFloatA(jtmp3, kjet[4][j], B54);    \n\
	AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
	AddJetJetA(jtmp1, jtmp3, jtmp4);        \n\
	AddJetJetA(kjet[6][j], jetIn[j], jtmp1);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A5, step);  \n\
    AddMyFloatA(ta, t, tmp1);    \n\
");
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[6], 1, 0, kjet[6], &jetOut);      \n\
      for(i=0;i<%d;i++){      \n\
	AssignJetToJet(kjet[5][i], jetOut[i][1]);\n\
      }\n\
",suffix,n);
    }  else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[6], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[5][i], fptr[i][1]);\n\
    }  \n\
",m);	     
  

    fprintf(outfile, "\n\
    //k6:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[5][j]*= h;\n\
    //      k[7][j] = xx_in[j] + B60*k[0][j] + B63*k[3][j] + B64*k[4][j] + B65*k[5][j];\n\
    //    }\n\
    //  ta=t+A6*h;\n\
    //  (*ode)(ta,k[7],n,k[6]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[5][j], step);\n\
      AssignMyFloat(k[5][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B60);\n\
      MultiplyMyFloatA(tmp2, k[3][j], B63);\n\
      MultiplyMyFloatA(tmp3, k[4][j], B64);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp2, tmp3, tmp4);            \n\
      MultiplyMyFloatA(tmp3, k[5][j], B65);    \n\
      AddMyFloatA(tmp1, tmp2, tmp3);            \n\
      AddMyFloatA(k[7][j], xx_in[j], tmp1);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[5][j], step);\n\
	AssignJetToJet(kjet[5][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B60);\n\
	MultiplyJetFloatA(jtmp2, kjet[3][j], B63);\n\
	MultiplyJetFloatA(jtmp3, kjet[4][j], B64);\n\
	AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
	AddJetJetA(jtmp2, jtmp3, jtmp4);            \n\
	MultiplyJetFloatA(jtmp3, kjet[5][j], B65);    \n\
	AddJetJetA(jtmp1, jtmp2, jtmp3);            \n\
	AddJetJetA(kjet[7][j], jetIn[j], jtmp1);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A6, step);  \n\
    AddMyFloatA(ta, t, tmp1);    \n\
");
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[7], 1, 0, kjet[7], &jetOut);      \n\
      for(i=0;i<%d;i++){      \n\
	AssignJetToJet(kjet[6][i], jetOut[i][1]);\n\
      }\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[7], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[6][i], fptr[i][1]);\n\
    }\n\
",m);
    

    fprintf(outfile, "\n\
    //k7:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[6][j]*= h;\n\
    //      k[8][j] = xx_in[j] + B70*k[0][j] + B75*k[5][j] + B76*k[6][j];\n\
    //    }\n\
    //  ta=t+A7*h;\n\
    //  (*ode)(ta,k[8],n,k[7]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[6][j], step);\n\
      AssignMyFloat(k[6][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B70);\n\
      MultiplyMyFloatA(tmp2, k[5][j], B75);\n\
      MultiplyMyFloatA(tmp3, k[6][j], B76);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);            \n\
      AddMyFloatA(k[8][j], xx_in[j], tmp1);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[6][j], step);\n\
	AssignJetToJet(kjet[6][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B70);\n\
	MultiplyJetFloatA(jtmp2, kjet[5][j], B75);\n\
	MultiplyJetFloatA(jtmp3, kjet[6][j], B76);\n\
	AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
	AddJetJetA(jtmp1, jtmp3, jtmp4);            \n\
	AddJetJetA(kjet[8][j], jetIn[j], jtmp1);\n\
      }      \n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A7, step);  \n\
    AddMyFloatA(ta, t, tmp1);    \n\
");
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[8], 1, 0, kjet[8], &jetOut);      \n\
      for(i=0;i<%d;i++){      \n\
	AssignJetToJet(kjet[7][i], jetOut[i][1]);\n\
      }\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[8], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[7][i], fptr[i][1]);\n\
    }  \n\
",m);
  

fprintf(outfile, "\n\
    //k8:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[7][j]*= h;\n\
    //      k[9][j] = xx_in[j] + B80*k[0][j] + B85*k[5][j] + B86*k[6][j] + B87*k[7][j];\n\
    //    }\n\
    //  ta=t+A8*h;\n\
    //  (*ode)(ta,k[9],n,k[8]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[7][j], step);\n\
      AssignMyFloat(k[7][j],tmp1);\n\
      \n\
      MultiplyMyFloatA(tmp1, k[0][j], B80);\n\
      MultiplyMyFloatA(tmp2, k[5][j], B85);\n\
      MultiplyMyFloatA(tmp3, k[6][j], B86);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp2, tmp3, tmp4);\n\
      MultiplyMyFloatA(tmp3, k[7][j], B87);    \n\
      AddMyFloatA(tmp1, tmp2, tmp3);                \n\
      AddMyFloatA(k[9][j], xx_in[j], tmp1);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[7][j], step);\n\
	AssignJetToJet(kjet[7][j],jtmp1);\n\
      \n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B80);\n\
	MultiplyJetFloatA(jtmp2, kjet[5][j], B85);\n\
	MultiplyJetFloatA(jtmp3, kjet[6][j], B86);\n\
	AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
	AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
	MultiplyJetFloatA(jtmp3, kjet[7][j], B87);    \n\
	AddJetJetA(jtmp1, jtmp2, jtmp3);                \n\
	AddJetJetA(kjet[9][j], jetIn[j], jtmp1);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
    MultiplyMyFloatA(tmp1, A8, step);  \n\
    AddMyFloatA(ta, t, tmp1);    \n\
");
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[9], 1, 0, kjet[9], &jetOut);      \n\
      for(i=0;i<%d;i++){      \n\
	AssignJetToJet(kjet[8][i], jetOut[i][1]);\n\
      }\n\
",suffix,n);
    } else {
      fprintf(outfile, "\n\
      fptr = taylor_coefficients_%s_A(ta, k[9], 1, 0, NULL, NULL);\n\
",suffix);
    }
    fprintf(outfile, "\n\
    for(i=0;i<%d;i++){\n\
      AssignMyFloat(k[8][i], fptr[i][1]);\n\
    }  \n\
",m);
    

    fprintf(outfile, "\n\
    //k9:\n\
    //  for (j = 0; j < n; ++j)\n\
    //    {\n\
    //      k[8][j]*= h;\n\
    //      k[10][j] = xx_in[j] + B90*k[0][j] + B95*k[5][j] + B96*k[6][j] + B97*k[7][j] + B98*k[8][j];\n\
    //    }\n\
    //  ta=t+A9*h;\n\
    //  (*ode)(ta,k[10],n,k[9]);\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyMyFloatA(tmp1, k[8][j], step);\n\
      AssignMyFloat(k[8][j],tmp1);\n\
\n\
      MultiplyMyFloatA(tmp1, k[0][j], B90);\n\
      MultiplyMyFloatA(tmp2, k[5][j], B95);\n\
      MultiplyMyFloatA(tmp3, k[6][j], B96);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
      MultiplyMyFloatA(tmp3, k[7][j], B97);\n\
      MultiplyMyFloatA(tmp4, k[8][j], B98);        \n\
      AddMyFloatA(tmp5, tmp3, tmp4);\n\
      AddMyFloatA(tmp1, tmp2, tmp5);                    \n\
      AddMyFloatA(k[10][j], xx_in[j], tmp1);\n\
    }\n\
",m);
    if(state_jet_vars > 0) {
      fprintf(outfile, "\n\
      for (j = 0; j < %d; ++j) {\n\
	MultiplyJetFloatA(jtmp1, kjet[8][j], step);\n\
	AssignJetToJet(kjet[8][j],jtmp1);\n\
\n\
	MultiplyJetFloatA(jtmp1, kjet[0][j], B90);\n\
	MultiplyJetFloatA(jtmp2, kjet[5][j], B95);\n\
	MultiplyJetFloatA(jtmp3, kjet[6][j], B96);\n\
	AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
	AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
	MultiplyJetFloatA(jtmp3, kjet[7][j], B97);\n\
	MultiplyJetFloatA(jtmp4, kjet[8][j], B98);        \n\
	AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
	AddJetJetA(jtmp1, jtmp2, jtmp5);                    \n\
	AddJetJetA(kjet[10][j], jetIn[j], jtmp1);\n\
      }\n\
",n);
    }
    fprintf(outfile, "\n\
  MultiplyMyFloatA(tmp1, A9, step);  \n\
  AddMyFloatA(ta, t, tmp1);    \n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[10], 1, 0, kjet[10], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[9][i], jetOut[i][1]);\n\
    }\n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[10], 1, 0, NULL, NULL);\n\
",suffix);
  }
  fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[9][i], fptr[i][1]);\n\
  }  \n\
",m);
  
  
  fprintf(outfile, "\n\
  //k10:\n\
  //  for (j = 0; j < n; ++j)\n\
  //    {\n\
  //      k[9][j]*= h;\n\
  //      k[11][j] = xx_in[j] + B100*k[0][j] + B106*k[6][j] + B107*k[7][j] + B108*k[8][j] + B109*k[9][j];\n\
  //    }\n\
  //  ta=t+A10*h;\n\
  //  (*ode)(ta,k[11],n,k[10]);\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[9][j], step);\n\
    AssignMyFloat(k[9][j],tmp1);\n\
    \n\
    MultiplyMyFloatA(tmp1, k[0][j], B100);\n\
    MultiplyMyFloatA(tmp2, k[6][j], B106);\n\
    MultiplyMyFloatA(tmp3, k[7][j], B107);\n\
    AddMyFloatA(tmp4, tmp1, tmp2);\n\
    AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
    MultiplyMyFloatA(tmp3, k[8][j], B108);\n\
    MultiplyMyFloatA(tmp4, k[9][j], B109);        \n\
    AddMyFloatA(tmp5, tmp3, tmp4);\n\
    AddMyFloatA(tmp1, tmp2, tmp5);                    \n\
    AddMyFloatA(k[11][j], xx_in[j], tmp1);\n\
  }\n\
",m);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyJetFloatA(jtmp1, kjet[9][j], step);\n\
      AssignJetToJet(kjet[9][j],jtmp1);\n\
    \n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], B100);\n\
      MultiplyJetFloatA(jtmp2, kjet[6][j], B106);\n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], B107);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
      MultiplyJetFloatA(jtmp3, kjet[8][j], B108);\n\
      MultiplyJetFloatA(jtmp4, kjet[9][j], B109);        \n\
      AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp5);                    \n\
      AddJetJetA(kjet[11][j], jetIn[j], jtmp1);\n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  MultiplyMyFloatA(tmp1, A10, step);  \n\
  AddMyFloatA(ta, t, tmp1);    \n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[11], 1, 0, kjet[11], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[10][i], jetOut[i][1]);\n\
    }\n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[11], 1, 0, NULL, NULL);\n\
",suffix);
  }
  fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[10][i], fptr[i][1]);\n\
  }  \n\
",m);



  fprintf(outfile, "\n\
  //k11:\n\
  //  for (j = 0; j < n; ++j)\n\
  //    {\n\
  //      k[10][j]*= h;\n\
  //      k[12][j] = xx_in[j] + B110*k[0][j] + B115*k[5][j] + B116*k[6][j] + B117*k[7][j] + B118*k[8][j] + B119*k[9][j] + B1110*k[10][j];\n\
  //    }\n\
  //  ta=t+A11*h;\n\
  //  (*ode)(ta,k[12],n,k[11]);\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[10][j], step);\n\
    AssignMyFloat(k[10][j],tmp1);\n\
\n\
    MultiplyMyFloatA(tmp1, k[0][j], B110);\n\
    MultiplyMyFloatA(tmp2, k[5][j], B115);\n\
    MultiplyMyFloatA(tmp3, k[6][j], B116);\n\
    AddMyFloatA(tmp4, tmp1, tmp2);\n\
    AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
    MultiplyMyFloatA(tmp3, k[7][j], B117);\n\
    MultiplyMyFloatA(tmp4, k[8][j], B118);        \n\
    AddMyFloatA(tmp5, tmp3, tmp4);\n\
    MultiplyMyFloatA(tmp3, k[9][j], B119);\n\
    MultiplyMyFloatA(tmp4, k[10][j], B1110);        \n\
    AddMyFloatA(tmp1, tmp3, tmp4);\n\
    AddMyFloatA(tmp3, tmp1, tmp5);    \n\
    \n\
    AddMyFloatA(tmp1, tmp2, tmp3);                    \n\
    AddMyFloatA(k[12][j], xx_in[j], tmp1);\n\
  }\n\
",m);
 
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyJetFloatA(jtmp1, kjet[10][j], step);\n\
      AssignJetToJet(kjet[10][j],jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], B110);\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], B115);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], B116);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], B117);\n\
      MultiplyJetFloatA(jtmp4, kjet[8][j], B118);        \n\
      AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
      MultiplyJetFloatA(jtmp3, kjet[9][j], B119);\n\
      MultiplyJetFloatA(jtmp4, kjet[10][j], B1110);        \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
      AddJetJetA(jtmp3, jtmp1, jtmp5);    \n\
    \n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);                    \n\
      AddJetJetA(kjet[12][j], jetIn[j], jtmp1);\n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  MultiplyMyFloatA(tmp1, A11, step);  \n\
  AddMyFloatA(ta, t, tmp1);    \n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[12], 1, 0, kjet[12], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[11][i], jetOut[i][1]);\n\
    }    \n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[12], 1, 0, NULL, NULL);    \n\
",suffix);
  }
  fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[11][i], fptr[i][1]);\n\
  }  \n\
",m);


  fprintf(outfile, "\n\
  //k12:\n\
  //  for (j = 0; j < n; ++j)\n\
  //    {\n\
  //      k[11][j]*= h;\n\
  //      k[3][j] = xx_in[j] + B120*k[0][j] + B125*k[5][j] + B126*k[6][j] + B127*k[7][j] + B128*k[8][j] + B129*k[9][j] + B1210*k[10][j] + B1211*k[11][j];\n\
  //    }\n\
  //  ta=t+A12*h;\n\
  //  (*ode)(ta,k[3],n,k[12]);\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[11][j], step);\n\
    AssignMyFloat(k[11][j],tmp1);\n\
\n\
    MultiplyMyFloatA(tmp1, k[0][j], B120);\n\
    MultiplyMyFloatA(tmp2, k[5][j], B125);\n\
    MultiplyMyFloatA(tmp3, k[6][j], B126);\n\
    AddMyFloatA(tmp4, tmp1, tmp2);\n\
    AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
    MultiplyMyFloatA(tmp3, k[7][j], B127);\n\
    MultiplyMyFloatA(tmp4, k[8][j], B128);        \n\
    AddMyFloatA(tmp5, tmp3, tmp4);\n\
    MultiplyMyFloatA(tmp3, k[9][j], B129);\n\
    MultiplyMyFloatA(tmp4, k[10][j], B1210);        \n\
    AddMyFloatA(tmp1, tmp3, tmp4);\n\
    AddMyFloatA(tmp4, tmp1, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[11][j], B1211);            \n\
    AddMyFloatA(tmp3, tmp4, tmp5);\n\
\n\
    AddMyFloatA(tmp1, tmp2, tmp3);                    \n\
    AddMyFloatA(k[3][j], xx_in[j], tmp1);\n\
  }\n\
",m);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyJetFloatA(jtmp1, kjet[11][j], step);\n\
      AssignJetToJet(kjet[11][j],jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], B120);\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], B125);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], B126);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], B127);\n\
      MultiplyJetFloatA(jtmp4, kjet[8][j], B128);        \n\
      AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
      MultiplyJetFloatA(jtmp3, kjet[9][j], B129);\n\
      MultiplyJetFloatA(jtmp4, kjet[10][j], B1210);        \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[11][j], B1211);            \n\
      AddJetJetA(jtmp3, jtmp4, jtmp5);\n\
\n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);                    \n\
      AddJetJetA(kjet[3][j], jetIn[j], jtmp1);\n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  MultiplyMyFloatA(tmp1, A12, step);  \n\
  AddMyFloatA(ta, t, tmp1);    \n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[3], 1, 0, kjet[3], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[12][i], jetOut[i][1]);\n\
    }\n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[3], 1, 0, NULL, NULL);\n\
",suffix);
  }
  fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[12][i], fptr[i][1]);\n\
  }\n\
",m);

fprintf(outfile, "\n\
  //k13:\n\
  //  for (j = 0; j < n; ++j)\n\
  //    {\n\
  //      k[12][j]*= h;\n\
  //      k[2][j] = xx_in[j] + B130*k[0][j] + B135*k[5][j] + B136*k[6][j] + B137*k[7][j] + B138*k[8][j] + B139*k[9][j] + B1310*k[10][j] + B1311*k[11][j] + B1312*k[12][j];\n\
  //    }\n\
  //  ta=t+h;\n\
  //  (*ode)(ta,k[2],n,k[3]);\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[12][j], step);\n\
    AssignMyFloat(k[12][j],tmp1);\n\
\n\
    MultiplyMyFloatA(tmp1, k[0][j], B130);\n\
    MultiplyMyFloatA(tmp2, k[5][j], B135);\n\
    MultiplyMyFloatA(tmp3, k[6][j], B136);\n\
    AddMyFloatA(tmp4, tmp1, tmp2);\n\
    AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
    MultiplyMyFloatA(tmp3, k[7][j], B137);\n\
    MultiplyMyFloatA(tmp4, k[8][j], B138);        \n\
    AddMyFloatA(tmp5, tmp3, tmp4);\n\
    MultiplyMyFloatA(tmp3, k[9][j], B139);\n\
    MultiplyMyFloatA(tmp4, k[10][j], B1310);        \n\
    AddMyFloatA(tmp1, tmp3, tmp4);\n\
    AddMyFloatA(tmp4, tmp1, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[11][j], B1311);            \n\
    AddMyFloatA(tmp1, tmp4, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[12][j], B1312);                \n\
    AddMyFloatA(tmp3, tmp1, tmp5);\n\
    \n\
    AddMyFloatA(tmp1, tmp2, tmp3);                    \n\
    AddMyFloatA(k[2][j], xx_in[j], tmp1);\n\
  }\n\
",m);
 
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyJetFloatA(jtmp1, kjet[12][j], step);\n\
      AssignJetToJet(kjet[12][j],jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], B130);\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], B135);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], B136);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], B137);\n\
      MultiplyJetFloatA(jtmp4, kjet[8][j], B138);        \n\
      AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
      MultiplyJetFloatA(jtmp3, kjet[9][j], B139);\n\
      MultiplyJetFloatA(jtmp4, kjet[10][j], B1310);        \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[11][j], B1311);            \n\
      AddJetJetA(jtmp1, jtmp4, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[12][j], B1312);                \n\
      AddJetJetA(jtmp3, jtmp1, jtmp5);\n\
    \n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);                    \n\
      AddJetJetA(kjet[2][j], jetIn[j], jtmp1);\n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  AddMyFloatA(ta, t, step);\n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[2], 1, 0, kjet[2], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[3][i], jetOut[i][1]);\n\
    }\n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[2], 1, 0, NULL, NULL);\n\
",suffix);
  }
  fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[3][i], fptr[i][1]);\n\
  }  \n\
",m);    


  fprintf(outfile, "\n\
  //k14:\n\
  //  for (j = 0; j < n; ++j)\n\
  //    {\n\
  //      k[3][j]*= h;\n\
  //      k[2][j] = xx_in[j] + B140*k[0][j] + B145*k[5][j] + B146*k[6][j] + B147*k[7][j] + B148*k[8][j] + B149*k[9][j] + B1410*k[10][j] + B1411*k[11][j] + B1412*k[12][j];\n\
  //    }\n\
  //  ta=t+A14*h;\n\
  //  (*ode)(ta,k[2],n,k[4]);\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[3][j], step);\n\
    AssignMyFloat(k[3][j],tmp1);\n\
\n\
    MultiplyMyFloatA(tmp1, k[0][j], B140);\n\
    MultiplyMyFloatA(tmp2, k[5][j], B145);\n\
    MultiplyMyFloatA(tmp3, k[6][j], B146);\n\
    AddMyFloatA(tmp4, tmp1, tmp2);\n\
    AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
    MultiplyMyFloatA(tmp3, k[7][j], B147);\n\
    MultiplyMyFloatA(tmp4, k[8][j], B148);        \n\
    AddMyFloatA(tmp5, tmp3, tmp4);\n\
    MultiplyMyFloatA(tmp3, k[9][j], B149);\n\
    MultiplyMyFloatA(tmp4, k[10][j], B1410);        \n\
    AddMyFloatA(tmp1, tmp3, tmp4);\n\
    AddMyFloatA(tmp4, tmp1, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[11][j], B1411);            \n\
    AddMyFloatA(tmp1, tmp4, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[12][j], B1412);                \n\
    AddMyFloatA(tmp3, tmp1, tmp5);\n\
    \n\
    AddMyFloatA(tmp1, tmp2, tmp3);                    \n\
    AddMyFloatA(k[2][j], xx_in[j], tmp1);\n\
  }\n\
",m);

  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyJetFloatA(jtmp1, kjet[3][j], step);\n\
      AssignJetToJet(kjet[3][j],jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], B140);\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], B145);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], B146);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], B147);\n\
      MultiplyJetFloatA(jtmp4, kjet[8][j], B148);        \n\
      AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
      MultiplyJetFloatA(jtmp3, kjet[9][j], B149);\n\
      MultiplyJetFloatA(jtmp4, kjet[10][j], B1410);        \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[11][j], B1411);            \n\
      AddJetJetA(jtmp1, jtmp4, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[12][j], B1412);                \n\
      AddJetJetA(jtmp3, jtmp1, jtmp5);\n\
    \n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);                    \n\
      AddJetJetA(kjet[2][j], jetIn[j], jtmp1);\n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  MultiplyMyFloatA(tmp1, A14, step);  \n\
  AddMyFloatA(ta, t, tmp1);\n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[2], 1, 0, kjet[2], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[4][i], jetOut[i][1]);\n\
    }\n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[2], 1, 0, NULL, NULL);\n\
",suffix);
  }
  fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[4][i], fptr[i][1]);\n\
  }  \n\
",m);


fprintf(outfile, "\n\
  //k15:\n\
  //  for (j = 0; j < n; ++j)\n\
  //    {\n\
  //      k[4][j]*= h;\n\
  //      k[5][j] = xx_in[j] + B150*k[0][j] + B1514*k[4][j] + B155*k[5][j] + B156*k[6][j] + B157*k[7][j] + B158*k[8][j] + B159*k[9][j] + B1510*k[10][j] + B1511*k[11][j] + B1512*k[12][j];\n\
  //    }\n\
  //  ta=t+h;\n\
  //  (*ode)(ta,k[5],n,k[6]);\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[4][j], step);\n\
    AssignMyFloat(k[4][j],tmp1);\n\
\n\
    MultiplyMyFloatA(tmp1, k[0][j], B150);\n\
    MultiplyMyFloatA(tmp2, k[5][j], B155);\n\
    MultiplyMyFloatA(tmp3, k[6][j], B156);\n\
    AddMyFloatA(tmp4, tmp1, tmp2);\n\
    AddMyFloatA(tmp2, tmp3, tmp4);\n\
    \n\
    MultiplyMyFloatA(tmp3, k[7][j], B157);\n\
    MultiplyMyFloatA(tmp4, k[8][j], B158);        \n\
    AddMyFloatA(tmp5, tmp3, tmp4);\n\
    MultiplyMyFloatA(tmp3, k[9][j], B159);\n\
    MultiplyMyFloatA(tmp4, k[10][j], B1510);        \n\
    AddMyFloatA(tmp1, tmp3, tmp4);\n\
    AddMyFloatA(tmp4, tmp1, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[11][j], B1511);            \n\
    AddMyFloatA(tmp1, tmp4, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[12][j], B1512);                \n\
    AddMyFloatA(tmp4, tmp1, tmp5);\n\
    MultiplyMyFloatA(tmp5, k[4][j], B1514);                \n\
    AddMyFloatA(tmp3, tmp4, tmp5);\n\
    \n\
    AddMyFloatA(tmp1, tmp2, tmp3);                    \n\
    AddMyFloatA(k[5][j], xx_in[j], tmp1);\n\
  }\n\
",m);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for (j = 0; j < %d; ++j) {\n\
      MultiplyJetFloatA(jtmp1, kjet[4][j], step);\n\
      AssignJetToJet(kjet[4][j],jtmp1);\n\
\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], B150);\n\
      MultiplyJetFloatA(jtmp2, kjet[5][j], B155);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], B156);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
    \n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], B157);\n\
      MultiplyJetFloatA(jtmp4, kjet[8][j], B158);        \n\
      AddJetJetA(jtmp5, jtmp3, jtmp4);\n\
      MultiplyJetFloatA(jtmp3, kjet[9][j], B159);\n\
      MultiplyJetFloatA(jtmp4, kjet[10][j], B1510);        \n\
      AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[11][j], B1511);            \n\
      AddJetJetA(jtmp1, jtmp4, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[12][j], B1512);                \n\
      AddJetJetA(jtmp4, jtmp1, jtmp5);\n\
      MultiplyJetFloatA(jtmp5, kjet[4][j], B1514);                \n\
      AddJetJetA(jtmp3, jtmp4, jtmp5);\n\
    \n\
      AddJetJetA(jtmp1, jtmp2, jtmp3);                    \n\
      AddJetJetA(kjet[5][j], jetIn[j], jtmp1);\n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  AddMyFloatA(ta, t, step);    \n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[5], 1, 0, kjet[5], &jetOut);    \n\
    for(i=0;i<%d;i++){      \n\
      AssignJetToJet(kjet[6][i], jetOut[i][1]);\n\
    }\n\
",suffix,n);
  } else {
    fprintf(outfile, "\n\
    fptr = taylor_coefficients_%s_A(ta, k[5], 1, 0, NULL, NULL);\n\
",suffix);
  }
fprintf(outfile, "\n\
  for(i=0;i<%d;i++){\n\
    AssignMyFloat(k[6][i], fptr[i][1]);\n\
  }  \n\
",m);


 fprintf(outfile, "\n\
  //k16:\n\
  //  for (j = 0; j < n; ++j) k[6][j]*= h;\n\
  for (j = 0; j < %d; ++j) {\n\
    MultiplyMyFloatA(tmp1, k[6][j], step);\n\
    AssignMyFloat(k[6][j],tmp1);\n\
  }\n\
",m);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
    for(j=0;j<%d;j++){\n\
      MultiplyJetFloatA(jtmp1, kjet[6][j], step);\n\
      AssignJetToJet(kjet[6][j],jtmp1);      \n\
    }\n\
",n);
  }
  fprintf(outfile, "\n\
  return;\n\
  }\n\
");  

  fprintf(outfile, "\n\
  void step_rk8_89_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n	\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4, initialized)\n\
  ", suffix);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
  static MY_JET jtmp1, jtmp2, jtmp3, jtmp4;\n\
#pragma omp threadprivate(jtmp1, jtmp2, jtmp3, jtmp4)\n\
");
  }
  fprintf(outfile, "\n\
\n\
  int j;\n\
\n\
  if(initialized == 0) {\n\
    InitMyFloat(nr); InitMyFloat(jnr); InitMyFloat(NN);\n\
    InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
    MakeMyFloatC(NN, \"%d\", (double)%d); //m    \n\
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
  // xx_in[j]+= C80*k[0][j] + C813*k[3][j] + C87*k[7][j] + C88*k[8][j] + C89*k[9][j] + C810*k[10][j] + C811*k[11][j] + C812*k[12][j];\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], C80);\n\
      MultiplyMyFloatA(tmp2, k[3][j], C813);\n\
      MultiplyMyFloatA(tmp3, k[7][j], C87);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp2, tmp3, tmp4);\n\
      MultiplyMyFloatA(tmp4, k[8][j], C88);\n\
      AddMyFloatA(tmp1, tmp2, tmp4); //1 \n\
      MultiplyMyFloatA(tmp2, k[9][j], C89);\n\
      MultiplyMyFloatA(tmp3, k[10][j], C810);\n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      AddMyFloatA(tmp2, tmp1, tmp4);\n\
      MultiplyMyFloatA(tmp1, k[11][j], C811);\n\
      MultiplyMyFloatA(tmp3, k[12][j], C812);\n\
      AddMyFloatA(tmp4, tmp1, tmp3);\n\
      AddMyFloatA(tmp1, tmp2, tmp4);\n\
      AddMyFloatA(tmp2, tmp1, xx_in[j]);\n\
      AssignMyFloat(xx_in[j], tmp2);\n\
\n\
      MultiplyMyFloatA(tmp3, tmp2, tmp2);\n\
      AddMyFloatA(tmp1, nr, tmp3);\n\
      AssignMyFloat(nr, tmp1);      \n\
  }\n\
",m);
  if(state_jet_vars > 0) {
  fprintf(outfile, "\n\
  if(kjet)\n\
    for(j = 0; j < %d; ++j) { //n\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], C80);\n\
      MultiplyJetFloatA(jtmp2, kjet[3][j], C813);\n\
      MultiplyJetFloatA(jtmp3, kjet[7][j], C87);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp3, jtmp4);\n\
      MultiplyJetFloatA(jtmp4, kjet[8][j], C88);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp4); //1 \n\
      MultiplyJetFloatA(jtmp2, kjet[9][j], C89);\n\
      MultiplyJetFloatA(jtmp3, kjet[10][j], C810);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp1, jtmp4);\n\
      MultiplyJetFloatA(jtmp1, kjet[11][j], C811);\n\
      MultiplyJetFloatA(jtmp3, kjet[12][j], C812);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp3);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp4);\n\
      AddJetJetA(jtmp2, jtmp1, jetIn[j]);\n\
      AssignJetToJet(jetIn[j], jtmp2);\n\
\n\
      xNormJet(tmp1,jetIn[j]);\n\
      AddMyFloatA(jnr, tmp1, jnr);\n\
    }\n\
",n);
  }

  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
     if(MyFloatA_GE_B(jnr,nr)) { AssignMyFloat(nr, jnr);}\n\
\n\
");
  }
  fprintf(outfile, "\n\
  DivideMyFloatA(tmp1, nr, NN);\n\
  sqrtMyFloatA( *nr_ret, tmp1);\n\
}\n\
");
  
  fprintf(outfile, "\n\
 static void step_rk9_%s(MY_FLOAT *xx_in, MY_FLOAT **k, MY_JET *jetIn, MY_JET **kjet, MY_FLOAT *nr_ret ) {\n\
\n\
  static int initialized=0;\n\
  static MY_FLOAT nr,jnr, NN, tmp1, tmp2, tmp3, tmp4;\n\
#pragma omp threadprivate(initialized,nr,jnr, NN, tmp1, tmp2, tmp3, tmp4, initialized)\n\
  int j;\n\
  ", suffix);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
   static MY_JET jtmp1, jtmp2, jtmp3, jtmp4;\n\
#pragma omp threadprivate(jtmp1, jtmp2, jtmp3, jtmp4)\n\
");
  }
  fprintf(outfile, "\n\
\n\
  if(initialized == 0) {\n\
    InitMyFloat(nr); InitMyFloat(jnr); InitMyFloat(NN);\n					\
    InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
    MakeMyFloatC(NN, \"%d\", (double)%d); //m    \n\
",m,m);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
	InitJet(jtmp1);InitJet(jtmp2);InitJet(jtmp3);InitJet(jtmp4);\n	\
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
  //  xx_in[j]+= C90*k[0][j] + C914*k[4][j] + C915*k[6][j] + C97*k[7][j] + C98*k[8][j] + C99*k[9][j] + C910*k[10][j] + C911*k[11][j] + C912*k[12][j];\n\
  for(j = 0; j < %d; ++j) { //m\n\
      MultiplyMyFloatA(tmp1, k[0][j], C90);\n\
      MultiplyMyFloatA(tmp2, k[4][j], C914);\n\
      MultiplyMyFloatA(tmp3, k[6][j], C915);\n\
      AddMyFloatA(tmp4, tmp1, tmp2);\n\
      AddMyFloatA(tmp1, tmp3, tmp4);\n\
      MultiplyMyFloatA(tmp2, k[7][j], C97);\n\
      MultiplyMyFloatA(tmp3, k[8][j], C98);\n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      AddMyFloatA(tmp2, tmp1, tmp4);\n\
      MultiplyMyFloatA(tmp1, k[9][j], C99);\n\
      MultiplyMyFloatA(tmp3, k[10][j], C910);\n\
      AddMyFloatA(tmp4, tmp1, tmp3);\n\
      AddMyFloatA(tmp1, tmp2, tmp4);\n\
\n\
      MultiplyMyFloatA(tmp2, k[11][j], C911);\n\
      MultiplyMyFloatA(tmp3, k[12][j], C912);\n\
      AddMyFloatA(tmp4, tmp3, tmp2);\n\
      AddMyFloatA(tmp2, tmp1, tmp4);      \n\
      \n\
      AddMyFloatA(tmp3, tmp2, xx_in[j]);\n\
      AssignMyFloat(xx_in[j], tmp3);\n\
\n\
      MultiplyMyFloatA(tmp2, tmp3, tmp3);\n\
      AddMyFloatA(tmp1, nr, tmp2);\n\
      AssignMyFloat(nr, tmp1);      \n\
  }\n\
",m);
  if(state_jet_vars > 0) {
  fprintf(outfile, "\n\
   if(kjet)\n\
    for(j = 0; j < %d; ++j) { //n\n\
      MultiplyJetFloatA(jtmp1, kjet[0][j], C90);\n\
      MultiplyJetFloatA(jtmp2, kjet[4][j], C914);\n\
      MultiplyJetFloatA(jtmp3, kjet[6][j], C915);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
      AddJetJetA(jtmp1, jtmp3, jtmp4); \n\
      MultiplyJetFloatA(jtmp2, kjet[7][j], C97);\n\
      MultiplyJetFloatA(jtmp3, kjet[8][j], C98);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp2); \n\
      AddJetJetA(jtmp2, jtmp1, jtmp4); \n\
      MultiplyJetFloatA(jtmp1, kjet[9][j], C99);\n\
      MultiplyJetFloatA(jtmp3, kjet[10][j], C910);\n\
      AddJetJetA(jtmp4, jtmp1, jtmp3);\n\
      AddJetJetA(jtmp1, jtmp2, jtmp4);\n\
\n\
      MultiplyJetFloatA(jtmp2, kjet[11][j], C911);\n\
      MultiplyJetFloatA(jtmp3, kjet[12][j], C912);\n\
      AddJetJetA(jtmp4, jtmp3, jtmp2);\n\
      AddJetJetA(jtmp2, jtmp1, jtmp4);\n\
      \n\
      AddJetJetA(jtmp3, jtmp2, jetIn[j]);\n\
      AssignJetToJet(jetIn[j], jtmp3);\n\
\n\
      xNormJet(tmp1,jetIn[j]);\n\
      AddMyFloatA(jnr, tmp1, jnr);\n\
    }\n\
",n);
  }

  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
     if(MyFloatA_GE_B(jnr,nr)) { AssignMyFloat(nr, jnr);}\n\
\n\
");
  }
  fprintf(outfile, "\n\
  DivideMyFloatA(tmp1, nr, NN);\n\
  sqrtMyFloatA( *nr_ret, tmp1);\n\
\n\
// JG: should we return jet norm as well?\n\
\n\
 }\n\
");

  fprintf(outfile, "\n\
 static void error_rk89_%s(MY_FLOAT **k, MY_JET **kjet, MY_FLOAT *er_ret) {\n\
   if(er_ret == NULL) return;\n\
   static MY_FLOAT s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero;\n\
   static int inited;\n\
#pragma omp threadprivate(inited,s, e, je, NN, tmp1, tmp2, tmp3, tmp4, zero,inited)\n\
   int j;\n\
",suffix);
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
   static MY_JET js, jtmp1,jtmp2,jtmp3,jtmp4;\n\
#pragma omp threadprivate(js, jtmp1, jtmp2, jtmp3, jtmp4)\n\
");
  }
  fprintf(outfile, "\n\
   if(!inited) {\n\
     InitMyFloat(s); InitMyFloat(je);  InitMyFloat(e); InitMyFloat(NN); InitMyFloat(zero);\n \
     InitMyFloat(tmp1);    InitMyFloat(tmp2);    InitMyFloat(tmp3);    InitMyFloat(tmp4);\n\
");
  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
   InitJet(js); InitJet(jtmp1); InitJet(jtmp2); InitJet(jtmp3); InitJet(jtmp4);\n\
");
  }
  
  fprintf(outfile, "\n\
     MakeMyFloatC(zero, \"0\", (double)0);\n	\
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
");
  }

  fprintf(outfile, "\n\
   // s = E0*k[0][j] + E13*k[3][j] + E14*k[4][j] + E15*k[6][j] + E7*k[7][j] + E8*k[8][j] + E9*k[9][j] + E10*k[10][j] + E11*k[11][j] + E12*k[12][j];   \n\
   for(j = _J_DIM_; j < %d; ++j) {\n\
\n\
     MultiplyMyFloatA(tmp1, k[0][j], E0);\n\
     MultiplyMyFloatA(tmp2, k[3][j], E13);\n\
     MultiplyMyFloatA(tmp3, k[4][j], E14);\n\
     AddMyFloatA(tmp4, tmp1, tmp2);\n\
     AddMyFloatA(s, tmp3, tmp4);     \n\
\n\
     MultiplyMyFloatA(tmp1, k[6][j], E15);\n\
     MultiplyMyFloatA(tmp2, k[7][j], E7);\n\
     MultiplyMyFloatA(tmp3, k[8][j], E8);\n\
     AddMyFloatA(tmp4, tmp1, tmp2);\n\
     AddMyFloatA(tmp1, tmp3, tmp4);\n\
     AddMyFloatA(tmp2, tmp1, s);\n\
     AssignMyFloat(s, tmp2);\n\
\n\
     MultiplyMyFloatA(tmp1, k[9][j], E9);\n\
     MultiplyMyFloatA(tmp2, k[10][j], E10);\n\
     MultiplyMyFloatA(tmp3, k[11][j], E11);\n\
     AddMyFloatA(tmp4, tmp1, tmp2);\n\
     AddMyFloatA(tmp1, tmp3, tmp4);\n\
     AddMyFloatA(tmp2, tmp1, s);\n\
     MultiplyMyFloatA(tmp1, k[12][j], E12);     \n\
     AddMyFloatA(s, tmp1, tmp2);\n\
\n\
     MultiplyMyFloatA(tmp1, s, s);     \n\
\n\
     AddMyFloatA(e, e, tmp1);     \n\
   }\n\
",m);

  if(state_jet_vars > 0) {
    fprintf(outfile, "\n\
 if(kjet)  \n\
   for(j = 0; j < %d; ++j) {\n\
\n\
     MultiplyJetFloatA(jtmp1, kjet[0][j], E0);\n\
     MultiplyJetFloatA(jtmp2, kjet[3][j], E13);\n\
     MultiplyJetFloatA(jtmp3, kjet[4][j], E14);\n\
     AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
     AddJetJetA(js, jtmp3, jtmp4);     \n\
\n\
     MultiplyJetFloatA(jtmp1, kjet[6][j], E15);\n\
     MultiplyJetFloatA(jtmp2, kjet[7][j], E7);\n\
     MultiplyJetFloatA(jtmp3, kjet[8][j], E8);\n\
     AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
     AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
     AddJetJetA(jtmp2, jtmp1, js);\n\
     AssignJetToJet(js, jtmp2);\n\
\n\
     MultiplyJetFloatA(jtmp1, kjet[9][j], E9);\n\
     MultiplyJetFloatA(jtmp2, kjet[10][j], E10);\n\
     MultiplyJetFloatA(jtmp3, kjet[11][j], E11);\n\
     AddJetJetA(jtmp4, jtmp1, jtmp2);\n\
     AddJetJetA(jtmp1, jtmp3, jtmp4);\n\
     AddJetJetA(jtmp2, jtmp1, js);\n\
     MultiplyJetFloatA(jtmp1, kjet[12][j], E12);     \n\
     AddJetJetA(js, jtmp1, jtmp2);\n\
\n\
     xNormJet(tmp2, js);     \n\
     MultiplyMyFloatA(tmp1, tmp2, tmp2);\n\
\n\
     AddMyFloatA(je, je, tmp1);     \n\
   }\n\
\n\
     AddMyFloatA(e, je, e);     \n\
",n);
  }
  
  fprintf(outfile, "\n\
   DivideMyFloatA(tmp1, e, NN);\n\
\n\
   sqrtMyFloatA( *er_ret, tmp1);\n\
\n\
\n\
}\n\
");

}
