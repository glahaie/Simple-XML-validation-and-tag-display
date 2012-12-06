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

    int position;
    Balise balise = (Balise)malloc(sizeof(struct balise));
    char * jeton;                           //pour strtok
    char * attributs;                       //pour les attributs, si nécessaire
    char * valeurNom = chaineValeur(nom);

    //A valider - quoi faire si malloc ne fonctionne pas
    if(balise == NULL || valeurNom == NULL) {
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

    } else {
        //C'est un début/ debutfin
    
        jeton = strtok(&valeurNom[1], " >");
        balise->nom = chaineCreeCopie(jeton, strlen(jeton));
         
        //Je prends la longueur de nom car les attributs ne peuvent pas être
        //plus grand que cela.
        position = 0;
        attributs = (char *)malloc(chaineLongueur(nom)*sizeof(char));
        while((jeton = strtok(NULL, " >")) != NULL) {
            strcat(attributs, jeton);
            position += strlen(jeton);
            printf("attributs:%s : %d\n",attributs, strlen(attributs));
            attributs[position++] = ' ';
            attributs[position] = '\0';
        }
        if(balise->type == DEBUTFIN) {
            attributs[position-2] = '\0';
        }
        balise->attribut = chaineCreeCopie(attributs, strlen(attributs));
        free(attributs);
     
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
        //Erreur
    }
    return retour;
}

TypeBalise baliseLitType(Balise balise) {
    
    assert(balise != NULL && "La balise est nulle.");

    return balise->type;
}
 
Chaine baliseLitAttributs(Balise balise) {

    assert(balise != NULL && "La balise est nulle.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != COMMENTAIRES &&
            "Mauvais type de balise.");


    if(balise->attribut == NULL) {
        return NULL;
    } else {
        att = chaineCreeCopie(chaineValeur(balise->attribut), chaineLongueur(balise->attribut));
        if(att == NULL) {
            //erreur
        }
    }
    return att;
}

//Pour le moment je ne vérifie pas le type de la balise, car je crois 
//qu'on peut devoir supprimer peu importe le type.
void baliseSupprimme(Balise balise) {

    assert(balise != NULL);

    free(balise->nom);
    free(balise->attribut);
    free(balise);
}


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

    return DEBUT;
}
