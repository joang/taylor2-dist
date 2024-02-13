
#include "tp_prep.h"

extern int isletter(int);
extern int equals(int, char *);
extern void copy_str(char*, int);
extern void capture(char*, int, int);
extern void int_error(char*, int);
extern int scanner(char *);
extern void CALC_DefineFunctionOrVariable(char *, char **, int *);

extern char *xmy_malloc(int s);
extern char *xmy_realloc(void *p, int s);
extern void xmy_free(void *ptr);

int match1Word(const char *buf, char *word, int buflen, int *pos_ret);
int match2Words(const char *buf, char *word, char *word2, int buflen, int *pos_ret);
void tp_error(char *format, char *txt);

//===============================================
extern char input_line[];
//===============================================
static int tp_count = 0;
static int tp_debug = 0;
//===============================================

#define TPB_HEAD    -1
#define TPB_NORMAL   0
#define TPB_LOOP     1
#define TPB_IF       2
#define TPB_ELIF     6
#define TPB_ELSE     10
#define TPB_UNKNOWN  127

typedef struct tp_buff_ {
  int len, slen;
  char *buffer;
  char *p;
} tp_buf;

typedef struct tp_block_ {
  int type, id;
  char args[256];
  char var[256];    // for loops
  char start[256];  
  char end[256];
  char incr[256];  
  char cond[256]; // for ifs
  char vbuf[256];
  int nsegs;

  tp_buf original; // original content
  tp_buf expanded; // expanded content
  char *buffer;    // working buffer
  char *buffer2;   // working buffer
  char **segs;     // segments

  struct tp_block_ *head, *sibling; // for elif & else
  struct tp_block_ *prev;
  struct tp_block_ *next;  
  
} tp_block, *tp_block_ptr;

tp_block *tp_new_block() {
  tp_block *a = (tp_block *)xmy_malloc( sizeof(tp_block));
  tp_buf *buf;
  a->id = tp_count++;
  buf = &(a->original);
  buf->buffer = (char *)xmy_malloc( 256 * sizeof(char));
  buf->slen = 256;
  return a;
}

void tp_destroy_block(tp_block *block_in) {
  tp_block *block = block_in;
  while(block) {
    if(block->original.buffer) xmy_free(block->original.buffer);
    if(block->expanded.buffer) xmy_free(block->expanded.buffer);
    if(block->buffer) xmy_free(block->buffer);
    if(block->buffer2) xmy_free(block->buffer2);
    if(block->segs) xmy_free(block->segs);
    block = block->sibling;
    xmy_free(block);
  }
}

tp_block *tp_enter_loop(char *txt) {
  int i, num_tokens, c_token;
  char tbuf[240];
  tp_block *a = tp_new_block();
  a->type = TPB_LOOP;
  strcpy( a->args, txt);
  strcpy(input_line, txt);
  num_tokens = scanner(input_line);
  c_token = 0;
  if(isletter(c_token) && equals(c_token+1, "=") ) {
    copy_str(a->var ,c_token); //var
  } else {
    int_error("Bad expression after #loop, should be like #loop i=1,10", c_token);
  }
  c_token+=2;
  i = c_token;
  while(!equals(i, ",") && i <= num_tokens) i++;
  if(i <= num_tokens) {
    capture(tbuf, c_token, i-1); //start
    sprintf(a->start, "tp_start%d=%s", tp_count, tbuf);
    c_token = i+1;
    i= c_token;
    while(!equals(i, ",") && i <= num_tokens) i++;
    capture(tbuf, c_token, i-1); // end
    sprintf(a->end, "tp_end%d=%s", tp_count, tbuf);	    
    if(equals(i, ",")) { // incr
      c_token = i+1;
      capture(tbuf, c_token, num_tokens-1);
      sprintf(a->incr, "tp_incr%d=%s", tp_count, tbuf);	          
    } else {
      sprintf(a->incr, "tp_incr%d=1", tp_count);	          
    }
  }
  return a;
}

tp_block *tp_enter_if(char *txt, int type) {
  char tbuf[240];
  int num_tokens;
  tp_block *a = tp_new_block();
  a->type = type;
  if(type == TPB_ELSE) strcpy(a->args, "1");
  else strcpy(a->args, txt);
  strcpy(input_line, a->args);
  num_tokens = scanner(input_line);
  capture(tbuf, 0, num_tokens);
  sprintf(a->cond, "tp_cond%d=(%s)", tp_count, tbuf);
  return a;
}

