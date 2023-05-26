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

#define CODE1_C
#include "Header.h"
/*************************************************************************************************/
static char *genCstFString(Node def);
static char *genCstIString(Node def);
static char *genLocalVarLvalue(Node var);
static char *genArrayRef(Node aref);
static char *genNodeRvalueF(Node node);
static char *genNodeRvalueI(Node node);
static void computeKthDeri(Node var, Node def);
static void outputBoolExpr(int code, Node left, Node right, char *name1);
static char *genNodeKthDerivative(Node var, char *kth);
char *genVarLvalueKth(Node var, char *kth);
char *genGlobalVarLvalue(Node var);
/*************************************************************************************************/
static int  icnt1=0, icnt2=0, jcnt3=0, jcnt4=0;
/*************************************************************************************************
 *
 *  Output a INT or FLOAT constant
 */
static char *genCstFString(Node def)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;
  if(def) sprintf(out, "%s", NODE_INFO(def));
  return(out);
}

/*************************************************************************************************
 *
 *  Output a INT or FLOAT as integer constant 
 */
static char *genCstIString(Node def)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(def)
    {
      int jnk;
      if(nodeIsNumber(def)) sprintf(out, "%d", intNodeValue(def, &jnk));
      else sprintf(out, "(int)(%s)", NODE_INFO(def));
    }

  return(out);
}

/*************************************************************************************************
 * output a local var as a lvalue: 
 * require loop counter's name to be 'm'
 */
static char *genLocalVarLvalue(Node var)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(var)
    {
      SumP sum = NODE_SUM_BLOCK(var);
      Node idx = SUMB_IDX(sum);
      int  vcount = SUMB_LVAR(sum);
      int  ccount = SUMB_LCST(sum);
      int  voffset = SUMB_VOFFSET(sum);
      int  icount = SUMB_LINT(sum);
      int  coffset = SUMB_COFFSET(sum);
      int  ioffset = SUMB_IOFFSET(sum);

      if(var == idx) return(NODE_NAME(idx));
      else if(VAR_IS_LOCAL_VAR(var))
        sprintf(out, "_jz_jet[_jz_m*%d+(%d+%d)][0]", vcount, voffset, ID_VOFFSET(var));
      else if(VAR_IS_LOCAL_CVAR(var))
        sprintf(out, "_jz_cvars[_jz_m*%d+(%d+%d)]", ccount, coffset, ID_COFFSET(var)); 
      else if(VAR_IS_LOCAL_IVAR(var))
        sprintf(out, "_jz_ivars[_jz_m*%d+(%d+%d)]", icount, ioffset, ID_IOFFSET(var)); 
      else
        {
          fprintf(stderr, "Internal error: don't know how to assign LVALUE to lvar %s\n",
                  NODE_GIVEN_NAME(var)?NODE_GIVEN_NAME(var): NODE_NAME(var));
          exit(31);
        }
    }
  return(out);
}

char  *genVarLvalueKth(Node var, char *kth)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(var && VAR_IS_LOCAL(var))
    {
      SumP sum = NODE_SUM_BLOCK(var);
      int  vcount = SUMB_LVAR(sum);
      int  voffset = SUMB_VOFFSET(sum);
      
      if(VAR_IS_LOCAL_VAR(var))
        sprintf(out, "_jz_jet[_jz_m*%d+(%d+%d)][%s]", vcount, voffset, ID_VOFFSET(var), kth);
      else
        {
          fprintf(stderr, "Internal error: don't know how to assign the kth LVALUE  to lvar %s\n",
                  NODE_GIVEN_NAME(var)? NODE_GIVEN_NAME(var):NODE_NAME(var));
          exit(31);
        }
    }
  else if(var && VAR_IS_GLOBAL(var))
    {
      if(VAR_IS_GLOBAL_VAR(var)) {
	if(VAR_IS_JET(var)) {
	  sprintf(out, "_sjz_jet[%d][%s]",  ID_JINDEX(var), kth);
	} else {
	  sprintf(out, "_jz_jet[%d][%s]", ID_VOFFSET(var) + ID_VINDEX(var), kth);
	}
      }
      else
        {
          fprintf(stderr, "Internal error: don't know how to assign the kth LVALUE to gvar %s\n",
                  NODE_GIVEN_NAME(var)?NODE_GIVEN_NAME(var):NODE_NAME(var));
          exit(32);
        }      
    }
  return(out);
}
/*************************************************************************************************
 * output a global var as a lvalue.
 */
char *genGlobalVarLvalue(Node var)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(var)
    {
      if(VAR_IS_GLOBAL_VAR(var)) {
	if(VAR_IS_JET(var)) {
	  sprintf(out, "_sjz_jet[%d][0]",  ID_JINDEX(var));
	} else {
	  sprintf(out, "_jz_jet[%d][0]", ID_VOFFSET(var) + ID_VINDEX(var));
	}
      }
      else if(VAR_IS_GLOBAL_CVAR(var))
        sprintf(out, "_jz_cvars[%d]", ID_COFFSET(var) + ID_CINDEX(var)); 
      else if(VAR_IS_GLOBAL_IVAR(var))
        sprintf(out, "_jz_ivars[%d]", ID_IOFFSET(var) + ID_IINDEX(var));
      else if(NODE_CODE(var) == ID_NODE && ID_IS_EXTERN(var)) {
	/* 2022/03/29: bug fix
	 *  when assign an extern variable to a variable, the RHS is just
	 *  an ID node, it is not assigned to one of the variaiables above.
	 *  We just need to output the name. The user should define it
	 *  externally.
	 */
	sprintf(out, "%s",NODE_INFO(var));
      }
      else
        {
          fprintf(stderr, "Internal error: don't know how to assign LVALUE to gvar %s\n",
                  NODE_GIVEN_NAME(var)?NODE_GIVEN_NAME(var):NODE_NAME(var));
          exit(32);
        }
    }
  return(out);
}

/*************************************************************************************************
 *
 * output array reference
 */
static char  *genArrayRef(Node aref)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(aref)
    {
      char  istr[256], tmpstr[128], *iout;
      Node aname = NODE_LEFT(aref);
      Node idx = NODE_RIGHT(aref);

      istr[0] = '\0';
      while(idx)
        {
          Node thisIdx = NODE_LEFT(idx);
          idx = NODE_RIGHT(idx);
          iout = genNodeRvalueI(thisIdx);
          sprintf(tmpstr, "[%s]", iout);
          strcat(istr, tmpstr);
        }
      sprintf(out, "%s%s", NODE_NAME(aname), istr);
    }
  return(out);
}


/*************************************************************************************************
 *
 * output a node as rvalue (float).
 */
static char *genNodeRvalueF(Node node)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(node)
    {
      int code = NODE_CODE(node);
      switch(code)
        {
        case ID_NODE:
          if(VAR_IS_LOCAL(node))
            {
              Node var = node;
              SumP sum = NODE_SUM_BLOCK(var);
              Node idx = SUMB_IDX(sum);
              int  vcount = SUMB_LVAR(sum);
              int  ccount = SUMB_LCST(sum);
              int  voffset = SUMB_VOFFSET(sum);
              int  icount = SUMB_LINT(sum);
              int  coffset = SUMB_COFFSET(sum);
              int  ioffset = SUMB_IOFFSET(sum);

              if(var == idx)
                {
                  if(cdouble) sprintf(out, "%s", NODE_NAME(idx));
                  else {
                    if(icnt1&1)sprintf(out, "MakeMyFloatB(_jz_tvar1,(double)%s)", NODE_NAME(idx));
		    else sprintf(out, "MakeMyFloatB(_jz_uvar1,(double)%s)", NODE_NAME(idx));
		    icnt1++;
                  }
                }
              else
                {
                  if(VAR_IS_LOCAL_VAR(var))
		    sprintf(out, "_jz_jet[_jz_m*%d+(%d+%d)][0]", vcount, voffset, ID_VOFFSET(var));
                  else if(VAR_IS_LOCAL_CVAR(var))
                    {
                      if(cdouble && nodeIsNumber(var)) {
			if(jcnt3&1) sprintf(out, "MakeMyFloatB(_jz_svar3,(double)(%.17f))", cstNodeValue(var));
			else  sprintf(out, "MakeMyFloatB(_jz_wvar3,(double)(%.17f))", cstNodeValue(var));
			jcnt3++;
		      } else sprintf(out, "_jz_cvars[_jz_m*%d+(%d+%d)]", ccount, coffset, ID_COFFSET(var));
                    }
                  else if(VAR_IS_LOCAL_IVAR(var))
                    {
                      int done = 0;
                      if(cdouble && nodeIsNumber(var))
                        {
                          int jnk=0, v = intNodeValue(var,&jnk);
                          if( jnk == 0)
                            {
                              sprintf(out, "(%d)", v);
                              done = 1;
                            }
                        }
                      if(!done) {
                        if(icnt1&1) 
			  sprintf(out, "MakeMyFloatB(_jz_tvar1, (double)_jz_ivars[_jz_m*%d+(%d+%d)])", icount, ioffset, ID_IOFFSET(var)); 
			else
			  sprintf(out, "MakeMyFloatB(_jz_uvar1, (double)_jz_ivars[_jz_m*%d+(%d+%d)])", icount, ioffset, ID_IOFFSET(var)); 
			icnt1++;
		      }
                    }
                  else
                    {
                      fprintf(stderr, "Internal error: don't know how to assign LVALUE to lvar %s\n",
                              NODE_GIVEN_NAME(var)? NODE_GIVEN_NAME(var): NODE_NAME(var));
                      exit(31);
                    }
                }
              return(out);
            }
          else if(VAR_IS_GLOBAL(node))
            {
              Node var = node;
              
              if(VAR_IS_GLOBAL_VAR(var)) {
		if(VAR_IS_JET(var)) {
		  sprintf(out, "_sjz_jet[%d][0]",  ID_JINDEX(var));
		} else {
		  sprintf(out, "_jz_jet[%d][0]", ID_VOFFSET(var) + ID_VINDEX(var));
		}
	      }
              else if(VAR_IS_GLOBAL_CVAR(var))
                {
                  if(cdouble && nodeIsNumber(var)) {
                    if(jcnt4&1) sprintf(out, "MakeMyFloatB(_jz_svar4,(double)(%.17f))", cstNodeValue(var));
		    else sprintf(out, "MakeMyFloatB(_jz_wvar4,(double)(%.17f))", cstNodeValue(var));
		    jcnt4++;
		  } else sprintf(out, "_jz_cvars[%d]", ID_COFFSET(var) + ID_CINDEX(var));
                }
              else if(VAR_IS_GLOBAL_IVAR(var))
                {
                  int done = 0;
                  if(cdouble && nodeIsNumber(var))
                    {
                      int jnk=0, v = intNodeValue(var,&jnk);
                      if( jnk == 0)
                        {
                          sprintf(out, "(%d)", v);
                          done = 1;
                        }
                    }
                  if(!done) {
                    if(icnt1&1)sprintf(out, "MakeMyFloatB(_jz_tvar1,(double)_jz_ivars[%d])", ID_IOFFSET(var) + ID_IINDEX(var)); 
		    else sprintf(out, "MakeMyFloatB(_jz_uvar1,(double)_jz_ivars[%d])", ID_IOFFSET(var) + ID_IINDEX(var)); 
		    icnt1++;
		  }
                }
              else
                {
                  fprintf(stderr, "Internal error: don't know how to assign LVALUE to gvar %s\n",
                          NODE_GIVEN_NAME(var)?NODE_GIVEN_NAME(var):NODE_NAME(var));
                  exit(32);
                }
              return(out);
            }
          else if(NODE_IS_INDEX(node)) 
	    {
	      if(icnt1&1)sprintf(out,"(MakeMyFloatB(_jz_tvar1, (double)%s))", NODE_NAME(node));
	      else sprintf(out,"(MakeMyFloatB(_jz_uvar1, (double)%s))", NODE_NAME(node));
	      icnt1++;
	      return(out);
	    }
	  else if(ID_IS_EXTERN(node)) 
	    {
	      if(ID_IS_INT(node))
		{
		  if(icnt1&1)sprintf(out, "(MakeMyFloatB(_jz_tvar1, (double)%s),_jz_tvar1)", NODE_NAME(node));
		  else sprintf(out, "(MakeMyFloatB(_jz_uvar1, (double)%s),_jz_uvar1)", NODE_NAME(node));
		  icnt1++;
		  return(out);
		}
	      else return(NODE_NAME(node));
	    }
	  else 
            {
              fprintf(stderr, "Internal Error: var '%s' is neither local nor global.\n", 
                      NODE_GIVEN_NAME(node)? NODE_GIVEN_NAME(node): NODE_NAME(node));
              exit(41);
            }
          break;
        case INT_CST:
        case FLOAT_CST:
          return(genCstFString(node));
          break;
        case ARRAY_REF:
          return(genArrayRef(node));
          break;
        default:
          fprintf(stderr, "Internal Error: Illegal node in 'genRvalue'. Code=%d (%s)\n", code,
                  NODE_GIVEN_NAME(node)? NODE_GIVEN_NAME(node): NODE_NAME(node));
          exit(42);
          break;
        }
    }
  return(NULL);
}

static char *genNodeRvalueI(Node node)
{
  if(node)
    {
      int code = NODE_CODE(node);

      if(cdouble && nodeIsNumber(node)) return(genCstIString(node));
        
      switch(code)
        {
        case ID_NODE:
          if(VAR_IS_LOCAL(node)) return(genLocalVarLvalue(node));
          else if(VAR_IS_GLOBAL(node)) return(genGlobalVarLvalue(node));
          else if(NODE_IS_INDEX(node)) return(NODE_NAME(node));
	  else if(ID_IS_EXTERN(node))  return(NODE_NAME(node));
          else 
            {
              fprintf(stderr, "Internal Error: var '%s' is neither local nor global.\n", 
                      NODE_GIVEN_NAME(node)?NODE_GIVEN_NAME(node): NODE_NAME(node));
              exit(41);
            }
          break;
        case INT_CST:
        case FLOAT_CST:
          return(genCstIString(node));
          break;
        case ARRAY_REF:
          return(genArrayRef(node));
          break;
        default:
          fprintf(stderr, "Internal Error: Illegal node in 'genRvalue'. Code=%d (%s)\n", code, 
                  NODE_GIVEN_NAME(node)? NODE_GIVEN_NAME(node):NODE_NAME(node));
          exit(42);
          break;
        }
    }
  return(NULL);
}
/****************************************************************************************************
 *
 *  output     name = left BOP right
 */
static void outputBoolExpr(int code, Node left, Node right, char *name1)
{
  int i1=0, i2=0; 
  char *lstr, *rstr;
  
  i1 = (VAR_IS_LOCAL_IVAR(left)  || VAR_IS_GLOBAL_IVAR(left));
  i2 = (VAR_IS_LOCAL_IVAR(right) || VAR_IS_GLOBAL_IVAR(right));

  fprintf(outfile,"\t\t {\n\t\t\t static MY_FLOAT tmpa, tmpb;\n");

  fprintf(outfile, "\t\t\t if(_jz_initialized == 0) ");
  fprintf(outfile," { InitMyFloat(tmpa); InitMyFloat(tmpb); }\n");
  if(i1 && i2)
    {
      lstr = genNodeRvalueI(left);
      rstr = genNodeRvalueI(right);
    }
  else
    {
      if(i1)
        {
          lstr = genNodeRvalueI(left);
          fprintf(outfile, "\t\t\t MakeMyFloatA(tmpa, (double)%s);\n", lstr);
          lstr = "tmpa";
        }
      else
        {
          lstr = genNodeRvalueF(left);
        }
      if(i2)
        {
          rstr = genNodeRvalueI(right);
          fprintf(outfile, "\t\t\t MakeMyFloatA(tmpb, (double)%s);\n", rstr);
          rstr = "tmpb";
        }
      else
        {
          rstr = genNodeRvalueF(right);
        }
    }

  if(i1 && i2)
    {
      switch(code)
        {
        case GE_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s >= %s);\n", name1, lstr, rstr);
          break;
        case GT_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s > %s);\n", name1, lstr, rstr);
          break;
        case LE_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s <= %s);\n", name1, lstr, rstr);
          break;
        case LT_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s < %s);\n", name1, lstr, rstr);
          break;
        case EQ_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s == %s);\n", name1, lstr, rstr);
          break;
        case NEQ_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s != %s);\n", name1, lstr, rstr);
          break;
        case AND_EXPR:
          fprintf(outfile, "\t\t\t %s= (%s && %s);\n", name1, lstr, rstr);
          break;
        case OR_EXPR:      
          fprintf(outfile, "\t\t\t %s= (%s || %s);\n", name1, lstr, rstr);
          break;
        }
    }
  else
    {
      switch(code)
        {
        case GE_EXPR:
          fprintf(outfile, "\t\t\t %s= MyFloatA_GE_B(%s, %s);\n", name1, lstr, rstr);
          break;
        case GT_EXPR:
          fprintf(outfile, "\t\t\t %s= MyFloatA_GT_B(%s, %s);\n", name1, lstr, rstr);
          break;
        case LE_EXPR:
          fprintf(outfile, "\t\t\t %s= MyFloatA_LE_B(%s, %s);\n", name1, lstr, rstr);
          break;
        case LT_EXPR:
          fprintf(outfile, "\t\t\t %s= MyFloatA_LT_B(%s, %s);\n", name1, lstr, rstr);
          break;
        case EQ_EXPR:
          fprintf(outfile, "\t\t\t %s= MyFloatA_EQ_B(%s, %s);\n", name1, lstr, rstr);
          break;
        case NEQ_EXPR:
          fprintf(outfile, "\t\t\t %s= MyFloatA_NEQ_B(%s, %s);\n", name1, lstr, rstr);
          break;
        case AND_EXPR:
          fprintf(stderr, "Error: Boolean AND_EXPR has non-integer operands\n");
          break;
        case OR_EXPR:      
          fprintf(stderr, "Error: Boolean OR_EXPR has non-integer operands\n");
          break;
        }
    }
  fprintf(outfile,"\t\t }\n");
}
/****************************************************************************************************
 *
 *  initialize variables in a sum block.
 */

