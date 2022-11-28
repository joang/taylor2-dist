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

#ifndef MY_TREE_H
#define MY_TREE_H

/* *******  ***************  ******* */
/* *******  MY_TREE_HEADER   ******* */
/* *******  ***************  ******* */
#define MY_TREE_TYPE1(PREFIX_TREE,SCAL,I) "\
typedef struct " PREFIX_TREE(t) "\n\
{\n\
  " I " nsymb, deg;\n\
  " SCAL " *coef;\n\
  struct " PREFIX_TREE(t) " *term;\n\
} " PREFIX_TREE(t) ";\n\
typedef " PREFIX_TREE(t) "* " PREFIX_TREE(ptr) ";\n\
" \

#define MY_TREE_FIXES(x) MY_JET_SPACE(x) "_node"
#define MY_TREE_TYPE MY_TREE_TYPE1

#define MY_TREE_API(PREFIX_TREE,MY_TREE_TYPE,PREFIX_SCAL,SCAL,I) \

#define MY_TREE_HEADER(PREFIX_TREE,MY_TREE_TYPE,PREFIX_SCAL,SCAL,I) "\
/* HEADER " PREFIX_TREE(t) " */\n\
" MY_TREE_API(PREFIX_TREE,MY_TREE_TYPE,PREFIX_SCAL,SCAL,I) "\
/* END HEADER " PREFIX_TREE(t) " */\n" \

