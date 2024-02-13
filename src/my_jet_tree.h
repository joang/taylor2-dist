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

#ifndef MY_JET_TREE_H
#define MY_JET_TREE_H

#include "my_tree.h"

/* *******  *******************  ******* */
/* *******  MY_JET_TREE_HEADER   ******* */
/* *******  *******************  ******* */
#define MY_JET_TREE_TYPE1(PREFIX_MYJET,\
  MY_TREE_TYPE,PREFIX_TREE,COEF,I) "\
" MY_TREE_TYPE(PREFIX_TREE,COEF,I) "\n\
\n\
typedef " PREFIX_TREE(t) "* " PREFIX_MYJET(t) ";\n\
//#define " PREFIX_MYJET(ptr) " " PREFIX_MYJET(t) "\n\
typedef " PREFIX_MYJET(t) "* " PREFIX_MYJET(ptr) ";\n\
" \

#define MY_JET_TREE_FIXES MY_JET_SPACE
#define MY_JET_TREE_TYPE(PREFIX_JET,COEF,I) \
  MY_JET_TREE_TYPE1(PREFIX_JET,MY_TREE_TYPE,MY_TREE_FIXES,COEF,I)

#define MY_JET_TREE_API(PREFIX_MYJET,MY_JET_TREE_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                        MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \
  MY_JET_APIS(PREFIX_MYJET,MY_JET_TREE_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) \

#define MY_JET_TREE_PREHEADER(PREFIX_MYJET,MY_JET_TREE_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                              MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* HEADER " PREFIX_MYJET(t) " */\n " \

#define MY_JET_TREE_HEADER(PREFIX_MYJET,MY_JET_TREE_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                           MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
" MY_JET_TREE_API(PREFIX_MYJET,MY_JET_TREE_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\n\
\n\
/* advanced functions (in testing) */\n\
void " PREFIX_MYJET(compo) "(" PREFIX_MYJET(t) ", " PREFIX_MYJET(t) ", " PREFIX_MYJET(t) "*);\n\
void " PREFIX_MYJET(reverse) "(" PREFIX_MYJET(t) "*, " PREFIX_MYJET(t) "*);\n\
void " PREFIX_MYJET(algT) "(" PREFIX_MYJET(t) "*, " PREFIX_MYJET(t) "*, " PREFIX_MYJET(t) "*);\n\
void " PREFIX_MYJET(genidx) "(" I "[], " I ");\n\
" \

#define MY_JET_TREE_POSTHEADER(PREFIX_MYJET,MY_JET_TREE_TYPE,PREFIX_MYCOEF,MYCOEF,MYFLOAT,I,\
                               MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END HEADER " PREFIX_MYJET(t) " */\n" \

/* *******  *****************  ******* */
/* *******  MY_JET_TREE_CODE  ******* */
/* *******  *****************  ******* */
#define MY_JET_TREE_PRECODE(PREFIX_MYJET,PREFIX_TREE,PREFIX_MYCOEF,I,\
                            MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* CODE " PREFIX_MYJET(t) " */\n\
int * " PREFIX_MYJET(monomial_counts) "(void) {return _" PREFIX_MYJET(monomial_counts) "_;}\n\
int * " PREFIX_MYJET(monomial_offsets) "(void) {return _" PREFIX_MYJET(monomial_offsets) "_;}\n\
\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
#include <string.h>\n\
" \

#define MY_JET_TREE_CODE(PREFIX_MYJET,PREFIX_TREE,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,\
                         MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME,\
                         MYDATA_ACCESS,MY_RECURSION) "\
\n\
static " PREFIX_MYFLOAT(t) " " PREFIX_MYJET(faux) ";\n\
static " PREFIX_MYCOEF(t) " " PREFIX_MYJET(caux) ";\n\
static " PREFIX_MYJET(t) " " PREFIX_MYJET(jaux) "=NULL;\n\
#pragma omp threadprivate(" PREFIX_MYJET(faux) "," PREFIX_MYJET(caux) "," PREFIX_MYJET(jaux) ")\n\
\n\
/* for advanced special functions */\n\
static " I " " PREFIX_MYJET(ipool) "=0;\n\
static " PREFIX_MYJET(t) " *" PREFIX_MYJET(pool) "=NULL;\n\
#pragma omp threadprivate(" PREFIX_MYJET(ipool) "," PREFIX_MYJET(pool) ")\n\
\n\
static int " PREFIX_MYJET(flag_init_jet_library) "=0;\n\
static " I " " PREFIX_MYJET(nsymb) "=" MAX_NUM_SYMBOLS_NAME "," PREFIX_MYJET(deg) "=" MAX_DEGREE_NAME ";\n\
#pragma omp threadprivate(" PREFIX_MYJET(flag_init_jet_library) "," PREFIX_MYJET(nsymb) "," PREFIX_MYJET(deg) ")\n\
\n\
#define " PREFIX_MYCOEF(axpy) "(y,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(add2) "(y,y," PREFIX_MYJET(caux) ");}\n\
#define " PREFIX_MYCOEF(sixpy) "(y,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2_si) "(" PREFIX_MYJET(caux) ",x,a);\\\n\
  " PREFIX_MYCOEF(add2) "(y,y," PREFIX_MYJET(caux) ");}\n\
\n\
#define " PREFIX_MYCOEF(axpy_coef) "(y,s,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) "," PREFIX_MYJET(caux) ",s);\\\n\
  " PREFIX_MYCOEF(add2) "(y,y," PREFIX_MYJET(caux) ");}\n\
#define " PREFIX_MYCOEF(axpy_myfloat) "(y,s,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(mul2_myfloat) "(" PREFIX_MYJET(caux) "," PREFIX_MYJET(caux) ",s);\\\n\
  " PREFIX_MYCOEF(add2) "(y,y," PREFIX_MYJET(caux) ");}\n\
#define " PREFIX_MYCOEF(axpy_si) "(y,s,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(mul2_si) "(" PREFIX_MYJET(caux) "," PREFIX_MYJET(caux) ",s);\\\n\
  " PREFIX_MYCOEF(add2) "(y,y," PREFIX_MYJET(caux) ");}\n\
\n\
\n\
#define " PREFIX_MYCOEF(axmy) "(y,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(sub2) "(y,y," PREFIX_MYJET(caux) ");}\n\
#define " PREFIX_MYCOEF(sixmy) "(y,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2_si) "(" PREFIX_MYJET(caux) ",x,a);\\\n\
  " PREFIX_MYCOEF(sub2) "(y,y," PREFIX_MYJET(caux) ");}\n\
\n\
#define " PREFIX_MYCOEF(axmy_coef) "(y,s,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) "," PREFIX_MYJET(caux) ",s);\\\n\
  " PREFIX_MYCOEF(sub2) "(y,y," PREFIX_MYJET(caux) ");}\n\
