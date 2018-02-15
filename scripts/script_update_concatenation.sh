#!/bin/bash


#variable used for regular expression test 
int_test='^[0-9]+$'


#test on arguments
#test if the list_input is correct
if [ "$1" == "-l" ] && [ -n "$2" ] && [ -f "$2" ] ; then list_input_check=true ;  fi

#test if the fastas_input folder is correct
if [ "$3" == "-f" ] && [ -n "$4" ] && [ -d "$4" ] ; then fastas_input_check=true ; fi

#test if the output is correct 
if [ "$5" == "-o" ] && [ ! -e "$6" ] ; then output_check=true ; fi 

#test if column interger is correct
if [ "$7" == "-c" ] && [[ "$8" =~ $int_test ]] ; then integer_check=true ; fi



#if all the tests are true, we execute de command line
if [ "$list_input_check" == true ] && [ "$fastas_input_check" == true ] && [ "$output_check" == true ] && [ "$integer_check" == true ] ; 

	then
		#awk is used to retrieve the fastas name at the column specified in the fastas file specified
		#then for each fastas name retrieved, it copy the contenu of the fastas file (from the fastas folder) in a new file 
		
		start=$SECONDS
		 for i in $(cat "$2" | awk -v var="$8" '{print $var}');  do cat "$4"/"$i".fasta >> $6;  done;
		duration=$(( SECONDS - start ))		
		echo "concatenation done in $duration second(s)" ; 
else 
	#when "-help" isn't explicitly inputed, it show which argument was wrong
	if [ "$list_input_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "list name argument is incorrect, type \"-help\" for more informations" ;  fi
	if [ "$fastas_input_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "the fastas folder argument is invalid, type \"-help\" for more informations" ; fi
	if [ "$output_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "the output is incorect, a file may already that have that name, type \"-help\" for more informations" ; fi 
	if [ "$integer_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "the column argument is incorrect, type \"-help\" for more informations" ; fi
	
fi ;


#if "-help" has been explicitly inputted, it shows the help, as when nothing is inputted
if [ "$#" == "0" ] || [ "$1" == "-help" ] ; then echo "
This bash script is used for the concatenation of fastas file.

The first argument -l must be the file that contain all the name of the fastas files to concatenate line by line.
the second argument -f must be the folder that contain all the separated fastas file to concatenate.
The third argument -o  it specifies the output file, the output name should not already exist.
The fourth argument -c specifie at which column should the script read the fastas name in the fastas file (if the document isn't well formated, you can try to input 0 ). 

Typical usage of the script :
./script_name.sh -l list/test_dataset.list_superfamily -f fastas -o output_file.txt -c 1" ; fi





