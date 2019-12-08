#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define OBJECTIONABLE -1

struct Node {
	short x, y, z;
	short prev_x, prev_y, prev_z;
	char type;
	short price;
};


struct Node heap[10000];
int heap_count = 0;
short ports[10][300];
int path[3000];


struct Node ***alloc_data(int xlen, int ylen, int zlen) {
	struct Node ***p;
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
	if (heap[0].type == ' ')
		printf("Prazdny heap - nie je riesenie\n");

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

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int *cesta(struct Node curr, struct Node ***map) {
	int arr[3500];
	int counter = 0, temp = 0;

	while (1) {
		if (curr.x != curr.prev_x || curr.y != curr.prev_y) {
			arr[counter] = curr.y;
			counter++;
			arr[counter] = curr.x;
			counter++;
		}
		if (curr.x == 0 && curr.y == 0 && (!((curr.prev_x == 0 && curr.prev_y == 1) || (curr.prev_x == 1 && curr.prev_y == 0))))
			break;
		curr = map[curr.prev_x][curr.prev_y][curr.prev_z];
	}

	for (int i = counter / 2 - 1; i >= 0; i--) {
		//	printf("\n%d ", arr[counter-2]);
		path[temp++] = arr[--counter];
		//	printf("%d", arr[counter-1]);
		path[temp++] = arr[--counter];
	}
	return path;
}



// jebnem ho do mapy, ak sa da
void insert_to_map(struct Node place, struct Node ***map) {
	short x = place.x;
	short y = place.y;
	short z = place.z;

	if (map[x][y][z].price > place.price)
		map[x][y][z] = place;
}

// Vyvori sa novy blok, urcia sa mu parametre a nasledne sa v dalsej funkcii hodi do heapu
void make_block(char **original, struct Node ***map, struct Node top_heap, int x, int y, int z) {

	map[x][y][z].x = x;
	map[x][y][z].y = y;
	map[x][y][z].z = z;
	map[x][y][z].type = original[x][y];
	//	if (top_heap.type == 'H')
	if (original[x][y] == 'H')
		map[x][y][z].price = top_heap.price + 2;
	else map[x][y][z].price = top_heap.price + 1;
	map[x][y][z].prev_x = top_heap.x;
	map[x][y][z].prev_y = top_heap.y;
	map[x][y][z].prev_z = top_heap.z;
}


// Fukncia posle do heapu vsetkych susedov teleportov s rovnakym cislom, ktori splnaju podmienky na vlozenie do heapu
void teleport(char **original, struct Node ***map, struct Node top_heap, int max_x, int max_y) {
	short num = 0, index = 0, x, y, z;

	num = top_heap.type - '0';
	//	char char_num = num;
	//	printf("%c", char_num);
	while (ports[num][index] != -1) {
		z = top_heap.z;
		x = ports[num][index];
		y = ports[num][index + 1];

		if (map[x][y][z].price > top_heap.price) {
			//			if(map[x][y][z].type == char_num) {
			make_block(original, map, top_heap, x, y, z);
			insert_heap(map[x][y][z]);
		}
		//		}
		/*		if (y - 1 >= 0 && original[x][y - 1] != 'N') {
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
				}*/
		index += 2;
	}
}


// ak sa nasiel drak, generator alebo princezna, zmeni sa dimenzia, zvacsi sa o 'i'
struct Node change_dimension(struct Node ***map, struct Node top_heap, short i) {
	top_heap.z += i;
	short x = top_heap.x;
	short y = top_heap.y;
	short z = top_heap.z;
	map[x][y][z].prev_x = top_heap.x;
	map[x][y][z].prev_y = top_heap.y;
	map[x][y][z].prev_z = top_heap.z - i;
	map[x][y][z].price = top_heap.price;
	map[x][y][z].x = top_heap.x;
	map[x][y][z].y = top_heap.y;
	map[x][y][z].z = top_heap.z;
	map[x][y][z].type = top_heap.type;
	return top_heap;
}

// funckia zisti, na akom poli v mape sa najnovsie vybrany blok z heapu nachadza
struct Node where_am_i(struct Node top_heap, struct Node ***map) {

