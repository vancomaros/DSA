#include <stdio.h>
#include <stdlib.h>

// pointer ukazuje na zaciatok pola, kde alokujem
// *((int *)pointer + 1) je hodnota prveho volneho bloku v zozname
void *pointer;

struct STR {
	int head; 	// velkost bloku v poli
	int next;	// index nasledujuceho bloku, ak zaplnam blok, tento udaj sa prepise
};

void vypis() {
	for (int i = 0; i < 100; i++) 
		printf("%3d:  %3c   %3d\n", i, *((char*)pointer + i), *((char*)pointer + i));
}


// funkcia najde hlavicku, za ktorou nasleduje pointer smerujuci na current 
void *previous(void *current) {
	void *ptr = pointer;
	while (*((int*)ptr + 1) != 0) {		// prechadzam pole pokial nenajdem zhodu, az do konca
		if (((char *)pointer + *((int*)ptr + 1)) == current)
			return (int*)ptr;
		else ptr = ((char *)pointer + *((int*)ptr + 1));
	}
	return (int*)pointer; // ak nenajde ziaden, vrati smernik na zaciatok celeho pola
}

void *memory_alloc(unsigned int size) {
	void *first;
	first = (char *)pointer + (*((int *)pointer + 1));  // first = prvy volny blok v celom poli,
	while (1) {											// smeruje na jeho hlavicku
		if (size <= *((int *)first) && (*((int *)first) != *(int *)pointer)) { 		// zmesi sa mi pozadovany do aktualneho bloku?
			if (*((int *)first) - size >= 12) {			// oplati sa mi vyrobit novu hlavicku?
				struct STR curr;
				// temp = novy index hlavicky, ziskam ho tak, ze sa posuniem o size, velkost hlavicky (4) a index 
				void *temp = (char *)pointer + (*((int *)previous(first) + 1) + size + sizeof(int));
				curr.head = *((int *)first) - size - sizeof(int); // nastavi hlavicku noveho bloku
				curr.next = *((int *)first + 1);				  // ukazovat bude tam, kde ukazoval blok, do ktoreho som prave alkoval
				*((struct STR *)temp) = curr;
				*((int *)previous(first) + 1) = (char *)first - (char *)pointer + size + sizeof(int);
				*(int *)first = 0 - size;  						  // alokovanemu bloku zmenim hodnotu na zapornu 
				return ((int *)first + 1); 					  	  // vratim pointer na prvy byte kde som alokoval miesto, nie na hlavicku
			}
			// ak netreba vyrabat novu hlavicku. len zmenim ukazovatel na dalsi volny blok, 
			// na ten, na ktory ukazoval blok, ktory som prave alokoval 
			else {
				*((int *)previous(first) + 1) = *((int *)first + 1);
				//*((int *)pointer + 1) = *((int *)first + 1);
				*(int *)first *= -1;;						// alokovanemu bloku zmenim hodnotu na zapornu 
				return ((int *)first + 1);
			}
		}
		else if (*((int *)first + 1) == 0)						// ak sa mi nezmesti a som na konci pola, vratim NULL
			return NULL;
		else if (size > *((int *)first))						// ak nie som na konci, len posuniem ukazovatel na dalsi blok
			first = ((char *)pointer + *((int *)first + 1));
		else if (*((int *)first) != *(int *)pointer)			// ak ziadna z moznosti, vratim NULL
			return NULL;
	}
}

int memory_free(void *valid_ptr) {

	if (*((char *)valid_ptr + (*((int *)valid_ptr - 1)*(-1))) > 0) {
		*((int *)valid_ptr - 1) *= -1;
		int pom = *((int *)valid_ptr - 1) + (sizeof(int) + *((char *)valid_ptr + *((int *)valid_ptr - 1))); //heh
		void *tempo = previous((char *)valid_ptr + *((int *)valid_ptr - 1));
		*(int *)valid_ptr = *((int*)(((char *)valid_ptr + (*((int *)valid_ptr - 1) + sizeof(int)))));
		*((int *)valid_ptr - 1) = pom;
		*((int *)tempo + 1) = ((char *)valid_ptr - 4) - (char *)pointer;
		for (int i = 4; i < *((int *)valid_ptr - 1); i++) 
			*((char *)valid_ptr + i) = 0;
		}
	else {
		struct STR curr;
		curr.head = 0 - *((int *)valid_ptr - 1);	// uvolnenemu bloku nastavim novu hlavicku, velkost si ponecha
		curr.next = *((int *)pointer + 1);			// smerovat bude tam, kde smeroval pointer + 1
		*((int *)pointer + 1) = ((char *)valid_ptr - (char *)pointer - sizeof(int));  // a pointer + 1 sa nasmeruje sem
		void *temp = ((int *)valid_ptr - 1);										  // novy volny blok sa ako keby stane prvy v zozname
		*((struct STR *)temp) = curr;
		}
	return 0;
}

int memory_check(void *ptr) {
	if (ptr) 										// pokial pointer existuje, ma hodnotu
		if (*((char *)ptr - sizeof(int)) < 0)		// ak je jeho hodnota zaporna, smeruje na obsadeny blok
			return 1;
	return 0;
}

void memory_init(void *ptr, unsigned int size) {
	pointer = ptr;						// nastavim globalny pointer na zaciatok pola
	struct STR free_nxt;
	free_nxt.head = size - 3 * (sizeof(int));		// velkost bloku bude velkost celeho pola - hlavicka pola (8) - hlavicka bloku (4)
	free_nxt.next = 0;							// kedze blok je len jeden, nebude smerovat nikam

	for (unsigned i = 0; i < size; i++) {
		*((char *)ptr + i) = 0; // vsetko na 0, aby boli prvy bit 0
	}

	*((int *)ptr) = size; 				// velkost pamate v prvom bajte
	*((int *)ptr + 1) = 8; 				// prvych 8 bajtov obsahuje hlavicku

	ptr = ((int *)ptr + 2);				// hlavicka prveho bloku bude na 3. bajte
	*((struct STR *) ptr) = free_nxt;	// a tam ju umiestnim
}

int main() {
	char pole[100];
	memory_init(pole, 100);

	/* some testing */
	char *p0 = memory_alloc(6);//
	char *p1 = memory_alloc(7);//
	char *p2 = memory_alloc(8);//
	char *p3 = memory_alloc(9);//
	char *p4 = memory_alloc(10);//
	char *p5 = memory_alloc(11);//
	vypis();
	char *p6 = memory_alloc(12);//
	vypis();

	printf("%d %d\n", memory_free(p6), (char *)p6 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p5), (char *)p5 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p3), (char *)p3 - (char *)pointer - 4);
	vypis();
	char *p7 = memory_alloc(8);//
	char *p8 = memory_alloc(8);//
	char *p9 = memory_alloc(8);//
	char *p10 = memory_alloc(8);//
	char *p11 = memory_alloc(8);//
	vypis();
	char *p12 = memory_alloc(8);//

	getchar();
	return 0;
}
