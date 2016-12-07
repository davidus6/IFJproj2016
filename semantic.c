/***** semantic.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#include "semantic.h"
#include "error_codes.h"

//contextClass = NULL;

void initGlobalTable()
{
	initClassTree(&globalTable);
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
	nodeVarPtr throwAway;
	if (searchVar((contextFunc)->localTable, key, &throwAway) == 1)
		return SEM_ERROR_UND;
	insertVar(&(contextFunc)->localTable, key, type, &localIndex);
	localIndex++;
	return OK;
}
int stAddLocalVar(char *key, dataTypes type)
{
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


	/*DODELAT Byly všechny návěští, které obsahuje tabulka symbolů,
	nalezeny v těle funkce? (Nebyl na neexistující návěští
	vytvořen pouze skok?*/
	/*Konec programu:
• Sémantické kontroly:
• Byly všechny deklarované funkce i definované?
• Byla nalezena funkce main a obsahovala správné parametry?
• Dále provést:
• Vyprázdnit tabulku symbolů globální úrovně + nagenerovat
instrukce, které dealokují datový blok globálních proměnných
*/


int doubleToInt(double convert)
{
	return (int)convert;
}

double intToDouble(int convert)
{
	return (double)convert;
}