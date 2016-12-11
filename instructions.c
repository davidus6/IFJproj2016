/***** instructions.c *****
*
*   Implementace interpretu imperativního jazyka IFJ16
*   Tým 029, varianta b/3/I
*   Autori: Jiruska Adam, Janecek David
*   Login: xjirus01, xjanec28
*   
*/

#include "instructions.h"

int MaxListLength = 100;
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
    Item->prevIns = L->Last;
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
    if (L->Act != NULL)
        return &L->Act->Instruction;
    return NULL;
}

//fce nastavi aktivitu na instrukci danou ukazatelem
void goToInstr(tInstrList *L, struct listItem *GTWhere){
    L->Act = GTWhere;
}

void print_elements_of_list(tInstrList TL)  {
    
    tInstrList TempList=TL;
    int CurrListLength = 0;
    printf("-----------------");
    while ((TempList.First!=NULL) && (CurrListLength<MaxListLength))    {
        printf("\n \t%d",TempList.First->Instruction.insType);
        if ((TempList.First==TL.Act) && (TL.Act!=NULL))
            printf("\t <= toto je aktivní prvek ");
        TempList.First=TempList.First->nextIns;
        CurrListLength++;
    }
    if (CurrListLength>=MaxListLength)
        printf("\nList exceeded maximum length!");
    printf("\n-----------------\n");
}