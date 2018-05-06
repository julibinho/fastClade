#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 24 10:59:51 2018

Permet de créer un fichier fasta au bon format.

@author: Nathan
"""
#this program is used for format fasta file, by remplacing it.
#it read the name of a list, and the family of a list. In the list, the name of sequence need to be at the first collumn, the family need to be at the specified collumn

#By default, it search xxxx.fasta in the fasta folder, if you define a 4th argument, it can search xxxx.4thArg.


#then, it look in the fasta folder, if there exist fasta that has the same name:
#	and for them, it write the sequence with the family addition behind



#Exemple of use for file right under : python formatFasta ./listInput ./fastaDirectory 2 => will read the list, and add at the end of each fasta in fastaDirectory the family in the commlumn 3 in the list

#(this is a fasta file in ./fastaDirectory)
#>ANaeg
#GGGGG


#(this is the list in ./listeInput)
#ANaeg a1


#The fasta file will become :
#>ANaeg
#GGGGG/a1




import os
import sys
from os.path import exists




PATH_TO_PDB_LIST = sys.argv[1]  #"../dataset/list/PDB_SCOP95_seq_scop1.75.list"
PATH_TO_DIRECTORY = sys.argv[2]  #"../dataset/fastas_2/train/"
COLLUMN_OF_FAMILY = int(sys.argv[3])-1 #For user, the first collumn, is the collumn number 1 (even, if for the programmer, it will be indice 0)

#Handeling of a possible 4eme arg, that is the extension of the file
try:
  EXTENSION=sys.argv[4]
except IndexError:
  EXTENSION=".fasta"
else:
  EXTENSION=sys.argv[4]



def remplacageFasta(pathToRead,family): #if we get there, we know that the file exist (tested previously)

    flux = open(pathToRead,'r')

    sequence = ""
    name=""    
    final=""
    for line in flux:
	if (line[0]=='>'):
	    name+=line        
	else :
	    sequence += line

    sequence = sequence[:-1]


    if (sequence.find('/')==-1): #sequence.find renvois soit -1 si trouve pas, soit l'indexe de où il a trouvé.
	sequence = sequence[:-1]
        sequence += "/"+family
        flux.close()
	  
        final=name+sequence
	print(final)

        flux = open(pathToRead,'w')
        flux.write(final)
        flux.close()
    
    else:
        flux.close()

#Get all the name and family in a map
# Indexes will be family and contents a list of sequence in the family
monDictionnaire = {}
if (not exists(PATH_TO_PDB_LIST)):
	print("Input List does not exist, exit")
	quit()

flux = open(PATH_TO_PDB_LIST,"r")

compteur_total=0
compteur_exist=0
compteur_notExist=0

for line in flux:
    line_tab = line.split("\t")       #Separator in the list file, you can change it if needed
    family = line_tab[COLLUMN_OF_FAMILY].replace("\n","")  # take the X collumn, and remove the "\n" if it's the last collumn 
    family =family.strip('\t')
    name = line_tab[0] # the name should be at the indice 0
    #print (name)
    #print (family)
    if family in monDictionnaire:
        monDictionnaire[family].append(name)
    else:
        monDictionnaire[family] = [name]
   
for family in monDictionnaire:
    for name in monDictionnaire[family]:
        path = PATH_TO_DIRECTORY + "/" + name + EXTENSION
        if os.path.exists(path):
            remplacageFasta(path,family)
	    compteur_exist+=1
	else :
	    print("the file at " + path + " does not exist")	
	    compteur_notExist+=1

print("number of file that are in the list, but doesn't in the folder : " + str(compteur_notExist ) + "/" + str(compteur_notExist+compteur_exist)) 
            

    
