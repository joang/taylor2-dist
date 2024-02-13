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

#define CODE_C
#include "Header.h"
#include "VERSION"
/****************************************************************************/
Node  *varlistFinal = NULL;       /* list of global vars */
Node  *constlistFinal = NULL;     /* list of global csts */
Node  *ivarFinal = NULL;          /* list of global int vars */
Node  *allVarsList = NULL;
Node  *jetVarsList = NULL;        /* list of vars in varlistFinal that are series */
Node  *jetParamsList = NULL;      /* list of jet vars that are variational parameters */
int    allVarsCount = 0;
int    ivarcountFinal = 0;        /* var counts ... */
int    varcountFinal = 0;
int    constcountFinal = 0;
int    goffset = 0;
int    igoffset = 0;
int    cgoffset = 0;
char   *jmarks = NULL;
char   *kmarks = NULL;
/****************************************************************************/
extern char *genGlobalVarLvalue(Node var);
extern char *genVarLvalueKth(Node var, char *kth);
/****************************************************************************/

double cstNodeValue(Node node)
{
  if (node)
    {
      int code = NODE_CODE(node);
      Node left = NODE_LEFT(node);
      Node right = NODE_RIGHT(node);
      double l, r, value;
      
      switch(code)
        {
        case INT_CST:
          value = (double) atoi(NODE_NAME(node));
          return(value);
          break;
        case FLOAT_CST:
          value = (double) atof(NODE_NAME(node));
          return(value);
          break;
        case ID_NODE:
          {
            Node def = variables[ID_V_INDEX(node)].def;
            double value = cstNodeValue(def);
            return(value);
          }
        break;
        case NEGATE_EXPR:
          value= cstNodeValue(left);
          return(-value);
          break;
        case PLUS_EXPR:
          l = cstNodeValue(left);
          r = cstNodeValue(right);
          value = l+r;
          NODE_DVALUE(node) = value;
          return(value);
          break;
        case MINUS_EXPR:
          l = cstNodeValue(left);
          r = cstNodeValue(right);
          value = l-r;
          return(value);
          break;
        case MULT_EXPR:
          l = cstNodeValue(left);
          r = cstNodeValue(right);
          value = l*r;
          return(value);
          break;
        case DIV_EXPR:
          l = cstNodeValue(left);
          r = cstNodeValue(right);
          value = l/r;
          return(value);
          break;
        case EXP_EXPR:
          l = cstNodeValue(left);
          r = cstNodeValue(right);
          return(pow(l,r));
          break;
        case CALL_EXPR:
          r = cstNodeValue(right);
          {
            char *name = NODE_NAME(left);
            if (*name == 's' && !strcmp(name, "sin")) value = ((double)sin(r));
            else if (*name == 's' && !strcmp(name, "sinh")) value = ((double)sinh(r));
            else if (*name == 'c' && !strcmp(name, "cos")) value = ((double)cos(r));
            else if (*name == 'c' && !strcmp(name, "cosh")) value = ((double)cosh(r));
            else if (*name == 'e' && !strcmp(name, "exp")) value = ((double)exp(r));
            else if (*name == 'l' && !strcmp(name, "log")) value = ((double)log(r));
            else if (*name == 't' && !strcmp(name, "tan")) value = ((double)tan(r));
            else if (*name == 'a' && !strcmp(name, "atan")) value = ((double)atan(r));
            else if (*name == 'a' && !strcmp(name, "arctan")) value = ((double)atan(r));
            else if (*name == 's' && !strcmp(name, "sqrt")) value = ((double)sqrt(r));
          }
          break;
        default:
          break;
        }
    }
  return(0.0);
}
// in order to pass high precision inputs to mpfr, we
// need the string form of the input, not the converted double value.
// This utility try to return the string value of input to buf. This
// works only for straight constants, like 0.123. It will not work
// for expressions, even 1.2 + 2.4.  If string value cannot be obtained,  
// it returns a reduced 16 digit double value to buf. This can always be
// done for constant expressions, like sin(1.2) +3.
//
char *cstNodeStringValue(Node node, char *buf ) {
  if (node)
    {
      int code = NODE_CODE(node);
      Node left = NODE_LEFT(node);
      double value;
      char *p, tbuf[128], sbuf[128];
      
      switch(code)
        {
        case INT_CST:
          strcpy(buf, NODE_NAME(node));
          break;
        case FLOAT_CST:
          strcpy(buf, NODE_INFO(node));
	  break;
        case NEGATE_EXPR:
          p =cstNodeStringValue(left,tbuf);
          sprintf(buf, "-%s", p);
          break;
        case ID_NODE:
	  {
            Node def = variables[ID_V_INDEX(node)].def;
	    p = cstNodeStringValue(def,sbuf);
	    sprintf(buf, "%s", p);	    
          }
	  break;
	default:
	  value = cstNodeValue(node);
	  sprintf(buf,"%.16g", value);
	  break;
	}
    } else {
    strcpy(buf, "0.0");
  }
  return buf;
}


/* Go through the variable list, build companion variables
 * for sin and cos.
 * This procedure is invoked right after all expresions
 * has been decomposed. So the argument to sin and cos
 * is always a simple variable.
 */
void makeCompanionVariables(void)
{
  int i, varcount = nvars;
  
  for(i = 0; i < varcount; i++)
    {
      Node name = variables[i].name;
      Node def = variables[i].def;
      int code = NODE_CODE(def);
      if (code == CALL_EXPR && NODE_IS_CST(def) == 0) /* f(var) */
	{
	  Node left  = NODE_LEFT(def);
	  Node right = NODE_RIGHT(def);
	  
          if (!strcmp(NODE_NAME(left), "sin"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  Node nvar = getTmpVar();
		  Node cpn  = install_id("cos");
		  Node ndef = build_op(CALL_EXPR, cpn, right);
		  insert_one_variable(nvar, ndef);
		  NODE_BUDY(nvar) = name;
		  NODE_BUDY(name) = nvar;
		  NODE_SUM_BLOCK(nvar) = NODE_SUM_BLOCK(name);
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar);}
		}
	    }
          else if (!strcmp(NODE_NAME(left), "cos"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  Node nvar = getTmpVar();
		  Node cpn  = install_id("sin");
		  Node ndef = build_op(CALL_EXPR, cpn, right);
		  insert_one_variable(nvar, ndef);
		  NODE_BUDY(nvar) = name;
		  NODE_BUDY(name) = nvar;
		  NODE_SUM_BLOCK(nvar) = NODE_SUM_BLOCK(name);
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar);}
		}
	    }
          else   if (!strcmp(NODE_NAME(left), "sinh"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  Node nvar = getTmpVar();
		  Node cpn  = install_id("cosh");
		  Node ndef = build_op(CALL_EXPR, cpn, right);
		  insert_one_variable(nvar, ndef);
		  NODE_BUDY(nvar) = name;
		  NODE_BUDY(name) = nvar;
		  NODE_SUM_BLOCK(nvar) = NODE_SUM_BLOCK(name);
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar);}
		}
	    }
          else if (!strcmp(NODE_NAME(left), "cosh"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  Node nvar = getTmpVar();
		  Node cpn  = install_id("sinh");
		  Node ndef = build_op(CALL_EXPR, cpn, right);
		  insert_one_variable(nvar, ndef);
		  NODE_BUDY(nvar) = name;
		  NODE_BUDY(name) = nvar;
		  NODE_SUM_BLOCK(nvar) = NODE_SUM_BLOCK(name);
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar);}
		}
	    } 
          else if (!strcmp(NODE_NAME(left), "tan"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  SumP sum = NODE_SUM_BLOCK(name);
		  Node nvar, nvar1 = getTmpVar();
		  Node cpn, cpn1  = install_id("cos");
		  Node ndef, ndef1 = build_op(CALL_EXPR, cpn1, right);
		  insert_one_variable(nvar1, ndef1);
		  nvar = getTmpVar();
		  ndef = build_op(MULT_EXPR, nvar1, nvar1);
		  insert_one_variable(nvar, ndef);
		  NODE_BUDY(name) = nvar;
		  /* NODE_BUDY(nvar) = name; */
		  NODE_SUM_BLOCK(nvar) = sum;
		  NODE_SUM_BLOCK(nvar1) = sum;
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar); SET_NODE_IS_LOCAL(nvar1);}
		  ID_DEPEND_B(name) = nvar1; /* !!! */
		  nvar = getTmpVar();
		  cpn  = install_id("sin");
		  ndef = build_op(CALL_EXPR, cpn, right);
		  insert_one_variable(nvar, ndef); 
		  NODE_SUM_BLOCK(nvar) = sum;
		  NODE_BUDY(nvar1) = nvar;
		  NODE_BUDY(nvar) = nvar1;
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar);}
		}
	    }
          else if (!strcmp(NODE_NAME(left), "tanh"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  SumP sum = NODE_SUM_BLOCK(name);
		  Node nvar, nvar1 = getTmpVar();
		  Node cpn, cpn1  = install_id("cosh");
		  Node ndef, ndef1 = build_op(CALL_EXPR, cpn1, right);
		  insert_one_variable(nvar1, ndef1);
		  nvar = getTmpVar();
		  ndef = build_op(MULT_EXPR, nvar1, nvar1);
		  insert_one_variable(nvar, ndef);
		  /* NODE_BUDY(nvar) = name;*/
		  NODE_BUDY(name) = nvar;
		  NODE_SUM_BLOCK(nvar) = sum;
		  NODE_SUM_BLOCK(nvar1) = sum;
		  ID_DEPEND_B(name) = nvar1; /* !!! */
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar); SET_NODE_IS_LOCAL(nvar1);}

		  nvar = getTmpVar();
		  cpn  = install_id("sinh");
		  ndef = build_op(CALL_EXPR, cpn, right);
		  insert_one_variable(nvar, ndef); 
		  NODE_BUDY(nvar1) = nvar;
		  NODE_BUDY(nvar) = nvar1;
		  NODE_SUM_BLOCK(nvar) = sum;
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar);}
		}
	    }
          else if (!strcmp(NODE_NAME(left), "atan") || !strcmp(NODE_NAME(left), "arctan"))
	    {
              if (ID_BUDY(name) == NULL)
		{
		  Node nvar, nvar1 = getTmpVar();
		  Node ndef, ndef1 = build_op(MULT_EXPR, right, right);
		  Node one = build_float("1.0");
		  insert_one_variable(nvar1, ndef1);
		  nvar = getTmpVar();
		  ndef = build_op(PLUS_EXPR, one, nvar1);
		  insert_one_variable(nvar, ndef);
		  NODE_BUDY(name) = nvar;
		  ID_DEPEND_A(nvar) = one; /* !!! */
		  ID_DEPEND_B(nvar) = nvar1; /* !!! */
		  ID_DEPEND_B(name) = nvar; /* !!! */
		  NODE_SUM_BLOCK(nvar) = NODE_SUM_BLOCK(name);
		  NODE_SUM_BLOCK(nvar1) = NODE_SUM_BLOCK(name);
                  if (NODE_IS_LOCAL(name)) { SET_NODE_IS_LOCAL(nvar); SET_NODE_IS_LOCAL(nvar1);}
		}
	    }
	  /* may be more later */
	}
    }
}
/***********************************************************************
 *
 *  return the definition (expression) of a variable.
 */
