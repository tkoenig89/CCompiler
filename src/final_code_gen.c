#include "final_code_gen.h"
#include "ir_code_gen.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

FILE *gfile;
struct symInt *symIntTable;
struct symFunc *symFuncTable;
char buffer [200];
int jmpLableCount = -1;
int tmpLocalVarCount = -1;
int getAdressFromGlobal = 0;

void initFinalCodeGen(FILE *file)
{
	gfile = file;
}

void addLine(const char *str)
{
	fputs (str,gfile);
}

void generateLocalVars(struct symFunc *sFunc)
{
	struct symInt *ptr;
	int pstackcount = 8;	//gets loaded via fp
	int lstackcount = 0;		//gets loaded via sp
	
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((ptr->scope == sFunc) && (!ptr->isTemp)) {			
			if(!ptr->isParam)
			{
				ptr->stackpos = lstackcount;				
				if(ptr->isArray)
				{
					lstackcount = lstackcount + ptr->var * 4;
				}
				else
				{
					lstackcount = lstackcount + 4;
				}
			}
			else
			{
				pstackcount = pstackcount + 4;
			}
		}
	}
	
	if(lstackcount>0)
	{
		sprintf (buffer, "\tADDI $sp, $sp, -%d\t#Allocate Memory on stackpointer for local Variables\n", lstackcount);
		addLine(buffer);
		tmpLocalVarCount = lstackcount;
	}
	
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((ptr->scope == sFunc) && (!ptr->isTemp)) {
			
			//Parameters are already allocated from the function call. we only have to calculate the fp position
			if(ptr->isParam)
			{
				//ptr->stackpos = pstackcount;
				pstackcount = pstackcount - 4;
				/*
				if(ptr->isArray)
				{
					pstackcount = pstackcount + ptr->var * 4;
				}
				else
				{
					pstackcount = pstackcount + 4;
				}*/
				ptr->stackpos = pstackcount;
			}
			else
			{
				//Allocating memory for all local variables				
				if(ptr->isArray)
				{
					lstackcount = lstackcount - ptr->var * 4;
					ptr->stackpos = lstackcount;

					//addLine("\tLI $5, 0\n");
					//sprintf (buffer, "\tSW $5, %d($sp)\t#int %s[%d]\n", ptr->stackpos, ptr->name, ptr->var);
					sprintf (buffer, "\t#int %s[%d]: %d($sp)\n", ptr->name, ptr->var, ptr->stackpos);
					addLine(buffer);
				}
				else
				{
					lstackcount = lstackcount - 4;
					ptr->stackpos = lstackcount;

					//addLine("\tLI $5, 0\n");
					//sprintf (buffer, "\tSW $5, %d($sp)\t#int %s\n", ptr->stackpos, ptr->name);
					sprintf (buffer, "\t#int %s: %d($sp)\n", ptr->name, ptr->stackpos);
					addLine(buffer);
				}
			}
		}
	}	
}

int loadvar(struct symInt *sInt, int last_reg)
{
	//TODO: Look for Return Value "v0" = $2 or "v1" = $3
	
	//primary number (e.g.: 0)
	if(strcmp (sInt->name,"int") == 0)
	{
		//LI $5, 0
		sprintf (buffer, "\tLI $%d, %d\t#Number recognised:%d\n", last_reg + 1, sInt->var, sInt->var);
		addLine(buffer);
		return last_reg + 1;
	}	
	
	//temp variable:
	if(sInt->isTemp)
	{
		//$15 - $25 can be used...
		//sprintf (buffer, "\t#temp variable recognised:$%d\n", sInt->isArray + 14);
		//addLine(buffer);
		return sInt->isArray + 14;
	}
	
	if(sInt->isParam)
	{
			//LW $5, 8($fp) 
			sprintf (buffer, "\tLW $%d, %d($fp)\t#Parameter recognised:%s\n", last_reg + 1, sInt->stackpos, sInt->name);
			addLine(buffer);
			return last_reg + 1;		
	}
	
	//Global Variable:
	if(sInt->scope==NULL)
	{
		if(getAdressFromGlobal) //Nasty workaround, dont ever ever do that!
		{
			//LA $5, global
			sprintf (buffer, "\tLA $%d, %s\t#Global Variable recognised:%s\n", last_reg + 1, sInt->name, sInt->name);
			addLine(buffer);
		}
		else
		{
			sprintf (buffer, "\tLW $%d, %s\t#Global Variable recognised:%s\n", last_reg + 1, sInt->name, sInt->name);
			addLine(buffer);
		}		
		return last_reg + 1;	
	}
	else //Local Variable
	{
		if(sInt->isArray)
		{
			//LA $5, 4($sp) 
			sprintf (buffer, "\tLA $%d, %d($sp)\t#Local Variable recognised:%s\n", last_reg + 1, sInt->stackpos, sInt->name);
			addLine(buffer);
			return last_reg + 1;
		}
		else
		{
			//LW $5, 4($sp) 
			sprintf (buffer, "\tLW $%d, %d($sp)\t#Local Variable recognised:%s\n", last_reg + 1, sInt->stackpos, sInt->name);
			addLine(buffer);
			return last_reg + 1;
		}
	}	
}

