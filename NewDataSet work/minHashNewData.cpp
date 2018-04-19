#include<iostream> //To write in the console
#include<time.h> //to get the execution time and a seed
#include<dirent.h> // to read files in a directory
#include<sys/types.h> //to read files in a directory
#include<unordered_map>
#include<fstream>
#include<set>
#include<vector>
#include"CRC.h" //Make the CRC
#include<iomanip>
#include<cstdint> //Unsigned integers
#include<stdlib.h> //Srand and rand
#include<algorithm> //find in a vector
#include<math.h> //pow function

std::unordered_map<std::string,std::string> calculateAllComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> >, std::unordered_map<std::string, std::vector<long> >);
std::pair<std::string,std::string> calculateComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> >, std::pair<std::string, std::vector<long> >);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string);
std::set<std::uint32_t> shingling_a_document(std::string);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string);
std::vector<std::string> liste_fichiers_dossier(std::string);
std::vector<int> pickRandomCoeffs(int);
std::vector<long> minHashSignature(std::set<std::uint32_t>, std::vector<int>, std::vector<int>);
std::unordered_map<std::string, std::vector<long> > minHashSignatures(std::unordered_map<std::string, std::set<std::uint32_t> >,std::vector<int>,std::vector<int>);
std::unordered_map<std::string, std::unordered_map<std::string, float> > calculateComparisonsAllvsAll (std::unordered_map<std::string, std::vector<int> >);

static const int TAILLE_SHINGLES = 2;
static const int NOMBRES_HASH = 1000;
static const float THRESHOLD = 0.4;


int main(){
  //printf("starting to shingling my datas\n");
  clock_t t;
  clock_t t2;
  clock_t t3;
  t = clock();
  std::unordered_map<std::string,std::set<std::uint32_t> > shinglesTrain = shingling_documents("/home/martin/projet_3I013/partC++/split/seed/seedPF04857/");
  std::unordered_map<std::string,std::set<std::uint32_t> > shinglesTest = shingling_documents("/home/martin/projet_3I013/partC++/split/test/allseq/");
  t = clock() - t;

  std::cout << "Taille_S : " << TAILLE_SHINGLES <<" nb hash : " << NOMBRES_HASH <<"  threshold : "<< THRESHOLD << "  Train : seedPF03880/    test : allseq/" <<  std::endl;
  //printf("I just finished the shingling operation, took me : %f seconds\n", ((float)t)/CLOCKS_PER_SEC);
  //printf("starting to calculate the minHahsh signatures for the all set\n");
  t2 = clock();
  std::vector<int> coeffA = pickRandomCoeffs(NOMBRES_HASH);
  std::vector<int> coeffB = pickRandomCoeffs(NOMBRES_HASH);
  std::unordered_map<std::string, std::vector<long> > train_signatures = minHashSignatures(shinglesTrain,coeffA,coeffB);
  std::unordered_map<std::string, std::vector<long> > test_signatures = minHashSignatures(shinglesTest,coeffA,coeffB);
  t2 = clock() - t2;
  //printf("I just finished the minHash signatures operation, took me : %f seconds\n", ((float)t2)/CLOCKS_PER_SEC);
  //printf("Total time elapsed : %f seconds\n", (float(t+t2)/CLOCKS_PER_SEC));

  //printf("Start the one to all minhash comparison\n");
  t3 = clock();
  //std::unordered_map<std::string, std::unordered_map<std::string, float> > res_comparaisons = calculateComparisonsAllvsAll(res_signature);
  //std::pair<std::string,std::string> resultComparison = calculateComparisonsOnevsAll(train_signatures,test_signature);
  std::unordered_map<std::string,std::string> resultComparisons = calculateAllComparisonsOnevsAll(train_signatures,test_signatures);
  printf("Took me : %f seconds\n",(float(t3)/CLOCKS_PER_SEC));

  std::unordered_map<std::string,std::string>::iterator it = resultComparisons.begin();
  int countRes = 0;
  int countTot = 0;
  for (it = resultComparisons.begin();it!=resultComparisons.end();it++){
    if((*it).second.compare("") != 0){
      countRes++;
    }
    std::cout << (*it).first << "  ---->  " << (*it).second << std::endl;
    countTot++;
  }
  //std::cout << "Nombres de Resultats possibles trouvés : " << countRes <<"\nNombres totals de sequences testées : " << countTot << std::endl;

}

