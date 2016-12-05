#ifndef IAL_DEFINED
#define IAL_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

typedef enum
{
	NODE_VAR,
	NODE_FUNC
}nodeTypes;

typedef enum
{
	DATA_INT,
	DATA_DOUBLE,
	DATA_STRING,
	DATA_VOID		//fce nemusi nic vracet
}dataTypes;

//data o funkci
typedef struct dataFunc
{
	//bool defined; //byla fce definovana, asi neni potreba, kdyz bude v tab tak je def
	//DODELAT
	struct nodeLocal *local;	//lokalni tabulka symbolu
	dataTypes dataType;
	//parametry
	//ukazatel na pasku
} dataFunc;

//data o promenne
typedef struct dataVar
{
	int index;
	bool defined;
	bool initialised;	//je to potreba?
	dataTypes dataType;
	union
	{
		int iNum;
		double dNum;
		char *string;
	}data;
}dataVar;


/****globalni tabulka ****/
typedef struct nodeClass 		// uzly hlavniho stromu ve kterem budou tridy
{
	char *keyName;
	struct nodeClass *left;
	struct nodeClass *right;
	struct nodeInner *inner;
} *nodeClassPtr;

typedef struct nodeInner 	//uzly stromu ve kterem budou funkce a promenne, kazda trida bude obsahovat takovy strom
{
	nodeTypes nodeType;
	char *keyName;
	struct nodeInner *left;
	struct nodeInner *right;
	void *data;
} *nodeInnerPtr;

//funkce pro praci s tabulkami
void initClassTree(nodeClassPtr *);
void insertClass(nodeClassPtr *, char *);
int searchClass(nodeClassPtr, char *);
void disposeClassTree(nodeClassPtr *);

void initInnerTree(nodeInnerPtr *);
void insertInner(nodeInnerPtr *, nodeTypes, char *, void *);
int searchInner(nodeInnerPtr, char *);
void disposeInnerTree(nodeInnerPtr *);

/****lokalni tabulka ****/
typedef struct nodeLocal
{
	char *keyName;
	struct nodeLocal *left;
	struct nodeLocal *right;
	dataVar data;
} *nodeLocalPtr;

void initLocalTree(nodeLocalPtr *);
void insertLocal(nodeLocalPtr *, char *, dataVar);
int searchLocal(nodeLocalPtr, char *);
void disposeLocalTree(nodeLocalPtr *);

#endif