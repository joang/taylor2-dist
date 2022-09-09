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

#ifndef MY_JET_NONE_H
#define MY_JET_NONE_H

/* *******  ******************  ******* */
/* *******  MY_JET_NONE_HEADER  ******* */
/* *******  ******************  ******* */

#define MY_JET_NONE_PREHEADER(MY_JET_FIXES) "\
" \

#define MY_JET_NONE_HEADER(MY_JET_FIXES) "\
typedef void* MY_JET;\n\
" \

#define MY_JET_NONE_POSTHEADER(MY_JET_FIXES) "\
" \

/* *******  ****************  ******* */
/* *******  MY_JET_NONE_CODE  ******* */
/* *******  ****************  ******* */
#define MY_JET_NONE_PRECODE(MY_JET_FIXES) "\
" \

#define MY_JET_NONE_CODE(MY_JET_FIXES) "\
\n\
" \

#define MY_JET_NONE_POSTCODE(MY_JET_FIXES) "\
" \

#endif /* MY_JET_NONE_H */
