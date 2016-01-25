#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _list_t_ {
    char *str;
    unsigned short *index;
    struct _list_t_ *next;
} list;
typedef struct _hash_table_t_ {
    int size;
    list **table;
} hashTable;

hashTable *createHashTable(int);
unsigned short hash(char *);
list *lookup(hashTable *hashtable, char *);
unsigned short hash(char *);
int add(hashTable *, char *, unsigned short *);
void freeTable(hashTable *);

int main(int argc,char** argv) {
	if (argc != 3) {
		printf("krivi broj parametara!\n");
		return -1;
	}
	
	unsigned short i;
    char * rjecnik[65536];
    hashTable *dictionary;
    int sizeTable = 65536;
    dictionary = createHashTable(sizeTable);
    list *list;

	FILE *ulaz, *izlaz;
	if ((ulaz = fopen(argv[1], "rb")) == NULL) return(-1);
	if ((izlaz = fopen(argv[2], "wb")) == NULL) return(-1);
	for (i=0;i<256;i++) {
        rjecnik[i] = (char *) calloc(2,sizeof(char));
        rjecnik[i][0] = (char) i;
        rjecnik[i][1] = '\0';
        add(dictionary, rjecnik[i], i);
    }
    unsigned char rijec, nova;
    unsigned short zadnji=256;

    fread(&rijec, 1, 1, ulaz);
    char radna[10000];
    char pomocna[10000];

    radna[0]= rijec;
    radna[1]='\0';

    while (fread(&nova, 1,1,ulaz)==1){
        strcpy(pomocna,radna);
        int duljina =strlen(pomocna);

        pomocna[duljina]=nova;
        pomocna[duljina+1]='\0';

        if (lookup(dictionary,pomocna) != NULL){
            strcpy(radna, pomocna);

        } else {
            if (zadnji<65536){

                add(dictionary, pomocna, zadnji);
                rjecnik[zadnji++]=pomocna;
            }
            list = lookup(dictionary,radna);
            if (list != NULL){
                fwrite(&list->index, 1, sizeof(unsigned short), izlaz);
            }

            radna[0]=nova;
            radna[1]='\0';
        }
    }
    if (zadnji<65536){
        add(dictionary, pomocna, zadnji);
    }
    list = lookup(dictionary,radna);
    if (list != NULL){
        fwrite(&list->index, 1, sizeof(unsigned short), izlaz);
    }
    free(dictionary);
    fclose(ulaz);
    fclose(izlaz);

    return 0;
}

hashTable *createHashTable(int size)
{
    hashTable *newTable;

    if (size<1) return NULL;

    if ((newTable = malloc(sizeof(hashTable))) == NULL) {
        return NULL;
    }

    if ((newTable->table = malloc(sizeof(list *) * size)) == NULL) {
        return NULL;
    }
    int i;

    for(i=0; i<size; i++) newTable->table[i] = NULL;

    newTable->size = size;

    return newTable;
}
unsigned short hash(char *str)
{
    unsigned short hashval;

    hashval = 0;

    for(; *str != '\0'; str++) hashval = *str + (hashval << 5) - hashval;

    return hashval % 65536;
}
list *lookup(hashTable *hashtable, char *str)
{
    list *list;
    unsigned int hashval = hash(str);

    for(list = hashtable->table[hashval]; list != NULL; list = list->next) {
        if (strcmp(str, list->str) == 0) return list;
    }
    return NULL;
}
int add(hashTable *hashtable, char *str, unsigned short *index)
{
    list *newList;
    list *currentList;
    unsigned int hashval = hash(str);


    currentList = lookup(hashtable, str);

    if (currentList != NULL) return 2;

    if ((newList = malloc(sizeof(list))) == NULL) return 1;
    newList->str = strdup(str);
    newList->index=index;
    newList->next = hashtable->table[hashval];
    hashtable->table[hashval] = newList;

    return 0;
}
void freeTable(hashTable *hashtable)
{
    int i;
    list *list, *tmp;

    if (hashtable==NULL) return;

    for(i=0; i<hashtable->size; i++) {
        list = hashtable->table[i];
        while(list!=NULL) {
            tmp = list;
            list = list->next;
            free(tmp->str);
            free(tmp->index);
            free(tmp);
        }
    }
    free(hashtable->table);
    free(hashtable);
}
