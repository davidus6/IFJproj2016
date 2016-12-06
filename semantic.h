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

void stAddClass(nodeClassPtr * ,char *);
void stAddStaticVar (char *, dataTypes);
void stAddFunc(char *, dataTypes);
void stAddParam(char *, dataTypes);

/**** po konci bloku ****/
void stEndFunc(nodeVarPtr *);
void stEndProg(nodeClassPtr *);



int doubleToInt(double);
double intToDouble(int);

//kontextove promenne
nodeClassPtr contextClass;
nodeFuncPtr contextFunc;


#endif