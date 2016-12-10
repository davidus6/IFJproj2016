#include "parser.h"
#include <stdio.h>
#include <string.h>

//#define DEBUG
#ifdef DEBUG
#define debug_print printf
#else 
#define debug_print(...) ((void)0)
#endif

void generateInstruction(int insType, dataTypes dataType, void *id, int op1, int op2, int res){
	tInstr I;
	I.insType = insType;
	I.dataType = dataType;
	I.ptr = (void*)id;
	I.op1 = op1;
	I.op2 = op2;
	I.res = res;
	addInstruction(&instructionList, I);
}

int runParser(){
	mode = EASY;
	int ret = prog();
	if (ret != OK){
		debug_print("BYLO VRACENO %d (0 je OK, jinak je chyba)\n", ret);
		return ret;
	}
	debug_print("BYLO VRACENO %d (0 je OK, jinak je chyba)\n", ret);
	mode = TRYHARD;
	ret = prog();
	debug_print("BYLO VRACENO %d (0 je OK, jinak je chyba)\n", ret);
	return ret;
}

int prog(){ 	//<prog> -> CLASS <class> <classes>
	int ret;
	debug_print("<prog>\n\n");
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	if (tok.type == TK_CLASS){
		ret = class();
		if (ret != OK){
			return ret;
		}
		else {
			ret = classes();
			if (ret != OK){
				return ret;
			} 
			debug_print("</prog>\n");
			return OK;
		}
	}
	return SYNTAX_ERROR;
}

