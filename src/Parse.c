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

#define PARSE_C
#include "Header.h"
/********************************************************/
int       nvars     = 0;
int       varSpace  = 0;
Variable  variables = NULL;
Equation  equations = NULL;
ExternVar externVars = NULL;
Node      *jetVars = NULL;
int       njetVars = 0;
int       jetVarSpace = 0;
int       nexterns = 0;
int       externSpace = 0;
int       neqns     = 0;
int       eqnSpace   = 0;
int       nonautonomous = 0;
int       etype = 0;

Control   controlParams;
/********************************************************/
/* list of elementary functions currently handled */
static char *elemFunctions[] =  
{
  "sqrt",
  "sin",
  "cos",
  "exp",
  "log",
  "cosh",
  "sinh",
  "tan",
  "atan",
  NULL,
};
/********************************************************/
void initialize_vars(void)
{
  variables = (Variable)my_malloc(512 * sizeof(struct variable_));
  varSpace  =  512;
  equations = (Equation)my_malloc(32 * sizeof(struct variable_));
  eqnSpace = 32;
  externVars= (ExternVar)my_malloc(32 * sizeof(struct variable_));
  externSpace = 32;

  controlParams.ninitials = 0;
  controlParams.initials = (Node *) my_malloc(32 * sizeof(Node));
  controlParams.space = 32;

  initialize_nodes();
}
/********************************************************/
void define_one_variable(Node left, Node right) 
{
  int i;

  if(VAR_IS_JET(left)) 
    {
      char *ispar="";   if(VAR_IS_JET_PARAM(left)) { ispar = "(param)";}
      fprintf(stderr, "Fatal error: %s= %s; ", NODE_NAME(left), NODE_INFO(right));
      fprintf(stderr, "%s was declared as a JET%s, it cannot be used as a local variable.\n", NODE_NAME(left), ispar);	  
      exit(3);
    }

  for(i = 0; i < nvars; i++)
    {
      if(left == variables[i].name)
	{
	  canonicalTree(right);	  
	  fprintf(stderr, "Fatal error: %s= %s; ", NODE_NAME(left), NODE_INFO(right));
	  fprintf(stderr, " >>redefinition of '%s' is not allowed.<<\n", NODE_NAME(left));
	  fprintf(stderr, "Previous def was: %s=%s;\n", NODE_NAME(left),
		  NODE_INFO(variables[i].def));
	  exit(3);
	}
      else if(ID_IS_EXTERN(left))
        {
	  canonicalTree(right);	  
	  fprintf(stderr, " >>definition of external symbol '%s' is not allowed.<<\n", NODE_NAME(left));
	  exit(3);
        }
    }
   insert_one_variable(left, right);
}
void  insert_one_variable(Node left, Node right)
{
  if(nvars+1 >= varSpace)
    {
      variables = (Variable)my_realloc(variables, 2 * varSpace * 
				       sizeof(struct variable_));
      varSpace *= 2;
    }
  variables[nvars].name = left;
  variables[nvars].def =  right;
  ID_V_INDEX(left) = nvars;
  SET_ID_IS_DEFINED(left);
  canonicalTree(left);
  canonicalTree(right);
  if(VAR_IS_JET(right)) {SET_VAR_IS_JET(left);}
  nvars++;
}
/********************************************************/
void parse_one_equation(Node left, Node var, Node right) 
{
  int  i;

  if(var == NULL)
    {
      if(timeVar == NULL) timeVar = install_id("t");
      var = timeVar;
    }
  else { if(timeVar == NULL) timeVar = var;}
  
  if(neqns+1 >= eqnSpace)
    {
      equations = (Equation)my_realloc(equations, 2 * eqnSpace * 
				       sizeof(struct variable_));
      eqnSpace *= 2;
    }
  for(i = 0; i < neqns; i++)
    {
      if(left == equations[i].name)
	{
	  canonicalTree(right);	  
	  fprintf(stderr, "Fatal error: %s'= %s; ", NODE_NAME(left), NODE_INFO(right));
	  fprintf(stderr, " >>redefinition of '%s' is not allowed.<<\n", NODE_NAME(left));
	  fprintf(stderr, "Previous def was: %s'=%s;\n", NODE_NAME(left),
		  NODE_INFO(equations[i].def));
	  exit(3);
	}
    }
  if(var != timeVar)
    {
      canonicalTree(right);	  
      fprintf(stderr, "Fatal error: %s'= %s; ", NODE_NAME(left), NODE_INFO(right));      
      fprintf(stderr, "Only one TIME variable is allowed.\n");
      fprintf(stderr, "TimeVar was '%s' and is renamed as '%s'\n", NODE_NAME(timeVar), NODE_NAME(var));
      exit(3);
    }
  /* insert to eqn list */
  equations[neqns].name = left;
  equations[neqns].def  = right;
  canonicalTree(left);
  canonicalTree(right);
  SET_ID_IS_VAR(left);
  SET_VAR_IS_STATE_VAR(left); // to check for jet parameter consistency
  ID_EINDEX(left) = 1+neqns;  // remember which equation it is used 
  if(VAR_IS_JET(right)) {SET_VAR_IS_JET(left);}
  neqns++;
}
/********************************************************/
void identifyFunctions()
{
  Node   name = id_list;
  while(name)
    {
      int i = 0;
      char *ptr = ID_NAME(name);
      while(elemFunctions[i])
	{
	  char *qtr = elemFunctions[i];
	  if( *ptr == *qtr && !strcmp(qtr, ptr))
	    {
	      SET_ID_IS_FUNC(name);
	      break;
	    }
	  i++;
	}
      name = NODE_NEXT(name);
    }
}
/********************************************************/
int nodeIsConst(Node node)
{
  if(node)
    {
      int code = NODE_CODE(node);
      int t1, t2;

      if(NODE_IS_CST(node)) return(1);
      switch(code)
	{
	case ID_NODE:
	  if(ID_IS_EXTERN(node) != 0) { SET_NODE_IS_CST(node); return(1);}
          else if(ID_IS_VAR(node)) { return(ID_IS_CST(node));}
          else if(ID_IS_DEFINED(node) == 0) return(0);
          else
            {
              Node def = variables[ID_V_INDEX(node)].def;
              if(nodeIsConst(def))
                {
                  SET_NODE_IS_CST(node);
                  if(NODE_IS_INT(def)) { SET_NODE_IS_INT(node);}
                  return(1);
                }
              return(0);
            }
	  break;
	case INT_CST:
	case FLOAT_CST:
	  return(1);
	  break;
	case ARRAY_REF:
	  t1 = nodeIsConst(NODE_LEFT(node)); 
	  t2 = nodeIsConst(NODE_RIGHT(node)); 
          SET_NODE_IS_CST(node);
	  if(NODE_IS_INT(NODE_LEFT(node)))
	    { SET_NODE_IS_INT(node); }
          return(1);
          break;
	case ARRAY_IDX:
	  t1 = nodeIsConst(NODE_LEFT(node));
	  t2 = nodeIsConst(NODE_RIGHT(node));
          if(NODE_IS_INT(NODE_LEFT(node)) == 0)
            fprintf(stderr,"\n********Error: Array Index doesn't reduce to an integer**********\n\n");
          { SET_NODE_IS_CST(node); }
          { SET_NODE_IS_INT(node);}
          return(1);
	  break;	  
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
        case EXP_EXPR:
          /*case DIV_EXPR: */       /*case DIV_EXPR:*/  /*09102001 */
	  t1 = nodeIsConst(NODE_LEFT(node));
	  t2 = nodeIsConst(NODE_RIGHT(node));
	  if(t1 && t2)
	    {
	      SET_NODE_IS_CST(node);
              if(NODE_IS_INT(NODE_LEFT(node)) && NODE_IS_INT(NODE_RIGHT(node)))
                { SET_NODE_IS_INT(node);}
	      return(1);
	    }
	  break;
	case DIV_EXPR:
	  t1 = nodeIsConst(NODE_LEFT(node));
	  t2 = nodeIsConst(NODE_RIGHT(node));
	  if(t1 && t2) { 
            SET_NODE_IS_CST(node); 
            return(1);            
          }
	  break;
        case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
	  t1 = nodeIsConst(NODE_LEFT(node));
	  t2 = nodeIsConst(NODE_RIGHT(node));
          SET_NODE_IS_CST(node);
          SET_NODE_IS_INT(node);
          return(1);
	  break;
	case NEGATE_EXPR:
	  if(nodeIsConst(NODE_LEFT(node)))
	    {
	      SET_NODE_IS_CST(node);
              if(NODE_IS_INT(NODE_LEFT(node))) {SET_NODE_IS_INT(node);}
	      return(1);
	    }
	  break;
        case AND_EXPR:
	  t1 = nodeIsConst(NODE_LEFT(node));
	  t2 = nodeIsConst(NODE_RIGHT(node));          
          SET_NODE_IS_CST(node);
          SET_NODE_IS_INT(node);
          return(1);
	  break;
        case OR_EXPR:
	  t1 = nodeIsConst(NODE_LEFT(node));
	  t2 = nodeIsConst(NODE_RIGHT(node));          
          SET_NODE_IS_CST(node);
          SET_NODE_IS_INT(node);
          return(1);
          break;
	case CALL_EXPR:
	  if(nodeIsConst(NODE_RIGHT(node)))
	    {
	      SET_NODE_IS_CST(node);
	      return(1);
	    }
	  break;	  
        case IF_ELSE_EXPR:
          t1 = nodeIsConst(NODE_LEFT(node));
          t2 = nodeIsConst(NODE_RIGHT(node));
	  if(t1 && t2)
            {
	      SET_NODE_IS_CST(node);
              if(NODE_IS_INT(NODE_LEFT(node)) && NODE_IS_INT(NODE_RIGHT(node)))
                {SET_NODE_IS_INT(node);}
	      return(1);
	    }
          t1= nodeIsConst(IF_ELSE_COND(node));
          break;
	default:
	  break;
	}
    }
  return(0);
}

