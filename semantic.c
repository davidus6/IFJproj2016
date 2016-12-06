/***** semantic.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#include "semantic.h"

void stAddClass(nodeClassPtr *globalTable, char *key)
{		// DODELAT sracka s ifj16 - jesto to chce promyslet
	if (searchClass(*globalTable, key, NULL) == 1)
		;//OSETRIT ERROR
	else
	{
		insertClass(globalTable, key);
		searchClass(*globalTable, key, contextClass);	//do contextClass ulozi ukazatel na aktualni tridu
	}
}

void stAddStaticVar(char *key, dataTypes type)
{
	insertVar(&(contextClass)->innerVar, key, type);
}

void stAddFunc(char *key, dataTypes type)
{
	insertFunc(&(contextClass)->innerFunc, key, type);
	searchFunc(contextClass->innerFunc , key, contextFunc);			//do contextFunc ulozi ukazatel na aktualni fci
}

void stAddParam(char *key, dataTypes type)
{
	insertVar(&(contextFunc)->localTable, key, type);
}

/* blokove akce */
void stEndFunc(nodeVarPtr *root)
{
	disposeVarTree(root);
}

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