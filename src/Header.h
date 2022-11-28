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

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * first, set the coding
 */
#ifndef NODE_C
extern char  *node_type[];
#else
#undef NODE_CODE
#define NODE_CODE(CODE,NAME) NAME,
char *node_names[] = {
#include "NodeCode.h"
  NULL,
};

#endif /* NODE_C */
#undef NODE_CODE
#define NODE_CODE(CODE,NAME) CODE,
enum node_code {
#include "NodeCode.h"
  LAST_CODE
};
#undef NODE_CODE
/*******************************************************************/
/*
 *  tree node
 */
struct node_
{
  struct node_   *left;    
  struct node_   *right;  
  struct node_   *cond;    /* for if .. else .. expr */
  struct node_   *next;    /* to chain variables */
  
  int            code;
  char           *name;    /* for terminal nodes: id, INTCON, FLOATCON    */
  char           *givenName;  /* for id only */
  int            infoLength;
  char           *info;
  char           *jinitv;   /* for jet var init v */
  unsigned long  hvalue;   /* info hash value    */
  double         cstDvalue;
  long           cstIvalue;
  
  struct node_   *budy;    /* for sin, cos and ??          */
  struct node_   *depa;    /* for id notes, mark dependent */
  struct node_   *depb;    /* vars. */
  struct node_   *depc;    /* vars. */
  void           *sumc;    /* for SUM block */

  int            idx;      /* location in var array        */
  int            cidx;     /* location in constant list    */
  int            vidx;     /* location in var list         */
  int            bidx;     /* location in boolean var list */
  int            voffset;  
  int            coffset;
  int            ioffset;
  int            flags;    /* all kinds of flags */
  int            flags2;   /* all kinds of flags */
  int            jidx;     /* index in jet array */
  int            sidx;     /* index in jet def */
  int            eidx;     /* in which equation */
  int            pidx;     /* jet parameter index */
  char           isarray;
  char           inttype;
  char           deridone;
  char           cpad;
};

#define ID_INT_TYPE(NODE)          ((NODE)->inttype)
#define ID_VOFFSET(NODE)           ((NODE)->voffset)
#define ID_COFFSET(NODE)           ((NODE)->coffset)
#define ID_IOFFSET(NODE)           ((NODE)->ioffset)
#define NODE_FLAGS(NODE)           ((NODE)->flags)
#define NODE_SUM_BLOCK(NODE)       ((NODE)->sumc)
#define IF_ELSE_COND(NODE)         ((NODE)->cond)
#define ID_IS_ARRAY(NODE)          ((NODE)->isarray)
#define NODE_DVALUE(NODE)          ((NODE)->cstDvalue)
#define NODE_IVALUE(NODE)          ((NODE)->cstIvalue)
#define NODE_BUDY(NODE)            ((NODE)->budy)
#define ID_BUDY(NODE)              ((NODE)->budy)
#define ID_DEPEND_A(NODE)          ((NODE)->depa)
#define ID_DEPEND_B(NODE)          ((NODE)->depb)
#define ID_DEPEND_C(NODE)          ((NODE)->depc)
#define NODE_DEPEND_A(NODE)        ((NODE)->depa)
#define NODE_DEPEND_B(NODE)        ((NODE)->depb)
#define NODE_DEPEND_C(NODE)        ((NODE)->depc)
#define NODE_LEFT(NODE)            ((NODE)->left)
#define NODE_RIGHT(NODE)           ((NODE)->right)
#define NODE_NEXT(NODE)            ((NODE)->next)
#define NODE_CODE(NODE)            ((NODE)->code)
#define NODE_INFO_LENGTH(NODE)     ((NODE)->infoLength)
#define NODE_INFO(NODE)            ((NODE)->info)
#define NODE_JINITV(NODE)          ((NODE)->jinitv)
#define NODE_INFO_HASH(NODE)       ((NODE)->hvalue)
#define ID_V_INDEX(NODE)           ((NODE)->vidx)
#define NODE_V_INDEX(NODE)         ((NODE)->vidx)
#define ID_VINDEX(NODE)            ((NODE)->idx)
#define ID_CINDEX(NODE)            ((NODE)->cidx)
#define ID_IINDEX(NODE)            ((NODE)->bidx)
#define ID_JINDEX(NODE)            ((NODE)->jidx)
#define ID_J_INDEX(NODE)           ((NODE)->jidx)
#define ID_SINDEX(NODE)            ((NODE)->sidx)
#define ID_EINDEX(NODE)            ((NODE)->eidx)
#define NODE_EINDEX(NODE)          ((NODE)->eidx)
#define ID_PINDEX(NODE)            ((NODE)->pidx)
#define NODE_PINDEX(NODE)          ((NODE)->pidx)
#define ID_NAME(NODE)              ((NODE)->name)
#define NODE_NAME(NODE)            ((NODE)->name)
#define NODE_DERI_DONE(NODE)       ((NODE)->deridone)
#define NODE_DERI_DONE(NODE)       ((NODE)->deridone)
#define ID_GIVEN_NAME(NODE)        ((NODE)->givenName)
#define NODE_GIVEN_NAME(NODE)      ((NODE)->givenName)

