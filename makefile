#Makefile pour TP2
#Par Guillaume Lahaie et Sylvain Labranche
#    LAHG04077707		 LABS02059007		

#Variables prédéfinies
CC = gcc
CFLAGS = -g -W -Wall

#Dépendances
extraitStructure: extraitStructure.o chaine.o fichierBalises.o balise.o pile.o

extraitStructure.o: extraitTexte.c chaine.h fichierBalises.h pile.h balise.h

fichierBalises.o: fichierBalises.c fichierBalises.h balise.h chaine.h

balise.o: balise.c balise.h chaine.h

chaine.o: chaine.c chaine.h

pile.o: pile.h pile.c

#Je nomme les fichiers .o, pour éviter d'en effacer qui ne seraient pas à effacer.
clean:
	rm chaine.o balise.o fichierBalises.o pile.o extraitStructure.o
