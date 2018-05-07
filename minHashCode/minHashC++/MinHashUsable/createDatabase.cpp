#include<iostream> //To write in the console
#include<unordered_map>
#include<set>
#include<vector>
#include<cstdint> //Unsigned integers

#include"auxiliary.h"
#include"shingles.h"
#include"minHash.h"

int TAILLE_SHINGLES;
int NOMBRES_HASH;

int main(int argc, char* argv[]){
	if(argc != 5){
		std::cout << "please use 4 arguments : The path to the directory, the shingles length, the number of hash and the path to the out file" << std::endl;
		return 0;
	}

	TAILLE_SHINGLES = std::stoi(argv[2]);
	NOMBRES_HASH = std::stoi(argv[3]);

	std::unordered_map<std::string,std::unordered_map<std::string,std::set<std::uint32_t> > > data = shingling_documents(argv[1]);
	std::pair<std::vector<int>,std::vector<int> > coeffs = pickRandomCoeffs(NOMBRES_HASH);
	std::vector<int> coeffA = coeffs.first;
	std::vector<int> coeffB = coeffs.second;
	std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > dataMinHash = minHashSignatures(data,coeffA,coeffB);
	writeDatabase(dataMinHash,coeffA,coeffB,argv[4]);


}