/* *******  *************  ******* */
/* *******  MY_TREE_CODE  ******* */
/* *******  *************  ******* */
#define MY_TREE_CODE(PREFIX_TREE,PREFIX_SCAL,I) "\
/* CODE " PREFIX_TREE(t) " */\n\
\n\
\n\
#define " PREFIX_TREE(nch) "(n,k) num_coefs_homogeneous[(n)*(max_deg+1)+(k)]\n\
\n\
size_t " PREFIX_TREE(create_tree) "(" PREFIX_TREE(ptr) " h, " PREFIX_SCAL(t) " *coef) {\n\
  static " PREFIX_SCAL(t) " *pcoef=NULL;\n\
#pragma omp threadprivate(pcoef)\n\
  " I " n, k, m;\n\
  size_t mem=0;\n\
  \n\
  if (coef) pcoef = coef;\n\
  n = h->nsymb;\n\
  k = h->deg;\n\
  h->coef = pcoef;\n\
  \n\
  if (n==1) { ++pcoef; h->term=NULL; }\n\
  else if (k == 0) { ++pcoef; h->term=NULL; }\n\
  else {\n\
    mem=(k+1)*sizeof(*h->term);\n\
    h->term = (__typeof__(h->term)) malloc(mem);\n\
    if (h->term==NULL) {\n\
      fprintf(stderr,\"\%s:\%d not enough memory to allocate\\n\",__FILE__,__LINE__);\n\
      fflush(stderr); exit(2);\n\
    }\n\
    \n\
    for (m = 0; m <= k; ++m) {\n\
      (h->term[m]).nsymb = n-1;\n\
      (h->term[m]).deg = k-m;\n\
      mem+= " PREFIX_TREE(create_tree) "(h->term + m, NULL);\n\
    }\n\
  }\n\
  return mem;\n\
}\n\
\n\
size_t " PREFIX_TREE(init) "(" PREFIX_TREE(ptr) " h, " I " nsymb, " I " deg, " PREFIX_SCAL(t) " *coef)\n\
{\n\
  // assert(h);\n\
  h->nsymb = nsymb;\n\
  h->deg = deg;\n\
  h->coef = coef;\n\
  h->term = NULL;\n\
  return " PREFIX_TREE(create_tree) "(h,coef);\n\
  size_t mem = 0;\n\
  if (nsymb > 2 && deg > 0)\n\
    {\n\
      __typeof__(h->deg) i;\n\
\n\
      mem = (deg + 1) * sizeof(*h->term);\n\
      h->term = (__typeof__(h->term)) malloc(mem); // assert(h->term);\n\
      --nsymb;\n\
//      for (i = 0; i <= deg; ++i)\n\
      for (i = deg; i > 0; --i)\n\
        {\n\
          mem  += " PREFIX_TREE(init) "(h->term + (deg-i), nsymb, i, coef);\n\
          coef += " PREFIX_TREE(nch) "(nsymb, i);\n\
        }\n\
      mem += " PREFIX_TREE(init) "(h->term + deg, nsymb, i, coef);\n\
    }\n\
  return mem;\n\
}\n\
\n\
void " PREFIX_TREE(clean) "(" PREFIX_TREE(ptr) " h)\n\
{\n\
  if (h->term==NULL) return;\n\
\n\
  __typeof__(h->term) ht, hf;\n\
  for (ht = h->term, hf = ht + h->deg;\n\
       ht < hf;\n\
       " PREFIX_TREE(clean) "(ht), ++ht){}\n\
  " PREFIX_TREE(clean) "(ht);\n\
\n\
  free(h->term); h->term = NULL;\n\
}\n\
\n\
void " PREFIX_TREE(set) "(" PREFIX_TREE(ptr) " h, " PREFIX_TREE(ptr) " a)\n\
{\n\
  // assert(h && a); // assert(h->nsymb == a->nsymb); // assert(h->deg == h->deg);\n\
  if (h == a || h->coef == a->coef) return;\n\
\n\
  __typeof__(h->coef) hc, ac, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg), ac = a->coef;\n\
       hc < hf;\n\
       ++hc, ++ac){" PREFIX_SCAL(set) "((*hc),(*ac));}\n\
}\n\
\n\
void " PREFIX_TREE(set_scal) "(" PREFIX_TREE(ptr) " h, " PREFIX_SCAL(t) " scal)\n\
{\n\
  __typeof__(h->coef) hc, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
       hc < hf;\n\
       ++hc){" PREFIX_SCAL(set) "((*hc),scal);}\n\
}\n\
\n\
void " PREFIX_TREE(set_si) "(" PREFIX_TREE(ptr) " h, int d)\n\
{\n\
  __typeof__(h->coef) hc, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
       hc < hf;\n\
       ++hc){" PREFIX_SCAL(set_si) "((*hc),d);}\n\
}\n\
\n\
void " PREFIX_TREE(set_d) "(" PREFIX_TREE(ptr) " h, double d)\n\
{\n\
  __typeof__(h->coef) hc, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
       hc < hf;\n\
       ++hc){" PREFIX_SCAL(set_d) "((*hc),d);}\n\
}\n\
\n\
#define " PREFIX_TREE(set_zero) "(h,hc,hf)\\\n\
{\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg);\\\n\
       hc < hf;\\\n\
       ++hc){" PREFIX_SCAL(set_zero) "(*hc);}\\\n\
}\\\n\
\n\
void " PREFIX_TREE(nrminf) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_TREE(ptr) " h)\n\
{\n\
  __typeof__(h->coef) hc, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
       hc < hf;\n\
       ++hc)\n\
   {\n\
      " PREFIX_SCAL(set_fabs) "(my_scal_tmp,(*hc));\n\
      if (" PREFIX_SCAL(lt) "(*nrm,my_scal_tmp))\n\
      {" PREFIX_SCAL(set) "(*nrm,my_scal_tmp);}\n\
   }\n\
}\n\
\n\
void " PREFIX_TREE(nrm2) "(" PREFIX_SCAL(t) " nrm[1], " PREFIX_TREE(ptr) " h)\n\
{\n\
  __typeof__(h->coef) hc, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
       hc < hf;\n\
       ++hc)\n\
   {\n\
      " PREFIX_SCAL(mul2) "(my_scal_tmp,(*hc),(*hc));\n\
      " PREFIX_SCAL(add2) "(*nrm,*nrm,my_scal_tmp);\n\
   }\n\
}\n\
\n\
static void " PREFIX_TREE(neg) "(" PREFIX_TREE(ptr) " h, " PREFIX_TREE(ptr) " a)\n\
{\n\
  // assert(h && a); // assert(h->nsymb == a->nsymb); // assert(h->deg == h->deg);\n\
  // assert(h->term && a->term);\n\
//  if (h == a || h->term == a->term) return;\n\
\n\
  __typeof__(h->coef) hc, ac, hf;\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg), ac = a->coef;\n\
       hc < hf;\n\
       ++hc, ++ac){" PREFIX_SCAL(neg) "(*hc,*ac);}\n\
}\n\
\n\
/* y <- y - a*x */\n\
#define " PREFIX_TREE(axmy) "(y,a,x,yc,yf,xc) {\\\n\
  for (yc = y->coef, yf = y->coef + " PREFIX_TREE(nch) "(y->nsymb,y->deg), xc = x->coef;\\\n\
       yc < yf;\\\n\
       ++yc, ++xc){" PREFIX_SCAL(axmy) "((*yc),a,(*xc));}\\\n\
}\n\
\n\
/* y <- y + a*x */\n\
#define " PREFIX_TREE(axpy) "(y,a,x,yc,yf,xc) {\\\n\
  for (yc = y->coef, yf = y->coef + " PREFIX_TREE(nch) "(y->nsymb,y->deg), xc = x->coef;\\\n\
       yc < yf;\\\n\
       ++yc, ++xc){" PREFIX_SCAL(axpy) "((*yc),a,(*xc));}\\\n\
}\n\
\n\
/* y <- y + s*a*x */\n\
#define " PREFIX_TREE(axpy_scal) "(y,s,a,x,yc,yf,xc) {\\\n\
  for (yc = y->coef, yf = y->coef + " PREFIX_TREE(nch) "(y->nsymb,y->deg), xc = x->coef;\\\n\
       yc < yf;\\\n\
       ++yc, ++xc){" PREFIX_SCAL(axpy_scal) "((*yc),s,a,(*xc));}\\\n\
}\n\
#define " PREFIX_TREE(axpy_d) "(y,s,a,x,yc,yf,xc) {\\\n\
  for (yc = y->coef, yf = y->coef + " PREFIX_TREE(nch) "(y->nsymb,y->deg), xc = x->coef;\\\n\
       yc < yf;\\\n\
       ++yc, ++xc){" PREFIX_SCAL(axpy_d) "((*yc),s,a,(*xc));}\\\n\
}\n\
#define " PREFIX_TREE(axpy_si) "(y,s,a,x,yc,yf,xc) {\\\n\
  for (yc = y->coef, yf = y->coef + " PREFIX_TREE(nch) "(y->nsymb,y->deg), xc = x->coef;\\\n\
       yc < yf;\\\n\
       ++yc, ++xc){" PREFIX_SCAL(axpy_si) "((*yc),s,a,(*xc));}\\\n\
}\n\
\n\
/* h <- a + b */\n\
void " PREFIX_TREE(add2) "(" PREFIX_TREE(ptr) " h, " PREFIX_TREE(ptr) " a, " PREFIX_TREE(ptr) " b)\n\
{\n\
  // assert(h && a && b);\n\
  // assert(h->nsymb == a->nsymb && p->nsymb == b->nsymb);\n\
  // assert(h->deg == a->deg && h->deg == b->deg);\n\
  " I " i, k = " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
  for (i = 0; i < k; ++i)\n\
  {" PREFIX_SCAL(add2) "(h->coef[i],a->coef[i],b->coef[i]);}\n\
}\n\
\n\
/* h <- a - b */\n\
static void " PREFIX_TREE(sub2) "(" PREFIX_TREE(ptr) " h, " PREFIX_TREE(ptr) " a, " PREFIX_TREE(ptr) " b)\n\
{\n\
  // assert(h && a && b); // assert(h != b);\n\
  // assert(h->nsymb == a->nsymb && h->nsymb == b->nsymb);\n\
  // assert(h->deg == a->deg && h->deg == b->deg);\n\
  " I " i, k = " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
  for (i = 0; i < k; ++i)\n\
  {" PREFIX_SCAL(sub2) "(h->coef[i],a->coef[i],b->coef[i]);}\n\
}\n\
\n\
/* h <- h + a*b */\n\
void " PREFIX_TREE(pph) "(" PREFIX_TREE(ptr) " h, " PREFIX_TREE(ptr) " a, " PREFIX_TREE(ptr) " b)\n\
{\n\
  // assert(h && a && b); //// assert(h != a && h != b);\n\
  if (h->nsymb > 2)\n\
    {\n\
      __typeof__(h->coef) hc, hf, ac;\n\
      if (a->deg > 0)\n\
        {\n\
          if (b->deg > 0)\n\
            {\n\
              __typeof__(*h) *aa, *bb, *hh, *hh0, *af, *bf;\n\
              af = a->term + a->deg; bf = b->term + b->deg;\n\
              for (aa = a->term, hh0 = h->term; aa < af; ++aa, ++hh0)\n\
                {\n\
                  for (bb = b->term, hh = hh0;\n\
                       bb < bf;\n\
                       " PREFIX_TREE(pph) "(hh, aa, bb), ++bb, ++hh){}\n\
//                  // assert(pp <= p->term + p->deg);\n\
                  " PREFIX_TREE(axpy) "(hh,*(bb->coef),aa, hc,hf,ac);\n\
                }\n\
//              // assert(pp0 <= p->term + p->deg);\n\
              for (bb = b->term, hh = hh0;\n\
                   bb < bf;\n\
                   ++bb, ++hh){" PREFIX_TREE(axpy) "(hh,*(aa->coef),bb, hc,hf,ac);}\n\
              " PREFIX_SCAL(axpy) "(*(hh->coef),*(aa->coef),*(bb->coef));\n\
            }\n\
          else " PREFIX_TREE(axpy) "(h,*(b->coef),a, hc,hf,ac);\n\
        }\n\
      else " PREFIX_TREE(axpy) "(h,*(a->coef),b, hc,hf,ac);\n\
    }\n\
  else if (h->nsymb == 2)\n\
    {\n\
      __typeof__(*h->coef) *aa, *bb, *hh, *hh0, *af, *bf;\n\
      af = a->coef + a->deg; bf = b->coef + b->deg;\n\
      for (aa = a->coef, hh0 = h->coef; aa <= af; ++aa, ++hh0)\n\
        for (bb = b->coef, hh = hh0;\n\
             bb <= bf;\n\
             ++bb, ++hh){" PREFIX_SCAL(axpy) "((*hh),(*aa),(*bb));}\n\
    }\n\
  else " PREFIX_SCAL(axpy) "(*(h->coef), *(a->coef), *(b->coef));\n\
}\n\
\n\
/* h <- h - a*b */\n\
void " PREFIX_TREE(mph) "(" PREFIX_TREE(ptr) " h, " PREFIX_TREE(ptr) " a, " PREFIX_TREE(ptr) " b)\n\
{\n\
  // assert(h && a && b); //// assert(h != a && h != b);\n\
  if (h->nsymb > 2)\n\
    {\n\
      __typeof__(h->coef) hc, hf, ac;\n\
      if (a->deg > 0)\n\
        {\n\
          if (b->deg > 0)\n\
            {\n\
              __typeof__(*h) *aa, *bb, *hh, *hh0, *af, *bf;\n\
              af = a->term + a->deg; bf = b->term + b->deg;\n\
              for (aa = a->term, hh0 = h->term; aa < af; ++aa, ++hh0)\n\
                {\n\
                  for (bb = b->term, hh = hh0;\n\
                       bb < bf;\n\
                       " PREFIX_TREE(mph) "(hh, aa, bb), ++bb, ++hh){}\n\
//                  // assert(pp <= p->term + p->deg);\n\
                  " PREFIX_TREE(axmy) "(hh,*(bb->coef),aa, hc,hf,ac);\n\
                }\n\
//              // assert(pp0 <= p->term + p->deg);\n\
              for (bb = b->term, hh = hh0;\n\
                   bb < bf;\n\
                   ++bb, ++hh){" PREFIX_TREE(axmy) "(hh,*(aa->coef),bb, hc,hf,ac);}\n\
              " PREFIX_SCAL(axmy) "(*(hh->coef),*(aa->coef),*(bb->coef));\n\
            }\n\
          else " PREFIX_TREE(axmy) "(h,*(b->coef),a, hc,hf,ac);\n\
        }\n\
      else " PREFIX_TREE(axmy) "(h,*(a->coef), b, hc,hf,ac);\n\
    }\n\
  else if (h->nsymb == 2)\n\
    {\n\
      __typeof__(*h->coef) *aa, *bb, *hh, *hh0, *af, *bf;\n\
      af = a->coef + a->deg; bf = b->coef + b->deg;\n\
      for (aa = a->coef, hh0 = h->coef; aa <= af; ++aa, ++hh0)\n\
        for (bb = b->coef, hh = hh0;\n\
             bb <= bf;\n\
             ++bb, ++hh) {" PREFIX_SCAL(axmy) "((*hh), (*aa), (*bb));}\n\
    }\n\
  else " PREFIX_SCAL(axmy) "(*(h->coef),*(a->coef),*(b->coef));\n\
}\n\
\n\
/* h <- h + s*a*b */\n\
void " PREFIX_TREE(pph_scal) "(" PREFIX_TREE(ptr) " h, " PREFIX_SCAL(t) " scal, " PREFIX_TREE(ptr) " a, " PREFIX_TREE(ptr) " b)\n\
{\n\
  if (h->nsymb > 2)\n\
    {\n\
      __typeof__(h->coef) hc, hf, ac;\n\
      if (a->deg > 0)\n\
        {\n\
          if (b->deg > 0)\n\
            {\n\
              __typeof__(*h) *aa, *bb, *hh, *hh0, *af, *bf;\n\
              af = a->term + a->deg; bf = b->term + b->deg;\n\
              for (aa = a->term, hh0 = h->term; aa < af; ++aa, ++hh0)\n\
                {\n\
                  for (bb = b->term, hh = hh0;\n\
                       bb < bf;\n\
                       " PREFIX_TREE(pph_scal) "(hh, scal, aa, bb), ++bb, ++hh){}\n\
//                  // assert(pp <= p->term + p->deg);\n\
                  " PREFIX_TREE(axpy_scal) "(hh,scal,*(bb->coef),aa, hc, hf, ac);\n\
                }\n\
//              // assert(pp0 <= p->term + p->deg);\n\
              for (bb = b->term, hh = hh0;\n\
                   bb < bf;\n\
                   ++bb, ++hh){" PREFIX_TREE(axpy_scal) "(hh,scal,*(aa->coef),bb, hc, hf, ac);}\n\
              " PREFIX_SCAL(axpy_scal) "(*(hh->coef),scal,*(aa->coef),*(bb->coef));\n\
            }\n\
          else " PREFIX_TREE(axpy_scal) "(h,scal,*(b->coef),a, hc, hf, ac);\n\
        }\n\
      else {" PREFIX_TREE(axpy_scal) "(h,scal,*(a->coef),b, hc, hf, ac);}\n\
    }\n\
  else if (h->nsymb == 2)\n\
    {\n\
      __typeof__(*h->coef) *aa, *bb, *hh, *hh0, *af, *bf;\n\
      af = a->coef + a->deg; bf = b->coef + b->deg;\n\
      for (aa = a->coef, hh0 = h->coef; aa <= af; ++aa, ++hh0)\n\
        for (bb = b->coef, hh = hh0;\n\
             bb <= bf;\n\
             ++bb, ++hh){" PREFIX_SCAL(axpy_scal) "((*hh), scal, (*aa), (*bb));}\n\
    }\n\
  else {" PREFIX_SCAL(axpy_scal) "(*(h->coef),scal,*(a->coef),*(b->coef));}\n\
}\n\
\n\
void " PREFIX_TREE(pph_si) "(" PREFIX_TREE(ptr) " h, " I " scal, " PREFIX_TREE(ptr) " a, " PREFIX_TREE(ptr) " b)\n\
{\n\
  // assert(h && a && b); //// assert(h != a && h != b);\n\
  if (h->nsymb > 2)\n\
    {\n\
      __typeof__(h->coef) hc, hf, ac;\n\
      if (a->deg > 0)\n\
        {\n\
          if (b->deg > 0)\n\
            {\n\
              __typeof__(*h) *aa, *bb, *hh, *hh0, *af, *bf;\n\
              af = a->term + a->deg; bf = b->term + b->deg;\n\
              for (aa = a->term, hh0 = h->term; aa < af; ++aa, ++hh0)\n\
                {\n\
                  for (bb = b->term, hh = hh0;\n\
                       bb < bf;\n\
                       " PREFIX_TREE(pph_si) "(hh, scal, aa, bb), ++bb, ++hh){}\n\
//                  // assert(pp <= p->term + p->deg);\n\
                  " PREFIX_TREE(axpy_si) "(hh,scal,*(bb->coef),aa, hc, hf, ac);\n\
                }\n\
//              // assert(pp0 <= p->term + p->deg);\n\
              for (bb = b->term, hh = hh0;\n\
                   bb < bf;\n\
                   ++bb, ++hh){" PREFIX_TREE(axpy_si) "(hh,scal,*(aa->coef),bb, hc, hf, ac);}\n\
              " PREFIX_SCAL(axpy_si) "(*(hh->coef),scal,*(aa->coef),*(bb->coef));\n\
            }\n\
          else " PREFIX_TREE(axpy_si) "(h,scal,*(b->coef),a, hc, hf, ac);\n\
        }\n\
      else {" PREFIX_TREE(axpy_si) "(h,scal,*(a->coef),b, hc, hf, ac);}\n\
    }\n\
  else if (h->nsymb == 2)\n\
    {\n\
      __typeof__(*h->coef) *aa, *bb, *hh, *hh0, *af, *bf;\n\
      af = a->coef + a->deg; bf = b->coef + b->deg;\n\
      for (aa = a->coef, hh0 = h->coef; aa <= af; ++aa, ++hh0)\n\
        for (bb = b->coef, hh = hh0;\n\
             bb <= bf;\n\
             ++bb, ++hh){" PREFIX_SCAL(axpy_si) "((*hh), scal, (*aa), (*bb));}\n\
    }\n\
  else {" PREFIX_SCAL(axpy_si) "(*(h->coef),scal,*(a->coef),*(b->coef));}\n\
}\n\
\n\
#define " PREFIX_TREE(mul2_scal) "(h,a,s, hc,hf,ac) {\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg), ac = (a).coef;\\\n\
       hc < hf;\\\n\
       ++hc, ++ac){" PREFIX_SCAL(mul2) "((*hc),(*ac),s);}\\\n\
}\n\
#define " PREFIX_TREE(mul2_si) "(h,a,s, hc,hf,ac) {\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg), ac = (a).coef;\\\n\
       hc < hf;\\\n\
       ++hc, ++ac){" PREFIX_SCAL(mul2_si) "((*hc),(*ac),s);}\\\n\
}\n\
#define " PREFIX_TREE(mul2_d) "(h,a,s, hc,hf,ac) {\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg), ac = (a).coef;\\\n\
       hc < hf;\\\n\
       ++hc, ++ac){" PREFIX_SCAL(mul2_d) "((*hc),(*ac),s);}\\\n\
}\n\
\n\
#define " PREFIX_TREE(div2_scal) "(h,a,s, hc,hf,ac) {\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg), ac = (a).coef;\\\n\
       hc < hf;\\\n\
       ++hc, ++ac){" PREFIX_SCAL(div2) "((*hc),(*ac),s);}\\\n\
}\n\
#define " PREFIX_TREE(div2_si) "(h,a,s, hc,hf,ac) {\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg), ac = (a).coef;\\\n\
       hc < hf;\\\n\
       ++hc, ++ac){" PREFIX_SCAL(div2_si) "((*hc),(*ac),s);}\\\n\
}\n\
#define " PREFIX_TREE(div2_d) "(h,a,s, hc,hf,ac) {\\\n\
  for (hc = (h).coef, hf = hc + " PREFIX_TREE(nch) "((h).nsymb, (h).deg), ac = (a).coef;\\\n\
       hc < hf;\\\n\
       ++hc, ++ac){" PREFIX_SCAL(div2_d) "((*hc),(*ac),s);}\\\n\
}\n\
\n\
#define get_zero_coef(x) (x)->coef\n\
\n\
" PREFIX_SCAL(t) "* " PREFIX_TREE(get_coef) "(" PREFIX_TREE(ptr) " h, const " I " idx[])\n\
{\n\
  " I " k;\n\
  for (k = h->nsymb-1; k >= 0; --k)\n\
    {\n\
      if (h->nsymb==1 || h->deg==0) break;\n\
      h = h->term+idx[k];\n\
    }\n\
  return h->coef;\n\
}\n\
\n\
void " PREFIX_TREE(sumeval) "(" PREFIX_SCAL(t) " val[1], " PREFIX_TREE(ptr) " h, " PREFIX_SCAL(t) " x[])\n\
{\n\
  " I " n, k, m;\n\
  " PREFIX_SCAL(t) " y, x1, xk;\n\
  " PREFIX_SCAL(init) "(y); " PREFIX_SCAL(set_zero) "(y);\n\
  " PREFIX_SCAL(init) "(x1);\n\
  " PREFIX_SCAL(init) "(xk);\n\
  n= h->nsymb;\n\
  k= h->deg;\n\
  " PREFIX_SCAL(set) "(x1,x[n-1]);\n\
  if (k== 0) {\n\
    " PREFIX_SCAL(add2) "(y,y,*(h->coef));\n\
  } else if (n==1) {\n\
    " PREFIX_SCAL(set_si) "(xk,1);\n\
    for (m = 0; m < k; ++m) {\n\
      " PREFIX_SCAL(mul2) "(xk,xk,x1);\n\
    }\n\
    " PREFIX_SCAL(mul2) "(y,*(h->coef),xk);\n\
  } else {\n\
    " PREFIX_TREE(sumeval) "(&y,(h->term)+k, x);\n\
    for (m= k; m>0; m--) {\n\
      " PREFIX_SCAL(mul2) "(y,y,x1);\n\
      " PREFIX_TREE(sumeval) "(&y,h->term+m-1, x);\n\
    }\n\
  }\n\
  " PREFIX_SCAL(add2) "(*val,*val,y);\n\
  " PREFIX_SCAL(clean) "(y);\n\
  " PREFIX_SCAL(clean) "(x1);\n\
  " PREFIX_SCAL(clean) "(xk);\n\
}\n\
\n\
void " PREFIX_TREE(fprintf) "(FILE *const file, const char *fmt, " PREFIX_TREE(ptr) " h)\n\
{\n\
  // assert(h && h->coef);\n\
  " PREFIX_SCAL(t) " *hc, *hf;\n\
\n\
  for (hc = h->coef, hf = hc + " PREFIX_TREE(nch) "(h->nsymb, h->deg);\n\
       hc < hf;\n\
       " PREFIX_SCAL(fprintf) "(file, fmt, *hc), ++hc){}\n\
}\n\
\n\
" \

