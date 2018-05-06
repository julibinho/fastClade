# MinHash Algorithm C++
This file contains functions that allow you to use a minHash algorithm on formated fasta sequences. Can also take care of creating shingles and work with them.

### Compile
Simple type "make" in the folder where is the makefile.

The make will create 2 program :
	- createDatabase that is used to create a database with a folder that contains fastas file in the format : 
	- TestDatabase that is used to


### The format your fastas need to be : 
To work, your fasta files need to be formated like this :
>Protein name 
SEQUENCE/FamilyName 

Exemple :
>d1a3pa_
PGPSSYDGYCLNGGVMHIESLDSYTCNCVIGYSGDRCQTRDL/g.3 


If you want to format normal fastas files, you may read the ReadMe of the script section for the program "formatFasta.py" and "formatFastaWOList.py"

### BashDoAll.sh
This bash script do all the comparaison as long as the Train and Test folder are well formated.

File it need to work (in the same folder) :
	-createDabase	
	-testDatabase 
	-ResultRead.py (=> userfriendly output)
	-ResultReadOutputFormated.py (=> clean output)



Input :
	- The Train folder with all the separated and formated fasta file.
	- The Test folder with all the separated and formated file file.
	- An Integer that renseign the length of the shingles
	- An Integer that renseign the number of Hash

Exemple of use :
./BashDoAll.sh -S ./allTrainFormated -Q ./allTestFormated -T 5 -N 100

Output :
A folder with the name of each parameter will be created.
In this folder it will have few files :
	-comparaison => the raw MinHash result file, formated like this : <Test ProtName> <Train ProtName> <Best IntraFamily similarity> <Best ExtraFamily similarity>
	-TrueResults => A file where all the well guessed proteins are reported, formated like this : <Test ProtName> <Test ProtFamily> <Train ProtName> <Train ProtFamily>
	-FalseResults => A file where all the not well guessed proteins are reported. (same format than TrueResults)
	-SeedDataBase => The TrainDB that has been created by the program : createDatabase. 
	-LogComparaison => Exploitable ResultFile that is formated by lines in that way :
		- Line 1 : Time taken for the comparaison ( the TrainDB creation , the TestDB creation and the comparaison )
		- Line 2 : The rate of True Positive 
		- Line 3 : The rate of False Positive 
		- Line 4 : the rate of False Negative (usualy 0%)
		- Line 5 : The F-SCORE


### createDatabase
This command will create a database with all the fastas associated with their family and their minHash signatures for using it to compare another set of data.
It will also save the parameters you use in order to use them to compare your database with another set of data.
It's important to notice that it also save the RandomSeed. 


Input :
	- The folder that contains the separated fasta file
	- The length of the shingles
	- The Number of Hash fonctions to use
	- The name of the output file that will be created

Exemple of use : 
createDatabase path/folder 2 1000 HashDB


### testDatabase
Do the all vs all comparaison, and write the result in a file.

Input :
	- TrainDB : The database you just created with createDatabase
	- TestDB : The TestFolder that contain all the formated Test fasta
	- Output : the name of the file where write the results

Exemple of use :
testDatabase HashDB TestFolder/ results.txt

Ouput :
The output is a file formated like this : 
<Test ProtName> <Train ProtName> <Best IntraFamily similarity> <Best ExtraFamily similarity>

####### Exploit of the raw result file #######

### ResultRead.py
This file read the raw result of the MinHash program, and print an userfriendly Output

Input :
	- The folder that contain all the TrainFasta
	- The folder that contain all the TestFasta
	- The MinHash result file
	- An empty file where write ( It should already exist : it will write the true association here)
	- An empty file where write ( It should already exist : it will write the false association here)

Exemple of use :
	python ResultRead.py TrainFasta TestFasta results.txt TrueResults FalseResults

### ResultRead.py
This file is the same that the ResultRead.py at the exception that it's a bit not userfriendly, and format the output as :
	- Line 1 : Time taken for the comparaison ( the TrainDB creation , the TestDB creation and the comparaison )
	- Line 2 : The rate of True Positive 
	- Line 3 : The rate of False Positive 
	- Line 4 : the rate of False Negative (usualy 0%)
	- Line 5 : The F-SCORE


### Plot the distribution
To plot the distribution, read the ReadMe of the ScriptSection