std::unordered_map<std::string,std::string> calculateAllComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> > signaturesTrain, std::unordered_map<std::string, std::vector<long> > signaturesTest){
  std::unordered_map<std::string, std::vector<long> >::iterator itTest = signaturesTest.begin();
  std::unordered_map<std::string,std::string> result;
  for(itTest = signaturesTest.begin(); itTest!=signaturesTest.end(); itTest++){
    result.insert(calculateComparisonsOnevsAll(signaturesTrain, (*itTest)));
  }

  return result;
}
/***
std::pair<std::string,std::string> calculateComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> > signaturesTrain, std::pair<std::string, std::vector<long> > signatureToTest){
  /***
  * Return the first sequence name that have a great ressemblance with the one to test.
  *
  * signaturesTrain : The map of all the sequence (with their signatures) that you want to compare to your sequence
  *
  * signatureToTest : the signature of the sequence you want to compare to other
  

  std::unordered_map<std::string, std::vector<long> >::iterator itTrain = signaturesTrain.begin();
  //std::unordered_map<std::string, std::vector<int> >::iterator itTest = signatureToTest.begin();
  float similarity = 0.0; //The buffer for the similarity result with each comparison.
  //ADDED
  float bestSimilarity = similarity; //The buffer to see the value of the best foud similarity
  std::string bestResponse = ""; //The buffer for the best response name
  // NOT ADDED
  std::string resultName = ""; //The name of the winner
  float count = 0.0; //The counter of same signature
  std::vector<long> signatureTestSequence = signatureToTest.second; //The signature of your tested sequence
  std::vector<long> signature2; //The signature of the next sequence to compare to your test sequence

  while( (itTrain != signaturesTrain.end()) && (similarity < THRESHOLD)){ //stop at the first result that fit the treshold => doesn't explore further
    count = 0.0;
    signature2 = (*itTrain).second;
    resultName = (*itTrain).first;
    for (int i = 0; i < NOMBRES_HASH; i++){
      if(signature2[i] == signatureTestSequence[i]){
        count++;
      }
    }
    similarity = count/float(NOMBRES_HASH);
    //ADDED
    if (similarity > bestSimilarity){
      bestSimilarity = similarity;
      bestResponse = resultName;
    }
    //NOT ADDED
    itTrain++;
  }

  if(itTrain == signaturesTrain.end()){
    resultName = "";
  }
  std::cout << " Best similarity for "<< signatureToTest.first << ": " << bestSimilarity << " Wich correspond to " << bestResponse <<std::endl;
  return std::pair<std::string,std::string>(signatureToTest.first,resultName);
} ***/


std::pair<std::string,std::string> calculateComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> > signaturesTrain, std::pair<std::string, std::vector<long> > signatureToTest){

  /***
  * Return the first sequence name that have a great ressemblance with the one to test.
  *
  * signaturesTrain : The map of all the sequence (with their signatures) that you want to compare to your sequence
  *
  * signatureToTest : the signature of the sequence you want to compare to other
  */

  std::unordered_map<std::string, std::vector<long> >::iterator itTrain = signaturesTrain.begin();
  //std::unordered_map<std::string, std::vector<int> >::iterator itTest = signatureToTest.begin();
  float similarity = 0.0; //The buffer for the similarity result with each comparison.
  //ADDED
  float bestSimilarity = similarity; //The buffer to see the value of the best foud similarity
  std::string bestResponse = ""; //The buffer for the best response name
  // NOT ADDED
  std::string resultName = ""; //The name of the winner
  float count = 0.0; //The counter of same signature
  std::vector<long> signatureTestSequence = signatureToTest.second; //The signature of your tested sequence

  //ligne suivante verifie que signatureToTest contient bien les sequences à test 
  //std::cout << " nom de test normalement : "<< signatureToTest.first << std::endl;

  std::vector<long> signature2; //The signature of the next sequence to compare to your test sequence

  while( (itTrain != signaturesTrain.end()) && (similarity < THRESHOLD)){ //stop at the first result that fit the treshold => doesn't explore further
    count = 0.0;
    signature2 = (*itTrain).second;
    resultName = (*itTrain).first;
    for (int i = 0; i < NOMBRES_HASH; i++){
      if(signature2[i] == signatureTestSequence[i]){
        count++;
      }
    }
    similarity = count/float(NOMBRES_HASH);
    //ADDED // updated similarity
    if (similarity > bestSimilarity){
      bestSimilarity = similarity;
      bestResponse = resultName;
    }
    //NOT ADDED
    itTrain++;
  }

  //std::cout << " Best similarity for "<< signatureToTest.first << ": " << bestSimilarity << " Wich correspond to " << bestResponse <<std::endl;
  //if (bestSimilarity < THRESHOLD){
  //  std::cout << " Best similarity for "<< signatureToTest.first << ": " << bestSimilarity << " Wich correspond to "  << bestResponse << "  THRESHOLD :" << THRESHOLD<<std::endl; 
  //}

  if(bestSimilarity<THRESHOLD){ // if we don't add the bestSimilarity < TRESHOLD, if the best sequence is at the end, then, it will be wipped
    bestResponse = "";
  }

  return std::pair<std::string,std::string>(signatureToTest.first,bestResponse);
}


