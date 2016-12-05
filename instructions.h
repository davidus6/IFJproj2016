#include <malloc.h>

//typy instrukci
typedef enum insType{
	I_ADD,
	I_SUB,
	I_MUL,
	I_DIV,
	I_CMP,
	
	I_READ,
	I_WRITE,
	I_STOP,
	I_LABEL,

	I_CALL,
	I_FRAMEC,
	I_FRAMED,
	I_RETURN,
	
	I_GOTO
} insType;

typedef struct
{
	int insType; //typ instrukce
	void *op1; //operand1
	void *op2; //operand2
	void *res; //vysledek
} tInstr;

typedef struct listItem
{
	tInstr Instruction;
	struct listItem *nextIns;
} tListItem;

typedef struct 
{
	struct listItem *First;  //ukazatel na prvni instrukci
	struct listItem *Last;   //ukazatel na posledni instrukci
	struct listItem *Act; //ukazatel na aktivni instrukci
} tInstrList;

void initList(tInstrList *L);
void addInstruction(tInstrList *L, tInstr I);
void destroyList(tInstrList *L);
void activateFirst(tInstrList *L);
void nextInstruction(tInstrList *L);
tInstr *getInstruction(tInstrList *L);
void goToInstr(tInstrList *L, void *GTWhere);