#define " PREFIX_MYCOEF(axmy_si) "(y,s,a,x) {\\\n\
  " PREFIX_MYCOEF(mul2) "(" PREFIX_MYJET(caux) ",a,x);\\\n\
  " PREFIX_MYCOEF(mul2_si) "(" PREFIX_MYJET(caux) "," PREFIX_MYJET(caux) ",s);\\\n\
  " PREFIX_MYCOEF(sub2) "(y,y," PREFIX_MYJET(caux) ");}\n\
\n\
#define " PREFIX_TREE(nch) "(n,k) " PREFIX_MYJET(num_coefs_homogeneous) "[(n)*(" MAX_DEGREE_NAME "+1)+(k)]\n\
\n\
" MY_TREE_CODE(PREFIX_TREE,PREFIX_MYCOEF,PREFIX_MYFLOAT,I,PREFIX_MYJET(caux),PREFIX_MYJET(faux),MY_RECURSION) "\n\
\n\
size_t " PREFIX_MYJET(init) "(" PREFIX_MYJET(ptr) " s)\n\
{\n\
  // assert(s);\n\
  " I " i, j;\n\
  " PREFIX_MYCOEF(t) " *coef=NULL;\n\
  size_t mem, k;\n\
\n\
  k = (" MAX_DEGREE_NAME " + 1) * sizeof(**s);\n\
  *s = (__typeof__(*s)) malloc(k);\n\
  if (*s==NULL) {\n\
    fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
    fflush(stderr); exit(2);\n\
   }\n\
  mem = k;\n\
  k = " MAX_COEFFS_COUNT_NAME ";\n\
  coef = (__typeof__(coef)) malloc(k * sizeof(*coef));\n\
  if (coef==NULL) {\n\
    fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
    fflush(stderr); exit(2);\n\
   }\n\
  for (j = 0; j < k; ++j) {" PREFIX_MYCOEF(init) "(coef[j]);}\n\
  mem+= k * sizeof(*coef);\n\
  for (i = 0; i <= " MAX_DEGREE_NAME "; ++i)\n\
    {\n\
      /*k = " PREFIX_TREE(nch) "(" MAX_NUM_SYMBOLS_NAME ", i);\n\
      coef = (__typeof__(coef)) malloc(k * sizeof(*coef));\n\
      if (coef==NULL) {\n\
        fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
        fflush(stderr); exit(2);\n\
       }\n\
      for (j = 0; j < k; ++j) {" PREFIX_MYCOEF(init) "(coef[j]);}\n\
      mem += k * sizeof(*coef);*/\n\
      mem += " PREFIX_TREE(init) "((*s) + i, " MAX_NUM_SYMBOLS_NAME ", i, coef);\n\
      coef+= " PREFIX_TREE(nch) "(" MAX_NUM_SYMBOLS_NAME ", i);\n\
    }\n\
  return mem;\n\
}\n\
\n\
void " PREFIX_MYJET(initup2) "(" I " ns, " I " dg)\n\
{\n\
  if (" PREFIX_MYJET(flag_init_jet_library) "==1) return;\n\
#pragma omp single\n\
{\n\
  if (ns > " MAX_NUM_SYMBOLS_NAME ") {\n\
    fprintf(stderr, \"\%s:\%d nsymb value illegal \%d > \%d\\n\", __FILE__,__LINE__, ns, " MAX_NUM_SYMBOLS_NAME ");\n\
    fflush(stderr); exit(1);\n\
  }\n\
  if (dg > " MAX_DEGREE_NAME ") {\n\
    fprintf(stderr, \"\%s:\%d deg value illegal \%d > \%d\\n\", __FILE__,__LINE__, dg, " MAX_DEGREE_NAME ");\n\
    fflush(stderr); exit(1);\n\
  }\n\
}\n\
  " PREFIX_MYJET(nsymb) " = ns;\n\
  " PREFIX_MYJET(deg) " = dg;\n\
  " PREFIX_MYCOEF(initup) "();\n\
  " PREFIX_MYFLOAT(init) "(" PREFIX_MYJET(faux) ");\n\
  " PREFIX_MYCOEF(init) "(" PREFIX_MYJET(caux) ");\n\
  \n\
  {\n\
  " I " i;\n\
  " PREFIX_MYJET(pool) " = (__typeof__(" PREFIX_MYJET(pool) ")) malloc((2*" MAX_NUM_SYMBOLS_NAME "+1)*sizeof(*" PREFIX_MYJET(pool) "));\n\
  if (" PREFIX_MYJET(pool) "==NULL)\n\
    {\n\
      fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
      fflush(stderr); exit(2);\n\
    }\n\
  for (i = 0; i <= 2*" MAX_NUM_SYMBOLS_NAME "; ++i)\n\
    {\n\
//      " PREFIX_MYJET(pool) "[i] = (__typeof__(*" PREFIX_MYJET(pool) ")) malloc(1*sizeof(**" PREFIX_MYJET(pool) "));\n\
//      if (" PREFIX_MYJET(pool) "[i]==NULL)\n\
//        {\n\
//          fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
//          fflush(stderr); exit(2);\n\
//        }\n\
      " PREFIX_MYJET(init) "(&" PREFIX_MYJET(pool) "[i]);\n\
    }\n\
  " PREFIX_MYJET(ipool) " = 0;\n\
  }\n\
  " PREFIX_MYJET(init) "(&" PREFIX_MYJET(jaux) ");//" PREFIX_MYJET(jaux) "=" PREFIX_MYJET(pool) "[0];\n\
  \n\
  " PREFIX_MYJET(flag_init_jet_library) "=1;\n\
}\n\
\n\
void " PREFIX_MYJET(clean) "(" PREFIX_MYJET(ptr) " s)\n\
{\n\
  if (s==NULL || *s==NULL) return;\n\
\n\
  " I " j, k = " MAX_COEFFS_COUNT_NAME ";\n\
  __typeof__(*s) st, sf;\n\
  for (st = *s, sf = st + " MAX_DEGREE_NAME ";\n\
       st <= sf;\n\
       ++st) {\n\
    " PREFIX_TREE(clean) "(st);\n\
    /*free(st->coef); st->coef = NULL;*/\n\
  }\n\
  for (j = 0; j < k; ++j) {" PREFIX_MYCOEF(clean) "((*s)->coef[j]);}\n\
  free((*s)->coef); (*s)->coef = NULL;\n\
\n\
  free(*s); *s = NULL;\n\
}\n\
\n\
void " PREFIX_MYJET(cleanup) "(void)\n\
{\n\
  if (" PREFIX_MYJET(flag_init_jet_library) "==0) return;\n\
  {\n\
  " I " i;\n\
  for (i = 0; i <= 2*" MAX_NUM_SYMBOLS_NAME "; ++i)\n\
    {\n\
      " PREFIX_MYJET(clean) "(&" PREFIX_MYJET(pool) "[i]);\n\
//      free(" PREFIX_MYJET(pool) "[i]); " PREFIX_MYJET(pool) "[i]=NULL;\n\
    }\n\
  " PREFIX_MYJET(ipool) " = 0;\n\
  free(" PREFIX_MYJET(pool) "); " PREFIX_MYJET(pool) "=NULL;\n\
  }\n\
  " PREFIX_MYJET(clean) "(&" PREFIX_MYJET(jaux) ");\n\
  " PREFIX_MYCOEF(clean) "(" PREFIX_MYJET(caux) ");\n\
  " PREFIX_MYFLOAT(clean) "(" PREFIX_MYJET(faux) ");\n\
  " PREFIX_MYCOEF(cleanup) "();\n\
  " PREFIX_MYJET(nsymb) " = 0;\n\
  " PREFIX_MYJET(deg) " = 0;\n\
  " PREFIX_MYJET(flag_init_jet_library) "=0;\n\
}\n\
\n\
" I " " PREFIX_MYJET(get_num_symbs) "(void)\
{\
return " PREFIX_MYJET(nsymb) ";\
}\n\
\n\
" I " " PREFIX_MYJET(get_deg) "(void)\
{\
return " PREFIX_MYJET(deg) ";\
}\n\
\n\
" I " " PREFIX_MYJET(set_num_symbs) "(" I " nsymbs)\n\
{\n\
  fprintf(stderr,\"\%s:\%d Action not allowed\\n\",__FILE__,__LINE__); fflush(stderr); exit(1);\n\
  return 0;\n\
}\n\
\n\
" I " " PREFIX_MYJET(set_deg) "(" I " degree)\n\
{\n\
  " I " k=" PREFIX_MYJET(deg) ";\n\
  if (degree > " MAX_DEGREE_NAME ") {\n\
    fprintf(stderr,\"\%s:\%d Invalid degree \%d > \%d\\n\",__FILE__,__LINE__,degree," MAX_DEGREE_NAME ");\n\
    fflush(stderr); exit(1);\n\
  }\n\
  " PREFIX_MYJET(deg) " = degree;\n\
  return k;\n\
}\n\
\n\
void " PREFIX_MYJET(set) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  if (s == a) return;\n\
\n\
  __typeof__(s) st, at, sf;\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ", at= a;\n\
       st <= sf;\n\
       ++st, ++at){" PREFIX_TREE(set) "(st,at);}\n\
}\n\
\n\
void " PREFIX_MYJET(set_zero) "(" PREFIX_MYJET(t) " s)\n\
{\n\
  __typeof__(s) st, sf;\n\
  " PREFIX_MYCOEF(t) " *hc, *hf;\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st <= sf; ++st)\n\
  {" PREFIX_TREE(set_zero) "(*st,hc,hf);}\n\
}\n\
\n\
void " PREFIX_MYJET(set_coef) "(" PREFIX_MYJET(t) " s, " PREFIX_MYCOEF(t) " coef)\n\
{\n\
  // assert(s && *s);\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_coef) "(s,coef);\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
\n\
void " PREFIX_MYJET(set_myfloat) "(" PREFIX_MYJET(t) " s, " PREFIX_MYFLOAT(t) " myf)\n\
{\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_myfloat) "(s,myf);\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
\n\
void " PREFIX_MYJET(set_d) "(" PREFIX_MYJET(t) " s, double d)\n\
{\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_d) "(s,d);\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
\n\
\n\
void " PREFIX_MYJET(set_si) "(" PREFIX_MYJET(t) " s, int d)\n\
{\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_si) "(s,d);\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
void " PREFIX_MYJET(set_coef_array) "(" PREFIX_MYJET(t) " s," PREFIX_MYCOEF(t) " *b)\n\
{\n\
  " I " i;\n\
\n\
  for (i=0;i <" MAX_COEFFS_COUNT_NAME ";\n\
        b++, i++){" PREFIX_MYCOEF(set) "( " MYDATA_ACCESS "(s,i) ,*b);}\n\
}\n\
\n\
void " PREFIX_MYJET(set_jet_2_coef_array) "(" PREFIX_MYCOEF(t) " *b, " PREFIX_MYJET(t) " s)\n\
{\n\
 " I " i;\n\
\n\
  for (i=0 ; i < " MAX_COEFFS_COUNT_NAME ";\n\
        b++, i++){" PREFIX_MYCOEF(set) "(*b, " MYDATA_ACCESS "(s,i));}\n\
}\n\
\n\
" PREFIX_MYFLOAT(t) "* " PREFIX_MYJET(to_myfloat) "(" PREFIX_MYJET(t) " s)\n\
{\
  return " PREFIX_MYCOEF(to_myfloat) "(*" PREFIX_TREE(get_zero_coef) "(s));\
}\n\
\n\
" PREFIX_MYCOEF(t) "* " PREFIX_MYJET(get_coef) "(" PREFIX_MYJET(t) " s, const " I " idx[])\n\
{\n\
  " I " i, k;\n\
  for (i = 0, k = 0; i < s->nsymb; k += idx[i], ++i){}\n\
  \n\
  return " PREFIX_TREE(get_coef) "(s+k, idx);\n\
}\n\
\n\
" PREFIX_MYCOEF(t) "* " PREFIX_MYJET(coef0) "(" PREFIX_MYJET(t) " s)\n\
{\
  return " PREFIX_TREE(get_zero_coef) "(s);\
}\n\
" PREFIX_MYCOEF(t) "* " PREFIX_MYJET(coef1) "(" PREFIX_MYJET(t) " s, " I " i)\n\
{\n\
  return &s[1].coef[i];\n\
}\n\
\n\
void " PREFIX_MYJET(nrminf) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_MYJET(t) " s)\n\
{\n\
  __typeof__(s) st, sf;\n\
  /* WARNING initial value for nrm GIVEN!! */\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st <= sf;\n\
       ++st){" PREFIX_TREE(nrminf) "(nrm,st);}\n\
}\n\
\n\
void " PREFIX_MYJET(nrm2) "(" PREFIX_MYFLOAT(t) " nrm[1], " PREFIX_MYJET(t) " s)\n\
{\n\
  __typeof__(s) st, sf;\n\
  /* WARNING initial value for nrm GIVEN!! */\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ";\n\
       st <= sf;\n\
       ++st){" PREFIX_TREE(nrm2) "(nrm,st);}\n\
  " PREFIX_MYFLOAT(set_sqrt) "(*nrm,*nrm);\n\
}\n\
\n\
void " PREFIX_MYJET(neg) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a )\n\
{\n\
  // assert(s && a); // assert(s->nsymb == a->nsymb); // assert(deg == deg);\n\
  // assert(*s && *a);\n\
\n\
  __typeof__(s) st, at, sf;\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ", at = a;\n\
       st <= sf;\n\
       ++st, ++at){" PREFIX_TREE(neg) "(st,at);}\n\
}\n\
\n\
void " PREFIX_MYJET(add2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYJET(t) " b)\n\
{\n\
  // assert(s && a && b);\n\
\n\
  " I " i;\n\
  for (i = 0; i <= " PREFIX_MYJET(deg) "; ++i) {" PREFIX_TREE(add2) "(s+i, a+i, b+i);}\n\
}\n\
\n\
void " PREFIX_MYJET(sub2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYJET(t) " b)\n\
{\n\
  // assert(s && a && b);\n\
\n\
  " I " i;\n\
  for (i = 0; i <= " PREFIX_MYJET(deg) "; ++i) {" PREFIX_TREE(sub2) "(s+i, a+i, b+i);}\n\
}\n\
\n\
void " PREFIX_MYJET(set_sq) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_MYCOEF(t) " *a0, *sc, *sf;\n\
  sc = " PREFIX_TREE(get_zero_coef) "(s);\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\n\
  " PREFIX_MYCOEF(mul2) "((*sc),(*a0),(*a0));\n\
\n\
  " I " i, j, k, ord=" PREFIX_MYJET(deg) ";\n\
  if (ord % 2)\n\
    {\n\
      i = ord;\n\
      /* odd case */\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      k = i/2;\n\
      for (j = 0; j <= k; ++j) " PREFIX_TREE(pph) "(s+i, a+j, a+(i-j));\n\
      " PREFIX_TREE(mul2_si) "(s[i], s[i], 2, sc,sf,a0);\n\
      --ord;\n\
    }\n\
  for (i = 1; i <= ord; ++i)\n\
    {\n\
       /* odd case */\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      k = i/2;\n\
      for (j = 0; j <= k; ++j) " PREFIX_TREE(pph) "(s+i, a+j, a+(i-j));\n\
      " PREFIX_TREE(mul2_si) "(s[i], s[i], 2, sc,sf,a0);\n\
\n\
      ++i;\n\
\n\
      /* even case */\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      k = i/2;\n\
      for (j = 0; j < k; ++j) " PREFIX_TREE(pph) "(s+i, a+j, a+(i-j));\n\
      " PREFIX_TREE(mul2_si) "(s[i], s[i], 2, sc,sf,a0);\n\
      " PREFIX_TREE(pph) "(s+i, a + k, a + k);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(mul2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYJET(t) " b)\n\
{\n\
// assert(s && a && b); // assert(s != a && s != b);\n\
// assert(*s && *a && *b);\n\
  " I " i, j;\n\
  " PREFIX_MYCOEF(t) " *sc,*sf;\n\
\n\
  if (a == b) {" PREFIX_MYJET(set_sq) "(s,a); return;}\n\
  for (i = 0; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j <= i; ++j)\n\
        {" PREFIX_TREE(pph) "(s+i, a+j, b+(i-j));}\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(div2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYJET(t) " b)\n\
{\n\
  // assert(s && a && b); // assert(s != b);\n\
  // assert(*s && *a && *b);\n\
  " PREFIX_MYCOEF(t) " *b0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  sc = " PREFIX_TREE(get_zero_coef) "(a);\n\
  b0 = " PREFIX_TREE(get_zero_coef) "(b);\n\
  sf = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(div2) "(*sf,*sc,*b0);\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, b + (i-j));}\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i],(*b0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(add2_coef) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(add2) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(sub2_coef) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(sub2) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(coef_sub2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYCOEF(t) " b, " PREFIX_MYJET(t) " a)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(sub2) "(*(s->coef),b,*(a->coef));\n\
  for (st = s, at= a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(neg) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(mul2_coef) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
  " I " k;\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= " PREFIX_MYJET(deg) "; ++k)\n\
  {" PREFIX_TREE(mul2_coef) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(div2_coef) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYCOEF(t) " b)\n\
{\n\
  // assert(s && a);\n\
  // assert(coef);\n\
\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  __typeof__(s) st, sf, at;\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ", at= a;\n\
       st <= sf;\n\
       ++st, ++at) {" PREFIX_TREE(div2_coef) "(st[0], at[0], b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(coef_div2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYCOEF(t) " b, " PREFIX_MYJET(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(coef);\n\
  " PREFIX_MYCOEF(t) " *a0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\n\
  sf = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(div2) "(*sf,b,*a0);\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, a+(i-j));}\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i],(*a0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
\n\
void " PREFIX_MYJET(add2_myfloat) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(add2_myfloat) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(sub2_myfloat) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(sub2_myfloat) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(myfloat_sub2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYFLOAT(t) " b, " PREFIX_MYJET(t) " a)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(myfloat_sub2) "(*(s->coef),b,*(a->coef));\n\
  for (st = s, at= a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(neg) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(mul2_myfloat) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
  " I " k;\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= " PREFIX_MYJET(deg) "; ++k)\n\
  {" PREFIX_TREE(mul2_myfloat) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(div2_myfloat) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYFLOAT(t) " b)\n\
{\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  __typeof__(s) st, sf, at;\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ", at= a;\n\
       st <= sf;\n\
       ++st, ++at) {" PREFIX_TREE(div2_myfloat) "(st[0], at[0], b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(myfloat_div2) "(" PREFIX_MYJET(t) " s, " PREFIX_MYFLOAT(t) " b, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYCOEF(t) " *a0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\n\
  sf = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(myfloat_div2) "(*sf,b,*a0);\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, a+(i-j));}\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i],(*a0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
\n\
void " PREFIX_MYJET(add2_d) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, double b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(add2_d) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(sub2_d) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, double b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(sub2_d) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(d_sub2) "(" PREFIX_MYJET(t) " s, double b, " PREFIX_MYJET(t) " a)\n\
{\n\
  fprintf(stderr,\"\%s:\%d to implement\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_MYJET(mul2_d) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, double b)\n\
{\n\
  " I " k;\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= " PREFIX_MYJET(deg) "; ++k)\n\
  {" PREFIX_TREE(mul2_d) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(div2_d) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, double b)\n\
{\n\
  fprintf(stderr,\"\%s:\%d to implement\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_MYJET(d_div2) "(" PREFIX_MYJET(t) " s, double b, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYCOEF(t) " *a0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\n\
  sf = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(d_div2) "(*sf,b,*a0);\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, a+(i-j));}\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i],(*a0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(add2_si) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, int b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(add2_si) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(sub2_si) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, int b)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(sub2_si) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_MYJET(si_sub2) "(" PREFIX_MYJET(t) " s, int b, " PREFIX_MYJET(t) " a)\n\
{\n\
  __typeof__(s) st, at, sf = s + " PREFIX_MYJET(deg) ";\n\
  " PREFIX_MYCOEF(si_sub2) "(*(s->coef),b,*(a->coef));\n\
  for (st = s, at= a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(neg) "(st,at)){}\n\
//  fprintf(stderr,\"\%s:\%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_MYJET(mul2_si) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, int b)\n\
{\n\
  " I " k;\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= " PREFIX_MYJET(deg) "; ++k)\n\
  {" PREFIX_TREE(mul2_si) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(div2_si) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, int b)\n\
{\n\
  " PREFIX_MYCOEF(t) " *sc, *scf, *ac;\n\
  __typeof__(s) st, sf, at;\n\
  for (st = s, sf = st + " PREFIX_MYJET(deg) ", at= a;\n\
       st <= sf;\n\
       ++st, ++at) {" PREFIX_TREE(div2_si) "(st[0], at[0], b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_MYJET(si_div2) "(" PREFIX_MYJET(t) " s, int b, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYCOEF(t) " *a0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\n\
  sf = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(si_div2) "(*sf,b,*a0);\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, a+(i-j));}\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i],(*a0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(set_sqrt) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  // assert(s && a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_MYCOEF(t) " *s0,*sc,*scf,*sc2;\n\
  s0 = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(set_sqrt) "((*s0), *" PREFIX_TREE(get_zero_coef) "(a)); // assert(fabs(*s0) > ZERO_TOL);\n\
\n\
  " I " i, j, k;\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
       /* odd case */\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      " PREFIX_TREE(div2_si) "(s[i], s[i], 2, sc,scf,sc2);\n\
      k = i/2;\n\
      for (j = 1; j <= k; ++j) " PREFIX_TREE(mph) "(s+i, s+j, s+(i-j));\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i], *s0, sc,scf,sc2);\n\
