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

	char *p0 = memory_alloc(6);//
	char *p1 = memory_alloc(7);//
	char *p2 = memory_alloc(8);//
	char *p3 = memory_alloc(9);//
	char *p4 = memory_alloc(10);//
	char *p5 = memory_alloc(11);//
	vypis();
	char *p6 = memory_alloc(12);//
	vypis();
//	printf("%d %d\n", memory_free(p4), (char *)p4 - (char *)pointer - 4);
//	vypis();
//	printf("%d %d\n", memory_free(p2), (char *)p2 - (char *)pointer - 4);
//	vypis();
//	printf("%d %d\n", memory_free(p1), (char *)p1 - (char *)pointer - 4);
//	vypis();
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
/*
	char *p0 = memory_alloc(9);//
	char *p1 = memory_alloc(16);//
	char *p2 = memory_alloc(20);//
	char *p3 = memory_alloc(8);//
	char *p4 = memory_alloc(8);//
	char *p5 = memory_alloc(12);
	char *p6 = memory_alloc(19);//
	char *p7 = memory_alloc(17);
	char *p8 = memory_alloc(23);
	char *p9 = memory_alloc(15);
	char *p10 = memory_alloc(14);//
	char *p11 = memory_alloc(18);//
	char *p12 = memory_alloc(15);
	char *p13 = memory_alloc(17);//
	char *p14 = memory_alloc(22);
	char *p15 = memory_alloc(22);//
	char *p16 = memory_alloc(14);
	char *p17 = memory_alloc(9);//
	char *p18 = memory_alloc(22);
	char *p19 = memory_alloc(12);
	char *p20 = memory_alloc(20);//
	char *p21 = memory_alloc(23);//
	char *p22 = memory_alloc(11);//
	char *p23 = memory_alloc(8);//
	char *p24 = memory_alloc(18);
	char *p25 = memory_alloc(13);
	char *p26 = memory_alloc(13);//
	char *p27 = memory_alloc(9);
	char *p28 = memory_alloc(21);//
	char *p29 = memory_alloc(8);
	char *p30 = memory_alloc(17);
	char *p31 = memory_alloc(19);//
	char *p32 = memory_alloc(13);//
	char *p33 = memory_alloc(10);//
	char *p34 = memory_alloc(23);
	char *p35 = memory_alloc(10);//
	char *p36 = memory_alloc(23);
	char *p37 = memory_alloc(11);//
	char *p38 = memory_alloc(19);//

	vypis();

	printf("%d %d\n", memory_free(p20), (char *)p20 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p0), (char *)p0 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p28), (char *)p28 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p26), (char *)p26 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p11), (char *)p11 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p31), (char *)p31 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p15), (char *)p15 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p10), (char *)p10 - (char *)pointer - 4); 
	printf("%d %d\n", memory_free(p22), (char *)p22 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p37), (char *)p37 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p17), (char *)p17 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p13), (char *)p13 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p33), (char *)p33 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p6), (char *)p6 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p4), (char *)p4 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p35), (char *)p35 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p21), (char *)p21 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p1), (char *)p1 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p3), (char *)p3 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p23), (char *)p23 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p32), (char *)p32 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p38), (char *)p38 - (char *)pointer - 4); 	
	printf("%d %d\n", memory_free(p2), (char *)p2 - (char *)pointer - 4);
	vypis();
	
	char *p39 = memory_alloc(13); 
	char *p40 = memory_alloc(18); 
	char *p41 = memory_alloc(11); 
	char *p42 = memory_alloc(20); 
	char *p43 = memory_alloc(18); 
	char *p44 = memory_alloc(10); 
	char *p45 = memory_alloc(15); 
	char *p46 = memory_alloc(13); 
	char *p47 = memory_alloc(8); 
	char *p48 = memory_alloc(8); 
	char *p49 = memory_alloc(11); 
	char *p50 = memory_alloc(22); 
	char *p51 = memory_alloc(14); 
	char *p52 = memory_alloc(16); 
	char *p53 = memory_alloc(19); 
	char *p54 = memory_alloc(18); 
	char *p55 = memory_alloc(15); 
	char *p56 = memory_alloc(18); 
	char *p57 = memory_alloc(13); 
	char *p58 = memory_alloc(18); 
	char *p59 = memory_alloc(11); 
	char *p60 = memory_alloc(11); 
	char *p61 = memory_alloc(13); 
	char *p62 = memory_alloc(23); 
	char *p63 = memory_alloc(20); 
	char *p64 = memory_alloc(10); 
	char *p65 = memory_alloc(18); 
	char *p66 = memory_alloc(22); 
	char *p67 = memory_alloc(11); 
	
	vypis();
	

	printf("%d %d\n", memory_free(p46), (char *)p46 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p29), (char *)p29 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p56), (char *)p56 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p30), (char *)p30 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p25), (char *)p25 - (char *)pointer - 4);
	printf("%d %d\n", memory_free(p40), (char *)p40 - (char *)pointer - 4);
	
	vypis();
	*/
	getchar();
	return 0;
}