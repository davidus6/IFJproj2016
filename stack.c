#include "stack.h"

void initStack(tStack *S){

	S->top = NULL;
	S->size = 0;
}

void pushStack (tStack *S, frameData *F) {

	struct frame *pptr = S->top; //pomocny ukazatel
    S->top = malloc(sizeof(struct frame));
    S->top->data = F;
    S->top->prev = pptr;
    S->size++;
}

void popStack (tStack *S) {
 
	if (S->top != NULL){ //kontrola prazdnosti seznamu
		frame *ptr = S->top->prev; //pomocny ukazatel
		free(S->top);
		S->top = ptr;
		S->size--;
	}
}	

bool emptyStack (tStack *S) {

	if (S->size == 0){
		return true;
	} else {
		return false;
	}
}

