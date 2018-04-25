#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Feb 22 07:59:20 2018

@author: Nathan
"""

import os
import sys

DOSSIER_FASTAS = sys.argv[1]   # "../dataset/fastas_2/"
LISTE_TRAIN = sys.argv[2]     #"../dataset/list/train_dataset.list"
LISTE_TEST =  sys.argv[3]  #"../dataset/list/test_dataset.list_family"



# Get our train dict
fluxTrain = open(LISTE_TRAIN,"r")
maListeTrain = {}
for line in fluxTrain:
    monSplit = line.split("\t")
    if monSplit[2] in maListeTrain:
        maListeTrain[monSplit[2]].append(monSplit[0])
    else:
        maListeTrain[monSplit[2]] = [monSplit[0]]
fluxTrain.close()

#Get our test dict
fluxTest = open(LISTE_TEST,"r")
maListeTest = {}
for line in fluxTest:
    monSplit = line.split("\t")
    if monSplit[2] in maListeTest:
        maListeTest[monSplit[2]].append(monSplit[0])
    else:
        maListeTest[monSplit[2]] = [monSplit[0]]
fluxTest.close()



#Create directories if they don't exists
if not os.path.exists(DOSSIER_FASTAS + "train/" ):
    os.makedirs(DOSSIER_FASTAS + "train/")

if not os.path.exists(DOSSIER_FASTAS + "test/" ):
    os.makedirs(DOSSIER_FASTAS + "test/")


# Move all the train files
for famille in maListeTrain:
    for fasta in maListeTrain[famille]:
        filepath = DOSSIER_FASTAS  + fasta + ".fasta"
        if os.path.isfile(filepath):
            os.rename(filepath, DOSSIER_FASTAS  + "train/" + fasta + ".fasta")


#Move all the test files
for famille in maListeTest:
    for fasta in maListeTest[famille]:
        filepath = DOSSIER_FASTAS  + fasta + ".fasta"
        if os.path.isfile(filepath):
            os.rename(filepath, DOSSIER_FASTAS  + "test/" + fasta + ".fasta")
