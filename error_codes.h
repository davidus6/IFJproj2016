/***** error_codes *****
*
*     Implementace interpretu imperativního jazyka IFJ16
*     Tým 029, varianta b/3/I
*     Autori: Janecek David, Jiruska Adam
*     
*/



#ifndef ERROR_DEFINED         //zkontroluje jestli už bylo ERROR_DEFINED definováno, pokud ne tak proběhne kód v ifu
#define ERROR_DEFINED

      #define OK              0    // činnost interpretu bez chyb
      #define LEX_ERROR       1    // chyba v programu v rámci lexikální analýzy
      #define SYNTAX_ERROR    2    // chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)

      //sémantické chyby v programu
      #define SEM_ERROR_UND   3    // nedefinovaná třída/funkce/proměnná, pokus o redefinici třídy/funkce/proměnné, atd.
      #define SEM_ERROR_TYPE  4    // chyba typové kompatibility v aritmetických, řetezcových a relačních výrazech; špatný pocet či typ parametrů u volání funkce
      #define SEM_ERROR_OTH   6    // ostatní sémantické chyby

      //běhové chyby
      #define NUM_ERROR       7    // chyba při načítání číselné hodnoty ze vstupu
      #define UNIT_ERROR      8    // chyba při práci s neinicializovanou proměnnou
      #define DIV_ERROR       9    // dělení nulou
      #define RUN_ERROR       10   // ostatní běhové chyby

      #define INTER_ERROR     99   // interní chyba interpretu (neovlivněná vstupním programem), např. chyba alokace paměti

 #endif
