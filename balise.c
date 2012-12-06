/* balise.c
 * Par Guillaume Lahaie et Sylvain Labranche
 *     LAHG04077707
 *
 * Date de remise: 18 décembre 2012
 *
 * Implémentation du fichier en-tête balise.h
 *
 */

#include "balise.h"
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


struct balise {
    Chaine nom;
    Chaine attribut;
    TypeBalise type;
};

TypeBalise obtenirType(char *nom);


//Vérifie si le malloc fonctionne bien. Retoune NULL dans ce cas.
//Pour le moment je suppose que la Chaine nom passée contient encore
//les < et >. Le nom doit contenir au moins un caractère.
Balise baliseCree(Chaine nom) {

    Balise balise = (Balise)malloc(sizeof(struct balise));
    char * jeton;                           //pour strtok
    char * attributs;                       //pour les attributs, si nécessaire
    char * valeurNom = chaineValeur(nom);

    //A valider - quoi faire si malloc ne fonctionne pas
    if(balise == NULL || valeurNom == NULL) {
        baliseSupprimme(balise);
        return NULL;
    }

    //Obtenir le type, si possible
    balise->type = obtenirType(valeurNom);
   
    if(balise->type == COMMENTAIRES || balise->type== DIRECTIVE) {
        balise->nom = balise->attribut = NULL;
    } else if (balise->type == FIN) {
        //Trouver le nom à l'aide de jetons. Comme une balise fin n'a
        //d'attributs, on n'y porte pas attention.
        jeton = strtok(&valeurNom[2], ">");
        balise->nom = chaineCreeCopie(jeton, strlen(jeton));
        balise->attribut = NULL;
    } else {
        //C'est un début/ debutfin
        jeton = strtok(&valeurNom[1], " >");
        balise->nom = chaineCreeCopie(jeton, strlen(jeton));
         
        //Traitement des attributs, si nécessaire. La longueur des
        //attributs ne peut pas être plus long de la longueur de nom.
        if((jeton = strtok(NULL, " >")) != NULL) {

            attributs = (char *)malloc(chaineLongueur(nom)*sizeof(char));
            if(!attributs) {
                baliseSupprimme(balise);
                return NULL;
            }
            attributs[0] = '\0';
            while (jeton != NULL) {
                strcat(attributs, jeton);
                strcat(attributs, " \0");
                jeton = strtok(NULL, " >");
            }
            if(balise->type == DEBUTFIN) {
                attributs[strlen(attributs)-2] = '\0';
            }
            balise->attribut = chaineCreeCopie(attributs, strlen(attributs));
            free(attributs);
        } else {
            balise->attribut = NULL;
        }
     
    }
    free(valeurNom);
    return balise;
}

Chaine baliseLitNom(Balise balise) {

    assert(balise != NULL && "La balise est nulle.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != COMMENTAIRES &&
            "Mauvais type de balise.");

    Chaine retour = chaineCreeCopie(chaineValeur(balise->nom), chaineLongueur(balise->nom));
    
    if(retour == NULL) {
        return NULL;
    }
    return retour;
}

TypeBalise baliseLitType(Balise balise) {
    
    assert(balise != NULL && "La balise est nulle.");

    return balise->type;
}
 
//Ici, NULL peut vouloir dire deux choses: soit la balise n'a pas d'attributs, soit
//l'allocation mémoire pour la chaine a rencontré une erreur.
Chaine baliseLitAttributs(Balise balise) {

    Chaine attribut;

    assert(balise != NULL && "La balise est nulle.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != COMMENTAIRES &&
            "Mauvais type de balise.");


    if(balise->attribut == NULL) {
        return NULL;
    } else {
        attribut = chaineCreeCopie(chaineValeur(balise->attribut), chaineLongueur(balise->attribut));
    }
    return attribut;
}

void baliseSupprimme(Balise balise) {

    assert(balise != NULL && "La balise est nulle.");

    if(balise->nom) {
        chaineSupprimme(balise->nom);
    }
    if(balise->attribut) {
        chaineSupprimme(balise->attribut);
    }
    free(balise);
}

//Fonction pour obtenir le TypeBalise d'une balise.
TypeBalise obtenirType(char * nom) {

    TypeBalise type;

    if(nom[1] == '?') {
        type = DIRECTIVE;
    } else if (nom[1] == '!') {
        type = COMMENTAIRES;
    } else if (nom[1] == '/') {
        type = FIN;
    } else if(nom[strlen(nom)-2] == '/') {
        type = DEBUTFIN;
    } else {
        type = DEBUT;
    }

    return type;
}