	if (top_heap.type == 'D' && (top_heap.z == 0 || top_heap.z == 2)) {
		return change_dimension(map, top_heap, 1);

	}

	if (top_heap.type == 'G' && (top_heap.z == 0 || top_heap.z == 1 || top_heap.z == 5 || top_heap.z == 13)) {
		return change_dimension(map, top_heap, 2);
	}


	// kontrola princezien, kazda je oznacena inak
	if (top_heap.type == 'P' && top_heap.z % 2) {
		//	if (top_heap.z < 4 || top_heap.z == 9 || top_heap.z == 11 || top_heap.z == 17 || top_heap.z == 25 || top_heap.z == 27) {
		if (!((top_heap.z & 4) == 4)) {
			//printf("Mam P v dimenzii %d\n", top_heap.z);
			return change_dimension(map, top_heap, 4);
		}
	}

	if (top_heap.type == 'O' && top_heap.z % 2) {
		//if (top_heap.z < 8 || top_heap.z == 17 || top_heap.z == 19 || top_heap.z == 21 || top_heap.z == 23) {
		if (!((top_heap.z & 8) == 8)) {
			//printf("Mam O v dimenzii %d\n", top_heap.z);
			return change_dimension(map, top_heap, 8);
		}
	}

	if (top_heap.type == 'Q' && top_heap.z % 2) {
		if (top_heap.z < 16) {
			//printf("Mam Q v dimenzii %d\n", top_heap.z);
			return change_dimension(map, top_heap, 16);
		}
	}
	return top_heap;
}


// dick
int *dijkstra(char **original, struct Node ***map, int m, int n, int princess) {
	struct Node top_heap;
	int max_sirka = m;
	int max_vyska = n;
	int dimension = 1;

	for (int i = princess + 2; i > 0; i--)
		dimension *= 2;
	dimension -= 3;

	while (!0) {
		top_heap = delete_heap();
		//printf("Vybral som z heapu %d %d %d\n", top_heap.x, top_heap.y, top_heap.z);


		insert_to_map(top_heap, map);
		top_heap = where_am_i(top_heap, map);
		if (top_heap.type >= '0' && top_heap.type <= '9')
			if ((top_heap.z & 2) == 2)
				teleport(original, map, top_heap, m, n);

		short vys = top_heap.x;
		short sir = top_heap.y;
		short z = top_heap.z;

		// tu to vypni ked chces koniec
		if (top_heap.type == 'P' || top_heap.type == 'O' || top_heap.type == 'Q') {
			if (top_heap.z >= dimension) {
				return cesta(top_heap, map);
				//return top_heap.price;
			}
		}

		if (sir - 1 >= 0 && original[vys][sir - 1] != 'N') {
			if (map[vys][sir - 1][z].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, vys, sir - 1, z);
				insert_heap(map[vys][sir - 1][z]);
			}
		}

		if (vys + 1 < max_vyska && original[vys + 1][sir] != 'N') {
			if (map[vys + 1][sir][z].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, vys + 1, sir, z);
				insert_heap(map[vys + 1][sir][z]);
			}
		}

		if (sir + 1 < max_sirka && original[vys][sir + 1] != 'N') {
			if (map[vys][sir + 1][z].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, vys, sir + 1, z);
				insert_heap(map[vys][sir + 1][z]);
			}
		}

		if (vys - 1 >= 0 && original[vys - 1][sir] != 'N') {
			if (map[vys - 1][sir][z].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, vys - 1, sir, z);
				insert_heap(map[vys - 1][sir][z]);
			}
		}
	}
}


