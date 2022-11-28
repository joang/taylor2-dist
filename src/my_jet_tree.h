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
#define MY_JET_TREE_TYPE1(PREFIX_JET_TREE,\
  MY_TREE_TYPE,PREFIX_TREE,SCAL,I) "\
" MY_TREE_TYPE(PREFIX_TREE,SCAL,I) "\n\
\n\
typedef " PREFIX_TREE(t) "* " PREFIX_JET_TREE(t) ";\n\
//#define " PREFIX_JET_TREE(ptr) " " PREFIX_JET_TREE(t) "\n\
typedef " PREFIX_JET_TREE(t) "* " PREFIX_JET_TREE(ptr) ";\n\
" \

#define MY_JET_TREE_FIXES MY_JET_SPACE
#define MY_JET_TREE_TYPE(PREFIX_JET,SCAL,I) \
  MY_JET_TREE_TYPE1(PREFIX_JET,MY_TREE_TYPE,MY_TREE_FIXES,SCAL,I)

#define MY_JET_TREE_API(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_SCAL,SCAL,I) \
  MY_JET_APIS(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_JET_TREE_PREHEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_JET_TREE(t) " */\n " \

#define MY_JET_TREE_HEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_SCAL,SCAL,I) "\
" MY_JET_TREE_API(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_SCAL,SCAL,I) "\n" \

#define MY_JET_TREE_POSTHEADER(PREFIX_JET_TREE,MY_JET_TREE_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_JET_TREE(t) " */\n" \

/* *******  *****************  ******* */
/* *******  MY_JET_TREE_CODE  ******* */
/* *******  *****************  ******* */
#define MY_JET_TREE_PRECODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_SCAL,I) "\
" MY_SCAL_MACROS(PREFIX_SCAL) "\n\
/* CODE " PREFIX_JET_TREE(t) " */\n\
int * " PREFIX_JET_TREE(monomial_counts) "(void) {return _monomial_counts_;}\n\
int * " PREFIX_JET_TREE(monomial_offsets) "(void) {return _monomial_offsets_;}\n\
\n\
#include <stdlib.h>\n\
#include <stdio.h>\n\
#include <math.h>\n\
" \

#define MY_JET_TREE_CODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_SCAL,I) "\
\n\
#define my_scal_tmp " PREFIX_JET_TREE(saux) "\n\
#define my_jet_tmp " PREFIX_JET_TREE(jaux) "\n\
static " PREFIX_SCAL(t) " my_scal_tmp;\n\
static " PREFIX_JET_TREE(t) " my_jet_tmp=NULL;\n\
#pragma omp threadprivate(my_scal_tmp,my_jet_tmp)\n\
\n\
#ifndef _NUMBER_OF_MAX_SYMBOLS_\n\
#define _NUMBER_OF_MAX_SYMBOLS_ 1\n\
#endif\n\
#ifndef _MAX_DEGREE_OF_JET_VARS_\n\
#define _MAX_DEGREE_OF_JET_VARS_ 0\n\
#endif\n\
#define max_nsymb _NUMBER_OF_MAX_SYMBOLS_ \n\
#define max_deg _MAX_DEGREE_OF_JET_VARS_ \n\
static int flag_init_jet_library=0;\n\
static " I " nsymb=0,deg=0;\n\
#pragma omp threadprivate(nsymb,deg)\n\
\n\
#define " PREFIX_SCAL(axpy) "(y,a,x) {\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,a,x);\\\n\
  " PREFIX_SCAL(add2) "(y,y,my_scal_tmp);}\n\
#define " PREFIX_SCAL(sixpy) "(y,a,x) {\\\n\
  " PREFIX_SCAL(mul2_si) "(my_scal_tmp,x,a);\\\n\
  " PREFIX_SCAL(add2) "(y,y,my_scal_tmp);}\n\
\n\
#define " PREFIX_SCAL(axpy_scal) "(y,s,a,x) {\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,a,x);\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,s);\\\n\
  " PREFIX_SCAL(add2) "(y,y,my_scal_tmp);}\n\
