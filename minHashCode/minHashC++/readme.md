# MinHash Algorithm C++
This file contains function that allow you to use a minHash algorithm on fasta sequences. Can also take care of creating shingles and work with them.

### shingling_documents
A function that takes a directory path and return the set of shingles of all the sequence in the directory

### shingling_long_document
Take a file who contains a concatenation of all the sequence you want to analyze and resturn a set of shingles for each one of them in a map

### shingling_a_document
Take your sequence and return the set of shingles represented by 32-bits integers

### liste_fichiers_dossier
Return a list of files in the directory you gave.
**Don't work recursivly !**
