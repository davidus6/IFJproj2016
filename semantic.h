/***** semantic.h *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "ial.h"

void stAddClass(nodeClassPtr ,char *);

void stAddStatic(nodeInnerPtr *, nodeTypes, char *, void *);

void stAddVar (nodeLocalPtr , char *, dataVar);


/**** po konci bloku ****/
void stEndFunc(nodeLocalPtr *);
void stEndProg(nodeClassPtr *);

/*
funkce: prirazeni
		definice??
		konverze v zadani
		KONEC funkce check 
		KONEC programu check
*/

int doubleToInt(double);
double intToDouble(int);

#endif