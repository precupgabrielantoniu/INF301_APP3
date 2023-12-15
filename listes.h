#ifndef _LISTES_H
#define _LISTES_H

#include "arbres.h"

typedef struct liste liste_t;
/* Type de liste à compléter selon votre besoin. */

typedef char* string;

struct cellule {
	string val;
	struct cellule* suivant;
};

typedef struct cellule cellule_t;

struct liste {
	cellule_t *tete;
};

typedef struct liste liste_t;

struct cellule_file {
	arbre val;
	struct cellule_file* suivant;
};

typedef struct cellule_file cellule_f;

struct file {
	cellule_f* tete;
	cellule_f* queue;
	int taille;
};

typedef struct file file_t;

/* cree une nouvelle liste, initialement vide */
void init_liste_vide(liste_t* L);

/* libère toutes les cellules de la liste */
void liberer_liste(liste_t *L);

/* Ajouter une nouvelle cellule contenant c
 * en tête de la liste L.
 * Si l'ajout est réussi, le résultat est 0,
 * et 1 sinon (échec de l'ajout)
 */
int ajouter_tete(liste_t *L, string c);

int enfiler(file_t *F, arbre a);

int defiler(file_t *F, arbre *a);


#endif /* _LISTES_H */


