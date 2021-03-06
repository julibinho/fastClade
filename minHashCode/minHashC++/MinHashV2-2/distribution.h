#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

float minDistanceJaccardProts(std::unordered_map<std::string, std::set<std::uint32_t> >, std::set<std::uint32_t>);
std::pair<std::string,float> minDistanceMinHashProts(std::unordered_map<std::string, std::vector<long> >, std::vector<long>);
std::vector<std::pair<float,float> > getDistributions(std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > >, std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > >);
std::unordered_map<std::string,std::pair<std::string,std::string> > getDistributionsMinHash(std::unordered_map<std::string, std::unordered_map<std::string, std::vector<long> > > trainSet, std::unordered_map<std::string, std::vector<long> > testSet);
float jaccardSim(std::set<std::uint32_t>, std::set<std::uint32_t>);


#endif
