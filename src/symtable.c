#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Represents a Pointer to the start of the Int Table
struct symInt 		*symIntTable;
//Represents a Pointer to the start of the Function Table
struct symFunc 	*symFuncTable;
//Represents the current Function scope
struct symFunc 	*currFunc;

/**	
  *	Initializes the Symbol table(sets everything to NULL)
**/	
void init_table ()
{
	symIntTable=NULL;
	symFuncTable=NULL;
	currFunc=NULL;
	//Symbol Table was initialized
}

/**	
  *	Takes a Pointer to an Int-Symbol and enqueues it into the Symbol table
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
	ptr->isVaildForCalc=1;
	ptr->nextFuncCallParam=NULL;
	
	if(strcmp (ptr->name,"int") == 0)
	{ptr->isTemp = 1;} else {ptr->isTemp = 0;}
	
	ptr->next = NULL;
	ptr->nextElement = NULL;

	putIntIntoTable(ptr);

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
	ptr->isVaildForCalc=1;
	ptr->nextFuncCallParam=NULL;
	
	ptr->next = 137; //Temp Var Marker. It is very very bad to do that, but for the scope of the project it works. We won't do it ever again :)
	
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

/**
 * Sets the first entry to the parameterlist into the required function
 * The parameterlist itself is a linked list, and the next parameters are aviavable via "next"
 * Hint: the parameters itself will not be stored differently from other variables. the number of parameters is determenined by the paramCount value of the function
 * @param sFunc The function where the parameter should be linked to
 * @param sInt The first parameter of the function
 */
void setParamP (struct symFunc *sFunc, struct symInt *sInt)
{	
	sFunc->paramCount = sFunc->paramCount + 1;	
	sFunc->params = sInt;
	sInt->isParam = 1;
}

/**
 * Increments the paramtercount of the given function
 * @param sFunc the function which paramCount value should be incremented
 */
void incParamCountP (struct symFunc *sFunc)
{
	sFunc->paramCount = sFunc->paramCount + 1;	
}

/**
 * Changes the name of a given function to another name
 * @param funcname the old name of a function (function names are unique, so there can not be a mismatch)
 * @param funcname_new the new name of the function
 */
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

/**
 * Checks whether the given function is already in the function table, as a function declaration
 * @param funcname the name of the function
 * @return int (0==false, 1==true)
 */
int isFuncProto (char const *funcname)
{
	if(existsFunc(funcname))
	{
		struct symFunc *ptr = getFunc(funcname);
		if(ptr->isPrototype)
		{
			return 1;
		} else {
			return 0;
		}
	}
	return 0;
}

/**
 * Sets the given function to be a prototype/declaration (these names are used synonmisly in the context of this project)
 * @param sFunc the pointer to the function
 */
void setFuncProtoP (struct symFunc *sFunc)
{
	struct symFunc *ptr = sFunc;
	ptr->isPrototype = 1;
}

/**
 * Sets the given function to be a prototype/declaration (these names are used synonmisly in the context of this project)
 * @param funcname the name of the function
 */
void setFuncIsDeclared (char const *funcname)
{
	if(existsFunc(funcname))
	{
		struct symFunc *ptr = getFunc(funcname);
		ptr->isPrototype = 1;
	}
}

/**
 * Sets the return type of the given function
 * @param sFunc the pointer to the function
 * @param type the return type (0==VOID, 1==INT)
 */
void setTypeP (struct symFunc *sFunc, int type)
{
	sFunc->retType = type;
}

/**
 * Resets the current function scope to NULL. Should be called after the end of a function definition
 */
void funcEnd()
{
	currFunc = NULL;
}


/**
 * Deletes a Variable from the Symbol Table
 * @param sInt pointer to the variable which should be deleted
 */
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

/**
 * Deletes all Variables from the symbol table which are declared to be paramters of the given function
 * @param sFunc0 pointer to the function
 */
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
		temp = param0->next;
		if(temp!=NULL) 
		{
			temp = param0->next;
			if(param0!=NULL) deleteParamInt (param0);
			param0 = temp;
		}
		else
		{
			if(param0!=NULL) deleteParamInt (param0);
			break;
		}

	}
}

/**
 * Prints all paramters of a given function in human readable text
 * @param sFunc0 pointer to the function
 */
void printAllParams (struct symFunc *sFunc0)
{
	struct symInt *param0 = sFunc0->params;
	
	if(param0==NULL)
	{
		//tNo Parameters
		return;
	}

	for(int i=0;i<sFunc0->paramCount;i++)
	{
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

/**
 * Deletes a function from the symbol table
 * @param name the name of the function
 */
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
			deleteParams (ptr);
			free(ptr->name);
			free(ptr);
			break;
		}
		ptr_back = ptr;
	};
}

