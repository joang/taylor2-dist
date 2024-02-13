#include <stdio.h>
#include <stdlib.h>

// test taylor preprocessor
// The preprocessor implements two macros
// #loop and #if
// Sample input is like
//
// #loop i=1,5
//  #if $i != 3
//   x$i'=y$i
//  #endif 
// #endloop
// 

extern char *taylor_preprocess(char *filename, char *in);
extern FILE *preprocessed_file(FILE *fp, char *filename);

char *saved_input_file = NULL;

int main(int ac, char **av)
{
  char *fname, buffer[256], *p;
  FILE *fp;

  fname = NULL;
  //  if(ac < 2) {
  //    fprintf(stderr,"Usage %s file\n", av[0]);
  //    exit(1);
  //  }
  if(ac >= 2)
    fname = av[1];
  
  // p = taylor_preprocess(fname, NULL);
  // fprintf(stdout, "%s\n", p);
  
  fp=preprocessed_file(fname == NULL? stdin : NULL, fname);
  
  while(fgets(buffer, 256, fp)) {
    p = buffer;
    fprintf(stdout, "%s", p);
  }
  fclose(fp);



  return 0;
}