void transOpCode(struct strCode  c)
{
	int i0,i1,i2, r, t0;
	struct symInt *tInt;
	i0=4;i1=4;i2=4;
	
	if(c.jmpLabel > -1)
	{
		sprintf (buffer, "l%d:\n", c.jmpLabel);
		addLine(buffer);
	}
	
	switch(c.op)
	{
		case opASSIGN:	
			getAdressFromGlobal=1;
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			getAdressFromGlobal=1;
			i0 = loadvar(c.int0, r);
			getAdressFromGlobal=0;
			
			if(c.int0->isParam)
			{
				//Target is a Parameter:
				sprintf (buffer, "\tSW $%d, %d($fp)\t#Assign one register to another\n", i1, c.int0->stackpos);
				addLine(buffer);
				break;
			}
			
			if((c.int0->scope==NULL) || (c.int0->isTemp))
			{
				//If the Variable is global or a temp register to a pointer, store the value it in the appropriate place
				sprintf (buffer, "\tSW $%d, 0($%d)\t#Assign one register to another\n", i1, i0);
			}
			else
			{
				//A Local Variable is stored in the normal stack counter:
				sprintf (buffer, "\tSW $%d, %d($sp)\t#Assign one register to another\n", i1, c.int0->stackpos);
			}
			addLine(buffer);
		break;
			
		case opRETURN:
			if(c.int0!=NULL)
			{
				i0 = loadvar(c.int0, 4);
			
				sprintf (buffer, "\tMOVE $2, $%d\t#Return %s\n", i0, c.int0->name);
				addLine(buffer);
			}
			/*
			if(tmpLocalVarCount>0)
			{
				sprintf (buffer, "\tADDI $sp, $sp, %d\t# delete local variables\n", tmpLocalVarCount);
				addLine(buffer);
			}*/		
			//TODO: Ask Andy if you should jump to the end of the function after a return command.
			//sprintf (buffer, "\tJ l%d\n", c.jmpTo);
			//addLine(buffer);
		break;
		
		case opADD:
			//i0 = i1 + i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tADD $%d, $%d, $%d\t#Add 2 Variables and store result int temp register\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opSUB:
			//i0 = i1 - i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tSUB $%d, $%d, $%d\t#Subtract 2 Variables and store result int temp register\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opMUL:
			//i0 = i1 * i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tMUL $%d, $%d, $%d\t#Multiply 2 Variables and store result int temp register\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opEQ:
			//i0 = i1 == i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			
			tInt = tempInt ("int");
			tInt->var = 1;
			if(i0<=14)
			{r=i0;} else {r=6;}
			t0 = loadvar(tInt, r);

			sprintf (buffer, "\tSUB $%d, $%d, $%d\t#If i1 == i2, i0 = 0, else i0 != 0\n", i0, i1, i2);
			addLine(buffer);			
			sprintf (buffer, "\tMOVN $%d, $%d, $%d\t#if i0 != 0, i0 = 1\n", i0, t0, i0);
			addLine(buffer);
			//sprintf (buffer, "\tNOT $%d, $%d\t#i0 = !i0\n", i0, i0);
			//addLine(buffer);
			sprintf (buffer, "\tXOR $%d, $%d, $%d\t#i0 = !i0\n", i0, i0, t0);
			addLine(buffer);
		break;
			
		case opNE:
			//i0 = i1 != i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			
			tInt = tempInt ("int");
			tInt->var = 1;
			if(i0<=14)
			{r=i0;} else {r=6;}
			t0 = loadvar(tInt, r);

			sprintf (buffer, "\tSUB $%d, $%d, $%d\t#If i1 != i2, i0 != 0, else i0 = 0\n", i0, i1, i2);
			addLine(buffer);			
			sprintf (buffer, "\tMOVN $%d, $%d, $%d\t#if i0 != 0, i0 = 1\n", i0, t0, i0);
			addLine(buffer);
		break;
			
		case opLS:
			//i0 = i1 < i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			
			sprintf (buffer, "\tSLT $%d, $%d, $%d\t#if i1 < i2 i0 = 1 else i0 = 0\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opGT:
			//i0 = i1 > i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			
			sprintf (buffer, "\tSLT $%d, $%d, $%d\t#if i1 > i2 i0 = 1 else i0 = 0\n", i0, i2, i1);
			addLine(buffer);
		break;
			
		case opLOGICAL_AND:
			//i0 = i1 && i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tAND $%d, $%d, $%d\t#Locigal And, is here equal to bit AND\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opLOGICAL_OR:
			//i0 = i1 && i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;} else {r=6;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tOR $%d, $%d, $%d\t#Locigal Or, is here equal to bit OR\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opLOGICAL_NOT:
			//i0 = !i1; i0 is always temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i0 = loadvar(c.int0, r);
			
			tInt = tempInt ("int");
			tInt->var = 1;
			if(i0<=14)
			{r=i0;} else {r=6;}
			t0 = loadvar(tInt, r);
			
			//sprintf (buffer, "\tNOT $%d, $%d\t#(pseudo):x = !y\n", i0, i1);
			//addLine(buffer);
			sprintf (buffer, "\tXOR $%d, $%d, $%d\t#i0 = !i0\n", i0, i1, t0);
			addLine(buffer);
		break;
			
		case opMINUS:
			//i0 = -i1; i0 is always temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;} else {r=5;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tNEGU $%d, $%d\t#(pseudo):x = -y\n", i0, i1);
			addLine(buffer);
		break;	

		case opIF:
			//IF i0 GOTO jmpTo
			//jmpLableCount = jmpLableCount + 1;
			i0 = loadvar(c.int0, 4);		
			//setJmpLabel(c.jmpTo, jmpLableCount);
			sprintf (buffer, "\tBGTZ $%d, l%d\n", i0, c.jmpTo);
			addLine(buffer);
		break;
		
		case opGOTO:
			//GOTO jmpTo
			//jmpLableCount = jmpLableCount + 1;
			//setJmpLabel(c.jmpTo, jmpLableCount);
			sprintf (buffer, "\tJ l%d\n", c.jmpTo);
			addLine(buffer);
		break;
		
		case opWHILE_BEGIN:
			/*Do Nothing.*/
		break;
		
		case opDO_WHILE_BEGIN:
			/*Do Nothing.*/
		break;
		
		case opMEM_LD:
			//i0 = i1[i2]; i0 is always a temp
			i2 = loadvar(c.int2, 4);
			sprintf (buffer, "\tLI $%d, 4\t#Load Number 4 into a register\n", i2 + 1);
			addLine(buffer);
			//i1 = loadvar(c.int1, i2 + 1);
			//i0 = loadvar(c.int0, i1);
			sprintf (buffer, "\tMUL $%d, $%d, $%d\t#Multiplying array position by 4 (each entry has the size of 4 bytes)\n", i2 + 2, i2 + 1, i2);
			addLine(buffer);
			i1 = loadvar(c.int1, i2 + 2);
			//sprintf (buffer, "\tLW $%d, %d($fp)\n", i2 + 2, c.int1->stackpos);
			//addLine(buffer);
			sprintf (buffer, "\tADD $%d, $%d, $%d\t#Add the starting position of the array to the position\n", i2 + 1, i2 + 2, i1);
			addLine(buffer);
		
			//c.int0->stackpos = i2 + 1;
			i0 = loadvar(c.int0, i1);
		
			sprintf (buffer, "\tLW $%d, 0($%d)\t#Load the Array position from the stack\n", i0, i2 + 1);
			//sprintf (buffer, "\tMOVE $%d, $%d\t#Move Array Pointer to Temp Register\n", i0, i2 + 1);
			addLine(buffer);
		break;
		
		case opMEM_ST:			
			i2 = loadvar(c.int2, 4);
			//sprintf(buffer, "#-----------------DEBUG:%d.\n", i2);
			//addLine(buffer);
			if(i2<=14)
			{r=i2;} else {r=5;}
			i0 = loadvar(c.int0, r);
			if(i0<=14)
			{r=i0;} else {r=6;}
			//c.int1->var = arrayPos
			
			sprintf (buffer, "\tLI $%d, %d\t#Load array position into a register\n", i0+1, c.int1->var);
			addLine(buffer);
			sprintf (buffer, "\tLI $%d, 4\t#Load Number 4 into a register\n", i0 + 2);
			addLine(buffer);
			sprintf (buffer, "\tMUL $%d, $%d, $%d\t#Multiplying array position by 4 (each entry has the size of 4 bytes)\n", i0 + 3, i0+2, i0 + 1);
			addLine(buffer);
			sprintf (buffer, "\tADD $%d, $%d, $%d\t#Add the starting position of the array to the position\n", i0 + 3, i0, i0 + 3);
			addLine(buffer);
			//sprintf (buffer, "\tSW $%d, %d($sp)\t#Assign one register to another\n", i2, r + 3);
			sprintf (buffer, "\tSW $%d, 0($%d)\t#Assign one register to another\n", i2, i0+3);
			addLine(buffer);
		break;
		
		case opPARAM:
			i0 = loadvar(c.int0, 4);
			addLine("\tADDI $sp, $sp, -4\t#Reserve 4 Bytes on the Stack for a parameter and the func call\n");
			sprintf (buffer, "\tSW $%d, 0($sp)\t#Copy Value/Adress of var to stack var\n", i0);
			addLine(buffer);
		break;
		
		case opCALL:
			i1 = loadvar(c.int1, 4);
			sprintf (buffer, "\tJAL %s\t#Call function\n", c.func->name);
			addLine(buffer);			
			if(c.int0->var>0)
			{
				sprintf (buffer, "\tADDI $sp, $sp, %d\t# clean up stack after function call is done\n", c.int0->var*4);
				addLine(buffer);
				
			}
			sprintf (buffer, "\tMOVE $%d, $2\t#Save return value by storing it into a temp register\n", i1);
			addLine(buffer);
			//TODO: Save return value to stack and not to a temp register
		break;
		
		case opFUNC_DEF:
			sprintf (buffer, "%s:\t# Beginning of a function. We will save the return adress $31 and the $fp.\n", c.func->name);
			addLine(buffer);
			addLine("\tADDI $sp, $sp, -8\n");
			addLine("\tSW $31, 4($sp)\n");
			addLine("\tSW $fp, 0($sp)\n");
			addLine("\tMOVE $fp, $sp\n\n");
			generateLocalVars(c.func);
		break;
		
		case opFUNC_DEF_END:			
			sprintf (buffer, "\t#End of function %s. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.\n", c.func->name);
			addLine(buffer);
			if(tmpLocalVarCount>0)
			{
				sprintf (buffer, "\tADDI $sp, $sp, %d\t# delete local variables\n", tmpLocalVarCount);
				addLine(buffer);
			}
			addLine("\tLW $fp, 0($sp)\n");
			addLine("\tLW $31, 4($sp)\n");
			addLine("\tADDI $sp, $sp, 8\n");
			addLine("\tJR $31\n\n");
		break;
		
		case opNOP:
			/*Do Nothing*/
		break;
		
		default:
			sprintf (buffer, "#Error: Unrecognised command %d.\n", c.op);
			addLine(buffer);	
		break;
	}
	
}

void generateFinalCode()
{
	symIntTable = getsymIntTable();
	symFuncTable = getsymFuncTable();
	struct strCode  *code = getopcodeArray();
	int code_count = getopcodeCount();
	
	//Declare Global Variables
	addLine("\n.data\n");
	
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if (ptr->scope == NULL) {
			sprintf (buffer, "%s:\n", ptr->name);
			addLine(buffer);
			if(ptr->isArray)
			{
				sprintf (buffer, "\t.word 0 : %d\n", ptr->var);
				addLine(buffer);	
			}
			else
			{
				addLine("\t.word 0\n");				
			}			
			sprintf (buffer, "%s:", ptr->name);
			addLine("\t.align 4\n");
		}
	}

	addLine("\n.text\n\n");
	
	//Calculate all Jump Labels:
	for(int i=0;i<code_count;i++)
	{		
		switch(code[i].op)
		{
			case opIF:
				//IF i0 GOTO jmpTo
				jmpLableCount = jmpLableCount + 1;
				if(!setJmpLabel(code[i].jmpTo, jmpLableCount))
					{jmpLableCount = jmpLableCount - 1;}
				code[i].jmpTo = jmpLableCount;
			break;
			
			case opGOTO:
				//GOTO jmpTo
				jmpLableCount = jmpLableCount + 1;
				if(!setJmpLabel(code[i].jmpTo, jmpLableCount))
					{jmpLableCount = jmpLableCount - 1;}
				code[i].jmpTo = jmpLableCount;
			break;
					
			case opRETURN:
				jmpLableCount = jmpLableCount + 1;
				if(!setJmpLabel(code[i].jmpTo, jmpLableCount))
					{jmpLableCount = jmpLableCount - 1;}
				code[i].jmpTo = jmpLableCount;
			break;
					
			default:
				/*Do Nothing.*/
			break;
		}
	}
	
	for(int i=0;i<code_count;i++)
	{		
		transOpCode(code[i]);
	}
}