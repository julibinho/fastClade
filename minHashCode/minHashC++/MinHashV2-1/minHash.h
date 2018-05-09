#ifndef MINHASH_H
#define MINHASH_H

std::vector<long> minHashSignature(std::set<std::uint32_t>, std::vector<int>, std::vector<int>);
std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > minHashSignatures(std::unordered_map<std::string,std::unordered_map<std::string,std::set<std::uint32_t> > >,std::vector<int>,std::vector<int>);
float minHashDistance(std::vector<long>, std::vector<long>);

#endif
