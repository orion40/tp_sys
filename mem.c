#include "mem.h"

/* TODO: a virer */
#include "stdio.h"

/* Implémentation de mem.c
 */

/* structure partagée entre blocs libres et occupés
blocs libres bool=1; blocs occupés bool=0  */
struct fb{
    size_t size;
    struct fb *next_block;
};

/* structure de blocs alloués
 */
struct ab{
    size_t size;
    struct ab *next_busy;
};

typedef struct fb fb;
typedef struct ab ab;

fb* free_block_list_start = NULL;
ab* allocated_block_list_start = NULL;
size_t max_size = 0;
fb* (*search_func)(fb*, size_t);

/* Iitialisation de la mémoire : on crée la première structure fb
 */
void mem_init(char* mem, size_t taille){
    fb *first_fb = (fb*) mem;

    first_fb->size = taille;
    first_fb->next_block = NULL;

    free_block_list_start = (fb*) mem;
    max_size = first_fb->size;

    /* aucun bloc n'est alloué pour le moment */
    allocated_block_list_start = NULL;

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
     result = search_func(free_block_list_start, good_size);
     if (result != NULL){
         /* Création du bloc libre suivant s'il reste de la place 
          * en mémoire */
         if (((fb*)result)->size - good_size > sizeof(fb) + 4){
             /* TODO: est-ce qu'on devrait vérifier que le prochain bloc n'est pas occupé ? */
             ((fb*)(result + good_size))->size =
                 ((fb*)result)->size - good_size;

             ((fb*)(result + good_size))->next_block =
                 ((fb*)result)->next_block;
         }
         /* On met à jour le bloc que l'on a récupéré avec sa taille,
          * son état (occupé), et l'adresse du prochain block
          */

         /* TODO : fix this;
          * comment alloué de la mémoire et mettre à jours correctement
          * les blocs libres et les blocs non alloués ?
          */

         /* est-ce que je ne risque pas d'écraser des données, le
          * ab/fb suivant ?
          */
         ((fb*)result)->next_block = result + good_size;
         free_block_list_start = ((fb*)result)->next_block;

         ((ab*)result)->size = good_size;
         ((ab*)result)->next_busy = allocated_block_list_start;
         allocated_block_list_start = ((ab*)result);


         return (void *) (result + sizeof(fb));
     }

     return result;
}

void mem_free(void *zone){
    zone = zone - sizeof(ab);
    /* vérifie les double free */
    // est-ce que le bloc que l'on libère est dans les alloués ?
    // TODO: il faut parcourir la BONNE liste pour les blocs libres !!!

    /*pour vérifier si le bloc précedent est libre doit parcourir la file
      pour le trouver*/
    fb *fb_courant = free_block_list_start;
    fb *fb_prec = NULL, *fb_suivant = NULL;

    ab *ab_courant = allocated_block_list_start;
    ab *ab_prec=NULL, *next = NULL;//on mémorise le bloc précédent

    /* il faudrait ajouter le bloc que l'on libère à la liste des 
     * blocks libres.
     */

    // TODO : si plus de place, on donne une erreur avec le search_func !

    while(courant != zone && courant != NULL){
        prec=courant;
        courant=courant->next_busy;
    }
    if(courant == NULL){
        printf("bloc inexistant\n");//pour voir le cas dans les tests
        return;
    }
    if (prec != NULL){
        /* On teste si le bloc occupé précédent est bien à coté de nous */
        if(prec + prec->size == courant){
            if((courant->next_busy) == NULL || (courant->next_busy) != (courant + courant->size)){
                //prec libre,suiv occ ou null
                prec->size=(prec->size)+(courant->size);
                prec->next_busy=courant->next_busy;
            }else{
                //prec et suiv libres
                prec->size += (courant->size)+(courant->next_busy->size);
                prec->next_busy=(courant->next_busy)->next_busy;
            }
        }else{
            /* le bloc précédent n'est pas occupé */
            if(courant->next_busy!=NULL && (courant->next_busy) != (courant + courant->size)){
                //prec occ,suiv libre
                courant->size=(courant->size)+(courant->next_busy)->size;
                courant->next_busy=(courant->next_busy)->next_busy;
            }
            //prec occ et suiv null->rien de plus à faire
            //prec et suiv occ->rien de plus à faire
        }
    }else{
        /* cas ou l'on libère le premier bloc */
        next = courant->next_busy;
        if (next != NULL){
            if (next == courant + courant->size){
                courant->size += next->size;
                courant->next_busy = next->next_busy;
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
    fb* current_free_block = free_block_list_start;
    ab* current_busy_block = allocated_block_list_start;
    int free = 0, busy = 0;
    while (current_free_block != NULL){
        free++;
        print(current_free_block + sizeof(fb), current_free_block->size, 1);
        current_free_block = current_free_block->next_block;
    }

    printf("%d blocs libres détectés.\n", free);

    while (current_busy_block != NULL){
        busy++;
        print(current_busy_block + sizeof(ab), current_busy_block->size, 0);
        current_busy_block = current_busy_block->next_busy;
    }

    printf("%d blocs occupés détectés.\n", busy);
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
        while ((current_block->size < size)){
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
