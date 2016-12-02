/*
Projekt IFJ 2016, 029

Michal Kuba - xkubam02

Binární vyhledávací strom, Boyer-Mooreùv algoritmus, øazení Shell sort
*/
#include "ial.h"

#define MAX_CHAR 255

/* ======================= Boyer-Mooreův algoritmus ==================== */
// Vrací menší ze dvou čísel
int min (int x, int y)
{
    if(x<y)
        return x;
    else
        return y;
}

//Vrací větší ze dvou čísel
int max (int x, int y)
{
    if(x>y)
        return x;
    else
        return y;
}

//Boyer-Moorův algoritmus, prvni heuristika
void count_jumps(char *p, int *jump, int len_p)
{
    for(int i=0; i < MAX_CHAR; i++)
        {
            jump[i]=len_p;
        }
    for(int i=0; i < len_p; i++)
    {
        jump[(unsigned char)p[i]]=len_p-i-1;
    }
}

void count_match_jump(char *p, int *match_jump, int len_p)
{
    int keep[len_p+1];
    int j;
    int q;
    int backup;

    for (int i=0; i < len_p; i++)
    {
        if(i==(len_p-1))
        {
           keep[i+1]=0;
        }
        keep[i]=0;
        match_jump[i]=0;
    }

    for(j=0; j < len_p+1; j++)
    {
        match_jump[j]=2*len_p-j;
    }
    j=len_p;
    q=j+1;

    while (j > 0)
    {
        keep[j]=q;

        while((q <= len_p) && (p[j-1] != p[q-1]))
        {
            match_jump[q]=min(match_jump[q],len_p-j);
            q=keep[q];
        }
        j--;
        q--;
    }

    for(j=0; j < q+1; j++)
    {
        match_jump[j]=min(match_jump[j],len_p-j+q);
    }

    backup=keep[q];

    while (q<=len_p)
    {
        while (q<=backup)
        {
            match_jump[q]=min(match_jump[q],backup-q+len_p);
            q++;
        }
        backup=keep[backup];
    }
}


/*  */
