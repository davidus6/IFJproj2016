/***** scanner.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Janecek David, Jiruska Adam
*	
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "scanner.h"
#include "str.h"
#include "error_codes.h"

//#define DEBUG

//DODELAT smaze se odsud az budou soubory pohromade
int errorHandle(int value)
{
  exit(value);
}

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
	','
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
	return -1;  //nemelo by nikdy nastat - uzitecne pokud je nekde neco spatne
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




void setSourceFile(FILE *f)
{
  source = f;
}

void tokenInit(token *tok)
{
	tok->type = T_ERROR;
	tok->attribute.str = NULL;
	return;
}


// funkce rozdeli kval. id na prvni id a druhe id
// a zjisti jestli nejake z nich neni keyword, pokud je, vraci 1, jinak 0
int checkQualid(char *qualid)
{
	string string;
	strInit(&string);
	char ch;
	int length = strlen(qualid);
	int size = 1;
	ch = qualid[0];
	while (ch != '.')
	{
		strAddChar(&string, ch);
		ch = qualid[size];
		size++;
	}
	if (iskeyword(string.str))
	{
		strFree(&string);
		return 1;
	}
	else
	{
		strClear(&string);
		for (; size < length; size++)
		{
			ch = qualid[size];
			strAddChar(&string, ch);
		}
		if (iskeyword(string.str))
		{
			strFree(&string);
			return 1;
		}
		else
		{
			strFree(&string);
			return 0;
		}
	}
}


void ungetToken(token tok)
{
	bufferNotEmpty = 1;
	buffer.type = tok.type;
	if (tok.type == T_ID || tok.type == T_QUALID || tok.type == T_STRING)
		buffer.attribute.str = tok.attribute.str;
	else if (tok.type == T_INT)
		buffer.attribute.inumber = tok.attribute.inumber;
	else if (tok.type == T_DOUBLE)
		buffer.attribute.dnumber = tok.attribute.dnumber;
	else
		;
}

token getToken()
{
	if (bufferNotEmpty == 1)
	{
		bufferNotEmpty = 0;
		return buffer;
	}
	// pomocna promenna pro kontrolu funkci volajicich malloc
	int checkMalloc;

	string asdf123;
	string *s = &asdf123;
	checkMalloc = strInit(s);
	if (checkMalloc == STR_ERROR)
	{
		strFree(s);
		errorHandle (INTER_ERROR);
	}
	token tok;
	tokenInit (&tok);
	
	//pomocne promenne k urceni pozice oddelovace a klicoveho slova
	int delim_assist = 0;
	int keyw_assist = 0;

	//pomocne pole pri reseni escape sequence typu \ddd
	char octa[4];
	octa[3] = '\0';

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
		ESCAPE,
		OCTA1,
		OCTA2,
		
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
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
						{
							strFree(s);
							errorHandle (INTER_ERROR);
						}
					state = ID;
				}
				else if (isalpha(c))
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
						{
							strFree(s);
							errorHandle (INTER_ERROR);
						}   
					state = ID_KEY;		
				}
				else if (isdigit(c))
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = DIGIT;
				}
				else if (isdelimiter(c))
				{
					delim_assist = isdelimiter(c);
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = F_DELIM;
				}
				else if (c == 34)
				{
					//strAddChar(s, c); //nepridava se pac jen oznacuje ze se jedna o string
					state = STRING;
				}
				else if (c == '/')
				{
					strAddChar(s, c);	//pokud to pak bude koment tak to musim smaznout
					state = SLASH;
				}
				else if (c == '*' || c == '+' || c == '-')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = F_OPER;
				}
				else if (c == '<' || c == '>' || c == '=')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = OP_GR1;
				}
				else if (c == '!')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;

			case ID_KEY: //identifikator nebo klicove slovo
				if (isalpha(c)) //pokracuje
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = ID_KEY;
				}
				else if (isdigit(c)) //bude to identifikator - klicova slova neobsahuji cisla
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = ID;
				}
				else if (c == '.')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
				if (isalnum(c) || c == '$' || c =='_')	// identifikator pokracuje
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					} 
					state = ID;
				}
				else if (c == '.')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = DIGIT;
				}
				else if (c == '.')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = DEC_ER;
				}
				else if (c == 'e' || c == 'E')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = DEC;
				}
				else
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;

			case DEC:
				if (isdigit(c))
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = DEC;
				}
				else if (c == 'e' || c == 'E')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = EXP;
				}
				else
					state = F_DOUBLE;
			break;

			case EXP:
				if (c == '+' || c == '-')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = S_EXP;
				}
				else if (isdigit(c))
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = NS_EXP;
				}
				else
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;

			case S_EXP:     //pripad kdy exponent zacina nepovinnym znamenkem
				if (isdigit(c))
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = NS_EXP;
				}
				else
				{
					ungetc(c, source);
					state = F_DOUBLE;
				}
			break;

			case STRING:
				if (c == 34)
				{
					state = F_STRING;
				}
				else if (c == '\n' || c == EOF)
				{
					ungetc (c, source);
					state = F_ERROR;
				}
				else if (c == '\\')
				{
					state = ESCAPE;
				}
				else
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = STRING;
				}
			break;

			case SLASH:
				if (c == '*')
				{
					strClear(s);
					state = B_COMM;
				}
				else if (c == '/')
				{
					strClear(s);
					state = L_COMM;
				}
				else
				{
					ungetc(c, source);
					state = F_OPER;
				}
			break;

			case L_COMM:
				if (c == '\n' || c == EOF)
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
				else if (c == EOF)
					{
						ungetc(c, source);
						state = F_ERROR;
					}
				else
					state = B_COMM;
			break;

			case BC_END:
				if (c == '/')
				{
					strClear(s);
					state = INIT;
				}
				else if (c == EOF)
					{
						ungetc(c, source);
						state = F_ERROR;
					}
				else
					state = B_COMM;
			break;

			case OP_GR1:
				if (c == '=')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
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
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = QUALID;
				}
				else
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;

			case QUALID:
				if (isalnum(c) || c == '$' || c =='_')
				{
					checkMalloc = strAddChar(s, c);
					if (checkMalloc == STR_ERROR)
					{
						strFree(s);
						errorHandle (INTER_ERROR);
					}
					state = QUALID;
				}
				else
				{
					ungetc(c, source);
					state = F_QUALID;
				}
			break;

			case ESCAPE:
				if (c == 34)
				{
					checkMalloc = strAddChar(s, 34);
					if (checkMalloc == STR_ERROR)
						errorHandle (INTER_ERROR);
					state = STRING;
				}
				else if (c == 'n')
				{
					checkMalloc = strAddChar(s, '\n');
					if (checkMalloc == STR_ERROR)
						errorHandle (INTER_ERROR);
					state = STRING;
				}
				else if (c == 't')
				{
					checkMalloc = strAddChar(s, '\t');
					if (checkMalloc == STR_ERROR)
						errorHandle (INTER_ERROR);
					state = STRING;
				}
				else if (c == '\\')
				{
					checkMalloc = strAddChar(s, '\\');
					if (checkMalloc == STR_ERROR)
						errorHandle (INTER_ERROR);
					state = STRING;
				}
				else if (isdigit(c) && ((c - '0') <= 3))
				{
					octa[0] = c;
					state = OCTA1;
				}
				else
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;

			case OCTA1:
				if (isdigit(c) && ((c - '0') <= 7))
				{
					octa[1] = c;
					state = OCTA2;
				}
				else
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;

			case OCTA2:
				if (isdigit(c) && ((c - '0') <= 7) && ((c - '0') >= 1))
				{
					octa[2] = c;
					int octalNumber = atoi(octa);
					int decimalNumber = 0;
					int i = 0;
					while(octalNumber != 0)
					{
						decimalNumber += (octalNumber%10) * pow(8,i);
						++i;
						octalNumber/=10;
					}
					char e = decimalNumber;
					checkMalloc = strAddChar(s, e);
					if (checkMalloc == STR_ERROR)
						errorHandle (INTER_ERROR);
					state = STRING;
				}
				else
				{
					ungetc (c, source);
					state = F_ERROR;
				}
			break;


			case OPER:      // stav vypada zbytecne, ale je potreba pro spravne vyhledavani operatoru
			{
				ungetc (c, source);
				state = F_OPER;
			}
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
				if (checkQualid(s->str))
				{
					strFree(s);
					errorHandle(LEX_ERROR);
				}
				tok.type = T_QUALID;
				tok.attribute.str = s->str;
				return tok;
			break;


			case F_ERROR:
				ungetc(c, source);
				#ifdef DEBUG
				printf("konecny stav ERROR\n");
				#endif
				strFree(s);
				errorHandle(LEX_ERROR);
			break;
		}
	}
	return tok;
}

/*
**DODELAT:
*			
*/