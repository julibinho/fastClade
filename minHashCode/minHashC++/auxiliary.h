
#ifndef AUXILIARY_H
#define AUXILIARY_H

std::vector<std::string> liste_fichiers_dossier(std::string);
std::vector<std::string> splitString(std::string, std::string);
std::pair<std::vector<int>,std::vector<int> > pickRandomCoeffs(int);
int writeDatabase(std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > >,std::vector<int>, std::vector<int>, std::string);
std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > readDatabase(std::string);
int writeResultDistribution(std::unordered_map<std::string,std::pair<std::pair<std::string,float>,std::pair<std::string,float> > >,std::string);


#endif
