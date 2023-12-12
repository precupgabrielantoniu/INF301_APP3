#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t* L) {
    L = malloc(sizeof(liste_t));
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

