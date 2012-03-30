#include "final_code_gen.h"
#include "ir_code_gen.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

FILE *gfile;
struct symInt *symIntTable;
struct symFunc *symFuncTable;
char buffer [200];

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
		}
	}
	
	if(lstackcount>0)
	{
		sprintf (buffer, "\tADDI $sp, $sp, -%d\t#Allocate Memory on stackpointer for local Variables\n", lstackcount);
		addLine(buffer);
	}
	
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((ptr->scope == sFunc) && (!ptr->isTemp)) {
			
			//Parameters are already allocated from the function call. we only have to calculate the fp position
			if(ptr->isParam)
			{
				ptr->stackpos = pstackcount;
				if(ptr->isArray)
				{
					pstackcount = pstackcount + ptr->var * 4;
				}
				else
				{
					pstackcount = pstackcount + 4;
				}
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
		//LA $5, global
		sprintf (buffer, "\tLA $%d, %s\t#Global Variable recognised:%s\n", last_reg + 1, sInt->name, sInt->name);
		addLine(buffer);
		return last_reg + 1;
	
	}
	else //Local Variable
	{
		//LW $5, 4($sp) 
		sprintf (buffer, "\tLW $%d, %d($sp)\t#Local Variable recognised:%s\n", last_reg + 1, sInt->stackpos, sInt->name);
		addLine(buffer);
		return last_reg + 1;
	}
}

void transOpCode(struct strCode  c)
{
	int i0,i1,i2, r;
	i0=4;i1=4;i2=4;
	
	switch(c.op)
	{
		case opASSIGN:			
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i0 = loadvar(c.int0, r);
			
			if((c.int0->scope==NULL) || (c.int0->isTemp))
			{
				//If the Variable is global or a temp register to a pointer, store the value it in the appropriate place
				sprintf (buffer, "\tSW $%d, 0($%d)\t#Assign one register to another\n", i1, i0);
			}
			else
			{
				sprintf (buffer, "\tSW $%d, %d($sp)\t#Assign one register to another\n", i1, c.int0->stackpos);
			}
			
			
			addLine(buffer);
		break;
		
		case opADD:
			//i0 = i1 OP i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tADD $%d, $%d, $%d\t#Add 2 Variables and store result int temp register\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opSUB:
			//i0 = i1 OP i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tSUB $%d, $%d, $%d\t#Subtract 2 Variables and store result int temp register\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opMUL:
			//i0 = i1 OP i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tMUL $%d, $%d, $%d\t#Multiply 2 Variables and store result int temp register\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opLOGICAL_AND:
			//i0 = i1 && i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tAND $%d, $%d, $%d\t#Locigal And, is here equal to bit AND\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opLOGICAL_OR:
			//i0 = i1 && i2; i0 is always a temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i2 = loadvar(c.int2, r);
			if(i2<=14)
			{r=i2;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tOR $%d, $%d, $%d\t#Locigal Or, is here equal to bit OR\n", i0, i1, i2);
			addLine(buffer);
		break;
			
		case opLOGICAL_NOT:
			//i0 = !i1; i0 is always temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tNOT $%d, $%d\t#(pseudo):x = !y\n", i0, i1);
			addLine(buffer);
		break;
			
		case opMINUS:
			//i0 = -i1; i0 is always temp
			i1 = loadvar(c.int1, 4);
			if(i1<=14)
			{r=i1;}
			i0 = loadvar(c.int0, r);
			sprintf (buffer, "\tNEGU $%d, $%d\t#(pseudo):x = -y\n", i0, i1);
			addLine(buffer);
		break;			
		
		case opMEM_LD:
			//i0 = i1[i2]; i0 is always a temp
			i2 = loadvar(c.int2, 4);
			sprintf (buffer, "\tLI $%d, 4\t#Load array position into a register\n", i2 + 1);
			addLine(buffer);
			//i1 = loadvar(c.int1, i2 + 1);
			//i0 = loadvar(c.int0, i1);
			sprintf (buffer, "\tMUL $%d, $5, $6\t#Multiplying array position by 4 (each entry has the size of 4 bytes)\n", i2 + 2, i2 + 1, i2);
			addLine(buffer);
			i1 = loadvar(c.int1, i2 + 2);
			//sprintf (buffer, "\tLW $%d, %d($fp)\n", i2 + 2, c.int1->stackpos);
			//addLine(buffer);
			sprintf (buffer, "\tADD $%d, $%d, $%d\t#Add the starting position of the array to the position\n", i2 + 1, i2 + 2, i1);
			addLine(buffer);
		
			i0 = loadvar(c.int0, i1);
		
			sprintf (buffer, "\tLW $%d, 0($%d)\t#Load the Array position from the stack\n", i0, i2 + 1);
			addLine(buffer);
		break;
		
		case opFUNC_DEF:
			sprintf (buffer, "\t%s:\t# Beggining of a function. We will save the return adress $31 and the $fp.\n", c.func->name);
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
			addLine("\tLW $fp, 0($sp)\n");
			addLine("\tLW $31, 4($sp)\n");
			addLine("\tADDI $sp, $sp, 8\n");
			addLine("\tJR $31\n");
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
	addLine(".data\n");
	
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if (ptr->scope == NULL) {
			sprintf (buffer, ".%s:\n", ptr->name);
			addLine(buffer);
			if(ptr->isArray)
			{
				sprintf (buffer, "\t.word 0 : %d\n", ptr->var);
				addLine(buffer);	
			}
			else
			{
				addLine("\t.word n");				
			}			
			sprintf (buffer, ".%s:", ptr->name);
			addLine("\talign 4\n");
		}
	}
	
	//Jump to the main function as the entry point to the application
	addLine("\n\n.text\n\n");
	addLine("_start:\n");
	addLine("\tJAL main\n\n");
	
	for(int i=0;i<code_count;i++)
	{		
		transOpCode(code[i]);
	}
}