std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string directoryPath){
  /***
  * Return the map containing all the sequences in the directory associated with their set of shingles represented by 32-bits integers
  *
  * directoryPath : Path to the directory you want to analyze
  */

  //We add the / at the end of the path if it doesn't exist
  if((directoryPath.size() > 0) && directoryPath[directoryPath.size()-1] != '/'){
    directoryPath += '/';
  }

  // We get all of the files name in our directory path
  std::vector<std::string> liste_fichiers = liste_fichiers_dossier(directoryPath); //Notre liste de fichiers à vérifier

  //add a test to check if the folder exist, and if not, display a message

  //We add the path to the files name to have a valid path and not just files name.
  for(int i = 0; i<liste_fichiers.size(); i++){
    liste_fichiers[i] =  directoryPath + liste_fichiers[i];
    //std::cout << "file added to the shinglelisation " << i << std::endl;
  }

  std::unordered_map<std::string,std::set<std::uint32_t> > resultat; //Our end result
  std::string nom; //The string who contain the name of the sequence
  std::string sequence; //The sequence we want to analyze
  std::string line; //Our buffer for each line
  std::ifstream myFile; //The flux to the file

  for(auto& file: liste_fichiers){
    myFile.open(file); //Opening the file
    //If the file exists
    if(myFile.is_open()){

      //We get the name by looking at the first line
      std::getline(myFile,line);
      nom = line.substr(1,line.size() - 1);

      // We get the sequence by looking at all the other lines in the file
      sequence = "";
      while(std::getline(myFile,line) && line.size() > 0){
        sequence += line.substr(0,line.size());
      }

      //We add the result in our map
      resultat.insert(std::pair<std::string,std::set<std::uint32_t> >(nom,shingling_a_document(sequence)));

      myFile.close();
    }
    else {
	std::cout << "file does not exist : "<< std::endl;
    }
  }
  return resultat;
}


std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string filePath){
  /***
  * Return the map of all the sequences in the file with their shingles set represented by 32-bits integers
  *
  * filePath : The path to the file
  */

  std::unordered_map<std::string,std::set<std::uint32_t> > result; //Our final result
  std::string nom; //The string who will contains the name of the sequence
  std::string sequence; //The sequence to analyze
  std::set<std::uint32_t> set_shingles; // The shingles set that we got from the sequence to analyze
  std::string line; //Our buffer for each line
  std::ifstream myFile(filePath); //Our flux to the file

  //If the file exist
  if(myFile.is_open()){
    //We get the first name
    std::getline(myFile,line);
    nom = line.substr(1,line.size()-1);

    //We process all the other elements
    while(std::getline(myFile,line)){
      //If it's a sequence name line
      if(line.substr(0,1).compare(">") == 0){
        set_shingles = shingling_a_document(sequence);
        result.insert(std::pair<std::string,std::set<std::uint32_t> >(nom,set_shingles));
        //We get the next name to use
        nom = line.substr(1,line.size()-1);
        sequence = "";
      }
      else{
        sequence += line.substr(0,line.size());
      }
    }

    //We process the last sequence
    set_shingles = shingling_a_document(sequence);
    result.insert(std::pair<std::string,std::set<std::uint32_t> >(nom,set_shingles));
  }
  myFile.close();
  return result;
}


std::set<std::uint32_t> shingling_a_document(std::string sequence){
  /***
  * Return a set of shingles represented by 32-bits integers
  *
  * sequence : The string that we need to process
  */
  std::set<std::uint32_t> monSet;
  std::string shingleTmp = "";
  std::uint32_t crc_result;
  // For all the substring of TAILLE_SHINGLE character in the sequence
  for( int i = 0; i<=sequence.size()-TAILLE_SHINGLES; i++){
    //We build our shingle
    shingleTmp = sequence.substr(i,TAILLE_SHINGLES);
    const char * myString = shingleTmp.c_str();
    // We pass our shingle under a 32-bits integer with a CRC32
    crc_result = CRC::Calculate(myString, TAILLE_SHINGLES, CRC::CRC_32());

    monSet.insert(crc_result);
  }

  return monSet;
}

std::vector<std::string> liste_fichiers_dossier(std::string path){
  /***
  * Give the list of files in a directory
  * /!\ Will not look recursivly !
  *
  * path : The path to the directory
  */
  const char *path_c = path.c_str();
  std::vector<std::string> liste_resultat;
  std::string file_name = "";
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (path_c)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      file_name = std::string((ent->d_name));
      if(file_name[0] != '.'){
          liste_resultat.push_back(file_name);
      }
    }
    closedir (dir);
  }
  return liste_resultat;
}

