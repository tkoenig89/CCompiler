/* 
 * parser.y - Parser utility for the DHBW compiler
 */

%{
	#include "include/utlist.h" 
	#include "symtable.h"
	#include "ir_code_gen.h"
	#include <stdio.h>
	#include "main.h"
	char buffer [200];
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
%type <sInt> function_call
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
     : program_element_list program_element 	//{printf("----------DEBUG printing all functions and variables:\n\n");printf("----------DEBUG Functions:\n");debugPrintAllsFunc();printf("----------DEBUG Variables:\n");debugPrintAllsint();printf("----------DEBUG opCodes:\n");debugPrintAllopcodes();}
     | program_element 					//{printf("----------DEBUG printing all functions and variables:\n\n");printf("----------DEBUG Functions:\n");debugPrintAllsFunc();printf("----------DEBUG Variables:\n");debugPrintAllsint();printf("----------DEBUG opCodes:\n");debugPrintAllopcodes();}
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
     | type identifier_declaration { if($1==0) { yyerror("You can not declare a variable as void.");}}
     ;
	
identifier_declaration
     : identifier_declaration BRACKET_OPEN NUM BRACKET_CLOSE {$1->isArray = 1;$1->var = $3;}/*{TODO ARRAY}*/
     | ID 	{
			if(existsFunc ($1))
			{
				sprintf(buffer,"The name %s was already used by a function.", $1);
				yyerror(buffer);
			}
			//TODO: Check if the name was already declaread as a function
			if(checkIntDec ($1))
			{
				//error wurde schon einmal deklariert
				$$ = getIntCurrScope ($1);
				sprintf(buffer,"The variable %s was already defined.", $1);
				yyerror(buffer);
			}
			else
			{
				$$ = putInt ($1, 0, 0);
			}
		}
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN 	{
											if(existsIntG ($2))
											{
												sprintf(buffer,"The name %s was already used as the name of a variable.",$2);
												yyerror(buffer);
											}
											else
											{
												if(existsFunc ($2))
												{
													if(isFuncProto ($2))
													{
														if($1!=getFunc($2)->retType)
														{
															sprintf(buffer,"Types mismatch from function %s's declaration.",$2);
															yyerror(buffer);
														}
														//Declaration found
														setFuncIsDeclared ($2);
														setFuncScopeP (getFunc($2));
													}
													else
													{
														sprintf(buffer,"A function definition with the same name (%s) already exists",$2);
														yyerror(buffer);
													}
												}
												else
												{
													//No declaration entry found. Create new function
													$<sFunc>$ = putFunc ($2, $1);
													setFuncScopeP (getFunc($2));
												}
												addcodeopfunc(opFUNC_DEF, NULL, getFunc($2), -1);
											}
										}
	stmt_list BRACE_CLOSE {addcodeopfunc(opFUNC_DEF_END, NULL, getFunc($2), -1);setFuncScopeP (NULL);backpatchreturn();}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN 	{
																if(existsIntG ($2))
																{
																	sprintf(buffer,"The name %s was already used as the name of a variable.",$2);
																	yyerror(buffer);
																}
																else
																{
																	if(existsFunc ($2))
																	{
																		if(isFuncProto ($2))
																		{
																			//printf("Declaration found. Checking if parameters match from the declaration.\n");
																			if(!paramFuncCheckP (getFunc($2), $4))
																			{
																				sprintf(buffer,"Function-Parameter definition does not match the declaration of function %s.",$2);
																				yyerror(buffer);
																			}
																			if($1!=getFunc($2)->retType)
																			{
																				sprintf(buffer,"Type mismatch from function %s's declaration.",$2);
																				yyerror(buffer);
																			}
																			
																			//Declaration and definition match																			
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
																			sprintf(buffer,"A function definition with the same name (%s) already exists.",$2);
																			yyerror(buffer);
																			/*TODO: ERROR a function with the same name already exists*/
																			//Error correction: Ignore stuff
																		}
																	}
																	else
																	{
																		//printf("No declaration entry. Checking for temp definition from param list.\n", $2);
																		if(strcmp ($4->name,"-1temp") == 0)
																		{
																			//printf("Temp Func found. will be renamed to %s to make it the definition.\n", $2);
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
															}
	stmt_list BRACE_CLOSE {addcodeopfunc(opFUNC_DEF_END, NULL, getFunc($2), -1);setFuncScopeP (NULL);backpatchreturn();}
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE						{
														if(existsIntG ($2))
														{
															sprintf(buffer,"The name %s was already used as the name of a variable.",$2);
															yyerror(buffer);
														}
														else
														{
																
															if(existsFunc ($2)) {
																sprintf(buffer,"A function declaration with the same name (%s) already exists.",$2);
																yyerror(buffer);
																setFuncProtoP (getFunc($2));
																setScopeForParams (getFunc($2));
															}
															else
															{
																//printf("Putting func into table and mark it as a prototype.\n");
																$$ = putFunc ($2, $1);
																setFuncProtoP ($$);
																setScopeForParams ($$);
															}														
														}
														setFuncScopeP (NULL);
													}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE		{
														if(existsIntG ($2))
														{
															sprintf(buffer,"The name %s was already used as the name of a variable.",$2);
															yyerror(buffer);
														}
														else
														{
															if(existsFunc ($2)) {
																sprintf(buffer,"A function declaration with the same name (%s) already exists.",$2);
																yyerror(buffer);
																deleteFunc ("-1temp");
																setFuncProtoP (getFunc($2));
																setScopeForParams (getFunc($2));
															}
															else
															{
																//printf("Putting func into table and mark it as a prototype.\n");
																renameFunc ("-1temp", $2);
																setTypeP ($4, $1);
																setFuncProtoP (getFunc($2));
																setScopeForParams (getFunc($2));															
															}
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
     : type identifier_declaration	 {$$ = $2; if($1==0) { 	yyerror("You can not declare a variable as void");}}
     ;

stmt_list
     : /* empty: epsilon */
     | stmt_list stmt					/*Nothing to be done here*/
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON	{resetTempCount();}
     | expression SEMICOLON			{resetTempCount();}
     | stmt_conditional				{resetTempCount();}
     | stmt_loop						{resetTempCount();}
     | RETURN expression SEMICOLON		{
									if($2->scope!=NULL)
									{
										if($2->scope->retType==0)
										{
											yyerror("Function was declared as VOID. It can not return a value. Either use \"RETURN;\" or use type int for the func.");
										}
									}
									addcodeop1(opRETURN, $2);
									{resetTempCount();}
								}
     | RETURN SEMICOLON				{
									struct symFunc *scope = getCurrentScope();
									if(scope!=NULL)
									{
										if(scope->retType==1)
										{
											yyerror("Function was declared as INT. It has to return a value. Either use \"RETURN 0;\" or use type void for the func.");
										}
									}
									addcodeop1(opRETURN, NULL);
									{resetTempCount();}
								}
     | SEMICOLON {resetTempCount();}
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
     : stmt {backpatchif(0);}
     | stmt ELSE {backpatchif(1);addifgoto();} stmt {backpatchif(0);}
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
     : expression ASSIGN expression				{
											if($1->isVaildForAssign==-1)
											{
												yyerror("The expression left from '=' is not vaild for an assignment.");
											}
											if($3->isVaildForCalc==-1)
											{
												yyerror("An expression on the right side of the '=' is not valid for calculation.");
											}
											$$ = $3;addcodeass($1, $3);if($1->tempCodePos>-1) {setCodeToNOP($1->tempCodePos);}
										}	//WARNING: Ambigious! You dont know if you have to assign to/load from an array or if it is an normal int at this point. check this when generating final code
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
     | ID BRACKET_OPEN primary BRACKET_CLOSE	{$$ = addcodeloadarr(getInt($1), $3);$$->tempArrPos=$3->var;$$->tempArrPos2=$3;$$->isVaildForAssign=1;} /*In c there is no check whether the array acces in the valid bounds*/
     | PARA_OPEN expression PARA_CLOSE			{$$ = $2}
     | function_call							{
											/*if($1->isVaildForCalc==-1)
											{
												yyerror("A function used in the expression chain is of type VOID and therefore can't return a value.");
											}*/
											$$ = $1;
										}
     | primary								{$$ = $1}
     ;

primary
     : NUM {$$ = putInt ("int", 0, $1)}
     | ID {	if(existsInt($1)) {
			$$ = getInt($1);
		} else {
			//TODO: It seems that global variables are not recognised, check this!
			sprintf (buffer,"The variable %s was not declared.", $1);
			yyerror(buffer);
			//We assume the variable should have been declared. so we declare it for the user...
			$$ = putInt ($1, 0, 0);
			//yyerror("syntax error");
		}
		if($$->isArray!=1)
		{
			$$->isVaildForAssign=1;
		}
	  }
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE						{
													struct symFunc *sFunc;
													//printf("Function call regocnised.\n");
													if(existsFunc($1))
													{
														sFunc = getFunc($1);
														if(sFunc->params!=NULL)
														{
															sprintf(buffer,"Functional call parameter check failed for function %s.",$1);
															yyerror(buffer);
														}
													}
													else
													{
														sprintf(buffer,"Function %s was not declared before the call!",$1);
														yyerror(buffer);
														sFunc = putFunc ("-1undeclared", -1);
													}
													$$ = addcodeopfunccall(opCALL, putInt ("int", 0, 0), sFunc, opcodeFindFunctionDef(sFunc));
													if(existsFunc($1))
													{
														if(sFunc->retType==0)
														{
															$$->isVaildForCalc=-1;
														}
													} else {
														$$->isVaildForCalc=-1;													
													}
												}
      | ID PARA_OPEN function_call_parameters PARA_CLOSE	{
													struct symFunc *sFunc;
													//Parameterised Function call regocnised
													if(existsFunc($1))
													{
														sFunc = getFunc($1);
														if(paramFuncCallCheckP (sFunc, $3))
														{
															//printf("Functional Call Param Check OK!\n");
														}
														else
														{
															sprintf(buffer,"Functional call parameter check failed for function %s.",$1);
															yyerror(buffer);
														}
														
													}
													else
													{
														sprintf(buffer,"Functional %s was not declared before the call.",$1);
														yyerror(buffer);
														sFunc = putFunc ("-1undeclared", -1);
													}													
													$$ = addcodeopfunccall(opCALL, putInt ("int", 0, $3->count), sFunc, opcodeFindFunctionDef(sFunc));
													if(existsFunc($1))
													{
														if(sFunc->retType==0)
														{
															$$->isVaildForCalc=-1;
														}
													} else {
														$$->isVaildForCalc=-1;													
													}
												}
      ;

function_call_parameters
     : function_call_parameters COMMA expression			{$$->count += 1;addcodeop1(opPARAM, $3);}
     | expression										{$$ = createParamList($1);addcodeop1(opPARAM, $1);}
     ;

%%

void yyerror (const char *msg)
{
	printf("ERROR at Line %d : %s\n",yylloc.first_line, msg);
	adderror();
	//return 0;
}