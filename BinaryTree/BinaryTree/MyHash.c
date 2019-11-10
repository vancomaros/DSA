#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int size_of_array;
double lambda;
int elements;
int SUM;

int my_hash(int *arr, int value) {
	int index, temp = 0;
	index = value % size_of_array;
	while (++temp) {
		if (index > size_of_array || index < 0) {
			return 1;
		}
		else if (*(arr + index) == -1) {
			*(arr + index) = value;
			lambda = ((double)(++elements / (double)((size_of_array))));
			return 0;
		}
		else index = index + 2 ^ temp - 1;
	}
}


void find_hash(int *arr, int value) {
	int index, temp = 0;
	index = value % size_of_array;
	while (1) {
		if (index < size_of_array && value == *(arr + index)) {
			printf("Je na indexe %d mojho hashu\n", index);
			return;
		}
		else if (index > size_of_array || *(arr + index) == -1) {
			printf("%d v mojom hashi nie je.\n", value);
			return;
		}
		index += 2 ^ ++temp - 1;
	}
}

int check_hash(int *arr, int value) {
	int index, temp = 0;
	index = value % size_of_array;
	while (1) {
		if (index < size_of_array && value == *(arr + index)) {
			return 1;
		}
		else if (index > size_of_array || *(arr + index) == -1) {
			return 0;
		}
		index += 2 ^ ++temp - 1;
	}
}

int *insert_hash(int *arr) {
	int num = SUM;
	int value;

	while (num--) {
		value = (rand() % (100000 + 1 - 0) + 1) * (rand() % (100000 + 1 - 0) + 1);
		if (check_hash(arr, num) == 1)
			continue;
		if (lambda >= 0.5)
			arr = resize_hash(arr);
		while (my_hash(arr, value) == 1)
			arr = resize_hash(arr);
	}
	return arr;
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
			while (my_hash(new_arr, *(arr + i)) == 1) {
				size_of_array *= 2;
				i = 0;
			}
	free(arr);
	return new_arr;
}