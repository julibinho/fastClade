#this script allow to regroupe pssm file indexed in a file, from a folder to a new other. With a log file

###There is an indeterminated bug that lead to a folder with wrong files in it => if this happen, choose another name for the folder 

#test on arguments
#test if the input pssm list is correct
if [ "$1" == "-l" ] && [ -n "$2" ] && [ -f "$2" ] ; then pssm_list=true ;  fi

#test if the input pssm folder is correct
if [ "$3" == "-p" ] && [ -n "$4" ] && [ -d "$4" ] ; then pssm_folder_check=true ; fi

#test if the output is correct 
if [ "$5" == "-o" ] && [ -n "$6" ] ; then output_check=true ; fi 

list_reference=$2
pssm_path=$4
folder_path=$6

#if all the tests are true, we execute de command line
if [ "$pssm_list" == true ] && [ "$pssm_folder_check" == true ] && [ "$output_check" == true ]; 
	then
	#commande with log output
	echo CONCAT_PSSM >>log.txt;

	if [ ! -e "$folder_path" ] ; then mkdir $folder_path ; fi ;

	start=$SECONDS
	for i in `cat $list_reference | awk '{print $1}'` ; 
		do if [ ! -e $folder_path$i.pssm ] ; then cp $pssm_path$i.pssm $folder_path$i.pssm 2>>log.txt ; fi
	done;
		
	duration=$(( SECONDS - start ))	
	echo "concat pssm done in $duration second(s)" ;

else 
	#when "-help" isn't explicitly inputed, it show which argument was wrong
	if [ "$pssm_list" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "input list of pssm is incorrect, type \"-help\" for more informations" ;  fi
	if [ "$pssm_folder_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "pssm folder argument is invalid, type \"-help\" for more informations" ; fi
	if [ "$output_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "the output is incorect, type \"-help\" for more informations" ; fi 
	
fi ;


#if "-help" has been explicitly inputted, it shows the help, as when nothing is inputted
if [ "$#" == "0" ] || [ "$1" == "-help" ] ; then echo "
This bash script is used for the compilation of pssm file indexed in a list, from a input folder, to an output folder.

The first argument -l must be the list of pssm.
the second argument -p must be the folder that contain all the pssm models that will be copied. 
The third argument -o specifies the output folder.

Typical usage of the script :
./comparaison.sh -l pssm_list.list -p pssmModels/ -o pssm_concat/" ; fi




