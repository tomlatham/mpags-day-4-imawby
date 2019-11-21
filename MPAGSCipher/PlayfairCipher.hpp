#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include <string>
#include <map>

#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {

  public:

  /**
   * Create a new PlayfairCipher with the given key
   *
   * \param key the key to use in the cipher
   */
  explicit PlayfairCipher(const std::string& key);

  /**
   * Set the key to be used for the encryption/decryption
   *
   * \param key the key to use in the cipher
   */
  void setKey(const std::string& key);

  /**
   * Apply the cipher to the provided text
   *
   * \param inputText the text to encrypt or decrypt
   * \param cipherMode whether to encrypt or decrypt the input text
   * \return the result of applying the cipher to the input text
   */
  std::string applyCipher(const std::string& inputText, const CipherMode cipherMode) const;


  private:

  /// The cipher key
  std::string cipherKey_;

  /// The grid dimension
  const unsigned int dimension_ = 5;

  /**
   * Helper function to swap any 'J' characters to 'I'
   *
   * \param letter the letter from the input text
   * \return the original letter or 'I' if the original was 'J'
   */
  static char swapJForI(char letter);

  // Lookup tables generated from the key

  /// Type definition for the coordinates in the grid
  using PlayfairCoord = std::vector<unsigned int>;

  /// Lookup table to go from the character to the coordinates
  std::map<const char, PlayfairCoord> charToPosMap_;

  /// Lookup table to go from the coordinates to the character
  std::map<PlayfairCoord, const char> posToCharMap_;

};

#endif
