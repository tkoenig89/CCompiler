#ifndef _DHBWCC_SYMTABLE_H
#define _DHBWCC_SYMTABLE_H

//Very simple symbol table, GET IT ? :D simple symbol ... HARHAR
struct symInt
{
	char *name;
	int isArray;
	void* scope;

	int var; //is used for number of elements when the var is an array
	struct symInt *nextElement;
	
struct symInt *next;
};
//TODO: make a third 'list' struct which contains a list of parameters for a function-CALL
//will also be used as the list for the ir code...
struct symFunc
{
	char *name;
	int retType;
	int isPrototype;

	int paramCount;
	struct symInt* params; //Will point to the first parameter in the symIntTable. 
	/*Maybe we need the same count and first pointer for the variables cotained in the function?*/

struct symFunc *next;
};

struct symFuncCallParamList
{
	struct symInt *sInt;
	int count;
};

struct symInt *putInt (char const *name, int isArray, int val);
struct symInt *getInt (char const * name);
int existsInt (char const * name);
//void deleteInt (char const * name);
struct symInt *tempInt (char const *name);
//struct symInt *putIntParam (char const *name, int isArray, int val);
//int setScopeParam (char const * name);
//int setScope (char const * name);
void setIntScopeP (struct symInt *sInt);
int checkIntDec (char const * name);
struct symInt *getIntCurrScope (char const * name);

struct symFunc *putFunc (char const *name, int isVoid);
struct symFunc *getFunc (char const *name);
int existsFunc (char const *name);
void setParamP (struct symFunc *sFunc, struct symInt *sInt);
void incParamCountP (struct symFunc *sFunc);
void renameFunc (char const *funcname, char const *funcname_new);
void setTypeP (struct symFunc *sFunc, int type);
int isFuncProto (char const *funcname);
void setFuncIsDeclared (char const *funcname);
void deleteFunc (char const * name);
int paramFuncCheckP (struct symFunc *sFunc0, struct symFunc *sFunc1);
struct symFunc *getFunc (char const *name);
void setFuncProtoP (struct symFunc *sFunc);
void setFuncScopeP (struct symFunc *sFunc);
void setScopeForParams (struct symFunc *sFunc0);
struct symFuncCallParamList *createParamList(struct symInt *sInt);
int paramFuncCallCheckP (struct symFunc *sFunc0, struct symFuncCallParamList *params);

void debugPrintAllsFunc();
void debugPrintAllsint();

void init_table ();

#endif