\n\
      if (++i > " PREFIX_MYJET(deg) ") break;\n\
\n\
      /* even case */\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      k = i/2;\n\
      " PREFIX_TREE(mph) "(s+i, s+k, s+k);\n\
      " PREFIX_TREE(div2_si) "(s[i], s[i], 2, sc,scf,sc2);\n\
      for (j = 1; j < k; ++j) " PREFIX_TREE(mph) "(s+i, s+j, s+(i-j));\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i], *s0, sc,scf,sc2);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(set_pow_myfloat) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a, " PREFIX_MYFLOAT(t) " myf)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
//  if (myf == 2)     {" PREFIX_MYJET(set_sq) "(s,a); return;}\n\
//  if (myf == 0.5e0) {" PREFIX_MYJET(set_sqrt) "(s,a); return;}\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_MYCOEF(t) " *a0, *s0, *tmp, *sc,*sf,*sc1;\n\
  tmp = " PREFIX_TREE(get_zero_coef) "(" PREFIX_MYJET(jaux) ");\n\
  sc = " PREFIX_TREE(get_zero_coef) "(s);\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a); // assert(fabs(*a0) > ZERO_TOL);\n\
  " PREFIX_MYCOEF(set_pow_myfloat) "((*sc), *a0, myf);\n\
\n\
  " I " i, j;\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {\n\
        " PREFIX_MYFLOAT(mul2_si) "(" PREFIX_MYJET(faux) ",myf,i-j);\n\
        " PREFIX_MYFLOAT(sub2_si) "(" PREFIX_MYJET(faux) "," PREFIX_MYJET(faux) ",j);\n\
        " PREFIX_TREE(pph_myfloat) "(s+i, " PREFIX_MYJET(faux) ", a+(i-j), s+j);\n\
      }\n\
      " PREFIX_MYCOEF(mul2_si) "(*tmp,*a0,i);\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i], (*tmp), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(set_exp) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_MYCOEF(t) " *s0,*scf,*sc;\n\
  s0 = " PREFIX_TREE(get_zero_coef) "(s);\n\
  " PREFIX_MYCOEF(set_exp) "((*s0), *" PREFIX_TREE(get_zero_coef) "(a));\n\
\n\
  " I " i, j;\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,scf);\n\
      for (j = 0; j < i; ++j)\n\
      {" PREFIX_TREE(pph_si) "(s+i, i-j, a+(i-j), s+j);}\n\
      " PREFIX_TREE(div2_si) "(s[i], s[i], i, sc,scf,s0);\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(set_log) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_MYCOEF(t) " *a0, *sc,*sf,*sc1;\n\
  sc = " PREFIX_TREE(get_zero_coef) "(s);\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\n\
  " PREFIX_MYCOEF(set_log) "((*sc), *" PREFIX_TREE(get_zero_coef) "(a));\n\
