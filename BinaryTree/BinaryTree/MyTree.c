#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct leaf {
	int value;
	struct leaf *left;
	struct leaf *right;
	int height;
} LEAF;
/*
void delete_tree(LEAF* node) {
	if (node != NULL) {
		delete_tree(node->left);
		delete_tree(node->right);
		node == NULL;
	}
}
*/
void preOrder(LEAF *node) {
	if (node != NULL) {
		printf("%d ", node->value);
		preOrder(node->left);
		preOrder(node->right);
	}
}

struct LEAF *add_tree(int new_value) {
	LEAF *new_component = (LEAF *)malloc(sizeof(LEAF));
	new_component->value = new_value;
	new_component->left = NULL;
	new_component->right = NULL;
	new_component->height = 1;
	return new_component;
}


int height(LEAF *node) {
	int R, L;

	if (node == NULL)
		return 0;
	if (node->right == NULL)
		R = 0;
	else R = node->right->height;

	if (node->left == NULL)
		L = 0;
	else L = node->left->height;

	if (L > R)
		return L + 1;
	else return R + 1;
}

struct LEAF *right_rotation(LEAF *node) {
	LEAF *temp_l = node->left;
	LEAF *temp_r = temp_l->right;

	temp_l->right = node;
	node->left = temp_r;

	node->height = height(node);
	temp_l->height = height(temp_l);

	return temp_l;
}

struct LEAF *left_rotation(LEAF *node) {
	LEAF *temp_r = node->right;
	LEAF *temp_l = temp_r->left;

	temp_r->left = node;
	node->right = temp_l;

	node->height = height(node);
	temp_r->height = height(temp_r);

	return temp_r;

}

struct LEAF *check_tree(LEAF *node, int new_value) {
	int balance = 0;
	if (node->left != NULL && node->right != NULL)
		balance = node->left->height - node->right->height;
	else if (node->left == NULL)
		balance = 0 - node->right->height;
	else if (node->right == NULL)
		balance = node->left->height;

	// Left Left Case
	if (balance > 1 && new_value < node->left->value)
		return right_rotation(node);

	// Right Right Case 
	if (balance < -1 && new_value > node->right->value)
		return left_rotation(node);

	// Left Right Case 
	if (balance > 1 && new_value > node->left->value) {
		node->left = left_rotation(node->left);
		return right_rotation(node);
	}

	// Right Left Case 
	if (balance < -1 && new_value < node->right->value) {
		node->right = right_rotation(node->right);
		return left_rotation(node);
	}
	return node;
}

struct LEAF *insert_tree(LEAF *component, int new_value) {

	if (component == NULL) {
		return add_tree(new_value);
	}

	if (component->value == new_value) {
		return component;
	}

	if (new_value < component->value)
		component->left = insert_tree(component->left, new_value);

	else if (new_value > component->value)
		component->right = insert_tree(component->right, new_value);

	if (component->left == NULL)
		component->height = 1 + component->right->height;
	else if (component->right == NULL)
		component->height = 1 + component->left->height;
	else if (component->right->height > component->left->height)
		component->height = 1 + component->right->height;
	else if (component->right->height <= component->left->height)
		component->height = 1 + component->left->height;

	component = check_tree(component, new_value);
	return component;
}

void search_tree(LEAF *component, int new_value) {

	if (component == NULL) {
//		printf("Prvok %d v mojom strome nebol najdeny.\n", new_value);
		return;
	}

	if (component->value == new_value) {
//		printf("Prvok %d najdeny v mojom strome v hlbke %d.\n", new_value, component->height);
		return;
	}

	if (new_value < component->value)
		search_tree(component->left, new_value);

	else if (new_value > component->value)
		search_tree(component->right, new_value);
}