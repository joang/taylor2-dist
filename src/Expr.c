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
/*
 *  Translate Expressions to C functions.
 *    energy= 0.5*y*y - cos(x);
 *    expression my_function_name2 = energy, y^2+cos(x);
 *  For the above input, taylor will generate a C function
 *    void my_function_name2(MY_FLOAT *x_in, MY_FLOAT *out);
 *  which will evaluate expression energy and y^2+cos(x), and return the
 *  result to out[0] and out[1].  Caller is responsible for setting up out.
 *
 *  To minimally affect the normal taylor translation, expression 
 *  translation is done last, with needed functions duplicated
 *  in this file. Translation starts with a fresh parsing of the input.
 *
 */

#define EXPR_C
#include "Header.h"

/*************************************************************************/
int       num_expr_vars = 0;
int       exprVarLen = 0;
Node      *exprVars = NULL;

/*************************************************************************/

extern FILE *yyin;
extern int yyparse();

/*************************************************************************/
void reset_all_variables(); 
void checkEquationsAndExpressions();
void decomposeExpressions();
void genVariablesExpressions();
void outputExpressionValue(Node var);
void showDependExpression();
void outputInitialVarExpression(Node var);
void list_dependent_vars(Node name);
void decomposeExpressions();
void EmitExpressionCode();
void EmitExpressinJetCode();
/*************************************************************************/
static int expr_counter = 0;
void gen_info_for_Wrappers(void);
/*************************************************************************/
void genExpressionCode() {
  FILE *fp = NULL;

  if(saved_input_file == NULL) {
    fprintf(stderr, "Translate Expression: unable to translate when input is stdin. Please use an input file.\n");
    return;
  }
  if((fp =fopen(saved_input_file,"r")) == NULL) {
    fprintf(stderr, "Translate Expression: unable to open %s.\n", saved_input_file);
    return;
  }

  if(debug) fprintf(stderr, "\n\n/******* Translate Expressions ********/\n\n");

  reset_all_variables();
  initialize_vars();
  parser_pass = 1;  
  yyin = fp;
  yyparse();
  fclose(fp);
  showPaserInfo();
  identifyFunctions();
  checkVars();
  checkEquationsAndExpressions();
  decomposeExpressions();
  showVars();
  genVariablesExpressions();
  EmitExpressinJetCode();  
  EmitExpressionCode();
  gen_info_for_Wrappers();  
}
/*************************************************************************/

void reset_all_variables() {
  expr_counter = 0;
  tvarCounter = 0;
  insum = 0; 
  nvars     = 0;

  timeVar = NULL;
  destroy_all_expressions();  
  destroy_all_nodes();

  my_free(variables);     variables = NULL;
  my_free(equations);     equations = NULL;
  my_free(externVars);    externVars = NULL;     
  my_free(jetVars);       jetVars = NULL;
  my_free(jetVarsList);   jetVarsList = NULL;
  my_free(jetParamsList); jetParamsList = NULL;

  varSpace  = 0;
  njetVars = 0;
  jetVarSpace = 0;
  nexterns = 0;
  externSpace = 0;
  neqns     = 0;
  eqnSpace   = 0;
  nonautonomous = 0;
  etype = 0;

  my_free(varlistFinal);   varlistFinal = NULL;
  my_free(constlistFinal); constlistFinal = NULL; 
  my_free(ivarFinal);      ivarFinal = NULL;
  my_free(allVarsList);    allVarsList = NULL;
  my_free(jetVarsList);    jetVarsList = NULL;
  my_free(jetParamsList);  jetParamsList = NULL;  
  my_free(jmarks);         jmarks = NULL;
  my_free(kmarks);         kmarks = NULL;
  my_free(exprVars);       exprVars = NULL;
  allVarsCount = 0;
  ivarcountFinal = 0;    
  varcountFinal = 0;
  constcountFinal = 0;
  goffset = 0;
  igoffset = 0;
  cgoffset = 0;

  num_jet_vars=0;
  num_jet_symbols=0;
  deg_jet_vars=0;
  state_jet_vars=0;
  param_jet_vars=0;
  total_jet_vars=0;
  all_vars_are_jets=0;

  num_expr_vars = 0;
  exprVarLen = 0;

  current_expression = NULL;
  expression_list    = NULL;
  id_list = NULL;


}

