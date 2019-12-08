#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define NON -1

struct Node {
	short x, y, z;
	short prev_x, prev_y, prev_z;
	char type;
	short price;
};


struct Node heap[10000];
int heap_next = 0;
short ports[10][300];


// vytvorenie vlastnej trojrozmernej mapy, do ktorej si budem ukladat prvky vybrate z heapu
struct Node ***alloc_data(int map_width, int map_height, int dimension) {
	struct Node ***new_map;
	int i, j;

	new_map = malloc(map_width * sizeof (*new_map));

	for (i = 0; i < map_width; i++)
		new_map[i] = NULL;

	for (i = 0; i < map_width; i++)
		new_map[i] = malloc(map_height * sizeof (*new_map[i]));

	for (i = 0; i < map_width; i++)
		for (j = 0; j < map_height; j++)
			new_map[i][j] = NULL;

	for (i = 0; i < map_width; i++)
		for (j = 0; j < map_height; j++)
			new_map[i][j] = malloc(dimension * sizeof (*new_map[i][j]));

	return new_map;
}

// vymena prvkov v heape
void heap_swap(int parent, int child) {
	struct Node temp;
	temp = heap[parent];
	heap[parent] = heap[child];
	heap[child] = temp;
}

// po priidani noveho prvku do heapu sa mu upravi pozicia
void heapify_up(int index) {
	int parent_index = (index - 1) / 2;
	if (heap[parent_index].price > heap[index].price) {
		heap_swap(parent_index, index);
		heapify_up(parent_index);
	}
}

// po vybere prvku z heapu ho nahradil iny, tento iny sa musi dostat nizsie na spravnu poziciu
void heapify_down(int parent) {
	
	int max;
	int left = parent * 2 + 1;
	int right = parent * 2 + 2;

	if (left >= heap_next || left < 0)
		left = NON;
	if (right >= heap_next || right < 0)
		right = NON;

	if (left != NON && heap[left].price < heap[parent].price)
		max = left;
	else
		max = parent;
	if (right != NON && heap[right].price < heap[max].price)
		max = right;

	if (max != parent) {
		heap_swap(parent, max);
		heapify_down(max);
	}
	return;
}

// vkladanie do heapu - najviac vpravo dole
void insert_heap(struct Node block) {
	heap[heap_next] = block;
	heapify_up(heap_next);
	heap_next++;
}

// vyberiem z heapu [0], nahradim ho tym, ktory je najviac vpravo dole
// poziciu najviac vpravo dole inicializujem na nuly (nic), neskor sa prepise
struct Node delete_heap() {
	if (heap[0].type == ' ')
		printf("Prazdny heap - nie je riesenie!\n");

