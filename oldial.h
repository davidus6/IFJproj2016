/*
Projekt IFJ 2016, 029

Michal Kuba - xkubam02

Binární vyhledávací strom, Boyer-Mooreův algoritmus, øazení Shell sort

*/

#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include "error_codes.h"

typedef char* TKey;

typedef struct table_sym{
    struct table_sym* left_child;
    struct table_sym* right_child;
    void* ptr_data;
    TKey key;
}TTableSym;

typedef TTableSym *TTableSymPoint;
typedef struct bin_tree{
    TTableSymPoint root;
}TBinTree;

void delete_url(TTableSymPoint ptr_tmp, TKey key);
void delete_tree(TBinTree *tree);

void count_jumps(char *p, int *jump, int len_p);
void count_match_jump(char *p, int *match_jump, int len_p);

int BMa(char *p, char *t, int len_p, int len_t); // Boyer-Mooruv algoritmus

void find_parent(TTableSymPoint ptr_tmp,
                 TKey key,
                 TTableSymPoint *searched_url,
                 TTableSymPoint *parent_url
                 );
TBinTree *initialize_tree();

int insert_in_tree(TTableSymPoint *url, TKey key, void *data);
int min(int x, int y);
int max(int x, int y);

void shellsort(char text[], int left, int right);

void search_insert(TTableSymPoint url, TKey key, bool *found, TTableSymPoint *spot);
void search_in_tree(TTableSymPoint ptr_tmp, TKey key, TTableSymPoint *searched_url);
#endif // IAL_H_INCLUDED
