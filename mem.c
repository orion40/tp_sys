#include "mem.h"

/* Implémentation de mem.c
 */

struct fb{
    size_t size;
    struct fb *next_free;
};

typedef struct fb fb;

void mem_init(char* mem, size_t taille){
    /* On cherche à initialiser le tableau mem avec les structures de données
     * nécéssaires, c'est à dire :
     * *next_free | size | *next_free | rest of array
     */

    /* On considère mem comme un pointeur sur un pointeur de
     * fb, et on lui donne l'adresse qui sera juste après
     * nous.
     */

    fb *first_fb = (fb**) mem + 1;
    *(fb**) mem = first_fb;

    first_fb->size = taille - sizeof(fb) - sizeof(fb*);
    first_fb->next_free = NULL;
    /*
    mem->size = taille;
    mem->next_free = NULL;
    */
}

void* mem_alloc(size_t size){
    return NULL;
}

void mem_free(void *zone){
    /* check les double free */

}

size_t mem_get_size(void *zone){
    // Check avec le prof si ça convertir bien
    // zone en pointeur vers fb et qu'on accede bien à la taille
    return ((fb*)zone)->size;
}


/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free)){

}

struct fb* mem_fit_first(struct fb *list, size_t size){
    return NULL;
}

/* Si vous avez le temps */
struct fb* mem_fit_best(struct fb *list, size_t size){
    return NULL;
}

struct fb* mem_fit_worst(struct fb *list, size_t size){
    return NULL;
}
