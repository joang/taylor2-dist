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

#define SUM_C
#include "Header.h"
/********************************************************/
int insum = 0; 
/********************************************************/

Node newIdx(char c)     /* return a special variable */
{
  Node         nid;
  char         tvarname[20];
  static int idxCounter = 0;

  if(c == 'v')  sprintf(tvarname, "_i_%02d", idxCounter++);
  else if(c == 'c')  sprintf(tvarname, "_c_%02d", idxCounter++);
  else   sprintf(tvarname, "_j_%02d", idxCounter++);
  nid = install_id(tvarname);
  SET_ID_IS_VAR(nid);
  SET_ID_IS_CST(nid);
  SET_ID_IS_INT(nid);
  SET_NODE_IS_INDEX(nid);
  return(nid);
}
/********************************************************/
void replaceA_by_B(Node *expr, Node a, Node b)
{
  if(expr && *expr)
    {
      int code = NODE_CODE(*expr);

      switch(code)
	{
	case INT_CST:
	case FLOAT_CST:
	  break;
	case ID_NODE:
	  if(*expr == a)  *expr = b;
	  else if(IS_INTERNAL_SUM(*expr))
	    {
	      Node *def = &(variables[ID_V_INDEX(*expr)].def);
	      replaceA_by_B(def, a,b);
	    }
	  break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case DIV_EXPR:
	case EXP_EXPR:
	case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
        case AND_EXPR:
        case OR_EXPR:
	  replaceA_by_B( &(NODE_LEFT(*expr)),  a, b);
	  replaceA_by_B( &(NODE_RIGHT(*expr)), a, b);
	  break;
	case  NEGATE_EXPR:
	  replaceA_by_B( &(NODE_LEFT(*expr)),  a, b);
	  break;
	case CALL_EXPR:
	  replaceA_by_B( &(NODE_RIGHT(*expr)), a, b);
	  break;
	case ARRAY_REF:
	  /* left:   array name
	   * right:  indices of type ARRAY_IDX, chained through -right- 
	   *         with -left- being the idx.
	   */
	  replaceA_by_B( &(NODE_RIGHT(*expr)), a, b);
	  break;
	case ARRAY_IDX:
	  /* left:   idx
	   */
	  replaceA_by_B( &(NODE_LEFT(*expr)), a, b);
	  replaceA_by_B( &(NODE_RIGHT(*expr)), a, b);
	  break;
        case IF_ELSE_EXPR:
	  replaceA_by_B( &(IF_ELSE_COND(*expr)), a, b);
	  replaceA_by_B( &(NODE_LEFT(*expr)), a, b);
	  replaceA_by_B( &(NODE_RIGHT(*expr)), a, b);
	  break;
	case SUM_EXPR:
	  {
	    SumP sum = NODE_SUM_BLOCK(*expr);
	    replaceA_by_B( &(SUMB_EXPR(sum)), a, b);	    
	  }
	break;
	default:
	  fprintf(stderr,"Error: Unknown NODE in build_sum.\n");
	  break;
	}
    }
}
/********************************************************/

Node build_sum(Node expr, Node var, Node ifrm, Node ito)
{
  int bad = 0, from, to;

  from = intNodeValue(ifrm, &bad);
  if(!bad) to = intNodeValue(ito, &bad);
  if(bad)  
    {
      fprintf(stderr, "Syntax Error: sum(expr, id=INT_CST, INT_CST)\n");
      exit(5);
    }
  if(from > to) 
    {
      fprintf(stderr, "Error in Sum: from_index > to_index.\n");
      exit(5);
    }

  /* for small sum, just expand the whole thing  */
  

  if(expandsum && expandsum > to-from)
    return(expand_sum(expr, var, ifrm, ito));

  if(insum > 1)
    {
      fprintf(stderr, "Netsted SUM is not supported, sorry! (nested level=%d)\n", insum);
      exit(5);
    }

  if(1)
    {
      Node  nexpr = make_node(SUM_EXPR);
      Node  nvar = getTmpVar();
      Node  idx = newIdx('v');
      SumP  sum = (SumP) my_malloc(sizeof(struct sum_));

      memset((char *)sum, 0, sizeof(struct sum_));
      havesum++;
      NODE_SUM_BLOCK(nexpr) = sum;
      replaceA_by_B(&expr, var, idx);
      SET_IS_INTERNAL_SUM(nvar);
      SUMB_EXPR(sum) = expr;
      SUMB_IDX(sum) =  idx;
      SUMB_FROM(sum) = from;
      SUMB_TO(sum) = to;
      insert_one_variable(nvar, nexpr);
      canonicalTree(idx);

      return(nvar);
    }
  return(NULL);
}

