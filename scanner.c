/************scanner.c************
 *
 *hlavička hlavička hlavička
 *    
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"
#include "str.h"
#include "error_codes.h"

#define DEBUG

static char *keywords[KEYWORDS_AMOUNT] = 
{
    "boolean",
    "break",
    "class",
    "continue",
    "do",
    "double",
    "else",
    "false",
    "for",
    "if",
    "int",
    "return",
    "String",
    "static",
    "true",
    "void",
    "while"
};

static char delimiters[DELIMITERS_AMOUNT] =
{
    ';',
    '(',
    ')',
    '{',
    '}',
    ','             //DODELAT je jeste neco delimiter? napr [] " '
};

static char *operators[OPERATORS_AMOUNT] =
{
    "*",
    "/",
    "+",
    "-",
    "<",
    ">",
    "=",
    "<=",
    ">=",
    "==",
    "!="
};

int whichOperator (char *str)
{
    for (int i = 0; i < OPERATORS_AMOUNT; i++)
    {
        if ((strcmp(str, operators[i])) == 0)   //funkce je volana az v F_OPER, musi tedy najit shodu
            return i+1;                 //vraci pozici operatoru v poli pocitano od 1
    }
    return -1;  //aby prekladac nebrecel, pripadne uzitacne jestli jsem neco dosral
}

int isdelimiter (char ch)
{
    for (int i = 0; i < DELIMITERS_AMOUNT; i++)
    {
        if (ch == delimiters[i])        //pokud najde shodu, vrati pozici oddelovace v poli pocitano od 1
            return i+1;
    }
    return 0;   //pokud nenajde vrati 0
}


int iskeyword (char *str)
{
    for (int i = 0; i < KEYWORDS_AMOUNT; i++)
    {
        if ((strcmp(str, keywords[i])) == 0)    //pokud najde shodu, vrati pozici klicoveho slova v poli pocitano od 1
            return i+1;
    }
    return 0; //pokud nenajde vrati 0
}


// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
  source = f;
}

void tokenInit(token tok)
{
    tok.type = T_ERROR;
    tok.attribute.str = NULL;
    return;
}


token getToken()
{
    string asdf123;
    string *s = &asdf123;
    strInit(s);
    token tok;
    tokenInit (tok);
    
    //pomocne promenne k urceni pozice oddelovace a klicoveho slova
    int delim_assist = 0;
    int keyw_assist = 0;

    typedef enum
    {
        INIT,
        ID_KEY,
        ID,
        F_KEY,
        F_ID,
        DIGIT,
        DEC_ER,
        DEC,
        EXP,
        S_EXP,
        NS_EXP,
        STRING,
        SLASH,
        L_COMM,
        B_COMM,
        BC_END,
        OP_GR1,
        OP_GR2,
        OPER,
        Q_ER,
        QUALID,
        
        F_QUALID,
        F_OPER,
        F_STRING,
        F_INT,
        F_DOUBLE,
        F_DELIM,
        F_ERROR
    }Tstates;

    Tstates state;
    state = INIT; //pocatecni stav

    char c;

    while ((c = fgetc(source)))
    {
        switch (state)
        {
            case INIT:
                if (isspace(c)) //bila mista
                    state = INIT;
                else if (c == '$' || c == '_')   //bude to identifikator
                {
                    strAddChar(s, c);
                    state = ID;
                }
                else if (isalpha(c))
                {
                    strAddChar(s, c);   //bude to identifikator nebo klicove slovo
                    state = ID_KEY;
                }
                else if (isdigit(c))
                {
                    strAddChar(s, c);
                    state = DIGIT;
                }
                else if (isdelimiter(c))
                {
                    delim_assist = isdelimiter(c);
                    strAddChar(s, c);
                    state = F_DELIM;
                }
                else if (c == '\"')    //mozna radsi predelat na ascii hodnotu 34 DODELAT
                {
                    //strAddChar(s, c); //nepridava se pac jen oznacuje ze se jedna o string
                    state = STRING;
                }
                else if (c == '/')
                {
                    state = SLASH;
                }
                else if (c == '*' || c == '+' || c == '-')
                {
                    strAddChar(s, c);
                    state = F_OPER;
                }
                else if (c == '<' || c == '>' || c == '=')
                {
                    strAddChar(s, c);
                    state = OP_GR1;
                }
                else if (c == '!')
                {
                    strAddChar(s, c);
                    state = OP_GR2;
                }
                else if (c == EOF)      //konec souboru
                {
                    #ifdef DEBUG
                    printf ("je to EOF\n");
                    #endif
                    strFree(s);
                    tok.type = T_EOF;
                    return tok;
                }
                else
                    state = F_ERROR;
            break;

            case ID_KEY: //identifikator nebo klicove slovo
                if (isalpha(c)) //pokracuje
                {
                    strAddChar(s, c);
                    state = ID_KEY;
                }
                else if (isdigit(c)) //bude to identifikator - klicova slova neobsahuji cisla
                {
                    strAddChar(s, c);
                    state = ID;
                }
                else if (c == '.')
                {
                    strAddChar(s, c);
                    state = Q_ER;
                }
                else //konec identifikatoru nebo klic slova
                {
                    ungetc (c, source); //vrati posledni nacteny znak
                    if (iskeyword(s->str)) //overi zda se jedna o klicove slovo, vraci 0 pokud NENI
                        {
                            keyw_assist = iskeyword(s->str);
                            state = F_KEY;
                        }
                    else
                        state = F_ID;
                }
            break;

            case ID: //identifikator
                if (isalnum(c) || c == '$' || c =='_')
                {
                    strAddChar(s, c); // identifikator pokracuje
                    state = ID;
                }
                else if (c == '.')
                {
                    strAddChar(s, c);
                    state = Q_ER;
                }
                else // konec identifikatoru
                {
                    ungetc (c, source);
                    state = F_ID;
                }
            break;

            case DIGIT:
                if (isdigit(c)) //cislo pokracuje
                {
                    strAddChar(s, c);
                    state = DIGIT;
                }
                else if (c == '.')
                {
                    strAddChar(s, c);
                    state = DEC_ER;
                }
                else if (c == 'e' || c == 'E')
                {
                    strAddChar(s, c);
                    state = EXP;
                }
                else
                {
                    ungetc (c, source);
                    state = F_INT;
                }
            break;

            case DEC_ER:        // desetinne cislo nebo error
                if (isdigit(c))
                {
                    strAddChar(s, c);
                    state = DEC;
                }
                else
                    state = F_ERROR;
            break;

            case DEC:
                if (isdigit(c))
                {
                    strAddChar(s, c);
                    state = DEC;
                }
                else if (c == 'e' || c == 'E')
                {
                    strAddChar(s, c);
                    state = EXP;
                }
                else
                    state = F_DOUBLE;
            break;

            case EXP:
                if (c == '+' || c == '-')
                {
                    strAddChar(s, c);
                    state = S_EXP;
                }
                else if (isdigit(c))
                {
                    strAddChar(s, c);
                    state = NS_EXP;
                }
                else
                    state = F_ERROR;
            break;

            case S_EXP:     //pripad kdy exponent zacina nepovinnym znamenkem
                if (isdigit(c))
                {
                    strAddChar(s, c);
                    state = S_EXP;
                }
                else
                {
                    ungetc(c, source);
                    state = F_DOUBLE;
                }
            break;

            case NS_EXP:    //pripad kdy exponent nezacina znamenkem
                if (isdigit(c))
                {
                    strAddChar(s, c);
                    state = NS_EXP;
                }
                else
                {
                    ungetc(c, source);
                    state = F_DOUBLE;
                }
            break;

            case STRING:
                if (c == '\"')  //mozna radsi dodelat na ascii hodnotu 34
                {
                    state = F_STRING;
                }
                else if (c == '\n' || c == EOF)
                    state = F_ERROR;
                /*else if (c == '\\')
                {
                    state = DODELAT;
                }                                       DODELAT ESCAPE SEQUENCE */
                else
                {
                    strAddChar(s, c);
                    state = STRING;
                }
            break;

            case SLASH:
                if (c == '*')
                {
                    state = B_COMM;
                }
                else if (c == '/')
                {
                    state = L_COMM;
                }
                else
                {
                    ungetc(c, source);
                    state = F_OPER;
                }
            break;

            case L_COMM:
                if (c == '\n')
                {
                    strClear(s);
                    state = INIT;
                }
                else
                    state = L_COMM;
            break;

            case B_COMM:
                if (c == '*')
                    state = BC_END;
                else
                    state = B_COMM;
            break;

            case BC_END:
                if (c == '/')
                {
                    strClear(s);
                    state = INIT;
                }
                else
                    state = B_COMM;
            break;

            case OP_GR1:
                if (c == '=')
                {
                    strAddChar(s, c);
                    state = OPER;
                }
                else
                {
                    ungetc(c, source);
                    state = F_OPER;
                }
            break;

            case OP_GR2:
                if (c == '=')
                {
                    strAddChar(s, c);
                    state = OPER;
                }
                else
                {
                    ungetc(c, source);
                    state = F_ERROR;
                }
            break;

            case Q_ER:
                if (isalnum(c) || c == '$' || c =='_')
                {
                    strAddChar(s, c);
                    state = QUALID;
                }
                else
                    state = F_ERROR;
            break;

            case QUALID:
            if (isalnum(c) || c == '$' || c =='_')
                {
                    strAddChar(s, c);
                    state = QUALID;
                }
                else
                {
                    ungetc(c, source);
                    state = F_QUALID;
                }
            break;


            case OPER:      // stav vypada zbytecne, ale je potreba pro spravne vyhledavani operatoru
                state = F_OPER;
            break;






            //konecne stavy
            //pri prechodu do konecneho stavu vzdy nactu dalsi znak, je ho tedy potreba odnacist

            case F_ID:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav ID\n");
                #endif
                tok.type = T_ID;
                tok.attribute.str = s->str;
                return tok;
            break;

            case F_KEY:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav KEY\n");
                #endif
                tok.type = keyw_assist + T_KEYWORD;
                strFree(s);
                tok.attribute.str = NULL;
                return tok;
            break;

            case F_INT:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav INT\n");
                #endif
                tok.type = T_INT;
                tok.attribute.inumber = atoi(s->str);
                strFree(s);
                return tok;
            break;

            case F_DOUBLE:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav DOUBLE\n");
                #endif
                tok.type = T_DOUBLE;
                //double number = strToDouble(s); pokud xey znamena x^y
                double number = strtod(s->str, NULL);
                tok.attribute.dnumber = number;
                strFree(s);
                return tok;
            break;

            case F_STRING:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav STRING\n");
                #endif
                tok.type = T_STRING;
                tok.attribute.str = (s->str);
                return tok;
            break;

            case F_OPER:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav OPERATOR\n");
                #endif
                tok.type = (whichOperator(s->str) + T_OPERATOR);
                strFree(s);
                return tok;
            break;

            case F_DELIM:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav DELIMITER\n");
                #endif
                tok.type = (delim_assist + T_DELIMITER);     //delim_assist bude 1-6
                strFree(s);
                return tok;
            break;

            case F_QUALID:
                ungetc(c, source);
                #ifdef DEBUG
                printf("konecny stav QUALID\n");
                #endif
                tok.type = T_QUALID;
                tok.attribute.str = s->str;
                return tok;
            break;


            case F_ERROR:
                ungetc (c, source);
                #ifdef DEBUG
                printf("konecny stav ERROR\n");
                #endif
                tok.type = T_ERROR;
                tok.attribute.inumber = LEX_ERROR;
                strFree(s);
                return tok;
            break;
        }
    }
    return tok;
}

/*
DODELAT osetreni EOF v neukoncenem retezci a komentu !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
DODELAT check malloc u strInit a strAddChar !!!!!!!!!!!!!!
*/