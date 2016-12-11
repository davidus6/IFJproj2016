#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "instructions.h"
#include "stack.h"
#include "interpret.h"
#include "parser.h"
#include "error_codes.h"
#include "scanner.h"
#include "str.h"

int main(int argc, char *argv[]){

	FILE *f;
	  if (argc != 2)
	  {
	    fprintf(stderr, "Wrong arguments\n");
	    return INTER_ERROR;
	  }
	  if ((f = fopen(argv[1], "r")) == NULL)
	    {
	      fprintf(stderr, "Cannot open the file\n");
	      return INTER_ERROR;
	    }
	    
	setSourceFile(f);
	initGlobalTable();

	int ret = runParser();
	if (ret == OK){
			ret = checkMainRun();
			if (ret == OK){
				ret = runInterpret();
				if (ret == OK){
					return OK;
				}
			}
	}
	
	switch(ret){
		case LEX_ERROR:
			fprintf(stderr, "Lexical error\n");
			return ret;
			break;
		case SYNTAX_ERROR:
			fprintf(stderr, "Syntax error\n");
			return ret;
			break;
		case SEM_ERROR_UND:
			fprintf(stderr, "Semantic error\n");
			return ret;
			break;
		case SEM_ERROR_TYPE:
			fprintf(stderr, "Semantic error\n");
			return ret;
			break;
		case SEM_ERROR_OTH:
			fprintf(stderr, "Semantic error\n");
			return ret;
			break;
		case NUM_ERROR:
			fprintf(stderr, "Input error\n");
			return ret;
			break;
		case UNIT_ERROR:
			fprintf(stderr, "Use of uninitialized value\n");
			return ret;
			break;
		case DIV_ERROR:
			fprintf(stderr, "Division by zero\n");
			return ret;
			break;
		case RUN_ERROR:
			fprintf(stderr, "Runtime error\n");
			return ret;
			break;
		case INTER_ERROR:
			fprintf(stderr, "Internal error\n");
			return ret;
			break;
	}

	destroyList(currentInstrList);
	stEndProg();
	return ret;
}