std::unordered_map<std::string,std::string> calculateAllComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> > signaturesTrain, std::unordered_map<std::string, std::vector<long> > signaturesTest){
  std::unordered_map<std::string, std::vector<long> >::iterator itTest = signaturesTest.begin();
  std::unordered_map<std::string,std::string> result;
  for(itTest = signaturesTest.begin(); itTest!=signaturesTest.end(); itTest++){
    result.insert(calculateComparisonsOnevsAll(signaturesTrain, (*itTest)));
  }

  return result;
}

std::pair<std::string,std::string> calculateComparisonsOnevsAll(std::unordered_map<std::string, std::vector<long> > signaturesTrain, std::pair<std::string, std::vector<long> > signatureToTest){
  /***
  * Return the first sequence name that have a great ressemblance with the one to test.
  *
  * signaturesTrain : The map of all the sequence (with their signatures) that you want to compare to your sequence
  *
  * signatureToTest : the signature of the sequence you want to compare to other
  */

  std::unordered_map<std::string, std::vector<long> >::iterator itTrain = signaturesTrain.begin();
  //std::unordered_map<std::string, std::vector<int> >::iterator itTest = signatureToTest.begin();
  float similarity = 0.0; //The buffer for the similarity result with each comparison.
  //ADDED
  float bestSimilarity = similarity; //The buffer to see the value of the best foud similarity
  std::string bestResponse = ""; //The buffer for the best response name
  // NOT ADDED
  std::string resultName = ""; //The name of the winner
  float count = 0.0; //The counter of same signature
  std::vector<long> signatureTestSequence = signatureToTest.second; //The signature of your tested sequence
  std::vector<long> signature2; //The signature of the next sequence to compare to your test sequence

  while( (itTrain != signaturesTrain.end()) && (similarity < THRESHOLD)){
    count = 0.0;
    signature2 = (*itTrain).second;
    resultName = (*itTrain).first;
    for (int i = 0; i < NOMBRES_HASH; i++){
      if(signature2[i] == signatureTestSequence[i]){
        count++;
      }
    }
    similarity = count/float(NOMBRES_HASH);
    //ADDED
    if (similarity > bestSimilarity){
      bestSimilarity = similarity;
      bestResponse = resultName;
    }
    //NOT ADDED
    itTrain++;
  }

  if(itTrain == signaturesTrain.end()){
    resultName = "";
  }
  std::cout << " Best similarity for "<< signatureToTest.first << ": " << bestSimilarity << " Wich correspond to " << bestResponse <<std::endl;
  return std::pair<std::string,std::string>(signatureToTest.first,resultName);
}

std::unordered_map<std::string, std::unordered_map<std::string, float> > calculateComparisonsAllvsAll (std::unordered_map<std::string, std::vector<std::uint32_t> > signatures){
  /***
  * Retturn the matrix of an all to all comparison using minHash signatures of sequences.
  *
  * signatures : The map of all sequence with their minHash signatures
  */

  std::unordered_map<std::string, std::unordered_map<std::string, float> > comparisons; //Our end result
  std::unordered_map<std::string, float> comparisonTmp;
  std::vector<std::uint32_t> signature1; //The first shingle of the comparison
  std::vector<std::uint32_t> signature2; //The second shingle of the comparison
  float count = 0.0; //the counter of equivalent minHash signatures.
  std::unordered_map<std::string, std::vector<std::uint32_t> >::iterator it1 = signatures.begin();
  std::unordered_map<std::string, std::vector<std::uint32_t> >::iterator it2;

  for (int i = 0; i < signatures.size()-1; i++){
    signature1 = (*it1).second;
    it2 = it1;
    it2++;

    for (int j = 0; j < signatures.size() - i - 1; j++){

      signature2 = (*it2).second;

      for(int k = 0; k<NOMBRES_HASH;k++){
        if(signature1[k] == signature2[k]){
          count++;
        }
      }

      comparisonTmp.insert(std::pair<std::string, float>((*it2).first, count/float(NOMBRES_HASH)));
      it2++;
      count = 0;
    }

    comparisons.insert(std::pair<std::string, std::unordered_map<std::string, float> >((*it1).first, comparisonTmp));
    comparisonTmp.clear();
    it1++;
  }

  return comparisons;
}
