/* 
 * parser.y - Parser utility for the DHBW compiler
 */

%{
	#include "include/utlist.h" 
	#include "symtable.h"
	#include "ir_code_gen.h"
	#include <stdio.h>
	
	void yyerror (char const *);
%}
 
%union{
	int num;
	char *id;
	struct symInt *sInt;
	struct symFunc *sFunc;
	struct symFuncCallParamList *sPList;
}

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
%token <id>INT <id>VOID
%token RETURN
%token COLON COMMA SEMICOLON
%token BRACE_OPEN BRACE_CLOSE

%token <id>ID
%token <num>NUM

%right ASSIGN 
%left  LOGICAL_OR
%left  LOGICAL_AND
%left  EQ NE     
%left  LS LSEQ GTEQ GT 
%left  SHIFT_LEFT SHIFT_RIGHT
%left  PLUS MINUS     
%left  MUL
%right LOGICAL_NOT UNARY_MINUS UNARY_PLUS
%left  BRACKET_OPEN BRACKET_CLOSE PARA_OPEN PARA_CLOSE

//%type <sFunc> stmt_list
//%type <sFunc> stmt
%type <sPList> function_call_parameters
%type <sFunc> function_definition
%type <sFunc> function_parameter_list
%type <sFunc> function_declaration
%type <sFunc> function_call
%type <num> type
%type <sInt> function_parameter
%type <sInt> identifier_declaration
%type <sInt> expression
%type <sInt> primary
%%

program
     : program_element_list				/*Nothing to be done here*/
     ;

program_element_list
     : program_element_list program_element 	{printf("----------DEBUG printing all functions and variables:\n\n");printf("----------DEBUG Functions:\n");debugPrintAllsFunc();printf("----------DEBUG Variables:\n");debugPrintAllsint();printf("----------DEBUG opCodes:\n");debugPrintAllopcodes();}
     | program_element 					{printf("----------DEBUG printing all functions and variables:\n\n");printf("----------DEBUG Functions:\n");debugPrintAllsFunc();printf("----------DEBUG Variables:\n");debugPrintAllsint();printf("----------DEBUG opCodes:\n");debugPrintAllopcodes();}
     ;

program_element
     : variable_declaration SEMICOLON		/*Nothing to be done here*/
     | function_declaration SEMICOLON		/*Nothing to be done here*/
     | function_definition					/*Nothing to be done here*/
     | SEMICOLON						/*Nothing to be done here*/
     ;
     
type
     : INT		{$$ = 1}
     | VOID		{$$ = 0}
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration	/*Nothing to be done here*/
     | type identifier_declaration { if($1==0) { printf("ERROR You can not declare a variable as void.\n"); } }
     ;
	
identifier_declaration
     : identifier_declaration BRACKET_OPEN NUM BRACKET_CLOSE {$1->isArray = 1;$1->var = $3;}/*{TODO ARRAY}*/
     | ID {	/*if(existsInt($1)) {
			printf("ERROR! The variable %s was already declared.\n", $1);
			$$ = getInt($1);
		} else {*/
			if(checkIntDec ($1))
			{
				//error wurde schon einmal deklariert
				$$ = getIntCurrScope ($1);
				printf("ERROR: This Variable was already defined.\n");
			}
			else
			{
				$$ = putInt ($1, 0, 0);
			}
			
		//}
	     }
     ;
     
