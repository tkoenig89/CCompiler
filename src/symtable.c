#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

//Represents a Pointer to the start of the Int Table
struct symInt 		*symIntTable;
//Represents a Pointer to the start of the Function Table
struct symFunc 	*symFuncTable;
//Represents the current Function scope
struct symFunc 	*currFunc;

/**	
  *	Initiliases the Symbol table(sets everything to NULL)
**/	
void init_table ()
{
	symIntTable=NULL;
	symFuncTable=NULL;
	currFunc=NULL;
	//printf("Symbol Table was initialised.\n");
}

/**	
  *	Takes a Pointer to an Int-Symbol and enques it into the Symbol table
  *	@param struct symInt *sInt - A pointer to the Int Symbol
**/
void putIntIntoTable (struct symInt *sInt)
{
	if(symIntTable==NULL)
	{
		symIntTable = sInt;
	}
	else
	{
		struct symInt *ptr;
		for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
			if(ptr->next==NULL)
			{
				ptr->next = sInt;
				break;
			}
		}
	}
}

/**	
  *	Creates an Int-Symbol and returns a pointer to the just created Int
  *	The Int-Symbol will also be automatically be put into the Symbol Table
  *	@param char const *name - The Name of the Int Symbol
  *	@param int isArray - A Flag which determines whether the Int Symbol is an array or not
  *	@param int val - Represents the size of an array (e.g. a[10]; val=10)
  *	@return struct symInt * - The pointer to the just created Int Symbol
**/
struct symInt *putInt (char const *name, int isArray, int val)
{
	struct symInt *ptr;
	//set name
	ptr = (struct symInt *) malloc (sizeof (struct symInt));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	//set value
	ptr->var = val; 
	//set isArray
	ptr->isArray = isArray; 
	//set scope; NULL == global
	ptr->scope = currFunc; 
	
	ptr->isParam = 0;
	ptr->stackpos = 0;	
	ptr->tempArrPos=-1;
	ptr->tempArrPos2=NULL;
	ptr->tempCodePos=-1;
	ptr->isVaildForAssign=-1;
	
	if(strcmp (ptr->name,"int") == 0)
	{ptr->isTemp = 1;} else {ptr->isTemp = 0;}
	
	ptr->next = NULL;
	ptr->nextElement = NULL;

	putIntIntoTable(ptr);
	
	//printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->var);
	return ptr;
}

/**	
  *	Creates an Int-Symbol and returns a pointer to the just created Int
  *	@param char const *name - The Name of the Int Symbol
  *	@return struct symInt * - The pointer to the just created Int Symbol
**/
struct symInt *tempInt (char const *name)
{
	struct symInt *ptr;
	//set name
	ptr = (struct symInt *) malloc (sizeof (struct symInt));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	
	ptr->isArray = 0; 
	ptr->scope = NULL;
	
	ptr->isParam = 0;
	ptr->stackpos = 0;	
	ptr->isTemp = 1;
	ptr->tempArrPos = -1;
	ptr->tempArrPos2=NULL;
	ptr->tempCodePos=-1;
	ptr->isVaildForAssign=-1;
	
	ptr->next = 137; //Temp Var Marker... 
	
	return ptr;
}

/**	
  *	Checks whether an Int Symbol, represented by its name, exists in the Symbol table in a global scope
  *	@param char const *name - The Name of the Int Symbol
  *	@return int - 1 If the Int-Symbol exists at a global scope; 0 If it was not found or the scope did not match
**/
int existsIntG (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == NULL)) {
			return 1;
		}
	}
	return 0;
}

/**	
  *	Checks whether an Int Symbol, represented by its name, exists in the Symbol table in a local scope
  *	@param char const *name - The Name of the Int Symbol
  *	@return int - 1 If the Int-Symbol exists at a local scope; 0 If it was not found or the scope did not match
**/
int existsIntL (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == currFunc) ) {
			return 1;
		}
	}
	return 0;
}

/**	
  *	Checks whether an Int Symbol exists, represented by its name
  *	@param char const *name - The Name of the Int Symbol
  *	@return int - 1 If the Int-Symbol exists; 0 If it was not found
**/
int existsInt (char const * name)
{
	if(!existsIntL(name)) {
		return existsIntG(name);
	} else {
		return existsIntL(name);
	}
}

