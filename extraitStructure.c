/* tp1.c
 * Par Guillaume Lahaie et Sylvain Labranche
 * LAHG04077707 LABS02059007
 *
 * Dernière modification: 9 décembre 2012
 *
 * Remise: 18 décembre 2012
 *
 *
 * Utilisation du programme: <nom_programme> <nom_fichier>
 *
 */


#include <stdio.h>
#include <ctype.h>
#include "pile.h"
#include "chaine.h"
#include "balise.h"
#include "fichierBalises.h"

#define ERR_ARG 1	//Erreur avec les arguments
#define MAX_BALISES 10000

void afficherErreur(int noErreur, const char *nomFichier);
void afficherBalise(int imbrication, const Balise *balise);

int main(int argc, char *argv[]) {
    int erreur = 0;
    fichierBalises fichier;
    Info texte;
    int imbrication = 0;


    if(argc != 2){
        afficherErreur(ERR_ARG,  argv[0]);
        erreur = ERR_ARG;
    }

    fichier = fichierBalisesOuvre(argv[1]);

    if (fichier == NULL) {
		fprintf(stderr,"Probleme a l'ouverture du fichier\n");
		return 2;
	}

	while ((texte = fichierBalisesLit(fichier)) != NULL) {
	    if (texte->type == BALISE) {
	        Balise balise = texte->contenu.balise;
	        if (baliseLitType(balise) == COMMENTAIRES || baliseLitType(balise) == DIRECTIVE || baliseLitType(balise) == DEBUTFIN) {
	            afficherBalise(imbrication, &balise);
	        } else if (baliseLitType(balise) == DEBUT) {
	            afficherBalise(imbrication, &balise);
	            //on emplie la balise
	            imbrication++;
	        } else {
	            //depiler la balise, en même temps on vérifie si c'est la bonne balise fermante.Si ce n'est pas la bonne fermante, on termine
	            imbrication--;
	        }
            baliseSupprimme(texte->contenu.balise);
	    } else {
	        chaineSupprimme(texte->contenu.texte);
	    }

    }

    fichierBalisesFerme(fichier);
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

void afficherBalise (int imbrication, const Balise *balise) {
    int i;
    for (i = 0;  i <= imbrication; i++)  {
        printf("  ");
    }

    printf("%s\n ", chaineValeur(baliseLitNom(*balise)));

    return;
}
