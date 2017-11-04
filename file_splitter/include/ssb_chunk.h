/*****************************************************************************************************/
/*                                                                                                   */
/* Application        : This application is intended to provide secure box for putting up your stuff.*/
/*                The basic idea behind this app is to split the file in multiple chuncks and        */
/*                encrypt each chunk with a key derived from the key of the previous.                */
/* Module           : SsbChunk                                                                       */
/* Description      : This is a file representative                                                  */
/* Input            : File path                                                                      */
/* Output           : Add file to or retrive file from pool of chunks                                */
/* Date             : 24 August 2014                                                                 */
/* Author           : Pratik Patil                                                                   */
/* License      :                                                                                    */
/*               Secure Storeage Box Version 1.0.0                                                   */
/*               Copyright (C) 2017  Pratik Patil                                                    */
/*                                                                                                   */
/*               This program is free software: you can redistribute it and/or modify                */
/*               it under the terms of the GNU General Public License Version 3 as published by      */
/*               the Free Software Foundation.                                                       */
/*                                                                                                   */
/*               This program is distributed in the hope that it will be useful,                     */
/*               but WITHOUT ANY WARRANTY; without even the implied warranty of                      */
/*               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       */
/*               GNU General Public License for more details.                                        */
/*                                                                                                   */
/*               You should have received a copy of the GNU General Public License                   */
/*               along with this program.  If not, see <http://www.gnu.org/licenses/>.               */
/*****************************************************************************************************/

#ifndef SSB_CHUNK_H
#define SSB_CHUNK_H

#include <fstream>      // std::fstream
#include <cstdio>       // std::remove
#include <sys/stat.h>   // std::stat
#include <iomanip>        // setw
#include "cryptopp_wrapper/cryptopp_wrapper.h"
#include "key_mutator/key.h"
#include "ssb_error.h"
namespace SSB
{
  /*

   Errors :
   open falied                         SSB_OPEN_ERR
   r/w failed                          SSB_IO_ERR
   cryptographic operations failed     SSB_CRYPT_ERR
   split failed                        SSB_SPLIT_ERR
   removal failed                      SSB_DEL_ERR
   empty object                        SSB_EMPTY_OBJ
   a bit value will be defined for above errors. _status will have bits assosiacted with each error.


   #define SSB_OPEN_ERR    0x00000001
   #define SSB_IO_ERR      0x00000010
   #define SSB_CRYPT_ERR   0x00000100
   #define SSB_SPLIT_ERR   0x00001000
   #define SSB_DEL_ERR     0x00010000
   #define SSB_EMPTY_OBJ   0x00100000
   #ifndef SSB_SPLIT_FILE
   #define SSB_SPLIT_FILE  0x01000000
   #endif
   #define SSB_CLOSE_ERR   0x10000000
   */

///
/// \brief This class will represent a chunk of the file. 
///
  class SsbChunk
  {
///
/// \class ssb_chunk
/// As the complete file is going to be divided in number of small files, each section of it will be represented by this class.
/// This class provides methods to encrypt or decrypt the chunk read from the pool.
/// Object of this class can be created by reading from the section of the original file, that is to be split and encrypted, 
/// or it by reading chunk file from the pool (in case of reconstruction of the file).
/// \endcode

