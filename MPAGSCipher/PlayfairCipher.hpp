#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include <string>
#include <map>

#include "CipherMode.hpp"

class PlayfairCipher {

  public:

  explicit PlayfairCipher(const std::string& key);

  void setKey(const std::string& key);

  std::string applyCipher(const std::string& inputText, const CipherMode cipherMode);


  private:

  std::string cipherKey_;
  const unsigned int dimension_ = 5;

  static char swapJForI(char letter);

  std::map<const char, std::vector<unsigned int>> charToPosMap_;
  std::map<std::vector<unsigned int>, const char> posToCharMap_;

  std::map<const char, std::vector<unsigned int>> encryptedCharToPosMap_;
  std::map<std::vector<unsigned int>, const char> encryptedPosToCharMap_;

};

#endif
