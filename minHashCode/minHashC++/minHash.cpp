#include<iostream>
#include<dirent.h>
#include<sys/types.h>
#include<unordered_map>
#include<fstream>
#include<set>
#include<vector>
#include"CRC.h"
#include<iomanip>
#include<cstdint>

std::set<std::uint32_t> shingling_a_document(std::string);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string);
std::vector<std::string> liste_fichiers_dossier(std::string);

static const int TAILLE_SHINGLES = 4;

int main(){
  std::unordered_map<std::string,std::set<std::uint32_t> > res = shingling_long_document("../fastas/concat_test.fasta");
}

/****************** FONCTION A FINIR DE CONSTRUIRE ************************/
// std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string directoryPath){
//
//   std::vector<std::string> liste_fichiers = liste_fichiers_dossier(directoryPath); //Notre liste de fichiers à vérifier
//   std::string nom; //La chaine qui va contenir l'identifiant de la séquence
//   std::string sequence; //La séquence que l'on doit annalyser
//   std::string line; //Notre conteneur temporaire de chaque ligne
//   for(auto& file: liste_fichiers){
//     ifstream myFile(file); //On ouvre notre fichier
//     //Si on a réussi à accéder à notre fichier
//     if(myFile.is_open()){
//       std::getLine(myFile,line);
//       nom = line
//     }
//   }
//   std::ifstream fileCourant;
// }
/****************************************************************************/

std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string filePath){
  /***
  * Retourne le dictionnaire contenant toutes les séquences du fichier associé avec leurs ensembles de set_shingles
  * sous la forme d'entier de 32 bits.
  *
  * filePath : Le chemin du fichier à analyser
  */

  std::unordered_map<std::string,std::set<std::uint32_t> > result; //Notre dictionnaire final
  std::string nom; //La chaine qui va contenir l'identifiant de la séquence
  std::string sequence; //La séquence que l'on doit annalyser
  std::set<std::uint32_t> set_shingles; //le set de Shingles que l'on récupère pour la séquence à analyser
  std::string line; //Notre conteneur temporaire de chaque ligne
  std::ifstream myFile(filePath); //Notre flux vers le fichier

  //Si le fichier est ouvert
  if(myFile.is_open()){
    //On obtient le premier nom
    std::getline(myFile,line);
    nom = line.substr(1,line.size()-1);

    //On traite tous les éléments suivants
    while(std::getline(myFile,line)){
      //Si c'est une ligne de nom de séquence
      if(line.substr(0,1).compare(">") == 0){
        set_shingles = shingling_a_document(sequence);
        result.insert(std::pair<std::string,std::set<std::uint32_t> >(nom,set_shingles));
        //On obtient le prochain nom à utiliser
        nom = line.substr(1,line.size()-1);
        sequence = "";
      }
      else{
        sequence += line.substr(0,line.size());
      }
    }

    //On gère la dernière séquence
    set_shingles = shingling_a_document(sequence);
    result.insert(std::pair<std::string,std::set<std::uint32_t> >(nom,set_shingles));
  }
  myFile.close();
  return result;
}


std::set<std::uint32_t> shingling_a_document(std::string sequence){
  /***
  * Retourne l'ensemble des shingles d'une chaine sous la forme d'entier de 32 bits
  *
  * sequence : La chaine de caractère à transformer
  */
  std::set<std::uint32_t> monSet;
  std::string shingleTmp = "";
  std::uint32_t crc_result;
  //Pour toutes les combinaisons TAILLE_SHINGLE lettres dans la sequence
  for( int i = 0; i<sequence.size()-TAILLE_SHINGLES; i++){
    //On construit notre shingle
    shingleTmp = sequence.substr(i,TAILLE_SHINGLES);
    const char * myString = shingleTmp.c_str();
    //On passe notre shingle sous la forme d'un entier de 32 bit avec un CRC32
    crc_result = CRC::Calculate(myString, sizeof(myString), CRC::CRC_32());

    monSet.insert(crc_result);
  }

  return monSet;
}

std::vector<std::string> liste_fichiers_dossier(std::string path){
  /***
  * Donne la liste des fichiers dans un dossier (sous forme de string)
  * /!\ Ne va pas chercher récursivement dans les sous dossiers du dossier initial
  *
  * path : Le cHemin du dossier dont il faut récupérer les fichiers.
  */
  const char *path_c = path.c_str();
  std::vector<std::string> liste_resultat;
  std::string file_name = "";
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (path_c)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      file_name = std::string((ent->d_name));
      if(file_name.compare(".") != 0 && file_name.compare("..") != 0){
          liste_resultat.push_back(file_name);
      }
    }
    closedir (dir);
  }
  return liste_resultat;
}
