#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct symInt 		*symIntTable;
struct symFunc 	*symFuncTable;
struct symFunc 	*currFunc;

void init_table ()
{
	currFunc=NULL;
	printf("Hello World from symbol table.\n");
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
	
	//ptr->next = (struct symInt *)symIntTable;
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
	ptr->scope = NULL; 
	
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
	printf("gf.\n");
	struct symFunc *ptr;
	printf("getFunc %s.\n", name);
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		printf("checking func %s.\n", ptr->name);
		if (strcmp (ptr->name,name) == 0) {
			printf("match!\n");
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
		printf("checking var:%s.\n", ptr->name);
		if (ptr==sInt) {
			if(ptr == symIntTable) {
				symIntTable = ptr->next;
			} else {
				ptr_back->next = ptr->next;
			}
			printf("%s was deleted as a var\n", ptr->name);
			free(ptr);
			free(ptr->name);
		}
		ptr_back = ptr;
	};
}

void deleteParams (struct symFunc *sFunc0)
{
	struct symInt *param0 = sFunc0->params;
	
	if(param0==NULL)
	{
		return;
	}
	
	printf("param count:%d.\n", sFunc0->paramCount);

	for(int i=0;i<sFunc0->paramCount;i++)
	{
		printf("deleting param:%s.\n", param0->name);
		deleteParamInt (param0);
		//free(param0->name);
		//free(param0);
		
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
		if(param0 != param1) 
		{
			return 0;
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
	 if(symIntTable==NULL)
	{
		printf("no variables in table.\n");
		return;
	}
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		printf("Int: %s", ptr->name);
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