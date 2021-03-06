/***** functions.c *****
*
*     Implementace interpretu imperativního jazyka IFJ16
*     Tým 029, varianta b/3/I
*     Autori: Jiruska Adam, Janecek David
*     Login: xjirus01, xjanec28
*     
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "functions.h"
#include "error_codes.h"
#include "str.h"
#include "ial.h"
#include "stack.h"
#include "interpret.h"

int ifj16readInt ()
{
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    for (int i = 0; i < 50; i++)
    {
        //printf("%c %d\n", buffer[i], isdigit(buffer[i]));
        if ((isdigit(buffer[i])) == 0)
            return -NUM_ERROR;
        if (buffer[i+1] == '\n' || buffer[i+1] == EOF)
            break;
    }
    return atoi(buffer);
}

double ifj16readDouble ()
{
    char buffer[50];
    int e = 0;
    int E = 0;
    int period = 0;
    fgets(buffer, sizeof(buffer), stdin);
    char *check = NULL;
    if (strtod(buffer, &check) == 0.0)
    {
        printf ("%s\n", check);
        return -NUM_ERROR;
    }
    for (int i = 0; i < 50; i++)
    {
        //printf("%c %d\n", buffer[i], isdigit(buffer[i]));
        if ((isdigit(buffer[i]) == 0) && (buffer[i] != 'e') && (buffer[i] != 'E') && (buffer[i] != '.'))
            return -NUM_ERROR;
        if (buffer[i] == 'e')
            e++;
        else if (buffer[i] == 'E')
            E++;
        else if (buffer[i] == '.')
            period++;
        if (e > 1 || E > 1 || period > 1 || e+E > 1)
            return -NUM_ERROR;
        if (buffer[i+1] == '\n' || buffer[i+1] == EOF)
            break;
    }
    return strtod(buffer, NULL);
}

char *ifj16readString ()
{
    string str;
    strInit(&str);
    char buffer[1000];
    fgets(buffer, sizeof(buffer), stdin);
    for (int i = 0; i < 1000; i++)
    {
        if (buffer[i] == '\n' || buffer[i+1] == EOF)
            break;
        else if (buffer[i] == '\\')
        {
            switch (buffer[i+1])
            {
                case 'n':
                    strAddChar(&str, '\n');
                    i++;
                break;

                case 't':
                    strAddChar(&str, '\t');
                    i++;
                break;

                case '"':
                    strAddChar(&str, '\"');
                    i++;
                break;

                case '\\':
                    strAddChar(&str, '\\');
                    i++;
                break;

                default:
                strAddChar(&str, buffer[i]);
                break;
            }
        }
        else
        {
            strAddChar(&str, buffer[i]);
        }
    }
    return str.str;
}


int ifj16length(char *s)
{
    return strlen(s);
}

int ifj16compare(char * s1, char * s2)
{
    int cmp = strcmp(s1, s2);
    if (cmp == 0)
        return 0;
    else if (cmp > 0)
        return 1;
    else
        return -1;
}

int ifj16find(char * s, char * search)
{
    if (search[0] == '\0')
        return 0;
    int a = strlen(s);
    int b = strlen(search);
    return boyerMoore(s, search, a, b);
}

char *ifj16substr(char * s, int i, int n)
{
    string str;
    strInit(&str);
    if (strlen(s) < (i+n))
        return "RUN_ERROR"; //DODELAT nejak lip tu chybu
    for (int j = i; j < (i+n); j++)
    {
        strAddChar(&str, s[j]);
    }
    return str.str;
}

char *ifj16sort(char *sorted)
{
    int length = strlen(sorted);
    char str[length];
    for (int i = 0; i < length; i++)
    {
        str[i] = sorted[i];
    }
    shellSort(str, length);
    for (int j = 0; j < length; j++)
    {
        sorted[j] = str[j];
    }
    return sorted;
}

void hlPrint(int n, int types[10])
{
    for (int i = 0; i < n; i++)
    {
        if (types[i] == 0)  //int
        {
            char str[50];
            sprintf(str, "%d", *(int *)frameStack.top->data->localData[i]);
            ifj16print(str);
        }
        else if (types[i] == 1) //double
        {
            char str[50];
            sprintf(str, "%g", *(double *)frameStack.top->data->localData[i]);
            ifj16print(str);
        }
        else    //string
            ifj16print((char *)frameStack.top->data->localData[i]);
    }
}


void ifj16print(char *string)
{
    printf("%s", string);
}