
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
     ID = 270,
     NUM = 271,
     ASSIGN = 272,
     LOGICAL_OR = 273,
     LOGICAL_AND = 274,
     NE = 275,
     EQ = 276,
     GT = 277,
     GTEQ = 278,
     LSEQ = 279,
     LS = 280,
     SHIFT_RIGHT = 281,
     SHIFT_LEFT = 282,
     MINUS = 283,
     PLUS = 284,
     MUL = 285,
     UNARY_PLUS = 286,
     UNARY_MINUS = 287,
     LOGICAL_NOT = 288,
     PARA_CLOSE = 289,
     PARA_OPEN = 290,
     BRACKET_CLOSE = 291,
     BRACKET_OPEN = 292
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 14 "parser.y"

	int num;
	char *id;
	//struct symrec *tptr;
	struct symInt *sInt;
	struct symFunc *sFunc;
	/*STRUCT ZUR WEITERGABE VON INFOS NACH OBEN... %type ...*/



/* Line 1676 of yacc.c  */
#line 100 "parser.tab.h"
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