std::vector<int> pickRandomCoeffs(int k){
  /***
  * Return a list of k random numbers (used to create hash functions)
  *
  * k : the size of the vector you want
  */

  std::vector<int> result; //Our end vector
  int randomTmp; //Our temporary random number
  int max_shingle_id = pow(2.0,31.0) - 1.0;

  //Set the seed and pass the first number beacause it's very predictable.
  srand (time(NULL));
  rand();

  for(int i = 0; i < k; i++){

    //Generating the number
    randomTmp = rand() % max_shingle_id;

    //While we have already picked this number
    while(std::find(result.begin(), result.end(), randomTmp) != result.end()){
      randomTmp = rand() % max_shingle_id;
    }

    result.push_back(randomTmp);
  }

  return result;
}

std::vector<long> minHashSignature(std::set<std::uint32_t> shingles, std::vector<int> coeffA, std::vector<int> coeffB){
  /***
  * return a vector containing all of the minHash signatures for the set of shingles passed in argument
  *
  * shingles : The set of shingles to be hashed
  * coeffA : Vector of coefficient to use to create hash functions
  * coeffB : Vector of coefficient to use to create hash functions
  */

  std::vector<long> signature; //Our vector of signatures
  long nextPrime = 4294967311; //The next prime for a 32 bit integer (used in our hash functions)

  std::set<std::uint32_t>::iterator it = shingles.begin();
  long hashCode; //our temporary HashCode
  long minHashCode = -1; //the minimum hash code find for that Hash function

  for (int i = 0; i < NOMBRES_HASH; i++){

    it = shingles.begin();
    minHashCode = -1;

    for(int j = 0; j < shingles.size(); j++){
      //Get the next hash code
      hashCode = (( (long)coeffA[i]) * (*it) + ((long)coeffB[i])) % nextPrime;

      //keep a track of the lowest hash code with that hash function
      if((hashCode > minHashCode) || minHashCode == -1){
        minHashCode = hashCode;
      }

      it ++;
    }
    signature.push_back(minHashCode);
  }

  return signature;
}

std::unordered_map<std::string, std::vector<long> > minHashSignatures(std::unordered_map<std::string, std::set<std::uint32_t> > shingles_map, std::vector<int> coeffA, std::vector<int> coeffB){
  /***
  * Return a map of all the sequences with their minHash signatures
  *
  * shingles_map : the map of shingles sets with their name
  */
  std::unordered_map<std::string, std::vector<long> > signatures;// Our map of minHash signatures
  std::unordered_map<std::string, std::set<uint32_t> >::iterator it = shingles_map.begin();//our iterator over the map of shingles
  //std::vector<int> coeffA = pickRandomCoeffs(NOMBRES_HASH);
  //std::vector<int> coeffB = pickRandomCoeffs(NOMBRES_HASH);
  std::vector<long> signature; //Our buffer for the signature of each set of shingles

  for(int i = 0; i < shingles_map.size() ; i++){
    signature =  minHashSignature((*it).second, coeffA, coeffB);
    signatures.insert(std::pair<std::string, std::vector<long> >((*it).first, signature));
    it++;
  }

  return signatures;
}

std::unordered_map<std::string, std::unordered_map<std::string, float> > calculateComparisonsAllvsAll (std::unordered_map<std::string, std::vector<std::uint32_t> > signatures){
  /***
  * Retturn the matrix of an all to all comparison using minHash signatures of sequences.
  *
  * signatures : The map of all sequence with their minHash signatures
  */

  std::unordered_map<std::string, std::unordered_map<std::string, float> > comparisons; //Our end result
  std::unordered_map<std::string, float> comparisonTmp;
  std::vector<std::uint32_t> signature1; //The first shingle of the comparison
  std::vector<std::uint32_t> signature2; //The second shingle of the comparison
  float count = 0.0; //the counter of equivalent minHash signatures.
  std::unordered_map<std::string, std::vector<std::uint32_t> >::iterator it1 = signatures.begin();
  std::unordered_map<std::string, std::vector<std::uint32_t> >::iterator it2;

  for (int i = 0; i < signatures.size()-1; i++){
    signature1 = (*it1).second;
    it2 = it1;
    it2++;

    for (int j = 0; j < signatures.size() - i - 1; j++){

      signature2 = (*it2).second;

      for(int k = 0; k<NOMBRES_HASH;k++){
        if(signature1[k] == signature2[k]){
          count++;
        }
      }

      comparisonTmp.insert(std::pair<std::string, float>((*it2).first, count/float(NOMBRES_HASH)));
      it2++;
      count = 0;
    }

    comparisons.insert(std::pair<std::string, std::unordered_map<std::string, float> >((*it1).first, comparisonTmp));
    comparisonTmp.clear();
    it1++;
  }

  return comparisons;
}
