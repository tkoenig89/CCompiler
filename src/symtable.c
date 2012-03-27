#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct symInt 		*symIntTable;
struct symFunc 	*symFuncTable;
struct symFunc 	*currFunc;

void init_table ()
{
	symIntTable=NULL;
	symFuncTable=NULL;
	currFunc=NULL;
	printf("Symbol Table was initialised.\n");
}

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
	
	ptr->next = NULL;
	//symIntTable = ptr;

	putIntIntoTable(ptr);
	
	printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->var);
	return ptr;
}

struct symInt *tempInt (char const *name)
{
	struct symInt *ptr;
	//set name
	ptr = (struct symInt *) malloc (sizeof (struct symInt));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	
	ptr->isArray = 0; 
	ptr->scope = 137; //Temp Var Marker... 
	
	return ptr;
}

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

//Prioritasie Local over Global
int existsInt (char const * name)
{
	if(!existsIntL(name)) {
		return existsIntG(name);
	} else {
		return existsIntL(name);
	}
}

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


/*
struct symInt *putIntParam (char const *name, int isArray, int val)
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
	ptr->scope = 1337; 
	
	ptr->next = (struct symInt *)symIntTable;
	symIntTable = ptr;
	printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->var);
	return ptr;
}

struct symInt *getIntParam (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == 1337) ) {
			return ptr;
		}
	}
	return NULL;
}*/
/*
int setScope (char const * name)
{
	struct symInt *ptr = getInt (name);
	if(ptr!=NULL)
	{
		ptr->scope = currFunc;
		return 1;
	} 
	else
	{
		return 0;
	}
}*/

void setIntScopeP (struct symInt *sInt)
{
	sInt->scope = currFunc;
}

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
	
	//ptr->next = (struct symFunc *)symFuncTable;
	//symFuncTable = ptr;
	
	putFuncIntoTable(ptr);
	
	printf("Function ->%s<- was put in the table pc:%d.\n", ptr->name, ptr->paramCount);
	
	currFunc = ptr;
	
	return ptr;
}

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
	printf("added var %s for func %s, parcount:%d.\n", sInt->name, sFunc->name, sFunc->paramCount);
}

void incParamCountP (struct symFunc *sFunc)
{
	sFunc->paramCount = sFunc->paramCount + 1;	
}


void renameFunc (char const *funcname, char const *funcname_new)
{
	if(existsFunc(funcname))
	{
		printf("renaming func %s to %s.\n", funcname, funcname_new);
		struct symFunc *ptr = getFunc(funcname);
		free(ptr->name);
		
		ptr->name = (char *) malloc (strlen (funcname_new) + 1);
		strcpy (ptr->name,funcname_new);		
		printf("function name is now:%s.\n", ptr->name);
	}
}

int isFuncProto (char const *funcname)
{
	if(existsFunc(funcname))
	{
		struct symFunc *ptr = getFunc(funcname);
		return ptr->isPrototype;
	}
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
		printf("param:%s.\n", param0->name);
		temp = param0->next;
		if(temp!=NULL) 
		{
			temp = param0->next;
			if(param0!=NULL) deleteParamInt (param0);
			param0 = temp;
		}
		else
		{
			printf("temp==NULL\n");
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
		printf("\tNo Parameters.\n");
		return;
	}

	for(int i=0;i<sFunc0->paramCount;i++)
	{
		printf("\tParam #%d :%s.\n", (i+1), param0->name);
		
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
			printf("%s was deleted as a function\n", ptr->name);
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

	struct symInt *param0 = sFunc0->params;;
	struct symInt *param1 = sFunc1->params;;

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
		//if(strcmp (ptr->name,"int") != 0)
		//{
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