Node findVarDef(Node node)
{
  if (node && NODE_CODE(node) == ID_NODE && ID_IS_DEFINED(node) != 0)
    {
      int i;
      for(i = 0; i < nvars; i++)
        if (variables[i].name == node) return(variables[i].def);
    }
  return(NULL);
}

/******************************************************************
 *
 *  Find and mark dependency relations among variables.
 *  Each node has at most 2 dependents.
 *  Name: var name
 *  Def:  var definition or NULL like in array_ref, array_idx and
 *        loop index.
 */
Node makeArrayIdxDependce(Node idx)
{
  if (idx && NODE_CODE(idx) == ARRAY_IDX)
    {
      Node  var = NODE_LEFT(idx), right = NODE_RIGHT(idx);
      Node  nodeList[20];
      int   i, cnt = 0;
      int   code = NODE_CODE(var);

      if (code == ARRAY_REF)
	{
	  Node nvar = getTmpVar();
	  insert_one_variable(nvar, var); 
	  NODE_FLAGS(nvar) = NODE_FLAGS(var);
	  NODE_FLAGS(nvar) &= ~DEP_BIT;
	  NODE_SUM_BLOCK(nvar) = NODE_SUM_BLOCK(var);
          if (ID_IS_INT(nvar)) NODE_NAME(nvar)[0] = 'i';
	  else  NODE_NAME(nvar)[0] = 'c';
	  var = nvar;
	  canonicalTree(var);	  
	}
      if (VAR_DEPENDED(var) == 0) {nodeList[cnt++] = var;}
      while(right)
	{
	  Node left = NODE_LEFT(right);
          if (VAR_DEPENDED(left) == 0 && (ID_IS_VAR(left) == 0 || NODE_IS_INDEX(left) != 0))
	    {
              for(i = 0; i < cnt; i++) { if (nodeList[i] == var) break;}
              if (i == cnt) { nodeList[cnt++] = var;}
	    }
          if (NODE_DEPEND_A(left) == NULL) makeDependencyGraph(left, findVarDef(left));
	  right = NODE_RIGHT(right);
	}
      if (cnt > 0)
	{
	  for(i = 1; i < cnt; i++)
	    {
	      Node prev = nodeList[i-1];
	      Node cur =  nodeList[i];
	      SET_VAR_DEPENDED(cur);
	      NODE_DEPEND_C(prev) = cur;
	    }
	  return(nodeList[0]);
	}
    }
  return(NULL);
}

void makeDependencyGraph(Node name, Node def)
{
  Node  depa = NULL, depb = NULL;

  if (name && def && NODE_CODE(name) == ID_NODE && NODE_DEPEND_A(name) == NULL)
    {
      int   code = NODE_CODE(def);
     
      switch(code)
	{
	case ID_NODE:
          if (ID_IS_EXTERN(def) == 0) depa = def;
	  break;
	case INT_CST:
	case FLOAT_CST:
	  break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case DIV_EXPR:
	case NEGATE_EXPR:
	case EXP_EXPR:
        case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
	case AND_EXPR:
	case OR_EXPR:
	  depa = NODE_LEFT(def);
	  depb = NODE_RIGHT(def);
	  break;
        case IF_ELSE_EXPR:
	  depa = NODE_LEFT(def);
	  depb = NODE_RIGHT(def);
	  {
	    Node cond = IF_ELSE_COND(def);
	    makeDependencyGraph(cond,findVarDef(cond));	    
	  }
	  break;
	case CALL_EXPR:
	  depa = NODE_RIGHT(def);
	  break;
	case SUM_EXPR:
	  {
	    SumP sum = NODE_SUM_BLOCK(def);
	    depa = SUMB_EXPR(sum);
	  }
	break;
	case ARRAY_REF:
	  depa = NODE_RIGHT(def);
	  break;
	case ARRAY_IDX:
	  fprintf(stderr, "+++++++ should never be here+++++\n");
	  exit(11);
	  break;
	default: break;
	}
    }

  if (depa && NODE_CODE(depa) == ARRAY_REF)    { depa = NODE_RIGHT(depa); }
  if (depb && NODE_CODE(depb) == ARRAY_REF)    { depb = NODE_RIGHT(depb); }
  if (depa && NODE_CODE(depa) == ARRAY_IDX )   { depa = makeArrayIdxDependce(depa);}
  if (depb && NODE_CODE(depb) == ARRAY_IDX )   { depb = makeArrayIdxDependce(depb);}
  
  if (depa)
    {
      Node def = findVarDef(depa);
      if (VAR_DEPENDED(depa) == 0 && (ID_IS_VAR(depa) == 0 || NODE_IS_INDEX(depa) != 0) && ID_IS_EXTERN(depa) == 0)
	{
	  SET_VAR_DEPENDED(depa);
	  NODE_DEPEND_A(name) = depa;
	}
      if (NODE_DEPEND_A(depa) == NULL) makeDependencyGraph(depa, def);
    }
  if (depb)
    {
      Node def = findVarDef(depb);
      if ( VAR_DEPENDED(depb) == 0 && (ID_IS_VAR(depb) == 0 || NODE_IS_INDEX(depb) != 0) && ID_IS_EXTERN(depb) == 0)
        {
	  SET_VAR_DEPENDED(depb);
          if (NODE_DEPEND_A(name) == NULL)  NODE_DEPEND_A(name) =depb;
          else NODE_DEPEND_B(name) = depb;
        }
      if (NODE_DEPEND_A(depb) == NULL) makeDependencyGraph(depb, def);
    }
}

/***********************************************************************/
void showDepWork(Node node)
{
  if (node && NODE_CODE(node) == ID_NODE && ID_IS_DEFINED(node) != 0)
    {
      Node depa = NODE_DEPEND_A(node);
      Node depb = NODE_DEPEND_B(node);
      Node depc = NODE_DEPEND_C(node);
      Node cond = IF_ELSE_COND(node);
      if (depa) fprintf(stderr,"%s->%s", NODE_NAME(node), NODE_NAME(depa));
      if (depb) fprintf(stderr,"     %s->%s", NODE_NAME(node), NODE_NAME(depb));
      if (depc) fprintf(stderr,"     %s->%s", NODE_NAME(node), NODE_NAME(depc));
      if (cond) fprintf(stderr,"     %s->%s", NODE_NAME(node), NODE_NAME(cond));
      if (NODE_BUDY(node)) fprintf(stderr, "     [<budy %s>=%s]", NODE_NAME(node),NODE_NAME(NODE_BUDY(node)));
      fprintf(stderr, "\n");
      showDepWork(depa);
      showDepWork(depb);
      showDepWork(depc);
      showDepWork(cond);
    }
}

void showDepend()
{
  int i;
  for(i = 0; i < neqns; i++)
    {
      Node def = equations[i].def;
      if (NODE_IS_CST(def) == 0)
	{
	  fprintf(stderr, "\n===== Dependency graph for %s =======\n", NODE_NAME(def));
	  showDepWork(def);
	}
    }
}
/***********************************************************************/
void outputVarDependcyGraph(Node name)
{
  if (name && NODE_CODE(name) == ID_NODE && ID_IS_CST(name) == 0)
    {
      if (NODE_COUNTED(name) == 0)
	{
	  Node depa = NODE_DEPEND_A(name);
	  Node depb = NODE_DEPEND_B(name);
	  Node depc = NODE_DEPEND_C(name);
	  Node budy = NODE_BUDY(name);

	  outputVarDependcyGraph(depa);
	  outputVarDependcyGraph(depb);
	  outputVarDependcyGraph(depc);

          if (NODE_COUNTED(name) == 0)
	    {
              if (IS_INTERNAL_SUM(name))
		{
		  Node def = variables[ID_V_INDEX(name)].def;
		  SumP sum;
		  int  ss;
                  if (!def || NODE_CODE(def) != SUM_EXPR)
		    {
		      fprintf(stderr, "Internal error: SUM has the wrong node code\n");
		      exit(12);
		    }
		  sum = NODE_SUM_BLOCK(def);
		  outputGVarsInSum(sum);
		  SUMB_VOFFSET(sum) = goffset + varcountFinal;
		  ss = SUMB_TO(sum) -SUMB_FROM(sum) + 1;
		  goffset += ss * SUMB_LVAR(sum);
		}
	      varlistFinal[varcountFinal] = name;
	      ID_VINDEX(name) = varcountFinal;
	      ID_VOFFSET(name) = goffset;
	      SET_VAR_GLOBAL_VAR(name);
	      SET_NODE_COUNTED(name);
	      varcountFinal++;
              if (budy && NODE_COUNTED(budy) == 0)
		outputVarDependcyGraph(budy);
	    }
	}
    }
}


