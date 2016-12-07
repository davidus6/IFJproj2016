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
int stAddStaticVar (char *, dataTypes);
int stAddFunc(char *, dataTypes);
int stAddParam(char *, dataTypes);
int stAddLocalVar(char *, dataTypes);

int stAssignment(char *, int, char *);	//prirazeni, zatim pocitam s tim ze na obou stranach je promenna
void stCompareTypes();

//jeji index a datovej typ
void retIndexType(char *, int *, dataTypes *);	//volano po pridani static prom. po jeji definici



/**** po konci bloku ****/
void stEndFunc();
void stEndProg();



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