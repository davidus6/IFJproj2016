#include <malloc.h>

//stack pro precedencku
typedef enum 
{
	OPER_ADD, 	// +
	OPER_SUB,	// -
	OPER_MUL,	// *
	OPER_DIV,	// /
	OPER_LB,	// (
	OPER_RB,	// )
	OPER_LT,	// <
	OPER_GT,	// >
	OPER_LET,	// <=
	OPER_GET,	// >=
	OPER_EQ,	// ==
	OPER_NEQ,	// !=
	OPER_I,		// i
	OPER_END,	// $

	NONTERM_E,	// E
	NONTERM_LBR	// <
} opType;

typedef struct pStackItem{
	opType type;
	int dataType;
	union {
		int i;
		double d;
		char *s;
		void *p;
	} data;
	struct pStackItem *prev;
} pStackItem;

typedef struct {
	struct pStackItem *top;
	int size;
} pStack;

void pStackInit(pStack *S);
void pStackPush(pStack *S, pStackItem *item);
void pStackPop(pStack *S);
pStackItem *pStackTop (pStack *S); //vraci prvek na vrcholu 
pStackItem *pStackTermTop (pStack *S); //vraci terminal nejbliz vrcholu