/**	
  *	Checks whether an Int Symbol, represented by its name, exists in the Symbol table in a global scope
  *	and returns the pointer to the Int Symbol, if it was found.
  *	@param char const *name - The Name of the Int Symbol
  *	@return struct symInt * - A Pointer to the found Int Symbol, or NULL if the symbol was not found
**/
struct symInt *getIntG (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == NULL)) {
			return ptr;
		}
	}
	return NULL;
}

/**	
  *	Checks whether an Int Symbol, represented by its name, exists in the Symbol table in a local scope
  *	and returns the pointer to the Int Symbol, if it was found.
  *	@param char const *name - The Name of the Int Symbol
  *	@return struct symInt * - A Pointer to the found Int Symbol, or NULL if the symbol was not found
**/
struct symInt *getIntL (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == currFunc) ) {
			return ptr;
		}
	}
	return NULL;
}

/**	
  *	Checks whether an Int Symbol, represented by its name, exists in the Symbol table in a global or local scope
  *	and returns the pointer to the Int Symbol, if it was found.
  *	The local scope has a higher priority and if both Int Symbols where found, just the local one will be returned.
  *	@param char const *name - The Name of the Int Symbol
  *	@return struct symInt * - A Pointer to the found Int Symbol, or NULL if the symbol was not found
**/
struct symInt *getInt (char const * name)
{
	if(!existsIntL(name)) {
		if(existsIntG(name)) {
			return getIntG(name);			
		} else {
			return NULL;
		}
	} else {
		return getIntL(name);
	}
}

/**	
  *	Checks whether an Int Symbol, represented by its name, already exists in the current Function scope
  *	(If it does there would have to be an error, because the Int was already declared)
  *	@param char const *name - The Name of the Int Symbol
  *	@return int - 1 If the Int Symbol already exists in the current Function scope; 0 if it does not
**/
int checkIntDec (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == currFunc)) {
			return 1;
		}
	}
	return 0;
}

/**	
  *	Checks whether an Int Symbol, represented by its name, already exists in the current Function scope
  *	(If it does there would have to be an error, because the Int was already declared)
  *	If the Symbol was found, a pointer to it will be returned.
  *	@param char const *name - The Name of the Int Symbol
  *	@return struct symInt * - A Pointer to the found Int Symbol, or NULL if the symbol was not found
**/
struct symInt *getIntCurrScope (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == currFunc)) {
			return ptr;
		}
	}
	return NULL;
}

/**	
  *	Sets the scope of the given Int Symbol to the current Function scope
  *	@param struct symInt *sInt - A pointer to the Int Symbol
**/
void setIntScopeP (struct symInt *sInt)
{
	sInt->scope = currFunc;
}

/**	
  *	Inserts a given Func Symbol into the Function Table
  *	@param struct symFunc *sFunc - A pointer to the Func Symbol
**/
void putFuncIntoTable (struct symFunc *sFunc)
{
	if(symFuncTable==NULL)
	{
		symFuncTable = sFunc;
	}
	else
	{
		struct symFunc *ptr;
		for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
			if(ptr->next==NULL)
			{
				ptr->next = sFunc;
				break;
			}
		}
	}
}

/**	
  *	Creates a Function-Symbol and returns a pointer to the just created Function
  *	The Function-Symbol will also be automatically be put into the Symbol Table
  *	@param char const *name - The Name of the Int Symbol
  *	@param int retType - The return Type of the Function
  *	@return struct symFunc * - The pointer to the just created Function Symbol
**/
struct symFunc *putFunc (char const *name, int retType)
{
	struct symFunc *ptr;
	//set name
	ptr = (struct symFunc *) malloc (sizeof (struct symFunc));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	//set retType
	ptr->retType = retType; 
	//set paramcount=0
	ptr->paramCount = 0; 	
	ptr->params = NULL;
	ptr->isPrototype = 0;
	
	ptr->next = NULL;
	//symFuncTable = ptr;
	
	putFuncIntoTable(ptr);
	
	//printf("Function ->%s<- was put in the table pc:%d.\n", ptr->name, ptr->paramCount);
	
	currFunc = ptr;
	
	return ptr;
}

