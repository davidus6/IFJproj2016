#include "instructions.h"

//funkce inicializuje seznam instrukci
void initList(tInstrList *L){
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

//fce prida instrukci na konec seznamu
void addInstruction(tInstrList *L, tInstr I){
	tListItem *Item;
	Item = malloc(sizeof(tListItem));
	Item->Instruction = I;
	Item->nextIns = NULL;
	
	if(L->First == NULL){ 
		L->First = Item;
	} else {
		L->Last->nextIns = Item;
	}
	L->Last = Item;
}

//fce uvolni seznam instrukci
void destroyList(tInstrList *L){
	tListItem *next = L->First; 
		while(next != NULL){ 
			tListItem *del = next; 
			next = next->nextIns; 
			free(del); 
		}
}

//fce aktivuje prvni instrukci
void activateFirst(tInstrList *L){
	L->Act = L->First;
}

//fce posune aktivitu na dalsi instrukci
void nextInstruction(tInstrList *L){
	if (L->Act != NULL)
		L->Act = L->Act->nextIns;
}

//fce vrati aktivni instrukci
tInstr *getInstruction(tInstrList *L){
	return &L->Act->Instruction;
}

//fce nastavi aktivitu na instrukci danou ukazatelem
void goToInstr(tInstrList *L, void *GTWhere){
	L->Act = (tListItem*) GTWhere;
}