  public:
    ///
    /// \brief This it the default constructor
    ///
    SsbChunk ();
    ///
    /// \brief create chunk from the provided chunk name. This constructor is used while retrieving a file from the pool.
    ///
    SsbChunk (std::string& chunk_name);
    ///
    /// \brief Create a chunk from the section of original file. This is used while adding the file to the pool. For this the section is till eof
    ///
    SsbChunk (std::fstream& file_stream);
    ///
    /// \brief Create a chunk from the section of original file. This is used while adding the file to the pool.
    ///
    SsbChunk (std::fstream& file_stream, size_t& chunk_size); // Initializes all members
    ///
    /// \brief Create a chunk from the section of original file. This is used while adding the file to the pool.
    ///
    SsbChunk (std::fstream& file_stream, size_t& chunk_size, KEY); // Initializes all members
    ///
    /// \brief Opens the chunk file for io
    ///
    bool
    open ();
    ///
    /// \brief closed the chunk object
    ///
    bool
    close ();
    ///
    /// \brief Creates a chunk from the file_stream, valid call only if file_stream and chunk_size was provided before
    ///
    bool
    create (KEY&);
    ///
    /// \brief Appends the chunk data to the file_stream. This is used while stitching back the chunks. It returns the next seed to be used for Key generation. returns 0 in case of error.
    ///
    int
    mergeTo (std::ofstream& file_stream, KEY&);
    ///
    /// \brief Creates a chunk from current to end of the file
    ///
    bool
    create (std::fstream& file_stream, KEY);
    ///
    /// \brief Creates a chunk, valid only if parameters are not provided before
    ///
    bool
    create (std::fstream& file_stream, size_t& chunk_size, KEY);
    std::ifstream
    read (); // reads data from the file ??? do we need it public????
    std::ofstream
    write (); // writes data from the file ??? do we need it public????
    ///
    /// \brief Deletes the chunk.
    /// \todo Implement removal of the chunks
    ///
    ///
    /// This method removes the chunk from the pool. Befor removal, it read the next chunk in the sequence. None of the other infomration is read.
    ///
    bool
    remove ();
    ///
    /// \brief  Encrypts the chunk data.
    ///
    std::string
    encrypt (const std::string& buffer, KEY&);
    ///
    /// \brief Decrypts the chunk data
    ///
    std::string
    decrypt (std::string& buffer, KEY&);
    ///
    /// \brief Returns the name of current chunk.
    ///
    std::string
    chunkName ();
    ///
    /// \brief Returns index of the next chunk if available, 0 otherwise
    ///
    std::string
    nextChunkName ();
    ///
    /// \brief It links the provided chunk to the current chunk
    ///
    bool
    linkNextChunk (std::string& next_index);

    ///
    /// \brief Returns true if _status is 0
    ///
    bool
    good ();
    ///
    /// \brief Returns error code. if all good, returns 0.
    ///
    int
    error ();
    ///
    /// \brief Returns true if open fails
    ///
    bool
    open_err ();
    ///
    /// \brief Returns true if io fails
    ///
    bool
    ioErr ();
    ///
    /// \brief Returns true if cryptographic operations fails
    ///
    bool
    cryptErr ();
    //bool split_err();                                       // returns true if file cannot be split
    ///
    /// \brief Returns true if file cannot be removed
    ///
    bool
    removeErr ();
    //bool is_split();                                        // returns true if file is split
    ///
    /// \brief Returns file size in bytes
    ///
    size_t
    size ();
    ~SsbChunk ();
  protected:

  private:
    ///
    /// \brief Adds the current chunk id in pool
    ///
    int
    addInPool ();
    ///
    /// \brief Gives a unique and random chunk name
    ///
    std::string
    getChunkName ();
    ///
    /// \brief Returns true only if there is no other chunk of the same name.
    ///
    bool
    isUniqueChunk (std::string&);
    ///
    /// \brief Padds the pc to the data to make its size multiple of 16
    ///
    bool
    dataPadding (std::string& data, const char pc);
    ///
    /// \brief Stores the name of the file
    ///
    std::string _chunkName;
    ///
    /// \brief Its the index of the node in file chunk sequence
    ///
    int _seqNo;
    ///
    /// \brief Its the index of the next node in pool
    ///
    std::string _nextIndex;
    ///
    /// \brief Marks consistency status of the file. True only if all operations were successful
    ///
    int _status;
    ///
    /// \brief Stores the size of the chunk
    ///
    size_t _chunkSize;
    ///
    /// \brief fstream object for file operations on chunk
    ///
    std::fstream _chunkStream;
    ///
    /// \brief Object to store parent file
    ///
    std::fstream _fileStream;
    ///
    /// \brief Key for aes encryption.
    ///
    byte _aesKey[SSB_DEFAULT_KEYLENGTH+8];
  ///
  /// \brief iv for aes encryption.
  ///
  byte _aesIv[CryptoPP::AES::BLOCKSIZE + 8];
};
}
#endif
