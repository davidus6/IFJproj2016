#ifndef IAL_DEFINED
#define IAL_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

typedef struct node
{
  char *keyName;
  struct node *left;
  struct node *right;
} *nodePtr;


void initTree(nodePtr *);
void insert(nodePtr *, char *);
int search(nodePtr, char *);
void disposeTree(nodePtr *);

#endif