int nodeIsNumber(Node node)
{
  if(node)
    {
      int code = NODE_CODE(node);
      int t1, t2;

      switch(code)
	{
	case ID_NODE:
          if(NODE_IS_EXTERN(node) || ID_IS_VAR(node)) return(0);
	  else
            {
              if(NODE_IS_A_NUMBER(node)) return(1);
              else
                {
                  Node def = variables[ID_V_INDEX(node)].def;
                  if(nodeIsNumber(def))
                    {
                      SET_NODE_IS_A_NUMBER(node);
                      SET_NODE_IS_CST(node);
                      if(NODE_IS_INT(def)) { SET_NODE_IS_INT(node);}
                      return(1);
                    }
                  else return(0);
                }
            }
	  break;
	case INT_CST:
	case FLOAT_CST:
	  return(1);
	  break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case EXP_EXPR:
          /*case DIV_EXPR:*/  /*09102001 */
	  t1 = nodeIsNumber(NODE_LEFT(node));
	  t2 = nodeIsNumber(NODE_RIGHT(node));
	  if(t1 && t2)
	    {
              SET_NODE_IS_A_NUMBER(node);
	      SET_NODE_IS_CST(node);
              if(NODE_IS_INT(NODE_LEFT(node)) && NODE_IS_INT(NODE_RIGHT(node)))
                { SET_NODE_IS_INT(node);}
	      return(1);
	    }
	  break;
	case DIV_EXPR:
	  t1 = nodeIsNumber(NODE_LEFT(node));
	  t2 = nodeIsNumber(NODE_RIGHT(node));
	  if(t1 && t2)
	    {
              SET_NODE_IS_A_NUMBER(node);
	      SET_NODE_IS_CST(node);
	      return(1);
	    }
          break;
        case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
	  t1 = nodeIsNumber(NODE_LEFT(node));
	  t2 = nodeIsNumber(NODE_RIGHT(node));
	  if(t1 && t2)
	    {
              SET_NODE_IS_A_NUMBER(node);
	      SET_NODE_IS_CST(node);
              SET_NODE_IS_INT(node);
	      return(1);
	    }
	  break;
        case AND_EXPR: /* short circuit AND/OR: removed */
	  t1 = nodeIsNumber(NODE_LEFT(node));
	  t2 = nodeIsNumber(NODE_RIGHT(node));          
          if(t1 && t2)
            {
              SET_NODE_IS_A_NUMBER(node);
	      SET_NODE_IS_CST(node);
              SET_NODE_IS_INT(node);
	      return(1);
            }
	  break;
        case OR_EXPR:
	  t1 = nodeIsNumber(NODE_LEFT(node));
	  t2 = nodeIsNumber(NODE_RIGHT(node));          
          if(t1 && t2) 
            {
              SET_NODE_IS_A_NUMBER(node);
              SET_NODE_IS_CST(node);
              SET_NODE_IS_INT(node);
              return(1);
            }
	  break;          
	case NEGATE_EXPR:
	  if(nodeIsNumber(NODE_LEFT(node)))
	    {
              SET_NODE_IS_A_NUMBER(node);
	      SET_NODE_IS_CST(node);
              if(NODE_IS_INT(NODE_LEFT(node))) {SET_NODE_IS_INT(node);}
	      return(1);
	    }
	  break;
	case CALL_EXPR:
	  if(nodeIsNumber(NODE_RIGHT(node)))
	    {
              SET_NODE_IS_A_NUMBER(node);
	      SET_NODE_IS_CST(node);
	      return(1);
	    }
	  break;   
        case IF_ELSE_EXPR:
	  t1 = nodeIsNumber(NODE_LEFT(node));
	  t2 = nodeIsNumber(NODE_RIGHT(node));          
          if(t1 && t2 &&  nodeIsNumber(IF_ELSE_COND(node)))
            {
              SET_NODE_IS_A_NUMBER(node);
              SET_NODE_IS_CST(node);
              SET_NODE_IS_INT(node);
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
void identifyConstants()
{
  int    i;
  int    done = 0;
  Node   name, def;

  /* get all defined constants */
  while( ! done)
    {
      done = 1;
      for(i = 0; i < nvars; i++)
	{
	  name = variables[i].name;
	  if( ID_IS_VAR(name) == 0 && ID_IS_CST(name) == 0)
	    {
	      def  = variables[i].def;
	      if(nodeIsConst(def))
		{
		  SET_NODE_IS_CST(def);
		  SET_ID_IS_CST(name);
		  done = 0;
		  if(NODE_IS_INT(def))
		    {
		      SET_ID_IS_INT(name);
		    }
		}
	    }
	}
    }
}

/********************************************************************
 *
 * Find name in def. To make sure no vars are recursively defined.
 */
int findName(Node def, Node name)
{
  if(def)
    {
      int code, i;
      if(def == name) return(1);
      if(findName(NODE_LEFT(def), name)) return(1);
      if(findName(NODE_RIGHT(def), name)) return(1);
      code = NODE_CODE(def);
      if(code == IF_ELSE_EXPR && findName(IF_ELSE_COND(def), name)) return(1);
      if(code == SUM_EXPR)
	{
	  SumP sum = (SumP)NODE_SUM_BLOCK(def);
	  Node expr = SUMB_EXPR(sum);
	  if(findName(expr, name)) return(1);	  
	}
      for(i = 0; i < nvars; i++)
	{
	  if(def == variables[i].name)
	    return(findName(variables[i].def, name));
	}
    }
  return(0);
}
/**********************************************************************
 *
 * Verify that variables are consistently defined.
 */
void checkVars()
{
  int i;
  for(i = 0; i < nvars; i++)
    {
      Node name = variables[i].name;
      Node def  = variables[i].def;
      if(findName(def, name))
	{
	  fprintf(stderr, "Fatal error: '%s' is used circularly in var definitions!\n",
		  NODE_NAME(name));
	  exit(1);
	}
      if(ID_IS_FUNC(name))
	{
	  fprintf(stderr, "Fatal error: symbol '%s' is used as a variable!\n",
		  NODE_NAME(name));
	  exit(1);
	}
    }  
  for(i = 0; i < neqns; i++)
    {
      Node name = equations[i].name;
      if(ID_IS_FUNC(name))
	{
	  fprintf(stderr, "Fatal error: symbol '%s' is used as a variable!\n",
		  NODE_NAME(name));
	  exit(1);
	}
      else if(ID_IS_EXTERN(name))
        {
	  fprintf(stderr, "Fatal error: external symbol '%s' is used as a variable!\n",
		  NODE_NAME(name));
	  exit(1);
        }
    }  
  for(i = 0; i < nvars; i++)  
    {
      Node name = variables[i].name;
      if(ID_IS_VAR(name))
	{
	  int j;
	  fprintf(stderr, "Fatal error: '%s' is used inconsistantly.\n", NODE_NAME(name));
	  fprintf(stderr, "===> %s=%s;\n", NODE_NAME(name), NODE_INFO(variables[i].def));	  
	  for(j = 0; j < neqns; j++)
	    {
	      if(name == equations[j].name)
		{
		  fprintf(stderr, "===> %s'=%s;\n", NODE_NAME(name), NODE_INFO(equations[j].def));
		  break;
		}
	    }
	  exit(1);
	}
    }
}
/********************************************************
 *
 *  make sure all vars appeared in 'def' are defined.
 */
void checkAllDefined(Node def, Node *badvar)
{
  if(def && *badvar == NULL)
    {
      int code = NODE_CODE(def);
      switch(code)
	{
	case ID_NODE:
	  if(ID_IS_EXTERN(def) == 0 && ID_IS_VAR(def) == 0 && ID_IS_DEFINED(def) == 0)
	    { *badvar = def;}
	  else 
	    {
	      int  idx = ID_V_INDEX(def);
	      Node ndef = variables[idx].def;
	      checkAllDefined(ndef,badvar);
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
	  checkAllDefined(NODE_LEFT(def), badvar);
	  if(*badvar == NULL) checkAllDefined(NODE_RIGHT(def), badvar);
	  break;
	case NEGATE_EXPR:
	  checkAllDefined(NODE_LEFT(def), badvar);
	  break;
	case CALL_EXPR:
	  checkAllDefined(NODE_RIGHT(def), badvar);
	  break;
        case IF_ELSE_EXPR:
	  checkAllDefined(NODE_LEFT(def), badvar);
	  if(*badvar == NULL) checkAllDefined(NODE_RIGHT(def), badvar);
          if(*badvar == NULL) checkAllDefined(IF_ELSE_COND(def), badvar);
          break;
	case SUM_EXPR:
	  {
	    SumP sum = NODE_SUM_BLOCK(def);
	    Node nexpr = SUMB_EXPR(sum);
	    checkAllDefined(nexpr, badvar);	    
	  }
	break;
	default:
	  break;
	}
    }
}
/**********************************************************************
 *
 * make sure that all vars appeared in the RHS of ODEs are all defined.
 */
void checkEquations()
{
  int  i;
  for(i = 0; i < neqns; i++)
    {
      Node def = equations[i].def;
      Node badvar = NULL;
      checkAllDefined(def, &badvar);
      if(badvar)
	{
	  if(badvar == timeVar) /* nonautonomous system, add t'=1 */
	    {
	      if(!nonautonomous) parse_one_equation(timeVar, timeVar, int_one_node);
	      nonautonomous = 1;
	    }
	}
      if(badvar && nonautonomous)
	{
	  badvar = NULL;
	  checkAllDefined(def, &badvar);      
	}
      if(badvar)
	{
	  fprintf(stderr, "Fatal Error: '%s' is undefined in\n", ID_NAME(badvar));
	  fprintf(stderr, "             %s'=%s\n",
		  NODE_NAME(equations[i].name), NODE_INFO(equations[i].def));
	  exit(2);
	}
    }

  
  /* 20220519 */  
  /* check if the order of jetVars matches the order of equations */
  {
    int order_does_not_match = 0, nvars= neqns>njetVars?njetVars:neqns;
    for(i=0; i< nvars; i++) {
      if(strcmp(NODE_NAME(jetVars[i]),NODE_NAME(equations[i].name))) {
	order_does_not_match++;
      }
    }
    if(order_does_not_match) {
      fprintf(stderr, "\nThe order of jetvars does not match the order of ODEs.\nPlease reorder your ODEs in the input file so they match the declared jetvars.\n");
      fprintf(stderr, "\nODEs seen:\n");
      for(i = 0; i < neqns; i++) {
	fprintf(stderr, "%s'= ... \n", NODE_NAME(equations[i].name));
      } 
      fprintf(stderr, "\nJetVars declared:\n");     
      for(i = 0; i < njetVars; i++) {
	fprintf(stderr, "%s\n",  NODE_NAME(jetVars[i])); 
      }
      exit(3);
    }
  }
  /* 20220519 */

  /* 20220523 */
  {
    if(njetVars != 0 && (num_symbols == 0 ||  deg_jet_vars == 0)) {
      fprintf(stderr, "\nThe 'Jet declaration' line in your input declaried %d varirables, each consists of %d symbols of %d degree, ",
	      njetVars, num_symbols, deg_jet_vars);
      fprintf(stderr, "which effectively make them non-jet variables.\nPlease either comment out the 'Jet declaration' line, or declare some nontrivial jet vars.\n\n");
      exit(4);
    }
  }
  /* 20220523 */    
  
}
/********************************************************/
static int   tvarCounter=0;
static char  tvarName[16];

Node getTmpVar()  /* return a tmp variable */
{
  Node         nid;

  sprintf(tvarName, "v_%03d", tvarCounter++);
  nid = install_id(tvarName);
  return(nid);
}

/********************************************************
 *  decompose expression tree to 3-code expression trees
 */
void simplify(Node *nodePtr)
{
  Node node = *nodePtr;
  Node nvar, ndef;
  if(node)
    {
      int code = NODE_CODE(node);

      switch(code)
	{
	case ID_NODE:
	case INT_CST:
	case FLOAT_CST:
	  break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	case MULT_EXPR:
	case DIV_EXPR:
	  simplify( &(NODE_LEFT(node)));
	  simplify( &(NODE_RIGHT(node)));
	  nvar = getTmpVar();
	  ndef = build_op(code, NODE_LEFT(node),  NODE_RIGHT(node));
	  insert_one_variable(nvar, ndef);
	  delete_node( *nodePtr);
	  *nodePtr = nvar;
	  break;
	case EXP_EXPR:
	  {
	    Node right = NODE_RIGHT(node);
	    int expanded = 0;
            int is_a_number = nodeIsNumber(right);
            int is_int = NODE_IS_INT(right);
	    if(is_a_number && is_int) /*_+_NODE_CODE(right) == ID_NODE && ID_IS_INT(right) && NODE_IS_A_NUMBER(right))*/
	      {
		/*
		 * if expandpower > 0
		 * rewrite x^n as products to avoid taking log in computing derivatives 
		 */
                int  value, avalue, mbits, j, junk=-1;
		Node  left, nn, n[100];
		simplify( &(NODE_LEFT(node)));
		left = NODE_LEFT(node);
                /*_+_
		j = ID_V_INDEX(right);
		df = variables[j].def;
		value = atoi(NODE_NAME(df));
                */
                value = intNodeValue(right, &junk);
		avalue = value >= 0? value: -value;
		if(avalue <= expandpower)
		  {
		    expanded = 1;

		    mbits = 0; j = avalue; while(j){ mbits++; j = j>>1;} 
		    n[0] = left;
		    for(j = 1; j < mbits; j++) 
		      {
			nvar = getTmpVar();
			ndef = build_op(MULT_EXPR, n[j-1], n[j-1]);
			insert_one_variable(nvar, ndef);
			n[j] = nvar;
		      }
		    nn = NULL;
		    for(j = 0; j < mbits; j++)
		      {
			if( avalue & (1 << j)) 
			  {
			    if(nn == NULL) nn = n[j];
			    else
			      {
				nvar = getTmpVar(); 
				ndef = build_op(MULT_EXPR, nn, n[j]);
				insert_one_variable(nvar, ndef);
				nn = nvar;
			      }
			  }
		      }
		    if(nn) { 
                      if(value > 0) { *nodePtr = nn;}
                      /*_+_ */
                      else {
                        nvar = getTmpVar(); 
                        ndef = build_op(DIV_EXPR, int_one_node, nn);
                        insert_one_variable(nvar, ndef);
                        *nodePtr = nvar;
                      }
                      /*_+_ */
                    } else { *nodePtr = int_one_node; /* x^0 = 1 */}
		  }
	      }
	    if(expanded == 0)
	      {
		simplify( &(NODE_LEFT(node)));
		simplify( &(NODE_RIGHT(node)));
		nvar = getTmpVar();
		ndef = build_op(code, NODE_LEFT(node),  NODE_RIGHT(node));
		insert_one_variable(nvar, ndef);
		delete_node( *nodePtr);
		*nodePtr = nvar;
	      }
	  }
	  break;
	case NEGATE_EXPR:
	  simplify( &(NODE_LEFT(node)));
	  nvar = getTmpVar();
	  ndef = build_op(code, NODE_LEFT(node),  NULL);	  
	  insert_one_variable(nvar, ndef);
	  delete_node( *nodePtr);
	  *nodePtr = nvar;
	  break;
	case CALL_EXPR:
	  simplify( &(NODE_RIGHT(node)));
	  nvar = getTmpVar();
	  ndef = build_op(code, NODE_LEFT(node), NODE_RIGHT(node));	  
	  insert_one_variable(nvar, ndef);
	  delete_node( *nodePtr);
	  *nodePtr = nvar;
	  break;
        case GE_EXPR:
        case GT_EXPR:
        case LE_EXPR:
        case LT_EXPR:
        case EQ_EXPR:
        case NEQ_EXPR:
        case AND_EXPR:
        case OR_EXPR:
	  simplify( &(NODE_LEFT(node)));
	  simplify( &(NODE_RIGHT(node)));
	  nvar = getTmpVar();
	  ndef = build_bop(code, NODE_LEFT(node),  NODE_RIGHT(node));
	  insert_one_variable(nvar, ndef);
	  delete_node( *nodePtr);
	  *nodePtr = nvar;
	  break;
        case IF_ELSE_EXPR:
	  nvar = getTmpVar();
          simplify( &(IF_ELSE_COND(node)));
          simplify( &(NODE_LEFT(node)));
          simplify( &(NODE_RIGHT(node)));
          ndef = build_if_else(IF_ELSE_COND(node),NODE_LEFT(node), NODE_RIGHT(node));
          insert_one_variable(nvar, ndef);
          delete_node( *nodePtr);
          *nodePtr = nvar;
          break;
	case ARRAY_REF: /* left is array name */;
	  {
	    simplify( &(NODE_RIGHT(node)));
	  }
	  break;
	case ARRAY_IDX:
	  simplify( &(NODE_LEFT(node)));
	  simplify( &(NODE_RIGHT(node)));
	  break;
	case SUM_EXPR:
	  {
	    SumP sum = NODE_SUM_BLOCK(node);
	    Node idx =  SUMB_IDX(sum);
	    Node expr = SUMB_EXPR(sum);
	    simplify( &(expr));	    
	    SUMB_EXPR(sum) = expr;
	    mark_local(sum, expr, idx);
	    /* this is needed when summands is a cst */
	    if(NODE_IS_LOCAL(expr) == 0)
	      {
		nvar = getTmpVar();
		NODE_FLAGS(nvar) = NODE_FLAGS(expr);
		insert_one_variable(nvar, expr);
		SUMB_EXPR(sum) = nvar;
		SET_NODE_IS_LOCAL(nvar);
		NODE_SUM_BLOCK(nvar) = sum;
		SET_KEEP_ME(nvar);
	      }
	    NODE_SUM_BLOCK(expr) = sum;
	  }
	break;
	default:
	  break;
	}
    }      
}
/********************************************************/
void replaceVarWork(Node *node, Node ovar, Node nvar)
{
  if(*node)
    {
      if(NODE_BUDY(*node) == ovar) { NODE_BUDY(*node) = nvar;}
      if(*node == ovar) { *node = nvar;}
      else
	{
	  int code = NODE_CODE(*node);
	  replaceVarWork(&(NODE_LEFT(*node)), ovar, nvar);
	  replaceVarWork(&(NODE_RIGHT(*node)), ovar, nvar);
	  if(code == IF_ELSE_EXPR) replaceVarWork(&(IF_ELSE_COND(*node)), ovar, nvar);
	  if(code == SUM_EXPR)
	    {
	      SumP sum = NODE_SUM_BLOCK(*node);
	      Node *nnode = &(SUMB_EXPR(sum));
	      replaceVarWork(nnode, ovar, nvar);
	    }
	}
    }
}

void replaceVariables(int aa, int bb)
     /* replace v[aa] by v[bb] */
{
  int   i;
  Node  ovar = variables[aa].name;
  Node  nvar = variables[bb].name;
  for(i = 0; i < nvars; i++)
    {
      if(i != aa && i != bb && ID_IS_DELETED(variables[i].name) == 0)
	{
	  replaceVarWork(&(variables[i].def), ovar, nvar);
	}
    }
  for(i = 0; i < neqns; i++)
    {
      replaceVarWork(&(equations[i].def), ovar, nvar);
    }
}
/********************************************************/
void decompose()
{
  int i,  varcount = nvars;
  /* replace var names with tmp names */
  for(i = 0; i < neqns; i++)
    {
      Node name = equations[i].name;
      sprintf(tvarName, "v_%03d", tvarCounter++);
      NODE_NAME(name) = ReallocCopyString(NODE_NAME(name), tvarName);
    }
  for(i = 0; i < varcount; i++)
    {
      Node name = variables[i].name;
      if(ID_IS_EXTERN(name) == 0)
	{
	  sprintf(tvarName, "v_%03d", tvarCounter++);
	  NODE_NAME(name) = ReallocCopyString(NODE_NAME(name), tvarName);
	}
    }

  /* generate tenary codes and tmp vars */
  for(i = 0; i < varcount; i++)
    {
      Node nn = variables[i].def;  /* !!!!!!!!!!! */
      simplify(&nn);
      variables[i].def = nn;
    }
  
  for(i = 0; i < neqns; i++)
    {
      Node nn = equations[i].def; /* !!!!!!!!!!! */
      simplify(&(nn));
      equations[i].def = nn;
      SET_KEEP_ME(equations[i].def);
    }
  
  identifyConstants();  

  /* sin(h) and cos(h) need each other */
  makeCompanionVariables();

  for(i = 0; i < nvars; i++)
    {
      canonicalTree(variables[i].name);
      canonicalTree(variables[i].def);
    }  
  for(i = 0; i < neqns; i++)
    {
      canonicalTree(equations[i].name);
      canonicalTree(equations[i].def);
    }

  /* eliminate redundant variables */
  {
    int i, j, done;
    
    while(1)
      {
	done = 1;
	for(i = 0; i < nvars; i++)
	  {
            if(ID_IS_DELETED(variables[i].name) == 0 && KEEP_ME(variables[i].name) == 0)
              {
                for(j = 0; j < nvars; j++)
                  {
                    if(j != i &&  ID_IS_DELETED(variables[j].name) == 0 && KEEP_ME(variables[j].name) == 0)
                      {
                        Node def = variables[j].def;
                        if(def == variables[i].name ||
                           ( (NODE_INFO_HASH(variables[i].def) == NODE_INFO_HASH(variables[j].def)) &&
                             !strcmp(NODE_INFO(variables[i].def), NODE_INFO(variables[j].def))))
                          {
                            done = 0;
                            SET_ID_IS_DELETED(variables[j].name);
                            replaceVariables(j,i);
                          }
                      }
                  }
	      }
	  }
	if(!done)
	  {
	    for(i = 0; i < nvars; i++)
	      {
		if(ID_IS_DELETED(variables[i].name) == 0)
		  canonicalTree(variables[i].def);
	      } 
	    for(i = 0; i < neqns; i++)
	      {
		canonicalTree(equations[i].name);
		canonicalTree(equations[i].def);
	      }
	  }
	else break;
      }
  }

  /* mark all used expressions */
  for(i = 0; i < neqns; i++)
    {
      Node def = equations[i].def;
      markUsedExpressions(def);
    }

  /* mark all if_else vars */
  for(i = 0; i < nvars; i++)
    {
      Node name = variables[i].name;
      Node def = variables[i].def;
      if(NODE_CODE(def) == IF_ELSE_EXPR)
        {
          Node cond = IF_ELSE_COND(def);
          markCondVars(cond);
          IF_ELSE_COND(name) = cond;
        }
    }  

  /* rename constants to c_### */
  {
    int  changed = 0;
    for(i = 0; i < nvars; i++)
      {
	Node name = variables[i].name;
	Node def  = variables[i].def;
	if(ID_IS_CST(name) || ID_IS_CST(def))
	  {
	    SET_ID_IS_CST(name);
	    if(ID_IS_INT(name))
	      NODE_NAME(name)[0] = 'i';
	    else
	      NODE_NAME(name)[0] = 'c';
	    changed = 1;
	  }
      }
    /* make sure the RHS of ODEs are not constant variables */
    for(i = 0; i < neqns; i++)
      {
	Node def  = equations[i].def; /* def is a variable */
	if(nodeIsConst(def))
	  {
	    def->flags &= ~(IS_CST_BIT|IS_INT_BIT); 
	    NODE_NAME(def)[0] = 'v';
	    changed = 1;
	  }
      } 
    if(changed)
      {
	for(i = 0; i < nvars; i++)
	  {
	    canonicalTree(variables[i].name);
	    canonicalTree(variables[i].def);
	  }
      }    
  }
}
/*************************************************************************************/
int getControlParameterValue(char *str, double *value_return, Node *node_return)
{
  int i;
  *node_return = NULL;
  if(str == NULL) { return(0); }
  for(i = 0; i< nvars; i++) {
    Node name = variables[i].name;
    char *tptr = ID_GIVEN_NAME(name);    
    if(tptr && !strcmp(str, tptr)) 
      {
        Node def  = variables[i].def;
        if(! nodeIsConst(def)) {
          fprintf(stderr, "Control parameter '%s' does not reduce to a constant!\n", str);
          return(0);
        } else {
          *value_return = (double)cstNodeValue(def);
	  *node_return = def;
          if(NODE_IS_INT(def)) {return(1);} else {return(2);}
        }
      }
  }
  return(0);
}
/*************************************************************************************/
void showVars()
{
  int i;
  if(debug)
    {
      Node   name, def;
      char   tstr[128];
      fprintf(stderr,"========== variables ============\n");
      for(i = 0; i < nvars; i++)
        {
          name = variables[i].name;
          def =  variables[i].def;
          sprintf(tstr, "Var: %s=%s", NODE_INFO(name), NODE_INFO(def));	      
          fprintf(stderr, "%-56s %-14s %-10s %s %s\n",tstr,
                  ID_IS_DELETED(name) != 0 ? "(redundant)":
                  (ID_IS_CST(name) != 0? "(cvar)" : " "),
                  (NODE_IS_LOCAL(name)? "(local)" : " "),
		  (NODE_IS_A_NUMBER(name)? "(a number)" : " "),
		  (VAR_IS_JET(name)? " JET " : " "));
          if(NODE_CODE(def) == SUM_EXPR)
            {
              SumP sum = NODE_SUM_BLOCK(def);
              Node sexpr = SUMB_EXPR(sum);
              Node idx = SUMB_IDX(sum);
              int  from = SUMB_FROM(sum);
              int  to   = SUMB_TO(sum);
              fprintf(stderr, "\t SUM(%s, %s=%d, %d)\n",
                      NODE_INFO(sexpr), NODE_NAME(idx), from, to);
            }
        }
      fprintf(stderr,"\n");      
      for(i = 0; i < neqns; i++)
        {
          name = equations[i].name;
          def =  equations[i].def;
          fprintf(stderr, "Eqn: %s'=%s\n", NODE_INFO(name), NODE_INFO(def));
        }
    }
}
/*************************************************************************************/

void showPaserInfo()
{
  if(debug)
    {
      int i;
      fprintf(stderr, "=========== Parsed Source Info ============\n");
      for(i = 0; i < nvars; i++)
	{
	  Node name = variables[i].name;
	  Node def =  variables[i].def;
	  fprintf(stderr, "%s=%s\n", NODE_INFO(name), NODE_INFO(def));	      
	  if(NODE_CODE(def) == SUM_EXPR)
	    {
	      SumP sum = NODE_SUM_BLOCK(def);
	      Node expr = SUMB_EXPR(sum);
	      Node idx =  SUMB_IDX(sum);
	      canonicalTree(expr);
	      fprintf(stderr, "\t SUM( %s, %s=%d,%d)\n",NODE_INFO(expr), NODE_NAME(idx),
		      SUMB_FROM(sum), SUMB_TO(sum));
	    }
	}
      fprintf(stderr,"\n");      
      for(i = 0; i < neqns; i++)
	{
	  Node name = equations[i].name;
	  Node def =  equations[i].def;
	  fprintf(stderr, "%s'=%s\n", NODE_INFO(name), NODE_INFO(def));
	  if(NODE_CODE(def) == SUM_EXPR)
	    {
	      SumP sum = NODE_SUM_BLOCK(def);
	      Node expr = SUMB_EXPR(sum);
	      Node idx =  SUMB_IDX(sum);
	      canonicalTree(expr);
	      fprintf(stderr, "\t SUM( %s, %s=%d,%d)\n",NODE_INFO(expr), NODE_NAME(idx),
		      SUMB_FROM(sum), SUMB_TO(sum));
	    }
	}
      if(num_jet_vars>0) {
	fprintf(stderr, "\nJet ");
	for(i = 0; i < neqns; i++) 
	  {
	    Node name = equations[i].name;
	    if(VAR_IS_JET(name)) {
	      if(VAR_IS_JET_PARAM(name)) {
		fprintf(stderr, "%s(param) ", NODE_INFO(name));
	      } else {
		fprintf(stderr, "%s ", NODE_INFO(name));
	      }
	    }
	  }
	fprintf(stderr, " vars %d deg %d\n\n", num_jet_vars, deg_jet_vars);
      }
    }
}
/*************************************************************************************/

int intNodeValue(Node node, int *flag)
{
  if(node)
    {
      int code = NODE_CODE(node);
      Node left = NODE_LEFT(node);
      Node right = NODE_RIGHT(node);
      int l, r, value=0;
      
      switch(code)
	{
	case INT_CST:
	  value = atoi(NODE_NAME(node));
	  NODE_IVALUE(node) = value;
	  return(value);
	  break;
	case FLOAT_CST:
	  value = (int) atof(NODE_NAME(node));
	  NODE_IVALUE(node) = value;
	  /* fprintf(stderr, "Warning: '%s' used as an integer.\n", NODE_NAME(node));*/
	  return(value);
	  break;
	case ID_NODE:
          if(ID_IS_DEFINED(node) && ID_IS_VAR(node) == 0)
            {
              Node def = variables[ID_V_INDEX(node)].def;
              value = intNodeValue(def, flag);
              NODE_IVALUE(node) = value;
              NODE_IVALUE(def) = value;
              return(value);
            }
          else { *flag = -1; return(0);}
          break;
	case PLUS_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  value = l+r;
	  NODE_IVALUE(node) = value;
	  return(value);
	  break;
	case MINUS_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  value = l-r;
	  NODE_IVALUE(node) = value;
	  return(value);
	  break;
	case MULT_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  value = l*r;
	  NODE_IVALUE(node) = value;
	  return(value);
	  break;
	case DIV_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(r == 0) { fprintf(stderr, "intNodValue: division by 0\n"); *flag = -1;}
	  else value = l/r;
	  NODE_IVALUE(node) = value;
	  return(value);
	  break;
        case NEGATE_EXPR:
	  value = -intNodeValue(left, flag);
	  NODE_IVALUE(node) = value;
	  return(value);       
          break;
	case EXP_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag)
            {
              value = (r< 0)? 0 : 1; 
              while(r > 0) { value *= l; r--;}
            }
	  NODE_IVALUE(node) = value;
	  return(value);	  
	  break;
	case GE_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag) { value = (l >= r);}
	  NODE_IVALUE(node) = value;
	  return(value);	  
          break;
        case GT_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag) { value = (l > r);}
	  NODE_IVALUE(node) = value;
	  return(value);	  
          break;
        case LE_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag) { value = (l <= r);}
	  NODE_IVALUE(node) = value;
	  return(value);	  
          break;
        case LT_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag) { value = (l < r);}
	  NODE_IVALUE(node) = value;
	  return(value);	  
          break;
        case EQ_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag) { value = (l == r);}
	  NODE_IVALUE(node) = value;
	  return(value);	  
          break;
        case NEQ_EXPR:
	  l = intNodeValue(left, flag);
	  r = intNodeValue(right, flag);
	  if(! *flag) { value = (l != r);}
	  NODE_IVALUE(node) = value;
	  return(value);	  
          break;
        case AND_EXPR:
          {
            int tmp=0;
            l = intNodeValue(left, flag);
            r = intNodeValue(right, &tmp);
            if( ((! *flag) && l == 0) || ((!tmp) && r == 0))
              {
                NODE_IVALUE(node) = 0;
                return(0);	  
              }
            *flag |= tmp;
            if(! *flag) { value = (l && r);}
            NODE_IVALUE(node) = value;
            return(value);	  
          }
          break;
        case OR_EXPR:
          {
            int tmp = 0;
            l = intNodeValue(left, flag);
            r = intNodeValue(right, &tmp);
            if( ((! *flag) && l) || ((! tmp) && r))
              {
                NODE_IVALUE(node) = 1;
                return(1);
              }
            *flag |= tmp;
            if(! *flag) { value = (l || r);}
            NODE_IVALUE(node) = value;
            return(value);	  
          }
          break;
	case CALL_EXPR:
	  {
	    fprintf(stderr, "cannot convert %s(%s) to an integer.\n ", NODE_NAME(left),
		    NODE_INFO(right));
	    *flag = -1;
	  }	  
	break;
	default:
	  break;
	}
    }
  *flag = -1;
  return(0);
}
/*******************************************************************************/
Node extend_array_idx(Node cidx , Node idx)
{
  Node nidx = make_node(ARRAY_IDX);
  Node nnn = NULL;
  if(idx != NULL) nnn = copy_node(idx);

  NODE_LEFT(nidx) = nnn;

  if(cidx == NULL) return(nidx);
  else
    {
      Node t = cidx;
      while(NODE_RIGHT(t)) {t = NODE_RIGHT(t);}
      NODE_RIGHT(t) = nidx;
      return(cidx);
    }
}

