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