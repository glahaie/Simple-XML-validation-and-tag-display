/* balise.c
 * Par Guillaume Lahaie et Sylvain Labranche
 *     LAHG04077707        LABS02059007
 *      
 * Date de remise: 18 décembre 2012.
 *
 * Dernières modifications: 8 décembre 2012.
 *
 * Implémentation du fichier en-tête balise.h. La fonction baliseCree
 * contient la majorité du code du module. D'après les données du TP,
 * on suppose que la chaine utilisée pour la fonction baliseCree contient
 * une balise valide, donc avec les délimiteurs tel qu'attendu (<>), et
 * le nom et les attributs valides. 
 *
 * Nous vérifions quand même que le nom de la balise ne contient pas de
 * caractères illégaux grâce à la fonction verifierChamp. On pourrait pousser
 * beaucoup plus loin la validation, par exemple en vérifiant que le nom ne
 * débute pas par xml, mais c'est hors de l'étendue du TP. De plus, comme le
 * comportement n'est pas spéficié pour les erreurs de balise, nous affichons
 * tout simplement un message d'erreur sur la sortie stderr.
 *
 * Le comportement des fonctions si l'allocation dynamique de mémoire
 * échoue n'est pas spécifié dans le TP. Si cela se produit, les fonctions
 * retournent immédiatement NULL. Cela amène une confusion pour la fonction
 * baliseLitAttributs, car la fonction pourrait retourner NULL car la balise
 * ne contient pas d'attributs, ou pour une erreur d'allocation de mémoire.
 *
 */


//Chaine des caractères interdits dans le nom d'une balise xml
//selon Wikipedia
#define VERIF_NOM "!\"#$%&'()*+,/;<=>?@[\\]^`{|}~, "
#define MESS_ERR_VERIF "Erreur lors de la vérification d'un champ\n."
#define MESS_ERR_CHAMP "Erreur: nom de balise mal formé.\n"

#include "balise.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct balise {
    Chaine nom;
    Chaine attribut;
    TypeBalise type;
};

//Retourne le type de balise de la chaine, selon l'énumération TypeBalise.
TypeBalise obtenirType(char *nom);

//Verifie qu'un champ d'une balise contient pas de caractères illégaux. 
//Retourne faux s'il y une erreur, vrai sinon. Si l'allocation de mémoire
//ne fonctionne pas, retourne -1.
int verifierChamp(Chaine chaine, char *carInterdits);


//La fonction obtient le champ balise->type, et les champs balise->nom et
//balise->attribut si nécéssaire, selon le type de balise. Elle utilise
//la fonction strtok pour obtenir le nom et les attributs. Si l'allocation
//de mémoire ne fonctionne pas à une certain moment de la fonction, elle
//retourne alors NULL.
Balise baliseCree(Chaine nom) {

    Balise balise = (Balise)malloc(sizeof(struct balise));
    char * jeton;                           //pour strtok
    char * attributs;                       //pour les attributs, si nécessaire
    char * valeurNom = chaineValeur(nom);
    int champ;

    if(balise == NULL || valeurNom == NULL) {
        baliseSupprimme(balise);
        return NULL;
    }

    balise->nom = balise->attribut = NULL;
    balise->type = obtenirType(valeurNom);
   
    if (balise->type == FIN) {
        //Trouver le nom à l'aide de jetons.
        jeton = strtok(&valeurNom[2], ">");
        balise->nom = chaineCreeCopie(jeton, strlen(jeton));
    } else {
        //C'est un début/ debutfin
        jeton = strtok(&valeurNom[1], " >");
        balise->nom = chaineCreeCopie(jeton, strlen(jeton));
         
        //Traitement des attributs, si nécessaire.
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
                //Pour enlever le '/' d'attributs
                attributs[strlen(attributs)-2] = '\0';
            }
            balise->attribut = chaineCreeCopie(attributs, strlen(attributs));
            free(attributs);
        }    
    }
    
    //Vérifie que le nom est bien formé, si nécessaire, et qu'il
    //n'est pas NULL
    if(balise->type != COMMENTAIRES && balise->type != DIRECTIVE) {

        assert(balise->nom != NULL && "Nom de balise NULL");
        champ = verifierChamp(balise->nom, VERIF_NOM);
        if(champ < 0) {
            fprintf(stderr, "%s",MESS_ERR_VERIF);
        } else if (champ == 0) {
            fprintf(stderr, "%s",MESS_ERR_CHAMP);
        }
    }

    free(valeurNom);
    return balise;
} //fin baliseCree


Chaine baliseLitNom(Balise balise) {

    assert(balise != NULL && "balise est NULL.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) 
            != COMMENTAIRES && "Mauvais type de balise.");
    char * temp = chaineValeur(balise->nom);
    Chaine nomBalise = chaineCreeCopie(temp, chaineLongueur(balise->nom));
    free(temp);
    if(nomBalise == NULL) {
        return NULL;
    }
    return nomBalise;
}

TypeBalise baliseLitType(Balise balise) {
    
    assert(balise != NULL && "balise est NULL.");

    return balise->type;
}
 
Chaine baliseLitAttributs(Balise balise) {

    assert(balise != NULL && "La balise est nulle.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != 
            COMMENTAIRES && "Mauvais type de balise.");

    Chaine attribut;
    char *temp;  //Pour libérer l'allocation

    if(balise->attribut == NULL) {
        return NULL;
    } else {
        temp = chaineValeur(balise->attribut);
        if(!temp) {
            return NULL;
        }
        attribut = chaineCreeCopie(temp,
                    chaineLongueur(balise->attribut));
        free(temp);
        
        if(!attribut) {
            return NULL;
        }
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

int verifierChamp(Chaine chaine, char *carInterdits) {

    char * valeurChamp = chaineValeur(chaine);
    int i, longueurChamp = chaineLongueur(chaine);
    if(!valeurChamp) {
        return -1;
    }

    for(i = 0; i < longueurChamp; i++) {
        if(strchr(carInterdits, valeurChamp[i]) != NULL) {
                free(valeurChamp);
                return 0;
        }
    }
    free(valeurChamp);
    return 1;

}
