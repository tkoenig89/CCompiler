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

struct symInt *putInt (char const *name, int isArray, int val)
{
	struct symInt *ptr;
	//set name
	ptr = (struct symInt *) malloc (sizeof (struct symInt));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	//set value
	ptr->value.var = val; 
	//set isArray
	ptr->isArray = isArray; 
	//set scope; NULL == global
	ptr->scope = currFunc; 
	
	ptr->next = (struct symInt *)symIntTable;
	symIntTable = ptr;
	printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->value.var);
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
	ptr->value.var = val; 
	//set isArray
	ptr->isArray = isArray; 
	//set scope; NULL == global
	ptr->scope = 1337; 
	
	ptr->next = (struct symInt *)symIntTable;
	symIntTable = ptr;
	printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->value.var);
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

void setScopeP (struct symInt *sInt)
{
	sInt->scope = currFunc;
}
/*
void deleteInt (char const * name)
{
	struct symInt *ptr;
	struct symInt *ptr_back;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && ((ptr->scope == currFunc) || (ptr->scope == NULL))) {
			if(ptr == symIntTable) {
				symIntTable = ptr->next;
			} else {
				ptr_back->next = ptr->next;
			}
			printf("%s was deleted as a var\n", ptr->name);
			free(ptr);
		}
		ptr_back = ptr;
	};
}*/

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
	ptr->params.paramCount = 0; 	
	ptr->params.params = NULL;
	ptr->isPrototype = 0;
	
	ptr->next = (struct symFunc *)symFuncTable;
	symFuncTable = ptr;
	printf("Function ->%s<- was put in the table\n", ptr->name);
	
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
	sFunc->params.paramCount += 1;	
	sFunc->params.params = sInt;
}

void incParamCountP (struct symFunc *sFunc)
{
	sFunc->params.paramCount += 1;	
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
			free(ptr->name);
			free(ptr);
			break;
		}
		ptr_back = ptr;
	};
}

int paramFuncCheckP (struct symFunc *sFunc0, struct symFunc *sFunc1)
{
	printf("-----------------SegTest01\n");
	if(sFunc0 == sFunc1) 
	{
		return 1;
	}
	printf("-----------------SegTest02\n");
	if(sFunc0->params.paramCount != sFunc1->params.paramCount)
	{
		return 0;
	}
	printf("-----------------SegTest03\n");
	struct symInt *param0 = sFunc0->params.params;;
	struct symInt *param1 = sFunc1->params.params;;
	printf("-----------------SegTest04\n");
	for(int i=0;i<sFunc0->params.paramCount;i++)
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
	printf("-----------------SegTest05\n");
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
		if(ptr!=NULL) printf("%d Function %d: %s.\n", ptr->retType, count, ptr->name);
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
			printf(" scope: %s.\n", ((struct symFunc *)(ptr->scope))->name);
		}
	}
 }