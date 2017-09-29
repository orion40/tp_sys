#include "mem.h"

/* Implémentation de mem.c
 */

/* structure partagée entre blocs libres et occupés
blocs libres bool=1; blocs occupés bool=0  */
struct fb{
    size_t size;
    int is_free; /* 1 = libre, 0 = busy */
    struct fb *next_block;
};

typedef struct fb fb;

/* TODO: demander au prof si le nom est clair ou pas */
fb* block_list_start = NULL;
size_t max_size = 0;
fb* (*search_func)(fb*, size_t);


/* TODO: is this correct ? */
/* Si on s'en sert, go le mettre en MACRO */
fb* get_next(fb* block){
    if (block == NULL)
        return NULL;
    return block->next_block;
}

void mem_init(char* mem, size_t taille){
    /* On cherche à initialiser le tableau mem avec les structures de données
     * nécéssaires, c'est à dire :
     * *next_block | next_block | rest of array
     */

    /* On considère mem comme un pointeur sur un pointeur de
     * fb, et on lui donne l'adresse qui sera juste après
     * nous.
     */

    fb *first_fb = (fb*) mem ;

    first_fb->size = taille - sizeof(fb);
    first_fb->next_block = NULL;
    first_fb->is_free = 1;

    block_list_start = (fb*) mem;
    max_size = first_fb->size;

    /* Fonction de recherche : first fit */
    search_func = mem_fit_first;
}

void* mem_alloc(size_t size){
    /* cast le next_free de fb en (void *) pour eviter un warning */
    /* D'abord on calcule la bonne_taille multiple de 2 la plus proche de
     * size + sizeof(fb).
     */
    /* TODO: on utilise le *fb de fb donc il faut enlever son sizeof */
    size_t real_size = size + sizeof(fb);
    size_t good_size = 4; /* on commence à 4 car la taille réelle est 
                             forcement supérieure */
    void* result = NULL;
    /* size est trop grand */
    if ( real_size > max_size) return result;

    /* calcul la taille optimale */
    while(good_size < real_size){
        good_size = good_size << 1;
    }
    
    /* la taille optimale est trop grande */
    if (good_size > max_size) return result;

    /* Ensuite, on cherche un bloc libre égal ou supérieur à cette taille
     * en utilisant la fonction choisie.
     */

     result = search_func(block_list_start, good_size);
     if (result != NULL){
         /* TODO: MAJ les blocs ! */
         return (void *) (result + sizeof(fb));
     } else {
        return result;
     }

    /*  Si on trouve (block libre, bonne_taille):
     *  a la fin de adress_trouve + bonne_taille on met le fb avec ntre *fb_next
     *  is_free = 1 et taille ancienne_taille - notre taille
     *  on met ntre taille a bonne_taille
     *  next_block à address_trouve + bonne_taille
     *  is_free à 0
     *  et on renvoie l'adresse (void*) de notre donnée, mais c'est ou ???
     *  TODO: prbleme de conception ! on met ou les donnes du bloc ?
     * Si on ne trouve pas:
     *  ??? Alloué plus de mem ?
     *  Renvoie NULL
     */

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
