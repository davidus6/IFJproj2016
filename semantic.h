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


#define LOC_LIMIT 25

void initGlobalTable();
int stAddClass(char *);
int stAddStaticVar (char *, dataTypes);
int stAddFunc(char *, dataTypes);
int stAddParam(char *, dataTypes);
int stAddLocalVar(char *, dataTypes);

int insertInstruct(char *, char *, tInstrList *);	//trida, fce, instrukcni paska
int returnInstruct(char *, tInstrList **);	//vzdy qualid fce, instrukcni paska na ulozeni

/* semanticke funkce pro precedencni analyzu */
int precConst(char *, char *, dataTypes, char **, int *);	//prvni 2 param je nazev tridy a fce 
int precVar(char *, char *, char *, int);	//trida, fce, promenna, 1=qualif-0=id
int precOper(char *, char *, opType, char *, char *, char **, int *, int *, int *, int *);	
//prvni 2 param je kontext, typ operace, ucastnici, vysledek
//Var s vysledkem musim vytvorit a vratit jeho jmeno a index

int checkMainRun();

int stAssignment(char *, int, char *);	//prirazeni, zatim pocitam s tim ze na obou stranach je promenna


//jeji index a datovej typ
void retIndexType(char *, int *, dataTypes *);	//volano po pridani static prom. po jeji definici
int retITfields(char *, int *, dataTypes *);	//prvni char MUSI byt qualid - je to id funkce
//pole indexu, pole datovych type, vracim pocet
void fillLocal(nodeVarPtr *, int *, dataTypes *, int *); //pomocna fce pro retITfields

//trida, fce, prom nebo NULL NULL qualid, index
int retGlobIndex(char *, char *, char *, int *, int *);

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