/*	Because of the new Grammar we don't have do delete the variable because functions are imediadly recongnised. Will stay here, if needed, for a few revesions..
function_signature
     : identifier_declaration PARA_OPEN {$$ = putFunc ($1->name, 1);deleteInt ($1->name)}
     ;*/

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN 	{
											printf("Function Definition %s found. Checking if there already is a declaration entry..\n", $2);
											if(existsFunc ($2))
											{
												if(isFuncProto ($2))
												{
													if($1!=getFunc($2)->retType)
													{
														printf("ERROR: Type mismatch from Function declaration.\n");
													}
													printf("Declaration found.\n");
													setFuncIsDeclared ($2);
													setFuncScopeP (getFunc($2));
												}
												else
												{
													printf("ERROR a function definition with the same name already exists.\n");
													/*TODO: ERROR a function with the same name already exists*/
													//Error correction: Ignore stuff
												}
											}
											else
											{
												printf("No declaration entry found. Create new function %s with type %d\n", $2, $1);
												$<sFunc>$ = putFunc ($2, $1);
												setFuncScopeP (getFunc($2));
											}
											addcodeopfunc(opFUNC_DEF, NULL, getFunc($2), -1);
										}
	stmt_list BRACE_CLOSE {addcodeopfunc(opFUNC_DEF_END, NULL, getFunc($2), -1);setFuncScopeP (NULL);}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN 	{
																printf("Function Definition %s found. Checking if there already is a declaration entry..\n", $2);
																if(existsFunc ($2))
																{
																	if(isFuncProto ($2))
																	{
																		//printf("Declaration found. Checking if parameters match from the declaration.\n");
																		if(!paramFuncCheckP (getFunc($2), $4))
																		{
																			printf("ERROR: Function-Parameter definition does not match function declaration.\n");
																		}
																		if($1!=getFunc($2)->retType)
																		{
																			printf("ERROR: Type mismatch from Function declaration.\n");
																		}
																		
																		//printf("Declaration and definition match!\n");
																		
																		if(strcmp ($4->name,"-1temp") == 0)
																		{
																			deleteFunc ("-1temp");
																			setFuncIsDeclared ($2);
																			setFuncScopeP (getFunc($2));
																			addcodeopfunc(opFUNC_DEF, NULL, getFunc($2), -1);
																		}
																		else
																		{
																			printf("INTERNAL ERROR: Function has not the expected TEMP_NAME!\n");
																		}
																	}
																	else
																	{
																		printf("ERROR a function definition with the same name already exists.\n");
																		/*TODO: ERROR a function with the same name already exists*/
																		//Error correction: Ignore stuff
																	}
																}
																else
																{
																	printf("No declaration entry. Checking for temp definition from param list.\n", $2);
																	if(strcmp ($4->name,"-1temp") == 0)
																	{
																		printf("Temp Func found. will be renamed to %s to make it the definition.\n", $2);
																		renameFunc ("-1temp", $2);
																		setTypeP ($4, $1);
																		setFuncScopeP ($4);
																		addcodeopfunc(opFUNC_DEF, NULL, $4, -1);
																	}
																	else
																	{
																		printf("INTERNAL ERROR: Function has not the expected TEMP_NAME!\n");
																	}
																}
															}
	stmt_list BRACE_CLOSE {addcodeopfunc(opFUNC_DEF_END, NULL, getFunc($2), -1);setFuncScopeP (NULL);}
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE						{
														printf("Function Declaration %s found.\n", $2);
														if(existsFunc ($2)) {
															printf("ERROR a function declaration with the same name already exists.\n");
															setFuncProtoP (getFunc($2));
															setScopeForParams (getFunc($2));
														}
														else
														{
															printf("Putting func into table and mark it as a prototype.\n");
															$$ = putFunc ($2, $1);
															setFuncProtoP ($$);
															setScopeForParams ($$);
														}
														setFuncScopeP (NULL);
													}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE		{
														printf("Function Declaration %s found.\n", $2);
														if(existsFunc ($2)) {
															printf("ERROR a function declaration with the same name already exists.\n");
															deleteFunc ("-1temp");
															setFuncProtoP (getFunc($2));
															setScopeForParams (getFunc($2));
														}
														else
														{
															printf("Putting func into table and mark it as a prototype.\n");
															renameFunc ("-1temp", $2);
															setTypeP ($4, $1);
															setFuncProtoP (getFunc($2));
															setScopeForParams (getFunc($2));															
														}
														setFuncScopeP (NULL);
													}
     ;

function_parameter_list
     : function_parameter								{$$ = putFunc ("-1temp", -1);setParamP ($$, $1);$1->scope=$$;}
     | function_parameter_list COMMA function_parameter		{
													if(!existsFunc("-1temp"))
													{
														$$ = putFunc ("-1temp", -1);														
														setParamP ($$, $3);
													} else {
														incParamCountP (getFunc("-1temp"));
														$3->isParam = 1;
													}
												}
     ;
	
function_parameter
     : type identifier_declaration	 {$$ = $2; if($1==0) { printf("ERROR You can not declare a variable as void.\n"); }}
     ;

stmt_list
     : /* empty: epsilon */
     | stmt_list stmt					/*Nothing to be done here*/
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON	/*Nothing to be done here*/
     | expression SEMICOLON			/*Nothing to be done here*/
     | stmt_conditional				/*Nothing to be done here*/
     | stmt_loop						/*Nothing to be done here*/
     | RETURN expression SEMICOLON		{
									if($2->scope!=NULL)
									{
										if($2->scope->retType==0)
										{
											printf("ERROR: Function was declarad as VOID. It can not return a value. Either use \"RETURN;\" or use type int for the func.\n");
										}
									}
									addcodeop1(opRETURN, $2);
								}
     | RETURN SEMICOLON				{addcodeop1(opRETURN, NULL);}
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
//the shift/reduce error which occurs here is expected. nothing to see here move along
//changed the grammar slightly to handle the goto statements easier.
stmt_conditional
     : IF PARA_OPEN expression {addif($3);addifgoto();} PARA_CLOSE stmt_conditional_r //stmt
     //| IF PARA_OPEN expression {addif($3);addifgoto();} PARA_CLOSE stmt ELSE stmt	//{addif($3);addifgoto();}
     ;
     
stmt_conditional_r
     : stmt {backpatchif();}
     | stmt ELSE {addifgoto();backpatchif();} stmt {backpatchif();}
     ;
     
stmt_loop
     : WHILE {addwhilebegin();} PARA_OPEN expression PARA_CLOSE {addwhile($4);addwhilegotobegin();} stmt {backpatchwhile();}
     | DO {adddowhile();} stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON {adddowhileend($6);}
     ;
     
