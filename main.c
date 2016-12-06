/***** main.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#include "ial.h"
#include "semantic.h"
#include <stdio.h>
#include "error_codes.h"

void errorHandle(int value)
{
	exit(value);
}

void printFuncTree(nodeFuncPtr *);
void printVarTree(nodeVarPtr *);

void printClassTree(nodeClassPtr *root)	//nejdrive tiskne veci ve tride, pak tridu
{
	if (*root != NULL)
	{
		printFuncTree(&(*root)->innerFunc);
		printVarTree(&(*root)->innerVar);
		printf("**************Class: %s\n", (*root)->keyName);
		printClassTree(&(*root)->left);
		printClassTree(&(*root)->right);
	}
}

void printFuncTree(nodeFuncPtr *root)
{
	if (*root != NULL)
	{
		printVarTree(&(*root)->localTable);
		printf("**************Func: %s\n", (*root)->keyName);
		printFuncTree(&(*root)->left);
		printFuncTree(&(*root)->right);
	}
}

void printVarTree(nodeVarPtr *root)
{
	if(*root != NULL)
	{
		printf("**************Var: %s\n", (*root)->keyName);
		printVarTree(&(*root)->left);
		printVarTree(&(*root)->right);
	}
}

int main()
{
	/* pretypovani, asi na hovno


	double convert = 42.89;
	int converted;
	converted = doubleToInt(convert);
	int Dconvert = 24;
	double Dconverted;
	Dconverted = intToDouble (Dconvert);
	printf ("%d\n", converted);
	printf ("%f\n", Dconverted);	*/

	//shellSort("dickinson", 9);

	/* testovaci staticke promenne */
	/*int i = 0;
	dataVar testVariableInt;
	testVariableInt.dataType = DATA_INT;
	testVariableInt.index = ++i;
	testVariableInt.defined = true;
	testVariableInt.initialised = true;
	testVariableInt.data.iNum = 42;
	dataVar testVariableDouble;
	testVariableDouble.dataType = DATA_DOUBLE;
	testVariableDouble.index = ++i;
	testVariableDouble.defined = true;
	testVariableDouble.initialised = true;
	testVariableDouble.data.dNum = 42e1;
	dataVar testVariableString;
	testVariableString.dataType = DATA_STRING;
	testVariableString.index = ++i;
	testVariableString.defined = true;
	testVariableString.initialised = true;
	testVariableString.data.string = "fml";*/
	/* testovaci staticke promenne */

	//"globalni" promenne
	//globalTable = NULL;	//NENI POTREBA
	//contextClass = NULL;
	//contextFunc = NULL;
	//testClass = NULL;

	initGlobalTable();

	printf ("---vytisknu si prazdny strom---\n");
	printClassTree (&globalTable);
	/* vlozim tridu */

	stAddClass("trida shit");
	printf("\n---contextClass---\n");
	printf("contextClass key :%s\n\n", (contextClass)->keyName);

	printf ("---vytisknu si strom---\n\n");
	printClassTree(&globalTable);

	int found = searchClass(globalTable, "trida shit", &testClass);
	printf ("---nasel jsem? -- %d -- melo by byt 1---\n", found);
	found = searchClass(globalTable, "trida shit", &testClass);
	printf ("---nasel jsem? -- %d -- melo by byt 1---\n", found);
	found = searchClass (globalTable, "damn", &testClass);
	printf ("---nasel jsem? -- %d -- melo by byt 0---\n", found);
	printf("v mainu: -----------------------------checkpoint0\n");

	printf("v mainu: -----------------------------checkpoint1\n");
	stAddClass("trida totalshit");
	printf("v mainu: -----------------------------checkpoint2\n");

	printf ("---vytisknu si strom---\n");
	printClassTree(&globalTable);

	printf("\n---contextClass---\n");
	printf("contextClass key :%s\n\n", (contextClass)->keyName);

	/* vlozim static promenne do dane tridy */
	/*insertInner(&(globalTable->inner), NODE_VAR, "testVariableInt", &testVariableInt);
	insertInner(&(globalTable->inner), NODE_VAR, "testVariableDouble", &testVariableDouble);
	insertInner(&(globalTable->inner), NODE_VAR, "testVariableString", &testVariableString);*/
	printf("v mainu: -----------------------------checkpoint4\n");
	printf("pridam funkci int a parametr int\n");
	stAddFunc("funkce int", DATA_INT);
	printf("\n---contextFunc---\n");
	printf("contextFunc key :%s\n\n", (contextFunc)->keyName);
	printf("v mainu: -----------------------------checkpoint5\n");

	printf ("---vytisknu si strom---\n");
	printClassTree(&globalTable);

	
	stAddFunc("funkce void", DATA_VOID);
	printf("v mainu: -----------------------------checkpoint5.1\n");
	printf("\n---contextFunc---\n");
	printf("contextFunc key :%s\n\n", (contextFunc)->keyName);
	printf("v mainu: -----------------------------checkpoint6\n");

	stAddParam("param int", DATA_INT);
	printf("v mainu: -----------------------------checkpoint7\n");

	
	printf ("---vytisknu si strom---\n");
	printClassTree(&globalTable);

	stEndProg (&globalTable);
	printf ("---vytisknu si prazdny strom---\n");
	printClassTree(&globalTable);
	return 0;
}