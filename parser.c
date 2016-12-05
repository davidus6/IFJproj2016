#include "parser.h"
#include <stdio.h>
#include <string.h>

#define DEBUG
#ifdef DEBUG
#define debug_print printf
#else 
#define debug_print(...) ((void)0)
#endif

int runParser(){
	int ret = prog();
	debug_print("BYLO VRACENO %d (0 je OK, jinak je chyba)\n", ret);
	printf("BYLO VRACENO %d (0 je OK, jinak je chyba)\n", ret);
	return ret;
}

int prog(){ 	//<prog> -> CLASS <class> <classes>
	int ret;
	debug_print("<prog>\n\n");
	token tok = getToken();
	if (tok.type == TK_CLASS){
		ret = class();
		if (ret == SYNTAX_ERROR){
			return SYNTAX_ERROR;
		}
		else {
			ret = classes();
			if (ret == SYNTAX_ERROR){
				return SYNTAX_ERROR;
			} 
			debug_print("</prog>\n");
			return OK;
		}
	}
	return SYNTAX_ERROR;
}

int classes(){	//<class> -> CLASS <class> <classes>
	int ret;
	token tok = getToken();
	if (tok.type == TK_CLASS){
		ret = class();
		if (ret == SYNTAX_ERROR){
			return SYNTAX_ERROR;
		}
		else {
			ret = classes();
			if (ret == SYNTAX_ERROR){
				return SYNTAX_ERROR;
			} 
			return OK;
		}
	} else if (tok.type == T_EOF){ // <classes> -> EPS
		return OK;
	}
	return SYNTAX_ERROR;
}

int class(){	//<class> -> ID { <def_list> }
	int ret;
	token tok = getToken();
	token tok2 = getToken();
	if (tok.type == T_ID && tok2.type == TD_LC_BRACKET){
		debug_print("   <class ID = %s>\n", tok.attribute.str);
		//stAddClass(ukazatel na strom, tok.attribute.str);
		ret = def_list();
		debug_print("   </class ID = %s>\n\n", tok.attribute.str);
		return ret;
	}
	return 0;
}

int def_list(){	//<def_list> -> STATIC <definition> <def_list>
	int ret;
	token tok = getToken(); //ocekavano STATIC
	if (tok.type == TK_STATIC){
		ret = definition(1);
		if (ret == SYNTAX_ERROR){
			return SYNTAX_ERROR;
		}
		else {
			ret = def_list();
			if (ret == SYNTAX_ERROR){
				return SYNTAX_ERROR;
			} 
			return OK;
		}

	} else if (tok.type == TD_RC_BRACKET){
		return OK;
	} else {
		return SYNTAX_ERROR;
	}
	return SYNTAX_ERROR;
}

int definition(int context){	//<definition> -> <data_type> ID <definition_rest>
	debug_print("      <definition>\n");
	int ret;
	token tok = getToken();
	if (tok.type == TK_INT || tok.type == TK_DOUBLE || tok.type == TK_STRING || tok.type == TK_VOID){
		int dataType = data_type(tok);
		token id = getToken();	//ocekavan ID
		if (id.type == T_ID){
			ret = definition_rest(id.attribute.str, dataType, context);
			debug_print("      </definition>\n");
			return ret;
		}
	}
	return SYNTAX_ERROR;
}

int data_type(token tok){ 
	//MUZE BEJT I VOID KDYZ JE TO FUNKCE	
	switch(tok.type){
		case TK_INT:
			return 0;
			break;
		case TK_DOUBLE:
			return 1;
			break;
		case TK_STRING:
			return 2; 
			break;
		case TK_VOID:
			return 3;
		default:
			return SYNTAX_ERROR;
			break;
	}
}

