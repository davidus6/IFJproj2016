/***** ial.h *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#ifndef IAL_DEFINED
#define IAL_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "str.h"
#include "instructions.h"

#define TRUE 1
#define FALSE 0

/**** vestavene funkce ****/
void shellSort(char [],int);

//DODELAT umazat az bude vse pohromade
void errorHandle(int);


/**** tabulka symbolu ****/






/****globalni tabulka ****/
/* strom pro tridy */
typedef struct nodeClass 		// uzly hlavniho stromu ve kterem budou tridy
{
	char *keyName;
	struct nodeClass *left;
	struct nodeClass *right;
	struct nodeFunc *innerFunc;
	struct nodeVar *innerVar;
} *nodeClassPtr;

/* strom pro promenne */
typedef struct nodeVar 	
{
	int index;
	dataTypes type;
	char *keyName;
	struct nodeVar *left;
	struct nodeVar *right;
} *nodeVarPtr;

/* strom pro funkce */
typedef struct nodeFunc	
{
	dataTypes type;
	char parameters[25];
	tInstrList *instructions;
	char *keyName;
	struct nodeVar *localTable;
	struct nodeFunc *left;
	struct nodeFunc *right;
} *nodeFuncPtr;

//funkce pro praci s tabulkami
void initClassTree(nodeClassPtr *);
void insertClass(nodeClassPtr *, char *);
int searchClass(nodeClassPtr, char *, nodeClassPtr *);
void disposeClassTree(nodeClassPtr *);

void initVarTree(nodeVarPtr *);
void insertVar(nodeVarPtr *, char *, dataTypes,int *);
int searchVar(nodeVarPtr, char *, nodeVarPtr *);
void disposeVarTree(nodeVarPtr *);

void initFuncTree(nodeFuncPtr *);
void insertFunc(nodeFuncPtr *, char *, dataTypes);
int searchFunc(nodeFuncPtr, char *, nodeFuncPtr *);
void disposeFuncTree(nodeFuncPtr *);


#endif