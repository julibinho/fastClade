# MinHash Algorithm C++
This file contains functions that allow you to use a minHash algorithm on formated fasta sequences. Can also take care of creating shingles and work with them.

### Compile
A simple *make* should work but if not, you can change the *Makefile*.

### The format
You need to have directory with fasta proteins formatted like this :\n
\>*Protein name* \n
*protein sequence* / *family name*\n

The */* is used as a separator and is very important for that reason.

If you want to format normal fastas files, you can find a python script in the script section named *formatFasta* which should help you.

### createDatabase
The first command you need to make is this one. It will take a directory with formatted fastas, the length of the shingles you want, the number of hashs functions to use and an out file to write your results.

This command will create a database with all the fastas associated with their family and their minHash signatures.
It will also save the parameters you use in order to use them to compare your database with another set of data.

### testDatabase
This command takes 3 arguments : The path to the database previously created that you want to use, the path to another directory of formatted fastas, and the path to an out file where you will write your results.

The result file is easy to read and work like this :

*name of the protein* | *name of the best correspondant protein in database* | *best intraFamily similarity* | *best ExtraFamily similarity*


### Plot the distribution
You can easily plot the distribution that you obtain by looking once again at the script section for *DistributionPlot.py*. More info on that in the script section.
