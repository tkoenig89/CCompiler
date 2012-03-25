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
	ptr->params.paramCount = 0; 
	
	ptr->params.params = NULL;
	
	ptr->next = (struct symFunc *)symFuncTable;
	symFuncTable = ptr;
	printf("Function ->%s<- was put in the table\n", ptr->name);
	
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
	return 0;
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
		struct symFunc *ptr = getFunc(funcname);
		free(ptr->name);
		
		ptr->name = (char *) malloc (strlen (funcname_new) + 1);
		strcpy (ptr->name,funcname_new);		
	}
}
/*
void setType (char const *name, int retType)
{
	if(existsFunc(name))
	{
		struct symFunc *ptr = getFunc(name);
		ptr->retType = retType; 
	}
}*/

void setTypeP (struct symFunc *sFunc, int type)
{
	sFunc->retType = type;
}

void funcEnd()
{
	currFunc = NULL;
}