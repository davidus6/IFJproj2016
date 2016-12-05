#include "ial.h"
#include <string.h>

void initClassTree(nodeClassPtr *root)
{
	root = NULL;
}

void insertClass(nodeClassPtr *root, char *key) //DODELAT kdyz uz tam klic je tak error, nebo check jestli uz bylo blabla
{
	nodeClassPtr new = malloc(sizeof(struct nodeClass));
	new->keyName = key;
	new->inner = NULL;
	initInnerTree (&(new->inner));
	new->left = NULL;
	new->right = NULL;
	if (*root == NULL)
		*root = new;
	else
	{
		nodeClassPtr actual;
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
				actual = NULL;
				free (new);
				exit(3);	//DODELAT printf error mozna neco uvolnit
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

int searchClass(nodeClassPtr root, char *key)
{
	int found = FALSE;		//FALSE == 0, TRUE ==1
	nodeClassPtr active = root;
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

void disposeClassTree(nodeClassPtr *root)
{
  if(*root != NULL)
  {
  	disposeInnerTree(&(*root)->inner);
	disposeClassTree(&(*root)->left);
	disposeClassTree(&(*root)->right);
	free(*root);
	*root = NULL;
  }
}


void initInnerTree(nodeInnerPtr *root)
{
	root = NULL;
}
void insertInner(nodeInnerPtr *root, nodeTypes type, char *key, void *data)
{
	nodeInnerPtr new = malloc(sizeof(struct nodeInner));
	new->nodeType = type;
	new->keyName = key;
	new->data = data;
	new->left = NULL;
	new->right = NULL;
	if (*root == NULL)
		*root = new;
	else
	{
		nodeInnerPtr actual;
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
				actual = NULL;	//DODELAT predelat viz vyse, vkladam do tabulky neco co uz tam je
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

int searchInner(nodeInnerPtr root, char *key)
{
	int found = FALSE;
	nodeInnerPtr active = root;
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



void disposeInnerTree(nodeInnerPtr *root)
{
	if(*root != NULL)
	{
	disposeInnerTree(&(*root)->left);
	disposeInnerTree(&(*root)->right);
	free(*root);
	*root = NULL;
	}
}


void initLocalTree(nodeLocalPtr *root)
{
	root = NULL;
}
void insertLocal(nodeLocalPtr *root, char * key, dataVar data)
{
	nodeLocalPtr new = malloc(sizeof(struct nodeLocal));
	new->keyName = key;
	new->data = data;
	new->left = NULL;
	new->right = NULL;
	if (*root == NULL)
		*root = new;
	else
	{
		nodeLocalPtr actual;
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
				actual->data = new->data;	//DODELAT predelat?
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
int searchLocal(nodeLocalPtr root, char * key)
{
	int found = FALSE;
	nodeLocalPtr active = root;
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

void disposeLocalTree(nodeLocalPtr *root)
{
	if(*root != NULL)
	{
	disposeLocalTree(&(*root)->left);
	disposeLocalTree(&(*root)->right);
	free(*root);
	*root = NULL;
	}
}