Node declareArray(Node var, Node aidx)
{
  if(var)
    {
      int code = NODE_CODE(var);
      int evar;
      if(code != ID_NODE)
        {
          fprintf(stderr, "Internal Error in external array declaration, Var is not an ID node.\n");
          exit(5);
        }
      if(ID_IS_EXTERN(var) == 0)
        {
          fprintf(stderr, "Internal Error in external array declaration, '%s' is not an external var.\n",
                  NODE_NAME(var));
          exit(6);
        }
      ID_IS_ARRAY(var) += 1;
      evar = findExtern(var);
      if(evar >= 0)
        {
          Node nidx = extend_array_idx(externVars[evar].def , aidx);
          externVars[evar].def = nidx;
        }
      return(var);
    }
  return(NULL);
}

Node declareExternVar(Node var)
{
  if(var)
    {
      int evar;
      int code = NODE_CODE(var);
      if(code != ID_NODE)
        {
          fprintf(stderr, "Internal Error in external array declaration, Var is not an ID node.\n");
          exit(5);
        }
      if(ID_IS_DEFINED(var))
        {
          fprintf(stderr, "Error in EXTERN DECLAE: symbol '%s' is already used as a local variable.\n", NODE_NAME(var));
          exit(5);
        }
      SET_ID_IS_EXTERN(var);
      SET_ID_IS_CST(var);
      if(etype) {SET_ID_IS_INT(var);}
      ID_INT_TYPE(var) = etype;
      evar = findExtern(var);
      if(evar >= 0)
        {
          fprintf(stderr, "Error: redeclaration of '%s'\n",NODE_NAME(var));
          exit(5);
        }

      if(nexterns+1 >= externSpace)
        {
          externVars = (ExternVar)my_realloc(externVars, 2 * externSpace * 
                                             sizeof(struct variable_));
          externSpace *= 2;
        }
      externVars[nexterns].name = var;
      externVars[nexterns].def = NULL;
      nexterns++;
      return(var);
    }
  return(NULL);
}
/*******************************************************************************/
Node build_array_idx(Node cidx , Node idx)
{
  if(cidx == NULL)
    {
      Node nidx = make_node(ARRAY_IDX);
      NODE_LEFT(nidx) = idx;
      return(nidx);
    }
  else
    {
      Node t = cidx;
      while(NODE_RIGHT(t)) {t = NODE_RIGHT(t);}
      NODE_RIGHT(t) = idx;
      return(cidx);
    }
}

