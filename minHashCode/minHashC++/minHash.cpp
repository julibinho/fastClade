#include<unordered_map>
#include<set>
#include<vector>
#include<cstdint> //Unsigned integers

#include<iostream> //To write in the console
// #include<time.h> //to get the execution time and a seed
// #include<dirent.h> // to read files in a directory
// #include<sys/types.h> //to read files in a directory
// #include<unordered_map>
// #include<set>
// #include<vector>
// #include<fstream>
// #include"CRC.h" //Make the CRC
// #include<iomanip>
// #include<cstdint> //Unsigned integers
// #include<stdlib.h> //Srand and rand
// #include<algorithm> //find in a vector
// #include<math.h> //pow function

#include "shingles.h"

extern int TAILLE_SHINGLES;
extern int NOMBRES_HASH;
extern float THRESHOLD;
extern float THRESHOLD_SUBSTITUTION;

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

std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > minHashSignatures(std::unordered_map<std::string, std::unordered_map<std::string,std::set<std::uint32_t> > > shingles_map, std::vector<int> coeffA, std::vector<int> coeffB){
  /**
  * Return a map of all the sequences with their minHash signatures
  *
  * shingles_map : the map of shingles sets with their name
  */
  std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > signatures;// Our map of minHash signatures
  std::unordered_map<std::string, std::unordered_map<std::string,std::set<std::uint32_t> > >::iterator itFamily = shingles_map.begin();//our iterator over the map of shingles family

  std::unordered_map<std::string,std::set<std::uint32_t> >::iterator itProt; //Our iterator for each protein
  std::vector<long> signature; //Our buffer for the signature of each set of shingles

  // for(int i = 0; i < shingles_map.size() ; i++){
  //   signature =  minHashSignature((*it).second, coeffA, coeffB);
  //   signatures.insert(std::pair<std::string, std::vector<long> >((*it).first, signature));
  //   it++;
  // }

  //for each family
  for(itFamily = shingles_map.begin(); itFamily != shingles_map.end();itFamily++){
    //we create that family
    std::string family = (*itFamily).first;
    signatures.insert(std::pair<std::string, std::unordered_map<std::string,std::vector<long> > >(family, std::unordered_map<std::string,std::vector<long> >()));

    //for each protein
    for(itProt = shingles_map[family].begin(); itProt != shingles_map[family].end(); itProt++){
      signature =  minHashSignature((*itProt).second, coeffA, coeffB);
      signatures[family].insert(std::pair<std::string, std::vector<long> >((*itProt).first, signature));
    }
  }

  return signatures;
}

float minHashDistance(std::vector<long> protA, std::vector<long> protB){
  /**
   * Return the minHash distance of 2 proteines represent by their minHash signatures
   */

  float count = 0;
  for(int i = 0; i<NOMBRES_HASH; i++){
    if (protA[i] == protB[i]){
      count++;
    }
  }
  //std::cout << count/float(NOMBRES_HASH) << std::endl;
  return count/float(NOMBRES_HASH);
}
