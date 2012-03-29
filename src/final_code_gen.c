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
	int stackcount = 0;	
	
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if (ptr->scope == sFunc) {/* TODO: stuf....
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
			addLine("\talign 4\n");*/
		}
	}
}

void transOpCode(struct strCode  c)
{	
	switch(c.op)
	{
		case opASSIGN:
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