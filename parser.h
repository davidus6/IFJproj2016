/***** parser.h *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Jiruska Adam, Janecek David
*	Login: xjirus01, xjanec28
*	
*/

#ifndef PARSER_DEF
#define PARSER_DEF

#include "scanner.h"
#include "prec.h"
#include "error_codes.h"
#include "semantic.h"
#include "ial.h"
#include "instructions.h"

#define TRYHARD 1
#define EASY 0

tInstrList globalInstrList;
tInstrList *currentInstrList;
int printArgs;

int mode;
void generateInstruction(int insType, dataTypes dataType, void*, int op1, int op2, int res);
int runParser();
int prog();
int classes();
int class();
int definition(int context);
int def_list();
int data_type(token tok);
int definition_rest(char *id, int dataType, int context); //context- 1=global 0=local
int param_list();
int param_rest();
int body(int def);
int stat_list(int def);
int stat(int def); //def- 1=povoleno 0=zakaz
int stat_rest(token id);
int arguments();
int arguments_rest();
int arg();

#endif