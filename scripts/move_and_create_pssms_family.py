'''
    Script who detect and move your pssms files into appropriate family directories
'''

import os

PATH_TO_PDB_LIST = "../dataset/list/PDB_SCOP95_seq_scop1.75.list"
PATH_TO_PSSM_DIRECTORY = "../dataset/pssmModels/"

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
        
    
        
# We have to create the right directories and put the files in them
for family in monDictionnaire:
    path = PATH_TO_PSSM_DIRECTORY + family + "/"
    
    if not os.path.exists(path):
        os.makedirs(path)
    
    for name in monDictionnaire[family]:
            filepath = PATH_TO_PSSM_DIRECTORY + name + ".pssm"
            if os.path.isfile(filepath):
                os.rename(filepath, PATH_TO_PSSM_DIRECTORY + family + "/" + name + ".pssm")
            
 