void outputConstDependcyGraph(Node name)
{
  if (name && NODE_CODE(name) == ID_NODE )
    {
      Node depa = NODE_DEPEND_A(name);
      Node depb = NODE_DEPEND_B(name);
      Node depc = NODE_DEPEND_C(name);
      Node budy = NODE_BUDY(name);
      Node cond = IF_ELSE_COND(name);

      outputConstDependcyGraph(cond);
      outputConstDependcyGraph(depa);
      outputConstDependcyGraph(depb);
      outputConstDependcyGraph(depc);

      if (IS_INTERNAL_SUM(name) && NODE_CCOUNTED(name) == 0)
        {
          Node def = variables[ID_V_INDEX(name)].def;
          SumP sum;
          int  ss;
          SET_NODE_CCOUNTED(name);
          SET_NODE_COUNTED(name);
          if (!def || NODE_CODE(def) != SUM_EXPR)
            {
              fprintf(stderr, "Internal error: SUM has the wrong node code (C)\n");
              exit(12);
            }
          sum = NODE_SUM_BLOCK(def);
          outputCVarsInSum(sum);
          SUMB_COFFSET(sum) = cgoffset + constcountFinal;
          ss = SUMB_TO(sum) -SUMB_FROM(sum) + 1;
          cgoffset += ss * SUMB_LCST(sum);
        }

      if (NODE_COUNTED(name) == 0)
        {
          if (NODE_IS_CST(name) && NODE_IS_INT(name) == 0)
            {
              SET_NODE_COUNTED(name);
              constlistFinal[constcountFinal] = name;
              ID_CINDEX(name) = constcountFinal;
              ID_COFFSET(name) = cgoffset;
              constcountFinal++;
              SET_VAR_GLOBAL_CVAR(name);
              if (budy && NODE_COUNTED(budy) == 0)
                outputConstDependcyGraph(budy);
            }
        }
    }
}
void outputIntDependcyGraph(Node name)
{
  if (name && NODE_CODE(name) == ID_NODE )
    {
      Node depa = NODE_DEPEND_A(name);
      Node depb = NODE_DEPEND_B(name);
      Node depc = NODE_DEPEND_C(name);
      Node budy = NODE_BUDY(name);
      Node cond = IF_ELSE_COND(name);

      outputIntDependcyGraph(cond);
      outputIntDependcyGraph(depa);
      outputIntDependcyGraph(depb);
      outputIntDependcyGraph(depc);

      if (IS_INTERNAL_SUM(name) && NODE_ICOUNTED(name) == 0)
        {
          Node def = variables[ID_V_INDEX(name)].def;
          SumP sum;
          int  ss;
          if (!def || NODE_CODE(def) != SUM_EXPR)
            {
              fprintf(stderr, "Internal error: SUM has the wrong node code (I)\n");
              exit(12);
            }
          SET_NODE_COUNTED(name);
          SET_NODE_ICOUNTED(name);
          sum = NODE_SUM_BLOCK(def);
          outputIVarsInSum(sum);
          SUMB_IOFFSET(sum) = igoffset + ivarcountFinal;
          ss = SUMB_TO(sum) -SUMB_FROM(sum) + 1;
          igoffset += ss * SUMB_LINT(sum);
        }
      if ( NODE_COUNTED(name) == 0)
        {
          if (NODE_IS_CST(name) && NODE_IS_INT(name))
            {
              SET_NODE_COUNTED(name);
              ivarFinal[ivarcountFinal] = name;
              ID_IINDEX(name) = ivarcountFinal;
              ID_IOFFSET(name) = igoffset;
              ivarcountFinal++;
              SET_VAR_GLOBAL_IVAR(name);
              if (budy && NODE_COUNTED(budy) == 0)
                outputIntDependcyGraph(budy);
            }
        }
    }
}
/***********************************************************************
 *
 *  Output all global avariables name depends on.
 */


void listAllVarsIn(Node def)
{
  if (def)
    {
      Node depa = NODE_DEPEND_A(def);
      Node depb = NODE_DEPEND_B(def);
      Node depc = NODE_DEPEND_C(def);
      Node cond = IF_ELSE_COND(def);
      Node budy = NODE_BUDY(def);
      listAllVarsIn(depa);
      listAllVarsIn(depb);
      listAllVarsIn(depc);
      listAllVarsIn(cond);

      if ( NODE_CODE(def) == ID_NODE)
	{
          if ( ID_COUNTED(def) && IS_GCOUNTED(def) == 0)
	    {
              if (IS_INTERNAL_SUM(def))
		{
		  Node ndef = variables[ID_V_INDEX(def)].def;
                  if (ndef && NODE_CODE(ndef) == SUM_EXPR)
		    {
		      SumP sum = NODE_SUM_BLOCK(ndef);
		      Node expr = SUMB_EXPR(sum);
		      listAllVarsIn(expr);
		    }
		}
              if (NODE_IS_LOCAL(def) == 0)
		{
		  SET_IS_GCOUNTED(def);
		  allVarsList[allVarsCount] = def;
		  allVarsCount++;
		  listAllVarsIn(budy);
		}
	    }
	}
    }
}

// 20220609 start
/************************************************************
 *
 *  clear out all DEP_MARKER_BIT in all Variables
 */
void clear_all_dep_markers() {
  int i;
  for(i = 0; i < nvars + neqns + 2; i++) {
    Node node = varlistFinal[i];
    if(node) {
      CLEAR_JET_DEP_MARKER(node);
    }
  }
}
/************************************************************
 *
 *  follow the parse tree of name mark all
 *  dependent ID nodes
 */

void mark_all_depend_vars(Node name)
{
  Node  depa = NULL, depb = NULL;

  if (name && NODE_CODE(name) == ID_NODE && ID_IS_CST(name) == 0 &&  IS_JET_DEP_MARKED(name) == 0)
    {
      Node  def=findVarDef(name);
      
      SET_JET_DEP_MARKER(name);

      if(def != NULL) {
	int   code = NODE_CODE(def);

	switch(code)
	  {
	  case ID_NODE:
	    if (ID_IS_EXTERN(def) == 0) depa = def;
	    break;
	  case INT_CST:
	  case FLOAT_CST:
	  break;
	  case PLUS_EXPR:
	  case MINUS_EXPR:
	  case MULT_EXPR:
	  case DIV_EXPR:
	  case NEGATE_EXPR:
	  case EXP_EXPR:
	  case GE_EXPR:
	  case GT_EXPR:
	  case LE_EXPR:
	  case LT_EXPR:
	  case EQ_EXPR:
	  case NEQ_EXPR:
	  case AND_EXPR:
	  case OR_EXPR:
	    depa = NODE_LEFT(def);
	    depb = NODE_RIGHT(def);
	    break;
	  case IF_ELSE_EXPR:
	    depa = NODE_LEFT(def);
	    depb = NODE_RIGHT(def);
	    {
	      Node cond = IF_ELSE_COND(def);
	      mark_all_depend_vars(cond);	    
	    }
	    break;
	  case CALL_EXPR:
	    depa = NODE_RIGHT(def);
	    break;
	  case SUM_EXPR:
	    {
	      SumP sum = NODE_SUM_BLOCK(def);
	      depa = SUMB_EXPR(sum);
	    }
	    break;
	  case ARRAY_REF:
	    depa = NODE_RIGHT(def);
	    break;
	  case ARRAY_IDX:
	    //fprintf(stderr, "+++++++ should never be here+++++\n");
	    //exit(11);
	    break;
	  default: break;
	  }
      }

      if (depa && NODE_CODE(depa) == ARRAY_REF)    { depa = NODE_RIGHT(depa); }
      if (depb && NODE_CODE(depb) == ARRAY_REF)    { depb = NODE_RIGHT(depb); }
      if (depa && NODE_CODE(depa) == ARRAY_IDX )   { depa = makeArrayIdxDependce(depa);}
      if (depb && NODE_CODE(depb) == ARRAY_IDX )   { depb = makeArrayIdxDependce(depb);}
  
      mark_all_depend_vars(depa);	    	  
      mark_all_depend_vars(depb);	    	  	  
    }
}


