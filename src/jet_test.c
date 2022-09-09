/*
 * Code used to in taylor test series operations. Most elementary functions are not implemented.
 *
 * This code is included in taylor's output if the JET header is empty.
 *
 */

#ifndef MY_JET_DEFINED

#include <ctype.h>
#include <stdlib.h>

#define _NV_  _JET_COEFFICIENTS_COUNT_TOTAL_
typedef struct { MY_FLOAT c[_JET_COEFFICIENTS_COUNT_TOTAL_];} my_jet_test;
typedef my_jet_test *MY_JET;

#define InitJet(a)                {init_jet_test(&a);}
#define ClearJet(a)               clear_jet_test(a)
#define InitJetParameter(a,i)     {init_jet_parameter(a,i);} /* */

#define NegateJetA(t,s)           negate_jet_a_test(t,s)
#define AssignJetToFloat(t,s)     {assign_jet_to_float_test(&(t),s);}
#define AssignFloatToJet(t,f)     assign_float_to_jet_test(t,f)
#define AssignJetToJet(t,s)       assign_jet_to_jet_test(t,s)
#define AddJetJetA(t,a,b)         add_jet_jet_a_test(t,a,b)
#define AddFloatJetA(t,a,b)       add_float_jet_a_test(t,a,b)
#define AddJetFloatA(t,a,b)       add_jet_float_a_test(t,a,b)

#define MultiplyJetFloatA(t,a,b)  multiply_jet_float_a_test(t,a,b)  
#define MultiplyJetJetA(t,a,b)    multiply_jet_jet_a_test(t,a,b)  
#define MultiplyFloatJetA(t,a,b)  multiply_float_jet_a_test(t,a,b)  

#define DivideFloatJetA(t,a,b)    divide_float_jet_a_test(t,a,b)
#define DivideJetFloatA(t,a,b)    divide_jet_float_a_test(t,a,b)
#define DivideJetJetA(t,a,b)      divide_jet_jet_a_test(t,a,b)

#define SubstractFloatJetA(t,a,b) substract_float_jet_a_test(t,a,b)
#define SubstractJetFloatA(t,a,b) substract_jet_float_a_test(t,a,b)
#define SubstractJetJetA(t,a,b)   substract_jet_jet_a_test(t,a,b)

#define ExponentiateJetCstA(t,s,e) exponentiate_jet_cst_a_test(t,s,e)

#define cosJetA(t,a)              cos_jet_a_test(t,a)
#define sinJetA(t,a)              sin_jet_a_test(t,a)
#define sqrtJetA(t,a)             sqrt_jet_a_test(t,a)

#define InputJetFromString(t,a)   input_jet_from_string_test(t,a)
#define InputJet(t,a,b)           input_jet_test(t,a,b)
#define OutputJet(t)              output_jet_test(t)

#define   tanJetA(r,a)        undefined
#define   atanJetA(r,a)       undefined  
#define   expJetA(r,a)        undefined  
#define   logJetA(r,a)        undefined  
#define   sinhJetA(r,a)       undefined
#define   coshJetA(r,a)       undefined   
#define   tanhJetA(r,a)       undefined  
#define   log10JetA(r,a)      undefined  


int init_jet_test(MY_JET *t) {
  MY_JET a=(MY_JET)malloc(sizeof(my_jet_test));
  memset( (char *)a,0,sizeof(my_jet_test));
  *t = a;
  for(i=0;i<_NV_;i++){  
    InitMyFloat(a->c[i]);
    MakeMyFloatA(a->c[i],0.0);
  }
}

int init_jet_parameter(MY_JET a, int i) {
  MakeMyFloatA(a->c[i+1], 1.0);
}

int clear_jet_test(MY_JET a) {
  (void) free(a);
}

int negate_jet_a_test(MY_JET t,MY_JET s) 
{
  int i; 
  for(i=0;i<_NV_;i++){
    NegateMyFloatA(t->c[i], s->c[i]);
  }
}

int assign_jet_to_float_test(MY_FLOAT *t, MY_JET f) {
  AssignMyFloat(*t, f->c[0]);
}

