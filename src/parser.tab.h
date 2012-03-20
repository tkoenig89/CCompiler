
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DO = 258,
     WHILE = 259,
     IF = 260,
     ELSE = 261,
     INT = 262,
     VOID = 263,
     RETURN = 264,
     COLON = 265,
     COMMA = 266,
     SEMICOLON = 267,
     BRACE_OPEN = 268,
     BRACE_CLOSE = 269,
     END_OF_FILE = 270,
     ID = 271,
     NUM = 272,
     SHIFT_LEFT = 273,
     SHIFT_RIGHT = 274,
     ASSIGN = 275,
     LOGICAL_OR = 276,
     LOGICAL_AND = 277,
     NE = 278,
     EQ = 279,
     GT = 280,
     GTEQ = 281,
     LSEQ = 282,
     LS = 283,
     MINUS = 284,
     PLUS = 285,
     MOD = 286,
     DIV = 287,
     MUL = 288,
     LOGICAL_NOT = 289,
     UNARY_PLUS = 290,
     UNARY_MINUS = 291,
     PARA_CLOSE = 292,
     PARA_OPEN = 293,
     BRACKET_CLOSE = 294,
     BRACKET_OPEN = 295
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 11 "parser.y"

	 int num;
	 char *id;
	 
	 /*STRUCT ZUR WEITERGABE VON INFOS NACH OBEN... %type ...*/



/* Line 1676 of yacc.c  */
#line 101 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

