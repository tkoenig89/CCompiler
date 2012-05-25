#include "ir_code_gen.h"
#include "symtable.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

//A dynamic array containing the ir code
struct strCode  *code;
//The length of the dynamic array
int code_count = 0;
//Used to determine which temporary register should be used
int temp_reg_count = -1;

/**
 * Packs the temp_reg_count variable into a symInt struct
 * @return the packed temp_reg_count
 */
struct symInt *irtempInt() 
{
	temp_reg_count += 1;
	//if temp_reg_count > 21 then ERROR, no space left for any more temp registers
	//we have to work around and put them into memory	
	char buffer [5];
	sprintf (buffer, ".t%d", temp_reg_count);
	
	struct symInt *ptr;
	ptr = tempInt (buffer);
	//ptr->isArray = temp_reg_count;
	
	return ptr;
}

/**
 * Increments the dynamic array containing the ir code, and adds all the given values into the new created array slot
 * @param operation the opCode to be used
 * @param int0 the first variable
 * @param int1 the second variable
 * @param int2 the third variable
 * @param func a function
 * @param jmpTo used to store jump values for e.g. function calls or if/then/else or while-loops
 */
void addcode(enum code_ops operation, struct symInt *int0, struct symInt *int1, struct symInt *int2, struct symFunc *func, int jmpTo)
{
	code_count += 1;
	struct strCode *codebuffer = (struct strCode*) realloc (code, code_count * sizeof(struct strCode));
	
	code = codebuffer;
	
	code[code_count-1].op = operation;
	
	code[code_count-1].int0 = int0;
	code[code_count-1].int1 = int1;
	code[code_count-1].int2 = int2;
	code[code_count-1].func = func;
	code[code_count-1].jmpTo = jmpTo;
	code[code_count-1].jmpLabel = -1;
}

/**
 * Creates an ir code entry for a variable assignment
 * @param int0 the var which should get the value
 * @param int1 the var containing the value to be assigned
 */
void addcodeass(struct symInt *int0, struct symInt *int1)
{
	//Check whether the assignment variable is an array, and if so use MEM_STORE as the opCode
	if(int0->tempArrPos>-1)
	{
		addcode(opMEM_ST, int0->nextElement, int0->tempArrPos2 , int1 /*=int2*/, NULL, -1);
	}
	else
	{	
		addcode(opASSIGN, int0, int1, NULL, NULL, -1);
	}
	temp_reg_count = 0;	
}

/**
 * Simplified version of addcode, for opCodes which require only 1 variable paramter. ONLY USED FOR NON-EXPRESSION single opCodes
 * @param operation the opCode
 * @param int0 the variable parameter
 */
void addcodeop1(enum code_ops operation, struct symInt *int0)
{	
	if(operation==opRETURN)
	{
		addcode(operation, int0, NULL, NULL, NULL, -137);
	}
	else
	{
		addcode(operation, int0, NULL, NULL, NULL, -1);
	}
}

/**
 * Simplified version of addcode, for opCodes which require only 1 variable paramter.
 * @param operation the opCode
 * @param int1 the variable parameter
 * @return the temp register variable
 */
struct symInt *addcodeopexp1(enum code_ops operation, struct symInt *int1)
{
	struct symInt *ptr;
	if(int1->next!=137)
	{
		ptr = irtempInt();
	}
	else
	{
		ptr=int1;		
	}
	

	addcode(operation, ptr, int1, NULL, NULL, -1);
	
	ptr->isVaildForCalc=int1->isVaildForCalc;	
	
	return ptr;
}

/**
 * Simplified version of addcode, for opCodes which require only 2 variable paramter.
 * @param operation the opCode
 * @param int0 the first variable parameter
 * @param int1 the second variable paramter
 */
void addcodeop2(enum code_ops operation, struct symInt *int0, struct symInt *int1)
{
	addcode(operation, int0, int1, NULL, NULL, -1);
}

/**
 * Creates an IF statement in the ir code. the given variable is the end of the boolean expression
 * @param int0 the variable parameter
 */
void addif(struct symInt *int0)
{
	addcode(opIF, int0, NULL, NULL, NULL, getopcodeCount()+2);
}

/**
 * Creates a goto ir code for an if expression, which will later be backpatched to the right adress
 */
void addifgoto()
{
	addcode(opGOTO, NULL, NULL, NULL, NULL, -137);
}

/**
 * Use this at the end of the if expression to patch the created goto statement.
 * @param shift use this to slightly shift the adress
 */
void backpatchif(int shift)
{	
	struct strCode  *c;	

	for(int i=code_count-1;i>=0;i--)
	{
		c = &code[i];
		
		if(c->op==opGOTO)
		{
			if(c->jmpTo==-137)
			{
				c->jmpTo = getopcodeCount() + shift;
				break;
			}
		}	
	}
}