/**
 * Checks whether 2 parameter lists from 2 given match by type
 * @param sFunc0 the pointer to the first function
 * @param sFunc1 the pointer to the second function
 * @return int (0==false, 1==true)
 */
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

/**
 * Checks whether 2 functions are equal
 * @param sFunc0 pointer to the first function
 * @param sFunc1 pointer to the second function
 * @return int (0==false, 1==true)
 */
int isFuncEqual (struct symFunc *sFunc0, struct symFunc *sFunc1)
{
	if(strcmp (sFunc0->name,sFunc0->name) != 0)
	{
		return 0;
	}
	
	return paramFuncCheckP (sFunc0, sFunc1);
}

/**
 * Prints all functions in human readable debug format
 */
void debugPrintAllsFunc()
{
	if(symFuncTable==NULL)
	{
		printf("no functions in table.\n");
		return;
	}
	struct symFunc *ptr;
	int count = 0;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		count++;
		if(ptr!=NULL) 
		{
			printf("Function #%d name:%s,type:%d.\n", count, ptr->name, ptr->retType);
			printAllParams (ptr);
		}
	}
}

/**
 * Prints all variables in human readable debug format
 */
void debugPrintAllsint()
{
	int count = 0;
	if(symIntTable==NULL)
	{
		printf("no variables in table.\n");
		return;
	}
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if(count>100) break; else count++;
		if(strcmp (ptr->name,"int") != 0)
		{
			printf("Int: %s:=%d isArray:=%d", ptr->name, ptr->var, ptr->isArray);
			if(ptr->scope==NULL)
			{
				printf(" scope: global.\n");
			}
			else
			{
				struct symFunc *ptr_ = ptr->scope;
				printf(" scope: %s.\n", ptr_->name);
			}
		}
	}
}
 
/**
 * Sets the current function scope to the given function. Should be called at the beginning of every function definition
 * @param sFunc pointer to the function
 */
void setFuncScopeP (struct symFunc *sFunc)
{
	//setting current scope
	currFunc = sFunc;
}

/**
 * Resets the scope of all parameters of the given function to the given function
 * @param sFunc0 pointer to the function
 */
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

/**
 * Creates a temporary parameter list which is later used to check for a valid function call
 * @param sInt the first parameter of a function call
 * @return the created param list
 */
struct symFuncCallParamList *createParamList(struct symInt *sInt)
{
	struct symFuncCallParamList *ptr;
	ptr = (struct symFuncCallParamList *) malloc (sizeof (struct symFuncCallParamList));
	
	ptr->count = 1;
	ptr->sInt = sInt;
	
	return ptr;
}

/**
 * Links the 2 given Variables together via the "nextFuncCallParam" pointer, which is later used to check for a valid function call
 * @param start the start of the linked list
 * @param sInt the entry which should be added
 */
void addParamFC(struct symInt *start, struct symInt *sInt)
{
	if(start->nextFuncCallParam==NULL)
	{
		start->nextFuncCallParam = sInt;
	}
	else
	{
		struct symInt *ptr;
		for (ptr = start->nextFuncCallParam; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->nextFuncCallParam) {
			if(ptr->nextFuncCallParam==NULL)
			{
				ptr->nextFuncCallParam = sInt;
				break;
			}
		}
	}
}

/**
 * Checks the parameters of a function call against its parameters from the declaration/definition
 * @param sFunc0 the function which should be checked
 * @param params the parameter list from the function call
 * @return int (0==false, 1==true)
 */
int paramFuncCallCheckP (struct symFunc *sFunc0, struct symFuncCallParamList *params)
{
	if(sFunc0->paramCount != params->count)
	{
		return 0;
	}

	struct symInt *param0 = sFunc0->params;
	struct symInt *param1 = params->sInt;

	for(int i=0;i<sFunc0->paramCount;i++)
	{		
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
		
		if(param1->nextFuncCallParam!=NULL)
		{
			param1 = param1->nextFuncCallParam;
		}
		else
		{
			break;
		}
	}

	return 1;
}

/**
 * Returns the Variables symbol table, represented via a linked list.
 * @return variable symbol table
 */
struct symInt *getsymIntTable()
{
	return symIntTable;
}

/**
 * Returns the Function symbol table, represented via a linked list.
 * @return function symbol table
 */
struct symFunc *getsymFuncTable()
{
	return symFuncTable;
}

/**
 * Returns the current function scope
 * @return current function scope.
 */
struct symFunc *getCurrentScope()
{
	return currFunc;
}
