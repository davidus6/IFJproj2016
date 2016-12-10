#include <stdio.h>
#include "instructions.h"
#include "stack.h"
#include "interpret.h"
#include "error_codes.h"
#include "semantic.h"

#define DEBUG
#ifdef DEBUG
#define debug_print printf
#else 
#define debug_print(...) ((void)0)
#endif

#define GLOB_DATASIZE 100

void *globalData[GLOB_DATASIZE];
char *contClass;
frameData prepFrame;
frameData lastFrame;
int paramIndex;

void dealocateGlobData(){
		for (int i = 0; i < GLOB_DATASIZE; i++){
			if (globalData[i] != NULL){
				free(globalData[i]);
			}
		}
	}

int runInterpret(tInstrList *instrList){
	contClass = "Main";
	activateFirst(instrList);
	tListItem *progStart;
	returnInstruct("Main.run", &progStart);
	goToInstr(instrList, progStart);
	tInstr *I;	
	tStack frameStack;
	initStack(&frameStack);

	while (1){
		I = getInstruction(instrList);
		printf("cyklim-typ instrukce je %d\n", I->insType);
		switch (I->insType){

		case I_STOP:
		printf("interpret: I_STOP\n");
			//MOZNA DEALOKOVAT GLOBAL/LOKAL DATA
			dealocateGlobData();
			printf("KONEEEEEEEEEEEEEEEEEEEC\n");
		//konec programu
			return 0;
			break;

		case I_READ:
		printf("interpret: I_READ\n");
			switch(I->dataType){
					case DATA_INT:
						scanf("%d", (int*)globalData[I->res]);
						break;
					case DATA_DOUBLE:
						scanf("%lf", (double*)globalData[I->res]);
						break;
					case DATA_STRING:
						scanf("%s", (char*)globalData[I->res]);
						break;
					default:
						break;
			}
			break;
		//nacteni hodnoty ze stdin
			break;

		case I_WRITE:
		printf("interpret: I_WRITE\n");
			if (I->op1 == 1){
				switch(I->dataType){
						case DATA_INT:
							printf("%d\n",*((int*)globalData[I->op2]));
							break;
						case DATA_DOUBLE:
							printf("%f\n",*((double*)globalData[I->op2]));
							break;
						case DATA_STRING:
							printf("%s\n",(char*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else {
				switch(I->dataType){
					case DATA_INT:
						printf("%d\n",*((int*)frameStack.top->data->localData[I->op2]));
						break;
					case DATA_DOUBLE:
						printf("%f\n",*((double*)globalData[I->op2]));
						break;
					case DATA_STRING:
						printf("%s\n",(char*)globalData[I->op2]);
						break;
					default:
						break;
			}
			break;
			}
			break;
		//zapis hodnoty na stdout
			break;

		case I_GOTO:
		printf("interpret: I_GOTO\n");
		//skok v programu
			goToInstr(instrList, I->ptr);
			break;

		case I_LABEL:
		printf("interpret: I_LABEL\n");
		//navesti, nic se neprovadi
			break;

		case I_ADD:
		printf("interpret: I_ADD\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) + *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) + *((double*)globalData[I->op2]);
							break;
						case DATA_STRING:
							//konkatenace retezcu
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) + *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) + *((double*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_STRING:
							//konkatenace retezcu
							break;
						default:
							break;
				}
			}
			break;
		case I_SUB:
			break;

		case I_MUL:
			break;

		case I_DIV:
			break;

		case I_FRAMEC: ;
		printf("interpret: I_FRAMEC\n");
			int localVarIndex[LOC_DATASIZE];
			dataTypes localVarDataType[LOC_DATASIZE];
			int * localVarIndexP = localVarIndex;
			dataTypes *localVarDataTypeP = localVarDataType;

			int count = 0;
			//zavolam funkci ktera mi do tech poli narve indexy a datatypy vsech promennych ve funkci
			count = retITfields((char*)I->ptr, localVarIndexP, localVarDataTypeP);
			for (int i = 0; i < count; i++){
				printf("ramec pro index %d\n", localVarIndex[i]);
				switch(localVarDataType[i]){
					case DATA_INT:
						prepFrame.localData[localVarIndex[i]] = malloc(sizeof(int)); 
						break;
					case DATA_DOUBLE:
						prepFrame.localData[localVarIndex[i]] = malloc(sizeof(double));
						break;
					case DATA_STRING:
						prepFrame.localData[localVarIndex[i]] = malloc(sizeof(char *)*50);
						break;
					default:
						break;
				}
				paramIndex = 0;
			}
			debug_print("vytvoren lokalni ramec s %d promennyma\n", count);
			//pushStack(&frameStack, &prepFrame);
			break;

		case I_PUSHP: //na pozici op2 je ulozeno odkud, na op1 je lok/glob
		printf("interpret: I_PUSHP\n");	
			switch(I->dataType){
				case DATA_UNKNOWN:
					if (I->op1 == 1){ //globalne
						prepFrame.localData[paramIndex] = globalData[I->op2];
					} else { //lokalne
						prepFrame.localData[paramIndex] = frameStack.top->data->localData[I->op2];
					}
					break;
				case DATA_INT:
					*((int*)prepFrame.localData[paramIndex]) = I->op2;
					break;
				case DATA_DOUBLE:
				//nejspis pretypovani druheho operandu na double!!!!!!!!!1
					*((double*)prepFrame.localData[paramIndex]) = I->op2;
					break;
				case DATA_STRING:
					prepFrame.localData[paramIndex] = (char*)I->ptr;
					break;
				default:
					return INTER_ERROR;
			}
			paramIndex++;
			break;

		case I_FRAMED: ;
		printf("interpret: I_FRAMED\n");
		//ulozit si odkladany frame!!
			lastFrame = topStack(&frameStack);
			(void) lastFrame;
			popStack(&frameStack); //UVOLNIT LOK DATOVY BLOK
			break;

		case I_CALL:
		printf("interpret: I_CALL\n");
			prepFrame.returnAddr = malloc(sizeof(tListItem*));
			prepFrame.returnAddr = instrList->Act;
			printf("instrukce pro navrat je %d\n", instrList->Act->nextIns->Instruction.insType);
			pushStack(&frameStack, &prepFrame);
			goToInstr(instrList, I->ptr);
			break;

		case I_RETURN:
		printf("interpret: I_RETURN\n");
			printf("chci se vratit na instrukci %d\n", frameStack.top->data->returnAddr->Instruction.insType);
			goToInstr(instrList, frameStack.top->data->returnAddr);
			break;

		case I_BLOCKC:
		printf("interpret: I_BLOCKC\n");
			if ((I->op1) == 1){ //jde o globalni promennou
				switch(I->dataType){
					case DATA_INT:
						globalData[I->op2] = malloc(sizeof(int));
						//*((int*)globalData[I->op2]) = 10;
						break;
					case DATA_DOUBLE:
						globalData[I->op2] = malloc(sizeof(double));
						break;
					case DATA_STRING:
						globalData[I->op2] = malloc(sizeof(char *));
						break;
					default:
						break;
				}
			//	printf("datatype:%d\n", I->dataType);
			//	*((int*)globalData[I->op2]) = I->res; //TOHLE TAM PAK NEBUDE, JEN NA TESTOVANI
			} else { //jde o lokalni promennou

			}
			break;

		case I_ASSIGN:
		printf("interpret: I_ASSIGN\n");
			if (I->op1 == 1){ //globalne konstanta
				switch(I->dataType){
							case DATA_INT:
								*((int*)globalData[I->res]) = I->op2;
								break;
							case DATA_DOUBLE:
								*((double*)globalData[I->res]) = I->op2;
								break;
							case DATA_STRING:
								*((char*)globalData[I->res]) = *(char*)I->ptr;
								break;
							default:
								break;
						}
					break;
				} else if (I->op1 == 0) { //lokalne konstanta
				switch(I->dataType){
							case DATA_INT:
								(*(int*)frameStack.top->data->localData[I->res]) = I->op2;
								//*((int*)globalData[I->res]) = I->op2;
								break;
							case DATA_DOUBLE:
								(*(double*)frameStack.top->data->localData[I->res]) = I->op2;
								break;
							case DATA_STRING:
								(*(char*)frameStack.top->data->localData[I->res]) = *(char*)I->ptr;
								break;
							default:
								break;
						}
					break;
				} else if (I->op1 == 2){ //lokalne promenna
					switch(I->dataType){
							case DATA_INT:
								(*(int*)frameStack.top->data->localData[I->res]) = (*(int*)frameStack.top->data->localData[I->op2]);
								//*((int*)globalData[I->res]) = I->op2;
								break;
							case DATA_DOUBLE:
								(*(double*)frameStack.top->data->localData[I->res]) = (*(double*)frameStack.top->data->localData[I->op2]);
								break;
							case DATA_STRING:
								//nevim jestli je to spravne
								(*(char*)frameStack.top->data->localData[I->res]) = (*(char*)frameStack.top->data->localData[I->op2]);
								break;
							default:
								break;
						}
				} else if (I->op1 == 3){ //globalne promenna	
					switch(I->dataType){
							case DATA_INT:
								*((int*)globalData[I->res]) = *((int*)globalData[I->op2]);
								break;
							case DATA_DOUBLE:
								*((double*)globalData[I->res]) = *((double*)globalData[I->op2]);
								break;
							case DATA_STRING:
								*((char*)globalData[I->res]) = *((char*)globalData[I->op2]);
								break;
							default:
								break;
						}
				}
			}
		nextInstruction(instrList);
	}
}