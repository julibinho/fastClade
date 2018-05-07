#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 24 10:59:51 2018

Permet de créer un fichier fasta au bon format.

@author: Nathan
"""

import os
import sys

PATH_TO_PDB_LIST = sys.argv[1]  #"../dataset/list/PDB_SCOP95_seq_scop1.75.list"
PATH_TO_DIRECTORY = sys.argv[2]  #"../dataset/fastas_2/train/"

def remplacageFasta(pathToRead,family):
    
    flux = open(pathToRead,'r')
    sequence = ""
    
    for line in flux:
        sequence += line
    sequence = sequence[:-1]
    
    #if not sequence.find('/'):
    #print('youpi !')
    sequence += "/"+family
    flux.close()
    
    flux = open(pathToRead,'w')
    flux.write(sequence)
    flux.close()
    
    #else:
 #       flux.close()

#Get all the name and family in a map
# Indexes will be family and contents a list of sequence in the family
monDictionnaire = {}
flux = open(PATH_TO_PDB_LIST,"r")

for line in flux:
    line_tab = line.split("\t")
    family = line_tab[1][:-1]
    name = line_tab[0]
    if family in monDictionnaire:
        monDictionnaire[family].append(name)
    else:
        monDictionnaire[family] = [name]
   
for family in monDictionnaire:
    for name in monDictionnaire[family]:
        path = PATH_TO_DIRECTORY + "/" + name + ".fasta"
        if os.path.exists(path):
            remplacageFasta(path,family)
            

    
