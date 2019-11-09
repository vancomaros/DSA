#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#pragma GCC optimize ("O1")

int size_of_array = 10;
double lambda = 0;
int elements = 0;
int SUM = 10;


//-------------------------------------------------------------------

typedef struct leaf {
	int value;
	struct leaf *left;
	struct leaf *right;
	int height;
} LEAF;

LEAF *root = NULL;

LEAF *add_tree(int new_value) {
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

void preOrder(LEAF *node)
{
	if (node != NULL) {
		printf("%d ", node->value);
		preOrder(node->left);
		preOrder(node->right);
	}
}



LEAF *left_rotation(LEAF *node) {
	printf("tocime lll\n");
	LEAF *temp_r = node->right;
	LEAF *temp_l = temp_r->left;

	temp_r->left = node;
	node->right = temp_l;

	node->height = height(node);
	temp_r->height = height(temp_r);

	/*
	if (node->left == NULL)
		node->height = 1 + node->right->height;
	else if (node->right == NULL)
		node->height = 1 + node->left->height;
	else if (node->right->height > node->left->height)
		node->height = 1 + node->right->height;
	else if (node->right->height <= node->left->height)
		node->height = 1 + node->left->height;

	if (temp_r->left == NULL)
		temp_r->height = 1 + temp_r->right->height;
	else if (temp_r->right == NULL)
		temp_r->height = 1 + temp_r->left->height;
	else if (temp_r->right->height > temp_r->left->height)
		temp_r->height = 1 + temp_r->right->height;
	else if (temp_r->right->height <= temp_r->left->height)
		temp_r->height = 1 + temp_r->left->height;
		*/
	return temp_r;


}

LEAF *right_rotation(LEAF *node) {
	printf("tocime rrr okolo %d\n", node->value);
	LEAF *temp_l = node->left;
	LEAF *temp_r = temp_l->right;
	
	temp_l->right = node;
	node->left = temp_r;

	node->height = height(node);
	temp_l->height = height(temp_l);
	/*
	if (node->left == NULL)
		node->height = 1 + node->right->height;
	else if (node->right == NULL)
		node->height = 1 + node->left->height;
	else if (node->right->height > node->left->height)
		node->height = 1 + node->right->height;
	else if (node->right->height <= node->left->height)
		node->height = 1 + node->left->height;

	if (temp_l->left == NULL)
		temp_l->height = 1 + temp_l->right->height;
	else if (temp_l->right == NULL)
		temp_l->height = 1 + temp_l->left->height;
	else if (temp_l->right->height > temp_l->left->height)
		temp_l->height = 1 + temp_l->right->height;
	else if (temp_l->right->height <= temp_l->left->height)
		temp_l->height = 1 + temp_l->left->height;
		*/
	return temp_l;
}


LEAF *check_tree(LEAF *node, int new_value) {
	int balance = 0;
	if (node->left != NULL && node->right != NULL)
		balance = node->left->height - node->right->height;
	else if (node->left == NULL)
		balance = 0-node->right->height;
	else if (node->right == NULL)
		balance = node->left->height;

	printf("Rozdiel vysok v hodnote %d je: %d\n\n", node->value, balance);

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


LEAF *insert_tree(LEAF *component, int new_value) {

	if (component == NULL) {
		printf("Prvok %d sa vlozil do stromu.\n", new_value);
		return add_tree(new_value);
	}

	if (component->value == new_value) {
		printf("Prvok %d sa uz v strome nachadza.\n", new_value);
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
		printf("Prvok %d nebol najdeny\n", new_value);
		return;
	}

	if (component->value == new_value) {
		printf("Prvok %d najdeny\n", new_value);
		return;
	}

	if (new_value < component->value)
		search_tree(component->left, new_value);

	else if (new_value > component->value)
		search_tree(component->right, new_value);
}

//-------------------------------------------------------------------

void hint() {
	printf("HELP\n");
}

int hash(int *arr, int value) {
	int index, temp = 0;
	index = value % size_of_array;
	while (++temp) {
		if (index > size_of_array || index < 0) {
			return 1;
			arr = resize_hash(arr);
			temp = 0;
			
		}
		else if (*(arr + index) == -1) {
			*(arr + index) = value;
			lambda = ((double)(++elements / (double)((size_of_array))));
			return 0;
		}
		else index = index + 2 ^ temp - 1;
	}
}

int resize_hash(int *arr) {
	elements = 0;
	int *new_arr;
	size_of_array *= 2;
	new_arr = (int *)malloc(size_of_array * sizeof(int));
	for (int i = 0; i < size_of_array; i++)
		*(new_arr + i) = -1;
	for (int i = 0; i < size_of_array / 2; i++)
		if (*(arr + i) != -1)
			while (hash(new_arr, *(arr + i)) == 1) {
				size_of_array *= 2;
				i = 0;
			}
	free(arr);
	return new_arr;
}

int *insert_hash(int *arr) {
	int num = SUM;
	int value;

	while (num--) {
		value = rand() % (10000) + 10;
		if (lambda >= 0.5)
			arr = resize_hash(arr);
		while (hash(arr, value) == 1)
			arr = resize_hash(arr);
	}
	return arr;
}

void find_hash(int *arr) {
	int value, index, temp = 0;
	scanf("%d", &value);
	index = value % size_of_array;
	search_tree(root, value);
	while (1) {
		if (index < size_of_array && value == *(arr + index)) {
			printf("Je na indexe %d\n", index);
			printf("%d", *(arr + index));
			return;
		}
		else if (index > size_of_array || *(arr + index) == -1) {
			printf("%d tu nie je.", value);
			return;
			}
		index += 2 ^ ++temp - 1;
	}
}



int main() {
	int *array_hash = (int *)malloc(10 * sizeof(int));
	size_of_array = 10;
	int num;
	for (int i = 0; i < size_of_array; i++)
		*(array_hash + i) = -1;
	char act = "";
	printf("Akcia: ");
	while (1) {
		scanf(" %c", &act);
		switch (act) {

			case 'v':

				srand(time(0));
				clock_t t;
				t = clock();
				array_hash = insert_hash(array_hash);
				t = clock() - t;
				double time_taken = ((double)t) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov: %f\n", SUM, time_taken);
				
				for (int i = 0; i < size_of_array; i++)
					printf("|%d| ", *(array_hash + i));

				printf("Prvok do stromu?\n");
				scanf("%d", &num);
				root = insert_tree(root, num);
				preOrder(root);
				break;

			case 'k':
				return 0;
				break;

			case 'h':
				find_hash(array_hash);
				break;

			default: 
				printf("Neplatny vstup! \n");
				hint();
			}

		printf("\n");
		printf("Akcia: ");
	}

	getchar();
	return 0;
}