/**
 * Backpatches the return statment to let it later jump to the end of the function after it was called
 */
void backpatchreturn()
{	
	struct strCode  *c;	
	
	for(int i=0;i<code_count;i++)
	{
		c = &code[i];
		
		if(c->op==opRETURN)
		{
			if(c->jmpTo==-137)
			{
				c->jmpTo = getopcodeCount() - 1;
			}
		}	
	}
}

/**
 * Creates an IF ir code which represents the loop condition
 * @param int0
 */
void addwhile(struct symInt *int0)
{
	addcode(opIF, int0, NULL, NULL, NULL, getopcodeCount()+2);
}

/**
 * Creates an ir code to better handle goto jump statements. But will be ignored in the final code generation
 */
void addwhilebegin()
{
	addcode(opWHILE_BEGIN, NULL, NULL, NULL, NULL, -137);
}

/**
 * Creates a GOTO ir code to create loop
 */
void addwhilegotobegin()
{
	addcode(opGOTO, NULL, NULL, NULL, NULL, -137);
}

/**
 * Backpatches all jump statements of a while loop to the current position in the ir code array
 */
void backpatchwhile()
{	
	struct strCode  *c;	

	for(int i=code_count-1;i>=0;i--)
	{
		c = &code[i];
				
		if(c->op==opGOTO)
		{
			if(c->jmpTo==-137)
			{
				c->jmpTo = getopcodeCount()+1;
				break;
			}
		}	
	}
	for(int i=code_count-1;i>=0;i--)
	{
		c = &code[i];
		
		if(c->op==opWHILE_BEGIN)
		{
			if(c->jmpTo==-137)
			{
				addcode(opGOTO, NULL, NULL, NULL, NULL, i);
				c->jmpTo=-1;
				break;
			}
		}
	}
}

/**
 * Creates an ir code to better handle goto jump statements. But will be ignored in the final code generation
 */
void adddowhile()
{
	addcode(opDO_WHILE_BEGIN, NULL, NULL, NULL, NULL, -137);
}

/**
 * Creates the if expression at the end of a do-while loop, which represents the loop condition
 * @param int0
 */
void adddowhileend(struct symInt *int0)
{
	struct strCode  *c;	
	int i;
	for(i=code_count-1;i>=0;i--)
	{
		c = &code[i];
		
		if(c->op==opDO_WHILE_BEGIN)
		{
			if(c->jmpTo==-137)
			{
				c->jmpTo=-1;
				break;
			}
		}
	}
	addcode(opIF, int0, NULL, NULL, NULL, i);
}

/**
 * Simplified version of addcode, for opCodes which require 2 variable paramters.
 * @param operation the opCode
 * @param int1 the first variable parameter
 * @param int2 the second variable parameter
 * @return
 */
struct symInt *addcodeopexp2(enum code_ops operation, struct symInt *int1, struct symInt *int2)
{
	struct symInt *ptr;

	if((int1->next==137) && (int2->next==137))
	{
		ptr = int1;
		temp_reg_count -= 1;
	}
	else
	{
		ptr = irtempInt();	
	}
	
	addcode(operation, ptr, int1, int2, NULL, -1);
	
	if((int1->isVaildForCalc==-1) || (int2->isVaildForCalc==-1))
	{
		ptr->isVaildForCalc=-1;
	}
	
	return ptr;
}

/**
 * Creates a MEM_LOAD statement which will later dereference an array in the final code
 * @param int1 the array
 * @param int2 a symInt containing the position of the array which should be loaded in the var attribute
 * @return the temporary register variable
 */
struct symInt * addcodeloadarr(struct symInt *int1, struct symInt *int2)
{
	struct symInt *ptr;
	ptr = irtempInt();
	
	int1->tempArrPos = int2->var;
	ptr->nextElement = int1;
	
	addcode(opMEM_LD, ptr, int1, int2, NULL, -1);
	
	ptr->tempCodePos = code_count -1 ;
	
	return ptr;
}

/**
 * Will generate the end or the beginning of a function definition
 * @param operation opFUNC_DEF or opFUNC_DEF_END
 * @param int0 depricated - it is not in use
 * @param func the function for which the header or footer should be created
 * @param jmpTo
 */
void addcodeopfunc(enum code_ops operation, struct symInt *int0, struct symFunc *func, int jmpTo)
{
	addcode(operation, int0, NULL, NULL, func, jmpTo);
}

/**
 * Creates the ir code for a function call
 * @param operation the opCALL opCode
 * @param int0 the number of parameters packed into a symInt
 * @param func the function which should be called
 * @param jmpTo the position of the function definition inside the ir code
 * @return
 */
