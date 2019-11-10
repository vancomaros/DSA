#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Header.h"
#pragma GCC optimize ("O1")

int size_of_array = 10;
double lambda = 0;
int elements = 0;
int SUM = 100;

struct Node* RBT = NULL;
struct LEAF *root = NULL;
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void hint() {
	printf("##############\n");
	printf("# v = vlozit #\n");
	printf("# h = hladat #\n");
	printf("# k = koniec #\n");
	printf("##############\n\n");
}

int main() {
	int *array_hash = (int *)malloc(10 * sizeof(int));
	size_of_array = 10;
	int num;
	for (int i = 0; i < size_of_array; i++)
		*(array_hash + i) = -1;
	char act = "";
	hint();
	printf("Akcia: ");
	while (1) {
		scanf(" %c", &act);
		switch (act) {

			case 'v':

				srand(time(0));
				clock_t t, t1, t2, t3;
				t = clock();
				array_hash = insert_hash(array_hash);
				t = clock() - t;
				double time_taken = ((double)t) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do mojej HASH tabulky: %.3f\n", SUM, time_taken);
				/*
				for (int i = 0; i < size_of_array; i++)
					printf("|%d| ", *(array_hash + i));
				*/


				t1 = clock();
				for (int i = 1; i < SUM + 1; i++) {
					num = (rand() % (1000 + 1 - 0) + 1) * (rand() % (100 + 1 - 0) + 1);
					//num = i;
					root = insert_tree(root, num);
				}
				t1 = clock() - t1;
				double time_taken1 = ((double)t1) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do mojho vyvazovacieho stromu: %.3f\n", SUM, time_taken1);
				//preOrder(root);

				t2 = clock();
				NotMyHash();
				t2 = clock() - t2;
				double time_taken2 = ((double)t2) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do cudzej HASH tabulky: %.3f\n", SUM, time_taken2);

				t3 = clock();
				RBT = NotMyTree(RBT);
				t3 = clock() - t3;
				double time_taken3 = ((double)t3) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do cudzieho vyvazovacieho stromu: %.3f\n", SUM, time_taken3);
				break;

			case 'k':
				return 0;
				break;

			case 'h':
				printf("Hladat: ");
				scanf("%d", &num);
				find_hash(array_hash, num);
				search_tree(root, num);

				if (find(num) == 1)
					printf("V cudzom hashi sa %d nachadza.\n", num);
				else printf("V cudzom hashi sa %d nenachadza.\n", num);

				if (search_RBT(RBT, num) == 1)
					printf("V cudzom strome sa %d nachadza.\n", num);
				else printf("V cudzom strome sa %d nenachadza.\n", num);
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