	struct Node del = heap[0];
	heap[0] = heap[--heap_next];
	heap[heap_next].y = 0;
	heap[heap_next].x = 0;
	heap[heap_next].z = 0;
	heap[heap_next].price = 0;
	heap[heap_next].price = 0;
	heap[heap_next].type = ' ';
	heapify_down(0);
	return del;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// funkcia vrati cestu od zaciatku az po poslednu princeznu
int *cesta(struct Node curr, struct Node ***map) {
	int arr[3500];
	int counter = 0, temp = 0;

	// nacitam cestu do pola, ale odzadu, pretoze idem podla predchodcov od posledneho prvku
	while (1) {
		if (curr.x != curr.prev_x || curr.y != curr.prev_y) {
			arr[counter] = curr.y;
			counter++;
			arr[counter] = curr.x;
			counter++;
		}
		// ak som na zaciatku [0][0] a neexistuje predchodca iny ako [1][0] alebo [0][1], koncim 
		if (curr.x == 0 && curr.y == 0 && (!((curr.prev_x == 0 && curr.prev_y == 1) || (curr.prev_x == 1 && curr.prev_y == 0))))
			break;
		curr = map[curr.prev_x][curr.prev_y][curr.prev_z];
	}

	// vytvorim pole presne na velkost, ktoru potrebujem, aby som don ulozil vsetky suradnice
	int *path = (int *)malloc((counter + 1) * sizeof(int));
	for (int i = 0; i < counter + 1; i++)
		path[i] = 1001;

	// otocim pole tak, aby bolo zoradene od zaciatku
	for (int i = counter / 2 - 1; i >= 0; i--) {
		path[temp++] = arr[--counter];
		path[temp++] = arr[--counter];
	}
	return path;
}



// ak je to cenovo vyhodne, umiestnim prvok do mapy
void insert_to_map(struct Node place, struct Node ***map) {
	short x = place.x;
	short y = place.y;
	short z = place.z;

	if (map[x][y][z].price > place.price)
		map[x][y][z] = place;
}

// vyvori sa novy blok, urcia sa mu parametre a nasledne sa v dalsej funkcii hodi do heapu
void make_block(char **original, struct Node ***map, struct Node top_heap, int x, int y, int z) {

	map[x][y][z].x = x;
	map[x][y][z].y = y;
	map[x][y][z].z = z;
	map[x][y][z].type = original[x][y];
	if (original[x][y] == 'H')
		map[x][y][z].price = top_heap.price + 2;
	else map[x][y][z].price = top_heap.price + 1;
	map[x][y][z].prev_x = top_heap.x;
	map[x][y][z].prev_y = top_heap.y;
	map[x][y][z].prev_z = top_heap.z;
}


// fukncia posle do heapu vsetky teleporty s rovnakym cislom, ktore splnaju podmienky na vlozenie do heapu
void teleport(char **original, struct Node ***map, struct Node top_heap) {
	short num = 0, index = 0, x, y, z;

	num = top_heap.type - '0';

	while (ports[num][index] != -1) {
		z = top_heap.z;
		x = ports[num][index];
		y = ports[num][index + 1];

		if (map[x][y][z].price > top_heap.price) {
			make_block(original, map, top_heap, x, y, z);
			insert_heap(map[x][y][z]);
		}
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
// pokial je to specialny blok (a zatial neaktivny), treba zmenit dimenziu
struct Node where_am_i(struct Node top_heap, struct Node ***map) {

	if (top_heap.type == 'D' && (top_heap.z == 0 || top_heap.z == 2))
		return change_dimension(map, top_heap, 1);

	if (top_heap.type == 'G' && (top_heap.z == 0 || ((top_heap.z & 1) == 1 && (top_heap.z & 2) == 0)))
		return change_dimension(map, top_heap, 2);


	// kontrola princezien, kazda je oznacena inak
	if (top_heap.type == 'P' && top_heap.z % 2)
		if (!((top_heap.z & 4) == 4))
			return change_dimension(map, top_heap, 4);

	if (top_heap.type == 'O' && top_heap.z % 2)
		if (!((top_heap.z & 8) == 8))
			return change_dimension(map, top_heap, 8);

	if (top_heap.type == 'Q' && top_heap.z % 2)
		if (!((top_heap.z & 16) == 16))
			return change_dimension(map, top_heap, 16);

	if (top_heap.type == 'R' && top_heap.z % 2)
		if (!((top_heap.z & 32) == 32))
			return change_dimension(map, top_heap, 32);

	if (top_heap.type == 'S' && top_heap.z % 2)
		if (top_heap.z < 64)
			return change_dimension(map, top_heap, 64);

	return top_heap;
}


// 1. Vyberiem najlacnejsi prvok z heapu 
// 2. Ulozim ho do vlastnej mapy (ak je to vyhodne)
// 3. Jeho susedov z mapy vlozim do heapu
// 4. Opakujem
int *dijkstra(char **original, struct Node ***map, int m, int n, int princess) {
	struct Node top_heap;
	int max_width = m;
	int max_height = n;
	int dimension = 1;

	// definujem si minimalnu velkost dimenzie, ktoru ked dosiahnem, ukoncim prehladavanie
	for (int i = princess + 2; i > 0; i--)
		dimension *= 2;
	dimension -= 3;

	while (!0) {
		top_heap = delete_heap();

		insert_to_map(top_heap, map);
		top_heap = where_am_i(top_heap, map);

		short height = top_heap.x;
		short width = top_heap.y;
		short dim = top_heap.z;

		// overim, ci nestojim na teleporte, ak ano,
		// pridam do haldy vsetky dalsie s rovnakym typom
		if (top_heap.type >= '0' && top_heap.type <= '9')
			if ((top_heap.z & 2) == 2)
				teleport(original, map, top_heap);



		///////////////////////////////// KONIEC ///////////////////////////////
		// AK SOM NA PRINCEZNEJ A MOJA DIMENZIA JE VACSIA AKO POZADOVANA, KONCIM
		if (top_heap.type == 'P' || top_heap.type == 'O' || top_heap.type == 'Q')
			if (top_heap.z >= dimension)
				return cesta(top_heap, map);


		// pre kazdeho suseda aktualneho bloku overim, ci existuje
		// v povodnej mape a ak ano, ci je vyhodne pridat ho do heapu
		if (height + 1 < max_height && original[height + 1][width] != 'N') {
			if (map[height + 1][width][dim].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, height + 1, width, dim);
				insert_heap(map[height + 1][width][dim]);
			}
		}

		if (width + 1 < max_width && original[height][width + 1] != 'N') {
			if (map[height][width + 1][dim].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, height, width + 1, dim);
				insert_heap(map[height][width + 1][dim]);
			}
		}

		if (height - 1 >= 0 && original[height - 1][width] != 'N') {
			if (map[height - 1][width][dim].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, height - 1, width, dim);
				insert_heap(map[height - 1][width][dim]);
			}
		}

		if (width - 1 >= 0 && original[height][width - 1] != 'N') {
			if (map[height][width - 1][dim].price - 1 > top_heap.price) {
				make_block(original, map, top_heap, height, width - 1, dim);
				insert_heap(map[height][width - 1][dim]);
			}
		}
	}
}

