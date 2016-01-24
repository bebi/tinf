#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * rjecnik[65536];

int main(int argc, char const *argv[])
{
	if(argc != 3){
		printf("krivi broj parametara!\n");
		return -1;
	}
    FILE *ulaz,*izlaz;
    if ((ulaz = fopen(argv[1], "rb")) == NULL) return(-1);
	if ((izlaz = fopen(argv[2], "wb")) == NULL) return(-1);

	int j;
	for(j = 0; j < 256; j++) {
		rjecnik[j] = (char *) malloc(2*sizeof(char));
		rjecnik[j][0] = (char) j;
		rjecnik[j][1] = '\0';
	}

	unsigned short trenutni, prosli;
	int zadnji = 256;
	char pomocna[2];
	pomocna[1]='\0';

	fread(&prosli, sizeof(short), 1, ulaz);
	while (fread(&trenutni, sizeof(short), 1, ulaz) == 1) {
		fwrite(rjecnik[prosli], sizeof(char), strlen(rjecnik[prosli]), izlaz);

		if (zadnji<65536) {
			char * novi = (char *) malloc((strlen(rjecnik[prosli])+2)*sizeof(char));
			strcpy(novi, rjecnik[prosli]);

			if (rjecnik[trenutni] == NULL)
                pomocna[0]=rjecnik[prosli][0];
            else pomocna[0]=rjecnik[trenutni][0];

			strcat(novi, pomocna);
			rjecnik[zadnji++] = novi;

		}
		prosli = trenutni;
	}
	fwrite(rjecnik[prosli], sizeof(char), strlen(rjecnik[prosli]), izlaz);

	fclose(ulaz);
	fclose(izlaz);

	return 0;
}
