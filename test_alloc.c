/* Fichier de test
*/

#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NB_TESTS 40
#define NB_POINTERS 40

static void *alloc(size_t estimate) {
    void *result;
    result = mem_alloc(estimate);

    debug("Alloced %05zu bytes at %p\n", estimate, result);
    return result;
}

static void *big_alloc(size_t estimate) {
    void* result;

    while ((result = mem_alloc(estimate)) == NULL) {
        estimate--;
    }

    debug("Alloced %05zu bytes at %p\n", estimate, result);
    return result;
}

void afficher_zone(void *adresse, size_t taille, int free)
{
    printf("Zone %s, Adresse : %lx, Taille : %lu\n", free?"libre":"occupee",
            (unsigned long) adresse, (unsigned long) taille);
}

void alloc_big(){
    big_alloc(get_memory_size());
}

void alloc_alot(){
    void* pointers[NB_POINTERS];
    for (int i=0; i<NB_TESTS; i++) {
        int size;
        if (i < 10)
            size = rand()%1000;
        else if (i < 20)
            size = rand()%2000;
        else if (i < 30)
            size = rand()%10000;
        else if (i < 40)
            size = rand()%40000;
        pointers[i] = alloc(size);
    }
    mem_show(afficher_zone);
    free(pointers[10]);
    alloc(rand()%1000);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    fprintf(stderr, "Test réalisant plusieurs allocations de taille variable.\n"
            "Définir DEBUG à la compilation pour avoir une sortie un peu plus verbeuse."
            "\n");
    mem_init(get_memory_adr(), get_memory_size());

    char* test = (char*) alloc(50);
    for (int i = 0; i < 50; i++){
        test[i] = "K";
    }
    test[49] = 0;
    printf("%s", test);
    
    mem_show(afficher_zone);
    mem_free(test);
    mem_show(afficher_zone);
    //alloc_alot();
    //alloc_big();

    // TEST OK
    return 0;
}