/**	
  *	Creates a Function-Symbol and returns a pointer to the just created Function
  *	The Function-Symbol will also be automatically be put into the Symbol Table
  *	@param char const *name - The Name of the Int Symbol
  *	@return struct symFunc * - The pointer to the found Function; NULL if no function was found
**/
struct symFunc *getFunc (char const *name)
{
	struct symFunc *ptr;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		if (strcmp (ptr->name,name) == 0) {
			return ptr;
		}
	}	
	return NULL;
}

/**	
  *	Checks whether a Function is inside the Symbol table, represented by its name
  *	@param char const *name - The Name of the Int Symbol
  *	@return int - 
**/
int existsFunc (char const *name) 
{
	struct symFunc *ptr;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		if (strcmp (ptr->name,name) == 0) {
			return 1;
		}
	}	
	return 0;
}

void setParamP (struct symFunc *sFunc, struct symInt *sInt)
{	
	sFunc->paramCount = sFunc->paramCount + 1;	
	sFunc->params = sInt;
	sInt->isParam = 1;
	//printf("added var %s for func %s, parcount:%d.\n", sInt->name, sFunc->name, sFunc->paramCount);
}

void incParamCountP (struct symFunc *sFunc)
{
	sFunc->paramCount = sFunc->paramCount + 1;	
}


void renameFunc (char const *funcname, char const *funcname_new)
{
	if(existsFunc(funcname))
	{
		//printf("renaming func %s to %s.\n", funcname, funcname_new);
		struct symFunc *ptr = getFunc(funcname);
		free(ptr->name);
		
		ptr->name = (char *) malloc (strlen (funcname_new) + 1);
		strcpy (ptr->name,funcname_new);		
		//printf("function name is now:%s.\n", ptr->name);
	}
}

int isFuncProto (char const *funcname)
{
	if(existsFunc(funcname))
	{
		struct symFunc *ptr = getFunc(funcname);
		return ptr->isPrototype;
	}
	return NULL;
}

void setFuncProtoP (struct symFunc *sFunc)
{
	struct symFunc *ptr = sFunc;
	ptr->isPrototype = 1;
}

void setFuncIsDeclared (char const *funcname)
{
	if(existsFunc(funcname))
	{
		struct symFunc *ptr = getFunc(funcname);
		ptr->isPrototype = 1;
	}
}

void setTypeP (struct symFunc *sFunc, int type)
{
	sFunc->retType = type;
}

void funcEnd()
{
	currFunc = NULL;
}

void deleteParamInt (struct symInt *sInt)
{
	struct symInt *ptr;
	struct symInt *ptr_back;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if (ptr==sInt) {			
			if(ptr == symIntTable) {
				symIntTable = ptr->next;
			} else {
				ptr_back->next = ptr->next;
			}
			free(sInt->name);
			free(sInt);			
			break;
		}
		ptr_back = ptr;
	};
}

void deleteParams (struct symFunc *sFunc0)
{
	struct symInt *param0 = sFunc0->params;
	struct symInt *temp;
	
	if(param0==NULL)
	{
		return;
	}

	for(int i=0;i<sFunc0->paramCount;i++)
	{	
		//printf("param:%s.\n", param0->name);
		temp = param0->next;
		if(temp!=NULL) 
		{
			temp = param0->next;
			if(param0!=NULL) deleteParamInt (param0);
			param0 = temp;
		}
		else
		{
			//printf("temp==NULL\n");
			if(param0!=NULL) deleteParamInt (param0);
			break;
		}

	}
}

void printAllParams (struct symFunc *sFunc0)
{
	struct symInt *param0 = sFunc0->params;
	
	if(param0==NULL)
	{
		//printf("\tNo Parameters.\n");
		return;
	}

	for(int i=0;i<sFunc0->paramCount;i++)
	{
		//printf("\tParam #%d :%s.\n", (i+1), param0->name);
		
		if(param0->next!=NULL) 
		{
			param0 = param0->next;
		}
		else
		{
			break;
		}

	}
}

void deleteFunc (char const * name)
{
	struct symFunc *ptr;
	struct symFunc *ptr_back;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		if (strcmp (ptr->name,name) == 0) {
			if(ptr == symFuncTable) {
				symFuncTable = ptr->next;
			} else {
				ptr_back->next = ptr->next;
			}
			//printf("%s was deleted as a function\n", ptr->name);
			deleteParams (ptr);
			free(ptr->name);
			free(ptr);
			break;
		}
		ptr_back = ptr;
	};
}