// najdem vsetky teleporty a ulozim ich do dvojrozmerneho pola, podla toho, aki su typ
// pocas prehladavania mapy rovno aj spocitam princezne a rozlisim ich - kazdu nazvem inak
int find_teleports(char **mapa, int vyska, int sirka) {
	int princess = 0, num, index = 0;
	char sign = 'O';

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 300; j++)
			ports[i][j] = -1;


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
			}
		}
	}
	return princess;
}


// funkcia zachrani princezne
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
	int k = 0, dimensions = 4;
	int princess = find_teleports(mapa, n, m);
	for (int i = 0; i < princess; i++)
		dimensions *= 2;

	////////// vypis mapy //////////////
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				printf("%c ", mapa[i][j]);
			}
			printf("\n");
		}
	
	//////////////////////////////////////

	struct Node ***arr;
	arr = alloc_data(n, m, dimensions);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int k = 0; k < dimensions; k++)
				arr[i][j][k].price = 600;

	// [0][0] na zaciatku
	struct Node start;
	start.x = 0;
	start.y = 0;
	start.z = 0;
	start.price = 0;
	start.type = mapa[0][0];
	insert_heap(start);

	int *smernik_na_cestu = dijkstra(mapa, arr, m, n, princess);

	for (int i = 0; smernik_na_cestu[i] <= 1000; i++)
		if (smernik_na_cestu >= 0)
			k++;

	int *pole = (int*)malloc(k * sizeof(int));
	for (int i = 0; i < k; i++)
		pole[i] = smernik_na_cestu[i];

	// 1 dlzka za kazde 2 suradnice
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
	free(arr);
	return pole;
}

int main() {
	int l = 0;

	char mapa[11][11] = { {'H','H','H','H','C','H','H','C','C','C','P'},
					  {'H','C','H','H','C','C','C','C','C','H','C'},
					  {'H','P','H','C','C','C','H','H','H','H','C'},
					  {'C','H','H','C','H','H','C','H','C','C','C'},
					  {'H','C','H','C','C','C','C','C','H','H','C'},
					  {'H','C','H','C','C','C','H','H','H','C','H'},
					  {'H','H','H','H','C','H','H','H','C','C','H'},
					  {'H','H','C','H','C','P','H','H','C','C','P'},
					  {'H','H','C','H','H','C','H','C','C','C','C'},
					  {'C','C','D','C','C','H','C','H','C','C','C'},
					  {'P','C','C','C','C','H','C','C','C','H','H'}, };

	char **moja;
	moja = (char**)malloc(11 * sizeof(char*));
	for (int i = 0; i < 11; i++)
		moja[i] = (char*)malloc(11 * sizeof(char));

	for (int i = 0; i < 11; i++)
		for (int j = 0; j < 11; j++)
			moja[i][j] = mapa[i][j];

	int *path = zachran_princezne(moja, 11, 11, 0, &l);
	free(path);
	getchar();
	return 0;
}