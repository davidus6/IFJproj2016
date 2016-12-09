#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "vestaveneFce.h"
#include "error_codes.h"
#include "str.h"


int readInt ()
{
	char buffer[50];
	fgets(buffer, sizeof(buffer), stdin);
	for (int i = 0; i < 50; i++)
	{
		//printf("%c %d\n", buffer[i], isdigit(buffer[i]));
		if ((isdigit(buffer[i])) == 0)
			return -NUM_ERROR;
		if (buffer[i+1] == '\n' || buffer[i+1] == EOF)
			return atoi(buffer);;
	}
	return atoi(buffer);
}
/*
double readDouble ();
*/
char *readString ()
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


int length(char *s)
{
	return strlen(s);
}

int compare(char * s1, char * s2)
{
	int cmp = strcmp(s1, s2);
	if (cmp == 0)
		return 0;
	else if (cmp > 0)
		return 1;
	else
		return -1;
}

int find(char * s, char * search);

char *substr(char * s, int i, int n)
{
	string str;
	strInit(&str);
	if (strlen(s) < (i+n))
		return "RUN_ERROR";	//DODELAT nejak lip tu chybu
	for (int j = i; j < (i+n); j++)
	{
		strAddChar(&str, s[j]);
	}
	return str.str;
}

char *sort(char *sorted)
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

void shellSort(char str[],int n)
{
	int i, j, step;
	char temp;
	step = n / 2;
	while (step > 0)
	{
		for (i = step; i < n; i++)
		{
			j = i;
			temp = str[i];
			while ((j >= step) && (str[j-step] > temp))
			{
				str[j] = str[j-step];
				j = j - step;
			}
		str[j] = temp;
		}
		if (step == 2)
			step = 1;
		else
			step = (int) (step / 2.2);
	}
}


void print(int cislo)
{
	printf("%d", cislo);
}