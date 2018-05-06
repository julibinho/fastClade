### How to use the PSI-BLAST on a DataBase

How to uses thoses multiples scripts to compare query sequence with pssm data base :

1) What you need :
(a) - A list of the name of each pssm sequence you want in your reference data babse
(b) - A list of the name of each query sequence you want to compare to your reference data base
(c) - A folder of pssm, their name need to fit the name of the list of pssm data base

2) How to compare :
(d) - build your pssm DataBase by using ConcatPSSM.sh , with the list of pssm sequence(a) and the folder that contains all the pssm(c).
(e) - build your fasta Compiled file by using ConcatFasta.sh , with the list of each query sequence(b).
(f) - Use the BLASTCompare.sh with the pssm database build(d) with concat_pssm and with the fasta file(e) that you just compiled


3) how to exploit this results :
    - Use ReadResult.sh with the result file (f) to have a file with best associations of [PSSM] [SCORE] [FASTA].
    - Use ExploitResults.sh with the result file (f) , with the list of query (b),  and finaly, with the list of pssm sequence (c) to have the statistics of the query


### Useful scripts to use with minHashing

#### create_test_train_directories.py
This script allow you to separate the Train and Test fasta present in the fasta directory with the help of two lists.

Input : 
	- The Fasta Folder that contain all the fasta file (separated)
	- The Train List ( under the format : <ProtName> <ProtFamily> )
	- The Test List ( under the format : <ProtName> <ProtFamily> )
	
Exemple of use :
	python create_test_train_directories.py FastaFolder SeedList TestList 

Output :
The output will be 2 new folder in the FastaFolder named "test" and "train"

Note : The fastas will only be copied from the FastaFolder to the folder test and train. So, think about copy your fasta folder before



#### formatFasta.py
This script allow you to format fastas file to use the MinHash program.

The format that MinHash requires is :
>Name
SEQUENCE/Family

Exemple :
>d1a3pa_
PGPSSYDGYCLNGGVMHIESLDSYTCNCVIGYSGDRCQTRDL/g.3

Input :
	- A List ( under the format : <ProtName> <...> <ProtFamily> ). The program will for each Protein in the list, add, if it exist in the folder, the family specified in the list.
	- A FastaFolder that contain all the separated fasta file to format.
	- A number that indicate the collumn where is the ProtFamily (start at one)

Option :
	- As a 4th argument : if your file havn't the .fasta extension, you can renseign it's extension Here (by default it's .fasta)


Exemple of use :
	python formatFasta.py ListFamily FastaFolder 3

Exemple of Use with 4th option :
	python formatFasta.py ListFamily FastaFolder 3 .fsa

Output :
All the fasta specified in the list will be modified and added with their correct family name directly from the folder.

Note : If a fasta already contain a "/" in their sequence ( If it's in the name, it's OK ) won't be modified again.

#### formatFastaWOList.py
The aim of this script is the same as formatFasta : format fasta file to use the MinHash program.

Input :
	- FastaDirectory that contain the all the separated fasta file to format.
	- A family (under the form of a char String)

Exemple of use:
	python formatFastaWOList.py FastaFolder PF06777

Output :
All the fasta in the folder will be added by "/String" at their end.



#### DistributionPlot.py
It will draw the distribution of all the best intraFamily and ExtraFamily similarities in an histogram with a MinHash result file
Input :
	- ResultFile : the MinHash result file
	- Output : the Name of the histogram file
	- Number of Bins : Integer that is used to scale the histogram (usualy 30/35 is good)

Exemple of use :
	python DistributionPlot.py MinHashResult Graph 30

Output : 
in this case, an histogram named Graph.
	