int assign_float_to_jet_test(MY_JET t, MY_FLOAT f) {
  int i;
  for(i=1; i<_NV_; i++) {
    MakeMyFloatA(t->c[i],0.0);
  }
  AssignMyFloat(t->c[0],f);
}


int assign_jet_to_jet_test(MY_JET t, MY_JET s) {
  int i;
  for(i=0;i<_NV_;i++){
    AssignMyFloat(t->c[i],s->c[i]);
  }
}

int add_jet_jet_a_test(MY_JET t, MY_JET a, MY_JET b) {
  int i; 
  for(i=0;i<_NV_;i++){
    AddMyFloatA(t->c[i],a->c[i],b->c[i]);
  }
}

int add_float_jet_a_test(MY_JET t, MY_FLOAT a, MY_JET b) {
  int i; 
  for(i=0;i<_NV_;i++){
    AssignMyFloat(t->c[i],b->c[i]);
  }
  AddMyFloatA(t->c[0],a,b->c[0]);
}

int add_jet_float_a_test(MY_JET t,MY_JET a, MY_FLOAT b) {
  int i;
  for(i=0;i<_NV_;i++){
    AssignMyFloat(t->c[i],a->c[i]);
  }
  AddMyFloatA(t->c[0],a->c[0],b);
}

int substract_jet_jet_a_test(MY_JET t,MY_JET a,MY_JET b) {
  int i; 
  for(i=0;i<_NV_;i++){
    SubstractMyFloatA(t->c[i],a->c[i],b->c[i]);
  }
}

int substract_float_jet_a_test(MY_JET t, MY_FLOAT a,MY_JET b) {
  int i; 
  for(i=0;i<_NV_;i++){
    NegateMyFloatA(t->c[i],b->c[i]);
  }
  SubstractMyFloatA(t->c[0],a,b->c[0]);
}

int substract_jet_float_a_test(MY_JET t,MY_JET a, MY_FLOAT b) {
  int i; 
  for(i=1;i<_NV_;i++){
    AssignMyFloat(t->c[i], a->c[i]);
  }
  SubstractMyFloatA(t->c[0],a->c[0],b);
}


int multiply_jet_float_a_test(MY_JET t,MY_JET a, MY_FLOAT b) {
  int i; 
  for(i=0;i<_NV_;i++){
    MultiplyMyFloatA(t->c[i],(a->c[i]), b);
  }
}

int multiply_float_jet_a_test(MY_JET t, MY_FLOAT a, MY_JET b) {
  int i; 
  for(i=0;i<_NV_;i++){
    MultiplyMyFloatA(t->c[i],(b->c[i]),a);
  }
}

/* xx */
int multiply_jet_jet_a_test(MY_JET t,MY_JET a,MY_JET b) {
  static MY_FLOAT temp1, temp2, temp3, temp4, a0, b0;
  static int inited = 0;
  int i,j,k,l,n1,n2,s1,s2,nv; 
  MY_FLOAT *ac, *bc, *sa,*sb, *st;

  if(inited == 0) {
    InitMyFloat(temp1); InitMyFloat(temp2);
    InitMyFloat(temp3); InitMyFloat(temp4);
    InitMyFloat(a0); InitMyFloat(b0);
    inited = 1;
  }
  // scaler
  MultiplyMyFloatA(t->c[0],a->c[0],b->c[0]); 
  // degree 1
  s1 = _monomial_offsets_[1];
  n1 = _monomial_counts_[1];
  for(i=1;i<=n1;i++){
    MultiplyMyFloatA(temp1,(b->c[i]), a->c[0]);
    MultiplyMyFloatA(temp2,(a->c[i]), b->c[0]);
    AddMyFloatA(t->c[i],temp1,temp2);
  }
  if(_degree_of_jet_vars_ > 1) {
    //* quadratic
    s2 = _monomial_offsets_[2];
    nv = _number_of_jet_vars_;

    ac = a->c;
    bc = b->c;
    sa = &(a->c[s2]);
    sb = &(b->c[s2]);
    st = &(t->c[s2]);
    AssignMyFloat(a0,a->c[0]);
    AssignMyFloat(b0,b->c[0]);
    for(k=0, i=1; i<= nv; i++) {
      for(j=i; j<=nv; j++) {    
	MultiplyMyFloatA(temp1, a0 ,  sb[k]);
	MultiplyMyFloatA(temp2, b0 ,  sa[k]);
	MultiplyMyFloatA(temp3, ac[i],bc[j]);
	AddMyFloatA(temp4,temp1,temp2);

	if(i != j) {
	  MultiplyMyFloatA(temp1, ac[j],  bc[i]);
	  AddMyFloatA(temp2,temp3,temp4);	  
	  AddMyFloatA(st[k],temp1,temp2);	  	  
	} else {
	  AddMyFloatA(st[k],temp3,temp4);	  
	}
	k++;
      }
    }
  }
}