void outputSumInitialVars(SumP sum)
{
  Node idx  = SUMB_IDX(sum);
  Node *all  = SUMB_ALL_VARS(sum);
  int  from = SUMB_FROM(sum);
  int  to   = SUMB_TO(sum);
  int  vcount = SUMB_LVAR(sum);
  int  ccount = SUMB_LCST(sum);
  int  icount = SUMB_LINT(sum);
  int  total = vcount + ccount + icount;
  int  k;
  
  fprintf(outfile, "\n\t /*begin sum block*/\n");
  fprintf(outfile, "\t {\n\t    register int %s;\n", NODE_NAME(idx));
  fprintf(outfile, "\t    for(%s = %d, _jz_m = 0; %s <= %d; %s += 1, _jz_m++) {\n", 
	  NODE_NAME(idx), from, NODE_NAME(idx), to, NODE_NAME(idx));
  for(k=0; k < total; k++)
    {
      Node var = all[k];
      char *name1, *name2, *leftStr, *rightStr;
      Node numer=NULL, deno=NULL;
      Node def = variables[ID_V_INDEX(var)].def;
      int  ratexp=0, code = NODE_CODE(def);

      if(VAR_IS_GLOBAL(var))
        {
          fprintf(stderr, "Internal error: Global var %s is also marked as a local var.\n", 
                  NODE_GIVEN_NAME(var)? NODE_GIVEN_NAME(var): NODE_NAME(var));
          exit(21);
        }
      if(VAR_IS_LOCAL_VAR(var) || VAR_IS_LOCAL_CVAR(var))
        {
          name1 = genLocalVarLvalue(var);
          switch(code)
            {
            case ID_NODE:
	      fprintf(outfile, "\t\t /* id: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	      name2 = genGlobalVarLvalue(def);
              fprintf(outfile, "\t\t AssignMyFloat(%s, %s);\n",name1,name2);
              break;
            case INT_CST:
            case FLOAT_CST:
	      fprintf(outfile, "\t\t /* constant: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              fprintf(outfile, "\t\t MakeMyFloatC(%s,\"%s\", %s);\n", name1, genCstFString(def), genCstFString(def));
              break;  
            case ARRAY_REF: 
	      fprintf(outfile, "\t\t /* array_ref: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              fprintf(outfile, "\t\t AssignMyFloat(%s,%s);\n", name1, genArrayRef(def));
              break;
            case PLUS_EXPR:
	      fprintf(outfile, "\t\t /* plus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueF(NODE_LEFT(def));
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
              fprintf(outfile, "\t\t AddMyFloatA(%s, %s, %s);\n", 
                      name1, leftStr, rightStr);
              break;
            case MINUS_EXPR:
	      fprintf(outfile, "\t\t /* minus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueF(NODE_LEFT(def));
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
              fprintf(outfile, "\t\t SubtractMyFloatA(%s, %s, %s);\n", 
                      name1, leftStr, rightStr);
              break;
            case MULT_EXPR:
	      fprintf(outfile, "\t\t /* mult: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueF(NODE_LEFT(def));
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
              fprintf(outfile, "\t\t MultiplyMyFloatA(%s, %s, %s);\n", 
                      name1, leftStr, rightStr);
              break;
            case DIV_EXPR:
	      fprintf(outfile, "\t\t /* div: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueF(NODE_LEFT(def));
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
              fprintf(outfile, "\t\t DivideMyFloatA(%s, %s, %s);\n", 
                      name1, leftStr, rightStr);
              break;
            case EXP_EXPR:           
	      fprintf(outfile, "\t\t /* exponentiation: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueF(NODE_LEFT(def));
              ratexp = isRational(NODE_RIGHT(def), &numer, &deno);
	      if(ratexp && (ratexp == 8 || ratexp == -8)) { if(use_rational_exponent) { ratexp=(ratexp==8? 1 : -1);}}
              if(ratexp == 1 || ratexp == -1)
                {
                  int spp=1, jk, pp = intNodeValue(numer, &jk);
                  int qq = intNodeValue(deno, &jk);
                  double expo;
                  if(ratexp < 0) qq = -qq;
                  if(abs(qq) <= 2) {
                    /* integer or half integer exponent */
                    fprintf(outfile, "\t\t  /* integer or half integer exponent */\n");
                    pp = pp * qq; 
                    if(pp < 0) { spp = -1; pp=-pp;}
                    if(abs(qq) == 2) { 
                      pp /= 2;
                      if(spp == -1) {
                        fprintf(outfile, "\t\t sqrtMyFloatA(_jz_svar1,%s);\n", leftStr);                      
                        fprintf(outfile, "\t\t DivideMyFloatA(_jz_svar5, _jz_oneOverN[0], _jz_svar1);\n");                      
                      } else
                        fprintf(outfile, "\t\t sqrtMyFloatA(_jz_svar5,%s);\n", leftStr);                      
                    } else {
                      if(spp == -1) {
                        fprintf(outfile, "\t\t AssignMyFloat(_jz_svar1,%s);\n", leftStr); 
                        fprintf(outfile, "\t\t DivideMyFloatA(_jz_svar5, _jz_oneOverN[0], _jz_svar1);\n");                      
                      }
                      else 
                        fprintf(outfile, "\t\t AssignMyFloat(_jz_svar5,%s);\n", leftStr); 
                    }
                    fprintf(outfile, "\t\t { int n=%d, m, mn=0; \n", pp);
                    fprintf(outfile, "\t\t   switch(n) {\n");
                    fprintf(outfile, "\t\t\t  case 0: AssignMyFloat(%s, _jz_oneOverN[0]); break;\n", name1);
                    fprintf(outfile, "\t\t\t  case 1: AssignMyFloat(%s, _jz_svar5); break;\n", name1);
                    fprintf(outfile, "\t\t\t  case 2: AssignMyFloat(_jz_svar1, _jz_svar5); MultiplyMyFloatA(%s,_jz_svar1,_jz_svar5); break;\n",
                            name1);
                    fprintf(outfile, "\t\t\t  case 3: AssignMyFloat(_jz_svar1, _jz_svar5); MultiplyMyFloatA(_jz_svar2,_jz_svar1,_jz_svar5);\n");
                    fprintf(outfile, "\t\t\t          MultiplyMyFloatA(%s,_jz_svar1,_jz_svar2); break;\n", name1);
                    fprintf(outfile, "\t\t\t  default:\n");
                    fprintf(outfile, "\t\t\t   AssignMyFloat(_jz_svar1, _jz_oneOverN[0]); AssignMyFloat(_jz_svar2, _jz_svar5);\n");
                    fprintf(outfile, "\t\t\t     while(mn==0) {\n");
                    fprintf(outfile, "\t\t\t        m=n; n /=2; if(n+n != m) {\n");
                    fprintf(outfile, "\t\t\t           AssignMyFloat(_jz_svar3, _jz_svar1); MultiplyMyFloatA(_jz_svar1, _jz_svar3, _jz_svar2);\n");
                    fprintf(outfile, "\t\t\t           if(n==0){ mn=1; ");
                    fprintf(outfile, "    AssignMyFloat(%s,_jz_svar1);}\n",name1);
                    fprintf(outfile, "\t\t\t         }\n");
                    fprintf(outfile, "\t\t\t        if(mn==0) {AssignMyFloat(_jz_svar3, _jz_svar2);AssignMyFloat(_jz_svar4, _jz_svar2);\n");
                    fprintf(outfile, "\t\t\t                   MultiplyMyFloatA(_jz_svar2, _jz_svar3,_jz_svar4);}\n");
                    fprintf(outfile, "\t\t\t       }\n");
                    fprintf(outfile, "\t\t\t   break; \n");
                    fprintf(outfile, "\t\t\t  }\n\t\t } \n");
                  } else { /* fishy here */
                    expo = (double)(pp)/(double)qq;
                    fprintf(outfile, "\t\t if(MyFloatA_GT_B(%s,_jz_MyFloatZERO)) { \n", leftStr);
		    if(icnt2&1)
		      fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, %s, MakeMyFloatB(_jz_tvar2,%.15f));\n", name1, leftStr, expo);
		    else
		      fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, %s, MakeMyFloatB(_jz_uvar2,%.15f));\n", name1, leftStr, expo);
		    icnt2++;
                    fprintf(outfile, "\t\t } else if(MyFloatA_LT_B(%s,_jz_MyFloatZERO)) {\n", leftStr);
                    if( qq & 1) { /* odd root, ok */
                      if(pp & 1) { /* pp is odd */
                        fprintf(outfile, "\t\t    NegateMyFloatA(_jz_svar1, %s);\n", leftStr);
                        if(icnt2&1) fprintf(outfile, "\t\t    ExponentiateMyFloatA(_jz_svar2, _jz_svar1, MakeMyFloatB(_jz_tvar2,%.15f));\n", expo);
			else fprintf(outfile, "\t\t    ExponentiateMyFloatA(_jz_svar2, _jz_svar1, MakeMyFloatB(_jz_uvar2,%.15f));\n", expo);
			icnt2++;
                        fprintf(outfile, "\t\t    NegateMyFloatA(%s,_jz_svar2);\n", name1);
                      } else { /* pp is even */
                        fprintf(outfile, "\t\t    NegateMyFloatA(_jz_svar1, %s);\n", leftStr);
                        if(icnt2&1)fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, _jz_svar1, MakeMyFloatB(_jz_tvar2,%.15f));\n", name1, expo);
			else fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, _jz_svar1, MakeMyFloatB(_jz_uvar2,%.15f));\n", name1, expo);
			icnt2++;
                      }
                    } else { /* qq is even: ?^(pp/qq) is undefiled */
                      fprintf(outfile, "\t\t     fprintf(stderr, \"Attemp to compute negative^(%d/%d)\");exit(0);\n",pp,qq);
                    }
                    fprintf(outfile, "\t\t } else {\n");                    
                    if(expo > 0.0) { fprintf(outfile, "\t\t    AssignMyFloat(%s,_jz_oneOverN[0]);\n\t\t }\n", name1);}
                    else { fprintf(outfile, "\t\t   fprintf(stderr,\"Attemp to compute 0^(%d/%d)\");exit(0);\n\t\t}\n",pp,qq);}
                  }
                }
              else
                {
                  rightStr = genNodeRvalueF(NODE_RIGHT(def));
                  fprintf(outfile, "\t\t ExponentiateMyFloatA(%s, %s, %s);\n", 
                          name1, leftStr, rightStr);
                }
              break;              
            case NEGATE_EXPR:
	      fprintf(outfile, "\t\t /* negate: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueF(NODE_LEFT(def));
              fprintf(outfile, "\t\t NegateMyFloatA(%s, %s);\n", 
                      name1, leftStr);
              break;                            
            case CALL_EXPR:
	      fprintf(outfile, "\t\t /* call: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = NODE_NAME(NODE_LEFT(def));
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
              fprintf(outfile, "\t\t %sMyFloatA(%s, %s);\n", 
                      leftStr, name1, rightStr);
              break;                            
            case IF_ELSE_EXPR:
	      fprintf(outfile, "\t\t /* if_else: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              {
                char *condstr = genNodeRvalueI(IF_ELSE_COND(def));
                leftStr = genNodeRvalueF(NODE_LEFT(def));
                rightStr = genNodeRvalueF(NODE_RIGHT(def));
                fprintf(outfile, "\t\t if(%s) \n\t\t\t { AssignMyFloat(%s,%s);} \n\t\t else \n\t\t\t{ AssignMyFloat(%s, %s);}\n",
			condstr, name1, leftStr, name1, rightStr);
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
              {
                fprintf(stderr, "Internal Error: bool expr appeared in wrong place\n");
                exit(42);
              }
              break;
            }
        }
      else if(VAR_IS_LOCAL_IVAR(var))
        {

          name1 = genLocalVarLvalue(var);
          switch(code)
            {
            case INT_CST:
            case FLOAT_CST:
	      fprintf(outfile, "\t\t /* constant: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              fprintf(outfile, "\t\t %s=%s;\n", name1, genCstIString(def));
              break;  
            case ID_NODE:
	      fprintf(outfile, "\t\t /* mult: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              name2 = genGlobalVarLvalue(var);
              fprintf(outfile, "\t\t %s=(int)%s;\n",name1,name2);
              break;
            case ARRAY_REF: 
	      fprintf(outfile, "\t\t /* array_ref: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              fprintf(outfile, "\t\t %s=(int)%s;\n", name1, genArrayRef(def));
              break;
            case PLUS_EXPR:
	      fprintf(outfile, "\t\t /* plus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueI(NODE_LEFT(def));
              rightStr = genNodeRvalueI(NODE_RIGHT(def));
              fprintf(outfile, "\t\t %s=%s+%s;\n",name1, leftStr, rightStr);
              break;
            case MINUS_EXPR:
	      fprintf(outfile, "\t\t /* minus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueI(NODE_LEFT(def));
              rightStr = genNodeRvalueI(NODE_RIGHT(def));
              fprintf(outfile, "\t\t %s=%s-%s;\n",name1, leftStr, rightStr);
              break;
            case MULT_EXPR:
	      fprintf(outfile, "\t\t /* mult: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueI(NODE_LEFT(def));
              rightStr = genNodeRvalueI(NODE_RIGHT(def));
              fprintf(outfile, "\t\t %s=%s*%s;\n",name1, leftStr, rightStr);
              break;
            case DIV_EXPR:
	      fprintf(outfile, "\t\t /* div: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueI(NODE_LEFT(def));
              rightStr = genNodeRvalueI(NODE_RIGHT(def));
              fprintf(outfile, "\t\t %s=%s/%s;\n",name1, leftStr, rightStr);
              break;
            case EXP_EXPR:           
	      fprintf(outfile, "\t\t /* exponentiation: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueI(NODE_LEFT(def));
              ratexp = isRational(NODE_RIGHT(def), &numer, &deno);
	      if(ratexp && (ratexp == 8 || ratexp == -8)) { if(use_rational_exponent) { ratexp=(ratexp==8? 1 : -1);}}
              if(ratexp == 1 || ratexp == -1)
                {
                  int spp=1, jk, pp = intNodeValue(numer, &jk);
                  int qq = intNodeValue(deno, &jk);
                  double expo;
                  if(ratexp < 0) qq = -qq;
                  if(abs(qq) <= 2) {
                    fprintf(outfile, "\t\t /* integer or half integer exponent */;\n");
                    pp = pp * qq;
                    if(pp < 0) {spp = -1; pp=-pp;}
                    if(abs(qq) == 2) {  pp /=2;}
                    fprintf(outfile, "\t\t { double base, dtmp,dsmp; int m,n=%d, mn=0;\n",pp);
                    //fprintf(outfile, "\t\t MyFloatToDouble(base,%s);\n",leftStr);
                    fprintf(outfile, "\t\t  base = (double)%s;\n",leftStr);
                    if(abs(qq) == 2) { fprintf(outfile, "\t\t base =sqrt(base);\n"); }
                    if(spp == -1) fprintf(outfile, "\t\t base =(double)1.0/base;\n");                      
                    fprintf(outfile, "\t\t switch(n) { case 0: %s=1.0; break; case 1: %s=base; break;\n", name1,name1);
                    fprintf(outfile, "\t\t             case 2: %s=base*base; break; case 3: dtmp=base*base;%s=dtmp*base; break;\n",
                            name1,name1);                    
                    fprintf(outfile, "\t\t             default: dtmp=1.0; dsmp=base;\n");
                    fprintf(outfile, "\t\t              while(mn==0){m=n;n/=2;if(n+n!=m){dtmp*=dsmp;if(n==0){mn=1;%s=dtmp;}}\n",name1);
                    fprintf(outfile, "\t\t                     if(mn==0){smp=smp*smp;}\n\t\t\t  }\n\t\t  }\n\t\t}\n");   
                  } else {
                    expo = (double)(pp)/(double)qq;
                    fprintf(outfile, "\t\t { double base;\n");
                    //fprintf(outfile, "\t\t MyFloatToDouble(base,%s);\n",leftStr);
                    fprintf(outfile, "\t\t base=(double)%s;\n",leftStr);
                    fprintf(outfile, "\t\t %s = (int)pow(base, (%.15f));\n", 
                            name1, expo);
                    fprintf(outfile, "\t\t }\n");
                  }
                }
              else
                {
                  rightStr = genNodeRvalueI(NODE_RIGHT(def));
                  fprintf(outfile, "\t\t %s=(int)(pow((double)%s, (double)%s));\n",name1, leftStr, rightStr);
                }
              break;              
            case NEGATE_EXPR:
	      fprintf(outfile, "\t\t /* negate: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = genNodeRvalueI(NODE_LEFT(def));
              fprintf(outfile, "\t\t %s=-%s;\n", name1, leftStr);
              break;                            
            case CALL_EXPR:
	      fprintf(outfile, "\t\t /* call: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              leftStr = NODE_NAME(NODE_LEFT(def));
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
              fprintf(outfile, "\t\t %sMyFloatA(_jz_zvar1,%s);\n", leftStr, rightStr);
              fprintf(outfile, "\t\t MyFloatToInt(%s, _jz_zvar1);\n",name1);
              break;                            
            case IF_ELSE_EXPR:
	      fprintf(outfile, "\t\t /* if_else: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              {
                char *condstr = genNodeRvalueI(IF_ELSE_COND(def));
                leftStr = genNodeRvalueI(NODE_LEFT(def));
                rightStr = genNodeRvalueI(NODE_RIGHT(def));
                fprintf(outfile, "\t\t %s=(%s)?(%s):(%s);\n", name1, condstr, leftStr, rightStr);
              }              
              break;
            case GE_EXPR:
	      fprintf(outfile, "\t\t /* GE: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));              
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case GT_EXPR:
	      fprintf(outfile, "\t\t /* GT: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case LE_EXPR:
	      fprintf(outfile, "\t\t /* LE: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case LT_EXPR:
	      fprintf(outfile, "\t\t /* LT: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case EQ_EXPR:
	      fprintf(outfile, "\t\t /* EQ: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case NEQ_EXPR:
	      fprintf(outfile, "\t\t /* NEQ: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case AND_EXPR:
	      fprintf(outfile, "\t\t /* AND: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
            case OR_EXPR:
	      fprintf(outfile, "\t\t /* OR: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
              outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
              break;
	    }
	}
    }
  fprintf(outfile, "\t   }\n\t } /*end sum block */\n");
}
/* compute the sum */
void sumUptoVar(SumP sum, Node var)
{
  Node expr = SUMB_EXPR(sum);
  int  from = SUMB_FROM(sum);
  int  to   = SUMB_TO(sum);
  int  ss = to-from+1;

  char *vname = genGlobalVarLvalue(var);
  char *lname = genLocalVarLvalue(expr);
  fprintf(outfile, "\t {/*add up sum block*/\n");
  fprintf(outfile, "\t    static MY_FLOAT tmpFloat, tmpA;\n");
  fprintf(outfile, "\t    if(_jz_initialized==0) { InitMyFloat(tmpA); InitMyFloat(tmpFloat);}\n");
  fprintf(outfile, "\t    AssignMyFloat(tmpFloat, _jz_MyFloatZERO);\n");
  fprintf(outfile, "\t    for(_jz_m = 0; _jz_m < %d; _jz_m++) {\n", ss);
  fprintf(outfile, "\t\t AddMyFloatA(tmpA, tmpFloat, %s);\n", lname);
  fprintf(outfile, "\t\t AssignMyFloat(tmpFloat, tmpA);\n");
  fprintf(outfile, "\t    }\n"); 
  fprintf(outfile, "\t    AssignMyFloat(%s, tmpFloat);\n", vname);
  fprintf(outfile, "\t }/*end add up sum block*/\n\n");
}

/*************************************************************************************************
 *
 *  Output initial values for all vars except SUM_EXPR
 */
void  outputInitialVar(Node var)
{
  Node def = variables[ID_V_INDEX(var)].def;
  int  code = NODE_CODE(def);
  char *name2, *name1 = genGlobalVarLvalue(var);
  char *leftStr, *rightStr;
  Node numer = NULL, deno = NULL;
  int ratexp = 0;


  if( VAR_IS_GLOBAL_VAR(var) || VAR_IS_GLOBAL_CVAR(var))
    {
      switch(code)
	{
	case ID_NODE:
	  fprintf(outfile, "\t /* id: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  name2 = genGlobalVarLvalue(def);
	  if(VAR_IS_JET(var)) {
	    if(VAR_IS_JET(def)) {
	      fprintf(outfile, "\t AssignJetToJet(%s, %s);\n",name1,name2);	      
	    } else {
	      fprintf(outfile, "\t AssignFloatToJet(%s, %s);\n",name1,name2);	      	      
	    }
	  } else {
	    fprintf(outfile, "\t AssignMyFloat(%s, %s);\n",name1,name2);
	  }
	  break;
	case INT_CST:
	case FLOAT_CST:
	  fprintf(outfile, "\t /* const: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  fprintf(outfile, "\t MakeMyFloatC(%s,\"%s\",(double)%s);\n", name1, genCstFString(def), genCstFString(def));
	  break;        
	case ARRAY_REF: 
	  fprintf(outfile, "\t /* array_ref: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  fprintf(outfile, "\t AssignMyFloat(%s,%s);\n", name1, genArrayRef(def));
	  break;
	case PLUS_EXPR:
	  fprintf(outfile, "\t /* plus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueF(NODE_LEFT(def));
	  rightStr = genNodeRvalueF(NODE_RIGHT(def));
	  if(VAR_IS_JET(NODE_LEFT(def))) {
	    if(VAR_IS_JET(NODE_RIGHT(def))) {
	      fprintf(outfile, "\t AddJetJetA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    } else {
	      fprintf(outfile, "\t AddJetFloatA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    }
	  } else if(VAR_IS_JET(NODE_RIGHT(def))) {
	    fprintf(outfile, "\t AddFloatJetA(%s, %s, %s);\n", 
		    name1, leftStr, rightStr);
	  } else {
	    fprintf(outfile, "\t AddMyFloatA(%s, %s, %s);\n", 
		    name1, leftStr, rightStr);
	  }
	  break;
	case MINUS_EXPR:
	  fprintf(outfile, "\t /* minus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueF(NODE_LEFT(def));
	  rightStr = genNodeRvalueF(NODE_RIGHT(def));
	  if(VAR_IS_JET(NODE_LEFT(def))) {
	    if(VAR_IS_JET(NODE_RIGHT(def))) {
	      fprintf(outfile, "\t SubtractJetJetA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    } else {
	      fprintf(outfile, "\t SubtractJetFloatA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    }
	  } else if(VAR_IS_JET(NODE_RIGHT(def))) {
	    fprintf(outfile, "\t SubtractFloatJetA(%s, %s, %s);\n", 
		    name1, leftStr, rightStr);
	  } else {
	    fprintf(outfile, "\t SubtractMyFloatA(%s, %s, %s);\n", 
		    name1, leftStr, rightStr);
	  }
	  break;
	case MULT_EXPR:
	  fprintf(outfile, "\t /* mult: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueF(NODE_LEFT(def));
	  rightStr = genNodeRvalueF(NODE_RIGHT(def));
	  if(VAR_IS_JET(NODE_LEFT(def))) {
	    if(VAR_IS_JET(NODE_RIGHT(def))) {
	      fprintf(outfile, "\t MultiplyJetJetA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    } else {
	      fprintf(outfile, "\t MultiplyJetFloatA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    }
	  } else if(VAR_IS_JET(NODE_RIGHT(def))) {
	    fprintf(outfile, "\t MultiplyFloatJetA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    
	  } else {
	    fprintf(outfile, "\t MultiplyMyFloatA(%s, %s, %s);\n", 
		    name1, leftStr, rightStr);
	  }
	  break;
	case DIV_EXPR:
	  fprintf(outfile, "\t /* div: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueF(NODE_LEFT(def));
	  rightStr = genNodeRvalueF(NODE_RIGHT(def));
	  if(VAR_IS_JET(NODE_LEFT(def))) {
	    if(VAR_IS_JET(NODE_RIGHT(def))) {
	      fprintf(outfile, "\t DivideJetJetA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);
	    } else {
	      fprintf(outfile, "\t DivideJetFloatA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);	      
	    }
	  } else if(VAR_IS_JET(NODE_RIGHT(def))) {
	    fprintf(outfile, "\t DivideFloatJetA(%s, %s, %s);\n", 
		      name1, leftStr, rightStr);

	  } else {
	    fprintf(outfile, "\t DivideMyFloatA(%s, %s, %s);\n", 
		    name1, leftStr, rightStr);
	  }
	  break;
	case EXP_EXPR:           
	  fprintf(outfile, "\t /* exponentiation: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueF(NODE_LEFT(def));
          ratexp = isRational(NODE_RIGHT(def), &numer, &deno);
	  if(ratexp && (ratexp == 8 || ratexp == -8)) { if(use_rational_exponent) { ratexp=(ratexp==8? 1 : -1);}}
          if(ratexp == 1 || ratexp == -1)
            {
              int spp=1, jk, pp = intNodeValue(numer,&jk);
              int qq = intNodeValue(deno,&jk);
              double expo;
              if(ratexp < 0) qq = -qq;
              if(abs(qq) <= 2) {
                    /* integer or half integer exponent */
                    fprintf(outfile, "\t\t  /* integer exponent or half integer */\n");
                    pp = pp * qq;
                    if(pp < 0) { spp = -1; pp=-pp;}
                    if(abs(qq) == 2) {  /* half integer */
                      pp /= 2;
                      if(spp == -1) {
			if(VAR_IS_JET(NODE_LEFT(def))) {
			  fprintf(outfile, "\t\t sqrtJetA(_sjz_svar1,%s);\n", leftStr);                      
			  fprintf(outfile, "\t\t DivideFloatJetA(_sjz_svar5,_jz_oneOverN[0],_sjz_svar1);\n");                      
			} else {
			  fprintf(outfile, "\t\t sqrtMyFloatA(_jz_svar1,%s);\n", leftStr);                      
			  fprintf(outfile, "\t\t DivideMyFloatA(_jz_svar5,_jz_oneOverN[0],_jz_svar1);\n");                      
			}
                      }
                      else
			{
			  if(VAR_IS_JET(NODE_LEFT(def))) {
			    fprintf(outfile, "\t\t sqrtJetA(_sjz_svar5,%s);\n", leftStr);                      
			  } else {
			    fprintf(outfile, "\t\t sqrtMyFloatA(_jz_svar5,%s);\n", leftStr);                      
			  }
			}
                    } else { /* integer */
                      if(spp == -1)
			{
			  if(VAR_IS_JET(NODE_LEFT(def))) {
			    fprintf(outfile, "\t\t DivideFloatJetA(_sjz_svar5,_jz_oneOverN[0],%s);\n", leftStr);                      
			  } else {
			    fprintf(outfile, "\t\t DivideMyFloatA(_jz_svar5,_jz_oneOverN[0],%s);\n", leftStr);                      
			  }
			}
                      else
			{
			  if(VAR_IS_JET(NODE_LEFT(def))) {
			    fprintf(outfile, "\t\t AssignJetToJet(_sjz_svar5,%s);\n", leftStr); 
			  } else {
			    fprintf(outfile, "\t\t AssignMyFloat(_jz_svar5,%s);\n", leftStr); 
			  }
			}
                    }
                    fprintf(outfile, "\t\t { int n=%d, m, mn=0; \n", pp);
                    fprintf(outfile, "\t\t   switch(n) {\n");
		    if(VAR_IS_JET(NODE_LEFT(def))) {
		      fprintf(outfile, "\t\t\t  case 0: AssignFloatToJet(%s, _jz_oneOverN[0]); break;\n", name1);
		      fprintf(outfile, "\t\t\t  case 1: AssignJetToJet(%s, _sjz_svar5); break;\n", name1);
 		      fprintf(outfile, "\t\t\t  case 2: AssignJetToJet(_sjz_svar1, _sjz_svar5); MultiplyJetJetA(%s,_sjz_svar1,_sjz_svar5); break;\n",name1);
                      fprintf(outfile, "\t\t\t  case 3: AssignJetToJet(_sjz_svar1, _sjz_svar5); MultiplyJetJetA(_sjz_svar2,_sjz_svar1,_sjz_svar5);\n");
                      fprintf(outfile, "\t\t\t          MultiplyJetJetA(%s,_sjz_svar1,_sjz_svar2); break;\n", name1);
                      fprintf(outfile, "\t\t\t  default:\n");
                      fprintf(outfile, "\t\t\t   AssignFloatToJet(_sjz_svar1, _jz_oneOverN[0]); AssignJetToJet(_sjz_svar2, _sjz_svar5);\n");
                      fprintf(outfile, "\t\t\t     while(mn==0) {\n");
                      fprintf(outfile, "\t\t\t        m=n; n /=2; if(n+n != m) {\n");
                      fprintf(outfile, "\t\t\t           AssignJetToJet(_sjz_svar3, _sjz_svar1); MultiplyJetJetA(_sjz_svar1, _sjz_svar3, _sjz_svar2);\n");
                      fprintf(outfile, "\t\t\t           if(n==0){ mn=1; ");
                      fprintf(outfile, "    AssignJetToJet(%s,_sjz_svar1);}\n",name1);
                      fprintf(outfile, "\t\t\t         }\n");
                      fprintf(outfile, "\t\t\t        if(mn==0) {AssignJetToJet(_sjz_svar3, _sjz_svar2);AssignJetToJet(_sjz_svar4, _sjz_svar2);\n");
                      fprintf(outfile, "\t\t\t                   MultiplyJetJetA(_sjz_svar2, _sjz_svar3,_sjz_svar4);}\n");
                      fprintf(outfile, "\t\t\t       }\n");
                      fprintf(outfile, "\t\t\t   break; \n");
                      fprintf(outfile, "\t\t\t  }\n\t\t } \n");

		    } else {
		      fprintf(outfile, "\t\t\t  case 0: AssignMyFloat(%s, _jz_oneOverN[0]); break;\n", name1);
		      fprintf(outfile, "\t\t\t  case 1: AssignMyFloat(%s, _jz_svar5); break;\n", name1);
 		      fprintf(outfile, "\t\t\t  case 2: AssignMyFloat(_jz_svar1, _jz_svar5); MultiplyMyFloatA(%s,_jz_svar1,_jz_svar5); break;\n",name1);
                      fprintf(outfile, "\t\t\t  case 3: AssignMyFloat(_jz_svar1, _jz_svar5); MultiplyMyFloatA(_jz_svar2,_jz_svar1,_jz_svar5);\n");
                      fprintf(outfile, "\t\t\t          MultiplyMyFloatA(%s,_jz_svar1,_jz_svar2); break;\n", name1);
                      fprintf(outfile, "\t\t\t  default:\n");
                      fprintf(outfile, "\t\t\t   AssignMyFloat(_jz_svar1, _jz_oneOverN[0]); AssignMyFloat(_jz_svar2, _jz_svar5);\n");
                      fprintf(outfile, "\t\t\t     while(mn==0) {\n");
                      fprintf(outfile, "\t\t\t        m=n; n /=2; if(n+n != m) {\n");
                      fprintf(outfile, "\t\t\t           AssignMyFloat(_jz_svar3, _jz_svar1); MultiplyMyFloatA(_jz_svar1, _jz_svar3, _jz_svar2);\n");
                      fprintf(outfile, "\t\t\t           if(n==0){ mn=1; ");
                      fprintf(outfile, "    AssignMyFloat(%s,_jz_svar1);}\n",name1);
                      fprintf(outfile, "\t\t\t         }\n");
                      fprintf(outfile, "\t\t\t        if(mn==0) {AssignMyFloat(_jz_svar3, _jz_svar2);AssignMyFloat(_jz_svar4, _jz_svar2);\n");
                      fprintf(outfile, "\t\t\t                   MultiplyMyFloatA(_jz_svar2, _jz_svar3,_jz_svar4);}\n");
                      fprintf(outfile, "\t\t\t       }\n");
                      fprintf(outfile, "\t\t\t   break; \n");
                      fprintf(outfile, "\t\t\t  }\n\t\t } \n");
		    }
              } else { /* fishy here */
                    expo = (double)(pp)/(double)qq;
		    if(VAR_IS_JET(NODE_LEFT(def))) {
		      /*zzzzzzzz*/
		    } else {
                    fprintf(outfile, "\t\t if(MyFloatA_GT_B(%s,_jz_MyFloatZERO)) { \n", leftStr);
		    if(icnt2&1)fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, %s, MakeMyFloatB(_jz_tvar2,%.15f));\n", name1, leftStr, expo);
		    else fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, %s, MakeMyFloatB(_jz_uvar2,%.15f));\n", name1, leftStr, expo);
		    icnt2++;
                    fprintf(outfile, "\t\t } else if(MyFloatA_LT_B(%s,_jz_MyFloatZERO)) {\n", leftStr);
                    if( qq & 1) { /* odd root, ok */
                      if(pp & 1) { /* pp is odd */
                        fprintf(outfile, "\t\t    NegateMyFloatA(_jz_svar1, %s);\n", leftStr);
			if(icnt2&1)fprintf(outfile, "\t\t    ExponentiateMyFloatA(_jz_svar2, _jz_svar1, MakeMyFloatB(_jz_tvar2,%.15f));\n", expo);
			else  fprintf(outfile, "\t\t    ExponentiateMyFloatA(_jz_svar2, _jz_svar1, MakeMyFloatB(_jz_uvar2,%.15f));\n", expo);
			icnt2++;
                        fprintf(outfile, "\t\t    NegateMyFloatA(%s,_jz_svar2);\n", name1);
                      } else { /* pp is even */
                        fprintf(outfile, "\t\t    NegateMyFloatA(_jz_svar1, %s);\n", leftStr);
                        if(icnt2&1)fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, _jz_svar1, MakeMyFloatB(_jz_tvar2,%.15f));\n", name1, expo);
			else fprintf(outfile, "\t\t    ExponentiateMyFloatA(%s, _jz_svar1, MakeMyFloatB(_jz_uvar2,%.15f));\n", name1, expo);
			icnt2++;
                      }
                    } else { /* qq is even: ?^(pp/qq) is undefiled */
                      fprintf(outfile, "\t\t   fprintf(stderr, \"Attemp to compute negative^(%d/%d)\");exit(0);\n",pp,qq);
                    }
                    fprintf(outfile, "\t\t } else {\n");                    
                    if(expo > 0.0) { fprintf(outfile, "\t\t    AssignMyFloat(%s,_jz_oneOverN[0]);\n\t\t }\n", name1);}
                    else { fprintf(outfile, "\t\t   fprintf(stderr,\"Attemp to compute 0^(%d/%d)\");exit(0);\n\t\t}\n",pp,qq);}
		    }
              }
            }
          else
            {
              rightStr = genNodeRvalueF(NODE_RIGHT(def));
	      if(VAR_IS_JET(NODE_LEFT(def))) {
		fprintf(outfile, "\t ExponentiateJetCstA(%s, %s, %s);\n", 
			name1, leftStr, rightStr);
	      } else {
		fprintf(outfile, "\t ExponentiateMyFloatA(%s, %s, %s);\n", 
			name1, leftStr, rightStr);
	      }
            }
	  break;              
	case NEGATE_EXPR:
	  fprintf(outfile, "\t /* negate: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueF(NODE_LEFT(def));
	  if(VAR_IS_JET(NODE_LEFT(def))) {
	    fprintf(outfile, "\t NegateJetA(%s,%s);\n", 
		    name1, leftStr);
	  } else {
	    fprintf(outfile, "\t NegateMyFloatA(%s,%s);\n", 
		    name1, leftStr);
	  }
	  break;                            
	case CALL_EXPR:
	  fprintf(outfile, "\t /* call: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = NODE_NAME(NODE_LEFT(def));
	  rightStr = genNodeRvalueF(NODE_RIGHT(def));
	  if(VAR_IS_JET(NODE_RIGHT(def))) {
	    fprintf(outfile, "\t %sJetA(%s, %s);\n", 
		    leftStr, name1, rightStr);	    
	  } else {
	    fprintf(outfile, "\t %sMyFloatA(%s, %s);\n", 
		    leftStr, name1, rightStr);
	  }
	  break;                            
	case IF_ELSE_EXPR:
	  fprintf(outfile, "\t /* if_else: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  {
	    char *condstr = genNodeRvalueI(IF_ELSE_COND(def));
	    leftStr = genNodeRvalueF(NODE_LEFT(def));
	    rightStr = genNodeRvalueF(NODE_RIGHT(def));
	    fprintf(outfile, "\t if(%s)\n\t\t {AssignMyFloat(%s,%s);}\n\t else\n\t\t {AssignMyFloat(%s,%s);}\n",
		    condstr, name1, leftStr, name1, rightStr);
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
	  {
	    fprintf(stderr, "Internal Error: bool expr appeared in wrong place.\n");
	    exit(43);
	  }
	break;
	}	  
    }
  else if(VAR_IS_GLOBAL_IVAR(var))
    {
      switch(code)
	{
	case ID_NODE:
	  fprintf(outfile, "\t /* id: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  name2 = genGlobalVarLvalue(var);
	  fprintf(outfile, "\t %s=(int)%s;\n",name1,name2);
	  break;
	case INT_CST:
	case FLOAT_CST:
	  fprintf(outfile, "\t /* const: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  fprintf(outfile, "\t %s=%s;\n", name1, genCstIString(def));
	  break;  
	case ARRAY_REF: 
	  fprintf(outfile, "\t /* array_ref: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  fprintf(outfile, "\t %s=(int)%s;\n", name1, genArrayRef(def));
	  break;
	case PLUS_EXPR:
	  fprintf(outfile, "\t /* plus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueI(NODE_LEFT(def));
	  rightStr = genNodeRvalueI(NODE_RIGHT(def));
	  fprintf(outfile, "\t %s=%s+%s;\n",name1, leftStr, rightStr);
	  break;
	case MINUS_EXPR:
	  fprintf(outfile, "\t /* minus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueI(NODE_LEFT(def));
	  rightStr = genNodeRvalueI(NODE_RIGHT(def));
	  fprintf(outfile, "\t %s=%s-%s;\n",name1, leftStr, rightStr);
	  break;
	case MULT_EXPR:
	  fprintf(outfile, "\t /* mult: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueI(NODE_LEFT(def));
	  rightStr = genNodeRvalueI(NODE_RIGHT(def));
	  fprintf(outfile, "\t %s=%s*%s;\n",name1, leftStr, rightStr);
	  break;
	case DIV_EXPR:
	  fprintf(outfile, "\t /* div: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueI(NODE_LEFT(def));
	  rightStr = genNodeRvalueI(NODE_RIGHT(def));
	  fprintf(outfile, "\t %s=%s/%s;\n",name1, leftStr, rightStr);
	  break;
	case EXP_EXPR:           
	  fprintf(outfile, "\t /* exponentiation: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueI(NODE_LEFT(def));
          ratexp = isRational(NODE_RIGHT(def), &numer, &deno);
	  if(ratexp && (ratexp == 8 || ratexp == -8)) { if(use_rational_exponent) { ratexp=(ratexp==8? 1 : -1);}}
          if(ratexp == 1 || ratexp == -1)
            {
              int jk, pp = intNodeValue(numer,&jk);
              int qq = intNodeValue(deno,&jk);
              double expo;
              if(ratexp < 0) qq = -qq;
              expo = (double)(pp)/(double)qq;
              fprintf(outfile, "\t { double base;\n");
              //fprintf(outfile, "\t FloatToDouble(base,%s);\n", leftStr);
              fprintf(outfile, "\t  base = (double)(%s);\n", leftStr);
              fprintf(outfile, "\t  %s=(int)pow(base, %.15f);\n", 
                      name1, expo);
              fprintf(outfile, "\t } \n");
            }
          else
            {
              rightStr = genNodeRvalueI(NODE_RIGHT(def));
              fprintf(outfile, "\t {double base, expo;\n");
              //fprintf(outfile, "\t MyFloatToDouble(base, %s);\n", leftStr);
              //fprintf(outfile, "\t MyFloatToDouble(expo, %s);\n", rightStr);
              fprintf(outfile, "\t  base = (double)(%s);\n", leftStr);
              fprintf(outfile, "\t  expo = (double)(%s);\n", rightStr);
              fprintf(outfile, "\t  %s=(int)(pow(base, expo));\n",name1);
              fprintf(outfile, "\t}\n");
            }
	  break;              
	case NEGATE_EXPR:
	  fprintf(outfile, "\t /* negate: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = genNodeRvalueI(NODE_LEFT(def));
	  fprintf(outfile, "\t %s=-%s;\n", name1, leftStr);
	  break;                            
	case CALL_EXPR:
	  fprintf(outfile, "\t /* call: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  leftStr = NODE_NAME(NODE_LEFT(def));
	  rightStr = genNodeRvalueF(NODE_RIGHT(def));
	  fprintf(outfile, "\t %sMyFloatA(_jz_zvar1,%s);\n", leftStr, rightStr);
	  fprintf(outfile, "\t MyFloatToInt(_jz_zvar1,%s);\n", name1);
	  break;                            
	case IF_ELSE_EXPR:
	  fprintf(outfile, "\t /* if_else: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
	  {
	    char *condstr = genNodeRvalueI(IF_ELSE_COND(def));
	    leftStr = genNodeRvalueI(NODE_LEFT(def));
	    rightStr = genNodeRvalueI(NODE_RIGHT(def));
	    fprintf(outfile, "\t %s=(%s)?(%s):(%s);\n", name1, condstr, leftStr, rightStr);
	  }              
	break;
        case GE_EXPR:
	  fprintf(outfile, "\t /* GE: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case GT_EXPR:
	  fprintf(outfile, "\t /* GT: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case LE_EXPR:
	  fprintf(outfile, "\t /* LE: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case LT_EXPR:
	  fprintf(outfile, "\t /* LT: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case EQ_EXPR:
	  fprintf(outfile, "\t /* EQ: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case NEQ_EXPR:
	  fprintf(outfile, "\t /* NEQ: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case AND_EXPR:
	  fprintf(outfile, "\t /* AND: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        case OR_EXPR:
	  fprintf(outfile, "\t /* OR: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
          outputBoolExpr(code, NODE_LEFT(def), NODE_RIGHT(def), name1);
          break;
        }
    }
  else
    {
      fprintf(stderr, "Internal Error: gvar %s has an illegal type.\n", name1);
      exit(44);
    }
}

/*************************************************************************************************
 *
 *  Output the kth order derivatives of local vars in a sum block
 */

void outputSumKthDerivative(SumP sum)
{
  Node idx  = SUMB_IDX(sum);
  Node *vvars  = SUMB_LVAR_LIST(sum);
  int  from = SUMB_FROM(sum);
  int  to   = SUMB_TO(sum);
  int  total = SUMB_LVAR(sum);
  int  k;
   
  fprintf(outfile, "\n\t    /*begin kth deri of a sum block*/\n");
  fprintf(outfile, "\t    {\n\t       register int %s;\n", NODE_NAME(idx));
  fprintf(outfile, "\t       for(%s = %d, _jz_m = 0; %s <= %d; %s += 1, _jz_m++) {\n", 
	  NODE_NAME(idx), from, NODE_NAME(idx), to, NODE_NAME(idx));
  for(k=0; k < total; k++)
    {
      Node var = vvars[k];
      Node def = variables[ID_V_INDEX(var)].def;
      computeKthDeri(var,def);
    }
  fprintf(outfile, "\t       }\n\t    }/* end of sum block, kth */\n");
}

void sumUptoVarKth(SumP sum, Node var)
{
  Node expr = SUMB_EXPR(sum);
  int  from = SUMB_FROM(sum);
  int  to   = SUMB_TO(sum);
  int  ss = to-from+1;

  char *vname = genVarLvalueKth(var, "_jz_k");
  char *lname = genVarLvalueKth(expr,"_jz_k");
  fprintf(outfile, "\t\t {/*add up deriv of a sum block*/\n");
  fprintf(outfile, "\t\t    static MY_FLOAT tmpFloat, tmpA;\n");
  fprintf(outfile, "\t\t    if(_jz_initialized==0) { InitMyFloat(tmpA); InitMyFloat(tmpFloat);}\n");
  fprintf(outfile, "\t\t    AssignMyFloat(tmpFloat, _jz_MyFloatZERO);\n");
  fprintf(outfile, "\t\t    for(_jz_m = 0; _jz_m < %d; _jz_m++) {\n", ss);
  fprintf(outfile, "\t\t\t AddMyFloatA(tmpA, tmpFloat, %s);\n", lname);
  fprintf(outfile, "\t\t\t AssignMyFloat(tmpFloat, tmpA);\n");
  fprintf(outfile, "\t\t    }\n"); 
  fprintf(outfile, "\t\t    AssignMyFloat(%s, tmpFloat);\n", vname);
  fprintf(outfile, "\t\t }/*end add up deriv of sum block*/\n\n");
}
/*************************************************************************************************
 *
 *  Output the kth order derivative of global vars except SUM_EXPR
 */
void outputVarKthDerivative(Node var)
{
  Node def = variables[ID_V_INDEX(var)].def;
  computeKthDeri(var,def);
}


void computeKthDeri(Node var, Node def)
{
  char *leftStr, *rightStr, *name1, *name2;  
  Node  larg, left = NODE_LEFT(def), right = NODE_RIGHT(def);
  int   ddflag, mmflag, code = NODE_CODE(def);
  name1 = genVarLvalueKth(var, "_jz_k");

  if(NODE_IS_CST(var) || NODE_IS_CST(def))
    {
      fprintf(outfile, "\t\t /* constants: %s=%s ! */\n", NODE_NAME(var), NODE_INFO(def));
      if(VAR_IS_JET(var)) {
	fprintf(outfile, "\t\t AssignFloatToJet(%s, _jz_MyFloatZERO);\n", name1);
      } else {
	fprintf(outfile, "\t\t AssignMyFloat(%s, _jz_MyFloatZERO);\n", name1);
      }
      return;
    }

  switch(code)
    {
    case INT_CST:
    case FLOAT_CST:
      fprintf(outfile, "\t\t /* constants: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      if(VAR_IS_JET(var)) {
	fprintf(outfile, "\t\t AssignFloatToJet(%s, _jz_MyFloatZERO);\n", name1);
      } else {
	fprintf(outfile, "\t\t AssignMyFloat(%s, _jz_MyFloatZERO);\n", name1);
      }
      break;
    case ID_NODE:
      fprintf(outfile, "\t\t /* id: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      if(ID_IS_CST(def) || ID_IS_EXTERN(def)) {
	if(VAR_IS_JET(var)) {
	  fprintf(outfile, "\t\t AssignFloatToJet(%s, _jz_MyFloatZERO);\n", name1);
	} else {
	  fprintf(outfile, "\t\t AssignMyFloat(%s, _jz_MyFloatZERO);\n", name1);
	}
      }
      else
	{
	  name2 = genNodeKthDerivative(def, "_jz_k");
	  if(VAR_IS_JET(var)) {
	    if(VAR_IS_JET(def)) {
	      fprintf(outfile, "\t\t AssignJetToJet(%s, %s);\n", name1, name2);
	    } else {
	      fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, name2);
	    }
	  } else {
	    fprintf(outfile, "\t\t AssignMyFloat(%s, %s);\n", name1, name2);
	  }
	}
      break;
    case PLUS_EXPR:
      fprintf(outfile, "\t\t /* plus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      if(NODE_IS_CST(left))
	{
	  name2 = genNodeKthDerivative(right, "_jz_k");
	  if(VAR_IS_JET(var)) {	  
	    if(VAR_IS_JET(right)) {
	      fprintf(outfile, "\t\t AssignJetToJet(%s, %s);\n", name1, name2);
	    } else {
	      fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, name2);
	    }
	  } else {
	    fprintf(outfile, "\t\t AssignMyFloat(%s, %s);\n", name1, name2);
	  }
	}
      else if(NODE_IS_CST(right))
	{
	  name2 = genNodeKthDerivative(left, "_jz_k");
	  if(VAR_IS_JET(var)) {	  
	    if(VAR_IS_JET(left)) {
	      fprintf(outfile, "\t\t AssignJetToJet(%s, %s);\n", name1, name2);
	    } else {
	      fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, name2);	      
	    }
	  } else {
 	    fprintf(outfile, "\t\t AssignMyFloat(%s, %s);\n", name1, name2);
	  }
	}
      else
	{
	  leftStr = genNodeKthDerivative(left, "_jz_k");
	  rightStr = genNodeKthDerivative(right, "_jz_k");
	  if(VAR_IS_JET(var)) {	 
	    if(VAR_IS_JET(left)) { 
	      if(VAR_IS_JET(right)) { 
		fprintf(outfile, "\t\t AddJetJetA(%s, %s,%s);\n", name1, leftStr, rightStr);
	      } else {
		fprintf(outfile, "\t\t AddJetFloatA(%s, %s,%s);\n", name1, leftStr, rightStr);
	      }
	    } else if(VAR_IS_JET(right)) { 
	      fprintf(outfile, "\t\t AddFloatJetA(%s, %s,%s);\n", name1, leftStr, rightStr);
	    } else {
	      fprintf(outfile, "\t\t AddMyFloatA(%s, %s,%s);\n", "_kthStmp", leftStr, rightStr);	      
	      fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthStmp");
	    }
	  } else {
	    fprintf(outfile, "\t\t AddMyFloatA(%s, %s,%s);\n", name1, leftStr, rightStr);
	  }
	}
      break;
    case MINUS_EXPR:
      fprintf(outfile, "\t\t /* minus: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      if(NODE_IS_CST(left))
	{
	  name2 = genNodeKthDerivative(right, "_jz_k");
	  if(VAR_IS_JET(var)) {	 
	    if(VAR_IS_JET(right)) {
	      fprintf(outfile, "\t\t NegateJetA(%s, %s);\n", name1, name2);
	    } else {
	      fprintf(outfile, "\t\t NegateFloatToJetA(%s, %s);\n", name1, name2);
	    }
	  } else {	  
	    fprintf(outfile, "\t\t NegateMyFloatA(%s, %s);\n", name1, name2);
	  }
	}
      else if(NODE_IS_CST(right))
	{
	  name2 = genNodeKthDerivative(left, "_jz_k");
	  if(VAR_IS_JET(var)) {	 
	    if(VAR_IS_JET(left)) {
	      fprintf(outfile, "\t\t AssignJetToJet(%s, %s);\n", name1, name2);
	    } else {
	      fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, name2);	      
	    }
	  } else {
	    fprintf(outfile, "\t\t AssignMyFloat(%s, %s);\n", name1, name2);
	  }
	}
      else
	{
	  leftStr = genNodeKthDerivative(left, "_jz_k");
	  rightStr = genNodeKthDerivative(right, "_jz_k");
	  if(VAR_IS_JET(var)) {	 
	    if(VAR_IS_JET(left)) { 
	      if(VAR_IS_JET(right)) { 
		fprintf(outfile, "\t\t SubtractJetJetA(%s, %s,%s);\n", name1, leftStr, rightStr);
	      } else {
		fprintf(outfile, "\t\t SubtractJetFloatA(%s, %s,%s);\n", name1, leftStr, rightStr);
	      }
	    } else if(VAR_IS_JET(right)) { 
	      fprintf(outfile, "\t\t SubtractFloatJetA(%s, %s,%s);\n", name1, leftStr, rightStr);
	    } else {
	      fprintf(outfile, "\t\t SubtractMyFloatA(%s, %s,%s);\n", "_kthtmp", leftStr, rightStr);	      
	      fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthtmp");
	    }
	  } else {
	    fprintf(outfile, "\t\t SubtractMyFloatA(%s, %s,%s);\n", name1, leftStr, rightStr);
	  }
	}
      break;
    case MULT_EXPR:
      fprintf(outfile, "\t\t /* mult: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));

      if(NODE_IS_CST(left))
	{
	  leftStr = genNodeRvalueF(left);
	  rightStr = genNodeKthDerivative(right, "_jz_k");
          larg = linearTime(right, &ddflag, &mmflag);
	  if(larg != NULL)
	    {
              if(larg == int_one_node)
                {
		  if(VAR_IS_JET(var)) { 
		    fprintf(outfile, "\t\t if(_jz_k==1) {AssignFloatToJet(%s, %s);}\n", name1, leftStr);	      
		    fprintf(outfile, "\t\t else {AssignFloatToJet(%s, _jz_MyFloatZERO);}\n", name1);  		    
		  } else {
		    fprintf(outfile, "\t\t if(_jz_k==1) {AssignMyFloat(%s, %s);}\n", name1, leftStr);	      
		    fprintf(outfile, "\t\t else {AssignMyFloat(%s, _jz_MyFloatZERO);}\n", name1);  
		  }
                }
              else
                {
		  if(VAR_IS_JET(var)) { 
		    fprintf(outfile, "\t\t if(_jz_k==1) { MultiplyMyFloatA(%s, %s, %s);\n", "_kthtmp", leftStr, rightStr);
		    fprintf(outfile, "\t\t                AssignFloatToJet(%s, %s);}\n", name1, "_kthtmp");
		    fprintf(outfile, "\t\t else {AssignFloatToJet(%s, _jz_MyFloatZERO);}\n", name1);  
		  } else {
		    fprintf(outfile, "\t\t if(_jz_k==1) { MultiplyMyFloatA(%s, %s, %s);}\n", name1, leftStr, rightStr);
		    fprintf(outfile, "\t\t else {AssignMyFloat(%s, _jz_MyFloatZERO);}\n", name1);  
		  }
                }
	    }
	  else
	    {
	      if(VAR_IS_JET(var)) { 
		if(VAR_IS_JET(right)) { 
		  fprintf(outfile, "\t\t MultiplyFloatJetA(%s, %s, %s);\n", name1, leftStr, rightStr);
		} else {
		  fprintf(outfile, "\t\t MultiplyMyFloatA(%s, %s, %s);\n", "_kthtmp", leftStr, rightStr);
		  fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthtmp");  
		}
	      } else {
		fprintf(outfile, "\t\t MultiplyMyFloatA(%s, %s, %s);\n", name1, leftStr, rightStr);
	      }
	    }
	}
      else if(NODE_IS_CST(right))
	{
	  rightStr = genNodeRvalueF(right);
	  leftStr = genNodeKthDerivative(left, "_jz_k");
          larg = linearTime(right, &ddflag, &mmflag);
	  if(larg != NULL)
            {    
              if(larg == int_one_node)
                {
		  if(VAR_IS_JET(var)) { 
		    fprintf(outfile, "\t\t if(_jz_k==1) {AssignFloatToJet(%s, %s);}\n", name1, rightStr);	      
		    fprintf(outfile, "\t\t else {AssignFloatToJet(%s, _jz_MyFloatZERO);}\n", name1);  		    
		  } else {
		    fprintf(outfile, "\t\t if(_jz_k==1) {AssignMyFloat(%s, %s);}\n", name1, rightStr);	      
		    fprintf(outfile, "\t\t else {AssignMyFloat(%s, _jz_MyFloatZERO);}\n", name1);  
		  }
                }      
              else
                {
		  if(VAR_IS_JET(var)) { 
		    fprintf(outfile, "\t\t if(_jz_k==1) { MultiplyMyFloatA(%s, %s, %s);\n", "_kthtmp", leftStr, rightStr);
		    fprintf(outfile, "\t\t                AssignFloatToJet(%s, %s);}\n", name1, "_kthtmp");
		    fprintf(outfile, "\t\t else {AssignFloatToJet(%s, _jz_MyFloatZERO);}\n", name1);  
		  } else {
		    fprintf(outfile, "\t\t if(_jz_k==1) { MultiplyMyFloatA(%s, %s, %s);}\n", name1, leftStr, rightStr);
		    fprintf(outfile, "\t\t else {AssignMyFloat(%s, _jz_MyFloatZERO);}\n", name1);  
		  }
                }
	    }
          else
            {
	      if(VAR_IS_JET(var)) { 
		if(VAR_IS_JET(left)) { 		
		  fprintf(outfile, "\t\t MultiplyJetFloatA(%s, %s, %s);\n", name1, leftStr, rightStr);
		} else {
		  fprintf(outfile, "\t\t MultiplyMyFloatA(%s, %s, %s);\n", "_kthtmp", leftStr, rightStr);
		  fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthtmp");  
		}
	      } else {
		fprintf(outfile, "\t\t MultiplyMyFloatA(%s, %s, %s);\n", name1, leftStr, rightStr);
	      }
            }
	}
      else if(left == right)
	{
	  /* optimization:  x*x */
	  char *lstr, *rstr;
	  fprintf(outfile, "\t\t { /* multiplication */\n");
	  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
	  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp)\n");	  
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp;\n");
            fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp)\n");
	  }
	  fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
	  fprintf(outfile, "\t\t\t   InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);\n");
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t   InitJet(stmp1);InitJet(stmp2); InitJet(stmp);\n");
	  }
	  fprintf(outfile,"\t\t\t   }\n");
	  if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
	    {
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
	      lstr = genNodeKthDerivative(left,  "0");
	      rstr = genNodeKthDerivative(right, "_jz_k");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
	      lstr = genNodeKthDerivative(left,  "1");
	      rstr = genNodeKthDerivative(right, "_jz_k-1");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,rstr);
	      fprintf(outfile, "\t\t\t AddMyFloatA(tmp, tmp2, tmp1);\n");
	      if(VAR_IS_JET(var)) { 	      
		fprintf(outfile, "\t\t\t AssignFloatToJet(%s, tmp);\n", name1);
	      } else {
		fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp);\n", name1);
	      }
	      fprintf(outfile, "\t\t }\n");
	    }
	  else
	    {
	      if(VAR_IS_JET(var)) {
		if(VAR_IS_JET(left)) {
		  fprintf(outfile, "\t\t\t int parity=(_jz_k&1), half=(_jz_k+1)>>1;\n");
		  fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<half; _jz_l++) {\n");
		  lstr = genNodeKthDerivative(left,  "_jz_l");
		  rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
		  fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s, %s);\n",lstr,rstr);
		  fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		  fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  fprintf(outfile, "\t\t\t AssignJetToJet(stmp2, stmp);\n");
		  fprintf(outfile, "\t\t\t AddJetJetA(stmp1, stmp2, stmp);\n");
		  fprintf(outfile, "\t\t\t if(parity==0) {\n");	  
		  lstr = genNodeKthDerivative(left,  "half");
		  fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, %s, %s);\n",lstr,lstr);	  
		  fprintf(outfile, "\t\t\t     AddJetJetA(%s, stmp2, stmp1);\n",name1);
		  fprintf(outfile, "\t\t\t } else {\n");
		  fprintf(outfile, "\t\t\t     AssignJetToJet(%s, stmp1);\n", name1);
		  fprintf(outfile, "\t\t\t }\n");
		  fprintf(outfile, "\t\t }\n");
		} else {
		  fprintf(outfile, "\t\t\t int parity=(_jz_k&1), half=(_jz_k+1)>>1;\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<half; _jz_l++) {\n");
		  lstr = genNodeKthDerivative(left,  "_jz_l");
		  rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
		  fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp2, tmp1);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, tmp);\n");
		  fprintf(outfile, "\t\t\t AddMyFloatA(tmp1, tmp2, tmp);\n");
		  fprintf(outfile, "\t\t\t if(parity==0) {\n");	  
		  lstr = genNodeKthDerivative(left,  "half");
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,lstr);	  
		  fprintf(outfile, "\t\t\t     AddMyFloatA(%s, tmp2, tmp1);\n","_kthtmp");
		  fprintf(outfile, "\t\t\t     AssignFloatToJet(%s, %s);\n",name1, "_kthtmp");
		  fprintf(outfile, "\t\t\t } else {\n");
		  fprintf(outfile, "\t\t\t     AssignFloatToJet(%s, tmp1);\n", name1);
		  fprintf(outfile, "\t\t\t }\n");
		  fprintf(outfile, "\t\t }\n");
		}
	      } else {
		fprintf(outfile, "\t\t\t int parity=(_jz_k&1), half=(_jz_k+1)>>1;\n");
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<half; _jz_l++) {\n");
		lstr = genNodeKthDerivative(left,  "_jz_l");
		rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
		fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp2, tmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, tmp);\n");
		fprintf(outfile, "\t\t\t AddMyFloatA(tmp1, tmp2, tmp);\n");
		fprintf(outfile, "\t\t\t if(parity==0) {\n");	  
		lstr = genNodeKthDerivative(left,  "half");
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,lstr);	  
		fprintf(outfile, "\t\t\t     AddMyFloatA(%s, tmp2, tmp1);\n",name1);
		fprintf(outfile, "\t\t\t } else {\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(%s, tmp1);\n", name1);
		fprintf(outfile, "\t\t\t }\n");
		fprintf(outfile, "\t\t }\n");
	      }
	    }
	}
      else
	{
	  char *lstr, *rstr;
	  fprintf(outfile, "\t\t { /* multiplication */\n");
	  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
	  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp)\n");	  	  
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp;\n");
            fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp)\n");
	  }
	  fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
	  fprintf(outfile, "\t\t\t   InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);\n");
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t   InitJet(stmp1);InitJet(stmp2); InitJet(stmp);\n");
	  }
	  fprintf(outfile,"\t\t\t   }\n");
	  
	  if(VAR_IS_JET(var)) {
	    if(VAR_IS_JET(left)) {
	      if(VAR_IS_JET(right)) {
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
		/* optimization: don't do full loop if operand is linear */
		if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=1; _jz_l++) {\n");
		else if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
		  fprintf(outfile, "\t\t\t for(_jz_l=_jz_k-1; _jz_l<=_jz_k; _jz_l++) {\n");
		else fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=_jz_k; _jz_l++) {\n");
		lstr = genNodeKthDerivative(left,  "_jz_l");
		rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s, %s);\n",lstr,rstr);
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		fprintf(outfile, "\t\t\t AssignJetToJet(%s, stmp);\n", name1);
		fprintf(outfile, "\t\t }\n");
	      } else {
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
		/* optimization: don't do full loop if operand is linear */
		if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=1; _jz_l++) {\n");
		else if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
		  fprintf(outfile, "\t\t\t for(_jz_l=_jz_k-1; _jz_l<=_jz_k; _jz_l++) {\n");
		else fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=_jz_k; _jz_l++) {\n");
		lstr = genNodeKthDerivative(left,  "_jz_l");
		rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp1, %s, %s);\n",lstr,rstr);
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		fprintf(outfile, "\t\t\t AssignJetToJet(%s, stmp);\n", name1);
		fprintf(outfile, "\t\t }\n");
	      }
	    } else if(VAR_IS_JET(right)) {
	      fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
	      /* optimization: don't do full loop if operand is linear */
	      if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
		fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=1; _jz_l++) {\n");
	      else if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
		fprintf(outfile, "\t\t\t for(_jz_l=_jz_k-1; _jz_l<=_jz_k; _jz_l++) {\n");
	      else fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=_jz_k; _jz_l++) {\n");
	      lstr = genNodeKthDerivative(left,  "_jz_l");
	      rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
	      fprintf(outfile, "\t\t\t     MultiplyFloatJetA(stmp1, %s, %s);\n",lstr,rstr);
	      fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
	      fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      fprintf(outfile, "\t\t\t AssignJetToJet(%s, stmp);\n", name1);
	      fprintf(outfile, "\t\t }\n");
	    } else {
	      fprintf(stderr, "Should not be here, 'multiplication expr'\n");
	      exit(211);
	    }
	  } else {
	    fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
	    /* optimization: don't do full loop if operand is linear */
	    if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
	      fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=1; _jz_l++) {\n");
	    else if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
	      fprintf(outfile, "\t\t\t for(_jz_l=_jz_k-1; _jz_l<=_jz_k; _jz_l++) {\n");
	    else fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<=_jz_k; _jz_l++) {\n");
	    lstr = genNodeKthDerivative(left,  "_jz_l");
	    rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");
	    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
	    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
	    fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp2, tmp1);\n");
	    fprintf(outfile, "\t\t\t }\n");
	    fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp);\n", name1);
	    fprintf(outfile, "\t\t }\n");
	  }
	}
      break;
    case DIV_EXPR:
      fprintf(outfile, "\t\t /* div: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));

      if(NODE_IS_CST(right))
	{
	  leftStr = genNodeKthDerivative(left, "_jz_k");
	  rightStr = genNodeRvalueF(right);
	  if(left == timeVar)
	    {
	      if(icnt1&1) 
		{
		  if(VAR_IS_JET(var)) {
		    fprintf(outfile, "\t\t if(_jz_k==1) {DivideMyFloatA(%s, MakeMyFloatB(_jz_tvar1, 1), %s);\n", "_kthtmp", rightStr);
		    fprintf(outfile, "\t\t\t  AssignFloatToJet(%s, %s);}\n", name1, "_kthtmp");
		  } else {
		    fprintf(outfile, "\t\t if(_jz_k==1) {DivideMyFloatA(%s, MakeMyFloatB(_jz_tvar1, 1), %s);}\n", name1, rightStr);
		  }
		}
	      else
		{
		  if(VAR_IS_JET(var)) {
		    fprintf(outfile, "\t\t if(_jz_k==1) {DivideMyFloatA(%s, MakeMyFloatB(_jz_uvar1, 1), %s);\n", "_kthtmp", rightStr);
		    fprintf(outfile, "\t\t\t  AssignFloatToJet(%s, %s);}\n", name1, "_kthtmp");
		  } else {
		    fprintf(outfile, "\t\t if(_jz_k==1) {DivideMyFloatA(%s, MakeMyFloatB(_jz_uvar1, 1), %s);}\n", name1, rightStr);
		  }
		}
	      icnt1++;
	      if(VAR_IS_JET(var)) {
		fprintf(outfile, "\t\t else {AssignFloatToJet(%s, _jz_MyFloatZERO);}\n", name1);	      
	      } else {
		fprintf(outfile, "\t\t else {AssignMyFloat(%s, _jz_MyFloatZERO);}\n", name1);	      
	      }
	    }
	  else {
	    if(VAR_IS_JET(var)) {
	      if(VAR_IS_JET(left)) {
		fprintf(outfile, "\t\t DivideJetFloatA(%s, %s, %s);\n", name1, leftStr, rightStr);
	      } else {
		fprintf(outfile, "\t\t DivideMyFloatA(%s, %s, %s);\n", "_kthtmp", leftStr, rightStr);
		fprintf(outfile, "\t\t\t  AssignFloatToJet(%s, %s);\n", name1, "_kthtmp");
	      }
	    } else {
	      fprintf(outfile, "\t\t DivideMyFloatA(%s, %s, %s);\n", name1, leftStr, rightStr);
	    }
	  }
	}
      else if(NODE_IS_CST(left))
	{
	  char *lstr, *rstr;
	  fprintf(outfile, "\t\t { /* division */\n");
	  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
	  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp)\n");	  	  
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp;\n");
            fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp)\n");
	  }
	  fprintf(outfile, "\t\t\t if(_jz_initialized==0) { InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);\n");
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t\t  InitJet(stmp1);InitJet(stmp2); InitJet(stmp);\n");
	  }
	  fprintf(outfile,"\t\t\t   }\n");

	  if(VAR_IS_JET(var)) { 	  
	    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
	  } else {
	    fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
	  }
	  /* optimization: don't do full loop if operand is linear */
	  if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
	    fprintf(outfile, "\t\t\t for(_jz_l=(_jz_k==1?1:_jz_k-1); _jz_l<=_jz_k; _jz_l++) {\n");
	  else 
	    fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
	  lstr = genNodeKthDerivative(var,  "_jz_k-_jz_l");
	  rstr = genNodeKthDerivative(right, "_jz_l");	      
	  if(VAR_IS_JET(var)) {
	    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s,%s);\n", rstr, lstr);
	    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
	    fprintf(outfile, "\t\t\t     SubtractJetJetA(stmp, stmp2, stmp1);\n");
	  } else {
	    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", rstr, lstr);
	    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
	    fprintf(outfile, "\t\t\t     SubtractMyFloatA(tmp, tmp2, tmp1);\n");
	  }
	  fprintf(outfile, "\t\t\t }\n");
	  rightStr = genNodeRvalueF(right);
	  if(VAR_IS_JET(var)) { 	  	  
	    fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, %s);\n", name1, rightStr);
	  } else {
	    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, %s);\n", name1, rightStr);
	  }
	  fprintf(outfile, "\t\t }\n");	      
	}
      else
	{
	  char *str, *rstr;
	  fprintf(outfile, "\t\t { /* division general */\n");
	  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
	  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp)\n");	  	  
	  if(total_jet_vars) {
	    fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp;\n");
            fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp)\n");
	  }
          fprintf(outfile, "\t\t\t if(_jz_initialized==0) { InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);\n");
	  if(total_jet_vars) {
            fprintf(outfile, "\t\t\t\t  InitJet(stmp1); InitJet(stmp2); InitJet(stmp);\n");
	  }
	  fprintf(outfile,"\t\t\t   }\n");

	  if(VAR_IS_JET(var)) {
	    if(VAR_IS_JET(left)) { 	  	  
	      if(VAR_IS_JET(right)) { 	  	  
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
		if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
		  fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=1; _jz_l++) {\n");
		else 
		  fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		rstr = genNodeKthDerivative(right, "_jz_l");
		str  = genNodeKthDerivative(var,  "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s,%s);\n", rstr, str);
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		leftStr  = genNodeKthDerivative(left,  "_jz_k");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		fprintf(outfile, "\t\t\t SubtractJetJetA(stmp, %s, stmp2);\n", leftStr);
		rightStr = genNodeRvalueF(right);
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, %s);\n", name1, rightStr);
		fprintf(outfile, "\t\t }\n");
	      } else {
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
		if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
		  fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=1; _jz_l++) {\n");
		else 
		  fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		rstr = genNodeKthDerivative(right, "_jz_l");
		str  = genNodeKthDerivative(var,  "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyFloatJetA(stmp1, %s,%s);\n", rstr, str);
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		leftStr  = genNodeKthDerivative(left,  "_jz_k");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		fprintf(outfile, "\t\t\t SubtractJetJetA(stmp, %s, stmp2);\n", leftStr);
		rightStr = genNodeRvalueF(right);
		fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, %s);\n", name1, rightStr);
		fprintf(outfile, "\t\t }\n");
	      }
	    } else if(VAR_IS_JET(right)) { 	  	  
	      fprintf(outfile, "\t\t\t AssignFloatToJet(stmp, _jz_MyFloatZERO);\n");
	      if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=1; _jz_l++) {\n");
	      else 
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
	      rstr = genNodeKthDerivative(right, "_jz_l");
	      str  = genNodeKthDerivative(var,  "_jz_k-_jz_l");
	      fprintf(outfile, "\t\t\t     MultiplyJetJetA(tmp1, %s,%s);\n", rstr, str);
	      fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
	      fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, tmp1);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      leftStr  = genNodeKthDerivative(left,  "_jz_k");
	      fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
	      fprintf(outfile, "\t\t\t SubtractJetFloatA(stmp, %s, stmp2);\n", leftStr);
	      rightStr = genNodeRvalueF(right);
	      fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, %s);\n", name1, rightStr);
	      fprintf(outfile, "\t\t }\n");
	    } else {
	      fprintf(stderr, "Should not be here, 'divide expr'\n");
	      exit(211);	      
	    }
	  } else {
	    fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
	    if( (larg = linearTime(right, &ddflag, &mmflag)) != NULL)      
	      fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=1; _jz_l++) {\n");
	    else 
	      fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
	    rstr = genNodeKthDerivative(right, "_jz_l");
	    str  = genNodeKthDerivative(var,  "_jz_k-_jz_l");
	    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", rstr, str);
	    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
	    fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp2, tmp1);\n");
	    fprintf(outfile, "\t\t\t }\n");
	    leftStr  = genNodeKthDerivative(left,  "_jz_k");
	    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
	    fprintf(outfile, "\t\t\t SubtractMyFloatA(tmp, %s, tmp2);\n", leftStr);
	    rightStr = genNodeRvalueF(right);
	    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, %s);\n", name1, rightStr);
	    fprintf(outfile, "\t\t }\n");
	  }
	}
      break;
    case NEGATE_EXPR:
      fprintf(outfile, "\t\t /* negation: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      leftStr = genNodeKthDerivative(left, "_jz_k");
      if(VAR_IS_JET(var)) { 	  	  
	if(VAR_IS_JET(left)) {
	  fprintf(outfile, "\t\t NegateJetA(%s, %s);\n", name1, leftStr);	  
	} else {
	  fprintf(outfile, "\t\t NegateMyFloatA(%s, %s);\n", "_kthtmp", leftStr);	  
	  fprintf(outfile, "\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthtmp");	  
	}
      } else {
	fprintf(outfile, "\t\t NegateMyFloatA(%s, %s);\n", name1, leftStr);	  
      }
      break;
    case EXP_EXPR:
      fprintf(outfile, "\t\t /* exponentiation: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      if(ID_IS_CST(right) == 0)
	{
	  fprintf(stderr, "Internal Error: exponent is not a constant in 'outputDerivative'.\n");
	  fprintf(stderr, "Node info: %s, offending variable %s\n", 
                  NODE_GIVEN_NAME(var)? NODE_GIVEN_NAME(var): NODE_INFO(var), 
                  NODE_GIVEN_NAME(right)?NODE_GIVEN_NAME(right): NODE_NAME(right));
	}
      else
	{
          Node numer = NULL, denom = NULL;
	  char *lstr, *rstr, *str;
	  int expo = 0, junk=0, ratexp=0;
	  fprintf(outfile, "\t\t { /* exponentiation */\n");
	  if( ID_IS_INT(right) ) expo = intNodeValue(right, &junk);

        if(VAR_IS_JET(var)) {
	  if(ID_IS_INT(right) && junk >= 0 && expo == 2)
	    {
	      fprintf(outfile, "\t\t\t\t /* expr^2 */ \n");
	      if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
		{
                  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
		  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp)\n");	  		  
                  fprintf(outfile, "\t\t\t if(_jz_initialized==0) { InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);}\n");
                  fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
		  lstr = genNodeKthDerivative(left,  "0");
		  rstr = genNodeKthDerivative(left, "_jz_k");
		  fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
		  lstr = genNodeKthDerivative(left,  "1");
		  rstr = genNodeKthDerivative(left, "_jz_k-1");
		  fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,rstr);
		  fprintf(outfile, "\t\t\t AddMyFloatA(tmp, tmp2, tmp1);\n");
		  fprintf(outfile, "\t\t\t AssignFloatToJet(%s, tmp);\n", name1);
		}
	      else
		{
                  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
		  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp)\n");	  		  
		  if(total_jet_vars) {
		    fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp;\n");
                    fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp)\n");
		  }
		  fprintf(outfile, "\t\t\t int parity=(_jz_k&1), half=(_jz_k+1)>>1;\n");
                  fprintf(outfile, "\t\t\t if(_jz_initialized==0) { InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);\n");
		  if(total_jet_vars) {
		    fprintf(outfile, "\t\t\t\t  InitJet(stmp1);InitJet(stmp2); InitJet(stmp);\n");
		  }
		  fprintf(outfile, "\t\t\t };\n");		  

		  if(VAR_IS_JET(left)) {
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<half; _jz_l++) {\n");
		    lstr = genNodeKthDerivative(left,  "_jz_l");
		    rstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s, %s);\n",lstr,rstr);
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp2, stmp);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp2, stmp1);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t AssignJetToJet(stmp2, stmp);\n");
		    fprintf(outfile, "\t\t\t AddJetJetA(stmp1, stmp2, stmp);\n");
		    fprintf(outfile, "\t\t\t if(parity==0) {\n");	  
		    lstr = genNodeKthDerivative(left,  "half");
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, %s, %s);\n",lstr,lstr);	  
		    fprintf(outfile, "\t\t\t     AddJetJetA(%s, stmp2, stmp1);\n", name1);
		    fprintf(outfile, "\t\t\t } else { \n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(%s, stmp1);\n", name1);
		    fprintf(outfile, "\t\t\t }\n");
		  } else {
		    fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<half; _jz_l++) {\n");
		    lstr = genNodeKthDerivative(left,  "_jz_l");
		    rstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
		    fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp2, tmp1);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, tmp);\n");
		    fprintf(outfile, "\t\t\t AddMyFloatA(tmp1, tmp2, tmp);\n");
		    fprintf(outfile, "\t\t\t if(parity==0) {\n");	  
		    lstr = genNodeKthDerivative(left,  "half");
		    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,lstr);	  
		    fprintf(outfile, "\t\t\t     AddMyFloatA(%s, tmp2, tmp1);\n", "_kthtmp");
		    fprintf(outfile, "\t\t\t     AssignFloatToJet(%s, %s);\n", name1, "_kthtmp");
		    fprintf(outfile, "\t\t\t } else { \n");
		    fprintf(outfile, "\t\t\t     AssignFloatToJet(%s, tmp1);\n", name1);
		    fprintf(outfile, "\t\t\t }\n");
		  }
		}
	    } 
          else if((ratexp = isRational(right, &numer, &denom)) != 0) /* x^ (p/q) */
            {
              int jnk1=0;
              int pp = intNodeValue(numer, &jnk1);
              int qq = intNodeValue(denom, &jnk1);
              if(ratexp == -1) pp = -pp;
              if(qq != 1)
                {
                  fprintf(outfile, "\t\t\t\t /* expr^(%d/%d)/ */ \n", pp, qq);
                  fprintf(outfile, "\t\t\t int  ppk=(%d)*_jz_k, qqk=(%d)*_jz_k, pq=%d;\n", pp, qq, pp+qq);
                }
              else
                {
                  fprintf(outfile, "\t\t\t\t /* expr^(%d)/ */ \n", pp);
                  fprintf(outfile, "\t\t\t int  ppk=(%d)*_jz_k, qqk=_jz_k, pq=(%d+1);\n", pp,pp);
                }
              fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmpC, tmp;\n");
	      fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmpC, tmp)\n");	  	      
	      if(total_jet_vars) {
		fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmpC, stmp;\n");
                fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmpC, stmp)\n");
	      }
              fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp3);\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmpC);InitMyFloat(tmp);\n");
	      if(total_jet_vars) {
		fprintf(outfile, "\t\t\t\t  InitJet(stmp1);InitJet(stmp2);InitJet(stmp3);InitJet(stmpC);InitJet(stmp);\n");
	      }
              fprintf(outfile, "\t\t\t }\n");              
	      if(VAR_IS_JET(left)) {
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		fprintf(outfile, "\t\t\t     MakeMyFloatA(tmpC, ppk);\n");
		fprintf(outfile, "\t\t\t     AssignFloatToJet(stmpC, tmpC);\n");
		fprintf(outfile, "\t\t\t     ppk -= pq  ;\n");
		str = genNodeKthDerivative(var, "_jz_l");
		lstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s,%s);\n", str, lstr);
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, stmpC, stmp1);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp1,  stmp, stmp2);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp,  stmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		leftStr = genNodeRvalueF(left);
		fprintf(outfile, "\t\t\t MakeMyFloatA(tmp3,qqk);\n");
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp3,tmp3);\n");
		fprintf(outfile, "\t\t\t MultiplyJetJetA(stmp1, %s, stmp3);\n", leftStr);
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, stmp1);\n", name1);              
	      } else {
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		fprintf(outfile, "\t\t\t     MakeMyFloatA(tmpC, ppk);\n");
		fprintf(outfile, "\t\t\t     ppk -= pq  ;\n");
		str = genNodeKthDerivative(var, "_jz_l");
		lstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp1, %s,%s);\n", str, lstr);
		fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp2, tmpC, stmp1);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp1,  stmp, stmp2);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp,  stmp1);\n");
		fprintf(outfile, "\t\t\t }\n");
		leftStr = genNodeRvalueF(left);
		fprintf(outfile, "\t\t\t MakeMyFloatA(tmp3,qqk);\n");
		fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, tmp3);\n", leftStr);
		fprintf(outfile, "\t\t\t DivideJetFloatA(stmp2, tmp, tmp1);\n");              
		fprintf(outfile, "\t\t\t AssignJetToJet(%s, stmp2);\n", name1);
	      }
            }
	  else /* general exponent, not rational */
	    {
              fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2, tmp3, tmpkml, tmpl,cco, exponent;\n");
	      fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3, tmpkml, tmpl, cco, exponent)\n");	  	      	      
	      if(total_jet_vars) {
                fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp;\n");
                fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp)\n");
	      }
              fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp3);InitMyFloat(tmpkml);\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmpl); InitMyFloat(cco); InitMyFloat(exponent);\n");
	      if(total_jet_vars) {
                fprintf(outfile, "\t\t\t\t  InitJet(stmp1);InitJet(stmp2);InitJet(stmp3);InitJet(stmp);\n");
	      }
              fprintf(outfile, "\t\t\t }\n");
	      
	      if(VAR_IS_JET(left)) {
		fprintf(outfile, "\t\t\t AssignMyFloat(exponent, %s);\n", genNodeRvalueF(right));
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		fprintf(outfile, "\t\t\t     int kml = _jz_k-_jz_l;\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmpkml, _jz_theNs[kml]);\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmpl, _jz_theNs[_jz_l]);\n");
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp3,  tmpkml, exponent);\n");
		fprintf(outfile, "\t\t\t     SubtractMyFloatA(cco,  tmp3, tmpl);\n");
		str = genNodeKthDerivative(var, "_jz_l");
		lstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s,%s);\n", str, lstr);
                fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp2, stmp1, cco);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp1, stmp);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp,  stmp1, stmp2);\n");
		fprintf(outfile, "\t\t\t }\n");
		leftStr = genNodeRvalueF(left);
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp3,_jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmp1, %s, tmp3);\n", leftStr);
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, stmp1);\n", name1);
	      } else {
		fprintf(outfile, "\t\t\t AssignMyFloat(exponent, %s);\n", genNodeRvalueF(right));
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		fprintf(outfile, "\t\t\t     int kml = _jz_k-_jz_l;\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmpkml, _jz_theNs[kml]);\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmpl, _jz_theNs[_jz_l]);\n");
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp3,  tmpkml, exponent);\n");
		fprintf(outfile, "\t\t\t     SubtractMyFloatA(cco,  tmp3, tmpl);\n");
		str = genNodeKthDerivative(var, "_jz_l");
		lstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp1, %s,%s);\n", str, lstr);
		fprintf(outfile, "\t\t\t     MultiplyJetFloatA(tmp2, stmp1, cco);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp1, stmp);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp1, stmp2);\n");
		fprintf(outfile, "\t\t\t }\n");
		leftStr = genNodeRvalueF(left);
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp3,_jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, tmp3);\n", leftStr);
		fprintf(outfile, "\t\t\t DivideJetFloatA(%s, tmp, tmp1);\n", name1);
	      }
	    }
	  fprintf(outfile, "\t\t}\n");
	} else { /* normal MYFLOAT */
          if(ID_IS_INT(right) && junk >= 0 && expo == 2)
	    {
	      fprintf(outfile, "\t\t\t\t /* expr^2 */ \n");
	      if( (larg = linearTime(left, &ddflag, &mmflag)) != NULL)
		{
                  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
                  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2)\n");
                  fprintf(outfile, "\t\t\t if(_jz_initialized==0) { InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);}\n");
                  fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_MyFloatZERO);\n");
		  lstr = genNodeKthDerivative(left,  "0");
		  rstr = genNodeKthDerivative(left, "_jz_k");
		  fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
		  lstr = genNodeKthDerivative(left,  "1");
		  rstr = genNodeKthDerivative(left, "_jz_k-1");
		  fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,rstr);
		  fprintf(outfile, "\t\t\t AddMyFloatA(tmp, tmp2, tmp1);\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp);\n", name1);
		}
	      else
		{
                  fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp;\n");
                  fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2)\n");
		  fprintf(outfile, "\t\t\t int parity=(_jz_k&1), half=(_jz_k+1)>>1;\n");
                  fprintf(outfile, "\t\t\t if(_jz_initialized==0) { InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp);}\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<half; _jz_l++) {\n");
		  lstr = genNodeKthDerivative(left,  "_jz_l");
		  rstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s, %s);\n",lstr,rstr);
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp2, tmp);\n");
		  fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp2, tmp1);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, tmp);\n");
		  fprintf(outfile, "\t\t\t AddMyFloatA(tmp1, tmp2, tmp);\n");
		  fprintf(outfile, "\t\t\t if(parity==0) {\n");	  
		  lstr = genNodeKthDerivative(left,  "half");
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s, %s);\n",lstr,lstr);	  
		  fprintf(outfile, "\t\t\t     AddMyFloatA(%s, tmp2, tmp1);\n", name1);
		  fprintf(outfile, "\t\t\t } else { \n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(%s, tmp1);\n", name1);
		  fprintf(outfile, "\t\t\t }\n");
		}
	    }
          else if((ratexp = isRational(right, &numer, &denom)) != 0) /* x^ (p/q) */
            {
              int jnk1=0;
              int pp = intNodeValue(numer, &jnk1);
              int qq = intNodeValue(denom, &jnk1);
              if(ratexp == -1) pp = -pp;
              if(qq != 1)
                {
                  fprintf(outfile, "\t\t\t\t /* expr^(%d/%d)/ */ \n", pp, qq);
                  fprintf(outfile, "\t\t\t int  ppk=(%d)*_jz_k, qqk=(%d)*_jz_k, pq=%d;\n", pp, qq, pp+qq);
                }
              else
                {
                  fprintf(outfile, "\t\t\t\t /* expr^(%d)/ */ \n", pp);
                  fprintf(outfile, "\t\t\t int  ppk=(%d)*_jz_k, qqk=_jz_k, pq=(%d+1);\n", pp,pp);
                }
              fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmpC, tmp;\n");
              fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3, tmpC)\n");
              fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp3);\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmpC);InitMyFloat(tmp);\n");
              fprintf(outfile, "\t\t\t }\n");              
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
	      fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
	      fprintf(outfile, "\t\t\t     MakeMyFloatA(tmpC, ppk);\n");
	      fprintf(outfile, "\t\t\t     ppk -= pq  ;\n");
	      str = genNodeKthDerivative(var, "_jz_l");
	      lstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", str, lstr);
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, tmpC, tmp1);\n");
	      fprintf(outfile, "\t\t\t     AddMyFloatA(tmp1,  tmp, tmp2);\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmp,  tmp1);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      leftStr = genNodeRvalueF(left);
	      fprintf(outfile, "\t\t\t MakeMyFloatA(tmp3,qqk);\n");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, tmp3);\n", leftStr);
	      fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmp1);\n", name1);              
            }
	  else
	    {
              fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2, tmp3, tmpkml, tmpl,cco, exponent;\n");
	      fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3, tmpkml, tmpl, cco, exponent)\n");	  	      	      	      
              fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp3);InitMyFloat(tmpkml);\n");
              fprintf(outfile, "\t\t\t  InitMyFloat(tmpl); InitMyFloat(cco); InitMyFloat(exponent);\n");
              fprintf(outfile, "\t\t\t }\n");
	      fprintf(outfile, "\t\t\t AssignMyFloat(exponent, %s);\n", genNodeRvalueF(right));
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
	      fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
	      fprintf(outfile, "\t\t\t     int kml = _jz_k-_jz_l;\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmpkml, _jz_theNs[kml]);\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmpl, _jz_theNs[_jz_l]);\n");
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp3,  tmpkml, exponent);\n");
	      fprintf(outfile, "\t\t\t     SubtractMyFloatA(cco,  tmp3, tmpl);\n");
	      str = genNodeKthDerivative(var, "_jz_l");
	      lstr = genNodeKthDerivative(left, "_jz_k-_jz_l");
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", str, lstr);
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, tmp1, cco);\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, tmp);\n");
	      fprintf(outfile, "\t\t\t     AddMyFloatA(tmp,  tmp1, tmp2);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      leftStr = genNodeRvalueF(left);
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp3,_jz_theNs[_jz_k]);\n");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, tmp3);\n", leftStr);
	      fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmp1);\n", name1);
	    }
	  fprintf(outfile, "\t\t}\n");
	}
	}
      break;	  
    case CALL_EXPR:
      fprintf(outfile, "\t\t /* call: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      {
	char *fname = ID_NAME(left);
	char *rstr, *astr, *bstr, *str;
	int  dflag, mflag;
	Node carg;

	if(NODE_DERI_DONE(var)) 
	  {
	    fprintf(outfile, "\t\t\t /* computed already */\n");
	    return;
	  }
	
	if(!strcmp(fname, "sin"))
	  {
	    Node budy = NODE_BUDY(var);
	    NODE_DERI_DONE(var) = 1;
	    NODE_DERI_DONE(budy) = 1;
	    carg = linearTime(right, &dflag, &mflag);
	    if(carg)
	      {
		fprintf(outfile, "\t\t { /* call sin cos */\n");
		if(carg == int_one_node)
		  {
		    if(mflag == 0) /* t+c */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");	  	      	      			
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); \n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	    
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n", name1, bstr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n", name1, bstr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n", name1, bstr);
			}
			  
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n", astr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n", astr);			  
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n", astr);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2, tmp1);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", genVarLvalueKth(budy, "_jz_k"));
		      }
		    else  /* cst-t */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");	  	      	      						
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); \n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	    
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n", bstr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n", bstr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n", bstr);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2, tmp1);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", name1);
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n",*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n",
				  genVarLvalueKth(budy, "_jz_k"), astr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n",
				  genVarLvalueKth(budy, "_jz_k"), astr);
			}
		      }
		  }
		else
		  {
		    char  *tstr = genNodeRvalueF(carg);
		    if(mflag == 0) /* a*t+cst */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2, tmp3;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)\n");	  	      	      						
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);\n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1"); 
			if(dflag == 0) {
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp, %s, _jz_oneOverN[_jz_k]);\n", tstr);*/
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, %s, _jz_k);\n", tstr);
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, %s, _jz_theNs[_jz_k]);\n", tstr);
			  }
			} else {
			  /* fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_oneOverN[_jz_k], %s);\n", tstr);*/
			  /*
			  fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp3, _jz_theNs[_jz_k], %s);\n", tstr);
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_theNs[1], tmp3);\n");
			  */
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp3, _jz_theNs[1], %s);\n",tstr);
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, tmp3, _jz_k);\n");
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, tmp3, _jz_theNs[_jz_k]);\n");
			  }
			}
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, tmp);\n", name1, bstr);
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, tmp, %s);\n", astr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp1, tmp2);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp1);\n", genVarLvalueKth(budy, "_jz_k"));
		      }
		    else  /* cst-a*t */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2, tmp3;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)\n");	  	      	      									
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3); InitMyFloat(tmp);\n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1"); 
			if(dflag == 0) {
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp,  %s, _jz_oneOverN[_jz_k]);\n", tstr);*/
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp,  %s, _jz_k);\n", tstr);
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp,  %s, _jz_theNs[_jz_k]);\n", tstr);
			  }
			} else {
			  /*fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_oneOverN[_jz_k], %s);\n", tstr);*/
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp3, _jz_theNs[_jz_k], %s);\n", tstr);
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_theNs[1], tmp3);\n");
			  */
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp3, _jz_theNs[1], %s);\n",tstr);
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, tmp3, _jz_k);\n");
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, tmp3, _jz_theNs[_jz_k]);\n");
			  }
			}
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, tmp, %s);\n", bstr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp1, tmp2);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp1);\n", name1);
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, tmp);\n", genVarLvalueKth(budy,"_jz_k"), astr);
		      }	
		  }
		fprintf(outfile, "\t\t }\n");		
	      }
	    else /* general sin */
	      {	    
		fprintf(outfile, "\t\t { /* call sin cos */\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmp, ctmp;\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT smp2, smp3, smp;\n");
		fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmp, ctmp, smp2, smp3, smp)\n");
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp, sctmp;\n");
		  fprintf(outfile, "\t\t\t static MY_JET ssmp2, ssmp3, ssmp;\n");
                  fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp, sctmp, ssmp2, ssmp3, ssmp)\n");
		}
                fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2); InitMyFloat(tmp3);\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(tmp);InitMyFloat(ctmp); InitMyFloat(smp2);\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(smp3); InitMyFloat(smp);\n");
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t  InitJet(stmp1);InitJet(stmp2); InitJet(stmp3);\n");
		  fprintf(outfile, "\t\t\t  InitJet(stmp);InitJet(sctmp); InitJet(ssmp2);\n");
		  fprintf(outfile, "\t\t\t  InitJet(ssmp3); InitJet(ssmp);\n");
		}
                fprintf(outfile, "\t\t\t }\n");

		if(VAR_IS_JET(var)) {
		  if(VAR_IS_JET(right)) { // Jet=sin(Jet)
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t AssignFloatToJet(ssmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_k-_jz_l]);\n");
		    rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyFloatJetA(sctmp, tmp1, %s);\n", rstr);
		    bstr = genNodeKthDerivative(budy, "_jz_l");	    
		    astr = genNodeKthDerivative(var,  "_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, sctmp, %s);\n", bstr);
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(ssmp2, sctmp, %s);\n", astr);
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp3, stmp, stmp2);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(ssmp3, ssmp, ssmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp3);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(ssmp, ssmp3);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", name1);
		    fprintf(outfile, "\t\t\t NegateJetA(stmp, ssmp);\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  } else { // Jet = sin(Float)
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t AssignFloatToJet(ssmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_k-_jz_l]);\n");
		    rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(ctmp, tmp1, %s);\n", rstr);
		    bstr = genNodeKthDerivative(budy, "_jz_l");	    
		    astr = genNodeKthDerivative(var,  "_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyFloatJetA(stmp2, ctmp, %s);\n", bstr);
		    fprintf(outfile, "\t\t\t     MultiplyFloatJetA(ssmp2, ctmp, %s);\n", astr);
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp3, stmp, stmp2);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(ssmp3, ssmp, ssmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(tmp, tmp3);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(ssmp, ssmp3);\n");
		    fprintf(outfile, "\t\t\t }\n");

		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", name1);

		    fprintf(outfile, "\t\t\t NegateJetA(stmp, ssmp);\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  }
		} else {
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(smp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_k-_jz_l]);\n");
		  rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(ctmp, tmp1, %s);\n", rstr);
		  bstr = genNodeKthDerivative(budy, "_jz_l");	    
		  astr = genNodeKthDerivative(var,  "_jz_l");	    
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, ctmp, %s);\n", bstr);
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(smp2, ctmp, %s);\n", astr);
		  fprintf(outfile, "\t\t\t     AddMyFloatA(tmp3, tmp, tmp2);\n");
		  fprintf(outfile, "\t\t\t     AddMyFloatA(smp3, smp, smp2);\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp3);\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(smp, smp3);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp, _jz_oneOverN[_jz_k]);\n", name1);*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, tmp, _jz_k);\n", name1);
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, _jz_theNs[_jz_k]);\n", name1);
		  }
		  fprintf(outfile, "\t\t\t NegateMyFloatA(tmp, smp);\n");
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp, _jz_oneOverN[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, tmp, _jz_k);\n", genVarLvalueKth(budy, "_jz_k"));
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  }
		}
		fprintf(outfile, "\t\t }\n");
	      }
	  }
	else if(!strcmp(fname, "cos"))
	  {
	    Node budy = NODE_BUDY(var);
	    NODE_DERI_DONE(var) = 1;
	    NODE_DERI_DONE(budy) = 1;
	    carg = linearTime(right, &dflag, &mflag);
	    if(carg)
	      {
		fprintf(outfile, "\t\t { /* call cos sin */\n");
		if(carg == int_one_node)
		  {
		    if(mflag == 0) /* t+cst */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
                        fprintf(outfile, "\t\t\t }\n");

			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	    
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n", bstr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n", bstr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n", bstr);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2, tmp1);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", name1);
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n", */
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n", genVarLvalueKth(budy, "_jz_k"), astr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"), astr);
			}
		      }
		    else /* cst-t */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	    
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n", name1, bstr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n", name1, bstr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n", name1, bstr);
			}
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", name1);
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n", astr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n", astr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n", astr);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2, tmp1);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", genVarLvalueKth(budy, "_jz_k"));
		      }
		  }
		else
		  {
		    char  *tstr = genNodeRvalueF(carg);
		    if(mflag == 0) /* a*t+cst */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2, tmp3;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)\n");
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);InitMyFloat(tmp);\n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1"); 
			if(dflag == 0) {
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp, %s, _jz_oneOverN[_jz_k]);\n", tstr);*/
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, %s, _jz_k);\n", tstr);
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, %s, _jz_theNs[_jz_k]);\n", tstr);
			  }
			} else {
			  /*fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_oneOverN[_jz_k], %s);\n", tstr);*/
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp3, _jz_theNs[_jz_k], %s);\n", tstr);
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_theNs[1], tmp3);\n");
			  */
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp3, _jz_theNs[1], %s);\n",tstr);
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, tmp3, _jz_k);\n");
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, tmp3, _jz_theNs[_jz_k]);\n");
			  }
			}
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, tmp, %s);\n", bstr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp1, tmp2);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp1);\n", name1);
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, tmp);\n", 
				genVarLvalueKth(budy, "_jz_k"), astr);
		      }
		    else  /* cst-a*t */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2,tmp3;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)\n");
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);InitMyFloat(tmp);\n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1"); 
			if(dflag == 0) {
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp, %s, _jz_oneOverN[_jz_k]);\n", tstr);*/
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, %s, _jz_k);\n", tstr);
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, %s, _jz_theNs[_jz_k]);\n", tstr);
			  }
			} else {
			  /*fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_oneOverN[_jz_k], %s);\n", tstr);*/
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp3, _jz_theNs[_jz_k], %s);\n", tstr);
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_theNs[1], tmp3);\n");
			  */
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp3, _jz_theNs[1], %s);\n",tstr);
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, tmp3, _jz_k);\n");
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, tmp3, _jz_theNs[_jz_k]);\n");
			  }
			}
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, tmp);\n", name1, bstr);
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, %s, tmp);\n", astr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp1, tmp2);\n");
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp1);\n", genVarLvalueKth(budy,"_jz_k"));
		      }		    
		  }
		fprintf(outfile, "\t\t }\n");		
	      }
	    else // cos
	      {
		fprintf(outfile, "\t\t { /* call cos sin*/\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmp, ctmp;\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT smp2, smp3, smp;\n");
		fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmp, ctmp, smp2, smp3, smp)\n");		
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp, sctmp;\n");
		  fprintf(outfile, "\t\t\t static MY_JET ssmp2, ssmp3, ssmp;\n");
                  fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp, sctmp, ssmp2, ssmp3, ssmp)\n");
		}
		fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
		fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);InitMyFloat(tmp);\n");
		fprintf(outfile, "\t\t\t  InitMyFloat(ctmp);InitMyFloat(smp2);InitMyFloat(smp3);InitMyFloat(smp);\n");
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t  InitJet(stmp1);InitJet(stmp2); InitJet(stmp3);\n");
		  fprintf(outfile, "\t\t\t  InitJet(stmp);InitJet(sctmp); InitJet(ssmp2);\n");
                  fprintf(outfile, "\t\t\t  InitJet(ssmp3); InitJet(ssmp);\n");
		}
		fprintf(outfile, "\t\t\t }\n");

		if(VAR_IS_JET(var)) {
		  if(VAR_IS_JET(right)) {
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t AssignFloatToJet(ssmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1,_jz_theNs[_jz_k-_jz_l]);\n");
		    rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    
                    fprintf(outfile, "\t\t\t     MultiplyFloatJetA(sctmp, tmp1, %s);\n", rstr);
		    bstr = genNodeKthDerivative(budy, "_jz_l");	    
		    astr = genNodeKthDerivative(var,  "_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, sctmp, %s);\n", bstr);
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(ssmp2, sctmp, %s);\n", astr);
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp3, stmp, stmp2);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(ssmp3, ssmp, ssmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp3);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(ssmp, ssmp3);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, ssmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		    fprintf(outfile, "\t\t\t NegateJetA(ssmp, stmp);\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, ssmp, _jz_theNs[_jz_k]);\n", name1);
		    fprintf(outfile, "\t\t }\n");
		  } else { // Jet=cos(Float)
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t AssignFloatToJet(ssmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1,_jz_theNs[_jz_k-_jz_l]);\n");
		    rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(ctmp, tmp1, %s);\n", rstr);
		    bstr = genNodeKthDerivative(budy, "_jz_l");	    
		    astr = genNodeKthDerivative(var,  "_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, sctmp, %s);\n", bstr);
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(ssmp2, sctmp, %s);\n", astr);
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp3, stmp, stmp2);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(ssmp3, ssmp, ssmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp3);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(ssmp, ssmp3);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, ssmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		    fprintf(outfile, "\t\t\t NegateJetA(ssmp, stmp);\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, ssmp, _jz_theNs[_jz_k]);\n", name1);
		    fprintf(outfile, "\t\t }\n");
		  }
  		} else {
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(smp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=0; _jz_l<_jz_k; _jz_l++) {\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1,_jz_theNs[_jz_k-_jz_l]);\n");
		  rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(ctmp, tmp1, %s);\n", rstr);
		  bstr = genNodeKthDerivative(budy, "_jz_l");	    
		  astr = genNodeKthDerivative(var,  "_jz_l");	    
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, ctmp, %s);\n", bstr);
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(smp2, ctmp, %s);\n", astr);
		  fprintf(outfile, "\t\t\t     AddMyFloatA(tmp3, tmp, tmp2);\n");
		  fprintf(outfile, "\t\t\t     AddMyFloatA(smp3, smp, smp2);\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp3);\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(smp, smp3);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, smp, _jz_oneOverN[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, smp, _jz_k);\n", genVarLvalueKth(budy, "_jz_k"));
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, smp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  }
		  fprintf(outfile, "\t\t\t NegateMyFloatA(smp,  tmp);\n");
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, smp, _jz_oneOverN[_jz_k]);\n", name1);*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, smp, _jz_k);\n", name1);
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, smp, _jz_theNs[_jz_k]);\n", name1);
		  }
		  fprintf(outfile, "\t\t }\n");

		}
	      }
	  }
	else if(!strcmp(fname, "exp"))
	  {
	    carg = linearTime(right, &dflag, &mflag);
	    if(carg)
	      {
		fprintf(outfile, "\t\t { /* call exp */\n");
		if(carg == int_one_node)
		  {
		    if(mflag == 0) /* t+cst */
		      {
			str = genNodeKthDerivative(var, "_jz_k-1");
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n", name1, str);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n", name1, str);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n", name1, str);
			}
		      }
		    else
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");		
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
                        fprintf(outfile, "\t\t\t }\n");

			str = genNodeKthDerivative(var, "_jz_k-1");
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n", str);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n", str);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n", str);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2,  tmp1);\n");			
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", name1);
		      }
		  }
		else
		  {
		    char  *tstr = genNodeRvalueF(carg);
		    if(mflag == 0) /* a*t+cst */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1)\n");		
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);\n");
                        fprintf(outfile, "\t\t\t }\n");
			str = genNodeKthDerivative(var, "_jz_k-1");
			if(dflag == 0)fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, %s);\n", str, tstr);
			else fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, %s);\n", str, tstr);
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp1, _jz_oneOverN[_jz_k]);\n", name1);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, tmp1, _jz_k);\n", name1);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp1, _jz_theNs[_jz_k]);\n", name1);
			}
		      }
		    else /* cst-a*t */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");		
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
                        fprintf(outfile, "\t\t\t }\n");
			str = genNodeKthDerivative(var, "_jz_k-1");
			if(dflag == 0)fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, %s);\n", str, tstr);
			else fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, %s);\n", str, tstr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2,  tmp1);\n");			
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp2, _jz_oneOverN[_jz_k]);\n", name1);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, tmp2, _jz_k);\n", name1);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp2, _jz_theNs[_jz_k]);\n", name1);
			}
		      }
		  }
		fprintf(outfile, "\t\t }\n");
	      }
	    else // Exp
	      {
		fprintf(outfile, "\t\t { /* call exp */\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmp;\n");
		fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmp)\n");		
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp;\n");
                  fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp)\n");
		}
                fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(tmp3);InitMyFloat(tmp);\n");
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t   InitJet(stmp1);InitJet(stmp2); InitJet(stmp3); InitJet(stmp);\n");
		}
                fprintf(outfile, "\t\t\t }\n");

		if(VAR_IS_JET(var)) {
		  if(VAR_IS_JET(right)) {
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		    str = genNodeKthDerivative(var, "_jz_k-_jz_l");
		    rstr = genNodeKthDerivative(right, "_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s,%s);\n", str, rstr);
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp3, _jz_theNs[_jz_l]);\n");            
		    fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp2, stmp1, tmp3);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp3, stmp);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp3, stmp2);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", name1);
		  } else {
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		    str  = genNodeKthDerivative(var, "_jz_k-_jz_l");
		    rstr = genNodeKthDerivative(right, "_jz_l");	    
		    fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp1, %s,%s);\n", str, rstr);
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp3, _jz_theNs[_jz_l]);\n");            
		    fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp2, stmp1, tmp3);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp3, stmp);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp, stmp3, stmp2);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", name1);
		  }
		} else {
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		  str = genNodeKthDerivative(var, "_jz_k-_jz_l");
		  rstr = genNodeKthDerivative(right, "_jz_l");	    
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", str, rstr);
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp3, _jz_theNs[_jz_l]);\n");            
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, tmp1, tmp3);\n");
		  fprintf(outfile, "\t\t\t     AddMyFloatA(tmp, tmp, tmp2);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp, _jz_oneOverN[_jz_k]);\n", name1);*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, tmp, _jz_k);\n", name1);
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, _jz_theNs[_jz_k]);\n", name1);
		  }
		}
		fprintf(outfile, "\t\t }\n");
	      }
	  }
	else if(!strcmp(fname, "log"))
	  {
	    fprintf(outfile, "\t\t { /* call log (natural log)*/\n");
	    fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmpm, tmpn, tmp2, tmp3;\n");
	    fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmpm, tmpn, tmp2, tmp3)\n");		
	    if(total_jet_vars) {
	      fprintf(outfile, "\t\t\t static MY_JET stmp, stmp1, stmpm, stmpn, stmp2, stmp3;\n");
              fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp, stmp1, stmpm, stmpn, stmp2, stmp3)\n");
	    }
            fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
            fprintf(outfile, "\t\t\t  InitMyFloat(tmp);InitMyFloat(tmp1);InitMyFloat(tmpm);\n");
            fprintf(outfile, "\t\t\t  InitMyFloat(tmpn);InitMyFloat(tmp2);InitMyFloat(tmp3);\n");
	    if(total_jet_vars) {
	      fprintf(outfile, "\t\t\t  InitJet(stmp);InitJet(stmp1);InitJet(stmpm);\n");
	      fprintf(outfile, "\t\t\t  InitJet(stmpn);InitJet(stmp2);InitJet(stmp3);\n");
	    }
            fprintf(outfile, "\t\t\t }\n");

	    if(VAR_IS_JET(var)) {
	      if(VAR_IS_JET(right)) {
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<_jz_k; _jz_l++) {\n");
		str = genNodeKthDerivative(var, "_jz_l");
		rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    	    	
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmpm, _jz_theNs[_jz_l]);\n");
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, %s,%s);\n", rstr, str);
		fprintf(outfile, "\t\t\t     MultiplyJetFloatA(stmp1, stmp2,tmpm);\n");
		fprintf(outfile, "\t\t\t     AddJetJetA(stmp2, stmp, stmp1);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp2);\n");
		fprintf(outfile, "\t\t\t }\n");
		rstr = genNodeKthDerivative(right, "_jz_k");	    	
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmpn, %s,tmp2);\n",rstr);
		fprintf(outfile, "\t\t\t SubtractJetJetA(stmp3, stmpn, stmp);\n");
		fprintf(outfile, "\t\t\t AssignJetToJet(stmp, stmp3);\n");
		rightStr = genNodeRvalueF(right);
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmpn, %s,tmp2);\n",rightStr);
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, stmpn);\n", name1);
		fprintf(outfile, "\t\t }\n");
	      } else {
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<_jz_k; _jz_l++) {\n");
		str = genNodeKthDerivative(var, "_jz_l");
		rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    	    	
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmpm, _jz_theNs[_jz_l]);\n");
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s,%s);\n", rstr, str);
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, tmp2,tmpm);\n");
		fprintf(outfile, "\t\t\t     AddMyFloatA(tmp2, tmp, tmp1);\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp2);\n");
		fprintf(outfile, "\t\t\t }\n");
		rstr = genNodeKthDerivative(right, "_jz_k");	    	
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmpn, %s,tmp2);\n",rstr);
		fprintf(outfile, "\t\t\t SubtractMyFloatA(tmp3, tmpn, tmp);\n");
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp, tmp3);\n");
		rightStr = genNodeRvalueF(right);
		fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmpn, %s,tmp2);\n",rightStr);
		fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmpn);\n", "_kthStmp");
		fprintf(outfile, "\t\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthStmp");
		fprintf(outfile, "\t\t }\n");
	      }
	    } else {
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
	      fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<_jz_k; _jz_l++) {\n");
	      str = genNodeKthDerivative(var, "_jz_l");
	      rstr = genNodeKthDerivative(right, "_jz_k-_jz_l");	    	    	
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmpm, _jz_theNs[_jz_l]);\n");
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s,%s);\n", rstr, str);
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, tmp2,tmpm);\n");
	      fprintf(outfile, "\t\t\t     AddMyFloatA(tmp2, tmp, tmp1);\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp2);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      rstr = genNodeKthDerivative(right, "_jz_k");	    	
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp2, _jz_theNs[_jz_k]);\n");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmpn, %s,tmp2);\n",rstr);
	      fprintf(outfile, "\t\t\t SubtractMyFloatA(tmp3, tmpn, tmp);\n");
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp, tmp3);\n");
	      rightStr = genNodeRvalueF(right);
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmpn, %s,tmp2);\n",rightStr);
	      fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmpn);\n", name1);
	      fprintf(outfile, "\t\t }\n");
	    }
	  }
	else if(!strcmp(fname, "sqrt"))
	  {
	    fprintf(outfile, "\t\t { /* call sqrt */\n");
	    fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmp;\n");
	    fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmp)\n");		
	    if(total_jet_vars) {
	      fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp;\n");
              fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp)\n");
	    }
            fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
            fprintf(outfile, "\t\t\t  InitMyFloat(tmp);InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
            fprintf(outfile, "\t\t\t  InitMyFloat(tmp3);\n");
	    if(total_jet_vars) {
	      fprintf(outfile, "\t\t\t  InitJet(stmp);InitJet(stmp1);InitJet(stmp2);InitJet(stmp3);\n");	    
	    }
            fprintf(outfile, "\t\t\t }\n");

	    if(VAR_IS_JET(var)) {
	      if(VAR_IS_JET(right)) {
		rstr = genNodeKthDerivative(right, "_jz_k");	    	    
		fprintf(outfile, "\t\t\t AssignJetToJet(stmp,  %s);\n", rstr);
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l< _jz_k; _jz_l++) {\n");
		str = genNodeKthDerivative(var, "_jz_k-_jz_l");
		rstr = genNodeKthDerivative(var, "_jz_l");	    
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp1, %s,%s);\n", str, rstr);
		fprintf(outfile, "\t\t\t     SubtractJetJetA(stmp3, stmp, stmp1);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp3);\n");
		fprintf(outfile, "\t\t\t }\n");
		str = genNodeRvalueF(var);
		fprintf(outfile, "\t\t\t AssignJetToJet(stmp1, %s);\n", str);
		fprintf(outfile, "\t\t\t AddJetJetA(stmp3, stmp1, stmp1);\n");
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, stmp3);\n",name1);
		fprintf(outfile, "\t\t }\n");
	      } else {
		rstr = genNodeKthDerivative(right, "_jz_k");	    	    
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  %s);\n", rstr);
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l< _jz_k; _jz_l++) {\n");
		str = genNodeKthDerivative(var, "_jz_k-_jz_l");
		rstr = genNodeKthDerivative(var, "_jz_l");	    
		fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", str, rstr);
		fprintf(outfile, "\t\t\t     SubtractMyFloatA(tmp3, tmp, tmp1);\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp3);\n");
		fprintf(outfile, "\t\t\t }\n");
		str = genNodeRvalueF(var);
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp1, %s);\n", str);
		fprintf(outfile, "\t\t\t AddMyFloatA(tmp3, tmp1, tmp1);\n");
		fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmp3);\n", "_kthStmp");
		fprintf(outfile, "\t\t\t AssignFloatToJet(%s, %s);\n", name1, "_kthStmp");
		fprintf(outfile, "\t\t }\n");
	      }
	    } else {
	      rstr = genNodeKthDerivative(right, "_jz_k");	    	    
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  %s);\n", rstr);
	      fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l< _jz_k; _jz_l++) {\n");
	      str = genNodeKthDerivative(var, "_jz_k-_jz_l");
	      rstr = genNodeKthDerivative(var, "_jz_l");	    
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp1, %s,%s);\n", str, rstr);
	      fprintf(outfile, "\t\t\t     SubtractMyFloatA(tmp3, tmp, tmp1);\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp3);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      str = genNodeRvalueF(var);
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp1, %s);\n", str);
	      fprintf(outfile, "\t\t\t AddMyFloatA(tmp3, tmp1, tmp1);\n");
	      fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmp3);\n", name1);
	      fprintf(outfile, "\t\t }\n");
	    }
    }

	else if(!strcmp(fname, "sinh") || !strcmp(fname, "cosh"))
	  {
	    Node budy = NODE_BUDY(var);
	    NODE_DERI_DONE(var) = 1;
	    NODE_DERI_DONE(budy) = 1;
	    carg = linearTime(right, &dflag, &mflag);
	    if(carg)
	      {
		fprintf(outfile, "\t\t { /* call sinh cosh */\n");
		if(carg == int_one_node)
		  {
		    if(mflag == 0) /* t+cst */
		      {
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	    
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n", name1, bstr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n", name1, bstr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n", name1, bstr);
			}
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, %s, _jz_oneOverN[_jz_k]);\n",*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, %s, _jz_k);\n",genVarLvalueKth(budy, "_jz_k"), astr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(%s, %s, _jz_theNs[_jz_k]);\n",genVarLvalueKth(budy, "_jz_k"), astr);
			}
		      }
		    else  /* cst-t */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2)\n");		
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);\n");
                        fprintf(outfile, "\t\t\t }\n");

			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	    
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n", bstr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n", bstr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n", bstr);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2,  tmp1);\n");   
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n",name1);
			/*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, _jz_oneOverN[_jz_k]);\n",astr);*/
                        if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			  fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp1, %s, _jz_k);\n",astr);
			} else {
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp1, %s, _jz_theNs[_jz_k]);\n",astr);
			}
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp2,  tmp1);\n");  
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp2);\n", genVarLvalueKth(budy, "_jz_k"));
		      }
		  }
		else 
		  {
		    char  *tstr = genNodeRvalueF(carg);
		    if(mflag == 0) /* a*t+cst */
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp3;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp3)\n");		
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp);InitMyFloat(tmp3);\n");
                        fprintf(outfile, "\t\t\t }\n");

			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	
			if(dflag == 0) {
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp, %s, _jz_oneOverN[_jz_k]);\n", tstr);*/
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, %s, _jz_k);\n", tstr);
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, %s, _jz_theNs[_jz_k]);\n", tstr);
			  }
			} else {
			  /*fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_oneOverN[_jz_k], %s);\n", tstr);*/
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp3, _jz_theNs[_jz_k], %s);\n", tstr);
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_theNs[1],tmp3);\n");
			  */
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp3, _jz_theNs[1], %s);\n",tstr);
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, tmp3, _jz_k);\n");
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, tmp3, _jz_theNs[_jz_k]);\n");
			  }
			}
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp, %s);\n", name1, bstr);
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp, %s);\n", 
				genVarLvalueKth(budy, "_jz_k"), astr);
		      }
		    else
		      {
			fprintf(outfile, "\t\t\t static MY_FLOAT tmp, tmp1, tmp2,tmp3;\n");
			fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp, tmp1, tmp2, tmp3)\n");		
                        fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                        fprintf(outfile, "\t\t\t  InitMyFloat(tmp);InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);\n");
                        fprintf(outfile, "\t\t\t }\n");
			bstr = genNodeKthDerivative(budy, "_jz_k-1");	    
			astr = genNodeKthDerivative(var,  "_jz_k-1");	
			if(dflag == 0) {
			  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp, %s, _jz_oneOverN[_jz_k]);\n", tstr);*/
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, %s, _jz_theNs[_jz_k]);\n", tstr);
			} else {
			  /*fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_oneOverN[_jz_k], %s);\n", tstr);*/
			  /*fprintf(outfile, "\t\t\t multiplyMyFloatA(tmp3, _jz_theNs[_jz_k], %s);\n", tstr);
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, _jz_theNs[1], tmp3);\n", tstr);
			  */
			  fprintf(outfile, "\t\t\t DivideMyFloatA(tmp3, _jz_theNs[1], %s);\n",tstr);
                          if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
			    fprintf(outfile, "\t\t\t DivideMyFloatByInt(tmp, tmp3, _jz_k);\n");
			  } else {
			    fprintf(outfile, "\t\t\t DivideMyFloatA(tmp, tmp3, _jz_theNs[_jz_k]);\n");
			  }

			}
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp2, tmp, %s);\n", bstr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp1,  tmp2);\n");  
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp1);\n", name1);
			fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, tmp, %s);\n", astr);
			fprintf(outfile, "\t\t\t NegateMyFloatA(tmp1,  tmp2);\n");  
			fprintf(outfile, "\t\t\t AssignMyFloat(%s, tmp1);\n", genVarLvalueKth(budy, "_jz_k"));  
		      }
		  }
		fprintf(outfile, "\t\t }\n");		
	      }
	    else /* general sinh cosh */
	      {
		fprintf(outfile, "\t\t { /* call sinh or cosh */\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmp, ctmp;\n");
		fprintf(outfile, "\t\t\t static MY_FLOAT smp1, smp2, smp3, smp;\n");
		fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmp, ctmp, smp1, smp2, smp3, smp)\n");				
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp, sctmp;\n");
		  fprintf(outfile, "\t\t\t static MY_JET ssmp1, ssmp2, ssmp3, ssmp;\n");
                  fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp, sctmp, ssmp1, ssmp2, ssmp3, ssmp)\n");
		}
                fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(tmp);InitMyFloat(ctmp);InitMyFloat(smp1);\n");
                fprintf(outfile, "\t\t\t  InitMyFloat(smp2);InitMyFloat(smp3);InitMyFloat(smp);\n");
		if(total_jet_vars) {
		  fprintf(outfile, "\t\t\t  InitJet(stmp1);InitJet(stmp2); InitJet(stmp3);\n");
                  fprintf(outfile, "\t\t\t  InitJet(stmp);InitJet(sctmp);\n");
                  fprintf(outfile, "\t\t\t  InitJet(ssmp2); InitJet(ssmp3); InitJet(ssmp);\n");
		}
                fprintf(outfile, "\t\t\t }\n");

		if(VAR_IS_JET(var)) {
		  if(VAR_IS_JET(right)) { // Jet=sin(Jet)
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t AssignFloatToJet(ssmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		    fprintf(outfile, "\t\t\t     AssignFloatToJet(stmp1, _jz_theNs[_jz_l]);\n");
		    rstr = genNodeKthDerivative(right, "_jz_l");	    	    
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(sctmp, stmp1, %s);\n", rstr);
		    bstr = genNodeKthDerivative(budy, "_jz_k-_jz_l");
		    astr = genNodeKthDerivative(var, "_jz_k-_jz_l");
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, sctmp, %s);\n", bstr);
		    fprintf(outfile, "\t\t\t     MultiplyJetJetA(ssmp2, sctmp, %s);\n", astr);
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp3, stmp, stmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp3);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(ssmp3, ssmp, ssmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(ssmp, ssmp3);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", name1);
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, ssmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  } else { /* jet = sinh(float) */
		    fprintf(outfile, "\t\t\t AssignFloatToJet(stmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t AssignFloatToJet(ssmp,  _jz_MyFloatZERO);\n");
		    fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		    fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_l]);\n");
		    rstr = genNodeKthDerivative(right, "_jz_l");	    	    
		    fprintf(outfile, "\t\t\t     MultiplyMyFloatA(ctmp, tmp1, %s);\n", rstr);
		    bstr = genNodeKthDerivative(budy, "_jz_k-_jz_l");
		    astr = genNodeKthDerivative(var, "_jz_k-_jz_l");
		    fprintf(outfile, "\t\t\t     MultiplyFloatJetA(stmp2, ctmp, %s);\n", bstr);
		    fprintf(outfile, "\t\t\t     MultiplyFloatJetA(ssmp2, ctmp, %s);\n", astr);
		    fprintf(outfile, "\t\t\t     AddJetJetA(stmp3, stmp, stmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp3);\n");
		    fprintf(outfile, "\t\t\t     AddJetJetA(ssmp3, ssmp, ssmp2);\n");
		    fprintf(outfile, "\t\t\t     AssignJetToJet(ssmp, ssmp3);\n");
		    fprintf(outfile, "\t\t\t }\n");
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, stmp, _jz_theNs[_jz_k]);\n", name1);
		    fprintf(outfile, "\t\t\t DivideJetFloatA(%s, ssmp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  }
		} else { /* float = sinh(float) */
		  fprintf(outfile, "\t\t\t AssignMyFloat(tmp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t AssignMyFloat(smp,  _jz_MyFloatZERO);\n");
		  fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<=_jz_k; _jz_l++) {\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_l]);\n");
		  rstr = genNodeKthDerivative(right, "_jz_l");	    	    
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(ctmp, tmp1, %s);\n", rstr);
		  bstr = genNodeKthDerivative(budy, "_jz_k-_jz_l");
		  astr = genNodeKthDerivative(var, "_jz_k-_jz_l");
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, ctmp, %s);\n", bstr);
		  fprintf(outfile, "\t\t\t     MultiplyMyFloatA(smp2, ctmp, %s);\n", astr);
		  fprintf(outfile, "\t\t\t     AddMyFloatA(tmp3, tmp, tmp2);\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp3);\n");
		  fprintf(outfile, "\t\t\t     AddMyFloatA(smp3, smp, smp2);\n");
		  fprintf(outfile, "\t\t\t     AssignMyFloat(smp, smp3);\n");
		  fprintf(outfile, "\t\t\t }\n");
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, tmp, _jz_oneOverN[_jz_k]);\n", name1);*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, tmp, _jz_k);\n", name1);
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, _jz_theNs[_jz_k]);\n", name1);
		  }
		  /*fprintf(outfile, "\t\t\t MultiplyMyFloatA(%s, smp, _jz_oneOverN[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));*/
                  if(my_float_arith == ARITH_MPFR || my_float_arith == ARITH_GMP) {
		    fprintf(outfile, "\t\t\t DivideMyFloatByInt(%s, smp, _jz_k);\n", genVarLvalueKth(budy, "_jz_k"));
		  } else {
		    fprintf(outfile, "\t\t\t DivideMyFloatA(%s, smp, _jz_theNs[_jz_k]);\n", genVarLvalueKth(budy, "_jz_k"));
		  }
		}
		fprintf(outfile, "\t\t }\n");
	      }
	  }
	else if(!strcmp(fname, "tan") || !strcmp(fname, "tanh") || !strcmp(fname, "atan") || !strcmp(fname, "arctan"))
	  {
	    Node budy = NODE_BUDY(var);
	    fprintf(outfile, "\t\t { /* call tan or tanh or atan */\n");

	    fprintf(outfile, "\t\t\t static MY_FLOAT tmp1, tmp2, tmp3, tmp4, tmp;\n");
	    fprintf(outfile, "\t\t\t #pragma omp threadprivate(tmp1, tmp2, tmp3, tmp4, tmp)\n");				
	    if(total_jet_vars) {
	      fprintf(outfile, "\t\t\t static MY_JET stmp1, stmp2, stmp3, stmp4, stmp;\n");
              fprintf(outfile, "\t\t\t #pragma omp threadprivate(stmp1, stmp2, stmp3, stmp4, stmp)\n");
	    }
	    fprintf(outfile, "\t\t\t if(_jz_initialized==0) {\n");
	    fprintf(outfile, "\t\t\t  InitMyFloat(tmp1);InitMyFloat(tmp2);InitMyFloat(tmp3);\n");
	    fprintf(outfile, "\t\t\t  InitMyFloat(tmp4);InitMyFloat(tmp);\n");
	    if(total_jet_vars) {
	      fprintf(outfile, "\t\t\t  InitJet(stmp1);InitJet(stmp2);InitJet(stmp3);\n");
	      fprintf(outfile, "\t\t\t  InitJet(stmp4);InitJet(stmp);\n");
	    }
	    fprintf(outfile, "\t\t\t }\n");

	    if(VAR_IS_JET(var)) {
	      if(VAR_IS_JET(right)) { // Jet=tan(Jet)
		rstr = genNodeKthDerivative(right, "_jz_k");	    	    
		fprintf(outfile, "\t\t\t AssignJetToJet(stmp2,  %s);\n", rstr);
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp4, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmp,  stmp2, tmp);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<_jz_k; _jz_l++) {\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_l]);\n");
		bstr = genNodeKthDerivative(budy, "_jz_k-_jz_l");
		str = genNodeKthDerivative(var, "_jz_l");	    	    
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, %s,%s);\n", bstr, str);
		fprintf(outfile, "\t\t\t     MultiplyFloatJetA(stmp3, tmp1, stmp2);\n");
		fprintf(outfile, "\t\t\t     SubtractJetJetA(stmp4, stmp, stmp3);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp4);\n");
		fprintf(outfile, "\t\t\t }\n");
		bstr = genNodeRvalueF(budy);
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp4, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmp1, %s, tmp4);\n", bstr);
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, stmp1);\n", name1);
	      } else { // jet = tan(float)
		rstr = genNodeKthDerivative(right, "_jz_k");	    	    
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp2,  %s);\n", rstr);
		fprintf(outfile, "\t\t\t AssignFloatToJet(stmp4, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmp,  stmp2, tmp);\n");
		fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<_jz_k; _jz_l++) {\n");
		fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_l]);\n");
		bstr = genNodeKthDerivative(budy, "_jz_k-_jz_l");
		str = genNodeKthDerivative(var, "_jz_l");	    	    
		fprintf(outfile, "\t\t\t     MultiplyJetJetA(stmp2, %s,%s);\n", bstr, str);
		fprintf(outfile, "\t\t\t     MultiplyFloatJetA(stmp3, tmp1, stmp2);\n");
		fprintf(outfile, "\t\t\t     SubtractJetJetA(stmp4, stmp, stmp3);\n");
		fprintf(outfile, "\t\t\t     AssignJetToJet(stmp, stmp4);\n");
		fprintf(outfile, "\t\t\t }\n");
		bstr = genNodeRvalueF(budy);
		fprintf(outfile, "\t\t\t AssignMyFloat(tmp4, _jz_theNs[_jz_k]);\n");
		fprintf(outfile, "\t\t\t MultiplyJetFloatA(stmp1, %s, tmp4);\n", bstr);
		fprintf(outfile, "\t\t\t DivideJetJetA(%s, stmp, stmp1);\n", name1);
	      }
	    } else {
	      rstr = genNodeKthDerivative(right, "_jz_k");	    	    
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp2,  %s);\n", rstr);
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp4, _jz_theNs[_jz_k]);\n");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp,  tmp2, tmp4);\n");
	      fprintf(outfile, "\t\t\t for(_jz_l=1; _jz_l<_jz_k; _jz_l++) {\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmp1, _jz_theNs[_jz_l]);\n");
	      bstr = genNodeKthDerivative(budy, "_jz_k-_jz_l");
	      str = genNodeKthDerivative(var, "_jz_l");	    	    
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp2, %s,%s);\n", bstr, str);
	      fprintf(outfile, "\t\t\t     MultiplyMyFloatA(tmp3, tmp1, tmp2);\n");
	      fprintf(outfile, "\t\t\t     SubtractMyFloatA(tmp4, tmp, tmp3);\n");
	      fprintf(outfile, "\t\t\t     AssignMyFloat(tmp, tmp4);\n");
	      fprintf(outfile, "\t\t\t }\n");
	      bstr = genNodeRvalueF(budy);
	      fprintf(outfile, "\t\t\t AssignMyFloat(tmp4, _jz_theNs[_jz_k]);\n");
	      fprintf(outfile, "\t\t\t MultiplyMyFloatA(tmp1, %s, tmp4);\n", bstr);
	      fprintf(outfile, "\t\t\t DivideMyFloatA(%s, tmp, tmp1);\n", name1);
	    }
	    fprintf(outfile, "\t\t }\n");
	  }
      }
    break;      
    case IF_ELSE_EXPR:
      fprintf(outfile, "\t\t /* if_else: %s=%s */\n", NODE_NAME(var), NODE_INFO(def));
      {
	Node cond = IF_ELSE_COND(var);
	char *boolstr = genNodeRvalueI(cond);
	leftStr = genNodeKthDerivative(left, "_jz_k");
	rightStr = genNodeKthDerivative(right, "_jz_k");	    	    
	fprintf(outfile, "\t\t if(%s) {\n", boolstr);
	fprintf(outfile, "\t\t\t AssignMyFloat(%s,%s);\n\t\t } else {\n",name1,leftStr);
	fprintf(outfile, "\t\t\t AssignMyFloat(%s,%s);\n\t\t }\n",name1,rightStr); 
      }
    break;    
    default:
      fprintf(stderr, "Internal error: don't know how to differentiate %s=%s\n",
              NODE_GIVEN_NAME(var)? NODE_GIVEN_NAME(var) : NODE_NAME(var), 
              NODE_GIVEN_NAME(def)? NODE_GIVEN_NAME(def): NODE_INFO(def));
      break;
    }
}