/*
 * The non-terminal 'expression' is one of the core statements containing all arithmetic, logical, comparison and
 * assignment operators.expression
 */									
expression								// 0 = "false", nonzero = "true"
     : expression ASSIGN expression				{addcodeass($1, $3);if($1->isArray) {setCodeToNOP($1->tempCodePos);}}	//WARNING: Ambigious! You dont know if you have to assign to/load from an array or if it is an normal int at this point. check this when generating final code
     | expression LOGICAL_OR expression			{$$ = addcodeopexp2(opLOGICAL_OR, $1, $3);}
     | expression LOGICAL_AND expression			{$$ = addcodeopexp2(opLOGICAL_AND, $1, $3);}
     | LOGICAL_NOT expression					{$$ = addcodeopexp1(opLOGICAL_NOT, $2);}
     | expression EQ expression					{$$ = addcodeopexp2(opEQ, $1, $3);}
     | expression NE expression					{$$ = addcodeopexp2(opNE, $1, $3);}
     | expression LS expression 					{$$ = addcodeopexp2(opLS, $1, $3);}
     | expression LSEQ expression 				{	//$$ = $1 <= $2 -> $$ = $1 < $2 || $1 == $2;
											struct symInt *t0;struct symInt *t1;
											t0 = addcodeopexp2(opLS, $1, $3);
											t1 = addcodeopexp2(opEQ, $1, $3);
											$$ = addcodeopexp2(opLOGICAL_OR, t0, t1);
											//$$ = addcodeopexp2(opLSEQ, $1, $3);
										}
     | expression GTEQ expression 				{	//$$ = $1 >= $2 -> $$ = $1 > $2 || $1 == $2;
											struct symInt *t0;struct symInt *t1;
											t0 = addcodeopexp2(opGT, $1, $3);
											t1 = addcodeopexp2(opEQ, $1, $3);
											$$ = addcodeopexp2(opLOGICAL_OR, t0, t1);
											//$$ = addcodeopexp2(opGTEQ, $1, $3);
										}
     | expression GT expression					{$$ = addcodeopexp2(opGT, $1, $3);}
     | expression PLUS expression				{$$ = addcodeopexp2(opADD, $1, $3);}
     | expression MINUS expression				{$$ = addcodeopexp2(opSUB, $1, $3);}
     | expression MUL expression				{$$ = addcodeopexp2(opMUL, $1, $3);}
     | MINUS expression %prec UNARY_MINUS		{$$ = addcodeopexp1(opMINUS, $2);}
     | ID BRACKET_OPEN primary BRACKET_CLOSE	{$$ = addcodeloadarr(getInt($1), $3);$$->tempArrPos=$3->var;} /*In c there is no check whether the array acces in the valid bounds*/
     | PARA_OPEN expression PARA_CLOSE			{$$ = $2}
     | function_call							{$$ = putInt (".v0", 0, 0);/*TODO: Check whether v0 or v1 is needed as a temp register. for e.g. i = f() + g() -> i = v0 + v1*/}
     | primary								{$$ = $1}
     ;

primary
     : NUM {$$ = putInt ("int", 0, $1)}
     | ID {	if(existsInt($1)) {
			$$ = getInt($1);
		} else {
			printf("ERROR! The variable %s was not declared. Line: %d Column: %d \n", $1, @1.first_line, @1.first_column);
			//We assume the variable should have been declared. so we declare it for the user...
			$$ = putInt ($1, 0, 0);
			//yyerror("syntax error");
		}
	  }
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE						{
													printf("Function call regocnised.\n");
													if(existsFunc($1))
													{
														$$ = getFunc($1);
													}
													else
													{
														printf("ERROR! Function was not declared before the call!\n");
														$$ = putFunc ("-1undeclared", -1);
													}
													addcodeopfunc(opCALL, putInt ("int", 0, 0), $$, opcodeFindFunctionDef($$));
												}
      | ID PARA_OPEN function_call_parameters PARA_CLOSE	{
													printf("Parameterised Function call regocnised.\n");
													if(existsFunc($1))
													{
														$$ = getFunc($1);
														if(paramFuncCallCheckP ($$, $3))
														{
															printf("Functional Call Param Check OK!\n");
														}
														else
														{
															printf("ERROR: Functional Call Param Check FAILED!\n");
														}
														
													}
													else
													{
														printf("ERROR! Function was not declared before the call!\n");
														$$ = putFunc ("-1undeclared", -1);
													}
													
													addcodeopfunc(opCALL, putInt ("int", 0, $3->count), $$, opcodeFindFunctionDef($$));
												}
      ;

function_call_parameters
     : function_call_parameters COMMA expression			{$$->count += 1;addcodeop1(opPARAM, $3);}
     | expression										{$$ = createParamList($1);addcodeop1(opPARAM, $1);}
     ;

%%

void yyerror (const char *msg)
{
	printf("ERROR: %s\n", msg);
	//return 0;
}