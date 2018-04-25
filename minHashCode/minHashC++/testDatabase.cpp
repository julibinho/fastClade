#include<iostream> //To write in the console
#include<unordered_map>
#include<set>
#include<vector>
#include<cstdint> //Unsigned integers
#include<fstream>

#include"auxiliary.h"
#include"shingles.h"
#include"minHash.h"
#include"distribution.h"

int TAILLE_SHINGLES;
int NOMBRES_HASH;
float THRESHOLD;

int main(int argc, char* argv[]){

	if(argc != 4){
		std::cout<<"please use 3 arguments : the path to the database, the path to the test directory and the path to write the result" << std::endl;
	}

	//We get the database under a usable data structure
	std::string dbPath = argv[1];
	std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > db = readDatabase(dbPath);

	//Get the parameters of the database
	std::ifstream myDb(dbPath);
	std::string line;

	if(myDb.is_open()){

		//The first line is the length of shingles
		std::getline(myDb,line);
		TAILLE_SHINGLES = std::stoi(line);

		//The second line is the number of hashes
		std::getline(myDb,line);
		NOMBRES_HASH = std::stoi(line);

		std::vector<int> coeffA;
		std::vector<int> coeffB;

		//We get the third line that correspond to coeffA
		std::vector<std::string> splitLine;
		std::getline(myDb,line);
		splitLine = splitString(line,"\t");

		for(int i = 0;i<splitLine.size();i++){
			coeffA.push_back(std::stoi(splitLine[i]));
		}

		//We get the fourth line that correspond to coeffB
		std::getline(myDb,line);
		splitLine = splitString(line,"\t");

		for(int i = 0;i<splitLine.size();i++){
			coeffB.push_back(std::stoi(splitLine[i]));
		}

		//We get the tests proteins under usable data structure
		std::unordered_map<std::string, std::unordered_map<std::string,std::set<std::uint32_t> > > testProteinsShingles = shingling_documents(argv[2]);
		std::unordered_map<std::string, std::unordered_map<std::string,std::vector<long> > > testProteins = minHashSignatures(testProteinsShingles,coeffA,coeffB);

		writeResultDistribution(getDistributionsMinHash(db,testProteins),argv[3]);

	}
	else{
		std::cout << "not a readable database file" << std::endl;
	}

}
