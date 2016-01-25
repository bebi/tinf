#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main(int argc,char** argv) {
	if (argc != 4) {
		printf("krivi broj parametara!\n");
		return -1;
	}
	
	FILE *ulaz,*tablica, *izlaz;
	if ((ulaz = fopen(argv[2], "rb")) == NULL) return(-1);
	if ((tablica = fopen(argv[1], "r")) == NULL) return(-1);
	if ((izlaz = fopen(argv[3], "wb")) == NULL) return(-1);
	fseek(ulaz, 0, SEEK_END);
	int velicina_ulaz = ftell(ulaz);
	rewind(ulaz);
	int i;

	char polje_kodova[256][256];
	for(i=0;i<256;i++){
		fscanf(tablica,"%s",polje_kodova[i]);
	}

	unsigned char buffer[136]={0},citaj_ulaz[18],*dodaj=(unsigned char*)malloc(1),tmp[8];
	int preostali_bajtovi=velicina_ulaz, j, k, m, l, buff_pozicija, trenutni_znak, duljina_koda, br_bajtova, bitova_procitano=0;
	fread(citaj_ulaz, 17, 1, ulaz);
	citaj_ulaz[17] = '\0';
	while(preostali_bajtovi > 0){
		buff_pozicija = 0;
		for(j=0; j<17; j++){
			trenutni_znak = citaj_ulaz[j];
			for(k=0; k<8; k++){
				tmp[k] = trenutni_znak%2 + 48;
				trenutni_znak/=2;
			}
			for(k=0; k<8; k++)
				buffer[buff_pozicija++] = tmp[7-k];
		}
		for(j=0; j<256; j++)
			if(preostali_bajtovi > 0 && (strstr((char*)buffer, polje_kodova[j]) - (char*)buffer == 0)){
				duljina_koda = strlen(polje_kodova[j]);
				bitova_procitano += duljina_koda;
				for(k=duljina_koda; k<136; k++)
					buffer[k-duljina_koda] = buffer[k];
				buffer[136-bitova_procitano]='\0';
				br_bajtova = (136-strlen((char*)buffer))/8;

				for(k=0; k<br_bajtova; k++){
					bitova_procitano -=8;
					fread(dodaj, 1, 1, ulaz);
					for(m=1; m<17; m++)
						citaj_ulaz[m-1]=citaj_ulaz[m];
					citaj_ulaz[16] = *dodaj;
					citaj_ulaz[17] = '\0';
					preostali_bajtovi--;
					buff_pozicija = 0;
					for(m=0; m<17; m++){
						trenutni_znak = citaj_ulaz[m];
						for(l=0; l<8; l++){
							tmp[l] = trenutni_znak%2 + 48;
							trenutni_znak/=2;
						}
						for(l=0; l<8; l++)
							buffer[buff_pozicija++] = tmp[7-l];
					}
					if(bitova_procitano>0)
						for(m=bitova_procitano; m<136; m++)
							buffer[m-bitova_procitano]=buffer[m];
				}
				fprintf(izlaz, "%c", (char)j);
				j=0;
			}

	}
	
	fprintf(izlaz, "%c", '\n');

	fclose(ulaz);
	fclose(tablica);
	fclose(izlaz);
	return 0;
}
