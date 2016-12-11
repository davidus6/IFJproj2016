/***** prec.h *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Jiruska Adam, Janecek David
*	Login: xjirus01, xjanec28
*	
*/

#ifndef PREC_DEF
#define PREC_DEF

#include "pstack.h"
#include "scanner.h"
#include "error_codes.h"
#include "ial.h"
#include "parser.h"

int runPrecedence();

char *currentClass;
char *currentFunction;

#endif