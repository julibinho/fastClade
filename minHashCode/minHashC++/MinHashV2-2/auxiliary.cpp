#include<dirent.h> // to read files in a directory
#include<sys/types.h> //to read files in a directory
#include<math.h> //pow function
#include<stdlib.h> //Srand and rand
#include<string>
#include<unordered_map>
#include<set>
#include<vector>
#include<time.h> //to get the execution time and a seed
#include<fstream>
#include<iostream>

extern int TAILLE_SHINGLES;
extern int NOMBRES_HASH;

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


std::vector<std::string> splitString(std::string str, std::string delimiter){
  /**
   * Return all the substrings between two occurences of the delimiter
   *
   * str: The string you want to parse
   *
   *  delimiter: The delimiter of each substring
   */

   std::vector<std::string> result; //Our vector of substrings
   std::string token; //Our buffer for each substring
   int posDel = 0; //The position in the string of the next delimiter

   while (posDel != -1){

     // We get the token
     posDel = str.find(delimiter);
     token = str.substr(0,posDel);

     // if the substring is not eampty, we can add it to our result
     if (token.size() > 0){
       result.push_back(token);
     }

     //We get the next string to evaluate
     str = str.substr(posDel + delimiter.size(),str.size());
   }

   return result;
}

std::pair<std::vector<int>,std::vector<int> > pickRandomCoeffs(int k){
  /***
  * Return two lists of k random numbers (used to create hash functions)
  *
  * k : the size of the vector you want
  */

  std::vector<int> resultA; //Our first vector
  std::vector<int> resultB; //our second vector
  std::pair<std::vector<int>,std::vector<int> > finalResult; //Our final pair result
  int randomTmp; //Our temporary random number
  int max_shingle_id = pow(2.0,31.0) - 1.0;

  //Set the seed and pass the first number beacause it's very predictable.
  srand (time(NULL));
  rand();

  for(int i = 0; i < k; i++){

    //Generating the number
    randomTmp = rand() % max_shingle_id;
    resultA.push_back(randomTmp);

    randomTmp = rand() % max_shingle_id;
    resultB.push_back(randomTmp);
  }
  finalResult.first = resultA;
  finalResult.second = resultB;
  return finalResult;
}

int writeDatabase(std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > signatures, std::vector<int> coeffA, std::vector<int> coeffB, std::string filePath){
  /**
   * Write in the file given in argument the minHash database given in signatures
   * signatures : A set of protein categorized by family and associated with their minHah signature
   * filePath: The path where you want to write the database
   */

   //Our iterators
   std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > >::iterator itFamily;
   std::unordered_map<std::string,std::vector<long> >::iterator itProt;

   std::ofstream myFile(filePath);
   if(myFile.is_open()){

     //Write the parameters on the first 4 lines
     // The two first lines are for TAILLE_SHINGLES and NOMBRE_HASH
     myFile << TAILLE_SHINGLES << "\n";
     myFile << NOMBRES_HASH << "\n";

     //The two other lines are for the coefficient of hashs functions
     std::string coeffAstr = "";
     std::string coeffBstr = "";
     for(int i = 0; i<coeffA.size(); i++){
       coeffAstr += std::to_string(coeffA[i]) + "\t";
       coeffBstr += std::to_string(coeffB[i]) + "\t";
     }

     myFile << coeffAstr << "\n";
     myFile << coeffBstr << "\n";


     for(itFamily = signatures.begin();itFamily != signatures.end();itFamily++){
       std::string family = (*itFamily).first;
       myFile << ">";
       myFile << family;
       myFile << "\n";

       for(itProt = signatures[family].begin(); itProt != signatures[family].end(); itProt++){
         std::string name = (*itProt).first;
         std::string signature = "";

         for(int i = 0; i < (*itProt).second.size(); i++){
           signature += std::to_string((*itProt).second[i]) + "\t";
         }

         myFile << name;
         myFile << "\t";
         myFile << signature.substr(0,signature.size()-1);
         myFile << "\n";
       }
       myFile << "\n";
     }

   }
   return 0;
}

std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > readDatabase(std::string databasePath){
  /**
   * Return the data structure that correspond to the sequence send with the parameter.
   * database : the database you want to convert
   */

   std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > result;
   std::vector<long> signatureTmp;
   std::string family;
   std::string name;
   std::vector<std::string> splitLine;
   std::string line;
   std::ifstream myFile(databasePath);

   if(myFile.is_open()){

     //We don't read the first 4 lines
     for(int i = 0;i<4;i++){
       std::getline(myFile,line);
     }

     //We start to read from here
     while(std::getline(myFile,line)){

       //If we can read something
       if(line.size() > 0){

         //If it's the start of a family
         if(line[0] == '>'){
           signatureTmp.clear();
           family = line.substr(1,line.size());

           //We initialise the family
           result.insert(std::pair<std::string, std::unordered_map<std::string,std::vector<long> > >(family,std::unordered_map<std::string,std::vector<long> >()));

         }

         else{
           splitLine = splitString(line,"\t");
           name = splitLine[0];

           for(int i = 1;i<splitLine.size();i++){
             signatureTmp.push_back(std::stol(splitLine[i]));
           }

           result[family].insert(std::pair<std::string,std::vector<long> >(name,signatureTmp));
           signatureTmp.clear();
         }

       }
     }

   }
   return result;
}


int writeResultDistribution(std::unordered_map<std::string,std::pair<std::string,std::string> > dataComparisons,std::string pathToWrite){
  /**
   * Write in a file the result obtain by the comparison function
   */

  std::ofstream myFile(pathToWrite);
  std::unordered_map<std::string,std::pair<std::string,std::string> >::iterator it;

  if(myFile.is_open()){
    for(it = dataComparisons.begin(); it != dataComparisons.end(); it++){
      std::string name = (*it).first;
      std::string family = (*it).second.first;
      std::string bestBet = (*it).second.second;

      myFile << name << "\t" << bestBet << "\t" << family << "\n";
    }
  }

  return 0;
}
