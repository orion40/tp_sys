#include "mem.h"

/* Implémentation de mem.c
 */

/* block libre  */
struct fb{
    size_t size;
    bool is_free;
    struct fb *next_free;
};

typedef struct fb fb;

void mem_init(char* mem, size_t taille){
    /* On cherche à initialiser le tableau mem avec les structures de données
     * nécéssaires, c'est à dire :
     * *next_free | next_free | rest of array
     */

    /* On considère mem comme un pointeur sur un pointeur de
     * fb, et on lui donne l'adresse qui sera juste après
     * nous.
     */

    // TODO: mettre le bool a faux, check si le pointeurs est ok
    fb *first_fb = (fb*) mem + 1;
    *(fb**) mem = first_fb;

    first_fb->size = taille - sizeof(fb) - sizeof(fb*);
    first_fb->next_free = NULL;
}

void* mem_alloc(size_t size){
    /* cast le next_free de fb en (void *) pour eviter un warning */
    return NULL;
}

void mem_free(void *zone){
    /* check les double free */

}

size_t mem_get_size(void *zone){
    // Check avec le prof si ça convertir bien
    // zone en pointeur vers fb et qu'on accede bien à la taille
    return ((fb*)zone - sizeof(size_t))->size;
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
