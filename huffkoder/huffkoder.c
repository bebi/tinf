#include <stdio.h>
#include <malloc.h>
#include <string.h>


typedef struct bajt_s{
	unsigned char bajt;
	int broj_pojavljivanja;
	struct bajt_s* lijevo, *desno;
	int interni;
	char kod[255];
	int duljina_koda;
}bajt_t;

bajt_t polje_bajtova[256];


void zamjeni(bajt_t* a, bajt_t* b) {
	bajt_t tmp = *a;
	*a = *b;
	*b = tmp;
}

void sortiraj(bajt_t* polje,int velicina) {
	int i,j;
	for (i = 0; i < velicina-1; i++) {
		for (j = 0; j < velicina-i-1; j++) {
			if (polje[j + 1].broj_pojavljivanja < polje[j].broj_pojavljivanja) {
				zamjeni(&polje[j], &polje[j + 1]);
			}
		}
		
	}
}

void ispis_bajt(bajt_t bajt) {
	int i;
	if(bajt.interni)
		printf("int %d\t", bajt.broj_pojavljivanja);
	else
		printf("%#.2x %d\t", bajt.bajt, bajt.broj_pojavljivanja);
	for(i=0;i<bajt.duljina_koda;i++)
		printf("%c",bajt.kod[i]);
	printf("\t%d\n",bajt.duljina_koda);
}

void ispis_polja(bajt_t * polje,int velicina) {
	int i;
	for (i = 0; i < velicina; i++) {
		printf("polje[%d]=",i);
		if (polje[i].bajt == (char)0) {
			printf("0x");
			ispis_bajt(polje[i]);
		}
		else
			ispis_bajt(polje[i]);
	}
}

bajt_t * pop(bajt_t* polje,int * velicina) {
	bajt_t *tmp = (bajt_t*)malloc(sizeof(bajt_t));
	*tmp = polje[0];
	*velicina=*velicina-1;
	memcpy(polje, polje + 1, sizeof(bajt_t)*(*velicina));
	return tmp;
}

bajt_t *zbroji(bajt_t *a, bajt_t *b) {
	bajt_t *tmp = (bajt_t *)malloc(sizeof(bajt_t));
	tmp->interni = 1;
	tmp->broj_pojavljivanja = a->broj_pojavljivanja + b->broj_pojavljivanja;
	tmp->lijevo = a;
	tmp->desno = b;
	tmp->duljina_koda=0;
	return tmp;
}

void dodaj_u_polje(bajt_t * polje,int *velicina,bajt_t * cvor) {
	int mjesto = 0,i;
	for (i = 0; i < *velicina; i++) {
		if (polje[i].broj_pojavljivanja >= cvor->broj_pojavljivanja) {
			mjesto = i;
			break;
		}
		mjesto = i;
	}
	*velicina=*velicina+1;
	polje=(bajt_t *)realloc(polje,sizeof(bajt_t)*(*velicina));
	bajt_t *tmp=(bajt_t *)malloc(sizeof(bajt_t)*(*velicina));
	memcpy(tmp,polje,sizeof(bajt_t)*(*velicina));
	for(i=0;i<mjesto;i++)
		polje[i]=tmp[i];
	polje[mjesto]=*cvor;
	for(i=mjesto+1;i<*velicina;i++)
		polje[i]=tmp[i-1];
	free(tmp);
}

void prepisi_kod(bajt_t *cvor){
	int i;
	for(i=0;i<cvor->duljina_koda;i++){
		polje_bajtova[(int)cvor->bajt].kod[i]=cvor->kod[i];
	}
	polje_bajtova[(int)cvor->bajt].duljina_koda=cvor->duljina_koda;
}

void kodiraj_stablo(bajt_t *korijen,char znak,char *prefix,int duljina_prefixa){
	int i;
	if(znak!='#'){
		for(i=0;i<duljina_prefixa;i++){
			korijen->kod[i]=prefix[i];
		}
		korijen->duljina_koda=duljina_prefixa+1;
		korijen->kod[duljina_prefixa]=znak;
	}
	
	
	if(korijen->lijevo==NULL && korijen->desno==NULL){
		//ispis_bajt(*korijen);
		prepisi_kod(korijen);
		return;
	}
	else{
		kodiraj_stablo(korijen->lijevo,'1',korijen->kod,korijen->duljina_koda);
		kodiraj_stablo(korijen->desno,'0',korijen->kod,korijen->duljina_koda);
	}
	
	
}


int main(int argc,char** argv) {
	if (argc != 4) {
		printf("krivi broj parametara!\n");
		return -1;
	}

	//init
	FILE *ulaz,*tablica, *izlaz;
	int velicina=256;
	if ((ulaz = fopen(argv[1], "rb")) == NULL) return(-1);
	if ((tablica = fopen(argv[2], "w+")) == NULL) return(-1);
	if ((izlaz = fopen(argv[3], "wb")) == NULL) return(-1);
	fseek(ulaz, 0, SEEK_END);
	int velicina_ulaz = ftell(ulaz);
	rewind(ulaz);
	int i;
	for (i = 0; i < velicina; i++) {
		polje_bajtova[i].broj_pojavljivanja = 0;
		polje_bajtova[i].bajt = (unsigned char)i;
		polje_bajtova[i].lijevo = NULL;
		polje_bajtova[i].desno = NULL;
		polje_bajtova[i].interni = 0;
		polje_bajtova[i].duljina_koda=0;
	}

	//citanje ulaza
	i = velicina_ulaz;
	unsigned char *ulazni_bajtovi=(unsigned char *)malloc(velicina_ulaz*sizeof(char));
	for(i=0;i<velicina_ulaz;i++){
		fread((ulazni_bajtovi+i), sizeof(char), 1, ulaz);
		polje_bajtova[(int)ulazni_bajtovi[i]].broj_pojavljivanja++;
	}

	//stvaranje koda	
	bajt_t *tmp1;
	tmp1 = (bajt_t*)malloc(velicina*sizeof(bajt_t));
	memcpy(tmp1, polje_bajtova, velicina*sizeof(bajt_t));
	while(velicina!=1){
		sortiraj(tmp1,velicina);
		dodaj_u_polje(tmp1,&velicina,zbroji(pop(tmp1,&velicina),pop(tmp1,&velicina)));
	}
	char prefix[255];
	prefix[0]='#';
	kodiraj_stablo(tmp1,'#',prefix,1);
	
	//ispis u tablicu
	for(i=0;i<256;i++){
		fprintf(tablica,"%s\n",polje_bajtova[i].kod);
	}
	
	//kopresija ulazne datoteke
	unsigned char buffer='\0';
	bajt_t tmp;
	int j,buffer_counter=0;
	for(i=0;i<velicina_ulaz;i++){
		tmp=polje_bajtova[(int)ulazni_bajtovi[i]];
		for(j=0;j<tmp.duljina_koda;j++){
			if(buffer_counter==8){
				fwrite(&buffer,sizeof(char),1,izlaz);
				buffer='\0';
				buffer_counter=0;
			}
			buffer|=tmp.kod[j]-(int)'0';
			buffer<<=1;
			buffer_counter++;
		}				
	}

	fclose(ulaz);
	fclose(tablica);
	fclose(izlaz);
	return 0;
}
