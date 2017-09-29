#include "mem.h"

/* TODO: a virer */
#include "stdio.h"

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
     * | fb | rest of array
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
    mem_fit(mem_fit_first);
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
         /* Création du bloc libre suivant si besoin */
         //(fb*)(result + sizeof(fb) + good_size) 

         (fb*) result->size = good_size;
         (fb*) result->is_free = 0;
         return (void *) (result + sizeof(fb));
     }

     return result;

    /*  Si on trouve (block libre, bonne_taille):
     *  a la fin de adress_trouve + bonne_taille on met le fb avec ntre *fb_next
     *  is_free = 1 et taille ancienne_taille - notre taille
     *  on met ntre taille a bonne_taille
     *  next_block à address_trouve + bonne_taille
     *  is_free à 0
     *  et on renvoie l'adresse (void*) de notre donnée, mais c'est ou ???
     * Si on ne trouve pas:
     *  ??? Alloué plus de mem ?
     *  Renvoie NULL
     */
}

void mem_free(void *zone){
    /* check les double free */
    if((((fb*)zone)->is_free) == 1){
    	printf("déjà libre");//pour voir le cas dans les tests
    	return;
    }
    /*pour vérifier si le bloc précedent est libre doit parcourir la file
    pour le trouver*/
    /*
    fb *prec=NULL;//on mémorise le bloc précédent
    while(courant!=zone && courant!=NULL){
    	prec=courant;
    	courant=courant-> next_block;
    }
    if(courant==NULL){
    	printf("bloc inexistant");//pour voir le cas dans les tests
    	return;
    }
    if(prec->is_free==1){
    	if((courant->next_block)==NULL || (courant->next_block)->isfree==0){
    		//prec libre,suiv occ ou null
    		prec->size=(prec->size)+(courant->size);
    		prec->next_block=courant->next_block;
    	}else{
    		//prec et suiv libres
    		prec->size=(prec->size)+(courant->size)+(courant->next_block->size);
    		prec->next_block=(courant->next_block)->next_block;
    	}
    }else{
    	courant->is_free=1;
    	if(){
    		//prec occ,suiv libre
    		//prec->size=;
    		//prec->next_block=;
    	}else if(){
    		//prec occ et suiv null
    		//prec->size=;
    		//prec->next_block=;
    	}else{
    		//prec et suiv occ
    		//prec->size=;
    		//prec->next_block=;
    	}
    }
    */
}

size_t mem_get_size(void *zone){
    // Check avec le prof si ça convertir bien
    // zone en pointeur vers fb et qu'on accede bien à la taille
    return ((fb*)zone - sizeof(size_t))->size;
}

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free)){
    fb* current_block = block_list_start;
    while (current_block != NULL){
        print(current_block + sizeof(fb), current_block->size, current_block->is_free);
        current_block = current_block->next_block;
    }
}

void mem_fit(mem_fit_function_t* fit_func){
    search_func = fit_func;
}

/*
 * TODO: plan de comment on fait ça
 * */
struct fb* mem_fit_first(struct fb *list, size_t size){
    fb* current_block = list;
    while ((current_block->size < size || current_block->is_free == 0) && current_block != NULL){
        current_block = current_block->next_block;
    }

    return current_block;
}

/* Si vous avez le temps */
struct fb* mem_fit_best(struct fb *list, size_t size){
    return NULL;
}

struct fb* mem_fit_worst(struct fb *list, size_t size){
    return NULL;
}
