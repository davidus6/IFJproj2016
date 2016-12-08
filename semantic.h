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
#include "str.h"
#include "scanner.h"
#include "pstack.h"

void initGlobalTable();
int stAddClass(char *);
int stAddStaticVar (char *, dataTypes);
int stAddFunc(char *, dataTypes);
int stAddParam(char *, dataTypes);
int stAddLocalVar(char *, dataTypes);

/* semanticke funkce pro precedencni analyzu */
int precConst(char *, char *, dataTypes, char **);	//prvni 2 param je nazev tridy a fce //predelat dataTypes na int
int precVar(char *, char *, char *, int);	//trida, fce, promenna, 1=qualif-0=id
int precOper(char *, char *, opType, char *, char *, char **);	//prvni 2 param je kontext, typ operace, ucastnici, vysledek
//Var s vysledkem musim vytvorit a vratit jeho jmeno


int stAssignment(char *, int, char *);	//prirazeni, zatim pocitam s tim ze na obou stranach je promenna


//jeji index a datovej typ
void retIndexType(char *, int *, dataTypes *);	//volano po pridani static prom. po jeji definici
void retITfields();



/**** po konci bloku ****/
void stEndFunc();
void stEndProg();



int doubleToInt(double);
double intToDouble(int);

/* generuje unikatni ID */
void generateVar(string *);

/* globalni promenne */

//tabulka symbolu
nodeClassPtr globalTable;

//index promenne
int globalIndex;
int localIndex;
int tempIndex;

//kontextove promenne
nodeClassPtr contextClass;
nodeFuncPtr contextFunc;
nodeClassPtr testClass;

//pouziva se u generateVar
int counterVar;



#endif