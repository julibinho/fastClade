#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 24 10:59:51 2018

Permet de créer un fichier fasta au bon format.

@author: Nathan
"""




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


            

    
