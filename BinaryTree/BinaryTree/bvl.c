#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct leaf {
	int value;
	struct ITEM *left;
	struct ITEM *right;
} ITEM;

struct ITEM *add_tree_bvl(int value) {
	ITEM *new_component = malloc(sizeof(ITEM));
	new_component->value = value;
	new_component->left = NULL;
	new_component->right = NULL;
	return new_component;
}

struct ITEM *insert_tree_bvl(ITEM *component, int new_value) {

	if (component == NULL) {
		return add_tree_bvl(new_value);
	}

	if (component->value == new_value) {
		return component;
	}

	if (new_value < component->value)
		component->left = insert_tree_bvl(component->left, new_value);

	else if (new_value > component->value)
		component->right = insert_tree_bvl(component->right, new_value);

	return component;
}

void search_tree_bvl(ITEM *component, int new_value) {

	if (component == NULL) {
//		printf("V mojom nevyvazenom strome sa %d nenachadza.\n", new_value);
		return;
	}

	if (component->value == new_value) {
//		printf("V mojom nevyvazenom strome sa %d nachadza.\n", new_value);
		return;
	}

	if (new_value < component->value)
		search_tree_bvl(component->left, new_value);

	else if (new_value > component->value)
		search_tree_bvl(component->right, new_value);
}