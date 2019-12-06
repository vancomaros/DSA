#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define OBJECTIONABLE -1

struct Node {
	short y, x, z;
	short prev_x, prev_y, prev_z;
	char type;
	short price;
};


struct Node heap[25000];
int heap_count = 0;
short ports[10][300];

// vypise heap
void print_heap() {
	printf("-----------------------------------------------\n");
	printf("vypis heapu:\n");
	for (int i = 0; i < heap_count; i++) {
		printf("heap[%d] = {%d, %d, ", i, heap[i].y, heap[i].x);
		printf("%d, %c},", heap[i].price, heap[i].type);
		printf("\n");
	}
	printf("-----------------------------------------------\n");
	return;
}


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
	if (heap[parent_index].price > heap[index].price) {
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
	if (left != OBJECTIONABLE && heap[left].price < heap[parent].price)
		max = left;
	else
		max = parent;
	if (right != OBJECTIONABLE && heap[right].price < heap[max].price)
		max = right;

	if (max != parent) {
		swap_heap(max, parent);
		heapify_top_bottom(max);
	}
	return;
}


void insert_heap(struct Node block) {
	heap[heap_count] = block;
	heapify_bottom_top(heap_count);
	heap_count++;
}

struct Node delete_heap() {

	struct Node del = heap[0];
	heap[0] = heap[--heap_count];
	heap[heap_count].y = 0;
	heap[heap_count].x = 0;
	heap[heap_count].z = 0;
	heap[heap_count].price = 0;
	heap[heap_count].price = 0;
	heap[heap_count].type = ' ';
	heapify_top_bottom(0);
	return del;
}


void cesta(struct Node curr, struct Node ***map) {
	int arr[50000];
	int counter = 0;
	int length = curr.price;
	while (curr.x != 0 || curr.y != 0) {
		arr[counter] = curr.y;
		counter++;
		arr[counter] = curr.x;
		counter++;
		curr = map[curr.prev_x][curr.prev_y][curr.prev_z];
	}
	printf("\n\n0 0\n");
		for (int i = counter/2 - 1; i >= 0; i--) {
			printf("%d ", arr[--counter]);
			printf("%d\n", arr[--counter]);
		}
}



// jebnem ho do mapy, ak sa da
void insert_to_map(struct Node place, struct Node ***map) {
	short x = place.x;
	short y = place.y;
	short z = place.z;

	if (map[x][y][z].price > place.price) 
		map[x][y][z] = place;
}


void make_block(char **original, struct Node ***map, struct Node top_heap, int x, int y, int z) {

	map[x][y][z].x = x;
	map[x][y][z].y = y;
	map[x][y][z].z = z;
	map[x][y][z].type = original[x][y];
	if (top_heap.type == 'H')
		map[x][y][z].price = top_heap.price + 2;
	else map[x][y][z].price = top_heap.price + 1;
	map[x][y][z].prev_x = top_heap.x;
	map[x][y][z].prev_y = top_heap.y;
	map[x][y][z].prev_z = top_heap.z;

}

void teleport(char **original, struct Node ***map, struct Node top_heap, int max_x, int max_y) {
	short num = 0, index = 0, x, y, z;
	z = top_heap.z;

	num = top_heap.type - '0';
		while (ports[num][index] != -1) {
			x = ports[num][index];
			y = ports[num][index+1];
			if (y - 1 >= 0 && original[x][y - 1] != 'N') {
				if (map[x][y - 1][z].price > top_heap.price) {
					make_block(original, map, top_heap, x, y - 1, z);
					insert_heap(map[x][y - 1][z]);
				}
			}

			if (x + 1 < max_x && original[x + 1][y] != 'N') {
				if (map[x + 1][y][z].price > top_heap.price) {
					make_block(original, map, top_heap, x + 1, y, z);
					insert_heap(map[x + 1][y][z]);
				}
			}

			if (y + 1 < max_y && original[x][y + 1] != 'N') {
				if (map[x][y + 1][z].price > top_heap.price) {
					make_block(original, map, top_heap, x, y + 1, z);
					insert_heap(map[x][y + 1][z]);
				}
			}

			if (x - 1 >= 0 && original[x - 1][y] != 'N') {
				if (map[x - 1][y][z].price > top_heap.price) {
					make_block(original, map, top_heap, x - 1, y, z);
					insert_heap(map[x - 1][y][z]);
				}
			}
			index += 2;
		}
}


