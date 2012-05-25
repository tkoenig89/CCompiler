/* 
 * parser.y - Parser utility for the DHBW compiler
 */

%{
	#include "include/utlist.h" 
	#include "symtable.h"
	#include "ir_code_gen.h"
	#include <stdio.h>
	#include "main.h"
	char buffer [200];				//used for temporarily storing strings and is then passed to yyerror, to generate complex strings (with %s, %d ..)
	void yyerror (char const *);		//Generates an error message with the correct line number and increments the error count
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
     : program_element_list program_element 	/*Nothing to be done here*/
     | program_element 					/*Nothing to be done here*/
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
     : variable_declaration COMMA identifier_declaration	/*Nothing to be done here*/			//The Variables were already created in identifier_declaration
     | type identifier_declaration { if($1==0) { yyerror("You can not declare a variable as void.");}}
     ;
	
identifier_declaration
     : identifier_declaration BRACKET_OPEN NUM BRACKET_CLOSE {$1->isArray = 1;$1->var = $3;}
     | ID 	{
			//check if the name is already in use by a function
			if(existsFunc ($1))
			{
				sprintf(buffer,"The name %s was already used by a function.", $1);
				yyerror(buffer);
			}
			//check if the variable has been already declared before
			if(checkIntDec ($1))
			{
				$$ = getIntCurrScope ($1);
				sprintf(buffer,"The variable %s was already defined.", $1);
				yyerror(buffer);
			}
			else
			{
				//everything is ok, generate the variable
				$$ = putInt ($1, 0, 0);
			}
		}
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN 	{
											//check if the given function name was already used by a global variable
											if(existsIntG ($2))
											{
												sprintf(buffer,"The name %s was already used as the name of a global variable.",$2);
												yyerror(buffer);
											}
											else
											{
												//check if the given function name was already used by a function ...
												if(existsFunc ($2))
												{
													//and if so, check if it was a function declaration
													if(isFuncProto ($2))
													{
														//if so, check if the return type matches
														if($1!=getFunc($2)->retType)
														{
															sprintf(buffer,"Types mismatch from function %s's declaration.",$2);
															yyerror(buffer);
														}
														//check that the parameter count was 0. if not error.
														if(getFunc($2)->paramCount>0)
														{
															sprintf(buffer,"Parameter mismatch from function %s's declaration.",$2);
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
																//check if the given function name was already used by a global variable
																if(existsIntG ($2))
																{
																	sprintf(buffer,"The name %s was already used as the name of a global variable.",$2);
																	yyerror(buffer);
																}
																else
																{
																	//check if the given function name was already used by a function ...
																	if(existsFunc ($2))
																	{
																		//and if so, check if it was a function declaration
																		if(isFuncProto ($2))
																		{
																			//Declaration found. Checking if parameters match from the declaration
																			if(!paramFuncCheckP (getFunc($2), $4))
																			{
																				sprintf(buffer,"Function-Parameter definition does not match the declaration of function %s.",$2);
																				yyerror(buffer);
																			}
																			//check if the return type matches
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
																		//No declaration entry. Checking for temp definition from param list
																		if(strcmp ($4->name,"-1temp") == 0)
																		{
																			//Temp Func found. will be renamed to %s to make it the definition
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
														//check if the given function name was already used by a global variable
														if(existsIntG ($2))
														{
															sprintf(buffer,"The name %s was already used as the name of a global variable.",$2);
															yyerror(buffer);
														}
														else
														{
															//check whether the function was already declared/defined...
															if(existsFunc ($2)) {
																//if so the retun type must match
																if($1!=getFunc($2)->retType)
																{
																	sprintf(buffer,"The function declaration for function %s differs from a previous function declaration (Return Type does not match).",$2);
																	yyerror(buffer);
																}
																//also check that the parameter count was 0. if not error.
																if(getFunc($2)->paramCount>0)
																{
																	sprintf(buffer,"Parameter mismatch from function %s's declaration.",$2);
																	yyerror(buffer);
																}
																setFuncProtoP (getFunc($2));
																setScopeForParams (getFunc($2));
															}
															else
															{
																//Putting func into table and mark it as a prototype
																$$ = putFunc ($2, $1);
																setFuncProtoP ($$);
																setScopeForParams ($$);
															}														
														}
														setFuncScopeP (NULL);
													}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE		{	
														//check if the given function name was already used by a global variable
														if(existsIntG ($2))
														{
															sprintf(buffer,"The name %s was already used as the name of a global variable.",$2);
															yyerror(buffer);
														}
														else
														{
															//check whether the function was already declared/defined...
															if(existsFunc ($2)) {
																//if so the retun type must match
																if($1!=getFunc($2)->retType)
																{
																	sprintf(buffer,"The function declaration for function %s differs from a previous function declaration (Return Type does not match).",$2);
																	yyerror(buffer);
																}
																//also check if all parameters match
																if(!paramFuncCheckP (getFunc($2), $4))
																{
																	sprintf(buffer,"The function declaration for function %s differs from a previous function declaration (Parameters do not match).",$2);
																	yyerror(buffer);
																}
																//TODO: Check if parameters of the old declaration matches
																deleteFunc ("-1temp");
																setFuncProtoP (getFunc($2));
																setScopeForParams (getFunc($2));
															}
															else
															{
																//Putting func into table and mark it as a prototype
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
     : function_parameter								{
													//a temporary function is created to which the first parameter is appended to.
													//the function name and type will later be set to the correct type
													$$ = putFunc ("-1temp", -1);
													setParamP ($$, $1);
													$1->scope=$$;
												}
     | function_parameter_list COMMA function_parameter		{
													//append all the following parameters to the function
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
     : type identifier_declaration	 {$$ = $2; if($1==0) {yyerror("You can not declare a variable as void");}}
     ;

stmt_list
     : /* empty: epsilon */
     | stmt_list stmt					/*Nothing to be done here*/
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON	{resetTempCount();/*the temp count represents the temporary registers which will later be used in the final code. here, the count gets reset to 0*/}
     | expression SEMICOLON			{resetTempCount();/*the temp count represents the temporary registers which will later be used in the final code. here, the count gets reset to 0*/}
     | stmt_conditional				{resetTempCount();/*the temp count represents the temporary registers which will later be used in the final code. here, the count gets reset to 0*/}
     | stmt_loop						{resetTempCount();/*the temp count represents the temporary registers which will later be used in the final code. here, the count gets reset to 0*/}
     | RETURN expression SEMICOLON		{
									//we check whether the return statement is vaild, depending on the return type of the function, and then generate ir code for the statement
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
									//we check whether the return statement is vaild, depending on the return type of the function, and then generate ir code for the statement
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
     
//changed the grammar slightly to handle the goto statements easier.
stmt_conditional
     : IF PARA_OPEN expression {addif($3);addifgoto();/*generate the if statement in ir code and add a jump statement to jump out of the if block if the condition was not met*/} PARA_CLOSE stmt_conditional_r //stmt
     //| IF PARA_OPEN expression {addif($3);addifgoto();} PARA_CLOSE stmt ELSE stmt	//{addif($3);addifgoto();}
     ;
     
stmt_conditional_r
     : stmt {backpatchif(0);/*backpatch the jumpstatement which jumps out of the if-block if the condition could not be met. at this point the if-block is finished.*/}
     | stmt ELSE {backpatchif(1);addifgoto();/*backpatch the jump statement which jumps if the condition could not be met, to jump to the else block. also add a goto which will jump to the end of the if block.*/} stmt {backpatchif(0);/*backpatch the jumpstatement from before the else block which jumps out of the if-block to not also execute the else block.*/}
     ;
     
stmt_loop
     : WHILE {addwhilebegin();/*set the beginning of the while loop*/} PARA_OPEN expression PARA_CLOSE {addwhile($4);addwhilegotobegin();/*generate the if condition which determines whether the loop should continue*/} stmt {backpatchwhile();/*backpatch the position in the code to where the loop will jump, if the loop condition is not met*/}
     | DO {adddowhile();/*set the beginning of the do-while loop*/} stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON {adddowhileend($6);/*generate the if-loop-condition at the end of the loop. we dont need an explicit jump if the condition could not be met, because we can just continues in the code.*/}
     ;
     
/*
 * The non-terminal 'expression' is one of the core statements containing all arithmetic, logical, comparison and
 * assignment operators.expression
 */									
expression								// 0 = "false", nonzero = "true"
     : expression ASSIGN expression				{
											//check if the expressions were vaild for calculation an are valid for assignment
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
     | expression LSEQ expression 				{	//The lesser equal "<=" can be easily made up of lesser "<", equal "=" and the logical OR "||"; "<=" = "< || ="
											//$$ = $1 <= $2 -> $$ = $1 < $2 || $1 == $2;
											struct symInt *t0;struct symInt *t1;
											t0 = addcodeopexp2(opLS, $1, $3);
											t1 = addcodeopexp2(opEQ, $1, $3);
											$$ = addcodeopexp2(opLOGICAL_OR, t0, t1);
											//$$ = addcodeopexp2(opLSEQ, $1, $3);
										}
     | expression GTEQ expression 				{	//The greater equal ">=" can be easily made up of greater ">", equal "=" and the logical OR "||"; ">=" = "> || ="
											//$$ = $1 >= $2 -> $$ = $1 > $2 || $1 == $2;
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
     | expression SHIFT_LEFT expression			{/*dummy implementatio. only to regognise the grammar*/}
     | expression SHIFT_RIGHT expression			{/*dummy implementatio. only to regognise the grammar*/}
     | MINUS expression %prec UNARY_MINUS		{$$ = addcodeopexp1(opMINUS, $2);}
     | ID BRACKET_OPEN primary BRACKET_CLOSE	{$$ = addcodeloadarr(getInt($1), $3);$$->tempArrPos=$3->var;$$->tempArrPos2=$3;$$->isVaildForAssign=1;$$->isArray=1;} /*In c there is no check whether the array acces in the valid bounds*/
     | PARA_OPEN expression PARA_CLOSE			{$$ = $2}
     | function_call							{$$ = $1}
     | primary								{$$ = $1}
     ;

primary
     : NUM {$$ = putInt ("int", 0, $1)/*because all expressions are of type symInt, a symble number is easily represented by packing it into a symInt Variable*/}
     | ID {	if(existsInt($1)) {
			$$ = getInt($1);
		} else {
			sprintf (buffer,"The variable %s was not declared.", $1);
			yyerror(buffer);
			//We assume the variable should have been declared. so we declare it for the user...
			$$ = putInt ($1, 0, 0);;
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
													//check if the function was already defined/declared
													//just a declaration is also sufficent, because e.g. "print" will be loaded from a library later via make
													if(existsFunc($1))
													{
														sFunc = getFunc($1);
														//check that the paramcount from the function is also 0.
														if(sFunc->params!=NULL)
														{
															sprintf(buffer,"Function call parameter check failed for function %s.",$1);
															yyerror(buffer);
														}
													}
													else
													{
														//If the function does not exist, generate a dummy function so we can gather additional erros for the user in other parts of the code
														sprintf(buffer,"Function %s was not declared before the call!",$1);
														yyerror(buffer);
														sFunc = putFunc ("-1undeclared", -1);
													}
													$$ = addcodeopfunccall(opCALL, putInt ("int", 0, 0), sFunc, opcodeFindFunctionDef(sFunc));
													//also say that the function is not suitable for calculation if it is of type void. because it cant return a value you cant use it inside of a calculation
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
													//check if the function was already defined/declared
													//just a declaration is also sufficent, because e.g. "print" will be loaded from a library later via make
													if(existsFunc($1))
													{
														sFunc = getFunc($1);
														//check if the given function-call parameters match the type of the parameters in the function declaration/definition
														if(paramFuncCallCheckP (sFunc, $3))
														{
															//Functional Call Param Check OK
														}
														else
														{
															sprintf(buffer,"Functional call parameter check failed for function %s.",$1);
															yyerror(buffer);
														}
														
													}
													else
													{
														//If the function does not exist, generate a dummy function so we can gather additional erros for the user in other parts of the code
														sprintf(buffer,"Functional %s was not declared before the call.",$1);
														yyerror(buffer);
														sFunc = putFunc ("-1undeclared", -1);
													}													
													$$ = addcodeopfunccall(opCALL, putInt ("int", 0, $3->count), sFunc, opcodeFindFunctionDef(sFunc));
													//also say that the function is not suitable for calculation if it is of type void. because it cant return a value you cant use it inside of a calculation
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
     : function_call_parameters COMMA expression			{$1->count += 1;$$=$1;addcodeop1(opPARAM, $3);addParamFC($1->sInt,$3);/*increment the function call parameter count, and append the variable to the function parameter list*/}
     | expression										{$$ = createParamList($1);addcodeop1(opPARAM, $1);/*Generate the function call parameter list which will hold the number of function call parameters and a pointer to the first entry*/}
     ;

%%

void yyerror (const char *msg)
{
	//generate the error statement with the line number
	printf("ERROR at Line %d : %s\n",yylloc.first_line, msg);
	//increment the error count. if the error count is greater than 0, the source file will not be compiled
	adderror();
	//return 0;
}