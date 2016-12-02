/*
Projekt IFJ 2016, 029

Michal Kuba - xkubam02

Semanticka analyza
*/

#include "sem.h"

static void is_func_defined_st(TTabSym *url, bool *func_defined)
{
    if(url->leftChild != NULL)
    {
        is_func_defined_st(url->leftChild, func_defined); // kontrola leveho syna
    }

    if(url->rightChild != NULL)
    {
        is_func_defined_st(url->rightChild, func_defined); // kontrola praveho syna
    }
    Tglobal_url_tab_sym *data= (Tglobal_url_tab_sym *)(url->ptr_data);

    if(data->defined == false)
    {
        *func_defined=false; //neni-li definovana funkce, zadna jina funkce neni definovana
    }
}