void checkJetVars() {
  int i, j, k, m, n, z=0, **deps;

  if(neqns == njetVars) return;
  
  deps = (int **) malloc( neqns * sizeof(int *));
  for(i = 0; i < neqns; i++) {
    deps[i] = (int *) malloc((neqns+1) * sizeof(int));
    bzero(deps[i], (neqns+1) * sizeof(int));
  }
  for(i = 0; i < neqns; i++) {
    Node name = equations[i].name;
    Node def = equations[i].def;
    clear_all_dep_markers();
    mark_all_depend_vars(def);
    deps[i][0] = ID_VINDEX(name);    
    for(j = 0; j < neqns; j++) {
      Node evar = equations[j].name;
      if(evar && IS_JET_DEP_MARKED(evar)) {      
	deps[i][j+1] = 1;
      }
    }
  }

  if(debug) {
    fprintf(stderr, "Var Dependency\n      idx");
    for(i = 0; i < neqns; i++) {
      fprintf(stderr, "%4s ", ID_GIVEN_NAME(equations[i].name));          
    }
    fprintf(stderr, "\n");    
    for(i = 0; i < neqns; i++) {
      fprintf(stderr, "%4s: ", ID_GIVEN_NAME(equations[i].name));    
      for(j = 0; j < neqns+1; j++) {
	fprintf(stderr, "%04d ", deps[i][j]);
      }
      fprintf(stderr, "\n");    
    }
  }

  
  for(i = 0; i < neqns; i++) {
    Node name = equations[i].name;
    if(!VAR_IS_DECLARED_JET(name)) {
      Node def = equations[i].def;
      clear_all_dep_markers();
      mark_all_depend_vars(def);
      for(j = 0; j < njetVars; j++) {
	Node jvar = jetVars[j];
	if(jvar && IS_JET_DEP_MARKED(jvar)) {
	  fprintf(stderr, "Error: Variable '%s' depends on jet variable '%s'. ", ID_GIVEN_NAME(name), ID_GIVEN_NAME(jvar));
	  z++;
	  m = ID_VINDEX(jvar);
	  n = -1;
	  for(k = 0; k <  neqns; k++) {
	    if(deps[k][0] == m) {
	      n = k;
	      break;
	    }
	  }
	  if(n >= 0) {
	    if(deps[n][i+1] > 0) {
	      z++;
	      fprintf(stderr, " '%s' depends on '%s' as well, which makes the jet variables inconsistent.",  ID_GIVEN_NAME(jvar), ID_GIVEN_NAME(name));
	    }
	  }
	  fprintf(stderr, " Please declare '%s' as a jet variable.\n", ID_GIVEN_NAME(name));	  
	}
      }
    }
  }

  if(z) { /* inconsitent jet vars */
    if(!ignoreJetInconsistency)
      exit(1);
  }
}
// 20220609 end


/***********************************************************************
 *
 * Generate all variables.
 */
