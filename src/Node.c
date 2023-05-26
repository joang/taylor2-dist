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

/******************************************************************
 *
 *     functions manipulating nodes
 *
 *****************************************************************/
#include <stdio.h>
#define NODE_C
#include "Header.h"

/****************************************************************/
Node error_node     = NULL;
Node int_zero_node  = NULL;
Node int_one_node   = NULL;
Node current_id     = NULL;
Node id_list        = NULL;
Node timeVar        = NULL;
char *current_qstring = NULL;
Expression current_expression = NULL;
Expression expression_list    = NULL;

Hash_Entry  node_hash_table[NODE_HASH_TABLE_SIZE];
/****************************************************************/
Expression new_expression()
{
  Expression tmp = (Expression)malloc(sizeof(struct expression_));
  bzero(tmp,sizeof(struct expression_));
  EXPRESSION_LIST(tmp) = (Node *)malloc(10*sizeof(Node));
  bzero(EXPRESSION_LIST(tmp),10*sizeof(Node));  
  EXPRESSION_SPACE(tmp) = 10;

  EXPRESSION_NEXT(tmp) = expression_list;
  expression_list = tmp;  
  return(tmp);
}

void destroy_all_expressions() {
  if(expression_list){
    Expression b,a =expression_list;
    while(a) {
      b = EXPRESSION_NEXT(a);
      my_free(EXPRESSION_LIST(a));     
      my_free(a);
      a=b;
    }
  }
}
/****************************************************************/

void initialize_nodes(void)
{
  error_node    = make_node(ERROR_NODE);
  int_zero_node = build_int("0");
  int_one_node  = build_int("1");
}
/*************************************************************/
void insert_node_to_hash_table(Node node) {
  if(! node) return;
  
  int hash_v = (long) node  % NODE_HASH_TABLE_SIZE;
  Hash_Entry *tmp = (Hash_Entry *)my_malloc(sizeof(Hash_Entry));
  bzero(tmp, sizeof(Hash_Entry));
  tmp->node = node;
  tmp->next = node_hash_table[hash_v].next;
  node_hash_table[hash_v].next = tmp;
}

void delete_node(Node node) {
  if(! node) return;
  
  int hash_v = (long) node  % NODE_HASH_TABLE_SIZE;
  Hash_Entry *p = &node_hash_table[hash_v], *q=p->next;
  while(p && q) {
    if(q->node == node) {
      p->next = q->next;
      my_free(NODE_NAME(node));
      my_free(NODE_GIVEN_NAME(node));      
      my_free(NODE_INFO(node));
      my_free(NODE_JINITV(node));            
      my_free((void *)node);
      my_free(q);
      break;
    }
    p = q;
    q = p->next;
  }
}

void destroy_all_nodes() {
  int i;
  for(i = 0; i < NODE_HASH_TABLE_SIZE; i++) {
    Hash_Entry *p,  *q=node_hash_table[i].next;
    node_hash_table[i].next = NULL;
    while(q) {
      p = q;
      q = p->next;
      my_free(p);
    }    
  }
}

void display_all_nodes() {
  int i;
  if(debug) {
    for(i = 0; i < NODE_HASH_TABLE_SIZE; i++) {
      Hash_Entry *p,  *q=node_hash_table[i].next;
      fprintf(stderr, "In Bucked %d:\n", i);
      while(q) {
	Node node = q->node;
	if(node)
	  fprintf(stderr, " %s %s %s\n", node_names[NODE_CODE(node)], NODE_NAME(node), NODE_GIVEN_NAME(node));
	p = q;
	q = p->next;
      }    
    }
  }
}
/*************************************************************/
Node make_node(enum node_code code)
{
  register Node   n;
  int             length = sizeof(struct node_);

  n = (Node) my_malloc(length);
  memset((char *)n,0,length);
  NODE_CODE(n) = code;
  insert_node_to_hash_table(n);   
  return (n);
}

/**************************************************************/

Node copy_node(Node n)
{
  register int            length;
  register Node           tmp_node;
  
  length = sizeof(struct node_);
  tmp_node = (node)my_malloc(length);
  bcopy(n,tmp_node,length);
  NODE_LEFT(tmp_node) = (Node) 0;
  NODE_RIGHT(tmp_node) = (Node) 0;

  return(tmp_node);
}
/**************************************************************/

