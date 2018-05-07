#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#this program is used for format fasta file, by remplacing it.
#it read the name of a list, and the family of a list. In the list, the name of sequence need to be at the first collumn, the family need to be at the specified collumn

#By default, it search xxxx.fasta in the fasta folder, if you define a 4th argument, it can search xxxx.4thArg.


#then, it look in the fasta folder, if there exist fasta that has the same name:
#	and for them, it write the sequence with the family addition behind



#Exemple of use for file right under : ./formatFasta ./listInput ./fastaDirectory 2 => will read the list, and add at the end of each fasta in fastaDirectory the family in the commlumn 3 in the list

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




PATH_TO_DIRECTORY = sys.argv[1]  #"../dataset/fastas_2/train/"
FAMILY_TO_ADD=sys.argv[2]

for filename in os.listdir(PATH_TO_DIRECTORY):
    flux = open(PATH_TO_DIRECTORY+"/"+filename,'r')


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
        sequence += "/"+FAMILY_TO_ADD
        
        flux.close()
	  
        final=name+sequence
	print(final)
        #print(filename)
        flux = open(PATH_TO_DIRECTORY+"/"+filename,'w')
        flux.write(final)
        flux.close()
    
    else:
        flux.close()


            

    
