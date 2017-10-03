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

/* Iitialisation de la mémoire : on crée la première structure fb
 */
void mem_init(char* mem, size_t taille){
    fb *first_fb = (fb*) mem ;

    first_fb->size = taille;
    first_fb->next_block = NULL;
    first_fb->is_free = 1;

    block_list_start = (fb*) mem;
    max_size = first_fb->size;

    mem_fit(mem_fit_first);
}

void* mem_alloc(size_t size){
    /* D'abord on calcule la bonne_taille multiple de 2 la plus proche de
     * size + sizeof(fb).
     */
    size_t real_size = size + sizeof(fb);
    size_t good_size = 32; /* on commence à 32 car la taille réelle est 
                             forcement supérieure ou égale , vu que la 
                             struct est de taille 24 */
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
     * en utilisant la fonction choisie; si on ne trouve pas, result
     * vaudra NULL et sera renvoyé à l'utilisateur.
     */
     result = search_func(block_list_start, good_size);
     if (result != NULL){
         /* Création du bloc libre suivant s'il reste de la place 
          * en mémoire */
         if (((fb*)result)->size - good_size > sizeof(fb) + 4){
             ((fb*)(result + good_size))->size =
                 ((fb*)result)->size - good_size;

             ((fb*)(result + good_size))->is_free = 1;

             ((fb*)(result + good_size))->next_block =
                 ((fb*)result)->next_block;
         }
         /* On met à jour le bloc que l'on a récupéré avec sa taille,
          * son état (occupé), et l'adresse du prochain block
          */

         ((fb*)result)->size = good_size;
         ((fb*)result)->is_free = 0;
         ((fb*)result)->next_block = result + good_size;

         return (void *) (result + sizeof(fb));
     }

     return result;
}

void mem_free(void *zone){
    /* vérifie si on essaye de libéré un block null */
    if (zone == NULL) return;

    zone = zone - sizeof(fb);
    /* vérifie les double free */
    if((((fb*)zone)->is_free) == 1){
        printf("déjà libre\n");//pour voir le cas dans les tests
        return;
    }
    /*pour vérifier si le bloc précedent est libre doit parcourir la file
      pour le trouver*/
    fb *courant=block_list_start;
    fb *prec=NULL, *next = NULL;//on mémorise le bloc précédent
    while(courant!=zone && courant!=NULL){
        prec=courant;
        courant=courant-> next_block;
    }
    if(courant==NULL){
        printf("bloc inexistant\n");//pour voir le cas dans les tests
        return;
    }
    if (prec != NULL){
        if(prec->is_free==1){
            if((courant->next_block)==NULL || (courant->next_block)->is_free==0){
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
            if(courant->next_block!=NULL && (courant->next_block)->is_free==1){
                //prec occ,suiv libre
                courant->size=(courant->size)+(courant->next_block)->size;
                courant->next_block=(courant->next_block)->next_block;
            }
            //prec occ et suiv null->rien de plus à faire
            //prec et suiv occ->rien de plus à faire
        }
    }else{
        /* cas ou l'on libère le premier bloc */
        courant->is_free = 1;
        next = courant->next_block;
        if (next != NULL){
            if (next->is_free == 1){
                courant->size += next->size;
                courant->next_block = next->next_block;
            }
        }
    }
}

/* la zone envoyée en paramètre est ce que l'utilisateur peut utiliser,
 * il faut donc décaler de sizeof(fb) pour accéder à la vrai structure
 */
size_t mem_get_size(void *zone){
    return ((fb*)(zone - sizeof(fb)))->size;
}

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free)){
    fb* current_block = block_list_start;
    while (current_block != NULL){
        print(current_block + sizeof(fb), current_block->size, current_block->is_free);
        current_block = current_block->next_block;
    }
}

/* notre fonction de recherche est mis à jour
 */
void mem_fit(mem_fit_function_t* fit_func){
    search_func = fit_func;
}

/* On parcourt la liste en testant si les blocs on suffisament d'espace, 
 * et qu'ils sont libres. On fait attention a tester si current_block
 * est NULL, afin d'éviter un segfault.
 */
struct fb* mem_fit_first(struct fb *list, size_t size){
    fb* current_block = list;
    if (current_block != NULL){
        while ((current_block->size < size || current_block->is_free == 0)){
            current_block = current_block->next_block;
            if (current_block == NULL) break;
        }
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
