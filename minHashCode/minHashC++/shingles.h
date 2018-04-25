
#ifndef SHINGLES_H
#define SHINGLES_H


std::unordered_map<std::string,std::unordered_map<std::string,std::set<std::uint32_t> > > shingling_documents(std::string,bool permutations = false,std::unordered_map<std::string, std::unordered_map<std::string, int> > substMat = std::unordered_map<std::string, std::unordered_map<std::string, int> >());
std::set<std::uint32_t> shingling_a_document(std::string);
//std::set<std::uint32_t> shingling_a_document(std::string,std::unordered_map<std::string, std::unordered_map<std::string, int> >);
std::unordered_map<std::string,std::set<std::uint32_t> > shingling_long_document(std::string);
//std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::uint32_t> > > getProtByFamily(std::string);


#endif