void genVariables(void)
{
  int i, jcnt=state_jet_vars,varcount = nvars,pcnt=0, qjcnt=0;

  /* verify that all declared jet variables are defined */
  pcnt = 0;
  for(i=0; i<njetVars; i++) {
    Node name = jetVars[i];
    int k = ID_EINDEX(name);
    if (k == 0) {
      if (VAR_IS_JET_PARAM(name)) {
	fprintf(stderr, "Error: variable '%s' is declared as a variation parameter, but is never defined.\n", ID_GIVEN_NAME(name));
      } else {
	fprintf(stderr, "Error: variable '%s' is declared as a jet variable, but is never defined.\n", ID_GIVEN_NAME(name));
      }
      pcnt++;
    }
  }
  if (pcnt) { exit(17);}

  /* generate dependency tree */
  for(i = 0; i < neqns; i++)
    {
      Node def = equations[i].def;

      if (NODE_CODE(def) != ID_NODE)
	{
	  fprintf(stderr,"Internal Error: RHS of %s'=%s is not a variable.\n",
		  NODE_NAME(equations[i].name), NODE_NAME(equations[i].def));
	  exit(10);
	}
      makeDependencyGraph(def,findVarDef(def));
    }


  if (debug) showDepend();
  
  /* count local vars in sums */
  if (havesum)
    {
      for(i = 0; i < varcount; i++)      
	{
	  Node def =   variables[i].def;

          if (def && NODE_CODE(def) == SUM_EXPR)
	    countSumVariables(def);
	} 
    }

  /* save jet var */
  if (state_jet_vars > 0) {
    int length = (nvars + neqns + 1)* sizeof(Node);
    jetVarsList = (Node *)my_malloc( length );
    memset((char *)jetVarsList,0, length);
  }

  /* variational parameters */
  if (param_jet_vars > 0) {
    int length = (param_jet_vars + 1)* sizeof(Node);
    jetParamsList = (Node *)my_malloc( length );
    memset((char *)jetParamsList,0, length);
  }

  /* caution: this modifies the var count */
  pcnt=0;
  qjcnt=0;
  for(i = 0; i < neqns; i++)
    {
      Node name = equations[i].name;
      Node def = equations[i].def; /* def must be a variable */

      insert_one_variable(name, def);
      ID_VINDEX(name) = i;
      ID_V_INDEX(name) = nvars-1;
      /* variation parameters must be defined by p'=0 */
      if (VAR_IS_JET_PARAM(name)) {
	int j=ID_PINDEX(name);
        if (j == 0) {
          fprintf(stderr, "Internal Error: variation parameter %s is not marked correctly.\n", NODE_GIVEN_NAME(name));
	  exit(7);
	} else {
	  jetParamsList[j-1] = name;
	}
	pcnt++;
      } else if (VAR_IS_JET(name)) {
	qjcnt++;
      }
    }
  /* end caution */

  if (qjcnt < state_jet_vars) {
    fprintf(stderr, "Error:  Only %d of %d jet variables are defined.\n", qjcnt, state_jet_vars);
    exit(15);
  }

  /* make sure all var parameters are defined */
  if (pcnt < param_jet_vars) { // ok, some variational parameters are undefined.
    fprintf(stderr, "Error:  You have declared %d variational parameters, ",param_jet_vars);
    if (pcnt == 0) { fprintf(stderr, "none of them are defined.\n");}
    else { fprintf(stderr, "only %d %s defined.\n",pcnt, pcnt>1? "of them are" : "of them is");}
    if (pcnt) {
      fprintf(stderr, "        Defined parameters: "); 
      for(i=0; i<pcnt; i++) {
	Node var = jetParamsList[i];
	fprintf(stderr, "%s ", ID_GIVEN_NAME(var));
      }
      fprintf(stderr, "\n");
    }
    exit(13);
  }



  varcount = nvars; /* updated var count */

  /* find and mark all JET vars */
  {
    int found=0;
    int limit=(1<<10);
    
    do {
      found=0;
      limit--;
      for(i = 0; i < nvars; i++) {
	int n=0;
	Node name = variables[i].name;
	Node def = variables[i].def; 
	Node depa=NULL, depb=NULL;
        if (1) {
	  int   code = NODE_CODE(def);
	  switch(code)
	    {
	    case ID_NODE:
	      n=VAR_IS_JET(def);
	      break;
	    case INT_CST:
	    case FLOAT_CST:
	      break;
	    case PLUS_EXPR:
	    case MINUS_EXPR:
	    case MULT_EXPR:
	    case DIV_EXPR:
	    case NEGATE_EXPR:
	    case EXP_EXPR:
	    case GE_EXPR:
	    case GT_EXPR:
	    case LE_EXPR:
	    case LT_EXPR:
	    case EQ_EXPR:
	    case NEQ_EXPR:
	    case AND_EXPR:
	    case OR_EXPR:
	      depa = NODE_LEFT(def);
	      depb = NODE_RIGHT(def);
	      break;
	    case IF_ELSE_EXPR:
	      depa = NODE_LEFT(def);
	      depb = NODE_RIGHT(def);
	      break;
	    case CALL_EXPR:
	      depa = NODE_RIGHT(def);
	      break;
	    case SUM_EXPR:
	      {
		SumP sum = NODE_SUM_BLOCK(def);
		depa = SUMB_EXPR(sum);
	      }
	      break;
	    case ARRAY_REF:
	      depa = NODE_RIGHT(def);
	      break;
	    case ARRAY_IDX:
	      fprintf(stderr, "+++++++ should never be here+++++\n");
	      exit(11);
	      break;
	    default: break;
	    }
          if (depa) n |= VAR_IS_JET(depa);
          if (depb) n |= VAR_IS_JET(depb);
          if (n) {
            if (!VAR_IS_JET(name) || !VAR_IS_JET(def)) {
	      SET_VAR_IS_JET(def);
	      SET_VAR_IS_JET(name);
	      found=1;
	    }
	  }
	}
      }
    } while(found && limit > 0);
    if (limit<=0) { // should never happen
      fprintf(stderr, "Internal error: infinite loop detected when looking for JET variables\n");
      exit(12);
    }
  }
  for(i = 0; i < neqns; i++)
    {
      Node name = equations[i].name;

      if (VAR_IS_JET(name)) {
	int k = (ID_SINDEX(name));
        if (k>0) {
	  ID_JINDEX(name)= (ID_SINDEX(name)) -1;
	} else {
	  ID_JINDEX(name)= jcnt++;
	}
	k=ID_JINDEX(name);
	jetVarsList[k] = name;
	SET_JET_VAR_COUNTED(name);
      }
    }

  /* output variables  */
  goffset = 0;
  varlistFinal = (Node *)my_malloc( (nvars + neqns + 2)* sizeof(Node));
  bzero(varlistFinal, (nvars + neqns + 2)* sizeof(Node));

  for(i = 0; i < neqns; i++)            /* state vars first */
    {
      Node name =  equations[i].name;
      varlistFinal[i] = name;
      ID_VINDEX(name) = i;
      SET_ID_COUNTED(name);
      SET_VAR_GLOBAL_VAR(name);       /* mark state variables global */
      SET_VAR_IS_STATE_VAR(name);
    }
  varcountFinal = neqns;

  /* all other tmp vars */
  for(i = 0; i < neqns - nonautonomous; i++)      /* t'=1 is handled separately: 3/28/99 */
    {
      Node def = equations[i].def;
      outputVarDependcyGraph(def);
    }

  /* output constant variables (don't need derivatives) */ 
  cgoffset = 0;
  constcountFinal = 0;
  constlistFinal = (Node *)my_malloc( (nvars + 2)* sizeof(Node));
  for(i = 0; i < neqns; i++)           /* all other tmp vars */
    {
      Node def = equations[i].def;
      outputConstDependcyGraph(def);
    }

  
  /* list boolean variables used in IF_ELSE_EXPR */
  ivarcountFinal = 0;  
  ivarFinal = (Node *)my_malloc( (nvars+2) * sizeof(Node));
  for(i = 0; i < neqns; i++)           /* all other tmp vars */
    {
      Node def = equations[i].def;
      outputIntDependcyGraph(def);
    }

 
  /* all variables are listed here, state vars first. The rest
   * are build up based on dependency relations.
   */
  allVarsList = (Node *)my_malloc( (nvars + neqns + 10)* sizeof(Node));
  allVarsCount = 0;
  for(i = 0; i < neqns; i++)            /* state vars first */
    {
      Node name =  equations[i].name;
      allVarsList[i] = name;
      SET_IS_GCOUNTED(name);
      SET_VAR_GLOBAL_VAR(name);
    }  
  allVarsCount = neqns;
  for(i = 0; i < neqns; i++) 
    {
      Node def =  equations[i].def;
      listAllVarsIn(def);
    }


  /* count jet vars */
  for(i = 0; i < allVarsCount; i++)      
    {
      Node name =   allVarsList[i];
      if (VAR_IS_JET(name)) {
        if (!JET_VAR_COUNTED(name)) {
	  int k = (ID_SINDEX(name));	  
          if (k>0) {
	    ID_JINDEX(name)= (ID_SINDEX(name)) -1;
	  } else {
	    ID_JINDEX(name)= jcnt++;
	  }
	  k = ID_JINDEX(name);
	  jetVarsList[k] = name;
	  SET_JET_VAR_COUNTED(name);
	}  
      }
    }
  total_jet_vars = jcnt;
  if (debug) fprintf(stderr, "Total number of JET vars: %d (including %d state jet vars, %d parameters)\n", jcnt, state_jet_vars, param_jet_vars);

  /* verify that variation parameters are all properly marked */
  for(i=0; i< jcnt; i++) {
    Node name = jetVarsList[i];
    if (VAR_IS_JET_PARAM(name)) {
      int k = ID_EINDEX(name);
      char *gname = ID_GIVEN_NAME(name);
      if (k == 0) {
	fprintf(stderr,"Variation parameter '%s' has to appear in an ODE like %s'=0;\n", gname, gname);
	exit(5);
      } else {
	Node rhs = equations[k-1].def; /* always a variabie */
	Node def  = variables[ID_V_INDEX(rhs)].def;
        if (!NODE_IS_CST(def)) {
	  fprintf(stderr,"The RHS of the ODE for variation parameter '%s' is not zero.\n",gname);
	  fprintf(stderr, "%s == %s\n",  NODE_NAME(equations[i].name), NODE_NAME(equations[i].def));
	  exit(5);
	} else {
	  double v = cstNodeValue(def);
          if (v != 0.0) {
	    fprintf(stderr,"The RHS of the ODE for variation parameter '%s' is not ZERO.\n",gname );
	    exit(5);
	  }
	}
      }
    }
  }
  if (debug) showVariables(stderr);
}
/***********************************************************************************************************/
void showVariables(FILE *fp)
{
  int i;


  fprintf(fp, "\n===================================================================================\n");
  fprintf(fp,   "=======                                                                      ======\n");
  fprintf(fp,   "=======                         Final Variable List                          ======\n");
  fprintf(fp, "\t    (%d + %d - %d) vars, (%d + %d) cvars and (%d + %d) ivars \n", varcountFinal, goffset,
	  total_jet_vars, constcountFinal, cgoffset, ivarcountFinal, igoffset);
  if(num_expr_vars) {
    fprintf(fp, "\t    Expression(s):\n");
    Expression expr = expression_list;
    while(expr) {
      Node name = EXPRESSION_NAME(expr);
      Node *vlist = EXPRESSION_LIST(expr);
      int  vcount = EXPRESSION_COUNT(expr);
      expr =  EXPRESSION_NEXT(expr);
      fprintf(fp, "\t\t  %s: ", NODE_NAME(name));
      for(i = 0; i < vcount; i++) { fprintf(fp,"%s,", NODE_NAME(vlist[i]));}
      fprintf(fp, "\n");
    }
  }
  fprintf(fp, "\t    (%d) JET vars\n", total_jet_vars);
  fprintf(fp, "\t    (%d) declared expressions\n", num_expr_vars);
  fprintf(fp,   "=======                                                                      ======\n");
  fprintf(fp,   "===================================================================================\n");

  for(i = 0; i < allVarsCount; i++)
    {
      char jet[16]=" "; char jidx[16]=" ";
      Node name = allVarsList[i];
      int  vidx = ID_V_INDEX(name);
      Node def = variables[vidx].def;
      if (VAR_IS_JET(name)) {
	jet[0] = '.'; 
	sprintf(jidx, "%d %d", ID_JINDEX(name), ID_SINDEX(name));
       } else {
	jet[0] = ' ';
	strcpy(jidx, " ");	
      }

      if (VAR_IS_GLOBAL_VAR(name))
	{
          if (name == timeVar)  fprintf(fp, "\t%s (time)\n", ID_NAME(name));
          else if (def && ID_IS_VAR(name)== 0)
	    fprintf(fp, "\t%s = %-40s V(%d %d) %s %s\n", ID_NAME(name), NODE_INFO(def), ID_VINDEX(name), ID_VOFFSET(name), jet, jidx);
	  else 
	    fprintf(fp, "\t%s   %-40s S(%d %d) %s %s\n", ID_NAME(name), "(state variable)", ID_VINDEX(name), ID_VOFFSET(name), jet, jidx);
	}
      else if (VAR_IS_GLOBAL_CVAR(name))
	{
	  fprintf(fp, "\t%s = %-40s C(%d %d) %s\n", ID_NAME(name), NODE_INFO(def), ID_CINDEX(name), ID_COFFSET(name),jet);
	}
      else  if (VAR_IS_GLOBAL_IVAR(name))
	{
	  fprintf(fp, "\t%s = %-40s I(%d %d) %s %s\n", ID_NAME(name), NODE_INFO(def), ID_IINDEX(name), ID_IOFFSET(name),
		  NODE_IS_A_NUMBER(name)? "(a number)" : " ", jet);
	}
      else
	{
	  fprintf(fp, "\n+++++\n+++++ Internal Error: Global var %s=%s is not marked as such (%d %d) (%d %d) (%d %d)\n+++++\n+++++\n",
		  ID_NAME(name), NODE_INFO(def), ID_VINDEX(name), ID_VOFFSET(name), ID_CINDEX(name), ID_COFFSET(name),
		  ID_IINDEX(name), ID_IOFFSET(name));

	}
      if (def && NODE_CODE(def) == SUM_EXPR)
	{
	  SumP sum = NODE_SUM_BLOCK(def);
	  int ss = SUMB_TO(sum) -SUMB_FROM(sum) + 1;
	  fprintf(fp, "\t\t+++ SUM HAS (%d+%d+%d) LOCAL VARS REPEADED %d TIMES. Offsets: (%d %d %d) %s +++\n", 
		  SUMB_LVAR(sum), SUMB_LCST(sum), SUMB_LINT(sum), ss, 
		  SUMB_VOFFSET(sum),SUMB_COFFSET(sum), SUMB_IOFFSET(sum),
		  NODE_NAME(SUMB_EXPR(sum)));
	  printSumBlockVariables(fp, sum);
	}
    }
}

/*********************************************************************************/
static void genMyFloatCode(void)
{
  fprintf(outfile, "%s\n", my_float_api_macros);
  fprintf(outfile, "%s\n", MY_FLOAT_FUN_MACRO);
//  if (genMyBlas) // move it to genMyJetCode to use auxiliary MY_FLOAT
  fprintf(outfile, "\n");
}

