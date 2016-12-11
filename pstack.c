/***** pstack.c *****
*
*   Implementace interpretu imperativního jazyka IFJ16
*   Tým 029, varianta b/3/I
*   Autori: Jiruska Adam, Janecek David
*   Login: xjirus01, xjanec28
*   
*/

#include "pstack.h"

void pStackInit(pStack *S){
    if (S == NULL){
        return;
    }
    S->top = NULL;
    S->size = 0;
}

void pStackPush(pStack *S, pStackItem *item){
    struct pStackItem *ptr = S->top;
    S->top = malloc(sizeof(struct pStackItem));
    S->top->data.s = NULL;
    S->top->type = item->type;
    S->top->prev = ptr;
    switch(item->dataType){
        case 0:
            S->top->dataType = 0;
            S->top->data.i = 0;
            break;
        case 1:
            S->top->dataType = 1;
            S->top->data.i = item->data.i;
            break;
        case 2:
            S->top->dataType = 2;
            S->top->data.d = item->data.d;
            break;
        case 3:
            S->top->dataType = 3;
            S->top->data.s = item->data.s;
            break;
        case 4:
            S->top->dataType = 4;
            S->top->data.s = item->data.s;
            break;
        case 5:
            S->top->dataType = 5;
            S->top->data.s = item->data.s;
        default:
            S->top->dataType = 0;
            S->top->data.i = 0;
            return;
    }
}

int pStackPop(pStack *S){
    if (S->size == 1){
        return SYNTAX_ERROR;
    }
    pStackItem *ptr = S->top->prev;
    free(S->top);
    S->top = ptr;
    S->size--;
    return OK;
}

void pStackDestroy(pStack *S){
    free(S->top);
    S->top = NULL;
    S->size = 0;
}

pStackItem pStackTop(pStack *S){
    return *S->top;
}

pStackItem *pStackTermTop(pStack *S){
    if (S->top->type == NONTERM_E){
        return S->top->prev;
    }
    return S->top;
}