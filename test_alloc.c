/* Fichier de test
*/

#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NB_TESTS 10

static void *alloc(size_t estimate) {
    void *result;

    debug("Alloced %05zu bytes at %p\n", estimate, result);
    return result;
}

static void *big_alloc(size_t estimate) {
    void *result;
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
    for (int i=0; i<NB_TESTS; i++) {
        int size = rand()%1000;
        alloc(size);
    }
    for (int i=0; i<NB_TESTS; i++) {
        int size = rand()%2000;
        alloc(size);
    }
    for (int i=0; i<NB_TESTS; i++) {
        int size = rand()%10000;
        alloc(size);
    }
    for (int i=0; i<NB_TESTS; i++) {
        int size = rand()%40000;
        alloc(size);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    fprintf(stderr, "Test réalisant plusieurs allocations de taille variable.\n"
            "Définir DEBUG à la compilation pour avoir une sortie un peu plus verbeuse."
            "\n");
    mem_init(get_memory_adr(), get_memory_size());

    alloc_alot();
    alloc_big();

    // TEST OK
    return 0;
}