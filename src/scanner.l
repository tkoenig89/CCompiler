/* 
 * scanner.l - Scanner utility for the BA compiler
 */

%{
	#include "parser.tab.h"
%}

/* 
 * Default setting for flex which indicates that yytext, which is one of the main connections
 * between the scanner and parser, is handled as pointer of char instead of an array of char. 
*/
%pointer	

/* save the line number of terminals */
%option yylineno

/* finish after scanning the provided file */
%option noyywrap		

identifier		[_a-zA-Z][_a-zA-Z0-9]*
number			[1-9][0-9]*|0	
comment			\/\/.*\n	 
%%

\n	/* new line */
[\t ]+	/* ignore white spaces */

do { return DO; }
else { return ELSE; }
if { return IF; }
int { return INT; }
return { return RETURN; }
void { return VOID; }
while { return WHILE; }

; { return SEMICOLON; }
: { return COLON; }
, { return COMMA; }

&& { return LOGICAL_AND; }
\|\| { return LOGICAL_OR; }
! { return LOGICAL_NOT; }
= { return ASSIGN; }
!= { return NE; }
==  { return EQ; }
\> { return GT; }
\>= { return GTEQ; }
\< { return LS; }
\<= { return LSEQ; }

\+ { return PLUS; }
\- { return MINUS; }
\* { return MUL; }
\/ { return DIV; }
% { return MOD; }

\{ { return BRACE_OPEN; }
\} { return BRACE_CLOSE; }
\( { return PARA_OPEN; }
\) { return PARA_CLOSE; }
\[ { return BRACKET_OPEN; }
\] { return BRACKET_CLOSE; }

{identifier} { return ID; }
{number} { return NUM; }
{comment} {return COM;}
							
<<EOF>>		{ return 0; }

.
