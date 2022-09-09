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

/**********************************************************************
 *
 *      Coding for syntax tree nodes
 *
 *********************************************************************/
/*
 * The coding is specified in the following manner:
 *      code, description, type, length
 * where CODE is a enum type, specifying which node it is;
 * DESCRIPTION is a human readble description of the node;
 * TYPE is a string specifing the node type; LENGTH is the
 * expected length of the node.
 */
/*
 *  to specify IDENTIFIER, OPERATOR, and LIST_OF_NODES
 */
NODE_CODE(ID_NODE,      "id_node"    )
NODE_CODE(INT_CST,      "int_cst"    )
NODE_CODE(FLOAT_CST,    "float_cst"  )
NODE_CODE(PLUS_EXPR,    "plus_expr"  )
NODE_CODE(MINUS_EXPR,   "minus_expr" )
NODE_CODE(MULT_EXPR,    "mult_expr"  )
NODE_CODE(DIV_EXPR,     "div_expr"   )
NODE_CODE(EXP_EXPR,     "exp_expr"   )
NODE_CODE(NEGATE_EXPR,  "negate_expr")  /* not boolean */
NODE_CODE(ARRAY_IDX,    "arrayIdx"   )
NODE_CODE(ARRAY_REF,    "array_ref"  )
NODE_CODE(CALL_EXPR,    "call_expr"  )
NODE_CODE(LE_EXPR,      "LE_expr"    )
NODE_CODE(LT_EXPR,      "LT_expr"    )
NODE_CODE(GE_EXPR,      "GE_expr"    )
NODE_CODE(GT_EXPR,      "GT_expr"    )
NODE_CODE(EQ_EXPR,      "EQ_expr"    )
NODE_CODE(NEQ_EXPR,     "NEQ_expr"   )
NODE_CODE(AND_EXPR,     "AND_expr"   )
NODE_CODE(OR_EXPR,      "OR_expr"    )

NODE_CODE(SUM_EXPR,     "sum_expr"   )  /* sum block */
NODE_CODE(IF_ELSE_EXPR, "if_else_expr") /* if .. else .. */

NODE_CODE(ERROR_NODE,   "error_node" )