/*********************************************************************************************/
#define  IS_CST_BIT                 (1 << 0)
#define  IS_INT_BIT                 (1 << 1)
#define  IS_FUNC_BIT                (1 << 2)
#define  IS_VAR_BIT                 (1 << 3)
#define  IS_DEFINED_BIT             (1 << 4)
#define  IS_DELETED_BIT             (1 << 5)
#define  IS_GCOUNTED_BIT            (1 << 6)
#define  IS_COUNTED_BIT             (1 << 7)
#define  IS_CCOUNTED_BIT            (1 << 8)
#define  IS_ICOUNTED_BIT            (1 << 9)

#define  VAR_USED_BIT               (1 << 12)
#define  IS_LOCAL_BIT               (1 << 13)
#define  IS_SIDX_BIT                (1 << 14)
#define  INTERNAL_SUM_BIT           (1 << 15)
#define  LOCAL_COUNTED              (1 << 16)
#define  IS_A_NUMBER_BIT            (1 << 17)
#define  INTERNAL_INDEX_BIT         (1 << 18)
#define  GVAR_BIT                   (1 << 19)
#define  GCVAR_BIT                  (1 << 20)
#define  GIVAR_BIT                  (1 << 21)
#define  LVAR_BIT                   (1 << 22)
#define  LCVAR_BIT                  (1 << 23)
#define  LIVAR_BIT                  (1 << 24)
#define  LMARK_1                    (1 << 25)
#define  LMARK_2                    (1 << 26)
#define  DEP_BIT                    (1 << 27)
#define  KEEP_BIT                   (1 << 28)
#define  IS_EXTERN_BIT              (1 << 29)


/*********************************************************************************************/

#define  KEEP_ME(NODE)              ((NODE)->flags & KEEP_BIT)
#define  SET_KEEP_ME(NODE)          {(NODE)->flags |= KEEP_BIT;}

#define  VAR_DEPENDED(NODE)         ((NODE)->flags & DEP_BIT)
#define  SET_VAR_DEPENDED(NODE)     {(NODE)->flags |= DEP_BIT;}

#define  VAR_IS_MARKED1(NODE)       ((NODE)->flags & LMARK_1)
#define  SET_VAR_IS_MARKED1(NODE)   {(NODE)->flags |= LMARK_1;}
#define  VAR_IS_MARKED2(NODE)       ((NODE)->flags & LMARK_2)
#define  SET_VAR_IS_MARKED2(NODE)   {(NODE)->flags |= LMARK_2;}

#define  VAR_IS_LOCAL(NODE)         ((NODE)->flags &(LVAR_BIT | LCVAR_BIT | LIVAR_BIT))
#define  VAR_IS_GLOBAL(NODE)        ((NODE)->flags &(GVAR_BIT | GCVAR_BIT | GIVAR_BIT))
#define  SET_VAR_LOCAL_VAR(NODE)    {(NODE)->flags |= LVAR_BIT;}
#define  SET_VAR_LOCAL_CVAR(NODE)   {(NODE)->flags |= LCVAR_BIT;}
#define  SET_VAR_LOCAL_IVAR(NODE)   {(NODE)->flags |= LIVAR_BIT;}
#define  SET_VAR_GLOBAL_VAR(NODE)   {(NODE)->flags |= GVAR_BIT;}
#define  SET_VAR_GLOBAL_CVAR(NODE)  {(NODE)->flags |= GCVAR_BIT;}
#define  SET_VAR_GLOBAL_IVAR(NODE)  {(NODE)->flags |= GIVAR_BIT;}