void tp_append_to_buf(tp_buf *buf, char *p) {

  if(p != NULL) {
    int len = strlen(p);
    if(buf->len +len+1 >= buf->slen) {
      int tmp =   (buf->slen + len) * 2;
      if(! buf->buffer) {
	buf->buffer = (char *)xmy_malloc( (1+tmp) * sizeof(char) );
      } else {
	buf->buffer = (char *)xmy_realloc(buf->buffer,(1+tmp) * sizeof(char));      
      }
      buf->slen = tmp;
    }
    buf->len =  buf->len + len;  
    strcat(buf->buffer, p);
  } else {
    fprintf(stderr,"+++ Tryint to append NULL to txt buf\n");
  }
}

void tp_append_to_block(tp_block *current, char *p) {
  tp_append_to_buf( &(current->original), p);
}

void tp_rewind_text_buf(tp_buf *txt_buf) {
  txt_buf->p = txt_buf->buffer;
}

int tp_get_line(char *buf, int len, tp_buf *txt_buf) {
  int i = 0;
  char *p,*q,*t,*s;
  s = p = txt_buf->p;
  q = strchr(p, '\n');
  if(q) {
    t = p;
    while(t != q && i < len-1) {
      buf[i++] = *t++;
    }
    buf[i++] = *t++;    
    txt_buf->p = q+1;    
  } else {
    t = p;
    while(*t && i < len-1) {
      buf[i++] = *t++;
    }
    txt_buf->p = t;    
  }
  buf[i]=0;
  if( i >= len) {
    fprintf(stderr, "Buffer overflow in tp_getline %s\n", s);
    exit(4);
  }
  return(i);
}

char *strip_spaces(char *p) {
  char *r = p;
  while( *r && (*r == ' ' || *r == '\t')) r++;
  return r;
}

tp_block *tp_parse_blocks(tp_buf *txt_buf) {
  char buff[8192];
  char *p, *q, *r;

  int loop_counter = 0;
  int if_counter = 0; 
  int line_no=0;
  
  tp_block *current_block, *block, *head_block, *tmp;  

  current_block = tp_new_block();
  head_block = current_block;
 
  tp_rewind_text_buf(txt_buf);
  while(tp_get_line(buff, 8192, txt_buf)) {
    line_no++;
    q = p = buff;
    r = strip_spaces(q);
    if(!strncasecmp(r, "#loop", 5)) {
      if(loop_counter ==0 && if_counter == 0) {
	p = r+5;
	block= tp_enter_loop(p);
	current_block->next = block;
	block->prev = current_block;
	current_block = block;
      } else {
	tp_append_to_block(current_block,q);
      }
      loop_counter++;	
    } else if(!strncasecmp(r, "#endloop", 8)) {
      loop_counter--;
      if(loop_counter < 0) {
	fprintf(stderr, "misplaced #endloop on line %d. %s\n",line_no, q);
	fprintf(stderr, "%s\n", txt_buf->buffer);
	exit(2);
      }
      if(loop_counter > 0 || if_counter >0) { // inside a micro block
	tp_append_to_block(current_block,p);
      } else { // end current loop
	block = tp_new_block();
	block->prev = current_block;
	current_block->next = block;
	current_block=block;
      }
    } else if(!strncasecmp(r, "#if", 3)) {
      if(loop_counter ==0 && if_counter == 0) {
	p = r+3;
	block= tp_enter_if(p, TPB_IF);
	current_block->next = block;
	block->prev = current_block;
	current_block = block;
      } else {
	tp_append_to_block(current_block,p);      
      }
      if_counter++;

    } else if(!strncasecmp(r, "#elif", 5)) {
      if(loop_counter ==0 && if_counter == 1) {
	p = r+5;
	block= tp_enter_if(p, TPB_ELIF);
	current_block->sibling = block;
	if(current_block->head)
	  block->head = current_block->head;
	else
	  block->head = current_block;	  
	current_block = block;
      } else {
	tp_append_to_block(current_block,p);      
      }      
    } else if(!strncasecmp(r, "#else", 4)) {
      if(loop_counter ==0 && if_counter == 1) {
	p = r+3;
	block= tp_enter_if(p, TPB_ELSE);
	current_block->sibling = block;
	if(current_block->head)
	  block->head = current_block->head;
	else
	  block->head = current_block;	  
	current_block = block;
      } else {
	tp_append_to_block(current_block,p);      
      }
    } else if(!strncasecmp(r, "#endif", 6)) {
      if_counter--;
      if(if_counter < 0) {
	fprintf(stderr, "misplaced #endif on line %d: %s\n", line_no, q);
	fprintf(stderr, "%s\n", txt_buf->buffer);	
	exit(3);
      }
      if(loop_counter > 0 || if_counter >0) { // inside a micro block
	tp_append_to_block(current_block,p);
      } else { // end if
	block = tp_new_block();
	if(current_block->head) { //  elif or else, prev/next chain is on the head block only
	  tmp = current_block->head;
	  block->prev = tmp;
	  tmp->next = block;	  
	} else {
	  block->prev = current_block;
	  current_block->next = block;
	}
	current_block=block;
      }
    } else {
      tp_append_to_block(current_block,p);
    }
  }
  return head_block;
}

