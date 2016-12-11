/***** instructions.h *****
*
*   Implementace interpretu imperativního jazyka IFJ16
*   Tým 029, varianta b/3/I
*   Autori: Jiruska Adam, Janecek David
*   Login: xjirus01, xjanec28
*   
*/

#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include <malloc.h>

//typy instrukci
typedef enum insType{
    I_ADD,      //0
    I_SUB,      //1
    I_MUL,      //2
    I_DIV,      //3
    I_LT,       //4
    
    I_READ,     //5
    I_WRITE,    //6
    I_STOP,     //7
    I_LABEL,    //8

    I_CALL,     //9
    I_FRAMEC,   //10
    I_PUSHP,    //11
    I_FRAMED,   //12
    I_RETURN,   //13

    I_BLOCKC,   //14
    I_ASSIGN,   //15

    I_GOTO,     //16
    I_GT,       //17
    I_LET,      //18
    I_GET,      //19
    I_EQ,       //20
    I_NEQ,      //21
    I_RUNRUN    //22
} insType;


typedef enum
{
    DATA_INT,
    DATA_DOUBLE,
    DATA_STRING,
    DATA_VOID,  //fce nemusi nic vracet
    DATA_BOOL,
    DATA_UNKNOWN
}dataTypes;

typedef struct
{
    int insType; //typ instrukce
    dataTypes dataType; //typ dat
    void *ptr; //ukazatel kam je potreba :)
    int op1; //operand1
    int op2; //operand2
    int res; //vysledek
} tInstr;

typedef struct listItem
{
    tInstr Instruction;
    struct listItem *nextIns;
    struct listItem *prevIns;
} tListItem;

typedef struct tInstrList
{
    struct listItem *First;  //ukazatel na prvni instrukci
    struct listItem *Last;   //ukazatel na posledni instrukci
    struct listItem *Act; //ukazatel na aktivni instrukci
} tInstrList;

void initList(tInstrList *L);
void addInstruction(tInstrList *L, tInstr I);
void destroyList(tInstrList *L);
void activateFirst(tInstrList *L);
void nextInstruction(tInstrList *L);
tInstr *getInstruction(tInstrList *L);
void goToInstr(tInstrList *L, tListItem *GTWhere);
void print_elements_of_list(tInstrList TL);

#endif