### First scripts

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
This script allow you to take a directory full of fastas file and two lists, one that give all the fastas that should be in the train directory and another that do the same thing for the test directory.

#### formatFasta.py
This script allow you to format fastas files so that they match the required format of the minHashCode part in c++

You just have to give it the file with the fasta you want to format and a file that give the correspondence between the name of the fasta and the family it should be associated with.

If it's use twice on the same file, the changes will just be done once. No worry if some of the fastas in the directory are already formatted.

#### DistributionPlot.py
This script will draw the distribution of the result file you gave him. (Typically the result file gave by the minHash part of the project)

It will draw the distribution of all the best intraFamily and ExtraFamily similarities in an histogram.

You need to give him in the following order : The path to the result file, the path to the out file of the script and the number of bins you want in your histogram (30 gives already a good idea).