#define " PREFIX_SCAL(axpy_si) "(y,s,a,x) {\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,a,x);\\\n\
  " PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,s);\\\n\
  " PREFIX_SCAL(add2) "(y,y,my_scal_tmp);}\n\
\n\
\n\
#define " PREFIX_SCAL(axmy) "(y,a,x) {\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,a,x);\\\n\
  " PREFIX_SCAL(sub2) "(y,y,my_scal_tmp);}\n\
#define " PREFIX_SCAL(sixmy) "(y,a,x) {\\\n\
  " PREFIX_SCAL(mul2_si) "(my_scal_tmp,x,a);\\\n\
  " PREFIX_SCAL(sub2) "(y,y,my_scal_tmp);}\n\
\n\
#define " PREFIX_SCAL(axmy_scal) "(y,s,a,x) {\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,a,x);\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,my_scal_tmp,s);\\\n\
  " PREFIX_SCAL(sub2) "(y,y,my_scal_tmp);}\n\
#define " PREFIX_SCAL(axmy_si) "(y,s,a,x) {\\\n\
  " PREFIX_SCAL(mul2) "(my_scal_tmp,a,x);\\\n\
  " PREFIX_SCAL(mul2_si) "(my_scal_tmp,my_scal_tmp,s);\\\n\
  " PREFIX_SCAL(sub2) "(y,y,my_scal_tmp);}\n\
\n\
" MY_TREE_CODE(PREFIX_TREE,PREFIX_SCAL,I) "\n\
\n\
size_t " PREFIX_JET_TREE(init) "(" PREFIX_JET_TREE(ptr) " s)\n\
{\n\
  // assert(s);\n\
  " I " i, j;\n\
  " PREFIX_SCAL(t) " *coef=NULL;\n\
  size_t mem, k;\n\
\n\
  k = (max_deg + 1) * sizeof(**s);\n\
  *s = (__typeof__(*s)) malloc(k);\n\
  if (*s==NULL) {\n\
    fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
    fflush(stderr); exit(2);\n\
   }\n\
  mem = k;\n\
  k = _JET_COEFFICIENTS_COUNT_TOTAL_;\n\
  coef = (__typeof__(coef)) malloc(k * sizeof(*coef));\n\
  if (coef==NULL) {\n\
    fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
    fflush(stderr); exit(2);\n\
   }\n\
  for (j = 0; j < k; ++j) {" PREFIX_SCAL(init) "(coef[j]);}\n\
  mem+= k * sizeof(*coef);\n\
  for (i = 0; i <= max_deg; ++i)\n\
    {\n\
      /*k = " PREFIX_TREE(nch) "(max_nsymb, i);\n\
      coef = (__typeof__(coef)) malloc(k * sizeof(*coef));\n\
      if (coef==NULL) {\n\
        fprintf(stderr,\"\%s:\%d Not enough memory\\n\",__FILE__,__LINE__);\n\
        fflush(stderr); exit(2);\n\
       }\n\
      for (j = 0; j < k; ++j) {" PREFIX_SCAL(init) "(coef[j]);}\n\
      mem += k * sizeof(*coef);*/\n\
      mem += " PREFIX_TREE(init) "((*s) + i, max_nsymb, i, coef);\n\
      coef+= " PREFIX_TREE(nch) "(max_nsymb, i);\n\
    }\n\
  return mem;\n\
}\n\
\n\
void " PREFIX_JET_TREE(initup) "(" I " ns, " I " dg)\n\
{\n\
  if (flag_init_jet_library==1) return;\n\
#pragma omp single\n\
{\n\
  if (ns > max_nsymb) {\n\
    fprintf(stderr, \"\%s:\%d nsymb value illegal \%d > \%d\\n\", __FILE__,__LINE__, ns, max_nsymb);\n\
    fflush(stderr); exit(1);\n\
  }\n\
  if (dg > max_deg) {\n\
    fprintf(stderr, \"\%s:\%d deg value illegal \%d > \%d\\n\", __FILE__,__LINE__, dg, max_deg);\n\
    fflush(stderr); exit(1);\n\
  }\n\
  /*max_nsymb = ns;\n\
  max_deg = dg;*/\n\
}\n\
  nsymb = ns;\n\
  deg = dg;\n\
  " PREFIX_JET_TREE(init) "(&my_jet_tmp);\n\
  " PREFIX_SCAL(init) "(my_scal_tmp);\n\
  flag_init_jet_library=1;\n\
}\n\
\n\
void " PREFIX_JET_TREE(clean) "(" PREFIX_JET_TREE(ptr) " s)\n\
{\n\
  if (s==NULL || *s==NULL) return;\n\
\n\
  " I " j, k = _JET_COEFFICIENTS_COUNT_TOTAL_;\n\
  __typeof__(*s) st, sf;\n\
  for (st = *s, sf = st + max_deg;\n\
       st < sf;\n\
       ++st) {\n\
    " PREFIX_TREE(clean) "(st);\n\
    /*free(st->coef); st->coef = NULL;*/\n\
  }\n\
  " PREFIX_TREE(clean) "(st);\n\
  /*free(st->coef); st->coef = NULL;*/\n\
  for (j = 0; j < k; ++j) {" PREFIX_SCAL(clean) "((*s)->coef[j]);}\n\
  free((*s)->coef); (*s)->coef = NULL;\n\
\n\
  free(*s); *s = NULL;\n\
}\n\
\n\
void " PREFIX_JET_TREE(cleanup) "(void)\n\
{\n\
  if (flag_init_jet_library==0) return;\n\
  " PREFIX_SCAL(clean) "(my_scal_tmp);\n\
  " PREFIX_JET_TREE(clean) "(&my_jet_tmp);\n\
  nsymb = 0;\n\
  deg = 0;\n\
/*#pragma omp single\n\
{\n\
  max_nsymb = 0;\n\
  max_deg = 0;\n\
}*/\n\
  flag_init_jet_library=0;\n\
}\n\
\n\
" I " " PREFIX_JET_TREE(get_num_symbs) "(void)\
{\
return nsymb;\
}\n\
\n\
" I " " PREFIX_JET_TREE(get_deg) "(void)\
{\
return deg;\
}\n\
\n\
" I " " PREFIX_JET_TREE(set_num_symbs) "(" I " nsymbs)\n\
{\n\
  fprintf(stderr,\"%s:%d Action not allowed\\n\",__FILE__,__LINE__); fflush(stderr); exit(1);\n\
  return 0;\n\
}\n\
\n\
" I " " PREFIX_JET_TREE(set_deg) "(" I " degree)\n\
{\n\
  " I " k=deg;\n\
  if (degree > max_deg) {\n\
    fprintf(stderr,\"%s:%d Invalid degree \%d > \%d\\n\",__FILE__,__LINE__,degree,max_deg);\n\
    fflush(stderr); exit(1);\n\
  }\n\
  deg = degree;\n\
  return k;\n\
}\n\
\n\
void " PREFIX_JET_TREE(set) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  // assert(s && a); // assert(s->nsymb == a->nsymb); // assert(deg == deg);\n\
  if (s == a) return;\n\
\n\
  __typeof__(s) st, at, sf;\n\
  for (st = s, sf = st + deg, at= a;\n\
       st < sf;\n\
       " PREFIX_TREE(set) "(st,at), ++st, ++at){}\n\
  " PREFIX_TREE(set) "(st,at);\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_scal) "(" PREFIX_JET_TREE(t) " s, " PREFIX_SCAL(t) " scal)\n\
{\n\
  // assert(s && *s);\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_scal) "(s,scal);\n\
  for (st = s, sf = st + deg;\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_d) "(" PREFIX_JET_TREE(t) " s, double d)\n\
{\n\
  // assert(s && *s);\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_d) "(s,d);\n\
  for (st = s, sf = st + deg;\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
\n\
\n\
void " PREFIX_JET_TREE(set_si) "(" PREFIX_JET_TREE(t) " s, int d)\n\
{\n\
  // assert(s && *s);\n\
  __typeof__(s) st, sf;\n\
\n\
  " PREFIX_TREE(set_si) "(s,d);\n\
  for (st = s, sf = st + deg;\n\
       st < sf;\n\
       ++st, " PREFIX_TREE(set_si) "(st,0)){}\n\
}\n\
void " PREFIX_JET_TREE(set_scal_array) "(" PREFIX_JET_TREE(t) " s," PREFIX_SCAL(t) " *b)\n\
{\n\
  int i;\n\
\n\
  for (i=0;i <_JET_COEFFICIENTS_COUNT_TOTAL_;\n\
        b++, i++){" PREFIX_SCAL(set) "( MY_JET_DATA(s,i) ,*b);}\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_jet_2_scal_array) "(" PREFIX_SCAL(t) " *b, " PREFIX_JET_TREE(t) " s)\n\
{\n\
 int i;\n\
\n\
  for (i=0 ; i < _JET_COEFFICIENTS_COUNT_TOTAL_;\n\
        b++, i++){" PREFIX_SCAL(set) "(*b, MY_JET_DATA(s,i) );}\n\
}\n\
\n\
void " PREFIX_JET_TREE(to_scal) "(" PREFIX_SCAL(t) " coef[1], " PREFIX_JET_TREE(t) " s)\n\
{\
  " PREFIX_SCAL(set) "(*coef,*get_zero_coef(s));\
}\n\
\n\
void " PREFIX_JET_TREE(get_coef) "(" PREFIX_SCAL(t) " val[1], " PREFIX_JET_TREE(t) " s, const " I " idx[])\n\
{\n\
  " I " i, k;\n\
  for (i = 0, k = 0; i < s->nsymb; k += idx[i], ++i){}\n\
  \n\
  " PREFIX_SCAL(set) "(*val,*" PREFIX_TREE(get_coef) "(s+k, idx));\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_coef) "(" PREFIX_JET_TREE(t) " s, const " I " idx[], " PREFIX_SCAL(t) " val)\n\
{\n\
  " I " i, k;\n\
  for (i = 0, k = 0; i < s->nsymb; k += idx[i], ++i){}\n\
  \n\
  " PREFIX_SCAL(set) "(*" PREFIX_TREE(get_coef) "(s+k, idx),val);\n\
}\n\
\n\
void " PREFIX_JET_TREE(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_TREE(t) " s)\n\
{\n\
  __typeof__(s) st, sf;\n\
  " PREFIX_SCAL(set_si) "(*nrm,0);\n\
  for (st = s, sf = st + deg;\n\
       st < sf;\n\
       " PREFIX_TREE(nrminf) "(nrm,st), ++st){}\n\
   " PREFIX_TREE(nrminf) "(nrm,st);\n\
}\n\
\n\
void " PREFIX_JET_TREE(nrm2) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_JET_TREE(t) " s)\n\
{\n\
  __typeof__(s) st, sf;\n\
  " PREFIX_SCAL(set_si) "(*nrm,0);\n\
  for (st = s, sf = st + deg;\n\
       st < sf;\n\
       " PREFIX_TREE(nrm2) "(nrm,st), ++st){}\n\
   " PREFIX_TREE(nrm2) "(nrm,st);\n\
}\n\
\n\
void " PREFIX_JET_TREE(neg) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a )\n\
{\n\
  // assert(s && a); // assert(s->nsymb == a->nsymb); // assert(deg == deg);\n\
  // assert(*s && *a);\n\
  if (s == a) return;\n\
\n\
  __typeof__(s) st, at, sf;\n\
  for (st = s, sf = st + deg, at = a;\n\
       st < sf;\n\
       " PREFIX_TREE(neg) "(st,at), ++st, ++at){}\n\
  " PREFIX_TREE(neg) "(st,at);\n\
}\n\
\n\
void " PREFIX_JET_TREE(add2) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_JET_TREE(t) " b)\n\
{\n\
  // assert(s && a && b);\n\
\n\
  " I " i;\n\
  for (i = 0; i <= deg; ++i) {\
" PREFIX_TREE(add2) "(s+i, a+i, b+i);}\n\
}\n\
\n\
void " PREFIX_JET_TREE(sub2) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_JET_TREE(t) " b)\n\
{\n\
  // assert(s && a && b);\n\
\n\
  " I " i;\n\
  for (i = 0; i <= deg; ++i) " PREFIX_TREE(sub2) "(s+i, a+i, b+i);\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_sq) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_SCAL(t) " *a0, *sc, *sf;\n\
  sc = get_zero_coef(s);\n\
  a0 = get_zero_coef(a);\n\
  " PREFIX_SCAL(mul2) "((*sc),(*a0),(*a0));\n\
\n\
  " I " i, j, k, ord=deg;\n\
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
void " PREFIX_JET_TREE(mul2) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_JET_TREE(t) " b)\n\
{\n\
// assert(s && a && b); // assert(s != a && s != b);\n\
// assert(*s && *a && *b);\n\
  " I " i, j;\n\
  " PREFIX_SCAL(t) " *sc,*sf;\n\
\n\
  if (a == b) {" PREFIX_JET_TREE(set_sq) "(s,a); return;}\n\
  for (i = 0; i <= deg; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j <= i; ++j)\n\
        {" PREFIX_TREE(pph) "(s+i, a+j, b+(i-j));}\n\
    }\n\
}\n\
\n\
void " PREFIX_JET_TREE(div2) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_JET_TREE(t) " b)\n\
{\n\
  // assert(s && a && b); // assert(s != b);\n\
  // assert(*s && *a && *b);\n\
  " PREFIX_SCAL(t) " *b0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  sc = get_zero_coef(a);\n\
  b0 = get_zero_coef(b);\n\
  sf = get_zero_coef(s);\n\
  " PREFIX_SCAL(div2) "(*sf,*sc,*b0);\n\
  for (i = 1; i <= deg; ++i)\n\
    {\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, b + (i-j));}\n\
      " PREFIX_TREE(div2_scal) "(s[i], s[i],(*b0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
void " PREFIX_JET_TREE(add2_scal) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(add2) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(sub2_scal) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(sub2) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(scal_sub2) "(" PREFIX_JET_TREE(t) " s, " PREFIX_SCAL(t) " b, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(sub2) "(*(s->coef),b,*(a->coef));\n\
  for (st = s, at= a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(neg) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(mul2_scal) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
  " I " k;\n\
  " PREFIX_SCAL(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= deg; ++k)\n\
  {" PREFIX_TREE(mul2_scal) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_JET_TREE(div2_scal) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_SCAL(t) " b)\n\
{\n\
  // assert(s && a);\n\
  // assert(scal);\n\
\n\
  " PREFIX_SCAL(t) " *sc, *scf, *ac;\n\
  __typeof__(s) st, sf, at;\n\
  for (st = s, sf = st + deg, at= a;\n\
       st < sf;\n\
       ++st, ++at) {" PREFIX_TREE(div2_scal) "(st[0], at[0], b, sc,scf,ac);}\n\
  " PREFIX_TREE(div2_scal) "(st[0], at[0], b, sc,scf,ac);\n\
}\n\
\n\
void " PREFIX_JET_TREE(scal_div2) "(" PREFIX_JET_TREE(t) " s, " PREFIX_SCAL(t) " b, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(scal);\n\
  " PREFIX_SCAL(t) " *a0, *sc,*sf,*sc1;\n\
  " I " i, j;\n\
  a0 = get_zero_coef(a);\n\
  sf = get_zero_coef(s);\n\
  " PREFIX_SCAL(div2) "(*sf,b,*a0);\n\
  for (i = 1; i <= deg; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {" PREFIX_TREE(mph) "(s+i, s+j, a+(i-j));}\n\
      " PREFIX_TREE(div2_scal) "(s[i], s[i],(*a0), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
\n\
void " PREFIX_JET_TREE(add2_d) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, double b)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(add2_d) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(sub2_d) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, double b)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(sub2_d) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(d_sub2) "(" PREFIX_JET_TREE(t) " s, double b, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  fprintf(stderr,\"%s:%d to implement\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_JET_TREE(mul2_d) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, double b)\n\
{\n\
  " I " k;\n\
  " PREFIX_SCAL(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= deg; ++k)\n\
  {" PREFIX_TREE(mul2_d) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_JET_TREE(div2_d) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, double b)\n\
{\n\
  fprintf(stderr,\"%s:%d to implement\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_JET_TREE(d_div2) "(" PREFIX_JET_TREE(t) " s, double b, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  fprintf(stderr,\"%s:%d to implement\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_JET_TREE(add2_si) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, int b)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(add2_si) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(sub2_si) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, int b)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(sub2_si) "(*(s->coef),*(a->coef),b);\n\
  if (s != a) for (st = s, at = a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(set) "(st,at)){}\n\
}\n\
\n\
void " PREFIX_JET_TREE(si_sub2) "(" PREFIX_JET_TREE(t) " s, int b, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  __typeof__(s) st, at, sf = s + deg;\n\
  " PREFIX_SCAL(si_sub2) "(*(s->coef),b,*(a->coef));\n\
  for (st = s, at= a;\n\
       st < sf; \n\
       ++st, ++at, " PREFIX_TREE(neg) "(st,at)){}\n\
//  fprintf(stderr,\"%s:%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_JET_TREE(mul2_si) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, int b)\n\
{\n\
  " I " k;\n\
  " PREFIX_SCAL(t) " *sc, *scf, *ac;\n\
  for (k = 0; k <= deg; ++k)\n\
  {" PREFIX_TREE(mul2_si) "(s[k],a[k],b, sc,scf,ac);}\n\
}\n\
\n\
void " PREFIX_JET_TREE(div2_si) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, int b)\n\
{\n\
  " PREFIX_SCAL(t) " *sc, *scf, *ac;\n\
  __typeof__(s) st, sf, at;\n\
  for (st = s, sf = st + deg, at= a;\n\
       st < sf;\n\
       ++st, ++at) {" PREFIX_TREE(div2_si) "(st[0], at[0], b, sc,scf,ac);}\n\
  " PREFIX_TREE(div2_si) "(st[0], at[0], b, sc,scf,ac);\n\
//  fprintf(stderr,\"%s:%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_JET_TREE(si_div2) "(" PREFIX_JET_TREE(t) " s, int b, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  fprintf(stderr,\"%s:%d to implement\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_sqrt) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  // assert(s && a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_SCAL(t) " *s0,*sc,*scf,*sc2;\n\
  s0 = get_zero_coef(s);\n\
  " PREFIX_SCAL(set_sqrt) "((*s0), *get_zero_coef(a)); // assert(fabs(*s0) > ZERO_TOL);\n\
\n\
  " I " i, j, k;\n\
  for (i = 1; i <= deg; ++i)\n\
    {\n\
       /* odd case */\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      " PREFIX_TREE(div2_si) "(s[i], s[i], 2, sc,scf,sc2);\n\
      k = i/2;\n\
      for (j = 1; j <= k; ++j) " PREFIX_TREE(mph) "(s+i, s+j, s+(i-j));\n\
      " PREFIX_TREE(div2_scal) "(s[i], s[i], *s0, sc,scf,sc2);\n\
\n\
      if (++i > deg) break;\n\
\n\
      /* even case */\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      k = i/2;\n\
      " PREFIX_TREE(mph) "(s+i, s+k, s+k);\n\
      " PREFIX_TREE(div2_si) "(s[i], s[i], 2, sc,scf,sc2);\n\
      for (j = 1; j < k; ++j) " PREFIX_TREE(mph) "(s+i, s+j, s+(i-j));\n\
      " PREFIX_TREE(div2_scal) "(s[i], s[i], *s0, sc,scf,sc2);\n\
    }\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_pow_scal) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a, " PREFIX_SCAL(t) " scal)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
//  if (scal == 2)     {" PREFIX_JET_TREE(set_sq) "(s,a); return;}\n\
//  if (scal == 0.5e0) {" PREFIX_JET_TREE(set_sqrt) "(s,a); return;}\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_SCAL(t) " *a0, *s0, *tmp, *sc,*sf,*sc1;\n\
  tmp = get_zero_coef(my_jet_tmp);\n\
  sc = get_zero_coef(s);\n\
  a0 = get_zero_coef(a); // assert(fabs(*a0) > ZERO_TOL);\n\
  " PREFIX_SCAL(set_pow) "((*sc), *a0, scal);\n\
\n\
  " I " i, j;\n\
  for (i = 1; i <= deg; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\n\
      for (j = 0; j < i; ++j) {\n\
        " PREFIX_SCAL(mul2_si) "(*tmp,scal,i-j);\n\
        " PREFIX_SCAL(sub2_si) "(*tmp,*tmp,j);\n\
        " PREFIX_TREE(pph_scal) "(s+i, *tmp, a+(i-j), s+j);\n\
      }\n\
      " PREFIX_SCAL(mul2_si) "(*tmp,*a0,i);\n\
      " PREFIX_TREE(div2_scal) "(s[i], s[i], (*tmp), sc,sf,sc1);\n\
    }\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_exp) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_SCAL(t) " *s0,*scf,*sc;\n\
  s0 = get_zero_coef(s);\n\
  s0 = get_zero_coef(s);\n\
  " PREFIX_SCAL(set_exp) "((*s0), *get_zero_coef(a));\n\
\n\
  " I " i, j;\n\
  for (i = 1; i <= deg; ++i)\n\
    {\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,scf);\n\
      for (j = 0; j < i; ++j)\n\
      {" PREFIX_TREE(pph_si) "(s+i, i-j, a+(i-j), s+j);}\n\
      " PREFIX_TREE(div2_si) "(s[i], s[i], i, sc,scf,s0);\n\
    }\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_log) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  // assert(s && a); // assert(s != a);\n\
  // assert(*s && *a);\n\
\n\
  " PREFIX_SCAL(t) " *a0, *sc,*sf,*sc1, *tmp;\n\
  tmp = get_zero_coef(my_jet_tmp);\n\
  sc = get_zero_coef(s);\n\
  a0 = get_zero_coef(a);\n\
  " PREFIX_SCAL(set_log) "((*sc), *get_zero_coef(a));\n\
\n\
  " I " i, j;\n\
  for (i = 1; i <= deg; ++i)\n\
    {\n\
      " PREFIX_TREE(set) "(s+i, a+i);\n\
      for (j = 1; j < i; ++j) {\n\
        " PREFIX_SCAL(set_si) "(*tmp, -j);\n\
        " PREFIX_SCAL(div2_si) "(*tmp, *tmp, i);\n\
        " PREFIX_TREE(pph_scal) "(s+i, *tmp, a+(i-j), s+j);\n\
      }\n\
      " PREFIX_TREE(div2_scal) "(s[i], s[i], *a0, sc,sf,sc1);\n\
    }\n\
}\n\
\n\
#define " PREFIX_JET_TREE(sincos) "(s,c,a)\\\n\
{\\\n\
  /* assert(s && c && a); assert(s != a && s != c && c != a);*/\\\n\
  /* assert(*s && *c && *a);*/\\\n\
\\\n\
  " I " i, j;\\\n\
  " PREFIX_SCAL(t) " aux1,aux2;\\\n\
  " PREFIX_SCAL(t) " *a0, *sc, *sf;\\\n\
  " PREFIX_SCAL(init) "(aux1);\\\n\
  " PREFIX_SCAL(init) "(aux2);\\\n\
  a0 = get_zero_coef(a);\\\n\
  " PREFIX_SCAL(set_sin) "((*get_zero_coef(s)),(*a0));\\\n\
  " PREFIX_SCAL(set_cos) "((*get_zero_coef(c)),(*a0));\\\n\
\\\n\
  for (i = 1; i <= deg; ++i)\\\n\
    {\\\n\
      " PREFIX_TREE(set_zero) "((s)[i], sc,sf);\\\n\
      " PREFIX_TREE(set_zero) "((c)[i], sc,sf);\\\n\
\\\n\
      for (j = 0; j < i; ++j)\\\n\
        {\\\n\
          " PREFIX_SCAL(set_si) "(aux1,j);\\\n\
          " PREFIX_SCAL(div2_si) "(aux1,aux1,i);\\\n\
          " PREFIX_SCAL(sub2_si) "(aux2,aux1,1);\\\n\
          " PREFIX_SCAL(si_sub2) "(aux1,1,aux1);\\\n\
\\\n\
          " PREFIX_TREE(pph_scal) "((s)+i, aux1, (a)+(i-j), (c)+j);\\\n\
          " PREFIX_TREE(pph_scal) "((c)+i, aux2, (a)+(i-j), (s)+j);\\\n\
        }\\\n\
    }\\\n\
  " PREFIX_SCAL(clean) "(aux2);\\\n\
  " PREFIX_SCAL(clean) "(aux1);\\\n\
}\\\n\
\n\
void " PREFIX_JET_TREE(set_sin) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  " PREFIX_JET_TREE(sincos) "(s,my_jet_tmp,a);\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_cos) "(" PREFIX_JET_TREE(t) " c, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  " PREFIX_JET_TREE(sincos) "(my_jet_tmp,c,a);\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_tan) "(" PREFIX_JET_TREE(t) " t, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  " I " j,k,ii,jj ,nk;\n\
  " PREFIX_SCAL(t) " *t0,*tcf,*tc;\n\
  " PREFIX_SCAL(set_tan) "(*get_zero_coef(t),*get_zero_coef(a));\n\
  " PREFIX_SCAL(mul2) "(*get_zero_coef(my_jet_tmp),*get_zero_coef(t),*get_zero_coef(t));\n\
\n\
  nk = (deg+1) - ((deg+1)%2==0);\n\
  for (k = 1; k < nk; k++) {\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 0; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k,k-j,a+(k-j),my_jet_tmp+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(add2) "(t+k,t+k,a+k);\n\
\n\
    /* k odd */\n\
    " PREFIX_TREE(set_zero) "(my_jet_tmp[k], tc,tcf);\n\
    for (j = 0; j <= (k-1)/2; j++)\n\
    {" PREFIX_TREE(pph) "(my_jet_tmp+k,t+j,t+(k-j));}\n\
    " PREFIX_TREE(mul2_si) "(my_jet_tmp[k],my_jet_tmp[k],2, tc,tcf,t0);\n\
\n\
    ++k;\n\
\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 0; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k,k-j,a+(k-j),my_jet_tmp+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(add2) "(t+k,t+k,a+k);\n\
\n\
    /* k even */\n\
    " PREFIX_TREE(set_zero) "(my_jet_tmp[k], tc,tcf);\n\
    for (j = 0; j <= (k-2)/2; j++)\n\
    {" PREFIX_TREE(pph) "(my_jet_tmp+k,t+j,t+(k-j));}\n\
    " PREFIX_TREE(mul2_si) "(my_jet_tmp[k],my_jet_tmp[k],2, tc,tcf,t0);\n\
\n\
    j = k/2;\n\
    " PREFIX_TREE(pph) "(my_jet_tmp+k,t+j,t+j);\n\
  }\n\
  if ((deg+1)%2==0) {\n\
    k = deg;\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 0; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k,k-j,a+(k-j),my_jet_tmp+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(add2) "(t+k,t+k,a+k);\n\
  }\n\
\n\
//  fprintf(stderr,\"%s:%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
void " PREFIX_JET_TREE(set_atan) "(" PREFIX_JET_TREE(t) " t, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  " I " j,k,ii,jj;\n\
  " PREFIX_SCAL(t) " *t0,*tcf,*tc;\n\
  " PREFIX_SCAL(set_atan) "(*get_zero_coef(t),*get_zero_coef(a));\n\
  " PREFIX_JET_TREE(set_sq) "(my_jet_tmp,a);\n\
  " PREFIX_JET_TREE(add2_si) "(my_jet_tmp,my_jet_tmp,1);\n\
  for (k = 1; k <= deg; k++) {\n\
    " PREFIX_TREE(set_zero) "(t[k], tc,tcf);\n\
    for (j = 1; j < k; j++) \n\
    {" PREFIX_TREE(pph_si) "(t+k, j, my_jet_tmp+(k-j), t+j);}\n\
    " PREFIX_TREE(div2_si) "(t[k],t[k],k, tc,tcf,t0);\n\
    " PREFIX_TREE(sub2) "(t+k,a+k,t+k);\n\
    " PREFIX_TREE(div2_scal) "(t[k],t[k],*get_zero_coef(my_jet_tmp), tc,tcf,t0);\n\
  }\n\
//  fprintf(stderr,\"%s:%d to test\\n\",__FILE__,__LINE__); exit(1);\n\
}\n\
\n\
#define " PREFIX_JET_TREE(sinhcosh) "(s,c,a)\\\n\
{\\\n\
  /* assert(s && c && a); assert(s != a && s != c && c != a);*/\\\n\
  /* assert(*s && *c && *a);*/\\\n\
\\\n\
  " I " i, j;\\\n\
  " PREFIX_SCAL(t) " aux1;\\\n\
  " PREFIX_SCAL(t) " *a0, *sc,*sf;\\\n\
  " PREFIX_SCAL(init) "(aux1);\\\n\
  a0 = get_zero_coef(a);\\\n\
  " PREFIX_SCAL(set_sinh) "((*get_zero_coef(s)),(*a0));\\\n\
  " PREFIX_SCAL(set_cosh) "((*get_zero_coef(c)),(*a0));\\\n\
\\\n\
  for (i = 1; i <= deg; ++i)\\\n\
    {\\\n\
      " PREFIX_TREE(set_zero) "(s[i], sc,sf);\\\n\
      " PREFIX_TREE(set_zero) "(c[i], sc,sf);\\\n\
\\\n\
      for (j = 0; j < i; ++j)\\\n\
        {\\\n\
          " PREFIX_SCAL(set_si) "(aux1,j);\\\n\
          " PREFIX_SCAL(div2_si) "(aux1,aux1,i);\\\n\
          " PREFIX_SCAL(si_sub2) "(aux1,1,aux1);\\\n\
\\\n\
          " PREFIX_TREE(pph_scal) "(s+i, aux1, a+(i-j), c+j);\\\n\
          " PREFIX_TREE(pph_scal) "(c+i, aux1, a+(i-j), s+j);\\\n\
        }\\\n\
    }\\\n\
  " PREFIX_SCAL(clean) "(aux1);\\\n\
}\\\n\
\n\
void " PREFIX_JET_TREE(set_sinh) "(" PREFIX_JET_TREE(t) " s, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  " PREFIX_JET_TREE(sinhcosh) "(s,my_jet_tmp,a);\n\
}\n\
\n\
void " PREFIX_JET_TREE(set_cosh) "(" PREFIX_JET_TREE(t) " c, " PREFIX_JET_TREE(t) " a)\n\
{\n\
  " PREFIX_JET_TREE(sinhcosh) "(my_jet_tmp,c,a);\n\
}\n\
\n\
void " PREFIX_JET_TREE(eval) "(" PREFIX_SCAL(t) " val[1], " PREFIX_JET_TREE(t) " x, " PREFIX_SCAL(t) " s[])\n\
{\n\
  " I " k, m;\n\
  " PREFIX_SCAL(set_zero) "(*val);\n\
  for (k = deg; k > 0; --k) {\n\
    " PREFIX_TREE(sumeval) " (val, x + k, s);\n\
  }\n\
  " PREFIX_SCAL(add2) "(*val,*val, *(get_zero_coef(x)));\n\
}\n\
\n\
void " PREFIX_JET_TREE(fprintf) "(FILE *file, const char *fmt, " PREFIX_JET_TREE(t) " s)\n\
{\n\
  int i;\n\
  for (i=0;i <_JET_COEFFICIENTS_COUNT_TOTAL_; i++){"  PREFIX_SCAL(fprintf) "(file, fmt,  MY_JET_DATA(s,i) );\n\
 }\n\
}\n\
#undef my_jet_tmp\n\
#undef my_scal_tmp\n\
\n" \

#define MY_JET_TREE_POSTCODE(PREFIX_JET_TREE,PREFIX_TREE,PREFIX_SCAL,I) "\
/* END CODE " PREFIX_JET_TREE(t) " */\n" \


#endif /* MY_JET_TREE_H */
