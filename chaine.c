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
    char *s;
    unsigned int longueur;
    unsigned int longueur_max;
};


Chaine chaineCreeVide() {

    Chaine ch = (Chaine)malloc(sizeof(struct chaine));

    //Vérifier que les deux alloctions ont fonctionnés
    ch->s = (char *)malloc(PAS*sizeof(char));

    //Verifie que l'allocation a fonctionné
    if (ch->s == NULL || ch == NULL)
       return NULL;

   //Sinon, on attribue les valeurs de base à la chaine
   ch->longueur = 0;
   ch->longueur_max = PAS;

   return ch;
}

Chaine chaineCreeCopie(char * ch, unsigned n) {
    
    //Pourrait avoir un assert pour vérifier que ch se termine par \0
    //Et que n est plus grand que 0.
    //assert(n <= (sizeof(ch)/sizeof(char)));

    Chaine chaine = (Chaine)malloc(sizeof(struct chaine));
    chaine->s = (char *)malloc((n)*sizeof(char)); //+1 pour \0
    
    if(chaine->s == NULL || chaine == NULL) {
        return NULL;
    }

    //On pourrait utiliser strncpy, mais le man semble dire que strcopy est meilleur.
    strcpy(chaine->s, ch);

    chaine->longueur = n-1;
    chaine->longueur_max = n;

    return chaine;
}

//Ici j'utilise une chaine temporaire pour éviter de perdre la chaine passer en entrée
//si le realloc ne fonctionne pas.
int chaineAjoute(Chaine chaine, unsigned char ch) {

    //Assert que chaine est pas NULL
    //assert(chaine != NULL);

    //Vérifie qu'il y a de la place dans la chaine pour un nouveau char.
    if(chaine->longueur == chaine->longueur_max) {
        chaine->s = (char *)realloc(chaine->s, (chaine->longueur_max + PAS)*sizeof(char));
        if(chaine->s == NULL)
            return 0;
        chaine->longueur_max += PAS;
        chaine->s[chaine->longueur++] = ch;
        chaine->s[chaine->longueur] = '\0';
        return 1;
    }
    chaine->s[chaine->longueur++] = ch;
    chaine->s[chaine->longueur] = '\0';
    return 1;
}

char * chaineValeur(Chaine chaine) {

    assert(chaine != NULL);

    char * ch = (char*)malloc(chaineLongueur(chaine)*sizeof(char));

    //Quoi faire si malloc ne fonctionne pas? Est-ce qu'on retourne NULL?
    strncpy(ch, chaine->s, chaineLongueur(chaine));

    return ch;
}

unsigned chaineLongueur(Chaine chaine) {
    assert(chaine != NULL);

    return chaine->longueur;
}

void chaineSupprimme(Chaine chaine) {
    assert(chaine != NULL);

    free(chaine->s);
    free(chaine);
    return;
}