Node build_int(char *v)
{
  register Node n, var;

  var = getTmpVar();  
  n = make_node(INT_CST);
  NODE_NAME(n) = AllocCopyString(v);
  SET_NODE_IS_CST(n);
  SET_NODE_IS_CST(var);
  SET_ID_IS_INT(var);
  SET_ID_IS_INT(n);
  SET_NODE_IS_A_NUMBER(n);
  SET_NODE_IS_A_NUMBER(var);
  insert_one_variable(var, n);
  return(var);
}
/*****************************************************************/
Node build_float(char *name)
{
  register Node n, var;

  var = getTmpVar();  
  n = make_node(FLOAT_CST);
  NODE_NAME(n) = AllocCopyString(name);
  SET_NODE_IS_CST(n);
  SET_NODE_IS_CST(var);
  SET_NODE_IS_A_NUMBER(n);
  SET_NODE_IS_A_NUMBER(var);
  insert_one_variable(var, n);
  return(var);
}
/*****************************************************************/
Node install_id(char *lex)
{
  Node id, ilist = id_list;
  while(ilist)
    {
      if(!strcmp(lex, ID_NAME(ilist))) return(ilist);
      ilist = NODE_NEXT(ilist);
    }
  id = make_node(ID_NODE);  
  NODE_NAME(id) = AllocCopyString(lex);
  ID_GIVEN_NAME(id) = AllocCopyString(lex);
  NODE_NEXT(id) = id_list;
  id_list = id;
  return(id);
}
/********************************************************************/
Node build_op(enum node_code code, Node op1, Node op2)
{
  register Node expr = make_node(code);
  NODE_LEFT(expr) = op1;
  NODE_RIGHT(expr) = op2;
  return(expr);
}

Node build_bop(enum node_code code, Node op1, Node op2)
{
  register Node expr = make_node(code);
  NODE_LEFT(expr) = op1;
  NODE_RIGHT(expr) = op2;
  return(expr);
}

/********************************************************************/

char *AllocCopyString(char *str)
{
  char *tmp = NULL;
  if(str)
    {
      tmp = (char *)my_malloc( strlen(str) + 2);
      (void)strcpy(tmp, str);
    }
  return(tmp);
}

char *ReallocCopyString(char *old, char *str)
{
  char *tmp = (char *)my_realloc(old, strlen(str)+2);
  (void)strcpy(tmp, str);
  return(tmp);
}
/****************************************************************/
char *my_malloc(int s)
{
  char *tmp = malloc(s * sizeof(char));
  if(tmp == NULL) {fprintf(stderr, "Out of memory\n"); exit(1);}
  return(tmp);
}

char *my_realloc(void *p, int s)
{
  char *tmp;
  if(p) tmp = realloc(p, s* sizeof(char));
  else  tmp = malloc(s * sizeof(char));
  if(tmp == NULL) {fprintf(stderr, "Out of memory\n"); exit(1);}
  return(tmp);
}

