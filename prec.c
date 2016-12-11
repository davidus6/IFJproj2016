#include "prec.h"

//#define DEBUG
#ifdef DEBUG
#define debug_print printf
#else 
#define debug_print(...) ((void)0)
#endif

static const char precTable [14][14] = 
{
//			 	 +	  -	  *	  /	  (   )	  <	  >	 <=	 >=	 ==	 !=	  i   $	
/*0 + */		{'>','>','<','<','<','>','>','>','>','>','>','>','<','>'},
/*1 - */		{'>','>','<','<','<','>','>','>','>','>','>','>','<','>'},
/*2 * */		{'>','>','>','>','<','>','>','>','>','>','>','>','<','>'},
/*3 / */		{'>','>','>','>','<','>','>','>','>','>','>','>','<','>'},
/*4 ( */		{'<','<','<','<','<','=','<','<','<','<','<','<','<',' '},
/*5 ) */		{'>','>','>','>',' ','>','>','>','>','>','>','>',' ','>'},
/*6 < */		{'<','<','<','<','<','>','>','>','>','>','>','>','<','>'},
/*7 > */		{'<','<','<','<','<','>','>','>','>','>','>','>','<','>'},
/*8 <= */		{'<','<','<','<','<','>','>','>','>','>','>','>','<','>'},
/*9 >= */		{'<','<','<','<','<','>','>','>','>','>','>','>','<','>'},
/*10 == */		{'<','<','<','<','<','>','<','<','<','<','<','<','<','>'},
/*11 != */		{'<','<','<','<','<','>','<','<','<','<','<','<','<','>'},
/*12 i */		{'>','>','>','>',' ','>','>','>','>','>','>','>',' ','>'},
/*13 $ */		{'<','<','<','<','<',' ','<','<','<','<','<','<','<',' '},
};


int getTableIndex(token tok){
	int index;
	switch(tok.type){
		case TO_ADD:
    		index = OPER_ADD;	// +
    		break;
    	case TO_MINUS: 
    		index = OPER_SUB;	// -
			break;
		case TO_MULTIPLY:
			index = OPER_MUL;	// *
			break;   		 	
    	case TO_DIVIDE:
    		index = OPER_DIV;	// /
    		break; 
    	case TD_L_BRACKET:
    		index = OPER_LB;	// (	
    		break;  
    	case TD_R_BRACKET:
    		index = OPER_RB; 	// )
    		break;  	
    	case TO_SMALLER:
    		index = OPER_LT;	// <
    		break;
    	case TO_GREATER:
    		index = OPER_GT;    // >
    		break;
    	case TO_SM_EQUAL:
    		index = OPER_LET;	// <=
    		break;
    	case TO_GR_EQUAL:
    		index = OPER_GET;	// >=
    		break;
    	case TO_EQUAL: 
    		index = OPER_EQ;	// ==
    		break;
    	case TO_NOTEQUAL:
    		index = OPER_NEQ;	// !=
    		break;
    	case T_ID: case T_QUALID: case T_INT: case T_DOUBLE: case T_STRING:
    		index = OPER_I;
    		break;
    	case TD_SEMICOLON: case TD_RC_BRACKET:
    		index = OPER_END;
    		break;
    	default:
    		index = OPER_END;
    		break;
	}
	return index;
}

void insertData(token tok, pStackItem *item){
	switch(tok.type){
		case T_INT:
			item->dataType = 1;
			item->data.i = tok.attribute.inumber;
			break;
		case T_DOUBLE:
			item->dataType = 2; 
			item->data.d = tok.attribute.dnumber;
			break;
		case T_STRING:
			item->dataType = 3;
			item->data.s = tok.attribute.str;
			break;
		case T_ID: 
			item->dataType = 4;
			item->data.s = tok.attribute.str;
			break;
		case T_QUALID:
			item->dataType = 5;
			item->data.s = tok.attribute.str;
			break;
		default:
			item->dataType = 0;
			break;
	}
	return;
}

char getPriority(pStackItem a, token b){
	int i = a.type;
	int j = getTableIndex(b);
	return precTable[i][j];
}

