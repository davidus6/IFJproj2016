#include <stdio.h>
#include <string.h>
#include "instructions.h"
#include "stack.h"
#include "interpret.h"
#include "error_codes.h"
#include "semantic.h"
#include "functions.h"
#include "parser.h"

#define DEBUG
#ifdef DEBUG
#define debug_print printf
#else 
#define debug_print(...) ((void)0)
#endif

#define GLOB_DATASIZE 100

void *globalData[GLOB_DATASIZE];
char *contClass;
frameData *prepFrame;
frameData *lastFrame;
int paramIndex;

void dealocateGlobData(){
		for (int i = 0; i < GLOB_DATASIZE; i++){
			if (globalData[i] != NULL){
				free(globalData[i]);
			}
		}
	}

void frameForRun(tInstrList *instrList){
	printf("interpret: tvorim frame pro funkci RUN\n");
			int localVarIndex[LOC_DATASIZE];
			dataTypes localVarDataType[LOC_DATASIZE];
			int * localVarIndexP = localVarIndex;
			dataTypes *localVarDataTypeP = localVarDataType;
			prepFrame = malloc(sizeof(frameData));
			int count = 0;
			//zavolam funkci ktera mi do tech poli narve indexy a datatypy vsech promennych ve funkci
			count = retITfields("Main.run", localVarIndexP, localVarDataTypeP);
			for (int i = 0; i < count; i++){
				printf("ramec pro index %d\n", localVarIndex[i]);
				switch(localVarDataType[i]){
					case DATA_INT:
						prepFrame->localData[localVarIndex[i]] = malloc(sizeof(int)); 
						break;
					case DATA_DOUBLE:
						prepFrame->localData[localVarIndex[i]] = malloc(sizeof(double));
						break;
					case DATA_STRING:
						prepFrame->localData[localVarIndex[i]] = malloc(sizeof(char *)*50);
						break;
					default:
						break;
				}
				paramIndex = 0;
			}
			tListItem *bla;
			bla = globalInstrList.Last->prevIns;
			printf("---------- navrat z RUN je na %d, za ni je instrukce %d\n", bla->Instruction.insType, bla->nextIns->Instruction.insType);
			prepFrame->returnAddr = bla;
			prepFrame->returnFunc = &globalInstrList;
			debug_print("vytvoren lokalni ramec s %d promennyma\n", count);
			pushStack(&frameStack, prepFrame);
}

