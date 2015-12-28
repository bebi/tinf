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
	
	//for(i=0;i<256;i++){
	//	printf("%s\n",polje_kodova[i]);
	//}
	
	unsigned char *tmp=ulazni_bajtovi;
	unsigned char buffer,bit;
	int valjani_kodovi[256];
	while(1){
		if(tmp==NULL) break;
		buffer=*tmp;
		for(i=8;i>0;i--){
			bit=buffer&i;
			if((int)bit|0>0){
				bit='1';
			}
			else{
				bit='0';
			}
			for(j=0;i<256;j++){
				for(k=0;k<256;k++){
					if(bit==polje_kodova[j][k]){
						valjani_kodovi[j]=1;
					}
					else{
						valjani_kodovi[j]=0;
					}
				}
			}
		}
		
	}
	

	
	fclose(ulaz);
	fclose(tablica);
	fclose(izlaz);
	return 0;
}
