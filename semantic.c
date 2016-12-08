/***** semantic.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#include "semantic.h"
#include "error_codes.h"
#include <string.h>

//contextClass = NULL;

void initGlobalTable()
{
	initClassTree(&globalTable);
	tempIndex = 25;
}

int stAddClass(char *key)	//odstranit gT
{		// DODELAT sracka s ifj16 - jesto to chce promyslet
	nodeClassPtr throwAway;
	if (searchClass(globalTable, key, &throwAway) == 1)	//nechci aby mi to prepsalo kontex nebo neco
		return SEM_ERROR_UND;
	else
	{
		insertClass(&globalTable, key);
		searchClass(globalTable, key, &contextClass);	//do contextClass ulozi ukazatel na aktualni tridu
		return OK;
	}
}

int stAddStaticVar(char *key, dataTypes type)
{
	if (type == DATA_VOID)
		return SYNTAX_ERROR;
	nodeVarPtr throwAway;
	if (searchVar((contextClass)->innerVar, key, &throwAway) == 1)
		return SEM_ERROR_UND;
	insertVar(&(contextClass)->innerVar, key, type, &globalIndex);
	globalIndex++;
	return OK;
}

int stAddFunc(char *key, dataTypes type)
{
	nodeFuncPtr throwAway;
	if (searchFunc((contextClass)->innerFunc, key, &throwAway) == 1)
		return SEM_ERROR_UND;
	stEndFunc();
	insertFunc(&((contextClass)->innerFunc), key, type);
	searchFunc((contextClass)->innerFunc , key, &contextFunc);		//do contextFunc ulozi ukazatel na aktualni fci
	return OK;
}

int stAddParam(char *key, dataTypes type)
{
	if (type == DATA_VOID)
		return SYNTAX_ERROR;
	nodeVarPtr throwAway;
	if (searchVar((contextFunc)->localTable, key, &throwAway) == 1)
		return SEM_ERROR_UND;
	insertVar(&(contextFunc)->localTable, key, type, &localIndex);
	printf("stAddParam: CHECKPOINT 1\n");
	if (type == DATA_INT)
		contextFunc->parameters[localIndex] = 'i';
	else if (type == DATA_DOUBLE)
		contextFunc->parameters[localIndex] = 'd';
	else //(type == DATA_STRING)
		contextFunc->parameters[localIndex] = 's';
	printf("stAddParam: CHECKPOINT 2\n");
	printf("stAddParam: CHECKPOINT 3\n");
	localIndex++;
	return OK;
}
int stAddLocalVar(char *key, dataTypes type)
{
	if (type == DATA_VOID)
		return SYNTAX_ERROR;
	nodeVarPtr throwAway;
	if (searchVar((contextFunc)->localTable, key, &throwAway) == 1)
		return SEM_ERROR_UND;
	insertVar(&(contextFunc)->localTable, key, type, &localIndex);
	localIndex++;
	return OK;
}

//idType == 1 -> je to qualid, == 0 -> je to normal id
int stAssignment(char *key1, int idType, char *key2)
{
	if (idType == 1)
	{
		return OK;//printf("DODELAT\n");
	}
	else
	{
		nodeVarPtr testVarPtr1;
		nodeVarPtr testVarPtr2;
		int check = searchVar((contextClass)->innerVar, key1, &testVarPtr1);
		if (check == 0)
		{
			return SEM_ERROR_UND;
		}
		else
		{
			check = searchVar((contextClass)->innerVar, key1, &testVarPtr2);
			if (check == 0)
			{
				return SEM_ERROR_UND;
			}
			else
			{
				if (testVarPtr1->type == testVarPtr2->type)
					return OK;
				else
					return SEM_ERROR_TYPE;
			}
		}
	}
}

void retIndexType(char * varName, int *index, dataTypes *type)
{
	nodeVarPtr found;
	searchVar(contextClass->innerVar, varName, &found);
	*index = (found)->index;
	*type = (found)->type;
}

/* blokove akce */
/* konec fce */
void stEndFunc()	//nodeVarPtr *localTable pokud bude potreba mazat VarTree
{
	contextFunc = NULL;
	localIndex = 0;
	//disposeVarTree(localTable);
}

/* konec programu */
void stEndProg()
{
	disposeClassTree(&globalTable);
}


void generateVar(string *var)
// generuje jedinecne nazvy identifikatoru
// nazev se sklada ze znaku $txP a rady cisel
{
	counterVar ++;
	strClear(var);
	strAddChar(var, '$');
	strAddChar(var, 't');
	strAddChar(var, 'x');
	strAddChar(var, 'P');
	int i;
	i = counterVar;
	while (i != 0)
	{
		strAddChar(var, (char)(i % 10 + '0'));
		i = i / 10;
	}
} 


