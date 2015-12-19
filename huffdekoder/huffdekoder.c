#include <stdio.h>
#include <malloc.h>
#include <string.h>


int main(int argc,char** argv) {
	if (argc != 4) {
		printf("krivi broj parametara!\n");
		return -1;
	}

	//init
	FILE *ulaz,*tablica, *izlaz;
	if ((ulaz = fopen(argv[2], "rb")) == NULL) return(-1);
	if ((tablica = fopen(argv[1], "r")) == NULL) return(-1);
	if ((izlaz = fopen(argv[3], "wb")) == NULL) return(-1);
	fseek(ulaz, 0, SEEK_END);
	int velicina_ulaz = ftell(ulaz);
	rewind(ulaz);
	int i;

	//citanje ulaza
	i = velicina_ulaz;
	unsigned char *ulazni_bajtovi=(unsigned char *)malloc(velicina_ulaz*sizeof(char));
	for(i=0;i<velicina_ulaz;i++){
		fread((ulazni_bajtovi+i), sizeof(char), 1, ulaz);
	}
	
	char polje_kodova[256][256];
	for(i=0;i<256;i++){
		fscanf(tablica,"%s",polje_kodova[i]);
	}
	
	for(i=0;i<256;i++){
		printf("%s\n",polje_kodova[i]);
	}

	
	fclose(ulaz);
	fclose(tablica);
	fclose(izlaz);
	return 0;
}
