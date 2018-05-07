#ifndef COMPARISONS_H
#define COMPARISONS_H

std::unordered_map<std::string,std::string> calculateAllComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> >, std::unordered_map<std::string, std::vector<long> >);
std::pair<std::string,std::string> calculateComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> >, std::pair<std::string, std::vector<long> >);
std::unordered_map<std::string, std::unordered_map<std::string, float> > calculateComparisonsAllvsAll (std::unordered_map<std::string, std::vector<int> >);


#endif
