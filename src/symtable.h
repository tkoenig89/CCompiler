#ifndef _DHBWCC_SYMTABLE_H
#define _DHBWCC_SYMTABLE_H

//Very simple symbol table, GET IT ? :D simple symbol ... HARHAR
struct symInt
{
	char *name;
	int isArray;
	void* scope;
	union
	{
		int var;
		struct symInt *nextElement;
	} value;
	struct symInt *next;
};

struct symFunc
{
	char *name;
	int isRetInt;
	union
	{
		int paramCount;
		struct symInt *params;
	} params;
struct symFunc *next;
};

//typedef struct symrec symrec;

/* The symbol table: a chain of struct symrec. */
//extern symInt *symIntTable;

//Function prototypes
struct symInt *putInt (char const *name, int isArray, int val, void *scope);
struct symInt *getInt (char const * name, void *scope);
int existsInt (char const * name, void *scope);
void deleteInt (char const * name, void *scope);

void init_table ();

#endif
