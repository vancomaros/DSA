#include "Header.h"
#include <stdio.h>
#include <stdlib.h>

int SUM;
//CLRS 
//Insertion and Deletion in a Red Black Tree
enum type { RED, BLACK };

struct Node
{
	int data;
	struct Node* left;
	struct Node* right;
	struct Node* parent;
	enum type color;
};

void LeftRotate(struct Node** T, struct Node** x)
{
	struct Node* y = (*x)->right;
	(*x)->right = y->left;

	if (y->left != NULL)
		y->left->parent = *x;

	y->parent = (*x)->parent;

	if ((*x)->parent == NULL)
		*T = y;

	else if (*x == (*x)->parent->left)
		(*x)->parent->left = y;

	else
		(*x)->parent->right = y;

	y->left = *x;

	(*x)->parent = y;

}
void RightRotate(struct Node** T, struct Node** x)
{
	struct Node* y = (*x)->left;
	(*x)->left = y->right;

	if (y->right != NULL)
		y->right->parent = *x;

	y->parent = (*x)->parent;

	if ((*x)->parent == NULL)
		*T = y;

	else if ((*x) == (*x)->parent->left)
		(*x)->parent->left = y;

	else
		(*x)->parent->right = y;

	y->right = *x;
	(*x)->parent = y;

}

void RB_insert_fixup(struct Node** T, struct Node** z)
{
	struct Node* grandparent = NULL;
	struct Node* parentpt = NULL;

	while (((*z) != *T) && ((*z)->color != BLACK) && ((*z)->parent->color == RED))
	{
		parentpt = (*z)->parent;
		grandparent = (*z)->parent->parent;

		if (parentpt == grandparent->left)
		{
			struct Node* uncle = grandparent->right;

			if (uncle != NULL && uncle->color == RED)
			{
				grandparent->color = RED;
				parentpt->color = BLACK;
				uncle->color = BLACK;
				*z = grandparent;
			}

			else
			{
				if ((*z) == parentpt->right)
				{
					LeftRotate(T, &parentpt);
					(*z) = parentpt;
					parentpt = (*z)->parent;
				}

				RightRotate(T, &grandparent);
				parentpt->color = BLACK;
				grandparent->color = RED;
				(*z) = parentpt;
			}
		}

		else
		{
			struct Node* uncle = grandparent->left;

			if (uncle != NULL && uncle->color == RED)
			{
				grandparent->color = RED;
				parentpt->color = BLACK;
				uncle->color = BLACK;
				(*z) = grandparent;
			}

			else
			{
				if ((*z) == parentpt->left)
				{
					RightRotate(T, &parentpt);
					(*z) = parentpt;
					parentpt = (*z)->parent;
				}

				LeftRotate(T, &grandparent);
				parentpt->color = BLACK;
				grandparent->color = RED;
				(*z) = parentpt;
			}
		}
	}
	(*T)->color = BLACK;

}

struct Node* RB_insert(struct Node* T, int data)
{
	struct Node* z = (struct Node*)malloc(sizeof(struct Node));
	z->data = data;
	z->left = NULL;
	z->right = NULL;
	z->parent = NULL;
	z->color = RED;

	struct Node* y = NULL;
	struct Node* x = T;//root

	while (x != NULL)
	{
		y = x;
		if (z->data < x->data)
			x = x->left;

		else
			x = x->right;
	}
	z->parent = y;

	if (y == NULL)
		T = z;

	else if (z->data < y->data)
		y->left = z;

	else
		y->right = z;

	RB_insert_fixup(&T, &z);

	return T;
}

void preorder(struct Node* root)
{
	if (root == NULL)
		return;

	printf("%d ", root->data);
	preorder(root->left);
	preorder(root->right);
}

int	search_RBT(struct Node* root, int x)
{
	if (root == NULL)
		return 0; 
	if (root->data == x)
		return 1;

	if (root->data > x)
		return  search_RBT(root->left, x);
	else
		return search_RBT(root->right, x);
}


struct Node* BST_search(struct Node* root, int x)
{
	if (root == NULL || root->data == x)
		return root;

	if (root->data > x)
		return  BST_search(root->left, x);
	else
		return BST_search(root->right, x);
}

struct Node* NotMyTree(struct Node* RBT, int *arr)
{
	int num;

	for (int i = 0; i < SUM; i++) {
		//num = (rand() % (1000 + 1 - 0) + 1) * (rand() % (1000 + 1 - 0) + 1);
		num = *(arr + i);
		if (BST_search(RBT, num) != NULL)
			continue;
		RBT = RB_insert(RBT, num);
	}
	return RBT;
}