int divide_jet_float_a_test(MY_JET t,MY_JET a, MY_FLOAT b) {
  int i;
  for(i=0;i<_NV_;i++){
    DivideMyFloatA(t->c[i],(a->c[i]),b);
  }
}

int divide_float_jet_a_test(MY_JET t, MY_FLOAT a, MY_JET b) {
  static MY_JET tmp1, tmp2, tmp3;
  static int initialized = 0;
  static MY_FLOAT b0, temp;

  if(!initialized) {
    InitMyFloat(b0);
    InitMyFloat(temp);
    init_jet_test(&tmp1);
    init_jet_test(&tmp2);
    init_jet_test(&tmp3);
    initialized=1;
  }
  AssignMyFloat(b0, b->c[0]);
  assign_jet_to_jet_test(tmp1, b);
  divide_jet_float_a_test(tmp2, tmp1, b0);
  MakeMyFloatA(tmp2->c[0],0.0);
  multiply_jet_jet_a_test(tmp1,tmp2,tmp2);
  substract_jet_jet_a_test(tmp3,tmp1,tmp2);
  MakeMyFloatA(tmp3->c[0], 1.0);
  DivideMyFloatA(temp,a,b0);
  multiply_jet_float_a_test(t, tmp3, temp);
}

int divide_jet_jet_a_test(MY_JET t,MY_JET a,MY_JET b)  {
  static MY_JET tmp1;
  static MY_FLOAT one;
  static int initialized = 0;

  if(!initialized) {
    InitMyFloat(one);
    init_jet_test(&tmp1);
    initialized=1;
  }
  MakeMyFloatA(one, 1.0);
  divide_float_jet_a_test(tmp1, one, b);
  multiply_jet_jet_a_test(t,a,tmp1);
}

int exponentiate_jet_cst_a_test(MY_JET t, MY_JET b, MY_FLOAT e) {
  static MY_JET tmp1, tmp2, tmp3;
  static int initialized = 0;
  static MY_FLOAT b0, s1, smp1,smp2,smp3,smp4,one;

  if(!initialized) {
    InitMyFloat(b0);
    InitMyFloat(s1);  
    InitMyFloat(smp1);InitMyFloat(smp2);
    InitMyFloat(smp3);InitMyFloat(smp4);
    InitMyFloat(one);    
    init_jet_test(&tmp1);
    init_jet_test(&tmp2);
    init_jet_test(&tmp3);
    initialized=1;
  }
  AssignMyFloat(b0, b->c[0]);
  ExponentiateMyFloatA(s1,b0,e);

  MultiplyMyFloatA(smp4,s1,e);

  assign_jet_to_jet_test(tmp1, b);
  divide_jet_float_a_test(tmp2, tmp1, b0);
  MakeMyFloatA(tmp2->c[0],0.0);
  multiply_jet_jet_a_test(tmp3,tmp2,tmp2);  
  MakeMyFloatA(one, 1.0);
  MakeMyFloatA(smp1, 0.5);
  SubstractMyFloatA(smp2,e,one);
  MultiplyMyFloatA(smp3,smp1,smp2);
  MultiplyMyFloatA(smp2,smp3,smp4);
  multiply_float_jet_a_test(tmp1, smp2, tmp3);
  multiply_float_jet_a_test(tmp3, smp4, tmp2);  
  add_jet_jet_a_test(t,tmp1,tmp3);
  AssignMyFloat(t->c[0],s1);
}

