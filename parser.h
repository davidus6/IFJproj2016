#include "scanner.h"
#include "prec.h"
#include "error_codes.h"
#include "semantic.h"

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
int stat_rest(char *id);
int arguments();
int arguments_rest();
int arg();