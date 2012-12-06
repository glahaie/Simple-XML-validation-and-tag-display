/* Chaine.c
 * Par Guillaume Lahaie et Sylvain Labranche
 * LAHG04077707
 *
 * Date de remise: 18 décembre 2012
 *
 * Implantation du module chaine.h
 *
 */

#define PAS 10  //Pour agrandir la chaine si elle est trop petite
#include "chaine.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//Vérifier la différence entre int et size_t
struct chaine {
    char *texte;
    unsigned longueur;
    unsigned longueur_max;
};


Chaine chaineCreeVide() {

    Chaine chaine = (Chaine)malloc(sizeof(struct chaine));

    //Vérifier que les deux alloctions ont fonctionnés
    chaine->texte = (char *)malloc(PAS*sizeof(char));

    //R
    if (!(chaine->texte && chaine))
       return NULL;

    //Insère \0 pour la fin de la chaine.
    chaine->texte[0] = '\0';

   //Sinon, on attribue les valeurs de base à la chaine
   chaine->longueur = 1;
   chaine->longueur_max = PAS;

   return chaine;
}

//Ici j'interprète l'assertion comme quoi n doit être plus petit
//que la chaine passée en argument. Pour pouvoir trouver sa longueur
//j'utilise strlen. J'ajoute plus 1 car n représente le nombre de 
//caractères, il peut ou ne peut pas contenir le \0, donc j'en insère
//un pour m'en assurer.
Chaine chaineCreeCopie(char * ch, unsigned n) {
    
    assert(n <= (strlen(ch)+1) && "n plus grand que la longeur de ch");
    
    Chaine chaine = (Chaine)malloc(sizeof(struct chaine));
    chaine->texte = (char *)malloc((n+1)*sizeof(char)); //+1 pour \0
    chaine->texte[n] = '\0';
    
    if(!(chaine->texte &&  chaine)) {
        return NULL;
    }

    strncpy(chaine->texte, ch, n);

    chaine->longueur = chaine->longueur_max = n+1;

    return chaine;
}

//Ici j'utilise une chaine temporaire pour éviter de perdre la chaine passer en entrée
//si le realloc ne fonctionne pas.
int chaineAjoute(Chaine chaine, unsigned char ch) {

    assert(chaine != NULL  && "Erreur: chaine NULL");

    char * temp; //Pour réalloc: on ne perd pas de données en cas d'échec
    //Vérifie qu'il y a de la place dans la chaine pour un nouveau char.
    if(chaine->longueur >= chaine->longueur_max) {
        temp = (char *)realloc(chaine->texte, (chaine->longueur_max + PAS)*sizeof(char));
        if(!temp)
            return 0; //Problème avec realloc
        if(temp != chaine->texte) {
            chaine->texte = temp;
        }
        chaine->texte = temp;
        chaine->longueur_max += PAS;
    }
    chaine->texte[chaine->longueur-1] = ch;
    chaine->texte[chaine->longueur++] = '\0';
    return 1;
}

//La fonction retourne NULL si l'allocation de mémoire a échoué.
char * chaineValeur(Chaine chaine) {

    assert(chaine != NULL  && "Erreur: chaine NULL");

    char * retour = (char*)malloc((chaineLongueur(chaine)+1)*sizeof(char));
    
    if(!retour) {
        return NULL;
    }

    strcpy(retour, chaine->texte);

    return retour;
}

unsigned chaineLongueur(Chaine chaine) {
    assert(chaine != NULL  && "Erreur: chaine NULL");
    return chaine->longueur-1;
}

void chaineSupprimme(Chaine chaine) {
    assert(chaine != NULL  && "Erreur: chaine NULL");

    free(chaine->texte);
    free(chaine);
    return;
}

