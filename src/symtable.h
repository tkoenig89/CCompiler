#ifndef _DHBWCC_SYMTABLE_H
#define _DHBWCC_SYMTABLE_H

struct symInt
{
	char *name;
	int isArray;
	int isParam;
	struct symFunc *scope;
	int var; //is used for number of elements when the var is an array
	int tempArrPos;
	struct symInt *tempArrPos2;
	int tempCodePos;
	int stackpos; //used by final_code for reference 
	int isTemp;
	int isVaildForAssign;
	int isVaildForCalc;
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
int existsIntG (char const * name);
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
struct symFunc *getCurrentScope();

struct symInt *getsymIntTable();
struct symFunc *getsymFuncTable();

void debugPrintAllsFunc();
void debugPrintAllsint();

void init_table ();

#endif