\n\
  " I " i, j;\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\n\
    {\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      for (j = 1; j < i; ++j) {\n\
        " PREFIX_MYFLOAT(set_si) "(" PREFIX_MYJET(faux) ", -j);\n\
        " PREFIX_MYFLOAT(div2_si) "(" PREFIX_MYJET(faux) ", " PREFIX_MYJET(faux) ", i);\n\
        " PREFIX_TREE(pph_myfloat) "(s+i, " PREFIX_MYJET(faux) ", a+(i-j), s+j);\n\
      }\n\
      " PREFIX_TREE(div2_coef) "(s[i], s[i], *a0, sc,sf,sc1);\n\
    }\n\
}\n\
\n\
#define " PREFIX_MYJET(sincos) "(s,c,a)\\\n\
{\\\n\
  /* assert(s && c && a); assert(s != a && s != c && c != a);*/\\\n\
  /* assert(*s && *c && *a);*/\\\n\
\\\n\
  " I " i, j;\\\n\
  " PREFIX_MYFLOAT(t) " aux1,aux2;\\\n\
  " PREFIX_MYCOEF(t) " *a0, *sc, *sf;\\\n\
  " PREFIX_MYFLOAT(init) "(aux1);\\\n\
  " PREFIX_MYFLOAT(init) "(aux2);\\\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\\\n\
  " PREFIX_MYCOEF(set_sin) "((*" PREFIX_TREE(get_zero_coef) "(s)),(*a0));\\\n\
  " PREFIX_MYCOEF(set_cos) "((*" PREFIX_TREE(get_zero_coef) "(c)),(*a0));\\\n\
\\\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\\\n\
    {\\\n\
      " PREFIX_TREE(set_zero) "((s)[i], sc,sf);\\\n\
      " PREFIX_TREE(set_zero) "((c)[i], sc,sf);\\\n\
\\\n\
      for (j = 0; j < i; ++j)\\\n\
        {\\\n\
          " PREFIX_MYFLOAT(set_si) "(aux1,j);\\\n\
          " PREFIX_MYFLOAT(div2_si) "(aux1,aux1,i);\\\n\
          " PREFIX_MYFLOAT(sub2_si) "(aux2,aux1,1);\\\n\
          " PREFIX_MYFLOAT(si_sub2) "(aux1,1,aux1);\\\n\
\\\n\
          " PREFIX_TREE(pph_myfloat) "((s)+i, aux1, (a)+(i-j), (c)+j);\\\n\
          " PREFIX_TREE(pph_myfloat) "((c)+i, aux2, (a)+(i-j), (s)+j);\\\n\
        }\\\n\
    }\\\n\
  " PREFIX_MYFLOAT(clean) "(aux2);\\\n\
  " PREFIX_MYFLOAT(clean) "(aux1);\\\n\
}\\\n\
\n\
void " PREFIX_MYJET(set_sin) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYJET(sincos) "(s," PREFIX_MYJET(jaux) ",a);\n\
}\n\
\n\
void " PREFIX_MYJET(set_cos) "(" PREFIX_MYJET(t) " c, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYJET(sincos) "(" PREFIX_MYJET(jaux) ",c,a);\n\
}\n\
\n\
void " PREFIX_MYJET(set_tan) "(" PREFIX_MYJET(t) " t, " PREFIX_MYJET(t) " a)\n\
{\n\
  " I " j,k,ii,jj ,nk;\n\
  " PREFIX_MYCOEF(t) " *t0,*tcf,*tc;\n\
  " PREFIX_MYCOEF(set_tan) "(*" PREFIX_TREE(get_zero_coef) "(t),*" PREFIX_TREE(get_zero_coef) "(a));\n\
  " PREFIX_MYCOEF(mul2) "(*" PREFIX_TREE(get_zero_coef) "(" PREFIX_MYJET(jaux) "),*" PREFIX_TREE(get_zero_coef) "(t),*" PREFIX_TREE(get_zero_coef) "(t));\n\
\n\
  nk = (" PREFIX_MYJET(deg) "+1) - ((" PREFIX_MYJET(deg) "+1)%2==0);\n\
  for (k = 1; k < nk; k++) {\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 0; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k,k-j,a+(k-j)," PREFIX_MYJET(jaux) "+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(add2) "(t+k,t+k,a+k);\n\
\n\
    /* k odd */\n\
    " PREFIX_TREE(set_zero) "(" PREFIX_MYJET(jaux) "[k], tc,tcf);\n\
    for (j = 0; j <= (k-1)/2; j++)\n\
    {" PREFIX_TREE(pph) "(" PREFIX_MYJET(jaux) "+k,t+j,t+(k-j));}\n\
    " PREFIX_TREE(mul2_si) "(" PREFIX_MYJET(jaux) "[k]," PREFIX_MYJET(jaux) "[k],2, tc,tcf,t0);\n\
\n\
    ++k;\n\
\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 0; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k,k-j,a+(k-j)," PREFIX_MYJET(jaux) "+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(add2) "(t+k,t+k,a+k);\n\
\n\
    /* k even */\n\
    " PREFIX_TREE(set_zero) "(" PREFIX_MYJET(jaux) "[k], tc,tcf);\n\
    for (j = 0; j <= (k-2)/2; j++)\n\
    {" PREFIX_TREE(pph) "(" PREFIX_MYJET(jaux) "+k,t+j,t+(k-j));}\n\
    " PREFIX_TREE(mul2_si) "(" PREFIX_MYJET(jaux) "[k]," PREFIX_MYJET(jaux) "[k],2, tc,tcf,t0);\n\
\n\
    j = k/2;\n\
    " PREFIX_TREE(pph) "(" PREFIX_MYJET(jaux) "+k,t+j,t+j);\n\
  }\n\
  if ((" PREFIX_MYJET(deg) "+1)%2==0) {\n\
    k = " PREFIX_MYJET(deg) ";\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 0; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k,k-j,a+(k-j)," PREFIX_MYJET(jaux) "+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(add2) "(t+k,t+k,a+k);\n\
  }\n\
