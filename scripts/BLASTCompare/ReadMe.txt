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


