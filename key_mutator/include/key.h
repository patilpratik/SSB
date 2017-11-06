/****************************************************************************************************/
/*                                                                                                  */
/* Application  : This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : key mutator                                                                       */
/* Description  : This module manipulates the key in irriversible way.                              */
/*                i.e. given mutated key and seed, original key could not be derived                */
/* Input        :   1) key                                                                          */
/*                  2) seed : number that controls the mutation.(in case of decryption)             */
/*                  3) num : total number of keys to make                                           */
/* Output       : mutated key                                                                       */
/* Date         : 3 October 2014                                                                    */
/* Author       : Pratik Patil                                                                      */
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

#ifndef KEY_H
#define KEY_H

#include <map>
#include <sstream>        // std::istringstream
#include "crypto++/modes.h" // byte
///
/// \todo move this template StrToNum to utility header.
///
template<class T>
  T
  StrToNum (const std::string& str)
  {
    T num;
    std::istringstream buffer (str);
    buffer >> num;
    return num;
  }
template<typename T>
  std::string
  NumToStr (T Number)
  {
    std::ostringstream ss;
    ss << Number;
    return ss.str ();
  }
//class KEY;
//#include "cryptopp_wrapper/cryptopp_wrapper.h" // byte
namespace SSB
{

///
/// \brief This class is to hold the key values.
///
/// This class simplifies the use of key.
///

  class KEY
  {

  public:
    ///
    /// \brief This constructor constructs an empty key
    ///
    KEY ();
    ///
    /// \brief This constructs a key
    ///
    KEY (byte* /*val*/, size_t /*_keySize*/);
    ///
    /// \brief This constructs a key
    ///
    KEY (std::string /*val*/, size_t /*_keySize*/);
    ///
    /// \brief Copy constructor
    ///
    KEY (const KEY& /*key*/);
    ///
    /// \brief This method converts key from byte to string. This method simpifies the use and eliminates the buffer overflow
    ///
    std::string
    value () const;
    ~KEY ();
    ///
    /// \brief This method will return key value in byte
    const byte*
    bvalue () const;
    ///
    /// \brief Returns the size
    ///
    size_t
    size () const;
    ///
    /// \brief Returns the seed
    ///
    const int
    seed () const;
    ///
    /// \brief Sets seed
    ///
    bool
    setSeed (int &);
    ///
    /// \brief This clears the key object
    ///
    bool
    clear ();
    ///
    /// \brief Overloading assignment opperator
    ///
    void
    operator= (const KEY& obj);
  protected:

  private:
    size_t _keySize;
    byte * _key;
// try
    int _seed;
//\\ try
  };
}
#endif
