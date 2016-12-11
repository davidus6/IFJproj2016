/***** stack.h *****
*
*   Implementace interpretu imperativního jazyka IFJ16
*   Tým 029, varianta b/3/I
*   Autori: Jiruska Adam, Janecek David
*   Login: xjirus01, xjanec28
*   
*/

#ifndef STACK_DEF
#define STACK_DEF

#include <malloc.h>
#include <stdbool.h>
#include "instructions.h"

#define LOC_DATASIZE 50

typedef struct frameData{
    void *returnWhere;
    tInstrList *returnFunc;
    tListItem *returnAddr;
    void *localData[LOC_DATASIZE];
} frameData;

typedef struct frame{
    frameData *data;
    struct frame *prev;
} frame;

typedef struct tStack{
    struct frame *top;
    int size;
} tStack;

void initStack(tStack *S);
void pushStack(tStack *S, frameData *F);
void popStack(tStack *S);
frameData *topStack (tStack *S);
bool emptyStack(tStack *S);

#endif