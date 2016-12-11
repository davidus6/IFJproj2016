/***** semantic.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Jiruska Adam, Janecek David
*	Login: xjirus01, xjanec28
*	
*/

#include "semantic.h"
#include "error_codes.h"
#include <string.h>
#include "instructions.h"


void initGlobalTable()
{
	initClassTree(&globalTable);
	tempIndex = LOC_LIMIT;
	stAddClass("ifj16");
	stAddFunc("readInt", DATA_INT);
	stAddFunc("readDouble", DATA_DOUBLE);
	stAddFunc("readString", DATA_STRING);
	stAddFunc("print", DATA_VOID);	//jak budou parametry?
	stAddParam("pp0", DATA_STRING);
	stAddParam("pp1", DATA_STRING);
	stAddParam("pp2", DATA_STRING);
	stAddParam("pp3", DATA_STRING);
	stAddParam("pp4", DATA_STRING);
	stAddParam("pp5", DATA_STRING);
	stAddParam("pp6", DATA_STRING);
	stAddParam("pp7", DATA_STRING);
	stAddParam("pp8", DATA_STRING);
	stAddParam("pp9", DATA_STRING);
	stAddFunc("length", DATA_INT);
	stAddParam("s", DATA_STRING);
	stAddFunc("substr", DATA_STRING);
	stAddParam("s", DATA_STRING);
	stAddParam("i", DATA_INT);
	stAddParam("n", DATA_INT);
	stAddFunc("compare", DATA_INT);
	stAddParam("s1", DATA_STRING);
	stAddParam("s2", DATA_STRING);
	stAddFunc("find", DATA_INT);
	stAddParam("s", DATA_STRING);
	stAddParam("search", DATA_STRING);
	stAddFunc("sort", DATA_STRING);
	stAddParam("s", DATA_STRING);
}

int stAddClass(char *key)
{
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
	if (type == DATA_INT)
		contextFunc->parameters[localIndex] = 'i';
	else if (type == DATA_DOUBLE)
		contextFunc->parameters[localIndex] = 'd';
	else //(type == DATA_STRING)
		contextFunc->parameters[localIndex] = 's';
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

int insertInstruct(char *class, char *func, tInstrList *instructions)
{
	int check;
	nodeClassPtr clNode;
	nodeFuncPtr fuNode;
	check = searchClass(globalTable, class, &clNode);
	if (check == 0)
		return SEM_ERROR_UND;
	check = searchFunc(clNode->innerFunc, func, &fuNode);
	if (check == 0)
		return SEM_ERROR_UND;
	fuNode->instructions = instructions;
	return OK;
}

int returnInstruct(char *qualFunc, tInstrList **instructions)
{
	int check;
	nodeClassPtr clNode;
	nodeFuncPtr fuNode;
	char *class = divideQualid(qualFunc, 0);
	char *func = divideQualid(qualFunc, 1);
	check = searchClass(globalTable, class, &clNode);
	if (check == 0)
		return SEM_ERROR_UND;
	check = searchFunc(clNode->innerFunc, func, &fuNode);
	if (check == 0)
		return SEM_ERROR_UND;
	*instructions = fuNode->instructions;
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
	int ret = searchVar(contextClass->innerVar, varName, &found);
	printf("ret = %d\n", ret);
	*index = (found)->index;
	*type = (found)->type;
}

int retGlobIndex(char *class, char *func, char *var, int *index, int *type)
{
	int check;
	nodeClassPtr clNode;
	nodeFuncPtr fuNode;
	nodeVarPtr vaNode;
	if (class == NULL && func == NULL)
	{
		char *trida = divideQualid(var, 0);
		char *prom = divideQualid(var, 1);
		check = searchClass(globalTable, trida, &clNode);
		if (check == 0)
			return SEM_ERROR_UND;
		check = searchVar(clNode->innerVar, prom, &vaNode);
		if (check == 0)
			return SEM_ERROR_UND;
		*index = vaNode->index;
		*type = vaNode->type;
		return OK;
	}
	else
	{
		check = searchClass(globalTable, class, &clNode);
		if (check == 0)
			return SEM_ERROR_UND;
		check = searchFunc(clNode->innerFunc, func, &fuNode);
		if (check == 0)
			return SEM_ERROR_UND;
		check = searchVar(fuNode->localTable, var, &vaNode);
		if (check == 0)
			return SEM_ERROR_UND;
		*index = vaNode->index;
		*type = vaNode->type;
		return OK;
	}
}

void fillLocal(nodeVarPtr *root, int *poleInt, dataTypes *poleTypes, int *ret)
{
	if (*root != NULL)
	{
		fillLocal(&(*root)->left, poleInt, poleTypes, ret);
		fillLocal(&(*root)->right, poleInt, poleTypes, ret);
		poleInt[*ret] = (*root)->index;
		poleTypes[*ret] = (*root)->type;
		*ret = *ret + 1;
	}
}

int retITfields(char *qualFunkce, int *poleInt, dataTypes *poleTypes)
{
	char *trida = divideQualid(qualFunkce, 0);
	char *funkce = divideQualid(qualFunkce, 1);
	int ret = 0;
	nodeClassPtr clNode;
	nodeFuncPtr fuNode;
	searchClass(globalTable, trida, &clNode);
	searchFunc(clNode->innerFunc, funkce, &fuNode);
	fillLocal(&(fuNode)->localTable, poleInt,  poleTypes, &ret);
	return ret;
	
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
int precConst(char *class, char *function, dataTypes type, char **ret, int *retIndex)	
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
    	*retIndex = tempIndex;
    	tempIndex++;
    	*ret = name.str;
    	
    	return OK;

    }
    else
    {
    	insertVar(&(clNode)->innerVar, name.str, type, &globalIndex);
    	*retIndex = globalIndex;
    	globalIndex++;
    	*ret = name.str;
    	return OK;
    }
}

//kontrola definice ID
int precVar(char *cl, char *func, char *var, int idType)
{
	char *class = cl;
	char *function = func;
	char *variable = var;
	if (idType == 1)
	{
		class = divideQualid(variable, 0);
		variable = divideQualid(variable, 1);
	}
	int checkFound;
	nodeClassPtr clNode;
    nodeFuncPtr fuNode;
    nodeVarPtr vaNode;
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

int precOper(char *class, char *function, opType operation, char *ident1, char *ident2, char **retName, int *indop1, int *indop2, int *retIndex, int *returnType)
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
    *indop1 = vaNode1->index;
    *indop2 = vaNode2->index;
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
    *returnType = (int)retType;
    precConst(class, function, retType, retName, retIndex);
    return 0;
	
}

int checkMainRun()
{
	nodeClassPtr clNode;
	nodeFuncPtr fuNode;
	int check;
	check = searchClass(globalTable, "Main", &clNode);
    if (check == 0)
    	return SEM_ERROR_UND;
    check = searchFunc(clNode->innerFunc, "run", &fuNode);
    if (check == 0)
    	return SEM_ERROR_UND;
    return OK;
}