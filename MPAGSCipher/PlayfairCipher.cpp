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

  // Store the original key
  cipherKey_ = key;

  // Append the alphabet to the key
  cipherKey_ += "abcdefghijklmnopqrstuvwxyz";

  // Make sure the key is upper case
  std::transform(cipherKey_.begin(), cipherKey_.end(), cipherKey_.begin(), ::toupper);

  // Remove non-alphabet characters
  auto notAlphaIter = std::remove_if(cipherKey_.begin(), cipherKey_.end(), [] (char c){return !std::isalpha(c);});

  cipherKey_.erase(notAlphaIter, cipherKey_.end());
  
  // Change J -> I
  std::transform(cipherKey_.begin(), cipherKey_.end(), cipherKey_.begin(), swapJForI);

  // Remove duplicated letters
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

  // Store the coordinates of each letter
  unsigned int column{0}, row{0};
  for(size_t i{0}; i < cipherKey_.size(); ++i){
    row = i / dimension_;
    column = i % dimension_;

    charToPosMap_.insert({cipherKey_[i], {column, row}});
    posToCharMap_.insert({{column, row}, cipherKey_[i]});
  }

  return;

}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode) const {

  // Create the output string, initially a copy of the input text
  std::string swappedIJString{inputText};

  // Change J -> I
  std::transform(swappedIJString.begin(), swappedIJString.end(), swappedIJString.begin(), swapJForI);

  // Check for repeated characters (but only when they occur within a digraph)
  std::string cipherString{""};
  for(std::string::const_iterator iter{swappedIJString.begin()}; iter != swappedIJString.end(); ++iter) {

    // To avoid dereferencing beyond the string
    if(iter == (swappedIJString.end()-1)) {
      cipherString += *iter;
      break;
    }

    // Always add the first of the digraph
    cipherString += *iter;

    // If the two characters are the same then add an X (or a Q for repeated
    // X's) between them, otherwise just add the second character
    if(*iter == *(iter+1)){
      (*iter == 'X') ? cipherString += 'Q': cipherString += 'X';   
    } else {
      cipherString += *(iter+1);
      ++iter;
    }
  }

  // If we've ended up with an odd-length input, add Z to the end
  // (or X if the last character is a Z)
  if((cipherString.size() % 2) == 1) {
    cipherString += (cipherString[cipherString.size()-1] == 'Z') ? 'X' : 'Z';
  }

  // Loop over the input digraphs
  std::string encryptedString{""};

  for(std::string::const_iterator iter{cipherString.begin()}; iter != cipherString.end(); iter+=2){

    PlayfairCoord pos1 { charToPosMap_.find(*iter)->second };
    PlayfairCoord pos2 { charToPosMap_.find(*(iter+1))->second };
    
    PlayfairCoord newPos1;
    PlayfairCoord newPos2;

    if(cipherMode == CipherMode::Encrypt){
      if(pos1[1] == pos2[1]) {
        // Rows are the same, so increment the column indices (modulo the grid dimension)
        newPos1 = {(pos1[0] + 1) % dimension_, pos1[1]};
        newPos2 = {(pos2[0] + 1) % dimension_, pos2[1]};
      } else if(pos1[0] == pos2[0]) {
        // Columns are the same, so increment the row indices (modulo the grid dimension)
        newPos1 = {pos1[0], (pos1[1] + 1) % dimension_};
        newPos2 = {pos2[0], (pos2[1] + 1) % dimension_};
      } else {
        // Rectangle/Square - so keep the rows the same and swap the columns
        newPos1 = {pos2[0], pos1[1]};
        newPos2 = {pos1[0], pos2[1]};
      }
    } else {
      if(pos1[1] == pos2[1]) {
        // Rows are the same, so decrement the column indices (modulo the grid dimension)
        newPos1 = {(pos1[0] - 1 + dimension_) % dimension_, pos1[1]};
        newPos2 = {(pos2[0] - 1 + dimension_) % dimension_, pos2[1]};
      } else if(pos1[0] == pos2[0]) {
        // Columns are the same, so decrement the row indices (modulo the grid dimension)
        newPos1 = {pos1[0], (pos1[1] - 1 + dimension_) % dimension_};
        newPos2 = {pos2[0], (pos2[1] - 1 + dimension_) % dimension_};
      } else {
        // Rectangle/Square - so keep the rows the same and swap the columns
        newPos1 = {pos2[0], pos1[1]};
        newPos2 = {pos1[0], pos2[1]};
      }

    }

    // Find the letters associated with the new coords and append to the encrypted string
    encryptedString += posToCharMap_.find(newPos1)->second;
    encryptedString += posToCharMap_.find(newPos2)->second;

  }

  // Return the output text
  return encryptedString;
}

char PlayfairCipher::swapJForI(char letter) {

  if(letter == 'J') { return 'I'; }
  return letter;

}
