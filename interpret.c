#include <stdio.h>
#include "instructions.h"
#include "stack.h"

int runInterpret(tInstrList *instrList){
	activateFirst(instrList);
	tInstr *I;	
	tStack frameStack;
	initStack(&frameStack);
	while (1){
		I = getInstruction(instrList);
		//printf("cyklim-typ instrukce je %d\n", I->insType);
		switch (I->insType){

		case I_STOP:
			printf("KONEEEEEEEEEEEEEEEEEEEC\n");
		//konec programu
			return 0;
			break;

		case I_READ:
		//nacteni hodnoty ze stdin
			break;

		case I_WRITE:
			printf("%d\n", frameStack.top->data->bla);
		//zapis hodnoty na stdout
			break;

		case I_GOTO:
		//skok v programu
			goToInstr(instrList, I->res);
			break;

		case I_LABEL:
		//navesti, nic se neprovadi
			break;

		case I_ADD:
			break;

		case I_SUB:
			break;

		case I_MUL:
			break;

		case I_DIV:
			break;

		case I_FRAMEC: ;
			frameData prepFrame;
			prepFrame.bla = 45;
			pushStack(&frameStack, &prepFrame);
			break;

		case I_FRAMED:
		//ulozit si odkladany frame!!
			popStack(&frameStack);
			break;

		case I_CALL:
			goToInstr(instrList, I->res);
			break;
		}
		nextInstruction(instrList);
	}
}