#!/bin/bash

#variables

#definition de la liste à utiliser
#liste=test_dataset.list_family
#liste=test_dataset.list_superfamily
liste=train_dataset.list

#definition des path à utiliser
path_liste=`find . -name "$liste"`
path_fastas=`find . -name "fastas"`
path_pssm=`find . -name "pssmModels"`



#avertissement
echo "Script bash pour la compilation et comparaison d'une liste pré-définie dans le script
Le script prendra un certain temps, et les erreures en derniere partie sont \"normales\"

conditions :
se situer dans le dossier qui contient (peu importe où) : 
	-la liste prédéfinie dans le script (ici : $liste)
	-le dossier fastas contenant les files fastas
	-le dossier pssmModels contenant les files pssm

Ne pas avoir de fichiers déjà només :
	- script_concat.fastas
	- script_concat.db
	- script_conpa.txt
"

echo "si tout cela est bon, appuyez sur o"
read input
if [ $input != o ]; then exit ;else echo "demarrage"; fi 


#commande de concaténation
for i in $(cat $path_liste | awk '{print $1}');  do cat $path_fastas/$i.fasta >> script_concat.fastas;  done;
echo "concatenation done"

#commande de formatage
makeblastdb -in script_concat.fastas -out script_concat.db -dbtype prot
echo "formatage done"

#commande de comparaison 
for i in $path_pssm/* ; do psiblast -db script_concat.db -in_pssm $i -outfmt 6 ; done >> script_conpa.txt
echo "end, fichier comparé : script_compa.txt"






