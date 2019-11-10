#include "Header.h"
#include <stdio.h>
#include <stdlib.h>

int SUM;

typedef struct node {
	int name;
	struct node* next;
} node;

#define HASH_MAX 10000000
node* hashtable[HASH_MAX];

unsigned int hash(int sum);
void create(int a);
int find(int a);

int NotMyHash() {
	srand(time(0));
	int num;
	// set pointers to NULL!
	for (int i = 0; i < HASH_MAX; i++)
	{
		hashtable[i] = NULL;
	}

	for (int i = 0; i < SUM; i++) {
		num = (rand() % (1000 + 1 - 0) + 1) * (rand() % (1000 + 1 - 0) + 1);
		//num = i;
		create(num);
	}
	/*for (int i = 0; i < HASH_MAX; i++)
		if (hashtable[i] != NULL)
			printf("|%d| ", hashtable[i]->name);*/
	return 0;
}

unsigned int hash(int sum)
{
	//from Doug L at Harvard's CS50
	return sum % HASH_MAX;
}

void create(int a)
{
	int n = hash(a);
	node* new_node = malloc(sizeof(node));
	if (new_node == NULL)
		exit(1);
	else
	{
		new_node->name = a;
		new_node->next = hashtable[n];
		hashtable[n] = new_node;

	}
}

int find(int a)
{
	int n = hash(a);
	if (hashtable[n] != NULL)
	{
		for (node* trav = hashtable[n]; trav != NULL; trav = trav->next) {
			if (trav->name == a)
				return 1;
		}
	}
	return 0;
}