int definition_rest(char *id, int dataType, int context){	 	//<definition_rest> -> ;
																//<definition_rest> -> = <expression> ;
																//<definition_rest> -> ( <param_list> ) <body>
	token tok = getToken(); //ocekavano ; = (
	switch(tok.type){
		case TD_SEMICOLON:
		//zavolat semantiku pro definici promenne
			if (context){
				debug_print("         <static variable ID = %s dataType = %d>\n", id, dataType);
			} else {
				debug_print("         <local variable ID = %s dataType = %d>\n", id, dataType);
			}
			return OK;
			break;

		case TO_ASSIGN: ;
			int prec = runPrecedence();
			//debug_print("PSA = %d\n", prec);
			if (prec == OK){
				token tok2 = getToken();
				if (tok2.type == TD_SEMICOLON){
					if (context){
						debug_print("         <static variable ID = %s dataType = %d + initialization>\n", id, dataType);
					} else {
						debug_print("         <local variable ID = %s dataType = %d + initialization>\n", id, dataType);
					}
					return OK;
				} 
			}
			return SYNTAX_ERROR;
			break;

		case TD_L_BRACKET:
		debug_print("         <static function ID = %s returnType = %d>\n", id, dataType);
			int ret = param_list();
			if (ret == OK){
				ret = body(1);
			}
			return ret;
			break;

		default:
			return SYNTAX_ERROR;
			break;
	}
	return SYNTAX_ERROR;
}

int param_list(){  //<param_list> -> <datovy typ> ID <param_rest>
	int ret;
	token tok = getToken();
	switch (tok.type){
		case TD_R_BRACKET:
			return OK;
			break;

		case TK_INT: case TK_DOUBLE: case TK_STRING: ;//je to datovy typ
			int dataType = data_type(tok);
			token tok2 = getToken();
			if (tok2.type == T_ID){
				printf("            <param ID = %s dataType = %d>\n", tok2.attribute.str, dataType);
				ret = param_rest();
				return ret;
			}
			return SYNTAX_ERROR;
			break;

		default:
			return SYNTAX_ERROR;
			break;
	}
}

int param_rest(){	//<param_rest> -> , <datovy typ> ID <param_rest>
	int ret;
	token tok = getToken();
	switch(tok.type){
		case TD_COMMA:
			tok = getToken();
			if (tok.type == TK_INT || tok.type == TK_DOUBLE || tok.type == TK_STRING){
				int dataType = data_type(tok);
				token tok2 = getToken();
				if (tok2.type == T_ID){
					debug_print("            <param ID = %s dataType = %d>\n", tok2.attribute.str, dataType);
					ret = param_rest();
					return ret;
				}
			}
			return SYNTAX_ERROR;
			break;

		case TD_R_BRACKET:
			return OK;
			break;

		default:
			return SYNTAX_ERROR;
			break;
	}
}

int body(int def){ 	//<body> -> { <stat_list> }
	debug_print("            <body>\n");
	int ret;
	token tok = getToken();
	if (tok.type == TD_LC_BRACKET){
		ret = stat_list(def);
		tok = getToken();
		if (tok.type == TD_RC_BRACKET){
			debug_print("            </body>\n");
			return ret;
		}
	}
	return SYNTAX_ERROR;
}

int stat_list(int def){	//<stat_list> -> <stat> <stat_list>
	int ret = stat(def);
	token tok;
	
	if (ret == SYNTAX_ERROR){
		return SYNTAX_ERROR;
	} else {
		tok = getToken();
		if (tok.type == TD_RC_BRACKET){
			ungetToken(tok);
			return OK;
		} else {
			ungetToken(tok);
			ret = stat_list(def);
			return ret;
		}
	}
	return SYNTAX_ERROR;
}

