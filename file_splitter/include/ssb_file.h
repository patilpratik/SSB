/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: SsbFile                                                                           */
/* Description	: This is a file representative                                                     */
/* Input    	: File path                                                                         */
/* Output   	: Add file to or retrive file from pool of chunks                                   */
/* Date	    	: 24 August 2014                                                                    */
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

#ifndef SsbFile_H
#define SsbFile_H

#include <fstream>      // std::fstream
#include <cstdio>       // std::remove
#include <sys/stat.h>   // std::stat
#include "cryptopp_wrapper/cryptopp_wrapper.h"
#include "key_mutator/key.h"
#include "key_mutator/key_mutator.h"
#include "ssb_error.h"
#include "ssb_chunk.h"

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
   #define SSB_POOL_ERR    0x01000000
   #ifndef SSB_SPLIT_FILE
   #define SSB_SPLIT_FILE  0x10000000
   #endif
   */
///
/// \brief This class represents a file
///
/// For the purpose of encrypting and decrypting file, this class is to be used.
/// This class provides methods to divide file into number of chunks and add them to
/// the pool. It also provides method to read chunks and retrive original file
  class SsbFile
  {

  public:
    SsbFile ();                                      // Initialises all members.
    SsbFile (const std::string&);

    bool
    open ();                                            // Opens the file.
    bool
    open (const std::string&);
    bool
    close ();                                   // close the file descriptor
    std::ifstream
    read (); // reads data from the file ??? do we need it public????
    std::ofstream
    write (); // writes data from the file ??? do we need it public????
    ///
    /// \brief splits the file into number of chunks. Splitting starts from the end of the file to start.
    ///
    bool
    split (const int&);
    ///
    /// \brief splits the file into number of chunks. Splitting starts from the end of the file to start.
    ///
    bool
    split (const int& N, std::string& pwd);
    ///
    /// \brief Checks if file is already added in the pool. returns false if not
    ///
    bool
    lookup (const std::string&);
    ///
    /// \brief Solves the puzzel and gives you complete file
    ///
    bool
    stitch (std::string& pwd);
    bool
    stitch ();
    ///
    /// \brief  Removes the file from SSB pool.
    bool
    remove (std::string& pwd);
    bool
    remove ();
    ///
    /// \brief encrypts the file.
    ///
    bool
    encrypt ();
    ///
    /// \brief decrypts the file
    ///
    bool
    decrypt ();
    ///
    /// \brief returns true if _status is 0
    ///
    bool
    good ();
    ///
    /// \brief returns error code. if all good, returns 0.
    ///
    int
    error ();
    ///
    /// \brief returns true if open fails
    ///
    bool
    openErr ();
    ///
    /// \brief returns true if io fails
    ///
    bool
    ioErr ();
    ///
    /// \brief returns true if cryptographic operations fails
    ///
    bool
    cryptErr ();
    ///
    /// \brief returns true if file cannot be split
    ///
    bool
    splitErr ();
    ///
    /// \brief returns true if file cannot be removed
    ///
    bool
    removeErr ();
    ///
    /// \brief returns true if file is split
    ///
    bool
    isSplit ();
    ///
    /// \brief returns file size in bytes
    ///
    size_t
    size ();

  protected:

  private:
    ///
    /// \brief adds this file in pool
    ///
    bool
    addFileInPool (const KEY&);
    ///
    /// \brief looks for the chunk id in pool index
    /// \todo check if this implementation is needed. if yes, implement.
    int
    searchPoolIndex (const int&);
    ///
    /// \brief seeks the start address of the chunk
    /// It calculates the start address from
    void
    adjustFileStream (unsigned int& chunk_num, size_t & chunk_size);
    ///
    /// \brief Returns acceptable value of n
    ///
    int
    correctNumOfChunks (const size_t&, const int&);
    ///
    /// \brief check if the given N is accpeptable or not
    ///
    bool
    valildateNumOfChunks (const size_t&, const int&);
    ///
    /// \brief calulates maximum value of N acceptable for file
    ///
    int
    calMaxNumOfChunks (const size_t& file_size);
    ///
    /// \brief Stores the name of the file
    ///
    std::string _fileName;
    ///
    /// \brief Its the index of the head node in the pool
    ///
    std::string _head;
    ///
    /// \brief Marks consistancy status of the file. True only if all operations were successfull
    ///
    int _status;
    ///
    /// \brief Initialised to number of total sections, decreamented on each chunk creation
    ///
    int _chunkCount;
    ///
    /// \brief fstream object for file operations
    ///
    std::fstream _fileStream;
    ///
    /// \brief This method gets the absolute path of the file. This wrapps the realpath() function
    ///
    std::string
    getAbsPath (std::string);
    ///
    /// \brief This method reads password from the command prompt.
    ///
    std::string
    readPassword ();
    ///
    ///
    ///
    bool
    removeChunks (SsbChunk& head_node);
  };
}	// end of SSB
#endif
