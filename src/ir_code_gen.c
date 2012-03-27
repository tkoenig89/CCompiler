#include "ir_code_gen.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct strCode  *code;

int code_count = 0;
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
void addcode(enum code_ops operation, struct symInt *int0, struct symInt *int1, struct symInt *int2, struct symFunc *func, const char *jmplabel, struct strCode *jmpTo)
{
	code_count += 1;
	//TODO: Check whether realloc does really work. if it doesnt make it a linked list
	struct strCode *codebuffer = (struct strCode*) realloc (code, code_count * sizeof(struct strCode));
	
	code = codebuffer;
	
	code[code_count-1].op = operation;
	
	code[code_count-1].int0 = int0;
	code[code_count-1].int1 = int1;
	code[code_count-1].int2 = int2;
	code[code_count-1].func = func;
	code[code_count-1].jmplabel = jmplabel;
	code[code_count-1].jmpTo = jmpTo;
}

void addcodeass(struct symInt *int0, struct symInt *int1)
{
	//int0 = int1
	addcode(opASSIGN, int0, int1, NULL, NULL, NULL, NULL);
	printf("Code offset: %d\n", code_count);
	printf("IR: ASSIGN %s = %s\n", code[code_count-1].int0->name, code[code_count-1].int1->name);
	
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
	struct symInt *ptr;
	if(int1->scope!=137)
	{
		ptr = irtempInt();
	}
	else
	{
		ptr=int1;		
	}
	

	addcode(operation, ptr, int1, NULL, NULL, NULL, NULL);
	printf("IR: %d %s = op %s\n", operation, ptr->name, int1->name);
	
	return ptr;
}

struct symInt *addcodeopexp2(enum code_ops operation, struct symInt *int1, struct symInt *int2)
{
	//TODO: If we regocnise that int1 and int2 are already a temp vars, we use either int1 or int2 as the result instead of creating a new temp var to save register space
	struct symInt *ptr;
	if((int1->scope!=137) && (int2->scope!=137))
	{
		ptr = irtempInt();
	}
	else
	{
		ptr = int1;
		temp_reg_count -= 1;
		//printf("temp_reg_count:%d.\n", temp_reg_count);
	}

	addcode(operation, ptr, int1, int2, NULL, NULL, NULL);
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
	while (i < code_count) {
		//printf("%3ld: %-10s%4ld\n",i,op_name[(int) code[i].op], code[i].arg );
		//printf("%s "
		i++;
	}
}

void debugPrintAllopcodes()
{
	struct strCode  *c;	
	struct symInt *int_;
	struct symInt *func_;
	struct strCode  *c_;
	
	for(int i=0;i<code_count;i++)
	{		
		c = &code[i];
		printf("OP: %s", enumStrings[c->op]);
		
		if(c->int0!=NULL) {int_=c->int0;printf(", INT0: %s", int_->name);}
		if(c->int1!=NULL) {int_=c->int1;printf(", INT1 %s", int_->name);}
		if(c->int2!=NULL) {int_=c->int2;printf(", INT2: %s", int_->name);}
		
		if(c->func!=NULL) {func_=c->func;printf(", FUNC: %s", func_->name);}
		
		if(c->jmplabel!=NULL) {printf(", JMP_LABEL: %s", c->jmplabel);}
		//if(c->jmpTo!=NULL) {*c_=c->jmpTo;printf(", INT2: %s", c_->name);}
		
		printf("\n");
	}
}

struct strCode  *getopcodeArray()
{
	return code;
}

int getopcodeCount()
{
	return code_count;
}