/*********************************************************
 *
 * Duplicate expr and replace all occurence of a by b.
 */
Node copyTreeReplaceAB(Node expr, Node a, Node b)
{
  if(expr)
    {
      int code = NODE_CODE(expr);
      Node tnode = NULL, left = NODE_LEFT(expr), right = NODE_RIGHT(expr);
      switch(code)
	{
	case INT_CST:
	case FLOAT_CST:
	  return(expr);
	  break;
	case ID_NODE:
	  if(expr == a) return(b);
	  else return(expr);
	  break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case DIV_EXPR:
	case EXP_EXPR:
	  left  = copyTreeReplaceAB(left, a,  b);
	  right = copyTreeReplaceAB(right, a,  b);
	  tnode = build_op(code, left, right);
	  NODE_FLAGS(tnode) = NODE_FLAGS(expr);
	  return(tnode);
          break;
        case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
        case AND_EXPR:
        case OR_EXPR:
	  left  = copyTreeReplaceAB(left, a,  b);
	  right = copyTreeReplaceAB(right, a,  b);
	  tnode = build_bop(code, left, right);
	  NODE_FLAGS(tnode) = NODE_FLAGS(expr);
	  return(tnode);
	  break;
	case  NEGATE_EXPR:
	  left  = copyTreeReplaceAB(left, a,  b);	  
	  tnode = build_op(code, left, NULL);
	  NODE_FLAGS(tnode) = NODE_FLAGS(expr);
	  return(tnode);
	  break;
	case CALL_EXPR:
	  right = copyTreeReplaceAB(right, a,  b);
	  tnode = build_op(code, left, right);
	  NODE_FLAGS(tnode) = NODE_FLAGS(expr);
	  return(tnode);
	  break;
	case ARRAY_REF:
	  left  = copyTreeReplaceAB(left, a,  b);  /* nothing */
	  right  = copyTreeReplaceAB(right, a,  b);  /* nothing */
	  tnode = build_op(code, left, right);
	  NODE_FLAGS(tnode) = NODE_FLAGS(expr);
	  return(tnode);	  
	  break;
	case ARRAY_IDX:
	  left  = copyTreeReplaceAB(left, a,  b);  
	  right  = copyTreeReplaceAB(right, a,  b); 
	  tnode = build_op(code, left, right);
	  NODE_FLAGS(tnode) = NODE_FLAGS(expr);
	  return(tnode);	  
	  break;	  
        case IF_ELSE_EXPR:
          {
            Node cond = IF_ELSE_COND(expr);
            Node ncond = copyTreeReplaceAB(cond, a,  b);
            left  = copyTreeReplaceAB(left, a,  b);  /* nothing */
            right = copyTreeReplaceAB(right, a,  b);  
            tnode = build_if_else(ncond, left, right);
	    NODE_FLAGS(tnode) = NODE_FLAGS(expr);
            return(tnode);
          }
	default:
	  fprintf(stderr,"Error: Unknown NODE in build_sum.\n");
	  break;
	}
    }
  return(NULL);
}

Node expand_sum(Node expr, Node var, Node ifrm, Node ito)
{
  int i, bad = 0, from, to;
  char tstr[32];
  Node tmpNode, tsum, sum=NULL;

  from = intNodeValue(ifrm, &bad);
  if(!bad) to = intNodeValue(ito, &bad);
  if(!bad)
    {
      if(from > to)
        {
          fprintf(stderr, "Error in Sum: from_index > to_index.\n");
          exit(5);
        }
      for(i = from; i <= to; i++)
	{
	  sprintf(tstr, "%d", i);
	  tmpNode =  build_int(tstr);
	  tsum = copyTreeReplaceAB(expr, var, tmpNode);
	  if(sum == NULL) sum = tsum;
	  else
	    {
	      Node nsum = build_op(PLUS_EXPR,sum,tsum);
	      sum = nsum;
	    } 
	}
      return(sum);
    }
  else
    {
      fprintf(stderr, "Syntax Error: Sum(expr, id=INT_CST, INT_CST)\n");
      exit(5);
    }
  return(NULL);
}