void genCode(void)
{
  int i,j, tmpcnt;

  if (genHeader) { genSampleHeader(); }
  else { fprintf(outfile, "#include \"%s\"\n\n", header_name);}
  if (genMain)   { genMainCode(); }
  if (genStep)   { genStepCode();}
  genMyFloatCode();
  if (genMyCoef) { genMyCoefCode(); }
  if (genMyCloud) { genMyCloudCode(); }
  else if (genMyJet)  { genMyJetCode(); }
  if (genJet == 0) return;

  fprintf(outfile,"/***********************************************************************\n");
  fprintf(outfile," *\n");
  fprintf(outfile," * Procedure generated by the TAYLOR translator. Do not edit!\n");
  fprintf(outfile," *\n");
  fprintf(outfile," * It needs the header file '%s' to compile.\n", header_name);
  fprintf(outfile," * Run taylor with the -header -o %s option to generate a sample '%s'\n\n", header_name, header_name);
  fprintf(outfile," * Translation info is at the end of this file.\n");
  fprintf(outfile," * %s\n", versionString);
  fprintf(outfile," ***********************************************************************/\n\n");

  fprintf(outfile, "#include <stdio.h>\n");
  fprintf(outfile, "#include <stdlib.h>\n");

  fprintf(outfile, "MY_FLOAT **%s_A(MY_FLOAT t, MY_FLOAT *x, int order, int rflag, MY_JET *sIn, MY_JET ***sOut)\n{\n", outName);

  fprintf(outfile, "   /* input: \n");
  fprintf(outfile, "      t:     current value of the time variable \n");
  fprintf(outfile, "      x:     array represent values of the state variables\n");
  fprintf(outfile, "      order: order of the taylor coefficients sought\n");
  fprintf(outfile, "      rflag: recompute flag. If you call this routine with one order \n");
  fprintf(outfile, "             first, but then decided that you need a higher order of the\n");
  fprintf(outfile, "             taylor polynomial, you can pass 0 to rflag. This routine \n");
  fprintf(outfile, "             will try to use the values already computed. Provided that \n");
  fprintf(outfile, "             both x and t have not been changed, and you did not modify \n");
  fprintf(outfile, "             the jet derivatives from the previous call.\n");
  fprintf(outfile, "      sIn:   array representing the input jet variables\n");
  fprintf(outfile, "             If NULL, the x values are assigned.\n");
  fprintf(outfile, "      sOut:  Pointer output jet variables\n");
  fprintf(outfile, "             If NULL, the pointer is not modified.\n");
  fprintf(outfile, "      Return Value:\n");
  fprintf(outfile, "            Two D Array, rows are the taylor coefficients of the\n");
  fprintf(outfile, "            state variables\n\n");
  fprintf(outfile, "     */\n");
  outputExtern();   /* first output extern declarations */

  if (ivarcountFinal + igoffset> 0) {
    fprintf(outfile, "    static int          _jz_ivars[%d];\n",  ivarcountFinal+igoffset);
    fprintf(outfile, "    #pragma omp threadprivate( _jz_ivars)\n");    
  }
  if (constcountFinal + cgoffset> 0) {
    fprintf(outfile, "    static MY_FLOAT     _jz_cvars[%d];\n", constcountFinal+cgoffset);
    fprintf(outfile, "    #pragma omp threadprivate( _jz_cvars)\n");        
  }
  fprintf(outfile, "    static MY_FLOAT     *_jz_jet[%d],  *_jz_save = NULL, *_jz_oneOverN=NULL,*_jz_theNs=NULL;\n", 
          varcountFinal+goffset);
  fprintf(outfile, "    static MY_FLOAT     _jz_tvar1, _jz_tvar2, _jz_tvar3, _jz_tvar4; /* tmp vars */\n");
  fprintf(outfile, "    static MY_FLOAT     _jz_uvar1, _jz_uvar2; /* tmp vars */\n");
  fprintf(outfile, "    static MY_FLOAT     _jz_svar1, _jz_svar2, _jz_svar3, _jz_svar4, _jz_svar5; /* tmp vars */\n");
  fprintf(outfile, "    static MY_FLOAT     _jz_wvar3, _jz_wvar4; /* tmp vars */\n");
  fprintf(outfile, "    static MY_FLOAT     _jz_zvar1, _jz_zvar2; /* tmp vars */\n");
  fprintf(outfile, "    static MY_FLOAT     _jz_MyFloatZERO, _kthtmp;\n");

  fprintf(outfile, "    #pragma omp threadprivate(_jz_jet, _jz_save, _jz_oneOverN, _jz_theNs, _jz_tvar1, _jz_tvar2, _jz_tvar3, _jz_tvar4, _jz_uvar1, _jz_uvar2, _jz_svar1, _jz_svar2, _jz_svar3, _jz_svar4, _jz_svar5, _jz_wvar3, _jz_wvar4, _jz_zvar1, _jz_zvar2, _jz_MyFloatZERO, _kthtmp)\n");
  
  if (total_jet_vars>0) {
  /* series */
    fprintf(outfile, "/* series bgn */\n");
    fprintf(outfile, "    static MY_JET       *_sjz_jet[%d], *_sjz_save = NULL;\n", total_jet_vars);
    fprintf(outfile, "    static MY_JET     _sjz_tvar1, _sjz_tvar2, _sjz_tvar3, _sjz_tvar4; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _sjz_uvar1, _sjz_uvar2; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _sjz_svar1, _sjz_svar2, _sjz_svar3, _sjz_svar4, _sjz_svar5; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _sjz_wvar3, _sjz_wvar4; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _sjz_zvar1, _sjz_zvar2; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _kthStmp; /* tmp vars */\n");
    fprintf(outfile, "    #pragma omp threadprivate(_sjz_jet, _sjz_save, _sjz_tvar1, _sjz_tvar2, _sjz_tvar3, _sjz_tvar4, _sjz_uvar1, _sjz_uvar2, _sjz_svar1, _sjz_svar2, _sjz_svar3, _sjz_svar4, _sjz_svar5,  _sjz_wvar3, _sjz_wvar4, _sjz_zvar1, _sjz_zvar2, _kthStmp)\n");    
    fprintf(outfile, "/* series end */\n");
  }

  fprintf(outfile, "    static int          _jz_maxOrderUsed  = -1;\n");
  fprintf(outfile, "    static int          _jz_lastOrder = 0, _jz_initialized=0, _jz_ginitialized=0;\n");
  fprintf(outfile, "    int                 _jz_i, _jz_j, _jz_k, _jz_l, _jz_m, _jz_n, _jz_oorder ;\n");
  fprintf(outfile, "    #pragma omp threadprivate(_jz_maxOrderUsed,_jz_lastOrder, _jz_initialized, _jz_ginitialized)\n");  
  
  fprintf(outfile, "    /* allocating memory if needed */\n");
  fprintf(outfile,"    if (_jz_maxOrderUsed < order )  { \n");
  fprintf(outfile, "    \t if (_jz_ginitialized == 0) { \n");
  fprintf(outfile, "    \t   InitMyFloat(_jz_tvar1); InitMyFloat(_jz_tvar2);InitMyFloat(_jz_tvar3);InitMyFloat(_jz_tvar4);\n");
  fprintf(outfile, "    \t   InitMyFloat(_jz_svar1); InitMyFloat(_jz_svar2);InitMyFloat(_jz_svar3);InitMyFloat(_jz_svar4);\n");
  fprintf(outfile, "    \t   InitMyFloat(_jz_svar5); InitMyFloat(_jz_zvar1);InitMyFloat(_jz_zvar2);\n");
  fprintf(outfile, "    \t   InitMyFloat(_jz_uvar1); InitMyFloat(_jz_uvar2);\n");
  fprintf(outfile, "    \t   InitMyFloat(_jz_wvar3);InitMyFloat(_jz_wvar4);\n");
  fprintf(outfile, "    \t   InitMyFloat(_jz_MyFloatZERO); InitMyFloat(_kthtmp);\n");
  fprintf(outfile, "    \t   MakeMyFloatC(_jz_MyFloatZERO, \"0\", (double)0);\n");  
  if (constcountFinal + cgoffset> 0) {
    fprintf(outfile, "    \t   for(_jz_i=0; _jz_i<%d; _jz_i++) {\n", constcountFinal+cgoffset);
    fprintf(outfile, "    \t       InitMyFloat(_jz_cvars[_jz_i]);\n");
    fprintf(outfile, "    \t   }\n");
  }
  if (total_jet_vars>0) {
    /* series */
    fprintf(outfile, "    \t   /* series bgn */\n");
    fprintf(outfile, "    \t   InitJet(_sjz_tvar1); InitJet(_sjz_tvar2);InitJet(_sjz_tvar3);InitJet(_sjz_tvar4);\n");
    fprintf(outfile, "    \t   InitJet(_sjz_svar1); InitJet(_sjz_svar2);InitJet(_sjz_svar3);InitJet(_sjz_svar4);\n");
    fprintf(outfile, "    \t   InitJet(_sjz_svar5); InitJet(_sjz_zvar1);InitJet(_sjz_zvar2);\n");
    fprintf(outfile, "    \t   InitJet(_sjz_uvar1); InitJet(_sjz_uvar2);\n");
    fprintf(outfile, "    \t   InitJet(_sjz_wvar3); InitJet(_sjz_wvar4);\n");
    fprintf(outfile, "    \t   InitJet(_kthStmp);\n");
    fprintf(outfile, "    \t   /* series end */\n");
  }

  fprintf(outfile, "    \t }\n");
  fprintf(outfile, "    \t if (rflag > 0) rflag = 0; /* have to recompute everything */\n");
  fprintf(outfile, "    \t _jz_oorder=_jz_maxOrderUsed;\n");
  fprintf(outfile, "    \t _jz_maxOrderUsed  = order;\n");
  fprintf(outfile, "    \t if (_jz_ginitialized) { \n");
  fprintf(outfile, "    \t   for(_jz_i=0; _jz_i< _jz_oorder+1; _jz_i++) {ClearMyFloat(_jz_oneOverN[_jz_i]); ClearMyFloat(_jz_theNs[_jz_i]);}\n");
  fprintf(outfile, "    \t   free(_jz_oneOverN); free(_jz_theNs);\n");
  fprintf(outfile, "    \t }\n");
  fprintf(outfile, "    \t _jz_theNs = (MY_FLOAT *)malloc((order+1) * sizeof(MY_FLOAT));\n");  
  fprintf(outfile, "    \t _jz_oneOverN = (MY_FLOAT *)malloc((order+1) * sizeof(MY_FLOAT));\n");  
  fprintf(outfile, "    \t for(_jz_i=0; _jz_i<order+1; _jz_i++) {InitMyFloat(_jz_oneOverN[_jz_i]);InitMyFloat(_jz_theNs[_jz_i]);}\n");
  fprintf(outfile, "    \t MakeMyFloatC(_jz_theNs[0],\"0.0\", (double)0.0);\n");
  fprintf(outfile, "    \t MakeMyFloatC(_jz_uvar1,\"1.0\", (double)1.0);\n");
  fprintf(outfile, "    \t for(_jz_i = 1; _jz_i <= order; _jz_i++) {\n");
  fprintf(outfile, "    \t\t AssignMyFloat(_jz_tvar2, _jz_theNs[_jz_i-1]);\n");
  fprintf(outfile, "    \t\t AddMyFloatA(_jz_theNs[_jz_i], _jz_tvar2, _jz_uvar1);\n\t}\n");
  fprintf(outfile, "    \t AssignMyFloat(_jz_oneOverN[0],_jz_uvar1);\n");
  fprintf(outfile, "    \t AssignMyFloat(_jz_oneOverN[1],_jz_uvar1);\n");
  fprintf(outfile, "    \t for(_jz_i = 2; _jz_i <= order; _jz_i++) {\n");
  fprintf(outfile, "    \t\t DivideMyFloatA(_jz_oneOverN[_jz_i], _jz_uvar1,_jz_theNs[_jz_i]);\n\t}\n");
  tmpcnt=varcountFinal+goffset;
  fprintf(outfile, "    \t if (_jz_ginitialized) {\n");
  fprintf(outfile, "    \t    for(_jz_i=0; _jz_i<(_jz_oorder+1)*(%d); _jz_i++) { ClearMyFloat(_jz_save[_jz_i]);} free(_jz_save);\n",tmpcnt);
  /* series */
  if (total_jet_vars>0) {
    fprintf(outfile, "\t  /* series bgn */\n");
    fprintf(outfile, "    \t    for(_jz_i=0; _jz_i<(_jz_oorder+1)*(%d); _jz_i++) { ClearJet(_sjz_save[_jz_i]);} free(_sjz_save);\n",total_jet_vars);
    fprintf(outfile, "\t  /* series end */\n");
  }
  
  fprintf(outfile, "    \t }\n");

  fprintf(outfile, "    \t _jz_save = (MY_FLOAT *)malloc((order+1)* %d *sizeof(MY_FLOAT));\n",  tmpcnt);
  fprintf(outfile, "    \t for(_jz_i=0; _jz_i<(order+1)*(%d); _jz_i++) { InitMyFloat(_jz_save[_jz_i]);}\n",tmpcnt);
  fprintf(outfile, "    \t for(_jz_j = 0, _jz_k = 0; _jz_j < %d ;  _jz_j++, _jz_k += order+1) { _jz_jet[_jz_j] =& (_jz_save[_jz_k]); }\n",tmpcnt);
  /* series */  
  if (total_jet_vars>0) {
    fprintf(outfile, "    \t /*s eries bgn */\n");
    fprintf(outfile, "    \t _sjz_save = (MY_JET *)malloc((order+1)* %d *sizeof(MY_JET));\n",  total_jet_vars);
    fprintf(outfile, "    \t for(_jz_i=0; _jz_i<(order+1)*(%d); _jz_i++) { InitJet(_sjz_save[_jz_i]);}\n",total_jet_vars);
    fprintf(outfile, "    \t for(_jz_j = 0, _jz_k = 0; _jz_j < %d ;  _jz_j++, _jz_k += order+1) { _sjz_jet[_jz_j] =& (_sjz_save[_jz_k]); }\n",total_jet_vars);
    fprintf(outfile, "    \t /* series end */\n");
  }

  if (nonautonomous)
    {
      fprintf(outfile, "    \t /* nonautonomous, derivatves of time */\n");
      fprintf(outfile, "    \t MakeMyFloatA(_jz_jet[%d][1], (double)1.0);\n", neqns-1);  
      fprintf(outfile, "    \t for(_jz_i = 2; _jz_i <= order; _jz_i++) { MakeMyFloatA(_jz_jet[%d][_jz_i], (double)0.0);}\n", neqns-1);  
    }
  fprintf(outfile, "\n    \t /* True constants, initialized only once. */\n");
  for(i=neqns; i < allVarsCount; i++)
    {
      Node var = allVarsList[i];
      int  issum = IS_INTERNAL_SUM(var);
      if (issum == 0 && nodeIsNumber(var))
        if (cdouble == 0) outputInitialVar(var);
    }
  fprintf(outfile, "    }\n\n");
          
  fprintf(outfile, "    if (rflag) {\n");
  fprintf(outfile, "    \t if (rflag < 0 ) return(NULL);\n");
  if (nonautonomous)  fprintf(outfile, "    \t if (MyFloatA_NEQ_B(t, _jz_jet[%d][0])) rflag = 0;\n", neqns-1);
  fprintf(outfile, "    \t for(_jz_i = 0; rflag != 0 && _jz_i < %d; _jz_i++) {\n", neqns - nonautonomous);
  fprintf(outfile, "    \t\t if (MyFloatA_NEQ_B(_jz_jet[_jz_i][0], x[_jz_i])) rflag = 0;\n    \t }\n    }\n");
  fprintf(outfile, "\n    if (rflag == 0) {\n");
  fprintf(outfile, "\t /* initialize all constant vars and state variables */\n");
  fprintf(outfile, "\t _jz_lastOrder = 1;\n");
  for(i = 0; i < neqns - nonautonomous; i++) {
    extern char *genGlobalVarLvalue(Node var);
      Node var  = varlistFinal[i];
      fprintf(outfile, "\t\t /* init state variable: */\n");
      if (VAR_IS_JET(var)) {
	char *xnam = genGlobalVarLvalue(var);
	fprintf(outfile, "\t AssignFloatToJet(%s, x[%d]);\n", xnam, i);
        if (VAR_IS_JET_PARAM(var)) {
	  int j = ID_PINDEX(var);
	  fprintf(outfile, "\t InitJetParameter(%s, %d); /* %s */\n", xnam, j-1, ID_GIVEN_NAME(var));		  
	}
      } 
      fprintf(outfile, "\t AssignMyFloat(_jz_jet[%d][0], x[%d]);\n", i, i);
  }
  if (nonautonomous) {
    fprintf(outfile,   "\t AssignMyFloat(_jz_jet[%d][0], t); /* nonautonomous, time */\n", neqns-1);
    i++;
  }

  /* series  */
  if (state_jet_vars>0) {
    fprintf(outfile, "/* series bgn */\n");
    fprintf(outfile,"\t if (sIn){\n");
    for(j=0; j<state_jet_vars; j++) {
      /*test*/
      Node var  = jetVarsList[j];
      int  sidx = ID_SINDEX(var);
      int  jidx = ID_JINDEX(var);
      int  vidx = ID_VINDEX(var);
      fprintf(outfile,"\t\t AssignJetToJet(_sjz_jet[%d][0], sIn[%d]);\n",jidx,sidx-1);
      fprintf(outfile,"\t\t AssignJetToFloat(_jz_jet[%d][0], sIn[%d]);\n",vidx,sidx-1);
    }
    fprintf(outfile,"\t }\n");      
    fprintf(outfile, "/* series end */\n");
  }
  
  for( ; i < allVarsCount; i++)
    {
      Node var = allVarsList[i];
      int  issum = IS_INTERNAL_SUM(var);
      if (issum)
	{
	  Node def = variables[ID_V_INDEX(var)].def;
	  SumP sum = NODE_SUM_BLOCK(def);
	  outputSumInitialVars(sum);
	  sumUptoVar(sum, var);
	}
      else if (nodeIsNumber(var) == 0) outputInitialVar(var);
    }
  fprintf(outfile, "\n\t /* the first derivative of state variables */\n");  
  for(j = 0; j < neqns - nonautonomous; j++) 
    {
      Node var  = varlistFinal[j];
      int  vidx = ID_V_INDEX(var);
      Node def  = variables[vidx].def; /* def is a variable */
      int  jidx  = ID_VINDEX(def) + ID_VOFFSET(def);
      fprintf(outfile, "\t /* state variable %d: */\n", j);
      if (VAR_IS_JET(var)) {
	char *xnam = genVarLvalueKth(def, "0");      
	char *ynam = genVarLvalueKth(var, "1"); 
        if (VAR_IS_JET(def)) {
	  fprintf(outfile, "\t AssignJetToJet(%s, %s);\n", ynam, xnam);
	} else {
	  fprintf(outfile, "\t AssignFloatToJet(%s, %s);\n", ynam, xnam);	  
	}
	fprintf(outfile, "\t AssignJetToFloat(_jz_jet[%d][1], %s);\n",j, ynam);
      } else {
	fprintf(outfile, "\t AssignMyFloat(_jz_jet[%d][1], _jz_jet[%d][0]);\n",j, jidx);
      }
    }
  if (nonautonomous)
    fprintf(outfile,   "\t /* nonautonomous, time'=1 has been initialized already */\n");

  fprintf(outfile, "\t}\n");
  
  fprintf(outfile, "\n\t /* compute the kth order derivatives of all vars */\n");
  fprintf(outfile, "\t for(_jz_k = _jz_lastOrder; _jz_k < order; _jz_k++) {\n");
  fprintf(outfile, "\t\t /* derivative for tmp variables */\n");

  for(j = neqns; j < varcountFinal; j++) 
    {
      Node var  = varlistFinal[j];
      int  issum = IS_INTERNAL_SUM(var);
      if (issum)
	{
	  Node def = variables[ID_V_INDEX(var)].def;
	  SumP sum = NODE_SUM_BLOCK(def);
	  outputSumKthDerivative(sum);
	  sumUptoVarKth(sum, var);
	}
      else outputVarKthDerivative(var);
    }

  fprintf(outfile, "\t\t /* derivative of state variables */\n");
  fprintf(outfile, "\t\t _jz_m = _jz_k+1;\n");
  for(j = 0; j < neqns - nonautonomous; j++) 
    {
      Node var  = varlistFinal[j];
      int  vidx = ID_V_INDEX(var);
      Node def  = variables[vidx].def; /* def is a variable */
      int  jidx  = ID_VINDEX(def) + ID_VOFFSET(def);

      fprintf(outfile, "\t\t /* state variable %d: */\n", j);

      if (VAR_IS_JET(var)) {
	char *xnam = genVarLvalueKth(var, "_jz_m");
	char *ynam = genVarLvalueKth(def, "_jz_k");
        if (VAR_IS_JET(def)) {
	  fprintf(outfile, "\t\t DivideJetFloatA(%s, %s, _jz_theNs[_jz_m]);\n",xnam,ynam);
	  fprintf(outfile, "\t\t AssignJetToFloat(_jz_jet[%d][_jz_m], %s);\n",j, xnam);
	} else { // happens only when the RHS is a constant
	  fprintf(outfile, "\t\t DivideMyFloatByInt(_jz_jet[%d][_jz_m], _jz_jet[%d][_jz_k], _jz_m);\n",j, jidx);
	  fprintf(outfile, "\t\t AssignFloatToJet(%s, _jz_jet[%d][_jz_m]);\n", xnam,j);	  
	}
      } else {
	/*fprintf(outfile, "\t\t MultiplyMyFloatA(_jz_jet[%d][_jz_m], _jz_jet[%d][_jz_k], _jz_oneOverN[_jz_m]);\n",j, jidx);*/
        if (my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
	  fprintf(outfile, "\t\t DivideMyFloatByInt(_jz_jet[%d][_jz_m], _jz_jet[%d][_jz_k], _jz_m);\n",j, jidx);
	} else {
	  fprintf(outfile, "\t\t DivideMyFloatA(_jz_jet[%d][_jz_m], _jz_jet[%d][_jz_k], _jz_theNs[_jz_m]);\n",j, jidx);
	}
      }
    }
  fprintf(outfile, "\t\t _jz_initialized=1;\n");
  fprintf(outfile, "\t }\n");
  fprintf(outfile, "    _jz_lastOrder = order;\n");
  fprintf(outfile, "    _jz_ginitialized=1;\n");
  fprintf(outfile, "    if (sOut) {\n");
  if (total_jet_vars) {
    fprintf(outfile, "     *sOut = _sjz_jet;\n");      
  } else {
    fprintf(outfile, "     *sOut = NULL;\n");
  }
  fprintf(outfile, "    }\n");
  fprintf(outfile, "    return(_jz_jet);\n}\n\n");
  /*  */

  fprintf(outfile, "MY_FLOAT **%sA(MY_FLOAT t, MY_FLOAT *x, int order, int rflag)\n{\n", outName);
  fprintf(outfile, "    return(%s_A(t,x,order,rflag, NULL, NULL));\n}\n\n", outName);

  fprintf(outfile, "MY_FLOAT **%s(MY_FLOAT t, MY_FLOAT *x, int order)\n{\n", outName);
  fprintf(outfile, "    return(%sA(t,x,order,0));\n}\n\n", outName);



  if(genJetHelper) {genJetHelpers(0);}

  if(debug)  {

  fprintf(outfile,"/******************** Translation Info *****************************/\n");
  fprintf(outfile, "/*\n\n"); /* */
  showVariables(outfile);
  fprintf(outfile, "===================================================================\n");
  fprintf(outfile, "=========                                                  ========\n");
  fprintf(outfile, "=========          Differential Equations                  ========\n");
  fprintf(outfile, "=========                                                  ========\n");
  fprintf(outfile, "===================================================================\n\n");
  for(i = 0; i < neqns; i++)
    fprintf(outfile,"\t %s'=%s\n", NODE_NAME(equations[i].name), NODE_NAME(equations[i].def));
  fprintf(outfile, "*/\n");  
  fprintf(outfile,"/*************** END  END  END ***************************************/\n");
  }

  
}
/**********************************************************************************/
void outputExtern(void)
{
  int i, jnk;
  for(i = 0; i < nexterns; i++)
    {
      Node var = externVars[i].name;
      Node idx = externVars[i].def;
      Node current=NULL, next;
      int  isint = ID_IS_INT(var);
      int  it = ID_INT_TYPE(var);
      char *ttt = NULL;

      if (isint) ttt = (it == 4 ? "int ": (it == 2 ? "short" : "char"));
      else ttt = "MY_FLOAT";
      fprintf(outfile, "    extern %s %s", ttt,  NODE_NAME(var));

      while(idx != NULL)
        {
          current = NODE_LEFT(idx);
          next = NODE_RIGHT(idx);
          if (current == NULL) fprintf(outfile, "[]");
          else fprintf(outfile, "[%d]", intNodeValue(current, &jnk));
          idx = next;
        }
      fprintf(outfile, ";\n");
    }
  fprintf(outfile, "\n");
}
/**********************************************************************************/
Node scaleTime(Node node, int *dflag)
{
  int vidx, code = NODE_CODE(node);
  Node left, right;
  if (code == ID_NODE)
    {
      if (node == timeVar)
	{
	  *dflag = 0;
	  return(int_one_node);
	}
      else if ((vidx = ID_V_INDEX(node)) > 0)
	{
	  node = variables[vidx].def;
	  code = NODE_CODE(node);
	}
      else return(NULL);
    }
  switch(code)
    {
    case MULT_EXPR:
      left = NODE_LEFT(node);
      right = NODE_RIGHT(node);
      if (left == timeVar)
	{
          if (NODE_IS_CST(right))
	    {
	      *dflag = 0;
	      return(right);
	    }
	}
      else if (right == timeVar)
	{
          if (NODE_IS_CST(left))
	    {
	      *dflag = 0;
	      return(left);
	    }
	}
      break;
    case DIV_EXPR:
      left = NODE_LEFT(node);
      right = NODE_RIGHT(node);
      if (left == timeVar)
	{
          if (NODE_IS_CST(right))
	    {
	      *dflag = 1;
	      return(right);
	    }
	}
      break;
    default:
      break;
    }
  return(NULL);
}

