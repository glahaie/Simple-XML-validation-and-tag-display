#Makefile pour TP2
#Par Guillaume Lahaie et Sylvain Labranche
#LAHG04077707

#Variables prédéfinies
CC = gcc
CFLAGS = -g -W -Wall

#Dépendances
extraitTexte: extraitTexte.o chaine.o fichierBalises.o balise.o

extraitTexte.o: extraitTexte.c chaine.h fichierBalises.h

fichierBalises.o: fichierBalises.c fichierBalises.h balise.h chaine.h

balise.o: balise.c balise.h chaine.h

chaine.o: chaine.c chaine.h


clean:
	rm *.o