#define  VAR_IS_LOCAL_VAR(NODE)    ((NODE)->flags & LVAR_BIT)
#define  VAR_IS_LOCAL_CVAR(NODE)   ((NODE)->flags & LCVAR_BIT)
#define  VAR_IS_LOCAL_IVAR(NODE)   ((NODE)->flags & LIVAR_BIT)
#define  VAR_IS_GLOBAL_VAR(NODE)   ((NODE)->flags & GVAR_BIT)
#define  VAR_IS_GLOBAL_CVAR(NODE)  ((NODE)->flags & GCVAR_BIT)
#define  VAR_IS_GLOBAL_IVAR(NODE)  ((NODE)->flags & GIVAR_BIT)

/*********************************************************************************************/

#define  IS_JET_BIT                 (1 << 0)
#define  IS_JET_PARAM_BIT           (1 << 1)
#define  IS_STATE_VAR_BIT           (1 << 2)
#define  JET_DEFINED_BIT            (1 << 3)
#define  JET_COUNTED_BIT            (1 << 4)
#define  JET_DEP_MARKER_BIT         (1 << 5)
#define  JET_IS_DECLARED_BIT        (1 << 6)    

/*********************************************************************************************/
#define  VAR_IS_JET(NODE)             ((NODE)->flags2 & IS_JET_BIT)
#define  SET_VAR_IS_JET(NODE)         {(NODE)->flags2 |= IS_JET_BIT;}

#define  VAR_IS_JET_PARAM(NODE)       ((NODE)->flags2 & IS_JET_PARAM_BIT)
#define  SET_VAR_IS_JET_PARAM(NODE)   {(NODE)->flags2 |= IS_JET_PARAM_BIT;}

#define  VAR_IS_STATE_VAR(NODE)       ((NODE)->flags2 & IS_STATE_VAR_BIT)
#define  SET_VAR_IS_STATE_VAR(NODE)   {(NODE)->flags2 |= IS_STATE_VAR_BIT;}

#define  JET_IS_DEFINED(NODE)         ((NODE)->flags2 & JET_DEFINED_BIT)
#define  SET_JET_IS_DEFINED(NODE)     {(NODE)->flags2 |= JET_DEFINED_BIT;}

#define  JET_VAR_COUNTED(NODE)     ((NODE)->flags2 & JET_COUNTED_BIT)
#define  SET_JET_VAR_COUNTED(NODE) {(NODE)->flags2 |= JET_COUNTED_BIT;}

#define  SET_JET_DEP_MARKER(NODE)       {(NODE)->flags2 |= JET_DEP_MARKER_BIT;}
#define  IS_JET_DEP_MARKED(NODE)        ((NODE)->flags2 & JET_DEP_MARKER_BIT)
#define  CLEAR_JET_DEP_MARKER(NODE)     {(NODE)->flags2 &= ~JET_DEP_MARKER_BIT;}

#define  VAR_IS_DECLARED_JET(NODE)     ((NODE)->flags2 & JET_IS_DECLARED_BIT)
#define  SET_VAR_IS_DECLARED_JET(NODE) {(NODE)->flags2 |= JET_IS_DECLARED_BIT;}
  
/*********************************************************************************************/
#define NODE_IS_INDEX(NODE)          (((NODE)->flags) &  INTERNAL_INDEX_BIT)       
#define SET_NODE_IS_INDEX(NODE)      {((NODE)->flags) |= INTERNAL_INDEX_BIT;}

#define NODE_IS_A_NUMBER(NODE)       (((NODE)->flags) &  IS_A_NUMBER_BIT)       
#define SET_NODE_IS_A_NUMBER(NODE)   {((NODE)->flags) |= IS_A_NUMBER_BIT;}

#define ID_IS_COUNT_LOCAL(NODE)      (((NODE)->flags) &  LOCAL_COUNTED)       
#define SET_ID_IS_COUNT_LOCAL(NODE)  {((NODE)->flags) |= LOCAL_COUNTED;}