void tp_show_block_info(tp_block *block) {
  tp_block *tmp;
  while(block) {
    fprintf(stderr,"-------------------------------- %d %d %s\n", block->id, block->type, block->args);
    fprintf(stderr,"%s", block->original.buffer);
    fprintf(stderr,"==-\n");
    tmp = block->sibling;
    while(tmp) {
      fprintf(stderr,"+++++++++ sibling %d %d %s\n",   tmp->id, tmp->type, tmp->args);
      fprintf(stderr,"%s", tmp->original.buffer);
      fprintf(stderr,"+++\n");	
      tmp = tmp->sibling;
    }
    block = block->next;
  }
}

int tp_process_blocks(tp_block *block) {
  tp_block *new_block, *tmp_block;
  char *func;
  int status_return;
 
  tmp_block = block;
  if(tp_debug) {
    tp_show_block_info(block);
  }
  block = tmp_block;
    
  while(block) {
    int type = block->type;
    if(type == TPB_LOOP) {
      char pattern[264], pattern2[264];
      int plen, plen2;
      int i, k, n = 0,m=-1;
      int start, end, incr;
      char *p = block->original.buffer;
      CALC_DefineFunctionOrVariable(block->start, &func, &status_return);
      if(status_return == 0)  tp_error("Error, unable to parse '%s'", block->start);
      start =get_var_int_value(func);
      CALC_DefineFunctionOrVariable(block->end, &func, &status_return);
      if(status_return == 0)  tp_error("Error, unable to parse '%s'", block->end);      
      end =get_var_int_value(func);
      CALC_DefineFunctionOrVariable(block->incr, &func, &status_return);
      if(status_return == 0)  tp_error("Error, unable to parse '%s'", block->incr);      
      incr =get_var_int_value(func);

      sprintf(pattern, "$%s", block->var);
      sprintf(pattern2, "${%s}", block->var);
      plen = strlen(pattern);
      plen2= plen+2;      
      while(*p && (i = match2Words(p, pattern,pattern2,strlen(p), &m))>0) {
	n++;
	if(i == 21 || i == 11) {	
	  p = p+m+plen;
	} else if(i == 22 || i == 12) {
	  p = p+m+plen2;
	} else {
	  tp_error("Bad return value in match: %s!!\n", pattern);	  
	}
      }

      block->nsegs = n+n+1;
      block->segs = (char **)xmy_malloc( (n+n+2) * sizeof(char *));
      
      block->buffer = (char *)xmy_malloc( (block->original.slen) * sizeof(char));
      block->buffer2 = (char *)xmy_malloc( 2*(block->original.slen) * sizeof(char));

      strcpy(block->buffer, block->original.buffer);
      n = 0;
      p = block->buffer;
      block->segs[0] = p;

      while((i = match2Words(p, pattern, pattern2, strlen(p), &m)) > 0) {
	p[m] = '\0';
	if(i == 21 || i == 11) {
	  p = p+m+plen;
	} else if(i == 22 || i == 12) {
	  p = p+m+plen2;
	} else {
	  tp_error("Bad return value in match: %s!!\n", pattern);
	}
	block->segs[++n] = block->vbuf;	
	block->segs[++n] = p;
      }

      for(k=start; k<=end; k += incr) {
	sprintf(block->vbuf, "%d", k);
	bzero(block->buffer2, 2*(block->original.slen) * sizeof(char));
	for(i=0; i< block->nsegs; i++) {
	  //fprintf(stderr, "%s", block->segs[i]);
	  if(block->segs[i]) strcat(block->buffer2, block->segs[i]);
	}
	//fprintf(stderr, "[%s]\n", block->buffer2);
	tp_append_to_buf(&(block->expanded), block->buffer2);
      }
      
      new_block = tp_parse_blocks( &(block->expanded));
      
      if(tp_debug) {
	fprintf(stderr, "============= parsing \n");
	fprintf(stderr, "%s\n", block->expanded.buffer);
	fprintf(stderr, "=====++++++++=\n");	      
	tp_block *a = new_block;
	tp_show_block_info(a);	    	
      }
      new_block->prev = block->prev;
      if(block->prev) { block->prev->next = new_block;}
      tmp_block = new_block;
      while(tmp_block && tmp_block->next)
	tmp_block = tmp_block->next;
      tmp_block->next = block->next;
      if(block->next) { block->next->prev =tmp_block;}
      tp_destroy_block(block);
      return(1);
    } else if(type == TPB_IF) {
      int cond, found=0;
      tp_block *tblock = block, *head = block;
      do {
	CALC_DefineFunctionOrVariable(tblock->cond, &func, &status_return);
	if(status_return == 0) tp_error("Error, unable to parse '%s'", tblock->cond);
	cond = get_var_int_value(func);
      
	if(cond) {
	  new_block = tp_parse_blocks( &(tblock->original));
	
	  if(tp_debug) {      
	    fprintf(stderr, "============= parsing \n");
	    fprintf(stderr, "%s\n", tblock->original.buffer);
	    fprintf(stderr, "=====++++++++=\n");	      
	    tp_block *a = new_block;
	    tp_show_block_info(a);	    
	  }	
	  new_block->prev = head->prev;
	  if(head->prev) { head->prev->next = new_block;}
	  tmp_block = new_block;
	  while(tmp_block && tmp_block->next)
	    tmp_block = tmp_block->next;
	  tmp_block->next = head->next;
	  if(head->next) { head->next->prev =tmp_block;}
	  found = 1;
	}
      } while(found == 0 && (tblock = tblock->sibling) != NULL);
      
      if(found == 0) {
	head->type = TPB_UNKNOWN;
	tp_block *prev = head->prev;
	tp_block *next = head->next;
	if(prev) prev->next = next;
	if(next) next->prev = prev;
      }
      tp_destroy_block(head);	      
      return(1);
    }
    block = block->next;
  }
  return 0;
}

