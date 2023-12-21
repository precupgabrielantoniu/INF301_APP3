#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"

void analyse_arbre (arbre racine, int* nb_esp, int* nb_carac)
{
    if (racine == NULL){
        *nb_carac = 0;
        *nb_esp = 0;
        return;
    }
    int espg, caracg;
    int espd, caracd;
    analyse_arbre(racine->droit, &espd, &caracd);
    analyse_arbre(racine->gauche, &espg, &caracg);

    if (racine->droit == NULL && racine->gauche == NULL) {
        *nb_esp = 1 + espd + espg;
        *nb_carac = caracd + caracg;
    }
    else {
        *nb_carac = 1 + caracd + caracg;
        *nb_esp = espd + espg;
    }
}




/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */

int rechercher_espece_noeud(arbre racine, char *espece, liste_t *seq)
{
    if (racine == NULL)
        return 1;
    if (!strcmp(racine->valeur, espece))
        return 0;
    int retrouvee = 1;
    if (racine->gauche)
    {
        retrouvee = rechercher_espece_noeud(racine->gauche, espece, seq);
        if (retrouvee == 0)
        {
            return retrouvee;
        }
    }
    if (racine->droit)
    {
        retrouvee = rechercher_espece_noeud(racine->droit, espece, seq);
        if (retrouvee == 0)
        {
            ajouter_tete(seq, racine->valeur);
            return retrouvee;
        }
    }
    return 1;
}

int rechercher_espece(arbre racine, char *espece, liste_t *seq)
{
    if (rechercher_espece_noeud(racine, espece, seq))
    {
        printf("L'espece %s n'a pas ete retrouvee.\n", espece);
        return 1;
    }
    return 0;
}

/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un
    * message d'erreur.
    */