#define IS_INTERNAL_SUM(NODE)     (((NODE)->flags) &  INTERNAL_SUM_BIT)
#define SET_IS_INTERNAL_SUM(NODE) {((NODE)->flags) |= INTERNAL_SUM_BIT;}

#define ID_IS_SIDX(NODE)          (((NODE)->flags) &  IS_SIDX_BIT)       
#define SET_ID_IS_SIDX(NODE)      {((NODE)->flags) |=  IS_SIDX_BIT;}

#define NODE_IS_LOCAL(NODE)        (((NODE)->flags) &  IS_LOCAL_BIT)       
#define SET_NODE_IS_LOCAL(NODE)    {((NODE)->flags) |=  IS_LOCAL_BIT;}       

#define  VAR_IS_USED(NODE)         (((NODE)->flags) &   VAR_USED_BIT)       
#define  SET_VAR_IS_USED(NODE)     {((NODE)->flags) |=  VAR_USED_BIT;}

#define NODE_IS_BOOL(NODE)        (((NODE)->flags) &  IS_BOOL_BIT)       
#define SET_NODE_IS_BOOL(NODE)    {((NODE)->flags) |=  IS_BOOL_BIT;}

#define ID_IS_EXTERN(NODE)         (((NODE)->flags) &  IS_EXTERN_BIT)       
#define SET_ID_IS_EXTERN(NODE)     {((NODE)->flags) |=  IS_EXTERN_BIT;}
#define NODE_IS_EXTERN(NODE)       (((NODE)->flags) &  IS_EXTERN_BIT)       
#define SET_NODE_IS_EXTERN(NODE)   {((NODE)->flags) |=  IS_EXTERN_BIT;}

#define NODE_COUNTED(NODE)         (((NODE)->flags) &  IS_COUNTED_BIT)       
#define SET_NODE_COUNTED(NODE)     {((NODE)->flags) |=  IS_COUNTED_BIT;}

#define NODE_CCOUNTED(NODE)         (((NODE)->flags) &  IS_CCOUNTED_BIT)       
#define SET_NODE_CCOUNTED(NODE)     {((NODE)->flags) |=  IS_CCOUNTED_BIT;}

#define NODE_ICOUNTED(NODE)         (((NODE)->flags) &  IS_ICOUNTED_BIT)       
#define SET_NODE_ICOUNTED(NODE)     {((NODE)->flags) |=  IS_ICOUNTED_BIT;}

#define ID_COUNTED(NODE)           (((NODE)->flags) &  IS_COUNTED_BIT)       
#define SET_ID_COUNTED(NODE)       {((NODE)->flags) |=  IS_COUNTED_BIT;}

#define IS_GCOUNTED(NODE)          (((NODE)->flags) &  IS_GCOUNTED_BIT)       
#define SET_IS_GCOUNTED(NODE)      {((NODE)->flags) |=  IS_GCOUNTED_BIT;}

#define ID_IS_DELETED(NODE)       (((NODE)->flags) &  IS_DELETED_BIT)       
#define SET_ID_IS_DELETED(NODE)   {((NODE)->flags) |=  IS_DELETED_BIT;} 

#define ID_IS_DEFINED(NODE)       (((NODE)->flags) &  IS_DEFINED_BIT)      
#define SET_ID_IS_DEFINED(NODE)   {((NODE)->flags) |=  IS_DEFINED_BIT;}

#define ID_IS_VAR(NODE)           (((NODE)->flags) &  IS_VAR_BIT)
#define SET_ID_IS_VAR(NODE)       {((NODE)->flags) |=  IS_VAR_BIT;}

#define ID_IS_FUNC(NODE)           (((NODE)->flags) &  IS_FUNC_BIT)
#define SET_ID_IS_FUNC(NODE)       {((NODE)->flags) |= IS_FUNC_BIT;}

#define ID_IS_CST(NODE)            (((NODE)->flags) &  IS_CST_BIT)
#define SET_ID_IS_CST(NODE)        {((NODE)->flags) |= IS_CST_BIT;}
#define NODE_IS_CST(NODE)          (((NODE)->flags) &  IS_CST_BIT)
#define SET_NODE_IS_CST(NODE)      {((NODE)->flags) |= IS_CST_BIT;}