char *xmy_malloc(int s)
{
  char *tmp = malloc(s * sizeof(char));
  if(tmp == NULL) {fprintf(stderr, "Out of memory\n"); exit(1);}
  bzero(tmp, s*sizeof(char));
  return(tmp);
}

char *xmy_realloc(void *p, int s)
{
  char *tmp;
  if(p) tmp = realloc(p, s* sizeof(char));
  else  tmp = malloc(s * sizeof(char));
  if(tmp == NULL) {fprintf(stderr, "Out of memory\n"); exit(1);}
  return(tmp);
}

void xmy_free(void *ptr)
{
  if(ptr) free(ptr);
}


int match1Word(const char *buf, char *word, int buflen, int *pos_ret)
//     const char *buf; char *word; int *pos_ret, buflen;
{
       int blen, wlen;
       int pfxS[256], *pfx = pfxS;

       blen = buflen;
       wlen = strlen(word);
       if(blen < wlen) return(0);
       if(wlen > 250)  pfx = (int *) xmy_malloc( (wlen + 4) * sizeof(int));

       /* compute the prefix function */
       {
	 int m = wlen;
	 int i,j;
	 pfx[0] = -2;
	 pfx[1] = -1;
	 for(i = 2; i < m; i++)
	   {
	     j = pfx[i-1] + 1;
	     while(word[i-1] != word[j] && j >= 0) j = pfx[j]+1;
	     pfx[i] = j;
	   }
       }

       /* match */
       {
	 int n = blen;
	 int m = wlen;
	 int i=0, j=0;

	 while(i < n)
	   {
	     if(word[j] == buf[i]) {j++; i++;}
	     else
	       {
		 j = pfx[j] + 1;
		 if(j < 0) {j = 0; i++;}
	       }
	     if(j == m)
	       {
		 if(pos_ret) *pos_ret = i-m;
		 if(pfx != pfxS) xmy_free(pfx);
		 return(1);
	       }
	   }
       }
       if(pfx != pfxS) xmy_free(pfx);
       return(0);
}



