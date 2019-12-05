#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define OBJECTIONABLE -1

struct Node {
	short n;
	short m;
	char type;
	short price;
};


struct Node heap[25000];
int heap_count = 0;


int ***alloc_data(int xlen, int ylen, int zlen) {
	int ***p;
	int i, j;

	p = malloc(xlen * sizeof *p);

	for (i = 0; i < xlen; ++i)
		p[i] = NULL;

	for (i = 0; i < xlen; ++i)
		p[i] = malloc(ylen * sizeof *p[i]);

	for (i = 0; i < xlen; ++i)
		for (j = 0; j < ylen; ++j)
			p[i][j] = NULL;

	for (i = 0; i < xlen; ++i)
		for (j = 0; j < ylen; ++j)
			p[i][j] = malloc(zlen * sizeof *p[i][j]);

	return p;
}


void swap_heap(int a, int b) {
	struct Node temp = heap[a];
	heap[a] = heap[b];
	heap[b] = temp;
}

void heapify_bottom_top(int index) {
	int parent_index = (index - 1) / 2;
	if (heap[parent_index].value < heap[index].value) {
		swap_heap(parent_index, index);
		heapify_bottom_top(parent_index);
	}
}


void heapify_top_bottom(int parent) {

	int left = parent * 2 + 1;
	int right = parent * 2 + 2;

	if (left >= heap_count || left < 0)
		left = OBJECTIONABLE;
	if (right >= heap_count || right < 0)
		right = OBJECTIONABLE;

	int max;
	if (left != OBJECTIONABLE && heap[left].value > heap[parent].value)
		max = left;
	else
		max = parent;
	if (right != OBJECTIONABLE && heap[right].value > heap[max].value)
		max = right;

	if (max != parent) {
		swap_heap(max, parent);
		heapify_top_bottom(max);
	}
	return;
}


void insert_heap(struct Node block) {
	struct Node new_node;
	new_node.m = block.m;
	new_node.n = block.n;
	new_node.price = block.price;
	new_node.type = block.type;

	heap[heap_count] = new_node;
	heapify_bottom_top(heap_count);
	heap_count++;
}

char *delete_heap() {
	if (heap_count == 0) {
		printf("\nHeap is empty!\n");
		return 0;
	}

	char *del = malloc(50 * sizeof(char));
	strcpy(del, heap[0].name);
	heap[0] = heap[--heap_count];
	strcpy(heap[heap_count].name, "");
	heap[heap_count].value = 0;
	heapify_top_bottom(0);
	return del;
}



int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%c ", *(*(mapa + i) + j));
		}
		printf("\n");
	}

	struct Node ***arr;
	arr = alloc_data(n, n, 32);
	
	arr[0][0][0].n = 0;
	arr[0][0][0].m = 0;
	arr[0][0][0].price = 0;
	arr[0][0][0].type = 0;
	arr[0][0][0].type = mapa[0][0];


	insert_heap(*(*(*arr))); // heh

	return NULL;
}

int main() {
	int j = 0;
	
	char **mapa = (char *)malloc(5 * sizeof(char));
	for (int i = 0; i < 5; i++)
		mapa[i] = (char *)malloc(5 * sizeof(char));
	

	*(*(mapa + 0) + 0) = 'P';
	*(*(mapa + 0) + 1) = 'P';
	*(*(mapa + 0) + 2) = 'H';
	*(*(mapa + 0) + 3) = 'H';
	*(*(mapa + 0) + 4) = 'H';
	*(*(mapa + 1) + 0) = 'H';
	*(*(mapa + 1) + 1) = 'C';
	*(*(mapa + 1) + 2) = 'H';
	*(*(mapa + 1) + 3) = 'H';
	*(*(mapa + 1) + 4) = 'C';
	*(*(mapa + 2) + 0) = 'C';
	*(*(mapa + 2) + 1) = 'C';
	*(*(mapa + 2) + 2) = 'H';
	*(*(mapa + 2) + 3) = 'C';
	*(*(mapa + 2) + 4) = 'C';
	*(*(mapa + 3) + 0) = 'C';
	*(*(mapa + 3) + 1) = 'H';
	*(*(mapa + 3) + 2) = 'C';
	*(*(mapa + 3) + 3) = 'H';
	*(*(mapa + 3) + 4) = 'C';
	*(*(mapa + 4) + 0) = 'H';
	*(*(mapa + 4) + 1) = 'P';
	*(*(mapa + 4) + 2) = 'C';
	*(*(mapa + 4) + 3) = 'D';
	*(*(mapa + 4) + 4) = 'C';


	zachran_princezne(mapa, 5, 5, 0, &j);
	getchar();
	return 0;
}




/*


int main() {
	char act[5];
	char name[100];
	int value;
	scanf("%s", act);
	do {
		if (act[1] == 'l') {
			scanf("%s", name);
			scanf("%d", &value);
			insert_heap(name, value);
		}
		if (act[1] == 'y') {
			printf("%s\n", delete_heap());
		}
	} while (scanf("%s", act) > 0);

	return 0;
}
*/