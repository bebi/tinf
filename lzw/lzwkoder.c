#include <stdio.h>
#include <stdlib.h>

int rijecnk[65536];

int main(int argc,char** argv) {
	if (argc != 3) {
		printf("krivi broj parametara!\n");
		return -1;
	}

	//init
	FILE *ulaz, *izlaz;
	if ((ulaz = fopen(argv[1], "rb")) == NULL) return(-1);
	if ((izlaz = fopen(argv[2], "wb")) == NULL) return(-1);
	fseek(ulaz, 0, SEEK_END);
	int velicina_ulaz = ftell(ulaz);
	rewind(ulaz);
	int i;
	for (i=0;i<256;i++){
		rijecnk[i]=i;
	}
	
	//citanje ulaza
	i = velicina_ulaz;
	unsigned char *bajtovi_ulaz=(unsigned char *)malloc(velicina_ulaz*sizeof(unsigned char));	
	if(fread(bajtovi_ulaz,sizeof(char),velicina_ulaz,ulaz)!=velicina_ulaz){
		printf("greska u citanju\n");
		exit(1);
	}
	
	
	
	fclose(ulaz);
	fclose(izlaz);
	return 0;
}
