#!/bin/bash

#variable used for regular expression test 
int_test='^[0-9]+$'


#test on arguments
#test if the input file is correct
if [ "$1" == "-i" ] && [ -n "$2" ] && [ -f "$2" ] ; then concat_fastas_check=true ;  fi

#test if the input pssm folder is correct
if [ "$3" == "-p" ] && [ -n "$4" ] && [ -d "$4" ] ; then pssm_input_check=true ; fi

#test if the output is correct 
if [ "$5" == "-o" ] && [ ! -e "$6" ] ; then output_check=true ; fi 

#test if input interger is correct
if [ "$7" == "-f" ] && [[ "$8" =~ $int_test ]] ; then integer_check=true ; fi



#if all the tests are true, we execute de command line
if [ "$concat_fastas_check" == true ] && [ "$pssm_input_check" == true ] && [ "$output_check" == true ] && [ "$integer_check" == true ] ; 
	then 
	start=$SECONDS
	makeblastdb -in "$2" -out "$6".db -dbtype prot
	duration=$(( SECONDS - start ))	
	echo "formating done in $duration second(s)" ;

	start=$SECONDS
	for i in "$4"/* ; do psiblast -db "$6".db -in_pssm $i -outfmt $8 ; done >> $6
	duration=$(( SECONDS - start ))	
	echo "comparaison all to all done in $duration second(s)" ;

else 
	#when "-help" isn't explicitly inputed, it show which argument was wrong
	if [ "$concat_fastas_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "concatenated input fastas file is incorrect, type \"-help\" for more informations" ;  fi
	if [ "$pssm_input_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "pssm folder argument is invalid, type \"-help\" for more informations" ; fi
	if [ "$output_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "the output is incorect, a file may already that have that name, type \"-help\" for more informations" ; fi 
	if [ "$integer_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "the format argument is incorrect, type \"-help\" for more informations" ; fi
	
fi ;



#if "-help" has been explicitly inputted, it shows the help, as when nothing is inputted
if [ "$#" == "0" ] || [ "$1" == "-help" ] ; then echo "
This bash script is used for the comparaison all to all of a concatenation of fastas file with a pssm folder.
It first format the concatenation of the fastas file to allow the comparaison, then it compares.

The first argument -i must be the concatenation of fastas.
the second argument -p must be the folder that contain all the pssm models. 
The third argument -o specifies the output file, the output name should not already exist.
The fourth argument -f specifie the output format. 

Typical usage of the script :
./comparaison.sh -i concat_fasta -p pssmModels -o all_to_all_comparaison -f 6" ; fi




