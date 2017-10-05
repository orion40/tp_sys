/* Fichier de test
*/

#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NB_TESTS 20
#define NB_POINTERS 40

static void *alloc(size_t estimate) {
    void *result;
    result = mem_alloc(estimate);

    debug("Alloced %zu bytes at %p\n", estimate, result);
    return result;
}

void afficher_zone(void *adresse, size_t taille, int free)
{
    printf("Zone %s, Adresse : %lx, Taille : %zu\n", free?"libre":"occupee",
            (unsigned long) adresse, (unsigned long) taille);
}

void alloc_big(){
    printf("Allocation de toute la mémoire (%zu)...\n", get_memory_size());
    mem_show(afficher_zone);
    void* big = alloc(get_memory_size());
    mem_free(big);
    mem_show(afficher_zone);
}

void alloc_alot(){
    void* pointers[NB_POINTERS];
    printf("Allocation des %d blocs...\n", NB_POINTERS);
    for (int i=0; i<NB_POINTERS; i++) {
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
    printf("Libération des %d blocs...\n", NB_POINTERS);
    for (int i=0; i<NB_POINTERS; i++) {
        mem_free(pointers[i]);
    }
    mem_show(afficher_zone);
}



int main(int argc, char *argv[]) {
    srand(time(NULL));
    fprintf(stderr, "Test réalisant plusieurs allocations de taille variable.\n");
    mem_init(get_memory_adr(), get_memory_size());

    for (int i = 0; i < NB_TESTS; i++){
        printf("Allocation stress test #%d\n", i);
        alloc_alot();
    }
    alloc_big();



    // TEST OK
    return 0;
}
