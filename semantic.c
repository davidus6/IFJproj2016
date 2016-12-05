#include "semantic.h"

void stAddClass(nodeClassPtr globalTable, char *key)
{		// DODELAT sracka s ifj16 - jesto to chce promyslet
	if (searchClass(globalTable, key) == 1)
		exit (3);	//DODELAT osetrit, ukoncit 
	else
	{
		insertClass(&globalTable, key);
	}
}

void stAddStatic (nodeInnerPtr *root, nodeTypes type, char *key, void *data)
{
	insertInner(root, type, key, data);
}

void stAddVar (nodeLocalPtr root, char *key, dataVar data)
{
	insertLocal(&root, key, data);
}

void stEndFunc(nodeLocalPtr *root)
{
	/*DODELAT Byly všechny návěští, které obsahuje tabulka symbolů,
	nalezeny v těle funkce? (Nebyl na neexistující návěští
	vytvořen pouze skok?*/
	disposeLocalTree(root);
}

void stEndProg(nodeClassPtr *globalTable)
{
	/*Konec programu:
• Sémantické kontroly:
• Byly všechny deklarované funkce i definované?
• Byla nalezena funkce main a obsahovala správné parametry?
• Dále provést:
• Vyprázdnit tabulku symbolů globální úrovně + nagenerovat
instrukce, které dealokují datový blok globálních proměnných
*/
	disposeClassTree(globalTable);
	*globalTable = NULL;
}

int doubleToInt(double convert)
{
	return (int)convert;
}

double intToDouble(int convert)
{
	return (double)convert;
}