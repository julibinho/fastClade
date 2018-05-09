#reformate and then compute the result file and show the result as [PSSM] [SCORE] [FASTA]


#test on arguments
#test if the result all vs all input file is correct
if [ "$1" == "-r" ] && [ -n "$2" ] && [ -f "$2" ] ; then result_file_check=true ;  fi


result_list=$2;




#if all the tests are true, we execute de command line
if [ "$result_file_check" == true ] ; 
	then 
	echo "" > PairedResult.txt
	###start of result formating
	start2=$SECONDS;
	#we first sort the fasta, like that, we have all the score of the same fasta located at the same spot, it allow a faster computation of the file 
	#complexity in O(sort)+O(x) , insted of O(x**) if for eache pssm, we need to check all the result file

	sort -k2,2 $result_list | awk '	
		#au begin, on initialise les champs => evite les 0 0 au debut du fichier
		BEGIN{current_fasta=$2 ; best_score=$12 ; best_pssm=$1}

		#lors que l on change de fasta que l on traite, alors on affiche les donnes recoltes : [FASTA] [SCORE] [PSSM] et on effectue le changement de fasta
		{if(current_fasta!=$2) {{print current_fasta" "best_score" "best_pssm}{current_fasta=$2 ; best_score=$12 ; best_pssm=$1}}
		
		#update du score et du pssm si jamais
		{if(best_score<$12) {{best_score=$12} {best_pssm=$1}}} 
	}' >> PairedResult.txt

	duration2=$(( SECONDS - start2 ))	
	echo "result formating done in $duration2 second(s)" ;
	###end of result formating
fi;



	#when "-help" isn't explicitly inputed, it show which argument was wrong
	if [ "$result_file_check" != true ] && [ "$#" != "0" ] && [ "$1" != "-help" ] ; then echo "result file input is incorrect, type \"-help\" for more informations" ;  fi
	




#if "-help" has been explicitly inputted, it shows the help, as when nothing is inputted
if [ "$#" == "0" ] || [ "$1" == "-help" ] ; then echo "
This bash script is used to format the all to all result to the best association of [PSSM] [SCORE] [FASTA] from the PSI_BLAST file result.
ouput is PairedResult.txt in the current folder

The first argument -r must be the all_vs_all result file.


Typical usage of the script :
./res_exploit.sh -r result_all_vs_all " ; fi