void my_free(void *ptr)
{
  if(ptr) free(ptr);
}
/****************************************************************/
void canonicalTree(Node node)
{
  if(node)
    {
      int   llen, rlen, len, cmp;
      char  *tstr;
      Node  tmpNode;
      enum node_code code = NODE_CODE(node);  
      switch(code)
        {
        case PLUS_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node), len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          cmp = strcmp(NODE_INFO(NODE_LEFT(node)), NODE_INFO(NODE_RIGHT(node)));
          if(cmp > 0)
            {
              tmpNode = NODE_LEFT(node);
              NODE_LEFT(node) = NODE_RIGHT(node);
              NODE_RIGHT(node) = tmpNode;
            }
          sprintf(tstr,"(%s+%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);

	  if((VAR_IS_JET(NODE_LEFT(node))) || (VAR_IS_JET(NODE_RIGHT(node)))) {SET_VAR_IS_JET(node);}
          break;
        case MULT_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node), len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          cmp = strcmp(NODE_INFO(NODE_LEFT(node)), NODE_INFO(NODE_RIGHT(node)));
          if(cmp > 0)
            {
              tmpNode = NODE_LEFT(node);
              NODE_LEFT(node) = NODE_RIGHT(node);
              NODE_RIGHT(node) = tmpNode;
            }
          sprintf(tstr,"(%s*%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
	  if((VAR_IS_JET(NODE_LEFT(node))) || (VAR_IS_JET(NODE_RIGHT(node)))) {SET_VAR_IS_JET(node);}
          break;
          
        case MINUS_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s-%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);

	  if((VAR_IS_JET(NODE_LEFT(node))) || (VAR_IS_JET(NODE_RIGHT(node)))) {SET_VAR_IS_JET(node);}
          break;
        case DIV_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s/%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);

	  if((VAR_IS_JET(NODE_LEFT(node))) || (VAR_IS_JET(NODE_RIGHT(node)))) {SET_VAR_IS_JET(node);}
          break;
        case EXP_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node), len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s^%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);

	  if((VAR_IS_JET(NODE_LEFT(node))) || (VAR_IS_JET(NODE_RIGHT(node)))) {SET_VAR_IS_JET(node);}
          break;
        case NEGATE_EXPR:
          canonicalTree(NODE_LEFT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          len = llen + 3;
          tstr = my_realloc(NODE_INFO(node), len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(-%s)", NODE_INFO(NODE_LEFT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);

	  if((VAR_IS_JET(NODE_LEFT(node))))  {SET_VAR_IS_JET(node);}
          break;
        case ID_NODE:
        case INT_CST:
        case FLOAT_CST:
          len = strlen(NODE_NAME(node));
          tstr = my_realloc(NODE_INFO(node), len+2);          
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          strcpy(tstr, NODE_NAME(node));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
        case CALL_EXPR:
          canonicalTree(NODE_LEFT(node));  /* func name, ID node */
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 2;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"%s(%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
	  if( (VAR_IS_JET(NODE_RIGHT(node)))) {SET_VAR_IS_JET(node);}
          break;
	case ARRAY_REF:
          canonicalTree(NODE_LEFT(node));  /* array name, ID node */
          canonicalTree(NODE_RIGHT(node)); /* indices */	  
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"%s%s", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);	  
	  break;
	case ARRAY_IDX:   /* arrays are external, indices must reduce to INT_CSTs */
	  canonicalTree(NODE_LEFT(node)); 
	  if(NODE_RIGHT(node))
	    {
	      canonicalTree(NODE_RIGHT(node));
	      rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
	    }
	  else rlen = 0;
	  llen = NODE_INFO_LENGTH(NODE_LEFT(node));
	  len = llen + rlen + 2;
	  tstr = my_realloc(NODE_INFO(node),len+2);
	  NODE_INFO(node) = tstr;
	  NODE_INFO_LENGTH(node) = len;
	  if(rlen > 0) sprintf(tstr,"[%s]%s",NODE_INFO(NODE_LEFT(node)) , NODE_INFO(NODE_RIGHT(node)));
	  else sprintf(tstr,"[%s]", NODE_INFO(NODE_LEFT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
	  break;
	case GE_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 4;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s>=%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case GT_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s>%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case LE_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 4;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s<=%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case LT_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 3;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s<%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case EQ_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 4;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s==%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case NEQ_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 4;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s!=%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;

	case AND_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 4;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s&&%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case OR_EXPR:
          canonicalTree(NODE_LEFT(node));
          canonicalTree(NODE_RIGHT(node));
          llen = NODE_INFO_LENGTH(NODE_LEFT(node));
          rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
          len = llen + rlen + 4;
          tstr = my_realloc(NODE_INFO(node),len+2);
          NODE_INFO(node) = tstr;
          NODE_INFO_LENGTH(node) = len;
          sprintf(tstr,"(%s||%s)", NODE_INFO(NODE_LEFT(node)),
                  NODE_INFO(NODE_RIGHT(node)));
	  NODE_INFO_HASH(node) = hash(tstr);
          break;
	case IF_ELSE_EXPR:
	  {
	    int clen = 0;
	    canonicalTree(NODE_LEFT(node));
	    canonicalTree(NODE_RIGHT(node));
	    canonicalTree(IF_ELSE_COND(node));
	    llen = NODE_INFO_LENGTH(NODE_LEFT(node));
	    rlen = NODE_INFO_LENGTH(NODE_RIGHT(node));
	    clen = NODE_INFO_LENGTH(IF_ELSE_COND(node));
	    len = llen + rlen + clen + 14;
	    tstr = my_realloc(NODE_INFO(node),len+2);
	    NODE_INFO(node) = tstr;
	    NODE_INFO_LENGTH(node) = len;
	    sprintf(tstr,"(if(%s){%s}else{%s})", NODE_INFO(IF_ELSE_COND(node)),
		    NODE_INFO(NODE_LEFT(node)), NODE_INFO(NODE_RIGHT(node)));
	    NODE_INFO_HASH(node) = hash(tstr);
	  }
	break;
	case SUM_EXPR:
	  {
            static int sumCount=0;
	    SumP  sum = (SumP) NODE_SUM_BLOCK(node);
	    tstr = my_realloc(NODE_INFO(node),40);
	    sprintf(tstr, "sUm%04dmUs", sumCount++);
	    NODE_INFO(node) = tstr;
	    NODE_INFO_LENGTH(node) = 40; 
	    canonicalTree(SUMB_EXPR(sum)); 
	  }
	break;
        default:
          fprintf(stderr, "Internal error ... Unknown NodeCode in canonicalTree.\n");
          break;
        }
    }
}
/***********************************************************************/
unsigned long hash(char *s)
{
  char *p;
  unsigned long h = 0L, g;
  
  for(p = s; *p ; p++) 
    {
      h = (h << 4) + (*p);
      if( (g = h & 0xf0000000) )
        {
          h = h ^ (g >> 24);
          h = h ^ g;
        }
    }
  return(h);
}
/***********************************************************************/