int sqrt_jet_a_test(MY_JET t,MY_JET a) {
  static int initialized = 0;
  static MY_FLOAT half;
  if(!initialized) {
    InitMyFloat(half);
  }
  MakeMyFloatA(half, 0.5);
  return exponentiate_jet_cst_a_test(t, a, half);
}

int sin_jet_a_test(MY_JET t,MY_JET b) {
  static MY_JET tmp1, tmp2, tmp3;
  static int initialized = 0;
  static MY_FLOAT b0, _c, _s, half,smp;
  if(!initialized) {
    InitMyFloat(b0);
    InitMyFloat(_c);
    InitMyFloat(_s);
    InitMyFloat(half);
    InitMyFloat(smp);
    init_jet_test(&tmp1);
    init_jet_test(&tmp2);
    init_jet_test(&tmp3);
    initialized=1;
  }
  AssignMyFloat(b0,b->c[0]);
  cosMyFloatA(_c,b0);
  sinMyFloatA(_s,b0);

  assign_jet_to_jet_test(tmp1, b);
  MakeMyFloatA(tmp1->c[0],0.0);
  multiply_jet_jet_a_test(tmp2,tmp1,tmp1);    
  MakeMyFloatA(half, -0.5);
  MultiplyMyFloatA(smp, half, _s);
  multiply_float_jet_a_test(tmp3,smp,tmp2);      
  multiply_float_jet_a_test(tmp2,_c,tmp1);      
  add_jet_jet_a_test(t,tmp3,tmp2);  
  AssignMyFloat(t->c[0], _s);
}

int cos_jet_a_test(MY_JET t,MY_JET b) {
  static MY_JET tmp1, tmp2, tmp3;
  static int initialized = 0;
  static MY_FLOAT b0, _c, _s, half,smp;

  if(!initialized) {
    InitMyFloat(b0);
    InitMyFloat(_c);
    InitMyFloat(_s);
    InitMyFloat(half);
    InitMyFloat(smp);
    init_jet_test(&tmp1);
    init_jet_test(&tmp2);
    init_jet_test(&tmp3);
    initialized=1;
  }
  AssignMyFloat(b0,b->c[0]);
  cosMyFloatA(_c,b0);
  sinMyFloatA(_s,b0);
  
  assign_jet_to_jet_test(tmp1, b);
  MakeMyFloatA(tmp1->c[0],0.0);
  multiply_jet_jet_a_test(tmp2,tmp1,tmp1);    
  MakeMyFloatA(half, -0.5);
  MultiplyMyFloatA(smp, half, _c);
  multiply_float_jet_a_test(tmp3,smp,tmp2);      
  NegateMyFloatA(smp, _s);
  multiply_float_jet_a_test(tmp2,smp,tmp1);      
  add_jet_jet_a_test(t,tmp3,tmp2);  
  AssignMyFloat(t->c[0], _c);
}

/* input output */

static char *next_number(char *s, int *j, char *buf) {
  char *p=s + *j;
  int i=0, k=0;
  while(p && *p && (!isdigit(*p)) && *p != '.' && *p != '-') {    k++; p++;  }
  while(p && *p && (isdigit(*p) || *p == '.' || *p == '-')) {  
    buf[i++] = *p++; k++;
  }
  buf[i++] = '\0';
  *j = *j+k;
}

int input_jet_from_string_test(MY_JET a, char *str) {
  static char buf[256]; 
  char *p = str;
  int i, j=0;
  double f;
  for(i=0; i<_NV_; i++) {
    next_number(str, &j, buf);
    f=atof(buf);
    MakeMyFloatA(a->c[i], f);
  }
}

int input_jet_test(MY_JET a, char *prompt, int i) {
  char buf[2048];
  if(i==0) {
    int j;
    fprintf(stderr, "Enter JET values in this order: F0, ");
    for(j=0; j<_NV_; j++) { fprintf(stderr, "v%d, ", j+1);}
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "%s: ", prompt);
  fgets(buf, 2048, stdin);
  input_jet_from_string_test(a, buf);
}

int output_jet_test(MY_JET a) {
  int i;
  for(i=1; i<_NV_; i++) {
    OutputMyFloat(a->c[i]);
    fprintf(stdout, " ");
  }
}
#endif
