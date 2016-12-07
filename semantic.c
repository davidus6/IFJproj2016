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
	if (searchClass(globalTable, key, &testClass) == 1)	//testClass pac nechci aby mi to prepsalo kontex nebo neco
		return (SEM_ERROR_UND);
	else
	{
		insertClass(&globalTable, key);
		searchClass(globalTable, key, &contextClass);	//do contextClass ulozi ukazatel na aktualni tridu
		return OK;
	}
}

void stAddStaticVar(char *key, dataTypes type)
{
	insertVar(&(contextClass)->innerVar, key, type, &globalIndex);
	globalIndex++;
}

void stAddFunc(char *key, dataTypes type)
{
	stEndFunc();
	insertFunc(&((contextClass)->innerFunc), key, type);
	searchFunc((contextClass)->innerFunc , key, &contextFunc);		//do contextFunc ulozi ukazatel na aktualni fci
}

void stAddParam(char *key, dataTypes type)
{
	insertVar(&(contextFunc)->localTable, key, type, &localIndex);
	localIndex++;
}
void stAddLocalVar(char *key, dataTypes type)
{
	insertVar(&(contextFunc)->localTable, key, type, &localIndex);
	localIndex++;
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
void stEndProg(nodeClassPtr *root)
{
	disposeClassTree(root);
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