int paramFuncCheckP (struct symFunc *sFunc0, struct symFunc *sFunc1)
{
	if(sFunc0 == sFunc1) 
	{
		return 1;
	}

	if(sFunc0->paramCount != sFunc1->paramCount)
	{
		return 0;
	}

	struct symInt *param0 = sFunc0->params;
	struct symInt *param1 = sFunc1->params;

	for(int i=0;i<sFunc0->paramCount;i++)
	{
		if(strcmp (param0->name, param1->name) != 0)
		{
			return 0;
		}
		
		if(param0->isArray != param1->isArray)
		{
			return 0;
		}
		
		if(param0->isArray)
		{
			if(param0->var != param1->var)
			{
			return 0;
			}
		}
		
		if(param0->next!=NULL) 
		{
			param0 = param0->next;
		}
		else
		{
			break;
		}
		
		if(param1->next!=NULL) 
		{
			param1 = param1->next;
		}
		else
		{
			break;
		}
	}

	return 1;
}

int isFuncEqual (struct symFunc *sFunc0, struct symFunc *sFunc1)
{
	if(strcmp (sFunc0->name,sFunc0->name) != 0)
	{
		return 0;
	}
	
	return paramFuncCheckP (sFunc0, sFunc1);
}

void debugPrintAllsFunc()
{
	if(symFuncTable==NULL)
	{
		//printf("no functions in table.\n");
		return;
	}
	struct symFunc *ptr;
	int count = 0;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		count++;
		if(ptr!=NULL) 
		{
			//printf("Function #%d name:%s,type:%d.\n", count, ptr->name, ptr->retType);
			printAllParams (ptr);
		}
	}
}

void debugPrintAllsint()
 {
	int count = 0;
	if(symIntTable==NULL)
	{
		//printf("no variables in table.\n");
		return;
	}
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if(count>100) break; else count++;
		//if(strcmp (ptr->name,"int") != 0)
		//{
			//printf("Int: %s:=%d isArray:=%d", ptr->name, ptr->var, ptr->isArray);
			if(ptr->scope==NULL)
			{
				printf(" scope: global.\n");
			}
			else
			{
				struct symFunc *ptr_ = ptr->scope;
				//printf(" scope: %s.\n", ptr_->name);
			}
		//}
	}
 }
 
void setFuncScopeP (struct symFunc *sFunc)
{
	//printf("setting current scope to:%s.\n", sFunc->name);
	currFunc = sFunc;
}

void setScopeForParams (struct symFunc *sFunc0)
{

	struct symInt *param0 = sFunc0->params;
	
	if(param0==NULL)
	{
		return;
	}

	for(int i=0;i<sFunc0->paramCount;i++)
	{
		param0->scope = sFunc0;
		if(param0->next!=NULL) 
		{
			param0 = param0->next;
		}
		else
		{
			break;
		}

	}
}

struct symFuncCallParamList *createParamList(struct symInt *sInt)
{
	struct symFuncCallParamList *ptr;
	ptr = (struct symFuncCallParamList *) malloc (sizeof (struct symFuncCallParamList));
	
	ptr->count = 1;
	ptr->sInt = sInt;
	
	return ptr;
}

int paramFuncCallCheckP (struct symFunc *sFunc0, struct symFuncCallParamList *params)
{
	/*
	if(sFunc0 == sFunc1) 
	{
		return 1;
	}*/

	if(sFunc0->paramCount != params->count)
	{
		return 0;
	}

	struct symInt *param0 = sFunc0->params;
	struct symInt *param1 = params->sInt;

	for(int i=0;i<sFunc0->paramCount;i++)
	{
		/*
		if(strcmp (param0->name, param1->name) != 0)
		{
			return 0;
		}*/
		
		if(param0->isArray != param1->isArray)
		{
			return 0;
		}
		
		if(param0->isArray)
		{
			if(param0->var != param1->var)
			{
				return 0;
			}
		}
		
		if(param0->next!=NULL) 
		{
			param0 = param0->next;
		}
		else
		{
			break;
		}
		
		if(param1->next!=NULL) 
		{
			param1 = param1->next;
		}
		else
		{
			break;
		}
	}

	return 1;
}

struct symInt *getsymIntTable()
{
	return symIntTable;
}

struct symFunc *getsymFuncTable()
{
	return symFuncTable;
}

struct symFunc *getCurrentScope()
{
	return currFunc;
}