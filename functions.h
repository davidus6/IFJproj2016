/***** functions.h *****
*
*   Implementace interpretu imperativního jazyka IFJ16
*   Tým 029, varianta b/3/I
*   Autori: Janecek David, Jiruska Adam
*   
*/
#include "ial.h"

int ifj16readInt ();
double ifj16readDouble ();
char *ifj16readString ();

int ifj16length(char *s);
int ifj16compare(char * s1, char * s2);
int ifj16find(char * s, char * search);

char *ifj16substr(char * s, int i, int n);

char *ifj16sort(char *);
//void ifj16shellSort(char [],int);
void hlPring(int);
void ifj16print(char *);