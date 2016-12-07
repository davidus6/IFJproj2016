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

void initGlobalTable();
int stAddClass(char *);
void stAddStaticVar (char *, dataTypes);
void stAddFunc(char *, dataTypes);
void stAddParam(char *, dataTypes);
void stAddLocalVar(char *, dataTypes);

//jeji index a datovej typ
void retIndexType(char *, int *, dataTypes *);	//volano po pridani static prom. po jeji definici

/**** po konci bloku ****/
void stEndFunc();
void stEndProg(nodeClassPtr *);



int doubleToInt(double);
double intToDouble(int);

/* globalni promenne */

//tabulka symbolu
nodeClassPtr globalTable;

//index promenne
int globalIndex;
int localIndex;

//kontextove promenne
nodeClassPtr contextClass;
nodeFuncPtr contextFunc;
nodeClassPtr testClass;


#endif