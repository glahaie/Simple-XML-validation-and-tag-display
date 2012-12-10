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
 * Pour le moment, le programme contient plusieurs fuites de mémoire. Surtout
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
#define MAX_BALISES 10000

void afficherErreur(int noErreur, const char *nomFichier);
void afficherBalise(int imbrication, const Balise balise);

int main(int argc, char *argv[]) {
    int erreur = 0;
    fichierBalises fichier;
    Info texte, depile;
    int imbrication = 0;
    TypeBalise typeBalise;
    char *compare1, *compare2;  //pour comparer deux balises
    Chaine chaine1, chaine2;

    if(argc != 2){
        afficherErreur(ERR_ARG,  argv[0]);
        return ERR_ARG;
    }

    fichier = fichierBalisesOuvre(argv[1]);

    if (fichier == NULL) {
		fprintf(stderr,"Probleme a l'ouverture du fichier\n");
		return 2;
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
                    baliseSupprimme(texte->contenu.balise);
                    free(texte);
                    afficherErreur(erreur = ERR_XML, argv[1]);
                    fichierBalisesFerme(fichier);
                    
                    return erreur;
                }
                depile = (Info)pileDepiler();
                chaine1 = baliseLitNom(texte->contenu.balise);
                chaine2 = baliseLitNom(depile->contenu.balise);

                compare1 = chaineValeur(chaine1);
                compare2 = chaineValeur(chaine2);
                if(strcmp(compare1, compare2) != 0) {
                    free(compare1);
                    free(compare2);
                    chaineSupprimme(chaine1);
                    chaineSupprimme(chaine2);
                    baliseSupprimme(depile->contenu.balise);
                    baliseSupprimme(texte->contenu.balise);
                    free(depile);
                    free(texte);
                    afficherErreur(erreur = ERR_XML, argv[1]);
                    fichierBalisesFerme(fichier);
                    return erreur;
                } else {
                    //pas d'erreur
                    imbrication--;
                    chaineSupprimme(chaine1);
                    chaineSupprimme(chaine2);
                    free(compare1);
                    free(compare2);
                    baliseSupprimme(depile->contenu.balise);
                    baliseSupprimme(texte->contenu.balise);
                    free(depile);
                    free(texte);
                }
	        } else {
                baliseSupprimme(texte->contenu.balise);
                free(texte);
            }
	    } else {
	        chaineSupprimme(texte->contenu.texte);
            free(texte);
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
                      fprintf(stderr, "Usage: %s <fichier>",
                              nomFichier);
                      break;
	    default:	  fprintf(stderr, "Erreur inconnue.\n");
			          break;
	}
    return;
}

void afficherBalise (int imbrication, const Balise balise) {
    char * temp1; //Pour eviter de la perte de mémoire
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