/********************************************************/
int mark_local(SumP sum, Node expr, Node idx)
{
  if(expr)
    {
      int a, b, c, code = NODE_CODE(expr);
      Node left = NODE_LEFT(expr), right = NODE_RIGHT(expr);
      switch(code)
	{
	case INT_CST:
	case FLOAT_CST:
	  break;
	case ID_NODE:
	  if(expr == idx)
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  else
	    {
	      Node def = variables[ID_V_INDEX(expr)].def;
	      if(mark_local(sum, def, idx))
		{
		  SET_NODE_IS_LOCAL(expr);
		  NODE_SUM_BLOCK(expr) = sum;
		  return(1);
		}
	    }
	  break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case DIV_EXPR:
	case EXP_EXPR:
	  a = mark_local(sum, left, idx);
	  b = mark_local(sum, right, idx);
	  if(a|b) 
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  break;
	case NEGATE_EXPR:
	  if(mark_local(sum, left, idx))
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  break;
	case CALL_EXPR:
	  if(mark_local(sum, right, idx))
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  break;
        case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
        case AND_EXPR:
        case OR_EXPR:
	  a = mark_local(sum, left, idx);
	  b = mark_local(sum, right, idx);
	  if(a|b) 
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  break;
        case IF_ELSE_EXPR:
	  a = mark_local(sum, left, idx);
	  b = mark_local(sum, right, idx);
	  c = mark_local(sum, IF_ELSE_COND(expr), idx);
	  if(a|b|c) 
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  break;
	case SUM_EXPR:
	  fprintf(stderr, "Nested sum detected, should never happen!\n");
	  exit(19);
	  {
	    SumP tsum = NODE_SUM_BLOCK(expr);
	    Node sexpr = SUMB_EXPR(tsum);
	    if(mark_local(tsum,sexpr,idx))
	      {
		NODE_SUM_BLOCK(expr) = sum;
		SET_NODE_IS_LOCAL(expr);
		return(1);
	      }
	  }
	break;
	case ARRAY_IDX:
	  a = mark_local(sum, left, idx);
	  b = mark_local(sum, right, idx);
	  if(a|b)
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }
	  break;
	case ARRAY_REF:
	  if(mark_local(sum, right, idx))
	    {
	      SET_NODE_IS_LOCAL(expr);
	      NODE_SUM_BLOCK(expr) = sum;
	      return(1);
	    }	  
	  break;
	default:
	  break;
	}	  
    }
  return(0);
}
/********************************************************/

