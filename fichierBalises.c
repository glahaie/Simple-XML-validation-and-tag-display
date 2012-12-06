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
#include <ctype.h>


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

    assert(fichier != NULL && "fichier est NULL");

    fclose(fichier->fic);

    free(fichier);
}   

Info fichierBalisesLit(fichierBalises fichier) {

    Info info;
    Chaine prochaine;
    int c, finBoucle = 1;
    int verif;
    char *temp; //Pour éviter de perdre de la mémoire allouée

    do {

        info = (Info)malloc(sizeof(struct info));

        //on s'assure que le fichier est au bon endroit
        fseek(fichier->fic, fichier->position*sizeof(char), SEEK_SET);
        
        //On obtient le prochain caractère qui n'est pas une whitespace.
        do {
            c = fgetc(fichier->fic);
            fichier->position++;
        } while (isspace(c));
        if(c == EOF) {
            free(info);
            return NULL;
        }else if (c == '<') {
            verif = '>';
            info->type = BALISE;
        } else {
            verif = '<';
            info->type = TEXTE;
        }

        prochaine = chaineCreeVide();
        chaineAjoute(prochaine, (unsigned char)c);
        
        //On remplie la chaine -- à vérifier pour balise, si EOF arrive avant la
        //fin de la balise (>)
        do {    
            c = fgetc(fichier->fic);
            if((c == verif && info->type == BALISE) || c != verif) {
                chaineAjoute(prochaine,(unsigned char)c);
                fichier->position++;
            }
        } while(c != verif && c != EOF);

        //retourne TEXTE OU OBTIENT BALISE
        if(info->type == TEXTE) {

            temp = chaineValeur(prochaine);
            if(!temp) {
                //Malloc a échoué
            } else {
                info->contenu.texte = chaineCreeCopie(temp, chaineLongueur(prochaine));
                free(temp);
            }
            chaineSupprimme(prochaine);
            finBoucle = 1;
        } else {
            //analyse de la balise
            info->contenu.balise = baliseCree(prochaine);
            chaineSupprimme(prochaine);
            if(baliseLitType(info->contenu.balise) == COMMENTAIRES ||
                        baliseLitType(info->contenu.balise) == DIRECTIVE) {
                //On libère le struct info et balise et on recommence la boucle.
                baliseSupprimme(info->contenu.balise);
                free(info);
                finBoucle = 0;
            } else {
                //C'est un bon type de balise à retourner. On libère prochaine             
                //avant de retourner
                finBoucle = 1;
            }
        }
    } while(!finBoucle);
    return info;

}
           
        
