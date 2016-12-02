/************scanner.h************
 *
 *hlavička hlavička hlavička
 *    
 */

#ifndef SCANNER_DEFINED
#define SCANNER_DEFINED

#include "str.h"


#define KEYWORDS_AMOUNT 17
#define DELIMITERS_AMOUNT 6
#define OPERATORS_AMOUNT 11


typedef enum 
{
    T_ID,           //atribut str
    T_QUALID,       //atribut str
    T_INT,          //atribut int
    T_DOUBLE,       //atribut double
    T_STRING,       //atribut str
    T_ERROR,        //atribut int
    T_EOF,          //atribut zadny

    //delimiters, pozice: DELIMITER+1 az T_DELIMITER+DELIMITERS_AMOUNT, atribut zadny
    T_DELIMITER,
    TD_SEMICOLON,   // ;
    TD_L_BRACKET,   // (
    TD_R_BRACKET,   // )
    TD_LC_BRACKET,  // {
    TD_RC_BRACKET,  // }
    TD_COMMA,       // ,

    //keywords, pozice: T_KEYWORD+1 az T_KEYWORD+KEYWORDS_AMOUNT,   atribut zadny
    T_KEYWORD,
    TK_BOOLEAN,
    TK_BREAK,
    TK_CLASS,
    TK_CONTINUE,
    TK_DO,
    TK_DOUBLE,
    TK_ELSE,
    TK_FALSE,
    TK_FOR,
    TK_IF,
    TK_INT,
    TK_RETURN,
    TK_STRING,
    TK_STATIC,
    TK_TRUE,
    TK_VOID,
    TK_WHILE,

    //operators, pozice: T_OPERATOR+1 az T_OPERATOR+OPERATORS_AMOUNT,   atribut zadny
    T_OPERATOR,
    TO_MULTIPLY,    // *
    TO_DIVIDE,      // /
    TO_ADD,         // +
    TO_MINUS,       // -
    TO_SMALLER,     // <
    TO_GREATER,     // >
    TO_ASSIGN,      // =
    TO_SM_EQUAL,    // <=
    TO_GR_EQUAL,    // >=
    TO_EQUAL,       // ==
    TO_NOTEQUAL,    // !=

}token_types;

    
typedef struct 
{
    token_types type;
    union
    {
        char *str;
        int inumber;
        double dnumber;
    } attribute;
} token;


void tokenInit (token tok);

int iskeyword (char *str);
int isdelimiter (char str);
int whichOperator (char *str);

void setSourceFile(FILE *f);
token getToken();


#endif