/*************************************************************************************************/
static char *genNodeKthDerivative(Node node, char *kth)
{
  static char str[2048];
  static char *outs[16]={ str, str+128, str+256, str+384, str+512, str+640, str+768, str+896,
			  str+1024, str+1152, str+1280,str+1408,str+1536,str+1664,str+1792,str+1920};
  static int  oidx = 0;
  char   *out = outs[oidx];
  out[0] = '\0';
  oidx++; if(oidx==16) oidx = 0;

  if(node)
    {
      if(NODE_IS_CST(node)) return("_jz_MyFloatZERO");
      else
	{
	  int code = NODE_CODE(node);
	  switch(code)
	    {
	    case ID_NODE:
	      if(VAR_IS_LOCAL(node) || VAR_IS_GLOBAL(node))
		return(genVarLvalueKth(node, kth));
	      else 
		{
		  fprintf(stderr, "Internal Error: don't know the derivative of '%s'.\n", 
                          NODE_GIVEN_NAME(node)?NODE_GIVEN_NAME(node): NODE_NAME(node));
		  exit(51);
		}
	      break;
	    default:
	      fprintf(stderr, "Internal Error: don't know the derivative of '%s' (%d)\n", 
                      NODE_GIVEN_NAME(node)? NODE_GIVEN_NAME(node): NODE_NAME(node),code);
	      exit(52);
	      break;
	    }
	}
    }
  return(NULL);
}
/*************************************************************************************************/