Node build_array_ref(Node var , Node idx)
{
  if(var && NODE_CODE(var) == ID_NODE)
    {
      if(ID_IS_EXTERN(var))
	{
	  int i=0, cnt = ID_IS_ARRAY(var);
	  Node tmp, nn = make_node(ARRAY_REF);  
	  NODE_LEFT(nn) = var;
	  NODE_RIGHT(nn) = idx;
          /* SET_NODE_IS_CST(nn); */
	  if(ID_IS_INT(var))  SET_NODE_IS_INT(nn); 
	  tmp = idx;
	  while(tmp) { i++; tmp = NODE_RIGHT(tmp);}
	  if(i != cnt)
	    fprintf(stderr, "Warning: Array dimension mismatch: '%s' was declared as %dD, referenced ad %dD\n",
		    NODE_NAME(var), cnt, i);
	  return(nn);
	}
      else fprintf(stderr, "Index to a local var '%s' is not allowed\n", NODE_NAME(var));
    }
  return(NULL);
}
/*******************************************************************************/
void check_array_ref(Node id)
{
  if(id && ID_IS_ARRAY(id))
    fprintf(stderr, "Warning: array '%s' is used as a scalar.\n",NODE_NAME(id));
}
/*******************************************************************************/
Node build_if_else(Node cond, Node ifs, Node els)
{
  if(cond && ifs && els)
    {
      Node nn = make_node(IF_ELSE_EXPR);
      IF_ELSE_COND(nn) = cond;
      SET_NODE_IS_INT(cond);
      NODE_LEFT(nn) = ifs;
      NODE_RIGHT(nn) = els;
      return(nn);
    }
  return(NULL);
}
/*******************************************************************************
 *
 * Mark all ID nodes that can be reached from the RHS of ODEs, minus the
 * condition boolean expr in IF_ELSE_EXPR. The puropse of this mark
 * is to separate expressions used only in IF_ELSE conditions. They
 * are constant variables (i.e, don't need to find their derivatives).
 */
