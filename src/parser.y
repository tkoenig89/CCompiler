/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{
%}
 
%debug
%locations
%start program

/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */
%expect 1

%token DO WHILE
%token IF ELSE
%token INT VOID
%token RETURN
%token COLON COMMA SEMICOLON
%token BRACE_OPEN BRACE_CLOSE
%token END_OF_FILE

%token ID
%token NUM

/* TODO: add associativity and precedence so that the 256 shift-reduce vanish */
%token ASSIGN
%token LOGICAL_OR LOGICAL_NOT LOGICAL_AND 
%token EQ NE LS LSEQ GTEQ GT
%token PLUS MINUS MUL DIV MOD
%token NOT UNARY_MINUS UNARY_PLUS
%token BRACKET_OPEN BRACKET_CLOSE PARA_OPEN PARA_CLOSE

%%

/* 
 * The start symbol of the programming language. According to the
 * original GNU C compiler (GCC), there has to be at least one
 * program_element. The End-of-File token is necessary for completing
 * the parsing process successfully by reducing to the start symbol
 * of grammar 'program'. 
 */
program
     : program_element_list
     ;

/*
 * The list of program elements which are considered as high-level elements like function
 * definitions, global (constant) variables. The non-terminal 'program_element_list' consists at 
 * least of one program element. Though, empty source files will not succeed.
 */									
program_element_list
     : program_element_list program_element 
     | program_element
     ;

/*
 * The different program elements which are variable declarations, function definitions and
 * function prototypes and type definitions for the basic version of the compiler. 
 */									
program_element
     : declaration SEMICOLON
     | function_definition
     | SEMICOLON
     ;
									
/* 
 * The different types of the subset including self-defined data types by using the typedef
 * instruction.
*/
type
     : INT
     | VOID
     ;

/* 
 * The non-terminal 'declaration' is used for declarations like
 * 'int a, b, *c[]' as well as function prototypes. 									
/* Each 'declaration' consists of at least one 'declaration_element'. The
 * left-recursion of this rule is positive for the synthesizing the type as 
 * stack-attribute to the 'identifier_declaration'.
*/						
declaration
     : declaration COMMA declaration_element
     | type declaration_element
     ;

/*
 * The non-terminal 'declaration_element' contains the different possible elements for 
 * an elementary declaration which could be either a 'function_header' acting as the definition of a function
 * prototype or the declaration of an identifier.
 */
declaration_element
     : identifier_declaration
     | function_header
     ;

/*
 * The non-terminal 'identifier_declaration' contains the specifics of the variable beside
 * the type definition like arrays, pointers and initial (default) values.
 */									
identifier_declaration
     : identifier_declaration BRACKET_OPEN expression BRACKET_CLOSE
     | ID
     ;

/*
 * The non-terminal 'function_definition' is the beginning of the function definition.
 */									
function_definition
     : type function_header BRACE_OPEN stmt_list BRACE_CLOSE
     ;

/*
 * The non-terminal 'function_header' is used within the non-terminals 'function' and
 * 'function_prototype'. The grammar for the function definition and the function prototype
 * is split up this way to facilitate the parsing process and to use synthesized attributes
 * during the parsing process.
 */									
function_header
     : function_prefix PARA_CLOSE
     ;
	
/*
 * The non-terminal 'function_prefix' is used within the non-terminal 'function_header'. The
 * 'function_prefix' distinguishes between functions with parameters by the non-terminal 
 * 'function_signature_parameters' and functions without by the non-terminal 'function_signature'.
 */									
function_prefix
     : function_signature
     | function_signature_parameters
     ;

/*
 * The non-terminal 'function_signature' initializes the function signature definition
 */ 									
function_signature
     : identifier_declaration PARA_OPEN
     ;

/*
 * The non-terminal 'function_signature_parameters' declares the function of the function prototype with
 * (input) parameters.
 */									
function_signature_parameters
     : function_signature_parameters COMMA function_parameter_element
     | function_signature function_parameter_element
     ;
	
/*
 * The non-terminal 'function_parameter_element' is used within the non-terminal 'function_definition_parameters'
 * and contains the declaration for ONE parameter.
 */									
function_parameter_element
     : type identifier_declaration
     ;
									
/*
 * The non-terminal 'stmt_list' is list of statements containing any number (including zero) of statements represented 
 * by the non-terminal 'stmt'.
 */									
stmt_list
     : /* empty: epsilon */
     | stmt_list stmt
     ;

/*
 * The non-terminal 'stmt' is used for the statements of the programming language whereas the non-terminal
 * 'expression' is one of the core statements.
 */									
stmt
     : stmt_block
     | declaration SEMICOLON
     | expression SEMICOLON
     | stmt_conditional
     | stmt_loop
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON /* empty statement */
     ;

/*
 * A statement block is just a statement list within braces.
 */									
stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
/*
 * The non-terminal 'stmt_conditional' contains the conditional statements of the language. The second rule
 * produces a SHIFT/REDUCE error which is solved by the default behavior of bison (see above).
 */									
stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt
     ;
									
/*
 * The non-terminal 'stmt_loop' contains the loop statements of the language.
 */									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON
     ;
									
/*
 * The non-terminal 'expression' is one of the core statements containing all arithmetic, logical, comparison and
 * assignment operators.expression
 */									
expression
     : expression ASSIGN expression
     | expression LOGICAL_OR expression
     | expression LOGICAL_AND expression
     | LOGICAL_NOT expression
     | expression EQ expression
     | expression NE expression
     | expression LS expression 
     | expression LSEQ expression 
     | expression GTEQ expression 
     | expression GT expression
     | expression PLUS expression
     | expression MINUS expression
     | expression MUL expression
     | expression DIV expression 
     | expression MOD expression 
     | MINUS expression %prec UNARY_MINUS
     | ID BRACKET_OPEN primary BRACKET_CLOSE
     | PARA_OPEN expression PARA_CLOSE
     | function_call PARA_CLOSE
     | primary
     ;

primary
     : NUM
     | ID
     ;

/*
 * The non-terminal 'function_call' is used by the non-terminal 'expression' for calling functions.
 */									
function_call
      : ID PARA_OPEN
      | function_call_parameters
      ;

/*
 * The non-terminal 'function_call_parameters' is used for the parameters of a function call 
 * by the non-terminal 'function_call'.
 */ 									
function_call_parameters
     : function_call_parameters COMMA expression
     | ID PARA_OPEN expression
     ;

%%

void yyerror (const char *msg)
{
}
