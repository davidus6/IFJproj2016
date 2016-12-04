#include "prec.h"

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
    	case TD_SEMICOLON:
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
		case T_ID: case T_QUALID:
			item->dataType = 4;
			//uvidime co bude tady
			//mozna kontrola definice a prip ukazatel do TS
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
	int i = 0;
	pStackItem *pptr;
	item.type = OPER_END;
	pStackInit(&stack);
	pStackPush(&stack, &item);
	token b = getToken();
	printf("prvni token: %d\n", b.type);
	do{
	printf("\nZACATEK CYKLU\n");
		item.dataType = 0;
		a = pStackTermTop(&stack);
		printf("vrchol zasobniku: %d\n", a->type);
		char priority = getPriority(*a, b);
		printf("priorita: %c\n", priority);
		switch (priority) {
			case '=':
			printf("==================================================\n");
				item.type = getTableIndex(b);
				pStackPush(&stack, &item);
				b = getToken();
			printf("==================================================\n");
				break;
			case '<':
			printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
				pptr = pStackTop(&stack);
				if (pptr->type == NONTERM_E){
					printf("na vrcholu je neterminal (%d)\n", pptr->type);
					pStackItem item2;
					item2.type = pptr->type;
					item2.dataType = pptr->dataType;
					item2.data = pptr->data; ///nevim jestli funguje
					pStackPop(&stack);
					item.type = NONTERM_LBR;
					pStackPush(&stack, &item);
					printf("chci vlozit neterminal %d\n", item2.type);
					pStackPush(&stack, &item2);
				} else {
					printf("na vrcholu je terminal (%d)\n", pptr->type);
					item.type = NONTERM_LBR;
					pStackPush(&stack, &item);
				}
				item.type = getTableIndex(b);
				if (item.type == 12){
					insertData(b, &item);
					printf("vkladam data\n");
				}
				pStackPush(&stack, &item);
				b = getToken();
				printf("nacten token %d\n", b.type);
			printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
				break;
			case '>': ;
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
				pStackItem *term = pStackTermTop(&stack);
				pStackItem *stackTop;
				switch (term->type){
					case OPER_I: 
					printf("provadim pravidlo E -> i\n");	//pravidlo E -> i
						//tady muze krome identifikatoru bejt jeste konstanta---nevyreseny
						stackTop = pStackTop(&stack);
						item.dataType = stackTop->dataType;
						item.data = stackTop->data; ////neni jisty jestli funguje
						pStackPop(&stack);
						pStackPop(&stack);
						item.type = NONTERM_E;
						pStackPush(&stack, &item);
					printf("konec pravidla E -> i\n");
						break;
					case OPER_RB:
					printf("provadim pravidlo E -> (E)\n"); 	//pravidlo E -> (E)
						pStackPop(&stack);
						stackTop = pStackTop(&stack);
						item.type = stackTop->type;
						item.dataType = stackTop->dataType;
						item.data = stackTop->data;	////neni jisty jestli funguje
						pStackPop(&stack);
						pStackPop(&stack);
						pStackPop(&stack);
						pStackPush(&stack, &item);
						break;
					case OPER_ADD: case OPER_SUB: case OPER_MUL: case OPER_DIV: case OPER_LT:
					case OPER_GT: case OPER_LET: case OPER_GET: case OPER_EQ: case OPER_NEQ: //pravidlo E op E
						printf("provadim pravidlo E -> E op E\n");
						pStackItem *op2 = pStackTop(&stack);
						pStackPop(&stack);
						pStackItem *operator = pStackTop(&stack);
						pStackPop(&stack);
						pStackItem *op1 = pStackTop(&stack);
						pStackPop(&stack);
						pStackPop(&stack);
						//tady se provede semantika vyrazu a potom se to priradi do item
						(void) op1;
						(void) op2;
						(void) operator;
						item.type = NONTERM_E;
						pStackPush(&stack, &item);
						break;
					default:
						printf("PSA SE POSRALA\n");
						break;
				}
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
				break;
			default:
				printf("PSA SE POSRALA\n");
				return SYNTAX_ERROR;
				break;
		}
		a = pStackTermTop(&stack);
		pStackItem *bla = pStackTop(&stack);
		printf("na zasobniku je terminal %d , neterminal %d, a token je %d\n",a->type, bla->type, b.type);
		i++;
	} while ((a->type != OPER_END) || (getTableIndex(b) != OPER_END));
	printf("PSA HLASI USPECH!!!\n");
	getToken();
	return 0;
}