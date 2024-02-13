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
#define MY_CLOUD_CODE_C
#include "my_cloud_code.h"

#include "my_cloud.h"

int num_cloud_vars = 0;
int max_cloud_size = 0;

char *prefixMyCloud = NULL;
char *suffixMyCloud = NULL;

#define prefixname prefixMyCloud
#define suffixname suffixMyCloud
static int mycloud_index=0;

char *mycloud_words[] = {\
  MY_CLOUD_PREFIX, MY_CLOUD_SUFFIX, \
};
char *mycloud_names[] = {\
  MY_CLOUD_NAME, \
};

char *mycloud_header_name[] = {\
  MY_CLOUD_HEADER_NAME, \
};

char *mycloud_fun[] = {\
  MY_CLOUD_MY_CLOUD_FUN_NAME, \
};

char *mycloud_preheader[] = {\
  MY_CLOUD_PREHEADER(MY_CLOUD_ENV,\
                     MY_CLOUD_BASE,MY_CLOUD_BASE_TYPE,\
                     MY_CLOUD_MY_REAL,MY_CLOUD_MY_INT), \
};

char *mycloud_header[] = {\
  MY_CLOUD_HEADER(MY_CLOUD_ENV,\
                  MY_CLOUD_BASE,MY_CLOUD_BASE_TYPE,\
                  MY_CLOUD_MY_REAL,MY_CLOUD_MY_INT,\
                  CLOUD_ENV,MY_REAL_ENV,MY_CLOUD_ALL_FUN_HEADER_BY_MACROS), \
};

char *mycloud_precode[] = {\
  MY_CLOUD_PRECODE(MY_CLOUD_ENV,\
                   MY_CLOUD_BASE,MY_CLOUD_BASE_TYPE,\
                   MY_CLOUD_MY_REAL,MY_CLOUD_MY_INT), \
};

char *mycloud_code[] = {\
  MY_CLOUD_CODE(MY_CLOUD_ENV,\
                MY_CLOUD_BASE,MY_CLOUD_BASE_TYPE,\
                MY_CLOUD_MY_REAL,MY_CLOUD_MY_INT,\
                CLOUD_ENV,MY_REAL_ENV,MY_CLOUD_ALL_FUN_CODE_BY_MACROS), \
};

char *mycloud_myjet_api_macros[] = {\
  MY_CLOUD_MY_JET_MACROS(MY_CLOUD_ENV,"&",MY_CLOUD_MAX_SIZE_MACRO_NAME), \
};

void genMyCloudHeader(void)
{
  fprintf(outfile, "/* LIB: ");
  print_and_subs(mycloud_words + mycloud_index*2, mycloud_names[mycloud_index], prefixname, suffixname);
  fprintf(outfile, " */\n");
  fprintf(outfile, "#ifndef %s\n#define %s\n\n", mycloud_header_name[mycloud_index], mycloud_header_name[mycloud_index]);

  print_and_subs(mycloud_words + mycloud_index*2, mycloud_preheader[mycloud_index], prefixname, suffixname);

  fprintf(outfile, "%s\n", MY_FLOAT_MACROS(MY_FLOAT_FIXES));

  print_and_subs(mycloud_words + mycloud_index*2, mycloud_header[mycloud_index], prefixname, suffixname);

  fprintf(outfile, "\n#define MY_CLOUD ");
  print_and_subs(mycloud_words + mycloud_index*2, MY_CLOUD_BASE_TYPE, prefixname, suffixname);
  fprintf(outfile, "\n#define %s(x) ", mycloud_fun[mycloud_index]);
  print_and_subs(mycloud_words + mycloud_index*2, mycloud_names[mycloud_index], prefixname, suffixname);
  fprintf(outfile, " ## _ ## x\n");
  /* in case we have different myclouds
  fprintf(outfile, "#ifndef %s\n", MY_CLOUD_MY_CLOUD_FUN_NAME);
  fprintf(outfile, "#define %s %s\n", MY_CLOUD_MY_CLOUD_FUN_NAME, mycloud_fun[mycloud_index]);
  fprintf(outfile, "#endif\n");
  */

  fprintf(outfile, "\n#endif /* END ");
  print_and_subs(mycloud_words + mycloud_index*2, mycloud_names[mycloud_index], prefixname, suffixname);
  fprintf(outfile, " */\n\n");

  fprintf(outfile, "#ifndef _MY_JET_H_\n#define _MY_JET_H_\n");
  print_and_subs(mycloud_words + mycloud_index*2, mycloud_myjet_api_macros[mycloud_index], prefixname, suffixname);
  fprintf(outfile, "#endif /* _MY_JET_H_ */\n");
}

void genMyCloudCode(void)
{
  fprintf(outfile, "#ifndef MY_CLOUD_CODE_C\n#define MY_CLOUD_CODE_C\n\n");

//  if (!genMyCoef) fprintf(outfile, "%s\n", MY_FLOAT_MACROS(MY_FLOAT_FIXES));

  print_and_subs(mycloud_words + mycloud_index*2, mycloud_precode[mycloud_index], prefixname, suffixname);
  print_and_subs(mycloud_words + mycloud_index*2, mycloud_code[mycloud_index], prefixname, suffixname);

  fprintf(outfile, "\n#endif /* MY_CLOUD_CODE_C */\n\n");
}
