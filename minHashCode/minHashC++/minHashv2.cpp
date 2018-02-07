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
#include<algorithm>

using namespace std;

std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string);
std::set<std::uint32_t> shingling_a_document(std::string);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string);
std::vector<std::string> liste_fichiers_dossier(std::string);
std::uint32_t getTriangleIndex(std::uint32_t i, std::uint32_t j,std::uint32_t numDocs);


int main(){
  printf("starting to shingling my datas...\n");
  clock_t t;
  t = clock();
  std::unordered_map<std::string,std::set<std::uint32_t> > res = shingling_documents("../../dataset/fastas");
  t = clock() - t;
  printf("I just finished my operation, took me : %f seconds\n", ((float)t)/CLOCKS_PER_SEC);
}

std::uint32_t getTriangleIndex(std::uint32_t i, std::uint32_t j,std::uint32_t numDocs){
  /** fonction qui calcule l'indice d'une matrice à 1 dimension
  * a partir d'une matrice à 2 dimensions
  **/

  if (i==j){ //si i==j c'est une erreur, et on exit
    count << "Can't access triangle matrix with i == j" << endl;
    exit(1);
  }

  if (j<i){ //si j<i on inverse les valeures
    std::uint32_t temp=i;
    i=j;
    j=temp;
  }

  //calcule l'indice de la matrice à 1 dimension, et le return
  return uint32_t(i*(numDocs-(i+1)/2.0)+j-i) -1;
}

std::uint32_t jaccalc(std::unordered_map<std::string,std::uint32_t> tab ){ //necessite <algorithme> et <time>
  /**
  *Calcule l'indice de similarite de jaccard,
  *néanmoins, besoin des shingles pour ce faire
  * => je dois récupérer le tableau de single avant le hashage
  * le mieux serais un tableau de type grossomodo (string, string[]) : pour ("nom",["singles"]);
  **/
  std::uint32_t numElems= uint32_t(tab.size() * (tab.size()-1) / 2);
  std::uint32_t JSim[numElems];
  std::uint32_t estJSim[numElems];

  time_t debut ;
  time(&debut);

  cout << "premier prise de temps : " << debut << endl;

  //boucle qui fait nbDoc tours
  for(std::uint32_t i=0; i<tab.size();i++){
    // affichage basique tous les 100tour de boucle
    if (i%100) == 0{
      cout << "  (" << i << " / " << tab.size() << ")" << endl;
      }

      //recupération de la liste des shingles pour le document i (selon le format en commantaire
      std::string[] s1 = tab[i][1];
      std::sort(s1,s1+(s1.size)); //on sort s1, car on aura besoin qu'elle soit rangee pour faire l'intersection et l'union

      for (int j=i+1; j<tab.size();j++){
        // retrouve le set de shingles pour le document j
        std::string[] s2 = tab[j][1];
        std::sort(s2,s2+(s2.size)); //on sort s2, car on aura besoin qu'elle soit rangee pour faire l'intersection et l'union


        //calcule et garde l'indice de similarite de jaccard

        //permet d'obtenir l'union
        std::vector<string> v(s1.size+s2.size);
        std::vector<string>::iterator it;
        it=std::set_union(s1,s1+(s1.size()),s2,s2+(s2.size()),v.begin());
        v.resize(it-v.begin());

        //permet d'obtenir l'intersection
        std::vector<string> v2(s1.size()+s2.size());
        std::vector<string>::iterator it2;
        it=std::set_intersection(s1,s1+(s1.size()),s2,s2+(s2.size()),v2.begin());
        v2.resize(it2-v2.begin());


        JSim[std::uint32_t getTriangleIndex(i,j,tab.size())] = (v1.size()/v2.size);
      }
    }
  clock_t fin = time(&fin);
  cout << "temps total : " << fin-debut << " secondes" << endl;
  return 0;
}


std::unordered_map<std::string,std::set<std::uint32_t> > shingling_documents(std::string directoryPath){
  /***
  * Retourne le dictionnaire contenant toutes les séquences des fichiers contenues dans le dossier associés avec leurs ensembles de set_shingles
  * sous la forme d'entier de 32 bits.
  *
  * directoryPath : Le chemin du dossier à analyser
  */

  //On ajoute le / a la fin du path s'il n'existe pas.
  if((directoryPath.size() > 0) && directoryPath[directoryPath.size()-1] != '/'){
    directoryPath += '/';
  }

  //on récupère la liste de tous nos fichiers dans notre dossier path
  std::vector<std::string> liste_fichiers = liste_fichiers_dossier(directoryPath); //Notre liste de fichiers à vérifier

  //On ajoute le path au nom des fichiers obtenus pour avoir un path complet et nom juste les noms de fichiers.
  for(int i = 0; i<liste_fichiers.size(); i++){
    liste_fichiers[i] =  directoryPath + liste_fichiers[i];
  }

  std::unordered_map<std::string,std::set<std::uint32_t> > resultat; //Notre résultat final
  std::string nom; //La chaine qui va contenir l'identifiant de la séquence
  std::string sequence; //La séquence que l'on doit annalyser
  std::string line; //Notre conteneur temporaire de chaque ligne
  std::ifstream myFile; //Notre flux vers chaque fichier.

  for(auto& file: liste_fichiers){
    myFile.open(file); //On ouvre notre fichier
    //Si on a réussi à accéder à notre fichier
    if(myFile.is_open()){

      //On obtient le nom en regardant la première ligne
      std::getline(myFile,line);
      nom = line.substr(1,line.size() - 1);

      //On obtient la séquence en regardant toutes les autres lignes du fichier
      sequence = "";
      while(std::getline(myFile,line) && line.size() > 0){
        sequence += line.substr(0,line.size());
      }

      //On insère le résultat dans notre dictionnaire
      resultat.insert(std::pair<std::string,std::set<std::uint32_t> >(nom,shingling_a_document(sequence)));

      myFile.close();
    }
  }
  return resultat;
}

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
      if(file_name[0] != '.'){
          liste_resultat.push_back(file_name);
      }
    }
    closedir (dir);
  }
  return liste_resultat;
}