/**********************************************************************************
 *
 * It node is an expression like   b +- a, return +-a else return  NULL
 */
Node linearTime(Node node, int *dflag, int *mflag)
{
  if (node && NODE_CODE(node) == ID_NODE)
    {
      int vidx, code;
      Node left, right, def;

      *mflag = 0;
      if (node == timeVar)
	return(int_one_node);
      else if ((vidx = ID_V_INDEX(node)) > 0)
	{
	  def = variables[vidx].def;
	  code = NODE_CODE(def);
	  switch(code)
	    {
	    case MULT_EXPR:
	    case DIV_EXPR:
	      return(scaleTime(def, dflag));
	      break;
	    case PLUS_EXPR:
	      left = NODE_LEFT(def);
	      right = NODE_RIGHT(def);	  
              if (NODE_IS_CST(left))
		return(scaleTime(right, dflag));
              else if (NODE_IS_CST(right))
		return(scaleTime(left, dflag));
	      break;
	    case MINUS_EXPR:
	      left = NODE_LEFT(def);
	      right = NODE_RIGHT(def);	  
              if (NODE_IS_CST(left))
		{
		  *mflag = 1;
		  return(scaleTime(right, dflag));
		}
              else if (NODE_IS_CST(right))
		return(scaleTime(left, dflag));
	      break;
	    default:
	      break;
	    }
	}
    }
  return(NULL);
}
/*********************************************************************************
 *
 * return +-1  if   var is rational.
 * return +-8  if   var if float and equivalent to a rational.
 */