int match2Words(const char *buf, char *word, char *word1, int buflen, int *pos_ret) {
  int p1, p2, m1, m2;
  m1 = match1Word(buf, word, buflen, &p1);
  m2 = match1Word(buf, word1, buflen, &p2);
  if(m1) {
    if(m2) {
      if(p1 < p2) {
	*pos_ret = p1;
	return 21;
      } else {
	*pos_ret = p2;
	return 22;
      }
    } else {
      *pos_ret = p1;
      return 11;
    }
  } else if(m2) {
    *pos_ret = p2;
    return 12;
  }
  return 0;
}

void tp_error(char *message, char *near) {
  fprintf(stderr, message, near);
  exit(1);
}


/****************************************************************************/
//
//  pre process content for taylor.
//  input:  filename or in
//   if filename is not NULL, read input from filename,
//   otherwise, take content from in.
//  output: processed text.
//
//
char *taylor_preprocess(char *filename, char *in) {
  char buff[1024];
  int processed;
  FILE *fp;
  tp_buf txt_buffer, *txt_buf = &txt_buffer;
  tp_block *head_block, *block;  
  
  txt_buf->slen = 0;
  txt_buf->len  = 0;
  txt_buf->buffer  = NULL;  
  tp_append_to_buf(txt_buf, " ");
  
  if(filename) {
    if((fp=fopen(filename, "r"))) {
      while(fgets(buff, 1024, fp)) {
	tp_append_to_buf(txt_buf, buff);    
      }
      fclose(fp);
    } else {
      tp_error("Unable to open '%s'\n",filename);
    }
  } else if(in) {
    tp_append_to_buf(txt_buf, in);
  } 
  
  head_block = tp_parse_blocks(txt_buf);

  processed = 0;
  do {
    block = head_block;
    processed = tp_process_blocks(block);
  } while(processed && 1);

  block = head_block;
  
  txt_buf->len  = 0;
  txt_buf->buffer[0] = '\0';
  
  while(block) {
    if(tp_debug) {
      sprintf(buff, "\n#========= %d %d\n", block->id, block->type);
      tp_append_to_buf(txt_buf, buff);
    }
    tp_append_to_buf(txt_buf, block->original.buffer);          
    block=block->next;
  }
  return txt_buf->buffer;
}
void remove_temp_input() {
  extern char *saved_input_file;
  unlink(saved_input_file);
}

FILE *preprocessed_file(FILE *fp_in, char *filename) {
  char *fname, *p;
  char tempfile[128], tempinput[128], buf[256];
  int filedes = -1;
  FILE *fp, *in;
  extern char *saved_input_file;

  // handle the case when there is no input file
  if(fp_in == stdin && filename == NULL) {
    sprintf(tempinput, "taylor-temp-%d", (int)getpid());
    if( (in = fopen(tempinput, "w")) == NULL) {
      fprintf(stderr, "Unable to open temp file %s\n", tempinput);
      exit(1);
    }
    while(fgets(buf, 256, stdin)) {
      fputs(buf, in);
    }
    fclose(in);
    saved_input_file=(char *) malloc((1+strlen(tempinput)) * sizeof(char));
    strcpy(saved_input_file, tempinput);
    filename = saved_input_file;
    fp_in = NULL;
    atexit(remove_temp_input);
  }
  if( !filename) {
    fprintf(stderr, "An input file is required, please run taylor with the -i option.\n");
    exit(12);
  }
  if(fp_in) fclose(fp_in);  
  sprintf(tempfile, "%s-XXXXXX", filename);
  fname = filename;
  
  p = taylor_preprocess(fname, NULL);
  filedes = mkstemp(tempfile);
  if(filedes == -1) {
    fprintf(stderr,"Unable to open tmp file\n");
    exit(2);
  }
  unlink(tempfile);
  if( (write(filedes,p, strlen(p) * sizeof(char)) == -1) )  {
    fprintf(stderr, "Unable to write to tmpfile\n");
    exit(3);
  }
  lseek(filedes, 0, SEEK_SET);
  fp = fdopen(filedes,"r");

  return fp;
}
