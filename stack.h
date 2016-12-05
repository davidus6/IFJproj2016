#include <malloc.h>
#include <stdbool.h>

typedef struct frameData{
//informace o funkci
	int bla;
} frameData;

typedef struct frame{
	frameData *data;
	struct frame *prev;
} frame;

typedef struct {
	struct frame *top;
	int size;
} tStack;

void initStack(tStack *S);
void pushStack(tStack *S, frameData *F);
void popStack(tStack *S);
bool emptyStack(tStack *S);