\n\
//  fprintf(stderr,\"\%s:\%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
void " PREFIX_MYJET(set_atan) "(" PREFIX_MYJET(t) " t, " PREFIX_MYJET(t) " a)\n\
{\n\
  " I " j,k,ii,jj;\n\
  " PREFIX_MYCOEF(t) " *t0,*tcf,*tc;\n\
  " PREFIX_MYCOEF(set_atan) "(*" PREFIX_TREE(get_zero_coef) "(t),*" PREFIX_TREE(get_zero_coef) "(a));\n\
  " PREFIX_MYJET(set_sq) "(" PREFIX_MYJET(jaux) ",a);\n\
  " PREFIX_MYJET(add2_si) "(" PREFIX_MYJET(jaux) "," PREFIX_MYJET(jaux) ",1);\n\
  for (k = 1; k <= " PREFIX_MYJET(deg) "; k++) {\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 1; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k, j, " PREFIX_MYJET(jaux) "+(k-j), t+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(sub2) "(t+k,a+k,t+k);\n\
    " PREFIX_TREE(div2_coef) "(t[k],t[k],*" PREFIX_TREE(get_zero_coef) "(" PREFIX_MYJET(jaux) "), tc,tcf,t0);\n\
  }\n\
//  fprintf(stderr,\"\%s:\%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
#define " PREFIX_MYJET(sinhcosh) "(s,c,a)\\\n\
{\\\n\
  /* assert(s && c && a); assert(s != a && s != c && c != a);*/\\\n\
  /* assert(*s && *c && *a);*/\\\n\
\\\n\
  " I " i, j;\\\n\
  " PREFIX_MYFLOAT(t) " aux1;\\\n\
  " PREFIX_MYCOEF(t) " *a0, *sc,*sf;\\\n\
  " PREFIX_MYFLOAT(init) "(aux1);\\\n\
  a0 = " PREFIX_TREE(get_zero_coef) "(a);\\\n\
  " PREFIX_MYCOEF(set_sinh) "((*" PREFIX_TREE(get_zero_coef) "(s)),(*a0));\\\n\
  " PREFIX_MYCOEF(set_cosh) "((*" PREFIX_TREE(get_zero_coef) "(c)),(*a0));\\\n\
\\\n\
  for (i = 1; i <= " PREFIX_MYJET(deg) "; ++i)\\\n\
    {\\\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\\\n\
      " PREFIX_TREE(set_zero) "(c[i], sc,sf);\\\n\
\\\n\
      for (j = 0; j < i; ++j)\\\n\
        {\\\n\
          " PREFIX_MYFLOAT(set_si) "(aux1,j);\\\n\
          " PREFIX_MYFLOAT(div2_si) "(aux1,aux1,i);\\\n\
          " PREFIX_MYFLOAT(si_sub2) "(aux1,1,aux1);\\\n\
\\\n\
          " PREFIX_TREE(pph_myfloat) "(s+i, aux1, a+(i-j), c+j);\\\n\
          " PREFIX_TREE(pph_myfloat) "(c+i, aux1, a+(i-j), s+j);\\\n\
        }\\\n\
    }\\\n\
  " PREFIX_MYFLOAT(clean) "(aux1);\\\n\
}\\\n\
\n\
void " PREFIX_MYJET(set_sinh) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYJET(sinhcosh) "(s," PREFIX_MYJET(jaux) ",a);\n\
}\n\
\n\
void " PREFIX_MYJET(set_cosh) "(" PREFIX_MYJET(t) " c, " PREFIX_MYJET(t) " a)\n\
{\n\
  " PREFIX_MYJET(sinhcosh) "(" PREFIX_MYJET(jaux) ",c,a);\n\
}\n\
\n\
void " PREFIX_MYJET(set_fabs) "(" PREFIX_MYJET(t) " s, " PREFIX_MYJET(t) " a)\n\
{\n\
  " I " i;\n\
  for (i = 0; i <= " PREFIX_MYJET(deg) "; ++i) {" PREFIX_TREE(set_fabs) "(s+i,a+i);}\n\
}\n\
\n\
void " PREFIX_MYJET(eval) "(" PREFIX_MYCOEF(t) " val[1], " PREFIX_MYJET(t) " x, " PREFIX_MYFLOAT(t) " s[])\n\
{\n\
  " I " k, m;\n\
  " PREFIX_MYCOEF(set_zero) "(*val);\n\
  for (k = " PREFIX_MYJET(deg) "; k > 0; --k) {\n\
    " PREFIX_TREE(sumeval) "(val, x + k, s);\n\
    /* TODO possible memory improvement... */\n\
  }\n\
  " PREFIX_MYCOEF(add2) "(*val,*val, *(" PREFIX_TREE(get_zero_coef) "(x)));\n\
}\n\
\n\
/* special functions */\n\
\n\
static " PREFIX_MYJET(t) " " PREFIX_TREE(compoh) "(" PREFIX_TREE(ptr) " h, " PREFIX_MYJET(t) " *v)\n\
{\n\
  " I " n, k, m;\n\
  " PREFIX_MYJET(t) " x1, xk, tmp1, tmp2;\n\
  \n\
  n = h->nsymb;\n\
  k = h->deg;\n\
  x1 = v[n-1];\n\
  \n\
  if (k == 0) {\n\
    tmp1 = " PREFIX_MYJET(pool) "[" PREFIX_MYJET(ipool) "]; ++" PREFIX_MYJET(ipool) ";\n\
    " PREFIX_MYJET(set_coef) "(tmp1, *(h->coef));\n\
  } else if (n==1) {\n\
    tmp1 = " PREFIX_MYJET(pool) "[" PREFIX_MYJET(ipool) "]; ++" PREFIX_MYJET(ipool) ";\n\
    xk = " PREFIX_MYJET(pool) "[" PREFIX_MYJET(ipool) "]; ++" PREFIX_MYJET(ipool) ";\n\
    " PREFIX_MYJET(set) "(xk, x1);\n\
    for (m = 1; m < k; ++m)\n\
      {\n\
        /* xk = xk * x1 */\n\
        " PREFIX_MYJET(mul2) "(tmp1, xk, x1);\n\
//        aprods(tmp1, xk, x1);\n\
        \n\
        tmp2 = xk;\n\
        xk = tmp1;\n\
        tmp1 = tmp2;\n\
      }\n\
    " PREFIX_MYJET(set) "(" PREFIX_MYJET(jaux) ", xk);\n\
    " PREFIX_MYJET(mul2_coef) "(xk, " PREFIX_MYJET(jaux) ", *(h->coef));\n\
    " PREFIX_MYJET(pool) "[--" PREFIX_MYJET(ipool) "] = tmp1;\n\
    return xk;\n\
  } else {\n\
      xk = " PREFIX_MYJET(pool) "[" PREFIX_MYJET(ipool) "]; ++" PREFIX_MYJET(ipool) ";\n\
      tmp1 = " PREFIX_TREE(compoh) "((h->term)+k, v);\n\
      for (m = k; m > 0; --m)\n\
        {\n\
          " PREFIX_MYJET(mul2) "(xk, tmp1, x1);\n\
//          aprods(xk, tmp1, x1);\n\
          " PREFIX_MYJET(pool) "[--" PREFIX_MYJET(ipool) "] = tmp1;\n\
          tmp1 = " PREFIX_TREE(compoh) "((h->term) + m-1, v);\n\
          " PREFIX_MYJET(add2) "(tmp1, tmp1, xk);\n\
        }\n\
      " PREFIX_MYJET(pool) "[--" PREFIX_MYJET(ipool) "] = xk;\n\
    }\n\
  return tmp1;\n\
//  fprintf(stderr, \"\%s:\%d to implement\\n\", __FILE__, __LINE__); fflush(stderr); exit(1);\n\
}\n\
\n\
/* Given u(s1,...,sd) and v=(v1,...,vd)\n\
 * series vector of d variables.\n\
 * It outputs w = u(v1,...,vd) serie of d variables.\n\
 * \n\
 * Note: It allocates a pool of d+1 series possibly\n\
 *  given auxiliary parameter\n\
 */\n\
\n\
void " PREFIX_MYJET(compo) "(" PREFIX_MYJET(t) " w, " PREFIX_MYJET(t) " u, " PREFIX_MYJET(t) " *v)\n\
{\n\
  " I " m, k;\n\
  " PREFIX_MYJET(t) " tmp=NULL;\n\
  \n\
  k = " PREFIX_MYJET(get_deg) "();\n\
  " PREFIX_MYJET(set_zero) "(w);\n\
  for (m = k; m > 0; --m)\n\
    {\n\
      tmp = " PREFIX_TREE(compoh) "(u+m, v);\n\
      " PREFIX_MYJET(add2) "(w, w, tmp);\n\
      " PREFIX_MYJET(pool) "[--" PREFIX_MYJET(ipool) "] = tmp;\n\
    }\n\
  " PREFIX_MYCOEF(add2) "(*" PREFIX_TREE(get_zero_coef) "(w),*" PREFIX_TREE(get_zero_coef) "(w),*" PREFIX_TREE(get_zero_coef) "(u));\n\
}\n\
void " PREFIX_MYJET(reverse) "(" PREFIX_MYJET(t) " *u, " PREFIX_MYJET(t) " *v)\n\
{\n\
  " I " m, n, k, i, ns = " MAX_NUM_SYMBOLS_NAME ";\n\
  " PREFIX_MYJET(t) " tmp=NULL, *tmp2=&" PREFIX_MYJET(pool) "[ns+1];\n\
  \n\
  for (i = 0; i < ns; ++i)\n\
    {\n\
      " PREFIX_MYJET(set_zero) "(u[i]);\n\
      " PREFIX_TREE(set) "(&u[i][0], &v[i][0]);\n\
      " PREFIX_TREE(set) "(&u[i][1], &v[i][1]);\n\
      \n\
      " PREFIX_MYJET(set_zero) "(tmp2[i]);\n\
    }\n\
  \n\
  " PREFIX_MYJET(ipool) " = 0;\n\
  k = " PREFIX_MYJET(get_deg) "();\n\
  for (m = 2; m <= k; ++m)\n\
    {\n\
      for (n = m; n > 0; --n) for (i = 0; i < ns; ++i)\n\
        {\n\
          tmp = " PREFIX_TREE(compoh) "(&v[i][n], u);\n\
          " PREFIX_MYJET(add2) "(tmp2[i], tmp2[i], tmp); /* it could just be homog */\n\
          " PREFIX_MYJET(pool) "[--" PREFIX_MYJET(ipool) "] = tmp;\n\
        }\n\
      for (i = 0; i < ns; ++i)\n\
        {\n\
          //" PREFIX_TREE(set) "(&u[i][m], &tmp2[i][m]);\n\
          " PREFIX_TREE(neg) "(&u[i][m], &tmp2[i][m]);\n\
          " PREFIX_MYJET(set_zero) "(tmp2[i]);\n\
        }\n\
    }\n\
}\n\
void " PREFIX_MYJET(algT) "(" PREFIX_MYJET(t) " *w, " PREFIX_MYJET(t) " *u, " PREFIX_MYJET(t) " *v)\n\
{\n\
  " I " m, n, k, i, ns = " MAX_NUM_SYMBOLS_NAME ";\n\
  " PREFIX_MYJET(t) " tmp=NULL, *tmp2=&" PREFIX_MYJET(pool) "[ns+1];\n\
  \n\
  for (i = 0; i < ns; ++i)\n\
    {\n\
      " PREFIX_MYJET(set_zero) "(w[i]);\n\
//      " PREFIX_TREE(set_zero) "(&w[i][0]);\n\
      " PREFIX_TREE(set) "(&w[i][1], &u[i][1]);\n\
      \n\
      " PREFIX_MYJET(set_zero) "(tmp2[i]);\n\
    }\n\
  \n\
  " PREFIX_MYJET(ipool) " = 0;\n\
  k = " PREFIX_MYJET(get_deg) "();\n\
  for (m = 2; m <= k; ++m)\n\
    {\n\
      for (n = m; n > 0; --n) for (i = 0; i < ns; ++i)\n\
        {\n\
          tmp = " PREFIX_TREE(compoh) "(&v[i][n], w);\n\
          " PREFIX_MYJET(add2) "(tmp2[i], tmp2[i], tmp); /* it could just be homog */\n\
          " PREFIX_MYJET(pool) "[--" PREFIX_MYJET(ipool) "] = tmp;\n\
        }\n\
      for (i = 0; i < ns; ++i)\n\
        {\n\
          " PREFIX_TREE(sub2) "(&w[i][m], &u[i][m], &tmp2[i][m]);\n\
          " PREFIX_MYJET(set_zero) "(tmp2[i]);\n\
        }\n\
    }\n\
}\n\
\n\
void " PREFIX_MYJET(fprintf) "(FILE *file, const char *fmt, " PREFIX_MYJET(t) " s)\n\
{\n\
  " I " i;\n\
  for (i=0;i <" MAX_COEFFS_COUNT_NAME "; i++){" PREFIX_MYCOEF(fprintf) "(file, fmt, " MYDATA_ACCESS "(s,i));}\n\
}\n\
\n\
void " PREFIX_MYJET(fscanf) "(FILE *file, const char *fmt, " PREFIX_MYJET(t) " s)\n\
{\n\
  " I " i;\n\
  for (i=0;i <" MAX_COEFFS_COUNT_NAME "; i++){" PREFIX_MYCOEF(fscanf) "(file, fmt, " MYDATA_ACCESS "(s,i));}\n\
}\n\
void " PREFIX_MYJET(genidx) "(" I " c[], " I " n)\n\
{\n\
  " I " deg, i, in;\n\
  for (in = 0; in < n; ++in) if (c[in] != 0) break;\n\
  if (in == n) return;\n\
  if (in == n-1) { c[0] = c[n-1]; c[n-1] = 0; return; }\n\
  deg = c[in]+c[in+1];\n\
  c[in+1]++;\n\
  for (i = 1; i <= in; ++i) c[i] = 0;\n\
  c[0] = deg-c[in+1];\n\
}\n\
\n\
static char *" PREFIX_MYJET(sscanf4) "(char *ptr, const char *myfmt, " PREFIX_MYJET(t) " s, int *l)\n\
{\n\
  " I " i;\n\
  for (i = 0; i <" MAX_COEFFS_COUNT_NAME " && *ptr!=\'\\0\'; i++){\n\
    ptr = " PREFIX_MYCOEF(sscanf4) "(ptr, myfmt, " MYDATA_ACCESS "(s,i), l);\n\
  }\n\
  for ( ; i <" MAX_COEFFS_COUNT_NAME "; i++) {" PREFIX_MYCOEF(set_zero) "(" MYDATA_ACCESS "(s,i));}\n\
  return ptr;\n\
}\n\
void " PREFIX_MYJET(sscanf) "(const char *str, const char *fmt, " PREFIX_MYJET(t) " s)\n\
{\n\
  int l;\n\
  const int len=fmt?strnlen(fmt,128):0; /* NOTE: safety string length of 128 */\n\
  char myfmt[len+3], *mystr = strdup(str);\n\
  sprintf(myfmt, \"\%s%%n\",fmt);\n\
  " PREFIX_MYJET(sscanf4) "(mystr, myfmt, s, &l);\n\
  free(mystr);\n\
}\n\
\n" \

#define MY_JET_TREE_POSTCODE(PREFIX_MYJET,PREFIX_TREE,PREFIX_MYCOEF,I,\
  MAX_NUM_SYMBOLS_NAME,MAX_DEGREE_NAME,MAX_COEFFS_COUNT_NAME) "\
/* END CODE " PREFIX_MYJET(t) " */\n\
" \


#endif /* MY_JET_TREE_H */
