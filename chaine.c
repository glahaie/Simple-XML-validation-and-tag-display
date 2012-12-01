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

struct chaine {
    void * tete;
    int longueur;
};


Chaine CreeVide() {

    Chaine chaine;
    chaine->tete = (char *)malloc(PAS*sizeof(char));

    //Verifie 
