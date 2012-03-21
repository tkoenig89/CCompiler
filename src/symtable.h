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
	int isVoid;
	union
	{
		int paramCount;
		struct symInt *params; //Will be a dynamic array with realloc
	} params;
struct symFunc *next;
};

struct symInt *putInt (char const *name, int isArray, int val, void *scope);
struct symInt *getInt (char const * name, void *scope);
int existsInt (char const * name, void *scope);
void deleteInt (char const * name, void *scope);

struct symFunc *putFunc (char const *name, int isVoid);
struct symFunc *getFunc (char const *name);
int existsFunc (char const *name);
void addParam (char const *funcname, struct symInt *sInt);

void init_table ();

#endif
