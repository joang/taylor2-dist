#include <stdio.h>
#include "vf.h"

int main(void)
{
  FILE *file=NULL;
  MY_JET x,y;
  
  InitUpJet();
  
  InitJet(x); InitJet(y); 
  
  MY_JET_FUN(sscanf)("1 2 2 1 0 3", "%lf", x);
  
  printf("x= "); OutputJet("%g ", x); printf("\n");
  
  MY_JET_FUN(mul2)(y, x, x);
  
  
  file = fopen("kk.txt", "w");   
  MY_JET_FUN(fprintf)(file, "% .15e\n", y);  
  fclose(file); file=NULL;
  
  file = fopen("kk.txt", "r");   
  MY_JET_FUN(fscanf)(file, "%lf ", x);  
  fclose(file); file=NULL;
  
  ClearJet(y); ClearJet(x); 
  ClearUpJet();
  return 0;
}
