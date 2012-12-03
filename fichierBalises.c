/* fichierBalises.c
 * Par Guillaume Lahaie et Sylvain Labranche
 * LAHG04077707
 *
 * Date de remise: 18 décembre 2012
 *
 * Implémentation de fichierBalises.h
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "fichierBalises.h"
#include "chaine.h"
#include "balise.h"


struct fichierBalises {
    FILE *fic;
    int position;
};

//Vérifier que nom_fichier n'est pas null?
fichierBalises fichierBalisesOuvre(char * nom_fichier) {

    fichierBalises fib = (fichierBalises)malloc(sizeof(struct fichierBalises));
    
    fib->fic = fopen(nom_fichier, "r");

    if(fib == NULL || fib->fic == NULL) {
        return NULL;
    } else {
        fib->position = 0;
        return fib;
    }
}

void fichierBalisesFerme(fichierBalises fichier) {

    assert(fichier != NULL);

    fclose(fichier->fic);

    free(fichier);
}

Info fichierBalisesLit(fichierBalises fichier) {

    Info info;
    Chaine prochaine;
    int c;
    char verif;

    do {

        info = (Info)malloc(sizeof(struct info));

        //on s'assure que le fichier est au bon endroit
        fseek(fichier->fic, fichier->position*sizeof(char), SEEK_SET);

        c = fgetc(fichier->fic);
        if(c == EOF) {
            return NULL;
        }else if (c == '<') {
            verif = '>';
            info->type = BALISE;
        } else {
            verif = '<';
            info->type = TEXTE;
        }

        prochaine = chaineCreeVide();
        
        //On remplie la chaine -- à vérifier pour balise, si EOF arrive avant la
        //fin de la balise (>)
        do {    
            chaineAjoute(prochaine,c);
            fichier->position++;
            c = fgetc(fichier->fic);
        } while(c != verif && c != EOF);
        
        //retourne TEXTE OU OBTIENT BALISE
        if(info->type == TEXTE) {
            info->contenu.texte = chaineCreeCopie(chaineValeur(prochaine), chaineLongueur(prochaine));
            free(prochaine);

            return info;
        } else {
            //analyse de la balise
            info->contenu.balise = baliseCree(prochaine);
            info->contenu.balise = baliseCree(prochaine);
            chaineSupprimme(prochaine);
            if(baliseLitType(info->contenu.balise) == COMMENTAIRES ||
                        baliseLitType(info->contenu.balise) == DIRECTIVE) {
                //On libère le struct info et balise et on recommence la boucle.
                baliseSupprimme(info->contenu.balise);
                free(info);
                continue;
            } else {
                //C'est un bon type de balise à retourner. On libère prochaine
                //avant de retourner
                return info;
            }
        }
    } while(1);

}
           
        
