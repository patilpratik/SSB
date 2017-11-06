/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: Wrapper class for cryptopp                                                        */
/* Description	: simplifies the used of cryptopp by encapsulating things                           */
/* Input    	:   1) Plantext Data buffer                                                         */
/*                  2) Algorithm selector                                                           */
/*                  3) Encrypt/Decrypt marker                                                       */
/* Output   	:                                                                                   */
/* Date	    	: 06 September 2014                                                                 */
/* Author   	: Pratik Patil                                                                      */
/* License      :                                                                                   */
/*               Secure Storeage Box Version 1.0.0                                                  */
/*               Copyright (C) 2017  Pratik Patil                                                   */
/*                                                                                                  */
/*               This program is free software: you can redistribute it and/or modify               */
/*               it under the terms of the GNU General Public License Version 3 as published by     */
/*               the Free Software Foundation.                                                      */
/*                                                                                                  */
/*               This program is distributed in the hope that it will be useful,                    */
/*               but WITHOUT ANY WARRANTY; without even the implied warranty of                     */
/*               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      */
/*               GNU General Public License for more details.                                       */
/*                                                                                                  */
/*               You should have received a copy of the GNU General Public License                  */
/*               along with this program.  If not, see <http://www.gnu.org/licenses/>.              */
/****************************************************************************************************/

#ifndef CRYPTOPP_WRAPPER_H
#define CRYPTOPP_WRAPPER_H
#include <iomanip>
#include <iostream>

#include "crypto++/modes.h"
#include "crypto++/aes.h"
#include "crypto++/filters.h"
#include "crypto++/hex.h"
#include "crypto++/sha.h"

#include "key_mutator/key.h"
namespace SSB
{
#define SSB_DEFAULT_KEYLENGTH CryptoPP::AES::DEFAULT_KEYLENGTH

/// 
/// \brief This class simplifies the use of crypto++ library. 
///
/// It is a wrapper class that does all the needed processing for encrypting and decrypting with selected algorithm
/// Currently only AES algorithm is supported in this class.
/// \todo Add remaining cryptographic algorithms.

  class CryptoPPWrapper
  {

  public:
    ///
    /// \brief encrypt plaintext using aes encryption and given key
    ///
    std::string
    aesEncrypt (const std::string plaintext, const KEY& key,
                byte iv[CryptoPP::AES::BLOCKSIZE]);
    ///
    /// \brief decrypt ciphertext using aes algorithm and given key
    ///
    std::string
    aesDecrypt (const std::string ciphertext, const KEY& key,
                byte iv[CryptoPP::AES::BLOCKSIZE]);
    ///
    /// \brief This method creates sha hash of the given length
    ///
    std::string
    sha (const std::string&, const size_t& size);
  protected:
  private:
    // key size
    // iv size
    // cbc mode
  };
}
#endif
