/* extraitTexte.c
   Extrait le texte d'un fichier de balises (en supprimant les balises)
*/

#include "fichierBalises.h"
#include "chaine.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	fichierBalises fic;
	Info texte;
    char * temp;
	if (argc != 2){
		fprintf(stderr,"Usage : %s fichier\n", argv[0]);
		return 1;
	}
	fic = fichierBalisesOuvre(argv[1]);
	if (fic == NULL) {
		fprintf(stderr,"Probleme a l'ouverture du fichier\n");
		return 2;
	}
	while ((texte = fichierBalisesLit(fic)) != NULL) {
		if (texte->type == TEXTE) {
            temp = chaineValeur(texte->contenu.texte);
			printf("%s ", temp);
            free(temp);
            chaineSupprimme(texte->contenu.texte);
        } else {
            baliseSupprimme(texte->contenu.balise);
        }
            

		free(texte);
	}
    printf("\n");
	fichierBalisesFerme(fic);
	return 0;
}