int runInterpret(){
	contClass = "Main";
	tInstrList *progStart;
	returnInstruct("Main.run", &progStart);
	currentInstrList = &globalInstrList;
	//printf("interpret: adresa aktualniho listu %p\n", (void*)currentInstrList);
	tInstr *I;	
	initStack(&frameStack);
	activateFirst(currentInstrList);
	while (1){
		I = getInstruction(currentInstrList);
		printf("cyklim-typ instrukce je %d\n", I->insType);
		switch (I->insType){

		case I_RUNRUN:
			currentInstrList = progStart;
			activateFirst(currentInstrList);
			frameForRun(&globalInstrList);
			break;

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
							printf("%s\n", (char*)globalData[I->op2]);
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
						//printf("%f\n",*((double*)globalData[I->op2]));
						printf("%d\n",*((int*)frameStack.top->data->localData[I->op2]));
						break;
					case DATA_STRING:
						//printf("%s\n",(char*)globalData[I->op2]);
						printf("%s\n",((char*)frameStack.top->data->localData[I->op2]));
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
			//goToInstr(currentInstrList, I->ptr);
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
			printf("interpret: I_SUB\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) - *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) - *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) - *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) - *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;

		case I_MUL:
			printf("interpret: I_MUL\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) * *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) * *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) * *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) * *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;	

		case I_DIV:
			printf("interpret: I_DIV\n");
			if (I->ptr == NULL){ //globalne

				if ((*((int*)globalData[I->op2])) == 0){
						return DIV_ERROR;
					}
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:

							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) / *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) / *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				if(*((int*)frameStack.top->data->localData[I->op2]) == 0){
					return DIV_ERROR;
				}

				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) / *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) / *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;

		case I_LT:
			printf("interpret: I_LT\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) < *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) < *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) < *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) < *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;

		case I_GT:
			printf("interpret: I_GT\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) > *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) > *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) > *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) > *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;	

		case I_LET:
			printf("interpret: I_LET\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) <= *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) <= *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) <= *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) <= *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;	

		case I_GET:
			printf("interpret: I_GET\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) >= *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) >= *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) >= *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) >= *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;

		case I_EQ:
			printf("interpret: I_EQ\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) == *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) == *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) == *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) == *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;	

		case I_NEQ:
			printf("interpret: I_NEQ\n");
			if (I->ptr == NULL){ //globalne
				switch(I->dataType){
					debug_print("jedem globalne\n");
						case DATA_INT:
							*((int*)globalData[I->res]) = *((int*)globalData[I->op1]) != *((int*)globalData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)globalData[I->res]) = *((double*)globalData[I->op1]) != *((double*)globalData[I->op2]);
							break;
						default:
							break;
				}
			} else { //lokalne
				switch(I->dataType){
					debug_print("jedem lokalne\n");
						case DATA_INT:
							*((int*)frameStack.top->data->localData[I->res]) = *((int*)frameStack.top->data->localData[I->op1]) != *((int*)frameStack.top->data->localData[I->op2]);
							break;
						case DATA_DOUBLE:
							*((double*)frameStack.top->data->localData[I->res]) = *((double*)frameStack.top->data->localData[I->op1]) != *((double*)frameStack.top->data->localData[I->op2]);
							break;
						default:
							break;
				}
			}
			break;

		case I_FRAMEC: ;
			printf("interpret: I_FRAMEC\n");
			int localVarIndex[LOC_DATASIZE];
			dataTypes localVarDataType[LOC_DATASIZE];
			int * localVarIndexP = localVarIndex;
			dataTypes *localVarDataTypeP = localVarDataType;
			prepFrame = malloc(sizeof(frameData));
			int count = 0;
			//zavolam funkci ktera mi do tech poli narve indexy a datatypy vsech promennych ve funkci
			count = retITfields((char*)I->ptr, localVarIndexP, localVarDataTypeP);
			for (int i = 0; i < count; i++){
				printf("ramec pro index %d\n", localVarIndex[i]);
				switch(localVarDataType[i]){
					case DATA_INT:
						prepFrame->localData[localVarIndex[i]] = malloc(sizeof(int)); 
						break;
					case DATA_DOUBLE:
						prepFrame->localData[localVarIndex[i]] = malloc(sizeof(double));
						break;
					case DATA_STRING:
						prepFrame->localData[localVarIndex[i]] = malloc(sizeof(char *)*50);
						break;
					default:
						break;
				}
				paramIndex = 0;
			}
			debug_print("vytvoren lokalni ramec pro funkci %s s %d promennyma\n", (char*)I->ptr, count);
			//pushStack(&frameStack, &prepFrame);
			break;

		case I_PUSHP: //na pozici op2 je ulozeno odkud, na op1 je lok/glob
			printf("interpret: I_PUSHP\n");	
			printf(">>>>>>>>>>>>>datovy typ je %d\n", I->res);
			switch(I->dataType){
				case DATA_UNKNOWN:
					if (I->op1 == 1){ //globalne
						if (I->res == 0){ //dataType INT
							*(int*)prepFrame->localData[paramIndex] = *((int*)globalData[I->op2]);
							//printf("pushuju %d na index %d\n", globalData[I->op2], paramIndex);
						} else if (I->res == 1){
							*(double*)prepFrame->localData[paramIndex] = *((double*)globalData[I->op2]);
						} else if (I->res == 2){
							prepFrame->localData[paramIndex] = ((char*)globalData[I->op2]);
						}
					} else { //lokalne
						if (I->res == 0){ //dataType INT
							*(int*)prepFrame->localData[paramIndex] = *((int*)frameStack.top->data->localData[I->op2]);
							printf("pushuju %d na index %d\n", *((int*)frameStack.top->data->localData[I->op2]), paramIndex);
						} else if (I->res == 1){
							*(double*)prepFrame->localData[paramIndex] = *((double*)frameStack.top->data->localData[I->op2]);
						} else if (I->res == 2){
							prepFrame->localData[paramIndex] = ((char*)frameStack.top->data->localData[I->op2]);
						}


						*(int*)prepFrame->localData[paramIndex] = *((int*)frameStack.top->data->localData[I->op2]);
						//printf("pushuju %d na index %d\n", *((int*)frameStack.top->data->localData[I->op2]), paramIndex);
						//printf("je tam %s\n", (char*)prepFrame->localData[paramIndex]);
					}
					break;
				case DATA_INT:
					*((int*)prepFrame->localData[paramIndex]) = I->op2;
					printf("pushujuiu %d na index %d\n", I->op2, paramIndex);
					break;
				case DATA_DOUBLE:
				//nejspis pretypovani druheho operandu na double!!!!!!!!!1
					*((double*)prepFrame->localData[paramIndex]) = I->op2;
					break;
				case DATA_STRING:
					prepFrame->localData[paramIndex] = (char*)I->ptr;
					printf("pushuju %s na index %d\n", (char*)I->ptr, paramIndex);
					break;
				default:
					return INTER_ERROR;
			}
			paramIndex++;
			break;

		case I_FRAMED: ;
			printf("interpret: I_FRAMED\n");
			printf("framed: adresa na topu %p, pocet prvku stacku: %d\n", (void*)frameStack.top->data->returnFunc, frameStack.size);
			//ulozit si odkladany frame!!
			lastFrame = topStack(&frameStack);
			(void) lastFrame;
			popStack(&frameStack); //UVOLNIT LOK DATOVY BLOK
			break;

		case I_CALL:
			printf("interpret: I_CALL\n");
			printf("printArgs = %d\n", (printArgs+1));
			if (I->res == 0){
				//printf("bude proveden skok na instrukci %d\n", (tListItem*)I->ptr->Instruction.insType);
				//prepFrame->returnAddr = malloc(sizeof(tListItem*));
				prepFrame->returnAddr = currentInstrList->Act;
				prepFrame->returnFunc = currentInstrList;
				if (I->op2 == 1){
					prepFrame->returnWhere = prepFrame->localData[I->op1];
				} else if (I->op2 == 2){
					prepFrame->returnWhere = globalData[I->op1];
				} else {
					prepFrame->returnWhere = NULL;
				}
				printf("\n\ncall: adresa na topu %p\na davam adresu %p\n\n", (void*)frameStack.top->data->returnFunc, (void*)currentInstrList);
				pushStack(&frameStack, prepFrame);
				printf("instrukce pro navrat je %d\n", frameStack.top->data->returnAddr->Instruction.insType);
				//goToInstr(currentInstrList, (tListItem*)I->ptr);
				currentInstrList = (tInstrList*)I->ptr;
				activateFirst(currentInstrList);
			} else {
				if (strcmp(I->ptr, "ifj16.print") == 0){
					pushStack(&frameStack, prepFrame);
					hlPrint((printArgs+1));
					popStack(&frameStack);
				}
			}
			break;

		case I_RETURN:
			printf("interpret: I_RETURN\n");
			printf("chci se vratit na funkci %p\n", (void*)lastFrame->returnFunc);
			//printf("chci se vratit na instrukci %d\n", lastFrame->returnAddr->Instruction.insType);//frameStack.top->data->returnAddr->Instruction.insType);
			//goToInstr(instrList, frameStack.top->data->returnAddr);
			//goToInstr(currentInstrList, lastFrame->returnAddr);
			if (I->op1 == 1){
				lastFrame->returnWhere = lastFrame->localData[I->res];
			}
			currentInstrList = lastFrame->returnFunc;
			goToInstr(currentInstrList, lastFrame->returnAddr);
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
						globalData[I->op2] = malloc(sizeof(char *) * 50);
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
								globalData[I->res] = (char*)I->ptr;
								printf("%s\n", (char*)globalData[I->res]);
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
								frameStack.top->data->localData[I->res] = (char*)I->ptr;
								break;
							default:
								break;
						}
					break;
				} else if (I->op1 == 2){ //lokalne promenna
					switch(I->dataType){
							case DATA_INT:
							//printf("assign z indexu %d na index %d\n", I->op2, I->res);
								//printf("instrukce pro navrat je porad %d\n", frameStack.top->data->returnAddr->Instruction.insType);
								(*(int*)frameStack.top->data->localData[I->res]) = (*(int*)frameStack.top->data->localData[I->op2]);
								//printf("instrukce pro navrat je porad %d\n", frameStack.top->data->returnAddr->Instruction.insType);
								//*((int*)globalData[I->res]) = I->op2;
								break;
							case DATA_DOUBLE:
								(*(double*)frameStack.top->data->localData[I->res]) = (*(double*)frameStack.top->data->localData[I->op2]);
								break;
							case DATA_STRING:
								//nevim jestli je to spravne
								frameStack.top->data->localData[I->res] = (char*)frameStack.top->data->localData[I->op2];
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
		nextInstruction(currentInstrList);
	}
	return 0;
}