int doubleToInt(double convert)
{
	return (int)convert;
}

double intToDouble(int convert)
{
	return (double)convert;
}


/* semanticke funkce pro precedencni analyzu */
int precConst(char *class, char *function, dataTypes type, char **ret)	
//prvni 2 param je nazev tridy a fce
//do ret potrebuji poslat vygenerovane id
{
	int checkMalloc, checkFound = 0;
	string name;
	checkMalloc = strInit(&name);
    if (checkMalloc == STR_ERROR)
    {
        strFree(&name);
        return INTER_ERROR;
    }
    generateVar(&name);
    nodeClassPtr clNode;
    nodeFuncPtr fuNode;
    checkFound = searchClass(globalTable, class, &clNode);
    if (checkFound == 0)
    	return SEM_ERROR_UND;
    if (function != NULL)
    {
    	checkFound = searchFunc(clNode->innerFunc, function, &fuNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	insertVar(&(fuNode)->localTable, name.str, type, &tempIndex);
    	tempIndex++;
    	*ret = name.str;
    	return OK;

    }
    else
    {
    	insertVar(&(clNode)->innerVar, name.str, type, &globalIndex);
    	globalIndex++;
    	*ret = name.str;
    	return OK;
    }
}

//kontrola definice ID
int precVar(char *class, char *function, char *variable, int idType)
{
	if (idType == 1)
	{
		variable = divideQualid(variable, 1);
	}
	int checkFound;
	nodeClassPtr clNode;
    nodeFuncPtr fuNode;
    nodeVarPtr vaNode;
    //printf("v precVar: jsem tady\n");
	if (function == NULL)
	{
		checkFound = searchClass(globalTable, class, &clNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchVar(clNode->innerVar, variable, &vaNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	return OK;
	}
	else
	{
		checkFound = searchClass(globalTable, class, &clNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchFunc(clNode->innerFunc, function, &fuNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchVar(fuNode->localTable, variable, &vaNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	return OK;
	}
}

int precOper(char *class, char *function, opType operation, char *ident1, char *ident2, char **retName)
{
	int checkFound;
	nodeClassPtr clNode;
    nodeFuncPtr fuNode;
    nodeVarPtr vaNode1;
    nodeVarPtr vaNode2;
    dataTypes retType;
	if (function == NULL)
	{
		checkFound = searchClass(globalTable, class, &clNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	    	checkFound = searchVar(clNode->innerVar, ident1, &vaNode1);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchVar(clNode->innerVar, ident2, &vaNode2);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
	}
	else
	{
		checkFound = searchClass(globalTable, class, &clNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchFunc(clNode->innerFunc, function, &fuNode);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchVar(fuNode->localTable, ident1, &vaNode1);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
    	checkFound = searchVar(fuNode->localTable, ident2, &vaNode2);
    	if (checkFound == 0)
    		return SEM_ERROR_UND;
	}
	if (vaNode1->type == DATA_BOOL || vaNode2->type == DATA_BOOL)
    	return SYNTAX_ERROR;
    switch (operation)
    {
    	case OPER_SUB: case OPER_MUL:
    		if (vaNode1->type == DATA_INT && vaNode2->type == DATA_INT)
    			retType = DATA_INT;
    		else if (vaNode1->type == DATA_DOUBLE || vaNode2->type == DATA_DOUBLE)
    			retType = DATA_DOUBLE;
    		else
    			return SEM_ERROR_TYPE;
    	break;

    	case OPER_ADD:
    		if (vaNode1->type == DATA_INT && vaNode2->type == DATA_INT)
    			retType = DATA_INT;
    		else if (vaNode1->type == DATA_STRING || vaNode2->type == DATA_STRING)
    			retType = DATA_STRING;
    		else if (vaNode1->type == DATA_DOUBLE || vaNode2->type == DATA_DOUBLE)
    			retType = DATA_DOUBLE;
    		else
    			return SEM_ERROR_TYPE;
    	break;

    	case OPER_DIV:	//DODELAT nema byt deleni stejne jako - a ??
    	if (vaNode1->type == DATA_INT && vaNode2->type == DATA_INT)
    		retType = DATA_INT;
    	else if (vaNode1->type == DATA_DOUBLE && vaNode2->type == DATA_DOUBLE)
    		retType = DATA_DOUBLE;
    	else
    		return SEM_ERROR_TYPE;
    	break;

    	default:
    	if (vaNode1->type == DATA_INT || vaNode1->type == DATA_DOUBLE)
    		if (vaNode2->type == DATA_INT || vaNode2->type == DATA_DOUBLE)
    			retType = DATA_BOOL;
    		else
    			return SEM_ERROR_TYPE;
    	else
    	return SEM_ERROR_TYPE;
    }
    precConst(class, function, retType, retName);
    return 0;
	
}
