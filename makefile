#Makefile pour TP2
#Par Guillaume Lahaie et Sylvain Labranche
#    LAHG04077707		 LABS02059007		
#
#    Dernières modifications: 18 décembre 2012
#    Remise: 18 décembre 2012

#Variables prédéfinies
CC = gcc
CFLAGS =  -W -Wall

#Dépendances
extraitStructure: extraitStructure.o chaine.o fichierBalises.o balise.o pile.o

extraitStructure.o: extraitStructure.c 

fichierBalises.o: fichierBalises.c 

balise.o: balise.c 

chaine.o: chaine.c 

pile.o: pile.c

#Je suppose ici que le répertoire contienr seulement  les fichiers .o 
#de ce programme. Si le répertoire en contient d'autres, ils seront effacés.
clean:
	rm *.o extraitStructure 

