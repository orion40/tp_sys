#include "mem.h"

/* Implémentation de mem.c
 */

/* on est sur du code existant, li le bien avant 
 * comment mieux se débrouiller en prog systeme ?
 * ressources ?
 */

struct fb{
    size_t size;
    struct fb *next_free;
};

void mem_init(char* mem, size_t taille){
    *(int*) mem = taille;
    /*
    mem->size = taille;
    mem->next_free = NULL;
    */
}

void* mem_alloc(size_t size){

}

void mem_free(void *zone){
    /* check les double free */

}

size_t mem_get_size(void *zone){

}


/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free)){

}

struct fb* mem_fit_first(struct fb *list, size_t size){

}

/* Si vous avez le temps */
struct fb* mem_fit_best(struct fb *list, size_t size){

}

struct fb* mem_fit_worst(struct fb *list, size_t size){

}