struct symInt *addcodeopfunccall(enum code_ops operation, struct symInt *int0, struct symFunc *func, int jmpTo)
{
	struct symInt *ptr;
	ptr = irtempInt();
	
	addcode(operation, int0, ptr, NULL, func, jmpTo);
	
	return ptr;
}

/**
 * Deprecated Function. DO NOT USE
 * TODO: deleteme
 */
void printcode()
{
	int i = 0;
	while (i < code_count) {
		//printf("%3ld: %-10s%4ld\n",i,op_name[(int) code[i].op], code[i].arg );
		//printf("%s "
		i++;
	}
}

/**
 * Prints the ir code array into human readable debug code
 */
void debugPrintAllopcodes()
{
	struct strCode  *c;	
	struct symInt *int_;
	struct symFunc *func_;
	int count=0;
	char tab = '\0';
	
	for(int i=0;i<code_count;i++)
	{
		c = &code[i];
		
		if(c->op==opFUNC_DEF_END) tab = '\0';
		
		//printf("%cOP #%d: %s", tab, count, enumStrings[c->op]);
		if(c->int0!=NULL) {int_=c->int0;/*printf(", INT0: %s", int_->name);*/}
		if(c->int1!=NULL) {int_=c->int1;/*printf(", INT1 %s", int_->name);*/}
		if(c->int2!=NULL) {int_=c->int2;/*printf(", INT2: %s", int_->name);*/}
		
		if(c->func!=NULL) {func_=c->func;/*printf(", FUNC: %s", func_->name);*/}

		if(c->jmpTo!=-1) {/*printf(", JMP_TO: %d", c->jmpTo);*/}
		
		//printf("\n");
		count++;
		
		if(c->op==opFUNC_DEF) tab = '\t';		
	}
}

/**
 * Prints the ir code array into a file (very simple)
 * @param f
 */
void generateIRCodeFile(FILE *f)
{
	struct strCode  *c;
	struct symInt *int_;
	struct symFunc *func_;
	int count=0;
	char tab = '\0';
	char buffer [200];

	for(int i=0;i<code_count;i++)
	{
		c = &code[i];

		if(c->op==opFUNC_DEF_END) tab = '\0';

		sprintf (buffer, "%cOP #%d: %s", tab, count, enumStrings[c->op]);
		fputs (buffer,f);
		if(c->int0!=NULL) {int_=c->int0; 	sprintf (buffer, ", INT0: %s", int_->name);		fputs (buffer,f);}
		if(c->int1!=NULL) {int_=c->int1; 	sprintf (buffer, ", INT1 %s", int_->name);		fputs (buffer,f);}
		if(c->int2!=NULL) {int_=c->int2; 	sprintf (buffer, ", INT2: %s", int_->name);		fputs (buffer,f);}

		if(c->func!=NULL) {func_=c->func; 	sprintf (buffer, ", FUNC: %s", func_->name);	fputs (buffer,f);}

		if(c->jmpTo!=-1) {					sprintf (buffer, ", JMP_TO: %d", c->jmpTo);		fputs (buffer,f);}

		sprintf (buffer, "\n");
		fputs (buffer,f);
		count++;

		if(c->op==opFUNC_DEF) tab = '\t';
	}
}

/**
 * Returns the dynamic ir code array
 * @return the ir code array
 */
struct strCode  *getopcodeArray()
{
	return code;
}

/**
 * Returns the length of the ir code array
 * @return length of the ir code array
 */
int getopcodeCount()
{
	return code_count;
}

/**
 * Returns the position of the function definition inside the ir code array
 * @param func pointer to the function
 * @return the code position of the function definition
 */
int opcodeFindFunctionDef(struct symFunc *func)
{
	struct strCode  *c;	
	for(int i=0;i<code_count;i++)
	{
		c = &code[i];
		if(c->func==func) 
		{
			return i;
		}
	}
	return 0;
}

/**
 * Sets the jumpTo position
 * @param cpos the position inside the ir code array
 * @param jmpLabel the jumpTo position, to where the code should jump to later in the final code
 * @return whether the jumpTo position was successfully set or not
 */
int setJmpLabel(int cpos, int jmpLabel)
{
	if(code[cpos].jmpLabel<0)
	{
		if((cpos>0) && (cpos<code_count))
		{
			code[cpos].jmpLabel = jmpLabel;
			return 1;
		}
	}
	return 0;
}

/**
 * Sets the opCode of the given position inside the ir code array to "opNOP", which means no operation
 * @param pos the position in the ir code array
 */
void setCodeToNOP(int pos)
{
	code[pos].op = opNOP;
}

/**
 * Resets the temp_reg_count back to 0
 */
void resetTempCount()
{
	temp_reg_count = 0;
}
