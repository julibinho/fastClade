# -*- coding: utf-8 -*-



"""
This program take in input 2 folder, and a File.
 * The first folder should contain one or more list of seed protein in collumn 1, and their family in collumn 2
 * The second folder should contain list of query protein in collumn 1, and their family in collumn 2
 * The File should be the output of the MinHash Algorihtme as [query] [match protein]

How it work :
The program build two dictionnary as [protein] [family] of the cSeq and cSeed list.
Then Check the file and for each query protein, it look in the cSeq Dictionnary, which family it is. Then it check the family of the matching protein.

, then with the proteins of cSeq.
Then, it read the result file. 
If the first column (the query) is contained in cSeq, then we know it should appartain to cSeed to.
    -> in contrary, if the query isn't contained in cSeq, it shouldn't appartain to cSeed.
those two case already give the number of True positif, and True negatif.
Then :
    If a query isn't contained in cSeq but in cSeed, it's a false positiv, since we know that cSeq appartain in the family and others don't.
    If a query is conained in cSeq but not in cSeed, it's a false negativ, since it should have been reconised, but is not.
"""

import os
import sys
from os.path import exists



#assign variables + verifications
###################
try :
    cSeedFolder=sys.argv[1]
except IndexError:
    print("pas d'argument 1, exit")
    quit()


try :
    cSeqFolder=sys.argv[2]
except IndexError:
    print("pas d'argument 2,exit")
    quit()


try :
    resultfile=sys.argv[3]
except IndexError:
    print("pas d'argument 3,exit")
    quit()

try :
    outputTrue=sys.argv[4]
except IndexError:
    print("pas d'argument 4,exit")
    quit()

try :
    outputFalse=sys.argv[5]
except IndexError:
    print("pas d'argument 5,exit")
    quit()


if (not exists(cSeedFolder)):
	print("arg1 doesn't exist : exit")
	quit()

if (not exists(cSeqFolder)):
	print("arg2 doesn't exist : exit")
	quit()

if (not exists(resultfile)):
	print("arg3 doesn't exist : exit")
	quit()

###################################
#cSeedFolder="./allSeedFormated"
#cSeqFolder="./allseqFormated"
#resultfile="./SeedFolder-QueryFolder-3-2000/comparaison"

checkSeed=False
checkSeq=False

################# Construction de la base de donnee :


def constructDicoFromFolder(path):
    a={}
    i=0
    for filename in os.listdir(path):
        f=open(path+"/"+filename,'r')
        for line in f:
	    if (line.find('>')!=-1):
		line=line.strip('\n')
		line=line.strip('>')
		temp1=line	
		#print(line)
		bool1=True
	    else :
		if (line.find('/')!=-1):
		    line=line[line.find('/')+1:len(line)]
		    #print("line :"+line)
		    temp2=line.strip('\n')
		    #print("temp2 :"+temp2)
		    if temp2==None :
			print("temp1 Bugge :"+temp1)
		    bool2=True

	if (bool1==True and bool2==True):
	    i+=1
	    bool1=False
	    bool2=False
	    #print(temp1+"   "+temp2)
	    a[temp1]=temp2


    print ("for the folder "+path+"    "+str(i)+" proteins added in dictionary")
    return a


def exploitRes(SeedDB,SeqDB,result,outputTrue,outputFalse):
    
    TPset={}
    FPset={}

    TP=0.0;
    TN=0.0;
    FP=0.0;
    FN=0.0;
    expected=len(SeqDB);
    
    
    
    #line 0 => query
    #line 1 => seed
    OT = open(outputTrue, 'w')
    OF = open(outputFalse, 'w')
    f=open(result,'r')
    for line in f :
        line=line.split()
        QueryProt=line[0]
	SeedProt=line[1]

	QueryFamily=SeqDB.get(QueryProt)
	SeedFamily=SeedDB.get(SeedProt)

	#print ("Query :"+QueryProt+"  "+QueryFamily)
	#print (" Seed :"+SeedProt+"  "+SeedFamily)

        if(QueryFamily==SeedFamily):
	    if(QueryFamily==None or SeedFamily==None):
		print("None object : "+QueryProt+"   "+SeedProt)
            TP+=1
	    OT.write(QueryProt+"\t"+QueryFamily+"\t"+SeedProt+"\t"+SeedFamily+"\n")

	elif(QueryFamily==None or SeedFamily==None):
            if(QueryFamily==None):
		QueryFamily="None"
	    if(SeedFamily==None):
		SeedFamily="None"

	    FN+=1
	    OF.write(QueryProt+"\t"+QueryFamily+"\t"+SeedProt+"\t"+SeedFamily+"\n")


        elif(QueryFamily!=SeedFamily):
	    if(QueryFamily==None or SeedFamily==None):
		print("None object : "+QueryProt+"   "+SeedProt)
            FP+=1
	    OF.write(QueryProt+"\t"+QueryFamily+"\t"+SeedProt+"\t"+SeedFamily+"\n")

    f.close()
    OT.close()
    OF.close()
    print ("\n\n########### results :")
    print ("True positif : "+str(TP)+ " sur "+str(expected)+" soit : "+str((TP/expected)*100.0)+"%");
    print ("False Positif : "+str(FP)+ " sur "+str(expected)+" soit : "+str((FP/(expected))*100.0)+"%");
    print ("False Negatif : "+str(FN)+ " sur "+str(expected)+" soit : "+str((FN/(expected))*100.0)+"%");


    #print ("True nefatif : ",TN, "sur ",total-expected," soit : ",(TN/(TN+FP))*100,"%");
    
    #print ("False Positif : ",FP, "sur ",total-expected," soit : ",(FP/(total-expected))*100,"%");
    #print ("False Negatif : ",FN, "sur ",expected," soit : ",(FN/expected)*100,"%");
    
    print("\n \n#### stats :")
    print("Precision :"+str(((TP)/(TP+FP))*100))
    print("Recall :"+str(((TP)/(TP+FN))*100))
    print("Accuracy :"+str(((TP+TN)/(TP+TN+FP+FN))*100))

	#FSCORE : 2*RECALL * PRECISION // RECALL + PRECISION
    print("FScore :"+str(((( 2* ((TP)/(TP+FN)) * ((TP)/(TP+FP))) /(((TP)/(TP+FN))+((TP)/(TP+FP)))))*100))




a=constructDicoFromFolder(cSeedFolder)
b=constructDicoFromFolder(cSeqFolder)
exploitRes(a,b,resultfile,outputTrue,outputFalse)
#exploitRes(constructSetFromFilename(cSeed),constructSetFromFilename(cSeq),resultfile)