void define_expression(Node var) {
  char tmp_name[32];
  int i,m;
  
  if(var) {
    int code = NODE_CODE(var);    
    //extern char *node_names[];
    // fprintf(stderr, "%s %s %s\n", node_names[code], NODE_NAME(var), NODE_GIVEN_NAME(var));
    if(code != ID_NODE) {
      Node nvar;
      sprintf(tmp_name, "expression__%d__",expr_counter++);
      nvar = install_id(tmp_name);
      define_one_variable(nvar, var);
      var = nvar;
    }
    
    code = NODE_CODE(var);    
    Node def =  findVarDef(var);
    
    if(code != ID_NODE)
        {
          fprintf(stderr, "Internal Error in expression declaration, Var is not an ID node.\n");
          exit(5);
        }
    if(! ID_IS_DEFINED(var) && !VAR_IS_STATE_VAR(var))
      {
	fprintf(stderr, "Error in expression declaration: symbol '%s' is not defined.\n", NODE_NAME(var));
	exit(5);
      }
   
    SET_VAR_IS_EXPR(var);
    SET_KEEP_ME(var);
    if(def)SET_KEEP_ME(def);      
  
    if(exprVars == NULL || exprVarLen  == 0) {
      exprVars = (Node *) malloc(32 * sizeof(Node));
      bzero(exprVars, 32 * sizeof(Node));
      exprVarLen = 32;
    }
    if(1+num_expr_vars >= exprVarLen) {
      exprVars = (Node *) my_realloc(exprVars, 2+2 * exprVarLen * sizeof(Node));
      exprVarLen = 2 * (1+exprVarLen);
    }
    exprVars[num_expr_vars] = var;
    num_expr_vars++;

    // record expression
    if(current_expression == NULL)
      current_expression=new_expression();
    
    i = EXPRESSION_COUNT(current_expression);
    m = EXPRESSION_SPACE(current_expression);
    if(i+1 >= m) {
      EXPRESSION_LIST(current_expression)= (Node *) my_realloc(EXPRESSION_LIST(current_expression), (m+m)*sizeof(Node));
      EXPRESSION_SPACE(current_expression) *= 2;
    }
    EXPRESSION_LIST(current_expression)[i] = var;
    EXPRESSION_COUNT(current_expression)=i+1;
  }
}
void build_expression_list(Node name) {
  if(name) {
    EXPRESSION_NAME(current_expression) = name;
    current_expression = NULL;
  }
}

