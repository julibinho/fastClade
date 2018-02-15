#include<iostream> //To write in the console
#include<time.h> //to get the execution time and a seed
#include<dirent.h> // to read files in a directory
#include<sys/types.h> //to rad files in a directory
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

std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string);
std::set<std::uint32_t> shingling_a_document(std::string);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string);
std::vector<std::string> liste_fichiers_dossier(std::string);
std::vector<int> pickRandomCoeffs(int);
std::vector<int> minHashSignature(std::set<std::uint32_t>, std::vector<int>, std::vector<int>);
std::unordered_map<std::string, std::vector<int> > minHashSignatures(std::unordered_map<std::string, std::set<std::uint32_t> >);

static const int TAILLE_SHINGLES = 4;
static const int NOMBRES_HASH = 10;


int main(){
  printf("starting to shingling my datas\n");
  clock_t t;
  clock_t t2;
  t = clock();
  std::unordered_map<std::string,std::set<std::uint32_t> > res = shingling_documents("../../dataset/fastas");
  t = clock() - t;
  printf("I just finished the shingling operation, took me : %f seconds\n", ((float)t)/CLOCKS_PER_SEC);
  printf("starting to calculate the minHahsh signatures for the all set\n");
  t2 = clock();
  std::unordered_map<std::string, std::vector<int> > res_signature = minHashSignatures(res);
  t2 = clock() - t2;
  printf("I just finished the minHash signatures operation, took me : %f seconds\n", ((float)t2)/CLOCKS_PER_SEC);
  printf("Total time elapsed : %f seconds\n", (float(t+t2)/CLOCKS_PER_SEC));
  // std::unordered_map<std::string, std::vector<int> >::iterator it = res_signature.begin();
  // for(int j = 0; j < res_signature.size(); j++){
  //   std::cout << (*it).first << std::endl;
  //   for(int i = 0; i < (*it).second.size(); i++){
  //     std::cout << (*it).second[i] << " , ";
  //   }
  //   std::cout << std::endl;
  //   it++;
  // }

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

  //We add the path to the files name to have a valid path and not just files name.
  for(int i = 0; i<liste_fichiers.size(); i++){
    liste_fichiers[i] =  directoryPath + liste_fichiers[i];
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
    crc_result = CRC::Calculate(myString, sizeof(myString), CRC::CRC_32());

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
  int max_shingle_id = pow(2.0,32.0) - 1.0;

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

std::vector<int> minHashSignature(std::set<std::uint32_t> shingles, std::vector<int> coeffA, std::vector<int> coeffB){
  /***
  * return a vector containing all of the minHash signatures for the set of shingles passed in argument
  *
  * shingles : The set of shingles to be hashed
  * coeffA : Vector of coefficient to use to create hash functions
  * coeffB : Vector of coefficient to use to create hash functions
  */

  std::vector<int> signature; //Our vector of signatures
  long nextPrime = 4294967311; //The next prime for a 32 bit integer (used in our hash functions)

  // NE PAS PRENDRE LES VALEURS ICI, ELLES DOIVENT ETRE DECIDE AVANT ! ERREUR !
  //std::vector<int> coeffA = pickRandomCoeffs(NOMBRES_HASH); //Our vector of coeffs in order to create differents hash functions
  //std::vector<int> coeffB = pickRandomCoeffs(NOMBRES_HASH); //Our vector of coeffs in order to create differents hash functions
  //ON LES DONNENT EN PARAMETRE ?

  std::set<std::uint32_t>::iterator it = shingles.begin();
  int hashCode; //our temporary HashCode
  int minHashCode = nextPrime + 1; //the minimum hash code find for that Hash function

  for (int i = 0; i < NOMBRES_HASH; i++){

    it = shingles.begin();
    minHashCode = nextPrime + 1;

    for(int j = 0; j < shingles.size(); j++){
      //Get the next hash code
      hashCode = (coeffA[i] * (*it) + coeffB[i]) % nextPrime;

      //keep a track of the lowest hash code with that hash function
      if(hashCode > minHashCode){
        minHashCode = hashCode;
      }

      it ++;
    }
    signature.push_back(minHashCode);
  }

  return signature;
}

std::unordered_map<std::string, std::vector<int> > minHashSignatures(std::unordered_map<std::string, std::set<std::uint32_t> > shingles_map){
  /***
  * Return a map of all the sequences with their minHash signatures
  *
  * shingles_map : the map of shingles sets with their name
  */
  std::unordered_map<std::string, std::vector<int> > signatures;// Our map of minHash signatures
  std::unordered_map<std::string, std::set<std::uint32_t> >::iterator it = shingles_map.begin();// = shingles_map.begin(); //our iterator over the map of shingles
  std::vector<int> coeffA = pickRandomCoeffs(NOMBRES_HASH);
  std::vector<int> coeffB = pickRandomCoeffs(NOMBRES_HASH);
  std::vector<int> signature; //Our buffer for the signature of each set of shingles

  for(int i = 0; i < shingles_map.size() ; i++){
    signature =  minHashSignature((*it).second, coeffA, coeffB);
    signatures.insert(std::pair<std::string, std::vector<int> >((*it).first, signature));
    it++;
  }

  return signatures;

  //FAUT INVERSER LES DEUX FONCTIONS POUR QUE CELLES DE PLUS HAUT NIVEAU SOIT PLUS HAUT
}