int ajouter_espece(arbre * a, char *espece, cellule_t *seq)
{
    if (!(*a)) // arbre vide
    {
        if (seq)
        {
            *a = nouveau_noeud();
            (*a)->valeur = seq->val;
            seq = seq->suivant;
            return ajouter_espece(&((*a)->droit), espece, seq);
        }
        else
        {
            *a = nouveau_noeud();
            (*a)->valeur = espece;
            return 0;
        }
    }
    else
    {
        if (!(*a)->gauche && !(*a)->droit)
        {
            if (!seq)
            {
                printf("Ne peut ajouter %s: possède les mêmes caractères que %s.", espece, (*a)->valeur);
                return 1;
            }
            else
            {
                char *esp = (*a)->valeur;
                (*a)->valeur = seq->val;
                (*a)->gauche = nouveau_noeud();
                (*a)->gauche->valeur = esp;
                seq = seq->suivant;
                return ajouter_espece(&((*a)->droit), espece, seq);
            }
        }
        if (seq)
        {
            if (!strcmp((*a)->valeur, seq->val))
            {
                seq = seq->suivant;
                return ajouter_espece(&((*a)->droit), espece, seq);
            }
            else
            {
                return ajouter_espece(&((*a)->gauche), espece, seq);
            }
        }
        else
        {
            return ajouter_espece(&((*a)->gauche), espece, seq);
        }
    }
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
    * à droite, dans le fichier fout.
    * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
    */
void afficher_par_niveau(arbre racine, FILE * fout)
{
    if (!racine)
        return;
    file_t F;
    F.tete = NULL;
    F.queue = NULL;
    F.taille = 0;
    if (racine->droit || racine->gauche)
        enfiler(&F, racine);
    while (F.tete)
    {
        int largeur = F.taille; // compter le nombre d'éléments dans le niveau

        for (int i = 0; i < largeur; ++i)
        {
            arbre a;
            defiler(&F, &a);
            fprintf(fout, "%s ", a->valeur);

            if (a->gauche && (a->gauche->droit || a->gauche->gauche))
            {
                enfiler(&F, a->gauche);
            }
            if (a->droit && (a->droit->droit || a->droit->gauche))
            {
                enfiler(&F, a->droit);
            }
        }

        fprintf(fout, "\n");
    }
}

// Acte 4

int path_to_noeud(arbre racine, char *espece, liste_t *seq)
{
    if (racine == NULL)
        return 1;
    if (!strcmp(racine->valeur, espece))
        return 0;
    int retrouvee = 1;
    if (racine->gauche)
    {
        retrouvee = path_to_noeud(racine->gauche, espece, seq);
        if (retrouvee == 0)
        {
            ajouter_tete(seq, "N");
            return retrouvee;
        }
    }
    if (racine->droit)
    {
        retrouvee = path_to_noeud(racine->droit, espece, seq);
        if (retrouvee == 0)
        {
            ajouter_tete(seq, "O");
            return retrouvee;
        }
    }
    return 1;
}

//sequence path est une sequence qui ne contient que des "N" et des "O"

int suivre_path(arbre ar, liste_t * path, arbre *res)
{
    if (!ar || !path) {
        printf("error in suivre_path\n");
        return 1;
    }
    *res = ar;
    cellule_t *t = path->tete;
    while (t != NULL)
    {
        if (!strcmp(t->val, "O"))
            *res = (*res)->droit;
        else if (!strcmp(t->val, "N"))
            *res = (*res)->gauche;
        t = t->suivant;
    }
    return 0;
}

int parent_commun(arbre ar, arbre *res, string esp1, string esp2)
{
    if (!ar)
        return 1;

    liste_t *path1 = malloc(sizeof(liste_t)), *path2 = malloc(sizeof(liste_t)), *path_final = malloc(sizeof(liste_t));
    init_liste_vide(path1);
    init_liste_vide(path2);
    init_liste_vide(path_final);

    if (path_to_noeud(ar, esp1, path1))
    {
        printf("Espece 1 n'a pas ete trouvee.\n");
        return 1;
    }
    if (path_to_noeud(ar, esp2, path2))
    {
        printf("Espece 2 n'a pas ete trouvee.\n");
        return 1;
    }

    if (strcmp(path1->tete->val, path2->tete->val))
    {
        *res = ar;
        return 1;
    }

    cellule_t *t1 = path1->tete;
    cellule_t *t2 = path2->tete;
    do
    {
        if (strcmp(t1->val, t2->val) != 0)
            break;
        else if (t2->suivant == NULL)
        {
            ajouter_tete(path_final, t2->val);
            break;
        }
        else if (t1->suivant == NULL)
        {
            ajouter_tete(path_final, t1->val);
            break;
        }
        ajouter_tete(path_final, t1->val);

        t1 = t1->suivant;
        t2 = t2->suivant;

    } while (t1 != NULL && t2 != NULL);
    affiche_liste(path_final);
    suivre_path(ar, path_final, res);

    return 0;
}

int ajouter_carac(arbre * a, char *carac, cellule_t *seq)
{
    if (!*a || !seq)
        return 0;
    int nb_esp = 1, nb_vraie, nb_car;

    string n1 = seq->val;
    arbre *parent = malloc(sizeof(arbre));
    seq = seq->suivant;
    while (seq != NULL)
    {
        string n2 = seq->val;
        seq = seq->suivant;
        nb_esp++;
        printf("avant\n");
        parent = malloc(sizeof(arbre));
        parent_commun(*a, parent, n1, n2);
        printf("apres\n");
        n1 = (*parent)->valeur;
    }
    printf("hi\n");
    analyse_arbre(*parent, &nb_vraie, &nb_car);
    printf("ici\n");
    if (nb_vraie != nb_esp)
    {
        printf("Ne peut ajouter %s: ne forme pas un sous-arbre.\n", carac);
        return 0;
    }
    printf("hi\n");
    arbre nouveau_ar = nouveau_noeud();
    printf("new treee\n");
    nouveau_ar->valeur = (*parent)->valeur;
    nouveau_ar->droit = (*parent)->droit;
    nouveau_ar->gauche = (*parent)->gauche;
    printf("new tree done\n");

    (*parent)->valeur = carac;
    (*parent)->droit = nouveau_ar;
    (*parent)->gauche = NULL;
    printf("hello\n");
    return 1;
}

///////////////////// les fonctions pour fich .dot

void afficher_noeud_dot(arbre nd, FILE * f_dot)
{
    if (!nd)
        return;
    if (!nd->droit && !nd->gauche)
        return;
    if (nd->gauche)
    {
        fprintf(f_dot, "\t%s -> %s [label = \"non\"]\n", nd->valeur, nd->gauche->valeur);
        afficher_noeud_dot(nd->gauche, f_dot);
    }
    if (nd->droit)
    {
        fprintf(f_dot, "\t%s -> %s [label = \"oui\"]\n", nd->valeur, nd->droit->valeur);
        afficher_noeud_dot(nd->droit, f_dot);
    }
}

void afficher_dot(arbre racine, FILE * f_dot)
{
    fprintf(f_dot, "digraph arbre {\n");
    afficher_noeud_dot(racine, f_dot);
    fprintf(f_dot, "}");
}
