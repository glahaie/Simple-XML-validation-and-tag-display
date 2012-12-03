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


struct fichierBalises {
    FILE *fic;
    int position;
};

//Vérifier que nom_fichier n'est pas null?
fichierBalises fichierBalisesOuvre(char * nom_fichier) {

    fichierBalises fib = (fichierBalises)malloc(struct fichierBalises));
    
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
    Balise balise;
    int c;
    char verif;

    do {
        free(info);
        chaineSupprime(prochaine);
        baliseSupprime(balise);

        info = (Info)malloc(struct info);
        prochaine = chaineCreeVide();

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
        
        chaineAjoute(prochaine, c);
        //On remplie la chaine -- à vérifier pour balise, si EOF arrive avant la
        //fin de la balise (>)
        while(c != verif && c != EOF) {
            
            
        
