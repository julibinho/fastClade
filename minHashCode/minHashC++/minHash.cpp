#include<iostream>
#include <time.h>
#include<dirent.h>
#include<sys/types.h>
#include<unordered_map>
#include<fstream>
#include<set>
#include<vector>
#include"CRC.h"
#include<iomanip>
#include<cstdint>

std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string);
std::set<std::uint32_t> shingling_a_document(std::string);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string);
std::vector<std::string> liste_fichiers_dossier(std::string);

static const int TAILLE_SHINGLES = 4;

int main(){
  printf("starting to shingling my datas...\n");
  clock_t t;
  t = clock();
  std::unordered_map<std::string,std::set<std::uint32_t> > res = shingling_documents("../../dataset/fastas");
  t = clock() - t;
  printf("I just finished my operation, took me : %f seconds\n", ((float)t)/CLOCKS_PER_SEC);
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
  for( int i = 0; i<sequence.size()-TAILLE_SHINGLES; i++){
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
