/* tp1.c
 * Par Guillaume Lahaie et Sylvain Labranche
 *     LAHG04077707        LABS02059007
 *
 * Dernière modification: 9 décembre 2012
 *
 * Remise: 18 décembre 2012
 *
 *
 * Utilisation du programme: <nom_programme> <nom_fichier>.
 *
 * Pour le moment, voici les vérifications faites par le programme:
 * - Le fichier xml ne commence pas par une balise FIN.
 * - Le fichier ne contient pas de balises fermées dans le mauvais ordre.
 * - Toutes les balises DEBUT sont fermées à la fin du fichier.
 *
 * Pour le moment, le programme contient plusieurs fuites de mémoire, Surtout
 * du aux appels de fonction chaineValeur et baliseLitNom. Lorsqu'elles sont
 * appelées directement pour une comparaison ou pour afficher une valeur,
 * la mémoire est alors perdu. Pour chaque appel, il faudrait pointer vers
 * cet endroit pour pouvoir le désallouer ensuite.
 *
 */


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "pile.h"
#include "chaine.h"
#include "balise.h"
#include "fichierBalises.h"

#define ERR_ARG 1	//Erreur avec les arguments
#define ERR_XML 2   //Erreur dans le fichier XML/HTML
#define ERR_FICHIER 3
#define ERR_MESS_XML "Ficiher XML ou HTML mal formé.\n"
#define ERR_MESS_FICHIER "Erreur lors de l'ouverture du fichier.\n"
#define MAX_BALISES 10000

//Affiche un message d'erreur approprié sur stderr pour l'erreur noErreur
void afficherErreur(int noErreur, const char *nomFichier);

//Affiche le nom de la balise avec le niveau d'imbrication approprié
void afficherBalise(int imbrication, const Balise balise);

//Retourne une chaine char * du nom de la balise
char *obtenirNomBalise(Balise balise);

//Libère l'allocation de mémoire pour une structure Info.
void libereInfo(Info info);

int main(int argc, char *argv[]) {
    int erreur = 0;
    fichierBalises fichier;
    Info texte, depile;
    int imbrication = 0;
    TypeBalise typeBalise;
    char *compare1, *compare2;  //pour comparer les noms des balises

    if(argc != 2){
        afficherErreur(ERR_ARG,  argv[0]);
        return ERR_ARG;
    }

    fichier = fichierBalisesOuvre(argv[1]);

    if (fichier == NULL) {
		afficherErreur(ERR_FICHIER, argv[0]);
		return ERR_FICHIER;
	}

	while ((texte = fichierBalisesLit(fichier)) != NULL) {
	    if (texte->type == BALISE) {
	        typeBalise = baliseLitType(texte->contenu.balise);
	        if (typeBalise == DEBUTFIN || typeBalise == DEBUT) {
	            afficherBalise(imbrication, texte->contenu.balise);
                if(typeBalise == DEBUT) {
                    //on empile
                    pileEmpiler(texte);
                    imbrication++;
                }
	        } else if (typeBalise == FIN) {
                //Vérifier si la pile est vide avant de dépiler
                if(pileTaille() == 0) {
                    libereInfo(texte);
                    afficherErreur(ERR_XML, argv[1]);
                    fichierBalisesFerme(fichier);
                    return ERR_XML;
                }
                depile = (Info)pileDepiler();

                compare1 = obtenirNomBalise(depile->contenu.balise);
                compare2 = obtenirNomBalise(texte->contenu.balise);
                libereInfo(texte);
                libereInfo(depile);
                if(strcmp(compare1, compare2) != 0) {
                    //Erreur - les noms des deux balises sont
                    //différents
                    free(compare1);
                    free(compare2);
                    afficherErreur(erreur = ERR_XML, argv[1]);
                    fichierBalisesFerme(fichier);
                    return erreur;
                } else {
                    //pas d'erreur
                    imbrication--;
                    free(compare1);
                    free(compare2);
                }
	        }
	    } else {
            //Info de type texte, on ignore
            libereInfo(texte);
	    }
    }

    fichierBalisesFerme(fichier);
    if(pileTaille() > 0) {
        afficherErreur(erreur = ERR_XML, argv[1]);
    }
    return erreur;
}

void afficherErreur(int noErreur, const char *nomFichier) {

    printf("\n");
	fprintf(stderr, "%s: Erreur %d\n", nomFichier, noErreur);
	switch(noErreur) {
	    case ERR_ARG: fprintf(stderr, "Mauvais arguments.\n");
                      fprintf(stderr, "Usage: %s <fichier>\n",
                              nomFichier);
                      break;
        case ERR_FICHIER: fprintf(stderr, "%s", ERR_MESS_FICHIER);
                          break;
        case ERR_XML: fprintf(stderr, "%s", ERR_MESS_XML);
                      break;
	    default:	  fprintf(stderr, "Erreur inconnue.\n");
			          break;
	}
    return;
}

void afficherBalise (int imbrication, const Balise balise) {
    char * temp1; //Pour eviter des fuites de mémoire
    Chaine temp2;
    int i;
    for (i = 0;  i < imbrication; i++)  {
        printf("  ");
    }

    temp2 = baliseLitNom(balise);
    temp1 = chaineValeur(temp2);
    printf("%s\n", temp1);
    chaineSupprimme(temp2);
    free(temp1);

    return;
}

char * obtenirNomBalise(Balise balise) {
    char * retour;
    Chaine nom = baliseLitNom(balise);
    if(!nom) {
        return NULL;
    }
    retour = chaineValeur(nom);
    chaineSupprimme(nom);
    return retour;
}

void libereInfo(Info info) {
    if(info->type == TEXTE) {
        chaineSupprimme(info->contenu.texte);
        free(info);
    } else {
        baliseSupprimme(info->contenu.balise);
        free(info);
    }
    return;
}