int stat(int def){	//OK<stat> -> <data_type> ID <definition_rest> ; --definice lokalni funkce (s inicializaci)
					//OK<stat> -> <id> <stat_rest> --prirazeni/volani funkce
					//OK<stat> -> { <stat_list> } --slozeny prikaz
					//<stat> -> IF ( <expression> ) <body> ELSE <body> --if
					//<stat> -> WHILE ( <expression> ) <body> --while
					//OK<stat> -> RETURN <expression> ; --return
	int ret;
	token tok = getToken();
		switch(tok.type){
			case T_ID: case T_QUALID: ;
				ret = stat_rest(tok.attribute.str);
				return ret;
				break;

			case TD_LC_BRACKET: //FUNGUJE
				debug_print("            <block>\n");
				int ret;
				ret = stat_list(0);
				tok = getToken();
				if (tok.type == TD_RC_BRACKET){
					debug_print("            </block>\n");
					return ret;
				}
				return SYNTAX_ERROR;
				break;

			case TK_IF:
				tok = getToken();
				if (tok.type == TD_L_BRACKET){
					ungetToken(tok);
					int ret = runPrecedence();
					if (ret == OK){
						ret = body(0);
						if(ret == OK){
							tok = getToken();
							if (tok.type == TK_ELSE){
								ret = body(0);
								if (ret == OK){
									debug_print("                  <if statement>\n");
									return OK;
								}
							}
						}
					}
				}
				return SYNTAX_ERROR;
				break;

			case TK_WHILE:
				tok = getToken();
				if (tok.type == TD_L_BRACKET){
					ungetToken(tok);
					int ret = runPrecedence();
					if (ret == OK){
						ret = body(0);
						if (ret == OK){
							debug_print("                  <while statement>\n");
							return OK;
						}
					}
				}	
				return SYNTAX_ERROR;	
				break;

			case TK_RETURN: //FUNGUJE
				ret = runPrecedence();
				if (ret == OK){
					token tok2 = getToken();
					if (tok2.type == TD_SEMICOLON){
						printf("               <return>\n");
						return OK;
					}
				}
				return SYNTAX_ERROR;
				break;

			case TK_INT: case TK_DOUBLE: case TK_STRING: ; //FUNGUJE
				if (def){
					printf("def je %d\n", def);
					int dataType = data_type(tok);
					token tokID = getToken();
					if (tokID.type == T_ID){
						ret = definition_rest(tokID.attribute.str, dataType, 0);
						return ret;
					}
				} else {
					return SYNTAX_ERROR;
				}
				return SYNTAX_ERROR;
				break;

			case TD_RC_BRACKET: // <stat> -> EPS FUNGUJE
				ungetToken(tok);
				return OK;
				break;

			default: //FUNGUJE
				return SYNTAX_ERROR;
				break;
		}
	return SYNTAX_ERROR;
}

int stat_rest(char *id){	//<stat_rest> -> = <expression> ;
							//<stat_rest> -> (<arguments>) ;
	token tok = getToken();
	switch(tok.type){
		case TO_ASSIGN: ;
			int prec = runPrecedence();
			if (prec == OK){
				tok = getToken();
				if (tok.type == TD_SEMICOLON){
					printf("                  <assign ID = %s>\n", id);
					return OK;
				}
			}
			return SYNTAX_ERROR;
			break;

		case TD_L_BRACKET: ;
			int ret = arguments();
			if (ret == OK){
				tok = getToken();
				if (tok.type == TD_SEMICOLON){
					debug_print("                  <function call ID = %s>\n", id);
					return OK;
				}
			}
			return ret;
			break;
		default:
			return SYNTAX_ERROR;
			break;
	}
}

int arguments(){
	token tok = getToken();
	int ret;
	if (tok.type == TD_R_BRACKET){
		return OK;
	} else {
		ungetToken(tok);
		ret = arg();
		if (ret == OK){
			ret = arguments_rest();
			return ret;
		}
	}
	return SYNTAX_ERROR;
}

int arguments_rest(){
	token tok = getToken();
	int ret;
	switch(tok.type){
		case TD_R_BRACKET:
			return OK;
			break;

		case TD_COMMA:
			ret = arg();
			if (ret == OK){
				ret = arguments_rest();
				return ret;
			}
			return SYNTAX_ERROR;
			break;

		default:
			return SYNTAX_ERROR;
			break;
	}
}

int arg(){
	token tok = getToken();
	switch(tok.type){
		case T_ID: case T_QUALID:
			debug_print("                  <arg ID = %s>\n", tok.attribute.str);
			return OK;
			break;

		case T_INT:
			debug_print("                  <arg value = %d>\n", tok.attribute.inumber);
			return OK;
			break;

		case T_DOUBLE:
			debug_print("                  <arg value = %f>\n", tok.attribute.dnumber);
			return OK;
			break;

		case T_STRING:
			debug_print("                  <arg value = %s>\n", tok.attribute.str);
			return OK;
			break;

		default:
			return SYNTAX_ERROR;
			break;
	}
}