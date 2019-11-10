#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>

#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher(const std::string& key)
{
  setKey(key);
}

void PlayfairCipher::setKey(const std::string& key){

  cipherKey_ = key;

  cipherKey_ += "abcdefghijklmnopqrstuvwxyz";

  std::transform(cipherKey_.begin(), cipherKey_.end(), cipherKey_.begin(), ::toupper);

  auto notAlphaIter = std::remove_if(cipherKey_.begin(), cipherKey_.end(), [] (char c){return !std::isalpha(c);});

  cipherKey_.erase(notAlphaIter, cipherKey_.end());
  
  std::transform(cipherKey_.begin(), cipherKey_.end(), cipherKey_.begin(), swapJForI);

  std::string foundChars{""};

  auto isRepeated = [&foundChars](char c){
    if(foundChars.find(c) != std::string::npos) {
      return true;
    }
    foundChars += c;
    return false;
  };

  auto repeatedCharIter = std::remove_if(cipherKey_.begin(), cipherKey_.end(), isRepeated);
  
  cipherKey_.erase(repeatedCharIter, cipherKey_.end());

  unsigned int column{0}, row{0};
  for(size_t i{0}; i < cipherKey_.size(); ++i){
    row = i / dimension_;
    column = i % dimension_;

    charToPosMap_.insert({cipherKey_[i], {column, row}});
    posToCharMap_.insert({{column, row}, cipherKey_[i]});
  }

  return;

}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode) {

  std::cout << "INPUT TEXT: " << inputText << std::endl;

  std::string swappedIJString{inputText};
  std::transform(swappedIJString.begin(), swappedIJString.end(), swappedIJString.begin(), swapJForI);

  std::string cipherString{""};
  for(std::string::const_iterator iter{swappedIJString.begin()}; iter != swappedIJString.end(); ++iter) {

    // To avoid dereferencing beyond the string
    if(iter == (swappedIJString.end()-1)) {
      cipherString += *iter;
      break;
    }

    if(*iter == *(iter+1)){
      cipherString += *iter;
      (*iter == 'X') ? cipherString += 'Q': cipherString += 'X';   
    } else {
      cipherString += *iter; 
      cipherString += *(iter+1);
      ++iter;
    }
  }

  if((cipherString.size() % 2) == 1) cipherString += 'Z';

  std::string encryptedString{""};

  for(std::string::const_iterator iter{cipherString.begin()}; iter != cipherString.end(); iter+=2){

    std::vector<unsigned int> pos1 = charToPosMap_.find(*iter)->second;
    std::vector<unsigned int> pos2 = charToPosMap_.find(*(iter+1))->second;
    
    std::vector<unsigned int> newPos1;
    std::vector<unsigned int> newPos2;

    if(cipherMode == CipherMode::Encrypt){
      if(pos1[1] == pos2[1]) {
        newPos1 = {(pos1[0] + 1) % dimension_, pos1[1]};
        newPos2 = {(pos2[0] + 1) % dimension_, pos2[1]};
      } else if(pos1[0] == pos2[0]) {
        newPos1 = {pos1[0], (pos1[1] + 1) % dimension_};
        newPos2 = {pos2[0], (pos2[1] + 1) % dimension_};
      } else {
        newPos1 = {pos2[0], pos1[1]};
        newPos2 = {pos1[0], pos2[1]};
      }
    } else {
      if(pos1[1] == pos2[1]) {
        newPos1 = {(pos1[0] - 1 + dimension_) % dimension_, pos1[1]};
        newPos2 = {(pos2[0] - 1 + dimension_) % dimension_, pos2[1]};
      } else if(pos1[0] == pos2[0]) {
        newPos1 = {pos1[0], (pos1[1] - 1 + dimension_) % dimension_};
        newPos2 = {pos2[0], (pos2[1] - 1 + dimension_) % dimension_};
      } else {
        newPos1 = {pos2[0], pos1[1]};
        newPos2 = {pos1[0], pos2[1]};
      }

    }

    encryptedString += posToCharMap_.find(newPos1)->second;
    encryptedString += posToCharMap_.find(newPos2)->second;

  }

  if(cipherMode == CipherMode::Decrypt){
    std::cout << "DECRYPTED STRING: " << encryptedString << std::endl;
  } else {
    std::cout << "ENCRYPTED STRING: " << encryptedString << std::endl;
  }

  return "";

}

char PlayfairCipher::swapJForI(char letter) {

  if(letter == 'J') return 'I';
  return letter;

}
