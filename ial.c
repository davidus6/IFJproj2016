#include "ial.h"
#include <string.h>

void initTree(nodePtr *root)
{
	root = NULL;
}

void insert(nodePtr *root, char *key) //DODELAT kdyz uz tam klic je tak error, nebo check jestli uz bylo blabla
{
	nodePtr new = malloc(sizeof(struct node));
	new->keyName = key;
	//DODELAT ostatni 
	new->left = NULL;
	new->right = NULL;
	if (*root == NULL)
		*root = new;
	else
	{
		nodePtr actual;
		actual = *root;
		while (actual != NULL)
		{
			if (strcmp(new->keyName, actual->keyName) < 0)
			{
				if (actual->left == NULL)
				{
					actual->left = new;
					actual = NULL;
				}
				else
					actual = actual->left;
			}
			else if (strcmp(new->keyName, actual->keyName) == 0)
			{
				actual = NULL;	//DODELAT predelat viz vyse
				free (new);
			}
			else
				if (actual->right == NULL)
				{
					actual->right = new;
					actual = NULL;
				}
				else
					actual = actual->right;
		}
	}
}

int search(nodePtr root, char *key)
{
	int found = FALSE;
	nodePtr active = root;
	while (active != NULL)
	{
		if (strcmp(key, active->keyName) < 0)	// pokud je hledany mensi
		{
			active = active->left;
		}
		else if (strcmp(key, active->keyName) > 0)	//pokud je hledany vetsi
		{
			active = active->right;
		}
		else			//rovnaji se
		{
			found = TRUE;
			break;
		}
	}
	return found;
}

void disposeTree(nodePtr *root)
{
  if(*root != NULL)
  {
	disposeTree(&(*root)->left);
	disposeTree(&(*root)->right);
	free(*root);
  }
}