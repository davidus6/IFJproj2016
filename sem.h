#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include "ial.h"
    #include "error_codes.h"

    /* Definice datovych typu */
    typedef enum types_data{
        TNon,
        TString,
        TAuto,
        TInt,
        TDouble,
    } TType_data;

   /* typedef struct{
        unsigned start_func;
        TType_data type_data;
        TStack *local_sym_tab;
        TStack *params;
        unsigned count_var;
        bool is_def;
    } Tglobal_url_tab_sym;*/

    typedef struct
    {
        TType_data type;
        char *id;
        bool is_def;
        int offset;

    } TLocal_url_tab_sym;
    typedef struct
    {
        char *id;
        TType_data type;
    } TParam;


    TTableSym *Find_func_st(char *name);


#endif // SEM_H_INCLUDED
