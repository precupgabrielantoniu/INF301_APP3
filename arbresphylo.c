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
int rechercher_espece (arbre racine, char *espece, liste_t* seq)
{
    if(racine == NULL) return 1;
    if(!strcmp(racine->valeur, espece)) return 0;
    int retrouvee = 1;
    if(racine->gauche)
    {
      retrouvee = rechercher_espece(racine->gauche, espece, seq);
      if(retrouvee == 0)
      {
         return retrouvee;
      }
    }
    if(racine->droit)
    {
      retrouvee = rechercher_espece(racine->droit, espece, seq);
      if(retrouvee == 0)
      {
         ajouter_tete(seq, racine->valeur);
         return retrouvee;
      }
    }
    printf("L'espece %s n'as pas ete retrouvee.\n", espece);
    return 1;
}



/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un 
 * message d'erreur.
 */
int ajouter_espece (arbre* a, char *espece, cellule_t* seq) {
   if(!seq)
   {
      if(!(*a))
      {
         if((*a)->gauche)
         {
            printf("Ne peut ajouter %s: possède les mêmes caractères que %s.", espece, (*a)->gauche->valeur);
            return 1;
         }
         noeud* nd_nouv = malloc(sizeof(noeud));
         nd_nouv->valeur = espece;
         nd_nouv->droit = NULL;
         nd_nouv->gauche = NULL;
         (*a)->gauche = nd_nouv;
         return 0;
      }
      else
      {
         printf("Ne peut ajouter %s: l'arbre est null.", espece);
         return 1;
      }
   }
   else 
   return 1;
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
void afficher_par_niveau (arbre racine, FILE* fout) {
   printf ("<<<<< À faire: fonction afficher_par_niveau fichier " __FILE__ "\n >>>>>");
}

// Acte 4


int ajouter_carac(arbre* a, char* carac, cellule_t* seq) {
   printf ("<<<<< À faire: fonction ajouter_carac fichier " __FILE__ "\n >>>>>");
   return 0;
}

void afficher_noeud_dot(arbre nd, FILE* f_dot)
{
   if(!nd->droit && !nd->gauche) return;
   if(nd->droit)
   {
      fprintf(f_dot, "\t%s -> %s [label = \"non\"]\n", nd->valeur, nd->droit->valeur);
      afficher_noeud_dot(nd->droit, f_dot);
   }
   if(nd->gauche)
   {
      fprintf(f_dot, "\t%s -> %s [label = \"oui\"]\n", nd->valeur, nd->gauche->valeur);
      afficher_noeud_dot(nd->gauche, f_dot);
   }
}

void afficher_dot(arbre racine, FILE* f_dot)
{
   fprintf(f_dot, "digraph arbre {\n");
   afficher_noeud_dot(racine, f_dot);
   fprintf(f_dot, "}");
}