#define ID_IS_INT(NODE)            (((NODE)->flags) &  IS_INT_BIT)
#define SET_ID_IS_INT(NODE)        {((NODE)->flags) |= IS_INT_BIT;}
#define NODE_IS_INT(NODE)          (((NODE)->flags) &  IS_INT_BIT)
#define SET_NODE_IS_INT(NODE)      {((NODE)->flags) |= IS_INT_BIT;}
/*********************************************************************************************/
typedef struct node_ *Node;
typedef struct node_ *node;
#define NULL_NODE (node)NULL
/*********************************************************************************************/
typedef struct sum_
{
  Node  expr;       /* the expression to sum over */
  Node  idx;        /* running idx node           */
  int   from, to;   /* range for summands. No. terms = to-from+1 */
  
  /* for output codes */
  Node  *lvarlist;  /* list of local vars */
  Node  *lcstlist;  /* list of local csts */
  Node  *lintlist;  /* list of local ints */
  Node  *all;       /* all local variables */
  int   lvar;       /* local var count  */
  int   lcst;       /* local cst count  */
  int   lint;       /* local  int cst count */
  int   voffset;    /* tmp var starting index */
  int   coffset;    /* tmp var starting index */
  int   ioffset;    /* tmp var starting index */

} sumBlock, *SumP;

#define SUMB_EXPR(s)        ((s)->expr)
#define SUMB_IDX(s)         ((s)->idx)
#define SUMB_FROM(s)        ((s)->from)
#define SUMB_TO(s)          ((s)->to)
#define SUMB_LVAR(s)        ((s)->lvar)
#define SUMB_LCST(s)        ((s)->lcst)
#define SUMB_LINT(s)        ((s)->lint)
#define SUMB_ALL_VARS(s)    ((s)->all)

#define SUMB_LVAR_LIST(s)        ((s)->lvarlist)
#define SUMB_LCST_LIST(s)        ((s)->lcstlist)
#define SUMB_LINT_LIST(s)        ((s)->lintlist)

#define SUMB_VOFFSET(s)     ((s)->voffset)
#define SUMB_COFFSET(s)     ((s)->coffset)
#define SUMB_IOFFSET(s)     ((s)->ioffset)

/*********************************************************************************************/
typedef  struct variable_
{
  Node  name, def;
} *Variable, *Equation, *ExternVar;
/*********************************************************************************************/
typedef struct control_
{
  int  ninitials, space;
  Node *initials;
} Control;
/*********************************************************************************************/
#ifndef NODE_C
extern Node error_node;
extern Node int_zero_node;
extern Node int_one_node;
extern Node current_id;
extern Node id_list;
extern Node timeVar;
extern char *current_qstring;
#endif

#ifndef PARSE_C
extern int neqns, nvars, varSpace, nexterns, externSpace, eqnSpace;
extern Variable   variables;
extern Equation   equations;
extern Equation   equations;
extern ExternVar  externVars;
extern int        nonautonomous, etype;
extern Control    controlParams;
extern Node      *jetVars;
extern int       njetVars;
#endif

#ifndef CODE_C
extern Node *varlistFinal;
extern int   varcountFinal;
extern Node  *constlistFinal;
extern int   constcountFinal;
extern Node  *ivarFinal;
extern int    ivarcountFinal;
extern Node  *allVarsList;
extern Node  *jetVarsList;
extern Node  *jetParamsList;
extern int    allVarsCount;
extern int    goffset;
extern int    igoffset;
extern int    cgoffset;
extern char  *jmarks;
extern char  *kmarks;
#endif
#ifndef SUM_C
extern int insum;
#endif

#ifndef MY_JET_CODE_C
extern int deg_jet_vars;
extern int num_jet_vars;
extern int num_symbols;
extern int state_jet_vars;
extern int param_jet_vars;
extern int total_jet_vars;
extern int all_vars_are_jets;

extern char *my_float_api_macros;
extern char *my_jet_prefixes[];
extern char *my_jet_headers[];
extern char *my_jet_codes[];

extern void genMyJetHeader(void);
extern void genMyJetCode(void);
#endif
#include "my_jet_header.h"