int classes(){	//<class> -> CLASS <class> <classes>
	int ret;
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	if (tok.type == TK_CLASS){
		ret = class();
		if (ret != OK){
			return ret;
		}
		else {

			ret = classes();
			if (ret != OK){
				return ret;
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	token tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
	currentClass = tok.attribute.str;
	if (tok.type == T_ID && tok2.type == TD_LC_BRACKET){
		if (mode == EASY){
			debug_print("   <class ID = %s>\n", tok.attribute.str);
			ret = stAddClass(tok.attribute.str);
			printf("currentClass nastaveno na %s\n", currentClass);
			if (ret != OK){
				return ret;
			}
			ret = def_list();
			debug_print("   </class ID = %s>\n\n", tok.attribute.str);
			return ret;
		} else {
			debug_print("   <class ID = %s>\n", tok.attribute.str);
			ret = def_list();
			debug_print("   </class ID = %s>\n\n", tok.attribute.str);
			return ret;
		}
	}
	return 0;
}

int def_list(){	//<def_list> -> STATIC <definition> <def_list>
	int ret;
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;} //ocekavano STATIC
	if (tok.type == TK_STATIC){
		ret = definition(1);
		if (ret != OK){
			return ret;
		}
		else {
			ret = def_list();
			if (ret != OK){
				return ret;
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	if (tok.type == TK_INT || tok.type == TK_DOUBLE || tok.type == TK_STRING || tok.type == TK_VOID){
		int dataType = data_type(tok);
		token id = getToken(); if (id.type == T_ERROR){return id.attribute.inumber;}	//ocekavan ID
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;} //ocekavano ; = (
	switch(tok.type){
		case TD_SEMICOLON:
		//zavolat semantiku pro definici promenne
			if (mode == EASY){
				if (context){
					debug_print("         <static variable ID = %s dataType = %d>\n", id, dataType);
					int ret = stAddStaticVar(id, dataType);
					if (ret != OK){
						return ret;
					}
					int index;
					dataTypes nenipotreba;
					retIndexType(id, &index, &nenipotreba);
					generateInstruction(I_BLOCKC, dataType, NULL, 1, index,0);
				} else {
					debug_print("         <local variable ID = %s dataType = %d>\n", id, dataType);
					int ret = stAddLocalVar(id, dataType);
					if (ret != OK){
						return ret;
					}
					//VOLANI FUNKCE PRO PRIDANI LOK. PROMENNE
				}
				return OK;
			} else {
				//tezko rict co bude tady
				if (context){
					debug_print("         <static variable ID = %s dataType = %d>\n", id, dataType);
				} else {
					debug_print("         <local variable ID = %s dataType = %d>\n", id, dataType);
				}
				return OK;
			}
			break;

		case TO_ASSIGN: ;
			int ret = runPrecedence();
			//debug_print("PSA = %d\n", ret);
			if (ret == OK){
				token tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
				if (tok2.type == TD_SEMICOLON){
					if (mode == EASY){	
						if (context){
							debug_print("         <static variable ID = %s dataType = %d + initialization>\n", id, dataType);
							ret = stAddStaticVar(id, dataType);
							if (ret != OK){
								return ret;
							}
							int index;
							dataTypes nenipotreba;
							retIndexType(id, &index, &nenipotreba);
							generateInstruction(I_BLOCKC, dataType, NULL, 1, index,0);

						} else {
							debug_print("         <local variable ID = %s dataType = %d + initialization>\n", id, dataType);
							ret = stAddLocalVar(id, dataType);
							//VRACI KTEREJ INDEX? POTREBUJU ZJISTIT LOKALNI INDEX PROMENNE
							//generateInstruction(I_ASSIGN, );
						}
						return ret;
					} else {
						if (context){
							debug_print("         <static variable ID = %s dataType = %d + initialization>\n", id, dataType);
							int index;
							dataTypes nenipotreba;
							retIndexType(id, &index, &nenipotreba);
							printf("generuju static I_ASSIGN z indexu %d na index %d\n", (globalIndex-1), index);
							generateInstruction(I_ASSIGN, nenipotreba, NULL, 3, globalIndex-1,index);
							//VOLANI FUNKCE PRO KONTROLU PRIRAZENI
						} else {
							debug_print("         <local variable ID = %s dataType = %d + initialization>\n", id, dataType);
							printf("generuju local I_ASSIGN z indexu %d na index %d\n", (tempIndex-1), (localIndex-1));
							generateInstruction(I_ASSIGN, dataType, NULL, 2, (tempIndex-1), (localIndex-1));
							generateInstruction(I_WRITE, dataType, NULL, 0,(localIndex-1),0);
							//VOLANI FUNKCE PRO KONTROLU PRIRAZENI
						}
						return OK;
					}
				} 
			}
			return ret;
			break;

		case TD_L_BRACKET:
		currentFunction = id;
			if (mode == EASY){
				debug_print("         <static function ID = %s returnType = %d>\n", id, dataType);
				int ret = stAddFunc(id, dataType);
				if (ret != OK){
					return ret;
				}
				ret = param_list();
				if (ret == OK){
					ret = body(1);
				}
				currentFunction = NULL;
				return ret;
			} else {
				debug_print("         <static function ID = %s returnType = %d>\n", id, dataType);
				generateInstruction(I_LABEL,0,0,0,0,0);
				insertInstruct(currentClass, id, instructionList.Last);
				//POTREBA ZJISTIT POSLEDNI INSTRUKCI Z LISTU --instructionList.Last
				//VOLANI FUNKCE KTERA PRIRADI DO TS UKAZATEL NA PASKU FUNKCE
				int ret = param_list();
				if (ret == OK){
					ret = body(1);
				}
				currentFunction = NULL;
				return ret;
			}
			break;

		default:
			return SYNTAX_ERROR;
			break;
	}
	return SYNTAX_ERROR;
}

int param_list(){  //<param_list> -> <datovy typ> ID <param_rest>
	int ret;
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	switch (tok.type){
		case TD_R_BRACKET:
			return OK;
			break;

		case TK_INT: case TK_DOUBLE: case TK_STRING: ;//je to datovy typ
			int dataType = data_type(tok);
			token tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
			if (tok2.type == T_ID){
				printf("            <param ID = %s dataType = %d>\n", tok2.attribute.str, dataType);
				stAddParam(tok2.attribute.str, dataType);
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	switch(tok.type){
		case TD_COMMA:
			tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
			if (tok.type == TK_INT || tok.type == TK_DOUBLE || tok.type == TK_STRING){
				int dataType = data_type(tok);
				(void) dataType;
				token tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
				if (tok2.type == T_ID){
					debug_print("            <param ID = %s dataType = %d>\n", tok2.attribute.str, dataType);
					stAddParam(tok2.attribute.str, dataType);
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	if (tok.type == TD_LC_BRACKET){
		ret = stat_list(def);
		if (ret != OK){
			return ret;
		}
		tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
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
	
	if (ret != OK){
		return ret;
	} else {
		tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
		switch(tok.type){
			case T_ID: case T_QUALID: ;
				ret = stat_rest(tok);
				return ret;
				break;

			case TD_LC_BRACKET: //FUNGUJE
				debug_print("            <block>\n");
				int ret;
				ret = stat_list(0);
				tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
				if (tok.type == TD_RC_BRACKET){
					debug_print("            </block>\n");
					return ret;
				}
				return SYNTAX_ERROR;
				break;

			case TK_IF:
				tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
				if (tok.type == TD_L_BRACKET){
					ungetToken(tok);
					int ret = runPrecedence();
					//printf("precedencka vratila %d\n", ret);
					if (ret == OK){
						ret = body(0);
						if(ret == OK){
							tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
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
				tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
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

			case TK_RETURN: ; //FUNGUJE
				token tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
				if (tok2.type == TD_SEMICOLON){
					printf("               <return>\n");

///////////////////NEJSOU ARGUMENTY///////////////////////////
					if (mode == TRYHARD){
						generateInstruction(I_RETURN,0,0,0,0,0);
					}
///////////////////NEJSOU ARGUMENTY///////////////////////////						

					return OK;
				} else {
					ungetToken(tok2);
					ret = runPrecedence();
					if (ret == OK){
						tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
						if (tok2.type == TD_SEMICOLON){
							printf("               <return>\n");

///////////////////NEJSOU ARGUMENTY///////////////////////////
					if (mode == TRYHARD){
						generateInstruction(I_RETURN,0,0,0,0,0);
					}
///////////////////NEJSOU ARGUMENTY///////////////////////////

							return OK;
						}
					}
				}
				return SYNTAX_ERROR;
				break;

			case TK_INT: case TK_DOUBLE: case TK_STRING: ; //FUNGUJE
				if (def){
					int dataType = data_type(tok);
					token tokID = getToken(); if (tokID.type == T_ERROR){return tokID.attribute.inumber;}
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

int stat_rest(token id){	//<stat_rest> -> = <expression> ;
							//<stat_rest> -> = id (<arguments);
							//<stat_rest> -> (<arguments>) ;
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	switch(tok.type){
		case TO_ASSIGN: ;
			tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
			
			if (tok.type == T_ID || tok.type == T_QUALID){
				token tok2 = getToken(); if (tok2.type == T_ERROR){return tok2.attribute.inumber;}
				
				if (tok2.type == TD_L_BRACKET){
					debug_print("                  <function call ID = %s + assign to id %s>\n", tok.attribute.str, id.attribute.str);
							

						////potreba doresit lok/glob u promenny do ktery se to prirazuje

							if (tok.type == T_ID){ //je to lokalni volani? --0
								/*tListItem *funcStart;
								char *qualID;
								qualID = makeQualid(currentClass, currentFunction);
								returnInstruct(qualID, &funcStart);
								generateInstruction(I_CALL, 0, (void*)funcStart, 0,0,0);
								//funCall(id.attribute.str, 0, tok.attribute.str);*/
							} else { // nebo pres qualid? --1
								/*tListItem *funcStart;
								returnInstruct(tok.attribute.str, &funcStart);
								generateInstruction(I_CALL, 0, (void*)funcStart, 0,0,0);
								//funCall(id.attribute.str, 1, tok.attribute.str);*/
							}
					int ret = arguments();
					if(ret == OK){
						token semi = getToken(); if (semi.type == T_ERROR){return semi.attribute.inumber;}
						if (semi.type == TD_SEMICOLON){
							return OK;
						}
					}
					return SYNTAX_ERROR;
				} else {
					ungetToken(tok2);
					ungetToken(tok);
				}
			} else {
				ungetToken(tok);
			}

			int prec = runPrecedence();
			if (prec == OK){
				tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
				if (tok.type == TD_SEMICOLON){
					printf("                  <assign ID = %s>\n", id.attribute.str);
					if(mode == TRYHARD){
						int index;
						dataTypes nenipotreba;
						retIndexType(id.attribute.str, &index, &nenipotreba);
						if(currentFunction == NULL){
							generateInstruction(I_ASSIGN, nenipotreba, NULL, 3, globalIndex, index);
						} else {
							generateInstruction(I_ASSIGN, nenipotreba, NULL, 2, tempIndex, index);
						}
						
					}
					return OK;
				}
			}
			return SYNTAX_ERROR;
			break;

		case TD_L_BRACKET: ;
			debug_print("                  <function call ID = %s>\n", id.attribute.str);
					if (id.type == T_ID){ //je to lokalni volani? -- 0
						if(mode == TRYHARD){
							//funCall(id.attribute.str, 0, data_void);
							//potreba udelat is qualid
							char *qualid; 
							qualid = makeQualid(currentClass,id.attribute.str);
							generateInstruction(I_FRAMEC,0, qualid,0,0,0);
						}
					} else { // nebo pres qualid? -- 1
						if (mode == TRYHARD){
							//funCall(id.attribute.str, 1, data_void);
							generateInstruction(I_FRAMEC,0,id.attribute.str,0,0,0);
						}
					}
			int ret = arguments();
			if (ret == OK){
				tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
				if (tok.type == TD_SEMICOLON){
					if (mode == TRYHARD){
						if (id.type == T_ID){
							char *qualid; 
							qualid = makeQualid(currentClass,id.attribute.str);
							tListItem *funcStart;
							returnInstruct(qualid, &funcStart);
							generateInstruction(I_CALL, 0, (tListItem*)funcStart, 0,0,0);
						} else {
							tListItem *funcStart;
							returnInstruct(id.attribute.str, &funcStart);
							generateInstruction(I_CALL, 0, (tListItem*)funcStart, 0,0,0);
						}
					}
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	int ret;
	switch(tok.type){
		case TD_R_BRACKET:
			return OK;
			break;

		case TD_COMMA: case TO_ADD:
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
	token tok = getToken(); if (tok.type == T_ERROR){return tok.attribute.inumber;}
	if (mode == EASY){
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
	} else {
		switch(tok.type){
			printf("2. pruchod u argumentu\n");
			int index;
			int ret;
			case T_ID:
			 	debug_print("                  <arg ID = %s>\n", tok.attribute.str);
			 	ret = retGlobIndex(currentClass, currentFunction, tok.attribute.str, &index);
			 	printf("bla index %d\n", index);
			 	if (ret != OK){
			 		return ret;
			 	}
			 	generateInstruction(I_PUSHP, DATA_UNKNOWN, NULL, 0, index,0);
				break;
			case T_QUALID:
				debug_print("                  <arg ID = %s>\n", tok.attribute.str);
				printf("bla index %d\n", index);
				ret = retGlobIndex(NULL, NULL, tok.attribute.str, &index);
				if (ret != OK){
			 		return ret;
			 	}
				generateInstruction(I_PUSHP, DATA_UNKNOWN, NULL, 1, index,0);
				return OK;
				break;
			case T_INT:
				debug_print("                  <arg value = %d>\n", tok.attribute.inumber);
				generateInstruction(I_PUSHP, DATA_INT, NULL, 1, tok.attribute.inumber,0);
				return OK;
				break;

			case T_DOUBLE:
				debug_print("                  <arg value = %f>\n", tok.attribute.dnumber);
				generateInstruction(I_PUSHP, DATA_DOUBLE, NULL, 1, tok.attribute.dnumber,0);
				return OK;
				break;

			case T_STRING:
				debug_print("                  <arg value = %s>\n", tok.attribute.str);
				generateInstruction(I_PUSHP, DATA_STRING, (char*)tok.attribute.str, 1, 0,0);

				return OK;
				break;

			default:
				return SYNTAX_ERROR;
				break;
		}
	}
	return SYNTAX_ERROR;
}