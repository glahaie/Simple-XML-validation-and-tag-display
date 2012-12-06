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

fichierBalises fichierBalisesOuvre(char * nom_fichier) {

    fichierBalises fib = (fichierBalises)malloc(sizeof(struct fichierBalises));
    if(!fib) {
        return NULL;
    }
    
    fib->fic = fopen(nom_fichier, "r");

    if(!fib->fic) {
        free(fib);
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
    return;
}   

Info fichierBalisesLit(fichierBalises fichier) {

    Info info;
    Chaine prochaine;
    int prochainCar, 
        verif;          
    char *temp;         //Pour éviter de perdre de la mémoire allouée

    info = (Info)malloc(sizeof(struct info));

    //On s'assure que le fichier est à la dernière position lue
    fseek(fichier->fic, fichier->position*sizeof(char), SEEK_SET);
        
    //On obtient le prochain caractère qui n'est pas un blanc.
    do {
        prochainCar = fgetc(fichier->fic);
        fichier->position++;
    } while (isspace(prochainCar));

    if(prochainCar == EOF) {
        free(info);
        return NULL;
    }else if (prochainCar == '<') {
        verif = '>';
        info->type = BALISE;
    } else {
        verif = '<';
        info->type = TEXTE;
    }

    prochaine = chaineCreeVide();
    chaineAjoute(prochaine, (unsigned char)prochainCar);
        
    //On rempli la chaine, jusq'au caractere verif ou jusqu'à la fin du fichier
    do {    
        prochainCar = fgetc(fichier->fic);
        if((prochainCar == verif && info->type == BALISE) || prochainCar != verif) {
            chaineAjoute(prochaine,(unsigned char)prochainCar);
            fichier->position++;
        }
    } while(prochainCar != verif && prochainCar != EOF);

        
    if(info->type == TEXTE) {

        temp = chaineValeur(prochaine);
        if(!temp) {
            //Malloc a échoué
        } else {
            info->contenu.texte = chaineCreeCopie(temp, chaineLongueur(prochaine));
            free(temp);
        }
    } else {
        //analyse de la balise
        info->contenu.balise = baliseCree(prochaine);
    }

    chaineSupprimme(prochaine);
    if(info->type == BALISE && (baliseLitType(info->contenu.balise) == COMMENTAIRES ||                       baliseLitType(info->contenu.balise) == DIRECTIVE)) {

        baliseSupprimme(info->contenu.balise);
        free(info);
        info = fichierBalisesLit(fichier);                    
    }
    return info;

}
           
        
