#Makefile pour TP2
#Par Guillaume Lahaie et Sylvain Labranche
#    LAHG04077707		 LABS02059007		

#Variables prédéfinies
CC = gcc
CFLAGS =  -W -Wall

#Dépendances
extraitStructure: extraitStructure.o chaine.o fichierBalises.o balise.o pile.o

extraitStructure.o: extraitTexte.c chaine.h fichierBalises.h pile.h balise.h

fichierBalises.o: fichierBalises.c fichierBalises.h balise.h chaine.h

balise.o: balise.c balise.h chaine.h

chaine.o: chaine.c chaine.h

pile.o: pile.h pile.c

#Je suppose ici que le répertoire contienr seulement  les fichiers .o 
#de ce programme. Si le répertoire en contient d'autres, ils seront
#effacés.
clean:
	rm *.o extraitStructure
