#include<iostream> //To write in the console
#include<unordered_map>
#include<set>
#include<vector>
#include<cstdint> //Unsigned integers

#include"auxiliary.h"
#include"shingles.h"
#include"minHash.h"
#include"distribution.h"

extern int TAILLE_SHINGLES;
extern int NOMBRES_HASH;


float minDistanceJaccardProts(std::unordered_map<std::string, std::set<std::uint32_t> > familyA, std::set<std::uint32_t> protToAnalyze){ //,std::unordered_map<std::string, std::set<std::uint32_t> > familyB){
  /**
   * Return the minimal jaccard distance between members of familyA and the protein to analyze
   */

  float minDist = -1;
  float distance = 0;
  for(std::unordered_map<std::string, std::set<std::uint32_t> >::iterator itA=familyA.begin(); itA != familyA.end(); itA++){
    //for(std::unordered_map<std::string, std::set<std::uint32_t> >::iterator itB=familyB.begin(); itB != familyB.end(); itB++){
    distance = jaccardSim((*itA).second, protToAnalyze);
    if (minDist == -1 || distance > minDist){
      minDist = distance;
    }
  }

  return minDist;
}

std::pair<std::string,float> minDistanceMinHashProts(std::unordered_map<std::string, std::vector<long> > familyA, std::vector<long> protToAnalyze){
  /**
   * Return the minimal jaccard distance between members of familyA and the protein to analyze
   */

  float minDist = -1;
  float distance = 0;
  std::string name;

  for(std::unordered_map<std::string, std::vector<long> >::iterator itA=familyA.begin(); itA != familyA.end(); itA++){
    distance = minHashDistance((*itA).second, protToAnalyze);
    if (minDist == -1 || distance > minDist){
      minDist = distance;
      name = (*itA).first;
    }
  }
  std::pair<std::string,float> result = std::make_pair(name,minDist);
  return result;
}

std::vector<std::pair<float,float> > getDistributionsJaccard(std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > > trainSet, std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > > testSet){
  /**
   * Return for each family the minimum jaccard similiraty intra-Family and extra-family
   */

  std::vector<std::pair<float,float> > result;
  std::pair<float,float> tmpResult = std::pair<float,float>();
  float minDist = -1;
  float dist = 0;
  std::string family;

  // For every family in the test set
  for(std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > >::iterator itTest = testSet.begin(); itTest != testSet.end(); itTest++){
    family = (*itTest).first;

    //For every member of this family
    for(std::unordered_map<std::string, std::set<std::uint32_t> >::iterator itMember = (*itTest).second.begin(); itMember != (*itTest).second.end(); itMember++){
      tmpResult = std::pair<float,float>();
      tmpResult.first = minDistanceJaccardProts(trainSet[family],(*itMember).second);//(*itTest).second,trainSet[(*itTest).first]);

      minDist = -1;
      for(std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > >::iterator itTrain = trainSet.begin(); itTrain != trainSet.end(); itTrain++){
        if((*itTrain).first != family){
          dist = minDistanceJaccardProts((*itTrain).second,(*itMember).second);
          if (minDist == -1 || dist>minDist){
            minDist = dist;
          }
        }
      }

      tmpResult.second = minDist;
      result.push_back(tmpResult);
    }

  }

  return result;
}


std::unordered_map<std::string,std::pair<std::pair<std::string,float>,std::pair<std::string,float> > > getDistributionsMinHash(std::unordered_map<std::string, std::unordered_map<std::string, std::vector<long> > > trainSet, std::unordered_map<std::string, std::unordered_map<std::string, std::vector<long> > > testSet){
  /**
   * Return for each protein in the test set, the best correspondant protein in its family and outside its family
   * trainSet : the set composed of train examples
   * testSet : the set to analyze
   */

   std::unordered_map<std::string,std::pair<std::pair<std::string,float>,std::pair<std::string,float> > > result;
   std::unordered_map<std::string, std::unordered_map<std::string, std::vector<long> > >::iterator itTest;
   std::unordered_map<std::string, std::unordered_map<std::string, std::vector<long> > >::iterator itTrain;
   std::unordered_map<std::string, std::vector<long> >::iterator itMember;

   //for each family of the testSet
   for(itTest = testSet.begin(); itTest != testSet.end(); itTest++){
     std::string family = (*itTest).first;

     //for each member in that family
     for(itMember = testSet[family].begin(); itMember != testSet[family].end(); itMember++){
       //We initialize our map for this name.
       std::string name = (*itMember).first;
       result.insert(std::pair<std::string,std::pair<std::pair<std::string,float>,std::pair<std::string,float> > >(name, std::pair<std::pair<std::string,float>,std::pair<std::string,float> >()));

       //We get the best value for it's own family
       result[name].first = minDistanceMinHashProts(trainSet[family],(*itMember).second);

       //We get the best value in other family
       std::pair<std::string,float>  tmpResult;
       for(itTrain = trainSet.begin(); itTrain != trainSet.end(); itTrain++){
         if((*itTrain).first != family){
           tmpResult = minDistanceMinHashProts((*itTrain).second,(*itMember).second);

           //if it's our first turn
           if(itTrain == trainSet.begin()){
             result[name].second = tmpResult;
           }

           //if it's a better solution
           else if(tmpResult.second > result[name].second.second){
             result[name].second = tmpResult;
           }
         }
       }
     }
   }
   return result;
}


float jaccardSim(std::set<std::uint32_t> setA, std::set<std::uint32_t> setB){
  /**
   * Return the jaccard similarity between two set
   * SetA : the first set to compare
   * SetB : the second set to compare
   */

  //float similarity; //Our end result
  int sizeSetA = setA.size();

  // We merge the two set into one global set
  for(std::set<std::uint32_t>::iterator it = setB.begin(); it != setB.end();it++){
    setA.insert((*it));
  }

  return (float(sizeSetA) + float(setB.size()) - float(setA.size())) /  float(setA.size());

}