#ifndef MAIN_C
extern int   debug;
extern int   ddouble, cdouble;
extern int   foldcst;
extern int   expandsum;
extern int   expandpower;
extern int   havesum;
extern int   genHeader;
extern int   genMain;
extern int   genStep;
extern int   genJet;
extern int   genTestJet;
extern int   genMyJet;
extern int   ignoreJetInconsistency;
extern int   genJetHelper;
extern int   jetStorageType;
extern int   genSeries;
extern int   genSeries;
extern int   use_rational_exponent;
extern int   gmp, gmp_precision,mpfr,mpfr_precision,mpc,mpc_precision[2];
extern int   ldouble, qd2, qd4, float128;
extern char  *outNames[10];
extern char  *suffixes[10];
extern int   num_names,f77hook;
extern int   step_ctrl;
extern char  *outName;
extern char  *suffix;
extern char  *my_jet_prefix;
extern index_my_jet_header_t index_my_jet_header;
extern index_my_jet_code_t index_my_jet_code;
extern char  *header_name;
extern char  *uss_name, *uso_name;
extern FILE  *infile, *outfile;
#endif
/*********************************************************************************************/

/******************************************************************
 * 
 *            Some forward declarations
 *
 ******************************************************************/
void initialize_nodes(void);
void initialize_vars(void);
Node make_node(enum node_code code);
void delete_node(Node node);
Node install_id(char *lexime);
Node build_int(char  *lexime);
Node build_float(char  *lexime);
Node build_op(enum node_code code, Node op1, Node op2);
Node build_bop(enum node_code code, Node op1, Node op2);
Node copy_node(Node a);
Node getTmpVar(void);
char *AllocCopyString(char *str);
char *ReallocCopyString(char *old, char *str);
char *my_malloc(int size);
char *my_realloc(void *p, int s);
void my_free(void *ptr);
void canonicalTree(Node node);
void parse_one_equation(Node left, Node var, Node right);
void define_one_variable(Node left, Node right) ;
void insert_one_variable(Node left, Node rignt);
void identifyFunctions(void);   
void identifyConstants(void);
int nodeIsNumber(Node node);
void checkVars(void);
void checkJetVars(void);
void checkEquations(void);
void makeCompanionVariables(void);
void decompose(void);
void genVariables(void);
void genCode(void);
void showPaserInfo(void);
unsigned long hash(char *str);
Node expand_sum(Node expr, Node var, Node ifrm, Node ito);
Node build_sum(Node expr, Node var, Node ifrm, Node ito);
Node declareArray(Node id, Node nothing);
Node declareExternVar(Node var);
Node markJet(Node var);
int  markAllVarsJet();
Node markJetParameter(Node var);
Node build_array_ref(Node var, Node idx);
Node build_array_idx(Node current, Node onemore);
void check_jet_params(void);
void check_array_ref(Node id);
Node build_if_else(Node be, Node ife, Node ele);
int  mark_local(SumP sum, Node expr, Node idx);
void countSumVariables(Node def);
void showVars(void);
void makeDependencyGraph(Node name, Node def);
void outputGVarsInSum(SumP sum);
void outputCVarsInSum(SumP sum);
void outputIVarsInSum(SumP sum);
void markUsedExpressions(Node node);
void markCondVars(Node cond);
void listAllVarsIn(Node def);
void mark_all_depend_vars(Node def);
void printSumBlockVariables(FILE *fp, SumP sum);
void outputSumInitialVars(SumP sum);
void outputInitialVar(Node var);
void outputSumKthDerivative(SumP sum);
void outputVarKthDerivative(Node var);
void sumUptoVar(SumP sum, Node var);
void sumUptoVarKth(SumP sum, Node var);
void showVariables(FILE *fp);
int  intNodeValue(Node def, int *jnk);
int  findExtern(Node var);
int  isRational(Node var, Node *num, Node *den);
void outputExtern(void);
void addInitial(Node n);
void genMainCode(void);
void genStepCode(void);
void genSampleHeader(void);
void genJetHelpers(int flag);
double cstNodeValue(Node node);
char *cstNodeStringValue(Node node, char *rbuf);
Node linearTime(Node node, int *dflag, int *mflag);
Node scaleTime(Node node, int *dflag);
int getControlParameterValue(char *str, double *value_return, Node *node_return);
void record_jet_initv(Node id, char *val);
void output_jet_multiplication_code(void);
void print_jet_tree_monomial_index_mapper();
/******************************************************************/
int matchWord(char *, char *, int, int *);

#endif /* HEADER_H */
