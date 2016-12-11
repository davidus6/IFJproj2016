/***** ial.c *****
*
*	Implementace interpretu imperativního jazyka IFJ16
*	Tým 029, varianta b/3/I
*	Autori: Jiruska Adam, Janecek David
*	Login: xjirus01, xjanec28
*	
*/


#include "ial.h"
#include <string.h>

/**** vestavene funkce ****/
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
		step = step / 2;
	}
}


/**** tabulka symbolu ****/
void initClassTree(nodeClassPtr *root)
{
	root = NULL;
}

void insertClass(nodeClassPtr *root, char *key)
{
	nodeClassPtr new = malloc(sizeof(struct nodeClass));
	new->keyName = key;
	new->innerFunc = NULL;
	new->innerVar = NULL;
	initFuncTree(&(new)->innerFunc);
	initVarTree(&(new)->innerVar);
	new->left = NULL;
	new->right = NULL;
	if (*root == NULL)
		*root = new;
	else
	{
		nodeClassPtr temp;
		temp = *root;
		while (temp != NULL)
		{
			if (strcmp(new->keyName, temp->keyName) < 0)
			{
				if (temp->left == NULL)
				{
					temp->left = new;
					temp = NULL;
				}
				else
					temp = temp->left;
			}
			else if (strcmp(new->keyName, temp->keyName) == 0)
			{
				temp = NULL;
				free (new);
				return;
			}
			else
				if (temp->right == NULL)
				{
					temp->right = new;
					temp = NULL;
				}
				else
					temp = temp->right;
		}
	}
}

//funkce vyhledava tridu s klicem key
//pokud najde, do promenne found vraci ukazatel na tuto tridu
int searchClass(nodeClassPtr root, char *key, nodeClassPtr *found)
{
	if (root == NULL)
		return FALSE;
	int find = FALSE;		//FALSE == 0, TRUE ==1
	*found = NULL;
	*found = root;
	while (*found != NULL)
	{
		if (strcmp(key, (*found)->keyName) < 0)	// pokud je hledany mensi
		{
			*found = (*found)->left;
		}
		else if (strcmp(key, (*found)->keyName) > 0)	//pokud je hledany vetsi
		{
			*found = (*found)->right;
		}
		else			//rovnaji se
		{
			find = TRUE;
			break;
		}
	}
	return find;
}

void disposeClassTree(nodeClassPtr *root)
{
  if(*root != NULL)
  {
  	disposeVarTree(&(*root)->innerVar);
  	disposeFuncTree(&(*root)->innerFunc);
	disposeClassTree(&(*root)->left);
	disposeClassTree(&(*root)->right);
	free(*root);
	*root = NULL;
  }
}

void initVarTree(nodeVarPtr *root)
{
	root = NULL;
}

void insertVar(nodeVarPtr *root, char *key, dataTypes type, int *index)
{
	nodeVarPtr new = malloc(sizeof(struct nodeVar));
	new->keyName = key;
	new->type = type;
	new->left = NULL;
	new->right = NULL;
	new->index = *index;
	if (*root == NULL)
		*root = new;
	else
	{
		nodeVarPtr temp;
		temp = *root;
		while (temp != NULL)
		{
			if (strcmp(new->keyName, temp->keyName) < 0)
			{
				if (temp->left == NULL)
				{
					temp->left = new;
					temp = NULL;
				}
				else
					temp = temp->left;
			}
			else if (strcmp(new->keyName, temp->keyName) == 0)
			{
				temp = NULL;
				free (new);
				return;
			}
			else
				if (temp->right == NULL)
				{
					temp->right = new;
					temp = NULL;
				}
				else
					temp = temp->right;
		}
	}
}

int searchVar(nodeVarPtr root, char *key, nodeVarPtr *found)
{
	if (root == NULL)
		return FALSE;
	int find = FALSE;		//FALSE == 0, TRUE ==1
	*found = root;
	while (*found != NULL)
	{
		if (strcmp(key, (*found)->keyName) < 0)	// pokud je hledany mensi
		{
			*found = (*found)->left;
		}
		else if (strcmp(key, (*found)->keyName) > 0)	//pokud je hledany vetsi
		{
			*found = (*found)->right;
		}
		else			//rovnaji se
		{
			find = TRUE;
			break;
		}
	}
	return find;
}

void disposeVarTree(nodeVarPtr *root)
{
	if (*root != NULL)
	{
		disposeVarTree(&(*root)->left);
		disposeVarTree(&(*root)->right);
		free(*root);
		*root = NULL;
	}
}

void initFuncTree(nodeFuncPtr *root)
{
	root = NULL;
}

void insertFunc(nodeFuncPtr *root, char *key, dataTypes type)
{
	nodeFuncPtr new = malloc(sizeof(struct nodeFunc));
	new->keyName = key;
	new->type = type;
	new->localTable = NULL;
	initVarTree(&((new)->localTable));
	new->left = NULL;
	new->right = NULL;
	if (*root == NULL)
	{
		*root = new;
	}
	else
	{
		nodeFuncPtr temp;
		temp = *root;
		while (temp != NULL)
		{
			if (strcmp(new->keyName, temp->keyName) < 0)
			{
				if (temp->left == NULL)
				{
					temp->left = new;
					temp = NULL;
				}
				else
					temp = temp->left;
			}
			else if (strcmp(new->keyName, temp->keyName) == 0)
			{
				temp = NULL;
				free (new);
				return;
			}
			else
				if (temp->right == NULL)
				{
					temp->right = new;
					temp = NULL;
				}
				else
					temp = temp->right;
		}
	}
}

int searchFunc(nodeFuncPtr root, char *key, nodeFuncPtr *found)
{
	if (root == NULL)
		return FALSE;
	int find = FALSE;		//FALSE == 0, TRUE ==1
	*found = root;
	while (*found != NULL)
	{
		if (strcmp(key, (*found)->keyName) < 0)	// pokud je hledany mensi
		{
			*found = (*found)->left;
		}
		else if (strcmp(key, (*found)->keyName) > 0)	//pokud je hledany vetsi
		{
			*found = (*found)->right;
		}
		else			//rovnaji se
		{
			find = TRUE;
			break;
		}
	}
	return find;
}

void disposeFuncTree(nodeFuncPtr *root)
{
	if (*root != NULL)
	{
		disposeVarTree(&(*root)->localTable);
		disposeFuncTree(&(*root)->left);
		disposeFuncTree(&(*root)->right);
		free(*root);
		*root = NULL;
	}
}