#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"
#include "arbres.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t* L) {
    L->tete = NULL;
}

void liberer_liste(liste_t* L) {
    if(L == NULL) return;
    cellule_t *c = L->tete;
    //free(L);
    while(c)
    {
        cellule_t *suiv = c->suivant;
        //free(c->val);
        free(c);
        c = suiv;
    }
}


int ajouter_tete(liste_t* L, string c) { /* retourne 0 si OK, 1 sinon  */
    /* a completer */
    if(!L) return 1;
    cellule_t *cel = malloc(sizeof(cellule_t));
    cel->val = c;
    cel->suivant = L->tete;
    L->tete = cel;
    return 0;
}

int enfiler(file_t *F, arbre a)
{
    if(!F) return 1;
    if(!F->tete)
    {
        cellule_f *cel = malloc(sizeof(cellule_f));
        cel->val = a;
        cel->suivant = F->tete;
        F->tete = cel;
        F->queue = cel;
    }
    else
    {
        cellule_f *cel = malloc(sizeof(cellule_f));
        cel->val = a;
        cel->suivant = NULL;
        F->queue->suivant = cel;
        F->queue = cel;
    }
    F->taille++;
    return 0;
}

int defiler(file_t *F, arbre *a)
{
    if(!F || !F->tete) return 1;
    *a = F->tete->val;
    if(F->tete == F->queue)
    {
        F->tete = NULL;
        F->queue = NULL;
    }
    else
    {
        F->tete = F->tete->suivant;
    }
    F->taille--;
    return 0;
}
