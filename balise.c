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


struct balise {
    Chaine nom;
    Chaine attribut;
    TypeBalise type;
};


//Vérifie si le malloc fonctionne bien. Retoune NULL dans ce cas.
//Pour le moment je suppose que la Chaine nom passée contient encore
//les < et >. Le nom doit contenir au moins un caractère.
Balise baliseCree(Chaine nom) {

    assert(nom != NULL && chaineLongueur(nom) > 2);
    int i =1;
    unsigned char c;
    Balise b = (Balise)malloc(sizeof(struct balise));
    char * ch = (char *)malloc(chaineLongueur(nom)*sizeof(char));

    if(b == NULL || ch == NULL) {
        return NULL;
    }

    ch = chaineValeur(nom);

    //Obtenir le type, si possible
    switch(ch[1]) {
        case '!': b->type = COMMENTAIRES;
                  break;
        case '?': b->type = DIRECTIVE;
                  break;
        case '/': b->type = FIN;
                  break;
    }

    if(b->type == COMMENTAIRES || b->type== DIRECTIVE) {
        b->nom= b->attribut = NULL;
        return b;
    } else if (b->type == FIN) {
        //Trouver le nom, vérifier qu'il y a seulement cela
        b->nom = chaineCreeVide();
        i = 2;
        while(1) {
            c = ch[i];
            if(isalpha(c) || isdigit(c)) {
                chaineAjoute(b->nom, c);
            } else if (c == '>') {
                //
                break;
            } else {
                //Erreur
            }
            i++;
        }
    } else {
        //C'est un début/ debutfin
        //On trouve le nom - La vérification que le nom est conforme
        //est à améliorer
        b->nom = chaineCreeVide();
        i = 1;
        while(1) {
            c = ch[i++];
            if(isalpha(c) || isdigit(c)) {
                chaineAjoute(b->nom, c);
            } else if (isspace(c)) {
                //nom fini
                break;
            } else {
                //Erreur
                fprintf(stderr, "Erreur - Mauvais nom de balise.\n");
                exit(1);
            }
        }
        
        //On ajoute dans attribut maintenant et / ou type de balise
        while(1) {
            c = ch[i++];
            if(c != '>' && c !='/') {
                if(b->attribut == NULL) {
                    b->attribut = chaineCreeVide();
                }
                chaineAjoute(b->attribut, c);
            } else if (c == '>') {
                b->type = DEBUT;
                break;
            } else if( c == '/') {
                b->type = DEBUTFIN;
                break;
            }
        }
    }

    return b;
}


Chaine baliseLitNom(Balise balise) {

    assert(balise != NULL);
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != COMMENTAIRES);

    Chaine retour = chaineCreeCopie(chaineValeur(balise->nom), chaineLongueur(balise->nom));
    
    if(retour == NULL) {
        //Erreur
    }

    return retour;
}

TypeBalise baliseLitType(Balise balise) {
    
    assert(balise != NULL);

    return balise->type;
}
 
Chaine baliseLitAttributs(Balise balise) {

    assert(balise != NULL);
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != COMMENTAIRES);
    Chaine att;

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