#include <limits.h>
void print_jet_tree_num_coef_homog_table(int nsymb, int deg)
{
  int **nch=NULL, m, k;

  /* memory allocation */
  k = (nsymb+1)*sizeof(__typeof__(*nch));
  nch = (__typeof__(nch)) malloc(k);
  if (nch == NULL)
    {
      fprintf(stderr, "%s:%d not enough memory to allocate %d\n", __FILE__, __LINE__, k);
      fflush(stderr); exit(2);
    }
  for (m = 0; m <= nsymb; ++m)
    {
      k = (deg+1)*sizeof(__typeof__(**nch));
      nch[m] = (__typeof__(*nch)) malloc(k);
      if (nch[m] == NULL)
        {
          fprintf(stderr, "%s:%d not enough memory to allocate %d\n", __FILE__, __LINE__, k);
          fflush(stderr); exit(2);
        }
    }

  /* table computation */
  nch[0][0] = 1;
  for (k = 1; k <= deg; ++k) {nch[0][k] = 0;}
  for (m = 1; m <= nsymb; ++m)
    {
      nch[m][0]= 1;
      for (k = 1; k <= deg; ++k)
        {
          nch[m][k]= nch[m][k-1] + nch[m-1][k];
          if (nch[m][k]> UINT_MAX - nch[m-1][k])
            {
              fprintf(stderr, "%s:%d impossible to generate table for nsymb=%d and deg=%d\n", __FILE__, __LINE__, nsymb, deg);
              fflush(stderr); exit(3);
            }
        }
    }

  /* printing */
  fprintf(outfile, "/*table for %d symbols and degree %d*/\n",num_symbols, deg_jet_vars);
  fprintf(outfile, "static int num_coefs_homogeneous[] = {\\\n");
  for (m = 0; m <= nsymb; ++m)
    {
      for (k = 0; k <= deg; k++)
        {
          fprintf(outfile, "%d, ", nch[m][k]);
        }
      fprintf(outfile, "\\\n");
    }
  fprintf(outfile, "};\n");

  /* deallocate memory */
  for (m = 0; m <= nsymb; ++m) {free(nch[m]); nch[m]=NULL;}
  free(nch); nch=NULL;
}

#endif /* MY_TREE_H */