void  markUsedExpressions(Node node)
{
  if(node && NODE_CODE(node)== ID_NODE && VAR_IS_USED(node) == 0)
    {
      int vidx = ID_V_INDEX(node);
      SET_VAR_IS_USED(node);
      if(vidx)
        {
          Node def = variables[vidx].def;
          Node left = NODE_LEFT(def);
          Node right = NODE_RIGHT(def);
          int  code = NODE_CODE(def);

          switch(code)
            {
            case ID_NODE: 
              markUsedExpressions(def); 
              break;
            case PLUS_EXPR:
            case MINUS_EXPR:
            case MULT_EXPR:
            case DIV_EXPR:
            case EXP_EXPR:
            case IF_ELSE_EXPR:
              markUsedExpressions(left); 
              markUsedExpressions(right); 
              break;
            case NEGATE_EXPR:
              markUsedExpressions(left); 
              break;
            case CALL_EXPR:
              markUsedExpressions(right); 
              break;
            case SUM_EXPR:
              {
                markUsedExpressions(right); 
              }
              break;
            }
        }
    }
}
/*******************************************************************************/
void markCondVars(Node node)
{
  if(node && NODE_CODE(node)== ID_NODE && VAR_IS_USED(node) == 0)
    {
      int vidx = ID_V_INDEX(node);
      SET_ID_IS_CST(node);
      if(vidx)
        {
          Node def = variables[vidx].def;
          Node left = NODE_LEFT(def);
          Node right = NODE_RIGHT(def);
          int  code = NODE_CODE(def);

          switch(code)
            {
            case ID_NODE: 
              if(ID_IS_CST(def) == 0) markCondVars(def); 
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
              markCondVars(left); 
              markCondVars(right); 
              break;
            case IF_ELSE_EXPR:
              markCondVars(left); 
              markCondVars(right); 
              right = IF_ELSE_COND(node);
              markCondVars(right); 
              break;
            case NEGATE_EXPR:
              markCondVars(left); 
              break;
            case CALL_EXPR:
              markCondVars(right); 
              break;
            case SUM_EXPR:
              {
                markCondVars(right); 
              }
              break;
            }
        }
    }
}
/*******************************************************************************/
int findExtern(Node var)
{
  if(var)
    {
      int i;
      for(i = 0; i < nexterns; i++)
        { if(externVars[i].name == var) return(i);}
    }
  return(-1);
}
/*******************************************************************************/
void addInitial(Node node)
{
  if(node)
    {
      if(controlParams.ninitials+1 >= controlParams.space)
        {
          controlParams.initials = (Node *)my_realloc(controlParams.initials,
                                                      2 * controlParams.space * sizeof(Node));
          controlParams.space *= 2;
        }
      controlParams.initials[controlParams.ninitials] = node;
      controlParams.ninitials += 1;
    }
}
/*******************************************************************************/
void  record_jet_var(Node var) {
  if(jetVarSpace == 0) {
    jetVars = (Node *)my_malloc(32 * sizeof(Node));
    jetVarSpace = 32;
  } else if(njetVars+1 >= jetVarSpace) {
    jetVars = (Node *)my_realloc(jetVars, 2 * jetVarSpace * sizeof(Node));
    jetVarSpace *= 2;
  }
  jetVars[njetVars++] = var;
}


