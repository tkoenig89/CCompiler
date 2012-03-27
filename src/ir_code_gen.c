#include "ir_code_gen.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct strCode  *code;

int code_offset = 0;
int temp_reg_count = -1;

struct symInt *irtempInt() 
{
	temp_reg_count += 1;
	//if temp_reg_count > 21 then ERROR, no space left for any more temp registers
	//we have to work around and put them into memory	
	char buffer [5];
	sprintf (buffer, ".t%d", temp_reg_count);
	
	struct symInt *ptr;
	ptr = tempInt (buffer);
	ptr->isArray = temp_reg_count;
	
	return ptr;
}

/* Generates code at current location */
void addcode(enum code_ops operation, struct symInt *int0, struct symInt *int1, struct symInt *int2, struct symFunc *func, const char *jmplabel)
{
	code_offset += 1;
	//TODO: Check whether realloc does really work. if it doesnt make it a linked list
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
	
	temp_reg_count = 0;
}
/*
struct symInt *addcodemin(struct symInt *int1)
{
	struct symInt *ptr = irtempInt();
	
	addcode(opMINUS, ptr, int1, NULL, NULL, NULL);
	printf("IR: MINUS %s = - %s\n", ptr->name, int1->name);
	
	return ptr;
}*/

struct symInt *addcodeopexp1(enum code_ops operation, struct symInt *int1)
{
	//TODO: If we regocnise that int1 is already a temp var, use int1 as the result instead of creating a new temp var to save register space
	struct symInt *ptr = irtempInt();

	addcode(operation, ptr, int1, NULL, NULL, NULL);
	printf("IR: %d %s = op %s\n", operation, ptr->name, int1->name);
	
	return ptr;
}

struct symInt *addcodeopexp2(enum code_ops operation, struct symInt *int1, struct symInt *int2)
{
	//TODO: If we regocnise that int1 and int2 are already a temp vars, we use either int1 or int2 as the result instead of creating a new temp var to save register space
	struct symInt *ptr = irtempInt();

	addcode(operation, ptr, int1, int2, NULL, NULL);
	printf("IR: %d %s = %s op %s\n", operation, ptr->name, int1->name, int2->name);
	
	return ptr;
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