int find_teleports(char **mapa, int vyska, int sirka) {
	int princess = 0;
	char sign = 'O';
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 300; j++)
			ports[i][j] = -1;
	int num, index = 0;
	for (int i = 0; i < vyska; i++) {
		for (int j = 0; j < sirka; j++) {
			if ((mapa[i][j]) == 'P') {
				mapa[i][j] = sign++;
				princess++;
			}
			if (mapa[i][j] >= '0' && mapa[i][j] <= '9') {
				num = mapa[i][j] - '0';
				while (ports[num][index] != -1)
					index += 2;
				ports[num][index] = i;
				ports[num][index + 1] = j;
				index = 0;
				printf("NUM: %d  ", num);
			}
		}
	}
	printf("\n");
	return princess;
}


// funkcia zachrani princezne
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
	int k = 0;
	int princess = find_teleports(mapa, n, m);
	/*	for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				printf("%c", mapa[i][j]);
			}
			printf("\n");
		}*/

	for (int i = 0; i < 3001; i++)
		path[i] = 1001;

	/*	for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 40; j++) {
				printf("%d ", ports[i][j]);
				}
			printf("\n");
		}*/


	struct Node ***arr;
	arr = alloc_data(n, m, 32);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int k = 0; k < 32; k++)
				arr[i][j][k].price = 600;


	struct Node start;
	start.x = 0;
	start.y = 0;
	start.z = 0;
	start.price = 0;
	start.type = mapa[0][0];

	insert_heap(start);

	int *smernik_na_cestu = dijkstra(mapa, arr, m, n, princess);

	for (int i = 0; smernik_na_cestu[i] <= 1000; i++) {
		//	if (i%2)
		//		printf("%d %d\n", smernik_na_cestu[i-1], smernik_na_cestu[i]);
		if (smernik_na_cestu >= 0)
			k++;
	}
	int *pole = (int*)malloc(k * sizeof(int));
	for (int i = 0; i < k; i++)
		pole[i] = smernik_na_cestu[i];

	*dlzka_cesty = k / 2;
	int temp;
	for (int i = 0; i < k; i++) {
		if (i % 2) {
			temp = pole[i - 1];
			pole[i - 1] = pole[i];
			pole[i] = temp;
			printf("%d %d\n", pole[i - 1], pole[i]);
		}
	}
	printf("\n\n%d\n", *dlzka_cesty);
	return pole;
}
/*	for (int i = 0; smernik_na_cestu[i] <= 1000; i++) {
//		if (i%2)
			//printf("%d %d\n", smernik_na_cestu[i-1], smernik_na_cestu[i]);
		if (smernik_na_cestu >= 0)
			k++;
	}
	int *pole = (int*)malloc(k*sizeof(int));
	for (int i = 0; i < k; i++)
		pole[i] = smernik_na_cestu[i];

	*dlzka_cesty = k / 2;

	for (int i = 0; i < k; i++) {
		if (i%2)
			printf("%d %d\n", pole[i-1], pole[i]);
	}
	printf("\n\n%d\n", *dlzka_cesty);
	return pole;
}*/



int main() {
	int j = 0;

	char mapa[5][5] = { {'P','P','H','H','H'},
					  {'H','C','H','H','C'},
					  {'C','C','H','C','C'},
					  {'C','H','C','H','C'},
					  {'H','P','C','D','C'}, };

	int vyska = 5;
	int sirka = 5;


	char **mapka;
	mapka = (char**)malloc(vyska * sizeof(char*));
	for (int i = 0; i < vyska; ++i) {
		mapka[i] = (char*)malloc(sirka * sizeof(char));
	}

	for (int j = 0; j < vyska; ++j) {
		for (int i = 0; i < sirka; ++i) {
			mapka[j][i] = mapa[j][i];
		}
	}

	int *path = zachran_princezne(mapka, sirka, vyska, 0, &j);
	free(path);
	getchar();
	return 0;
}

/*
trik: v portoch volam obzcajny insert do heapu a pridavam cenu.
make blok - nespravna cena
generator sa da zapnut len raz ty blbec, aj drak skantrit


ak su 3 tak (2^pocet princezien + 2) - 3
*/