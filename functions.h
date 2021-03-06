/***** functions.h *****
*
*     Implementace interpretu imperativního jazyka IFJ16
*     Tým 029, varianta b/3/I
*     Autori: Jiruska Adam, Janecek David
*     Login: xjirus01, xjanec28
*     
*/


#ifndef FUNCTIONS_DECLARED
#define FUNCTIONS_DECLARED
#include "ial.h"


int ifj16readInt ();
double ifj16readDouble ();
char *ifj16readString ();

int ifj16length(char *s);
int ifj16compare(char * s1, char * s2);
int ifj16find(char * s, char * search);

char *ifj16substr(char * s, int i, int n);

char *ifj16sort(char *);

void hlPrint(int, int[10]);
void ifj16print(char *);

#endif