void genVariablesExpressions(void)
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

  for(i = 0; i < num_expr_vars; i++)
    {
      Node def =  exprVars[i];

      if (NODE_CODE(def) != ID_NODE)
	{
	  fprintf(stderr,"Internal Error: RHS of %s'=%s is not a variable.\n",
		  NODE_NAME(equations[i].name), NODE_NAME(equations[i].def));
	  exit(10);
	}
      makeDependencyGraph(def,findVarDef(def));
    }
  

  if (debug) showDependExpression();
  
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

  for(i = 0; i < num_expr_vars; i++)
    {
      Node def =  exprVars[i];
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
  for(i = 0; i < num_expr_vars; i++)
    {
      Node def =  exprVars[i];
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
  for(i = 0; i < num_expr_vars; i++)
    {
      Node def =  exprVars[i];
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
  for(i = 0; i < num_expr_vars; i++)
    {
      Node def =  exprVars[i];
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

void showDependExpression()
{
  int i;
  Expression expr = expression_list;
  while(expr) {
    Node name = EXPRESSION_NAME(expr);
    Node *vlist = EXPRESSION_LIST(expr);
    int  vcount = EXPRESSION_COUNT(expr);    
    expr =  EXPRESSION_NEXT(expr);

    fprintf(stderr, "\n\n===== Expression %s: ", NODE_NAME(name));
    for(i = 0; i < vcount; i++) { fprintf(stderr,"%s,", NODE_NAME(vlist[i]));}
    for(i = 0; i < vcount; i++) {
      Node var = vlist[i];
      if(NODE_IS_CST(var) == 0) {
	fprintf(stderr, "\n\nDependency Graph for %s\n", NODE_NAME(var));
	showDepWork(var);
      }
    }
  }

}
void decomposeExpressions()
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
      //Node name = variables[i].name;      
      Node nn = variables[i].def;  /* !!!!!!!!!!! */
      simplify(&nn);
      variables[i].def = nn;
      //if(VAR_IS_EXPR(name)) {
	//SET_KEEP_ME(nn);
      //}
    }
  
  for(i = 0; i < neqns; i++)
    {
      Node nn = equations[i].def; /* !!!!!!!!!!! */
      simplify(&(nn));
      equations[i].def = nn;
      //SET_KEEP_ME(equations[i].def);
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

  for(i = 0; i < num_expr_vars; i++)
    {
      Node v =  exprVars[i];
      Node def = findVarDef(v);      
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


void  checkEquationsAndExpressions() {
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

  for(i = 0; i < num_expr_vars; i++) 
    {
      Node v =  exprVars[i];
      Node def = findVarDef(v);
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
	  exit(2);
	}      
    }  
}



void EmitExpressionCode() {

  int  j, ei;
  

  Expression expr = expression_list;
  while(expr) {

    Node name = EXPRESSION_NAME(expr);
    Node *vlist = EXPRESSION_LIST(expr);
    int  vcount = EXPRESSION_COUNT(expr);    
    expr =  EXPRESSION_NEXT(expr);
    
    fprintf(outfile, "\nMY_FLOAT *%s(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *out, MY_JET *sIn, MY_JET ***sOut)\n{\n", NODE_GIVEN_NAME(name));

    fprintf(outfile, "    MY_FLOAT **fptr;\n");
    fprintf(outfile, "    int _jz_i;\n");    
    
    fprintf(outfile, "    static MY_FLOAT  *_jz_save=NULL; \n");
    fprintf(outfile, "    static int  _jz_ginitialized=0;\n");
    fprintf(outfile, "    #pragma omp threadprivate(_jz_save, _jz_ginitialized)\n");      
    fprintf(outfile, "    /* allocating memory if needed */\n");
    fprintf(outfile, "    if (_jz_ginitialized == 0) { \n");

    fprintf(outfile, "     _jz_save = (MY_FLOAT *)malloc(%d *sizeof(MY_FLOAT));\n",  vcount+1);
    fprintf(outfile, "     for(_jz_i=0; _jz_i<=%d; _jz_i++) { InitMyFloat(_jz_save[_jz_i]);}\n",vcount);
    fprintf(outfile, "\n    _jz_ginitialized =1;\n");    
    fprintf(outfile, "    }\n\n"); 

    fprintf(outfile, "    fptr = %s_taylor_coefficients(t,x,0,sIn,sOut);\n", NODE_GIVEN_NAME(name));

    for(ei = 0; ei < vcount; ei++) {
      Node ev = vlist[ei];
      j=  ID_VOFFSET(ev) + ID_VINDEX(ev);
      fprintf(outfile, "    AssignMyFloat(_jz_save[%d], fptr[%d][0]); /* %s %s */\n",ei,j, NODE_GIVEN_NAME(ev), NODE_NAME(ev));
    }

    fprintf(outfile, "    if(out) {\n"); 
    fprintf(outfile, "       for(_jz_i = 0; _jz_i < %d; _jz_i++) {\n", vcount);
    fprintf(outfile, "          AssignMyFloat(out[_jz_i], _jz_save[_jz_i]);\n");        
    fprintf(outfile, "       }\n");
    fprintf(outfile, "     }\n");    
    fprintf(outfile, "    return _jz_save;\n");    
    fprintf(outfile, "}\n\n");

    //  derivative of this expression
    fprintf(outfile, "\n/* derivative of the above expression */\n");
    fprintf(outfile, "   /* input: \n");
    fprintf(outfile, "      t:     current value of the time variable \n");
    fprintf(outfile, "      x:     array represent values of the state variables\n");
    fprintf(outfile, "      sIn:   array representing the input jet variables\n");
    fprintf(outfile, "             If NULL, the x values are assigned.\n");
    fprintf(outfile, "      sOut:  Pointer output jet variables\n");
    fprintf(outfile, "             If NULL, the pointer is not modified.\n");
    fprintf(outfile, "      Return Value:\n");
    fprintf(outfile, "            Two D Array, rows are the taylor coefficients of the\n");
    fprintf(outfile, "            state variables\n\n");
    fprintf(outfile, "     */\n");    
    fprintf(outfile, "MY_FLOAT *%s_derivative(MY_FLOAT t, MY_FLOAT *x, MY_FLOAT *out, MY_JET *sIn, MY_JET ***sOut)\n{\n", NODE_GIVEN_NAME(name));

    fprintf(outfile, "    int                 _jz_i; ;\n");
    fprintf(outfile, "    MY_FLOAT           **fptr;\n");     

    fprintf(outfile, "    static MY_FLOAT  *_jz_save=NULL; \n");
    fprintf(outfile, "    static int  _jz_ginitialized=0;\n");
    fprintf(outfile, "    #pragma omp threadprivate(_jz_save, _jz_ginitialized)\n");      
    fprintf(outfile, "    if (_jz_ginitialized == 0) { \n");

    fprintf(outfile, "     _jz_save = (MY_FLOAT *)malloc(%d *sizeof(MY_FLOAT));\n",  vcount+1);
    fprintf(outfile, "     for(_jz_i=0; _jz_i<=%d; _jz_i++) { InitMyFloat(_jz_save[_jz_i]);}\n",vcount);
    fprintf(outfile, "\n    _jz_ginitialized =1;\n");    
    fprintf(outfile, "    }\n\n"); 

    fprintf(outfile, "    fptr = %s_taylor_coefficients(t,x,1,sIn,sOut);\n\n", NODE_GIVEN_NAME(name));    
    for(ei = 0; ei < vcount; ei++) {
      Node ev = vlist[ei];
      j=  ID_VOFFSET(ev) + ID_VINDEX(ev);
      fprintf(outfile, "    AssignMyFloat(_jz_save[%d], fptr[%d][1]); /* %s %s */\n",ei,j, NODE_GIVEN_NAME(ev), NODE_NAME(ev));
    }
    
    fprintf(outfile, "    if(out) {\n"); 
    fprintf(outfile, "       for(_jz_i = 0; _jz_i < %d; _jz_i++) {\n", vcount);
    fprintf(outfile, "          AssignMyFloat(out[_jz_i], _jz_save[_jz_i]);\n");        
    fprintf(outfile, "       }\n");
    fprintf(outfile, "    }\n");
    fprintf(outfile, "    return _jz_save;\n");    
    fprintf(outfile, "}\n\n");
  }          
}  


void EmitExpressinJetCode(void)
{
  extern char *genVarLvalueKth(Node var, char *kth);  
  int i,j,tmpcnt;
  
  Expression expr = expression_list;
  while(expr) {

    Node name = EXPRESSION_NAME(expr);
    Node *vlist = EXPRESSION_LIST(expr);
    int  vcount = EXPRESSION_COUNT(expr);    
    expr =  EXPRESSION_NEXT(expr);

    for( i=0; i < allVarsCount; i++) {
      Node var = allVarsList[i];
      NODE_DERI_DONE(var)=0;
    }
    
    fprintf(outfile, "MY_FLOAT **%s_taylor_coefficients(MY_FLOAT t, MY_FLOAT *x, int order_in, MY_JET *sIn, MY_JET ***sOut)\n{\n", NODE_GIVEN_NAME(name));

    outputExtern();   /* first output extern declarations */
    
    fprintf(outfile, "    int                 order = order_in + 1; \n");

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
    fprintf(outfile, "    static MY_FLOAT     *_jz_out[%d];\n",    vcount+1);   
    fprintf(outfile, "    static MY_FLOAT     _jz_tvar1, _jz_tvar2, _jz_tvar3, _jz_tvar4; /* tmp vars */\n");
    fprintf(outfile, "    static MY_FLOAT     _jz_uvar1, _jz_uvar2; /* tmp vars */\n");
    fprintf(outfile, "    static MY_FLOAT     _jz_svar1, _jz_svar2, _jz_svar3, _jz_svar4, _jz_svar5; /* tmp vars */\n");
    fprintf(outfile, "    static MY_FLOAT     _jz_wvar3, _jz_wvar4; /* tmp vars */\n");
    fprintf(outfile, "    static MY_FLOAT     _jz_zvar1, _jz_zvar2; /* tmp vars */\n");
    fprintf(outfile, "    static MY_FLOAT     _jz_MyFloatZERO, _kthtmp;\n");

    fprintf(outfile, "    #pragma omp threadprivate(_jz_out, _jz_jet, _jz_save, _jz_oneOverN, _jz_theNs, _jz_tvar1, _jz_tvar2, _jz_tvar3, _jz_tvar4, _jz_uvar1, _jz_uvar2, _jz_svar1, _jz_svar2, _jz_svar3, _jz_svar4, _jz_svar5, _jz_wvar3, _jz_wvar4, _jz_zvar1, _jz_zvar2, _jz_MyFloatZERO, _kthtmp)\n");
  
    if (total_jet_vars>0) {
      /* series */
      fprintf(outfile, "/* series bgn */\n");
      fprintf(outfile, "    static MY_JET     *_sjz_out[%d];\n",  vcount+1);           
      fprintf(outfile, "    static MY_JET       *_sjz_jet[%d], *_sjz_save = NULL;\n", total_jet_vars);
      fprintf(outfile, "    static MY_JET     _sjz_tvar1, _sjz_tvar2, _sjz_tvar3, _sjz_tvar4; /* tmp vars */\n");
      fprintf(outfile, "    static MY_JET     _sjz_uvar1, _sjz_uvar2; /* tmp vars */\n");
      fprintf(outfile, "    static MY_JET     _sjz_svar1, _sjz_svar2, _sjz_svar3, _sjz_svar4, _sjz_svar5; /* tmp vars */\n");
      fprintf(outfile, "    static MY_JET     _sjz_wvar3, _sjz_wvar4; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _sjz_zvar1, _sjz_zvar2; /* tmp vars */\n");
    fprintf(outfile, "    static MY_JET     _kthStmp; /* tmp vars */\n");
    fprintf(outfile, "    #pragma omp threadprivate(_sjz_out, _sjz_jet, _sjz_save, _sjz_tvar1, _sjz_tvar2, _sjz_tvar3, _sjz_tvar4, _sjz_uvar1, _sjz_uvar2, _sjz_svar1, _sjz_svar2, _sjz_svar3, _sjz_svar4, _sjz_svar5,  _sjz_wvar3, _sjz_wvar4, _sjz_zvar1, _sjz_zvar2, _kthStmp)\n");    
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
          

  fprintf(outfile, "\n    if (1) {\n");
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
  fprintf(outfile, "    _jz_ginitialized=1;\n");
    for(i=0; i< vcount; i++) {
      Node ev = vlist[i];
      fprintf(outfile, "    _jz_out[%d] = _jz_jet[%d];\n", i, ID_VOFFSET(ev) + ID_VINDEX(ev)); 
    }

  fprintf(outfile, "    if (sOut) {\n");
  if (total_jet_vars) {
      for(i=0; i< vcount; i++) {
	Node ev = vlist[i];
	fprintf(outfile, "    _sjz_out[%d] = _sjz_jet[%d];\n", i, ID_VOFFSET(ev) + ID_VINDEX(ev));
      }
    fprintf(outfile, "     *sOut = _sjz_jet;\n");      
  } else {
    fprintf(outfile, "     *sOut = NULL;\n");
  }
  fprintf(outfile, "    }\n");
  fprintf(outfile, "    return(_jz_jet);\n}\n\n");

 }
}


//
void gen_info_for_Wrappers(void)
{
  int i, jnk;

  fprintf(outfile, "char **taylor_info_%s(){\n",suffix);
  fprintf(outfile, "   static char *info[] = {\n");  
  
  for(i = 0; i < nexterns; i++)
    {
      Node var = externVars[i].name;
      Node idx = externVars[i].def;
      Node current=NULL, next;
      int  isint = ID_IS_INT(var);
      int  it = ID_INT_TYPE(var);
      char *ttt = NULL;

      if (isint) ttt = (it == 4 ? "int": (it == 2 ? "short" : "char"));
      else ttt = "MY_FLOAT";
      fprintf(outfile, "\t\"extern %s\", \"%s", ttt,  NODE_NAME(var));

      while(idx != NULL)
        {
          current = NODE_LEFT(idx);
          next = NODE_RIGHT(idx);
          if (current == NULL) fprintf(outfile, "[]");
          else fprintf(outfile, "[%d]", intNodeValue(current, &jnk));
          idx = next;
        }
      fprintf(outfile, "\",\n ");
    }
  for(i = 0; i < neqns; i++)
    fprintf(outfile,"\t\"state variable\", \"%s\",\n", NODE_GIVEN_NAME(equations[i].name));
  fprintf(outfile,"\t\"time variable\", \"%s\",\n", NODE_GIVEN_NAME(timeVar));
  fprintf(outfile,"\t\"model name\", \"%s\",\n", suffix);
  if(nonautonomous) {
    fprintf(outfile,"\t\"autonomous\", \"False\",\n");
  }
  fprintf(outfile,"\tNULL, NULL\n");      
  fprintf(outfile, "   };\n");
  fprintf(outfile, "  return info;\n");
  fprintf(outfile, "};\n");  
  
}