void doCountSumVariables(SumP sum, Node def, int flag)
{
  if(def == NULL) return;
  if(flag == 0)
    {
      if(VAR_IS_MARKED1(def)) return;
      SET_VAR_IS_MARKED1(def);
    }
  else
    {
      if(VAR_IS_MARKED2(def)) return;
      SET_VAR_IS_MARKED2(def);      
    }

  if(1)
    {
      Node depa = NODE_DEPEND_A(def);
      Node depb = NODE_DEPEND_B(def);
      Node depc = NODE_DEPEND_C(def);
      Node cond = IF_ELSE_COND(def);
      Node budy = NODE_BUDY(def);
      doCountSumVariables(sum, depa, flag);
      doCountSumVariables(sum, depb, flag);
      doCountSumVariables(sum, depc, flag);
      doCountSumVariables(sum, cond, flag);

      if(NODE_IS_LOCAL(def) && NODE_CODE(def) == ID_NODE)
	{
          if( ID_IS_COUNT_LOCAL(def) == 0 || (flag && ID_COUNTED(def) == 0))
            {
              if(ID_IS_INT(def)) 
                {
		  if(NODE_IS_INDEX(def) == 0)  /* don't count running idx */
		    {
		      ID_IOFFSET(def) = SUMB_LINT(sum);
		      if(flag) { SUMB_LINT_LIST(sum)[SUMB_LINT(sum)] = def;}
		      SUMB_LINT(sum) += 1;
		    }
		  else
		    {
		      SET_ID_IS_COUNT_LOCAL(def); 
		      SET_ID_COUNTED(def);
		    }
		  SET_VAR_LOCAL_IVAR(def);
                }
              else if(ID_IS_CST(def)) 
                {
                  ID_COFFSET(def) = SUMB_LCST(sum);
                  if(flag) { SUMB_LCST_LIST(sum)[SUMB_LCST(sum)] = def;}
                  SUMB_LCST(sum) += 1;
                  SET_VAR_LOCAL_CVAR(def);
                }
              else
                {
                  ID_VOFFSET(def) = SUMB_LVAR(sum); 
                  if(flag) { SUMB_LVAR_LIST(sum)[SUMB_LVAR(sum)] = def;}                  
                  SUMB_LVAR(sum) += 1;
                  SET_VAR_LOCAL_VAR(def);
                }
              { SET_ID_IS_COUNT_LOCAL(def);}
              if(flag && NODE_IS_INDEX(def) == 0)  /* don't count running idx */
                {
                  int k = SUMB_LINT(sum) + SUMB_LCST(sum) + SUMB_LVAR(sum);
                  SUMB_ALL_VARS(sum)[k-1] = def;
                  SET_ID_COUNTED(def);
                }
	      doCountSumVariables(sum, budy, flag);
	    }
	}
    }
}
/********************************************************/
void countSumVariables(Node def) /* def is a SUM_EXPR */
{
  if(def)
    {
      SumP sum = NODE_SUM_BLOCK(def);
      Node expr;
      int ss;

      if(SUMB_LVAR(sum) || SUMB_LCST(sum) || SUMB_LINT(sum)) return;
      expr = SUMB_EXPR(sum);
      doCountSumVariables(sum, expr, 0);
      ss = SUMB_LVAR(sum) + SUMB_LCST(sum) + SUMB_LINT(sum) + 3;
      SUMB_ALL_VARS(sum) = (Node *)my_malloc( (ss+1) * sizeof(Node));
      SUMB_LVAR_LIST(sum) = (Node *)my_malloc( (ss+1) * sizeof(Node));
      SUMB_LCST_LIST(sum) =  SUMB_LVAR_LIST(sum) + SUMB_LVAR(sum) + 1;
      SUMB_LINT_LIST(sum) =  SUMB_LCST_LIST(sum) + SUMB_LCST(sum) + 1;
      /* count again */
      SUMB_LVAR(sum) = 0;  SUMB_LCST(sum) = 0;  SUMB_LINT(sum) = 0;
      doCountSumVariables(sum, expr, 1);
    }
}
void printSumBlockVariables(FILE *fp, SumP sum)
{
  int i, idx, total = SUMB_LINT(sum) + SUMB_LCST(sum) + SUMB_LVAR(sum);
  Node name=NULL, def=NULL, *vars;
  vars = SUMB_ALL_VARS(sum);
  fprintf(fp, "\t\t  (%s is the running index)\n",  NODE_NAME(SUMB_IDX(sum)));	 
  for(i = 0; i < total; i++)
    {
      if(VAR_IS_LOCAL_IVAR(vars[i]))
	{
	  if(NODE_IS_INDEX(vars[i]))
	    fprintf(fp, "\t  %s (running index)\n",  NODE_INFO(vars[i]));	 
	  else
	    {
	      idx = NODE_V_INDEX(vars[i]);
	      name = variables[idx].name;
	      def =  variables[idx].def;
	      fprintf(fp, "\t\t  %s=%s   (%d+%d*%s, %d)\n", NODE_INFO(name), NODE_INFO(def),
		      ID_IOFFSET(name),SUMB_LINT(sum), NODE_INFO(SUMB_IDX(sum)),SUMB_IOFFSET(sum));
	    }
	}
      else if(VAR_IS_LOCAL_CVAR(vars[i]))
	{
	  idx = NODE_V_INDEX(vars[i]);
	  name = variables[idx].name;
	  def =  variables[idx].def;
	  fprintf(fp, "\t\t  %s=%s   (%d+%d*%s, %d)\n", NODE_INFO(name), NODE_INFO(def),
		  ID_COFFSET(name),SUMB_LCST(sum), NODE_INFO(SUMB_IDX(sum)),SUMB_COFFSET(sum));	      
	}
      else if(VAR_IS_LOCAL_VAR(vars[i]))
	{
	  idx = NODE_V_INDEX(vars[i]);
	  name = variables[idx].name;
	  def =  variables[idx].def;
	  fprintf(fp, "\t\t  %s=%s   (%d+%d*%s, %d)\n", NODE_INFO(name), NODE_INFO(def),
		  ID_VOFFSET(name),SUMB_LVAR(sum), NODE_INFO(SUMB_IDX(sum)),SUMB_VOFFSET(sum));	      	      
	}
      else
	{
	  idx = NODE_V_INDEX(vars[i]);
	  name = variables[idx].name;
	  def =  variables[idx].def;
	  fprintf(fp, "Internal error: Var '%s=%s' is in a local varlist but is not marked as such.\n",
		  NODE_INFO(name), NODE_INFO(def));
	}
    }
}
/********************************************************/
void doCountSumGVariables(Node def)
{
  if(def)
    {
      Node depa = NODE_DEPEND_A(def);
      Node depb = NODE_DEPEND_B(def);
      Node budy = NODE_BUDY(def);
      doCountSumGVariables(depa);
      doCountSumGVariables(depb);
      
      if( NODE_CODE(def) == ID_NODE) 
	{
	  if( ID_COUNTED(def) == 0  && ID_IS_INT(def) == 0 &&  ID_IS_CST(def) == 0)
	    {
	      varlistFinal[varcountFinal] = def;
	      ID_VINDEX(def) = varcountFinal;
	      ID_VOFFSET(def) = goffset;
	      varcountFinal++;	      
	      SET_ID_COUNTED(def);
	      SET_VAR_GLOBAL_VAR(def);
	      doCountSumGVariables(budy);
	    }
	}
    }
}      
void outputGVarsInSum(SumP sum)
{
  if(sum)
    {
      Node expr = SUMB_EXPR(sum);
      doCountSumGVariables(expr);
    }
}
/********************************************************/
void doCountSumCVariables(Node def)
{
  if(def)
    {
      Node depa = NODE_DEPEND_A(def);
      Node depb = NODE_DEPEND_B(def);
      Node cond = IF_ELSE_COND(def);
      Node budy = NODE_BUDY(def);
      doCountSumCVariables(depa);
      doCountSumCVariables(depb);
      doCountSumCVariables(cond);
      
      if( NODE_CODE(def) == ID_NODE) 
	{
	  if( ID_COUNTED(def) == 0  && ID_IS_INT(def) == 0 &&  ID_IS_CST(def) != 0)
	    {
	      constlistFinal[constcountFinal] = def;
	      ID_CINDEX(def) = constcountFinal;
	      ID_COFFSET(def) = cgoffset;
	      constcountFinal++;	      
	      SET_ID_COUNTED(def);
	      SET_VAR_GLOBAL_CVAR(def);
	      doCountSumCVariables(budy);
	    }
	}
    }
}      
void outputCVarsInSum(SumP sum)
{
  if(sum)
    {
      Node expr = SUMB_EXPR(sum);
      doCountSumCVariables(expr);
    }
}
/********************************************************/
void doCountSumIVariables(Node def)
{
  if(def)
    {
      Node depa = NODE_DEPEND_A(def);
      Node depb = NODE_DEPEND_B(def);
      Node depc = NODE_DEPEND_C(def);
      Node cond = IF_ELSE_COND(def);
      Node budy = NODE_BUDY(def);

      doCountSumIVariables(depa);
      doCountSumIVariables(depb);
      doCountSumIVariables(depc);
      doCountSumIVariables(cond);
      
      if( NODE_CODE(def) == ID_NODE) 
	{
	  if( ID_COUNTED(def) == 0  && ID_IS_INT(def) != 0 )
	    {
	      ivarFinal[ivarcountFinal] = def;
	      ID_IINDEX(def) = ivarcountFinal;
	      ID_IOFFSET(def) = igoffset;
	      ivarcountFinal++;	      
	      SET_ID_COUNTED(def);
	      SET_VAR_GLOBAL_IVAR(def);
	      doCountSumCVariables(budy);
	    }
	}
    }
}      
void outputIVarsInSum(SumP sum)
{
  if(sum)
    {
      Node expr = SUMB_EXPR(sum);
      doCountSumIVariables(expr);
    }
}
/********************************************************/
