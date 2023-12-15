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


int rechercher_espece_noeud (arbre racine, char *espece, liste_t* seq)
{
    if(racine == NULL) return 1;
    if(!strcmp(racine->valeur, espece)) return 0;
    int retrouvee = 1;
    if(racine->gauche)
    {
      retrouvee = rechercher_espece_noeud(racine->gauche, espece, seq);
      if(retrouvee == 0)
      {
         return retrouvee;
      }
    }
    if(racine->droit)
    {
      retrouvee = rechercher_espece_noeud(racine->droit, espece, seq);
      if(retrouvee == 0)
      {
         ajouter_tete(seq, racine->valeur);
         return retrouvee;
      }
    }
    return 1;
}




/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */
int rechercher_espece(arbre racine, char *espece, liste_t* seq)
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
int ajouter_espece (arbre* a, char *espece, cellule_t* seq) {
   if(!(*a)) // arbre vide
   {
      if(seq)
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
      if(!(*a)->gauche && !(*a)->droit)
      {
         if(!seq)
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
      if(seq)
      {
         if(!strcmp((*a)->valeur,seq->val))
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
void afficher_par_niveau (arbre racine, FILE* fout) {
   if(!racine) return;
   file_t F;
   F.tete = NULL;
   F.queue = NULL;
   F.taille = 0;
   if(racine->droit || racine->gauche) enfiler(&F, racine);
   while (F.tete) {
        int largeur = F.taille; // compter le nombre d'éléments dans le niveau

        for (int i = 0; i < largeur; ++i) {
            arbre a;
            defiler(&F, &a);
            fprintf(fout, "%s ", a->valeur);

            if (a->gauche && (a->gauche->droit || a->gauche->gauche)) {
                enfiler(&F, a->gauche);
            }
            if (a->droit && (a->droit->droit || a->droit->gauche)) {
                enfiler(&F, a->droit);
            }
        }

        fprintf(fout, "\n"); 
    }
}

// Acte 4


int ajouter_carac(arbre* a, char* carac, cellule_t* seq) {
   printf ("<<<<< À faire: fonction ajouter_carac fichier " __FILE__ "\n >>>>>");
   return 0;
}

void afficher_noeud_dot(arbre nd, FILE* f_dot)
{
   if(!nd) return;
   if(!nd->droit && !nd->gauche) return;
   if(nd->gauche)
   {
      fprintf(f_dot, "\t%s -> %s [label = \"non\"]\n", nd->valeur, nd->gauche->valeur);
      afficher_noeud_dot(nd->gauche, f_dot);
   }
   if(nd->droit)
   {
      fprintf(f_dot, "\t%s -> %s [label = \"oui\"]\n", nd->valeur, nd->droit->valeur);
      afficher_noeud_dot(nd->droit, f_dot);
   }
}

void afficher_dot(arbre racine, FILE* f_dot)
{
   fprintf(f_dot, "digraph arbre {\n");
   afficher_noeud_dot(racine, f_dot);
   fprintf(f_dot, "}");
}
