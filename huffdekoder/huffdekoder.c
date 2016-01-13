#include <stdio.h>
#include <malloc.h>
#include <string.h>

int broj_valjanih_kodova(int *polje,int velicina){
	int i,counter=0;
	for(i=0;i<velicina;i++){
		if(polje[i]==1)
			counter++;
		}
	return counter;
}

int maska(int broj){
	if(broj==1)
		return 0;
	else
		return (1<<(broj-1));
}

void reset_polja(int *polje,int velicina){
	int i;
	for(i=0;i<velicina;i++){
		polje[i]=1;
	}
}

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
	char tmp;
	unsigned char buffer,bit;
	int valjani_kodovi[256],k=0,j;
	reset_polja(valjani_kodovi,256);
	while(1){
		if(k==velicina_ulaz) break;
		buffer=ulazni_bajtovi[k];
		for(i=8;i>0;i--){
			bit=buffer&maska(i);
			
			if(bit>0){
				bit='1';
			}
			else{
				bit='0';
			}
			for(j=0;j<256;j++){
				if(valjani_kodovi[j]==1 && bit==polje_kodova[j][8*k+(8-i)]){
					valjani_kodovi[j]=1;
				}
				else{
					valjani_kodovi[j]=0;
				}				
			}
			if(broj_valjanih_kodova(valjani_kodovi,256)==1){
				for(j=0;j<256;j++)
					if(valjani_kodovi[j]==1) break;
				
				tmp=(char)j;
				fwrite(&tmp,sizeof(char),1,izlaz);
				reset_polja(valjani_kodovi,256);
				if(i!=1) continue;
				else break;
			}
		}
		k++;
		
	}
	

	
	fclose(ulaz);
	fclose(tablica);
	fclose(izlaz);
	return 0;
}