Node markJet(Node var)
{
  if(var)
    {
      int code = NODE_CODE(var);
      if(code != ID_NODE)
        {
          fprintf(stderr, "Internal Error in jet declaration, Var is not an ID node.\n");
          exit(5);
        }
      if(ID_IS_DEFINED(var))
        {
          fprintf(stderr, "Error in JET declaration: symbol '%s' is already used as a local variable.\n", NODE_NAME(var));
          exit(5);
        }
      SET_VAR_IS_JET(var);
      SET_VAR_IS_DECLARED_JET(var);
      ID_SINDEX(var) = ++state_jet_vars;
      record_jet_var(var);
      return(var);
    }
  return(NULL);
}

void record_jet_initv(Node id, char *val) {
  int len = strlen(val);
  val[len-1]=0;
  NODE_JINITV(id)=val;
}

// 20220609 start
/*******************************************************************************/
int markAllVarsJet() {
  int i;
  for(i = 0; i < neqns; i++) {
    Node var = equations[i].name;
    if(var) {
      SET_VAR_IS_JET(var);
      SET_VAR_IS_DECLARED_JET(var);
      ID_SINDEX(var) = ++state_jet_vars;
      record_jet_var(var);      
    }
  }
  all_vars_are_jets=1;  
  num_jet_vars = neqns;
  return num_jet_vars;
}
// 20220609 end
/*******************************************************************************/

Node markJetParameter(Node var)
{
  if(var)
    {
      int code = NODE_CODE(var);

      if(code != ID_NODE)
        {
          fprintf(stderr, "Internal Error in jet parameter declaration, Var is not an ID node.\n");
          exit(5);
        }
      if(ID_IS_DEFINED(var))
        {
          fprintf(stderr, "Error in JET parameter declaration: symbol '%s' is already used as a local variable.\n", NODE_NAME(var));
          exit(5);
        }
      if(VAR_IS_JET(var)) 
	{
	  fprintf(stderr, "Error in JET parameter declaration: '%s' is already declared as a JET variable.\n", NODE_NAME(var));
          exit(5);
	}
      SET_VAR_IS_JET(var);
      SET_VAR_IS_JET_PARAM(var);
      ID_PINDEX(var) = ++param_jet_vars;
      record_jet_var(var);
      return(var);
    }
  return(NULL);
}

void check_jet_params() {
  if(num_jet_vars < param_jet_vars) {
    fprintf(stderr, "Number of variational variables (%d) CANNOT be less than the number of variational parameters(%d).\n", num_jet_vars, param_jet_vars);
    exit(6);
  }
}