int isRational(Node var, Node *num, Node *den)
{
  if (var && nodeIsNumber(var) && NODE_CODE(var)== ID_NODE)
    {
      int  ans = 8;
      int  vidx = ID_V_INDEX(var);
      Node def = variables[vidx].def; 

      if ( NODE_IS_INT(var) ) ans = 1;
      if (def && NODE_CODE(def) == DIV_EXPR)
        {
	  *num = NODE_LEFT(def);
	  *den = NODE_RIGHT(def);
          if (ans != 1) /* num or den is float */
	    {
	      double fv; int iv;
	      fv = cstNodeValue(*num);
	      iv = (int)fv;
              if ((double)iv == fv)
		{
		  fv = cstNodeValue(*den);
		  iv = (int)fv;
                  if ((double)iv != fv) ans = 0;
		}
	      else ans = 0;
	    }
          return(ans);
        }
      else if (def && NODE_CODE(def) == NEGATE_EXPR)
        {
          Node left = NODE_LEFT(def);
          if (left && NODE_CODE(left) == ID_NODE)
            return(- isRational(left, num, den));
        }
      else if (def && ans == 1)
	{
	  *num = def;
	  *den = int_one_node;
	  return(ans);
	}
      else
	{
	  double fv; int iv;
	  *num = def;
          *den = int_one_node;
	  fv = cstNodeValue(*num);
	  iv = (int)fv;
          if ((double)iv != fv) return(0);
	  else return(ans);
	}	  
    }
  return(0);
}
/**********************************************************************************/
