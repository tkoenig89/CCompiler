#include "ir_code_gen.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct strCode  *code;

int code_offset = 0;

/* Generates code at current location */
void addcode(enum code_ops operation, struct symInt *int0, struct symInt *int1, struct symInt *int2, struct symFunc *func, const char *jmplabel)
{
	code_offset += 1;
	
	struct strCode *codebuffer = (struct strCode*) realloc (code, code_offset * sizeof(struct strCode));
	
	code = codebuffer;
	
	code[code_offset-1].op = operation;
	
	code[code_offset-1].int0 = int0;
	code[code_offset-1].int1 = int1;
	code[code_offset-1].int2 = int2;
	code[code_offset-1].func = func;
	code[code_offset-1].jmplabel = jmplabel;
}

void addcodeass(struct symInt *int0, struct symInt *int1)
{
	//int0 = int1
	addcode(opASSIGN, int0, int1, NULL, NULL, NULL);
	printf("Code offset: %d\n", code_offset);
	printf("IR: ASSIGN %s = %s\n", code[code_offset-1].int0->name, code[code_offset-1].int1->name);
}

void addcodemin(struct symInt *int0, struct symInt *int1)
{
	//int0 = -int1
	addcode(opMINUS, int0, int1, NULL, NULL, NULL);
}

void addcodesop(enum code_ops operation, struct symInt *int0, struct symInt *int1, struct symInt *int2)
{
	//int0 = int1 +*/- int2
	addcode(operation, int0, int1, int2, NULL, NULL);
}

/* Generates code at a reserved location */
/*
void backpatch(int addr, enum code_ops operation, int arg )
{
	code[addr].op = operation;
	code[addr].arg = arg;
}
*/
void printcode()
{
	int i = 0;
	while (i < code_offset) {
		//printf("%3ld: %-10s%4ld\n",i,op_name[(int) code[i].op], code[i].arg );
		//printf("%s "
		i++;
	}
}