int runPrecedence(){
	pStack stack;
	pStackItem item;
	pStackItem *a;
	pStackItem pptr;
	item.type = OPER_END;
	item.dataType = 0;
	pStackInit(&stack);
	pStackPush(&stack, &item);
	token b = getToken(); if(b.type == T_ERROR){return b.attribute.inumber;}
	debug_print("prvni token: %d\n", b.type);
	do{
	debug_print("\nZACATEK CYKLU\n");
		item.dataType = 0;
		a = pStackTermTop(&stack);
		debug_print("vrchol zasobniku: %d\n", a->type);
		char priority = getPriority(*a, b);
		debug_print("priorita: %c\n", priority);
		switch (priority) {
			case '=':
			debug_print("==================================================\n");
				item.type = getTableIndex(b);
				pStackPush(&stack, &item);
				b = getToken(); if(b.type == T_ERROR){return b.attribute.inumber;}
			debug_print("==================================================\n");
				break;
			case '<':
			debug_print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
				pptr = pStackTop(&stack);
				if (pptr.type == NONTERM_E){
					debug_print("na vrcholu je neterminal (%d)\n", pptr.type);
					pStackItem item2;
					item2.type = pptr.type;
					item2.dataType = pptr.dataType;
					item2.data = pptr.data; ///nevim jestli funguje
					pStackPop(&stack);
					item.type = NONTERM_LBR;
					pStackPush(&stack, &item);
					debug_print("chci vlozit neterminal %d\n", item2.type);
					pStackPush(&stack, &item2);
				} else {
					debug_print("na vrcholu je terminal (%d)\n", pptr.type);
					item.type = NONTERM_LBR;
					pStackPush(&stack, &item);
				}
				item.type = getTableIndex(b);
				if (item.type == 12){
					insertData(b, &item);
					debug_print("vkladam data\n");
				}
				pStackPush(&stack, &item);
				b = getToken(); if(b.type == T_ERROR){return b.attribute.inumber;}
				debug_print("nacten token %d\n", b.type);
			debug_print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
				break;
			case '>': ;
			debug_print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
				pStackItem *term = pStackTermTop(&stack);
				pStackItem stackTop;
				switch (term->type){
					case OPER_I: 
					debug_print("provadim pravidlo ");	//pravidlo E -> i
						//tady muze krome identifikatoru bejt jeste konstanta---nevyreseny
						//debug_print("currentClass = %s\n", currentClass);
						//debug_print("currentFunc = %s\n", currentFunction);
						stackTop = pStackTop(&stack);
						char *name;
						int index;

						if (stackTop.dataType == 1 || stackTop.dataType == 2 || stackTop.dataType == 3){
							debug_print("E->const\n");
							if (mode == TRYHARD){
								dataTypes dt = stackTop.dataType - 1;
								int ret = precConst(currentClass, currentFunction, dt, &name, &index);
								debug_print("E->const index %d\n", index);
								//generovat instrukci co nacte to cislo do pameti na misto pro name
								if (currentFunction == NULL){ //je to globalne
									switch(stackTop.dataType){
										case 1:
											generateInstruction(I_BLOCKC, DATA_INT, NULL, 1, index, 0);
											generateInstruction(I_ASSIGN, DATA_INT, NULL, 1, stackTop.data.i, index);
											break;
										case 2:
											generateInstruction(I_BLOCKC, DATA_DOUBLE, NULL, 1, index, 0);
											generateInstruction(I_ASSIGN, DATA_DOUBLE, NULL, 1, stackTop.data.d, index);
											break;
										case 3:
											generateInstruction(I_BLOCKC, DATA_STRING, NULL, 1, index, 0);
											generateInstruction(I_ASSIGN, DATA_STRING, stackTop.data.s, 1, 0, index);
											break;
										default:
											return SYNTAX_ERROR;
									}
								} else { //je to lokalne
									switch(stackTop.dataType){
										case 1:
											generateInstruction(I_ASSIGN, DATA_INT, NULL, 0, stackTop.data.i, index);
											debug_print("index kam = %d\n", index);
											break;
										case 2:
											generateInstruction(I_ASSIGN, DATA_DOUBLE, NULL, 0, stackTop.data.d, index);
											break;
										case 3:
											generateInstruction(I_ASSIGN, DATA_STRING, stackTop.data.s, 0, 0, index);
											break;
										default:
											return SYNTAX_ERROR;
									}
								}
								if (ret != OK){
									return ret;
								}
								item.dataType = stackTop.dataType;
								item.data.s = name;
							} else {
								item.dataType = stackTop.dataType;
							}

						} else if (stackTop.dataType == 4 || stackTop.dataType == 5){
							debug_print("E->id\n");
							if (mode == TRYHARD){
								int dt = stackTop.dataType - 4;
								int ret = precVar(currentClass, currentFunction, stackTop.data.s, dt);
								debug_print("stacktop data = %s a dt = %d\n", stackTop.data.s, dt);
								debug_print("ret ====== %d\n", ret);
								if (ret != OK){
									return ret;
								}
								item.dataType = stackTop.dataType;
								item.data.s = stackTop.data.s;
							} else {
								item.dataType = stackTop.dataType;
							}
						} else {
							return SYNTAX_ERROR;
						}

						pStackPop(&stack);
						pStackPop(&stack);
						item.type = NONTERM_E;
						pStackPush(&stack, &item);
						break;

					case OPER_RB: //HOTOVO
					debug_print("provadim pravidlo E -> (E)\n"); 	//pravidlo E -> (E)
						pStackPop(&stack);
						stackTop = pStackTop(&stack);
						if (stackTop.type != NONTERM_E){
							return SYNTAX_ERROR;
						}
						item.type = stackTop.type;
						item.dataType = stackTop.dataType;
						item.data.s = stackTop.data.s;	
						pStackPop(&stack);
						pStackPop(&stack);
						pStackPop(&stack);
						pStackPush(&stack, &item);
						break;

					case OPER_ADD: case OPER_SUB: case OPER_MUL: case OPER_DIV: case OPER_LT:
					case OPER_GT: case OPER_LET: case OPER_GET: case OPER_EQ: case OPER_NEQ: ;//pravidlo E op E
						opType optype = term->type;
						debug_print("provadim pravidlo E -> E op E\n");
						pStackItem op2 = pStackTop(&stack);
						pStackPop(&stack);
						pStackItem operator = pStackTop(&stack);
						pStackPop(&stack);
						pStackItem op1 = pStackTop(&stack);
						pStackPop(&stack);
						pStackPop(&stack);
						//tady se provede semantika vyrazu a potom se to priradi do item
						//precOper(typ operace, id z prvniho E, id z druhyho E); //dostanu id vyslednyho E 
						if (mode == TRYHARD){
							char *nameres;
							int indexop1, indexop2, indexres;
							int resDataType;
							precOper(currentClass,currentFunction, operator.type, op1.data.s, op2.data.s, &nameres, &indexop1, &indexop2, &indexres, &resDataType);
							item.dataType = resDataType-1;//DATATYPE SI MUSIM NECHAT POSLAT
							item.data.s = nameres;
							debug_print("index vysledku operace %d a indexy operandu %d a %d\n", indexres, indexop1, indexop2);
							if (currentFunction == NULL){ //je to globalne
								switch (optype){
									case OPER_ADD:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_ADD, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_SUB:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_SUB, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_MUL: 
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_MUL, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_DIV:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_DIV, resDataType, currentFunction, indexop1, indexop2,indexres);
										break; 
									case OPER_LT:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_LT, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_GT:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_GT, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_LET:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_LET, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_GET:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_GET, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_EQ:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_EQ, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_NEQ:
										generateInstruction(I_BLOCKC, resDataType, NULL, 1, indexres, 0);
										generateInstruction(I_NEQ, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									default:
										return SYNTAX_ERROR;
										break;
								}
							} else {
								//generateInstruction(I_BLOCKC, resDataType, NULL, 0, indexres, 0);
								switch (optype){
									case OPER_ADD:
										generateInstruction(I_ADD, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_SUB:
										generateInstruction(I_SUB, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_MUL:
										generateInstruction(I_MUL, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_DIV:
										generateInstruction(I_DIV, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_LT:
										generateInstruction(I_LT, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_GT:
										generateInstruction(I_GT, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_LET:
										generateInstruction(I_LET, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_GET:
										generateInstruction(I_GET, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_EQ:
										generateInstruction(I_EQ, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									case OPER_NEQ:
										generateInstruction(I_NEQ, resDataType, currentFunction, indexop1, indexop2,indexres);
										break;
									default:
										return SYNTAX_ERROR;
										break;
								}
							}
						}
						item.type = NONTERM_E;
						pStackPush(&stack, &item);
						break;
					default:
						debug_print("PSA SE POSRALA\n");
						return SYNTAX_ERROR;
						break;
				}
				debug_print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
				break;
			default:
				debug_print("PSA SE POSRALA\n");
				return SYNTAX_ERROR;
				break;
		}
		a = pStackTermTop(&stack);
		pStackItem bla = pStackTop(&stack);
		(void) bla;
		debug_print("na zasobniku je terminal %d , neterminal %d, a token je %d\n",a->type, bla.type, b.type);
	} while ((a->type != OPER_END) || (getTableIndex(b) != OPER_END));
	ungetToken(b);
	pStackDestroy(&stack);
	debug_print("PSA HLASI USPECH!!!\n");
	return OK;
}