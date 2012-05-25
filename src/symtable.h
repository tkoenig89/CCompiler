#ifndef _DHBWCC_SYMTABLE_H
#define _DHBWCC_SYMTABLE_H

//HINT: Some Attributes are used in ways that are not represented by their name. Please watch out.

struct symInt
{
	char *name;
	int isArray;					
	int isParam;
	struct symFunc *scope;				//If the var is global than scope=NULL , otherwise this points to the function where the var was declared
	int var; 							//is used for number of elements when the var is an array
	int tempArrPos;
	struct symInt *tempArrPos2;
	int tempCodePos;
	int stackpos; 						//used by final_code for reference 
	int isTemp;
	int isVaildForAssign; 				//used in the parser to check if the expression to the left of "=" is valid
	int isVaildForCalc; 				//used in the parser to check if the expression to the left of "=" is valid
	struct symInt *nextElement;
	struct symInt *nextFuncCallParam;	//only used for temporarily stored function call parameters, to validate the function call
	
struct symInt *next;
};

struct symFunc
{
	char *name;
	int retType;
	int isPrototype;
	int paramCount;
	struct symInt* params; //Will point to the first parameter in the symIntTable. 

struct symFunc *next;
};

struct symFuncCallParamList
{
	struct symInt *sInt;
	int count;
};

//Public function declarations:
	//For a detailed description, please have a look into the symtable.c file

struct symInt *putInt (char const *name, int isArray, int val);
struct symInt *getInt (char const * name);
int existsInt (char const * name);
int existsIntG (char const * name);
struct symInt *tempInt (char const *name);
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
void addParamFC(struct symInt *start, struct symInt *sInt);

struct symInt *getsymIntTable();
struct symFunc *getsymFuncTable();

void debugPrintAllsFunc();
void debugPrintAllsint();

void init_table ();

#endif
