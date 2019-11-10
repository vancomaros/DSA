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
int SUM = 1000000;

struct Node* RBT = NULL;
struct LEAF *root = NULL;
struct ITEM *root_bvl = NULL;
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void hint() {
	printf("#################\n");
	printf("# v = vlozit    #\n");
	printf("# h = hladat 1  #\n");
	printf("# a = hladat al #\n");
	printf("# k = koniec    #\n");
	printf("# u = vypisy    #\n");
	printf("# m = moj vstup #\n");
//	printf("# f = vymazanie #\n");
	printf("#################\n\n");
}

int main() {
	int *array_hash = (int *)malloc(10 * sizeof(int));
	int *pom = (int *)malloc(SUM * sizeof(int));
	size_of_array = 10;
	int num, s, p;
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
				clock_t t, t1, t2, t3, t4, t5, t6, t7, t8, t9;
				t = clock();
				array_hash = insert_hash(array_hash);
				t = clock() - t;
				double time_taken = ((double)t) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do mojej HASH tabulky: %.3f\n", SUM, time_taken);

				t1 = clock();
				for (int i = 1; i < SUM + 1; i++) {
					num = (rand() % (1000 + 1 - 0) + 1) * (rand() % (100 + 1 - 0) + 1);
					*(pom + i - 1) = num;
					//num = i;
					root = insert_tree(root, num);
				}
				t1 = clock() - t1;
				double time_taken1 = ((double)t1) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do mojho vyvazovacieho stromu: %.3f\n", SUM, time_taken1);
				//preOrder(root);

				t2 = clock();
				NotMyHash(pom);
				t2 = clock() - t2;
				double time_taken2 = ((double)t2) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do cudzej HASH tabulky: %.3f\n", SUM, time_taken2);


				t3 = clock();
				RBT = NotMyTree(RBT, pom);
				t3 = clock() - t3;
				double time_taken3 = ((double)t3) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do cudzieho vyvazovacieho stromu: %.3f\n", SUM, time_taken3);
				//preorder(RBT);

				t4 = clock();
				for (int i = 1; i < SUM + 1; i++) {
					//num = (rand() % (1000 + 1 - 0) + 1) * (rand() % (1000 + 1 - 0) + 1);
					//num = i;
					num = *(pom + i);
					root_bvl = insert_tree_bvl(root_bvl, num);
				}
				t4 = clock() - t4;
				double time_taken4 = ((double)t4) / CLOCKS_PER_SEC;
				printf("\n Cas vkladania %d prvkov do mojho nevyvazovacieho stromu: %.3f\n", SUM, time_taken4);
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

				search_tree_bvl(root_bvl, num);
				break;

			case 'u':
				printf("Moj HASH: \n");
				for (int i = 0; i < size_of_array; i++)
					printf("|%d| ", *(array_hash + i));

				printf("\n\nMoj STROM: \n");

				preOrder(root);

				printf("\n\nCudzi HASH: \n");

				read();

				printf("\n\nCudzi STROM: \n");

				preorder(RBT);

				printf("\n\n");
				break;
			case 'm':
				printf("Kolko prvkov?.. ");
				scanf(" %d", &s);
				for (int i = 0; i < s; i++) {
					printf("%d. prvok: ", i + 1);
					scanf(" %d", &p);

					array_hash = insert_my_hash(array_hash, p);
					root = insert_tree(root, p);
					create(p);
					if (BST_search(RBT, p) == NULL)
						RBT = RB_insert(RBT, p);
					root_bvl = insert_tree_bvl(root_bvl, p);
				}
				break;
	/*		case 'f':
				free(array_hash);
				array_hash = (int *)malloc(10 * sizeof(int));
				size_of_array = 0;
				delete_tree(root);
				break;
				*/
			case 'a':
				t5 = clock();
				for (int i = 0; i < SUM; i++)
					find_hash(array_hash, *(pom + i));
				t5 = clock() - t5;
				double time_taken5 = ((double)t5) / CLOCKS_PER_SEC;
				printf("\n Cas hladania %d prvkov v mojej HASH tabulke: %.3f\n", SUM, time_taken5);

				t6 = clock();
				for (int i = 0; i < SUM; i++)
					search_tree(root, *(pom + i));
				t6 = clock() - t6;
				double time_taken6 = ((double)t6) / CLOCKS_PER_SEC;
				printf("\n Cas hladania %d prvkov v mojom strome: %.3f\n", SUM, time_taken6);

				t7 = clock();
				for (int i = 0; i < SUM; i++)
					if (find(*(pom + i)) == 1)
						continue;
//						printf("V cudzom hashi sa %d nachadza.\n", *(pom + i));
//					else printf("V cudzom hashi sa %d nenachadza.\n", *(pom + i));
					else continue;
				t7 = clock() - t7;
				double time_taken7 = ((double)t7) / CLOCKS_PER_SEC;
				printf("\n Cas hladania %d prvkov v cudzom strome: %.3f\n", SUM, time_taken7);

				t8 = clock();
				for (int i = 0; i < SUM; i++)
					if (search_RBT(RBT, *(pom + i)) == 1)
						continue;
//						printf("V cudzom strome sa %d nachadza.\n", *(pom + i));
//				else printf("V cudzom strome sa %d nenachadza.\n", *(pom + i));
					else continue;
				t8 = clock() - t8;
				double time_taken8 = ((double)t8) / CLOCKS_PER_SEC;
				printf("\n Cas hladania %d prvkov v cudzom strome: %.3f\n", SUM, time_taken8);

				t9 = clock();
				for (int i = 0; i < SUM; i++)
					search_tree_bvl(root_bvl, *(pom + i));
				t9 = clock() - t9;
				double time_taken9 = ((double)t9) / CLOCKS_PER_SEC;
				printf("\n Cas hladania %d prvkov v cudzom strome: %.3f\n", SUM, time_taken9);

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