int where_am_i(struct Node top_heap) {

	if (top_heap.type == 'D') {
		top_heap.z += 1;
		return 0;
	}
	if (top_heap.type == 'G') {
		top_heap.z += 2;
		return 0;
	}
	if (top_heap.type == 'P' && top_heap.z % 2) {
		top_heap.z += 4;
		if (top_heap.z < 4)
			top_heap.z += 4;
		if (top_heap.z < 8)
			top_heap.z += 8;
		if (top_heap.z >= 13)
			return 1;
	}
	if (top_heap.type >= '0' && top_heap.type <= '9') {
		if (top_heap.z >= 2)
			return 2;
	}
	return 0;
}


// dick
int dijkstra(char **original, struct Node ***map, int m, int n, int princess) {
	struct Node top_heap;
	int max_x = m;
	int max_y = n;

	while (!0) {
		top_heap = delete_heap();
		short x = top_heap.x;
		short y = top_heap.y;
		short z = top_heap.z;

		insert_to_map(top_heap, map);
		if (where_am_i(top_heap) == 1);

		if (where_am_i(top_heap) == 2)
			teleport(original, map, top_heap, m, n);

// tu to vypni ked chces koniec ty kokot
		if (top_heap.type == 'D') {
			top_heap.z += 1;
			cesta(top_heap, map);
			return top_heap.price;
		}

		if (y - 1 >= 0 && original[x][y-1] != 'N') {
			if (map[x][y - 1][z].price > top_heap.price) {
				make_block(original, map, top_heap, x, y - 1, z);
				insert_heap(map[x][y - 1][z]);
				}
			}
		
		if (x + 1 < max_x && original[x+1][y] != 'N') {
			if (map[x + 1][y][z].price > top_heap.price) {
				make_block(original, map, top_heap, x + 1, y, z);
				insert_heap(map[x + 1][y][z]);
				}
			}
			
		if (y + 1 < max_y && original[x][y+1] != 'N') {
			if (map[x][y + 1][z].price > top_heap.price) {
				make_block(original, map, top_heap, x, y + 1, z);
				insert_heap(map[x][y + 1][z]);
				}
			}

		if (x - 1 >= 0 && original[x-1][y] != 'N') {
			if (map[x - 1][y][z].price > top_heap.price) {
				make_block(original, map, top_heap, x - 1, y, z);
				insert_heap(map[x - 1][y][z]);
			}
		}
	}
}

int find_teleports(char **mapa, int n, int m) {
	int princess = 0;
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 50; j++)
			ports[i][j] = -1;
	int num, index = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if ((*(*(mapa + i) + j)) == 'P')
				princess++;
			if ((*(*(mapa + i) + j)) >= '0' && (*(*(mapa + i) + j)) <= '9') {
				num = mapa[i][j] - '0';
				while (ports[num][index] != -1)
					index += 2;
				*(*(ports + num) + index) = i;
				*(*(ports + num) + index+1) = j;
			}
		}
	}
	return princess;
}


// funkcia zachrani princezne
void zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {

	int princess = find_teleports(mapa, n, m);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			printf("%c ", *(*(mapa + i) + j));
		}
		printf("\n");
	}
/*	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%d ", *(*(ports + i) + j));
		}
		printf("\n");
	}*/


	struct Node ***arr;
	arr = alloc_data(m, n, 16);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < 16; k++)
				arr[i][j][k].price = 600;

	
	struct Node start;
	start.x = 0;
	start.y = 0;
	start.z = 0;
	start.price = 0;
	start.type = mapa[0][0];

	insert_heap(start); 
	dlzka_cesty = dijkstra(mapa, arr, m, n, princess);


	printf("\n%d\n", dlzka_cesty);
}

int main() {
	int j = 0;

	char mapa[10][5] = { {'C','8','C','H','N'},
					   {'H','G','C','H','0'},
					   {'H','C','C','N','C'},
					   {'N','C','N','N','C'},
					   {'H','P','H','H','C'},
					   {'C','N','P','0','N'},
					   {'H','0','H','N','C'},
					   {'C','N','C','8','C'},
					   {'H','C','H','P','C'},
					   {'4','H','C','4','D'} };

	int vyska = 10;
	int sirka = 5;

	char **mapka;
	mapka = (char**)malloc(vyska * sizeof(char*));
	for (int i = 0; i < vyska; ++i) {
		mapka[i] = (char*)malloc(sirka * sizeof(char));
	}

	for (int j = 0; j < vyska; ++j) {
		for (int i = 0; i < vyska; ++i) {
			mapka[j][i] = mapa[j][i];
		}
	}

	zachran_princezne(mapka, 5, 10, 0, &j);
	system("pause");
	return 0;
}

