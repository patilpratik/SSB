/****************************************************************************************************/
/*                                                                                                  */
/* Application  : This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : Wrapper class for cryptopp                                                        */
/* Description  : simplifies the used of cryptopp by encapsulating things                           */
/* Input        :   1) Plantext Data buffer                                                         */
/*                  2) Algorithm selector                                                           */
/*                  3) Encrypt/Decrypt marker                                                       */
/* Output       :                                                                                   */
/* Date         : 06 September 2014                                                                 */
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

#include <iostream>
#include <fstream>
#include "cryptopp_wrapper.h"

using namespace std;
namespace SSB
{
///
/// \brief This method accepts the plaintext encryption key and IV for AES encryption.
///
/// It uses CryptoPP::AES to encrypt the given plaintext and returns a string 
/// containing ciphertext.
/// None of the exeptions thrown by CryptoPP are handled in this class.
/// Calling object should handle exeptions.
/// \todo Check if the class can be made exeption safe.

  string
  CryptoPPWrapper::aesEncrypt (const string plaintext, const KEY& pkey,
                               byte iv[CryptoPP::AES::BLOCKSIZE])
  {

    //cout<<"aes encryption will be done"<<endl;
    KEY key (pkey);
    string ciphertext;
    CryptoPP::AES::Encryption aesEncryption (key.bvalue (), key.size ());
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption (aesEncryption,
                                                                 iv);
    CryptoPP::StreamTransformationFilter stfEncryptor (
        cbcEncryption, /*new CryptoPP::HexEncoder(*/
        new CryptoPP::StringSink (ciphertext));
    stfEncryptor.Put (
        reinterpret_cast<const unsigned char*> (plaintext.c_str ()),
        plaintext.length () + 1);
    stfEncryptor.MessageEnd ();

    return ciphertext;
  }

///
/// \brief This method accepts the ciphertext decryption key and IV for AES decryption.
///
/// It uses CryptoPP::AES to decrypt the given ciphertext and returns a string 
/// containing plaintext.
/// None of the exeptions thrown by CryptoPP are handled in this class. Calling object
/// should handle exeptions.
/// Care should be taken while passing ciphertext, as '\0' in ciphertext may cause incomplete data passing.
/// \todo Check if the class can be made exeption safe.

  string
  CryptoPPWrapper::aesDecrypt (const string ciphertext, const KEY& pkey,
                               byte iv[CryptoPP::AES::BLOCKSIZE])
  {

    //cout<<"aes decryption will be done"<<endl;
    KEY key (pkey);
    string decryptedtext;
    CryptoPP::AES::Decryption aesDecryption (key.bvalue (), key.size ());
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption (aesDecryption,
                                                                 iv);

    //cout<<"ciphertext size : "<<ciphertext.size()<<endl;
    //cout<<"ciphertext : "<<ciphertext<<endl;

#if 1
    CryptoPP::StreamTransformationFilter stfDecryptor (
        cbcDecryption,
        //new CryptoPP::HexDecoder(
        new CryptoPP::StringSink (decryptedtext)
        //)
            );
    stfDecryptor.Put (
        reinterpret_cast<const unsigned char*> (ciphertext.c_str ()),
        ciphertext.size ());
    stfDecryptor.MessageEnd ();
#else
    CryptoPP::StringSource(ciphertext, true,
        new CryptoPP::HexDecoder(
            new CryptoPP::StreamTransformationFilter( cbcDecryption,
                new CryptoPP::StringSink( decryptedtext ) ) ) );
#endif
    /*
     * CryptoPP::StringSource(ciphertext, true,
     *             new CryptoPP::StreamTransformationFilter( decryptor,
     *                             new CryptoPP::StringSink( decryptedtext )
     *   *                                      )
     */

    //cout<<"returning : " <<decryptedtext<<endl;
    return decryptedtext;
  }
///
/// This method will produce sha512 hash of the given data. and return first 'hash_size' hexadecimal char. If 'hash_size is greater than 128,
/// that is, total hex chars created by hash, it will return an empty string 
///

  string
  CryptoPPWrapper::sha (const string& data, const size_t& hash_size)
  {

    string value;
    CryptoPP::SHA512 hash;
    CryptoPP::StringSource filter (
        data,
        true /* PumpAll */,
        new CryptoPP::HashFilter (
            hash, new CryptoPP::HexEncoder (new CryptoPP::StringSink (value)) // HexEncoder
                )// HashFilter
                );// StringSource
    if (value.size () >= hash_size)
      value.erase (hash_size, std::string::npos);
    else
      value.clear ();
    return value;
  }
#if 0
int main(int argc, char** argv)
  {

    cout<<"Work in progess"<<endl;
    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    std::string plaintext = "abcdef\nphijkl\nmnopqr\nstuvwx\n123456\nab";
    std::string ciphertext;
    std::string decryptedtext;
    CryptoPPWrapper crypt_obj;
    KEY key_obj("BF98889D70675DA7",CryptoPP::AES::DEFAULT_KEYLENGTH);
    ciphertext = crypt_obj.aesEncrypt(plaintext.c_str(), key_obj,iv);

    fstream file1;
    file1.open("testfile.txt",ios::out | std::ios_base::binary);
    cout<<"written to the file : "<<endl<<ciphertext<<endl;
    file1.write(ciphertext.c_str(), ciphertext.size());
    file1.close();
    fstream file2;
    file2.open("testfile.txt",ios::in | std::ios_base::binary);
    ciphertext.clear();
    char * buffer = new char[513];
    file2.read(buffer,512);
    ciphertext.append(buffer, file2.gcount());
    delete buffer;
    file2.close();
    cout<<"read from the file : "<<endl<<ciphertext<<endl;
    cout<<"size of plaintext : "<<plaintext.size()<<endl;
    cout<<"size of ciphertext : "<<ciphertext.size()<<endl;

    decryptedtext = crypt_obj.aesDecrypt( ciphertext, key_obj, iv);
    cout<<"ciphertext = "<<ciphertext<<endl;
    cout<<"Plaintext = "<<plaintext<<endl;
    cout<<"Decryptedtext = "<<decryptedtext<<endl;
    return 0;
  }
#endif

#if 0

int main(int argc, char** argv)
  {

    string data("This is some data");
    CryptoPPWrapper crypt_obj;
    string hash_data = crypt_obj.sha(data,128);
    cout<<"hash is :"<<hash_data<<endl;
    return 0;
  }
#endif
}
