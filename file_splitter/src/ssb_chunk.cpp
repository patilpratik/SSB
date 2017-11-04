/****************************************************************************************************/
/*                                                                                                  */
/* Application  : This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : ssb_chunk                                                                         */
/* Description  : This represents a section of file                                                 */
/* Input        :   1)File path                                                                     */
/*                  2)Number of section to make                                                     */
/* Output       : Split file to or retrive file from the pool of chunk                              */
/* Date            : 29 August 2014                                                                 */
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
#include <cstdlib>

#include "ssb_chunk.h"
#include "ssb_setting.h"
#include "key_mutator/key.h"
#include "cryptopp_wrapper/cryptopp_wrapper.h"
using namespace std;
namespace SSB
{
/******************************************

 uses following macros to mark object status

 #define SSB_OPEN_ERR    0x00000001
 #define SSB_IO_ERR      0x00000010
 #define SSB_CRYPT_ERR   0x00000100
 #define SSB_SPLIT_ERR   0x00001000
 #define SSB_DEL_ERR     0x00010000
 #define SSB_EMPTY_OBJ   0x00100000

 #define SSB_SPLIT_FILE  0x01000000
 *******************************************************************/
/// \todo Create separate module for random number and character generater
/// so that these can be reused.
///
/// Number of char in name of the chunk
///
#define CHUNK_NAME_LENGHT 14                                

///
/// \brief alphanum lists the valid char for file name
///
static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789_-";

const int stringLength = sizeof(alphanum) - 1;

///
/// \brief Generates a pseudo-random number.
///

///int rand(){

///    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
///    std::mt19937_64 generator (seed);  // mt19937 is a standard mersenne_twister_engine
///    unsigned long num = generator();

//cout<<"Random : "<<num<<"\tIndex : ";
///    return num;
///}

///
/// \brief Returns random alphanumeric character
///
char
get_random_char ()
{
  char c;
  long index = (rand () % stringLength);
  if (index < 0)
    index *= -1;
  //cout<<index<<endl;
  c = alphanum[index];
  //cout<<"returning char : "<<c<<endl;
  return c;
}

/**********************************************************************************/
///
/// \todo Initialise _aesKey. Key will be passed from split method of ssb_file class.
///
SsbChunk::SsbChunk ()
{
//    cout<<"Empty obj constructed"<<endl;
//    cout<<"Initialise name of the chunk"<<endl;
  _chunkName = getChunkName ();
  _seqNo = -1;
  _nextIndex.clear ();
  _chunkSize = 0;
  _status = SSB_EMPTY_OBJ;
  memset (_aesKey, 0x00,
  SSB_DEFAULT_KEYLENGTH /* CryptoPP::AES::DEFAULT_KEYLENGTH*/);
  memset (_aesIv, 0x00, CryptoPP::AES::BLOCKSIZE);

//    cout<<"Initialise name of the chunk"<<endl;
}

SsbChunk::SsbChunk (string& chunk_name)
{
//    cout<<"Empty obj constructed"<<endl;
  _chunkName = chunk_name;
  _seqNo = -1;
  _nextIndex.clear ();
  _chunkSize = 0;
  _status = SSB_EMPTY_OBJ;
  memset (_aesKey, 0x00,
  SSB_DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/);
  memset (_aesIv, 0x00, CryptoPP::AES::BLOCKSIZE);
  //cout<<"Initialise name of the chunk"<<endl;
}

SsbChunk::SsbChunk (std::fstream& file_stream)
{
  //cout<<"Obj with file stream "<<endl;
  //cout<<"Initialise name of the chunk"<<endl;
  //cout<<(_chunk_name = getChunkName())<<endl;;
  _chunkName = getChunkName ();
  _seqNo = -1;
  _nextIndex.clear ();
  _chunkSize = 0;
  _status = SSB_EMPTY_OBJ;
  _fileStream.copyfmt (file_stream);                // copies all data members
  _fileStream.basic_ios<char>::rdbuf (file_stream.rdbuf ()); // copies stream buffer pointer
  memset (_aesKey, 0x00,
  SSB_DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/);
  memset (_aesIv, 0x00, CryptoPP::AES::BLOCKSIZE);
}
SsbChunk::SsbChunk (fstream& file_stream, size_t& chunk_size)
{
  //cout<<"Obj with file stream "<<endl;
  // cout<<"Initialise name of the chunk"<<endl;
  // cout<<(_chunk_name = getChunkName())<<endl;;
  _chunkName = getChunkName ();
  _seqNo = -1;
  _nextIndex.clear ();
  _chunkSize = chunk_size;
  _status = SSB_EMPTY_OBJ;
  _fileStream.copyfmt (file_stream);                // copies all data members
  _fileStream.basic_ios<char>::rdbuf (file_stream.rdbuf ()); // copies stream buffer pointer
  memset (_aesKey, 0x00,
  SSB_DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH */);
  memset (_aesIv, 0x00, CryptoPP::AES::BLOCKSIZE);
//    cout<<"Initialise name of the chunk"<<endl;
}

SsbChunk::SsbChunk (fstream& file_stream, size_t& chunk_size,/*std::map<int/*random seed,*/
		    KEY /*key*\/>::iterator*/key_it)
{
  //cout<<"Obj with file stream "<<endl;
  // cout<<"Initialise name of the chunk"<<endl;
  // cout<<(_chunk_name = getChunkName())<<endl;;
  _chunkName = getChunkName ();
  _seqNo = -1;
  _nextIndex.clear ();
  _chunkSize = chunk_size;
  _status = SSB_EMPTY_OBJ;
  _fileStream.copyfmt (file_stream);                // copies all data members
  _fileStream.basic_ios<char>::rdbuf (file_stream.rdbuf ()); // copies stream buffer pointer
  memset (_aesKey, 0x00,
  SSB_DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/);
  memset (_aesIv, 0x00, CryptoPP::AES::BLOCKSIZE);
//    cout<<"Initialise name of the chunk"<<endl;
}
///
/// \brief Adds the chunk in pool
/// \todo Implementation of add_in_pool is to be done.
///
int
SsbChunk::addInPool ()
{

  cout << "Implement add in pool" << endl;
  return 0;
}

/// 
/// This method generates a unique and random file name. The maximum length of the 
/// file name is CHUNK_NAME_LENGHT (14). Currently this is prep-pend by "pool_base/"
/// \todo make the pool_base programable.

string
SsbChunk::getChunkName ()
{

  SsbSetting * ssb_setting = SsbSetting::getSetting ();
  string name (ssb_setting->poolDir ());
  int count (0);
  bool is_unique (false);
  do
    {
      for (int i = 0; i < CHUNK_NAME_LENGHT; i++)
	{
	  name += get_random_char ();
	}
      count++;
      is_unique = isUniqueChunk (name);
    }
  while ((true != is_unique) && (count < 10)); // try to get unique name for maximum 10 times
  if (!is_unique)
    {
      name.clear ();
      cerr << "duplicate name" << endl;
      _status |= SSB_SPLIT_ERR;
    }
  ssb_setting->free ();
  return name;
}

///
/// Checks if the there is not other chunk present in the file-system.
/// Returns false if duplicate chunk is found.
///
bool
SsbChunk::isUniqueChunk (string& name)
{

  struct stat buffer;
  if (stat (name.c_str (), &buffer) != 0)
    return true;
  return false;
}


//Open file stram for _chunk_name
bool
SsbChunk::open ()
{
  bool ret_val (false);
  //cout<<"File will be opened"<<endl;
  _chunkStream.open (_chunkName,
		      ios::in | ios::out | ios::app | std::ios_base::binary);
  if (_chunkStream.good ())
    {
      //cout<<"Opened"<<endl;
      ret_val = true;
      _status = 0;
    }
  else
    {
      //cout<<"failed to open file"<<endl;
      _status |= SSB_OPEN_ERR;
    }
  return ret_val;

}

bool
SsbChunk::close ()
{
  bool ret_val (false);
  //cout<<"File will be opened"<<endl;
  _chunkStream.close ();
  if (_chunkStream.good ())
    {
      ret_val = true;
      _status = 0;
    }
  else
    {
      //cout<<"failed to open file"<<endl;
      _status |= SSB_CLOSE_ERR;
    }
  return ret_val;

}
///
/// This method makes use of CryptoPPWrapper object to encrypt the plaintext
/// \todo check if passing const char * will create ant problem or not.

string
SsbChunk::encrypt (const string& plaintext, KEY& key)
{

  //string plaintext;
  //plaintext.append(buffer);
// padding is not needed rightnow.
  /*    if(data_padding(plaintext,'\0'))
   cout<<"padding done"<<endl;
   else
   cout<<"padding failed"<<endl;
   */
  CryptoPPWrapper crypt_obj;
  //KEY key(_aesKey,DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/);
  return crypt_obj.aesEncrypt (plaintext, key, _aesIv);
}

///
/// This method makes use of CryptoPPWrapper object to decrypt the plaintext
///

string
SsbChunk::decrypt (string& buffer, KEY& key)
{

  string ciphertext;
  //cout<<"buffer size : "<<buffer.size()<<endl;
  ciphertext.append (buffer.c_str (), buffer.size ());
  //cout<<"ciphertext size : "<<ciphertext.size()<<endl;
  //cout<<"buffer : "<<buffer<<endl;
  //cout<<"ciphertext : "<<ciphertext<<endl;
  CryptoPPWrapper crypt_obj;
  //KEY key(_aesKey,DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/);
  return crypt_obj.aesDecrypt (ciphertext, key, _aesIv);
}

///
/// \todo magic number removal is needed.
bool
SsbChunk::dataPadding (string& data, const char pc)
{

  bool ret_val (true);
  size_t count = 16 + (16 - data.size () % 16);
  try
    {
      data.append (count, pc);
    }
  catch (...)
    {
      ret_val = false;
    }

  return ret_val;
}

///
/// This method creates a chunk out of parent file and for specified size, starting 
/// from the current offset of the file. Thus the parent file stream has to be 
/// adjusted before passing it for chunk creation.
///
/// \todo Handling to "file_stream and chunk_size not initialize" is to be done

bool
SsbChunk::create (
/*std::map<int/*random seed*\/,*/KEY& /*key*\/>::iterator*/key_it)
{ // creates a chunk from the file_stream, valid call only if file_stream and chunk_size was provided before

  //cout<<"chunk creation is remaining"<<endl;
  if (_fileStream)
    {
      char *buffer = new char[_chunkSize + 1];
      _fileStream.read (buffer, _chunkSize);
      size_t read_size (0);
      read_size = _fileStream.gcount ();
      if (_chunkSize != read_size)
	{

	  cout << "Only : " << read_size << " could be read" << endl;
	}
//    cout<<"buffer : "<<buffer;
//    cout<<"size : "<<_chunkSize;
      buffer[read_size + 1] = '\0';
      string data (buffer, read_size);
      //data.append(buffer, read_size);
      data = encrypt (data, /*key_0*/key_it/*->second*/); // encrypting data befor writing to the file
      _chunkStream << setw (20) << data.size ();
      _chunkStream << data;
      _chunkStream << setw (20) << key_it.seed ()/*->first*/;
      //cout<<"seed : "<<key_it.seed()/*->first*/<<endl;
      _chunkStream << _nextIndex;

      //cout<<"encrypted data : "<<data<<endl;
      //cout<<"size : "<<data.size()<<endl;
      //delete[] buffer;
    }
  else
    cerr << "Could not read _fileStream" << endl;
  return true;
}
///
/// This overload is to be used for changing the start of chunk after object construction.
/// Just re-position file stream and pass it to the method. It will create chunk of 
/// previously initialized chunk_size 
bool
SsbChunk::create (std::fstream& file_stream, /*std::map<int/*random seed*\/,*/
		  KEY /*key*\/>::iterator*/key_it)
{

  _fileStream.copyfmt (file_stream);                 // set _file_name to file
  return create (key_it);                                         // open file.

}

///
/// This overload gives flexibility to update file_stream and chunk_size both
///

bool
SsbChunk::create (std::fstream& file_stream, size_t& chunk_size, /*std::map<int/*random seed*\/,*/
		  KEY /*key*\/>::iterator*/key_it)
{ // creates a chunk, valid only if params are not provided befor

  _fileStream.copyfmt (file_stream);                 // set _file_name to file
  _chunkSize = chunk_size;
  return create (key_it);                                         // open file.
}

SsbChunk::~SsbChunk ()
{

}
///
/// This method appends the content of the chunk to the file_stream.
/// For this it also makes used of decrypt method to obtain plaintext from the 
/// encrypted block
/// \todo change this method to return the seed for next key mutation. caller function will use this seed to calculate next key.

int
SsbChunk::mergeTo (ofstream& file_stream, KEY& key)
{

  bool ret_val (true);
  char* buffer = new char[20 + 1];
  for (size_t i = 0; i < 21; i++)
    buffer[i] = '\0';
  _chunkStream.read (buffer, 20);                // reading size of the buffer
  //cout<<_chunkStream.gcount()<<" char read"<<endl;
  //cout<<"buffer : ."<<buffer<<endl;
  std::istringstream iss (buffer);
  size_t chunk_size;
  iss >> chunk_size;
  //cout<<"chunk size to be stitched is "<<chunk_size<<endl;
  delete buffer;
  buffer = new char[chunk_size + 1];
  _chunkStream.read (buffer, chunk_size);                      // reading data
  //cout<<_chunkStream.gcount()<<" char read"<<endl;
  buffer[_chunkStream.gcount ()] = '\0';
  string data;
  data.append (buffer, _chunkStream.gcount ());
  data = decrypt (data, key); // decrypting the data. its appending null byte at the end.
  file_stream.write (data.c_str (), data.size () - 1); // 1 less byte is for avoiding null byte. 
  //for(size_t i=0; i<chunk_size+1; i++)
  //    buffer[i] = '\0';
  //_chunkStream.read(buffer, chunk_size);                       // reading next_index
  delete buffer;
  buffer = new char[21];
  _chunkStream.read (buffer, 20);                     // reading seed
  buffer[_chunkStream.gcount ()] = '\0'; // incorrect seed may result if not terminated correctly
  int seed (0);
  seed = StrToNum<int> (buffer);
  //cout<<"seed for next key is : "<<seed<<endl;
  _chunkStream >> _nextIndex;
  //cout<<"next_index : "<<_nextIndex<<endl;
  //return ret_val;
  return seed;

}

bool
SsbChunk::linkNextChunk (string & next_chunk)
{

  _nextIndex = next_chunk;
  return true;
}
/*
 bool ssb_chunk::split(const int& N ){
 bool ret_val(false);
 cout<<"file splitting is to be done"<<endl;
 size_t n = get_valid_n(size(), N);
 cout<<"Using N = " << n <<" for splitting"<<endl;
 
 while(not all file is done) {
 create a chunk of file from curr location to N bytes.
 curr location += N
 }
 
 //if (successful)
 //_status |= SSB_SPLIT_FILE; ret_val = true;
 //else 
 // _status |= SSB_SPLIT_ERR;
 return ret_val;
 }
 */

bool
SsbChunk::remove ()
{
  bool ret_val (true);
  char* buffer = new char[20 + 1];
  for (size_t i = 0; i < 21; i++)
    buffer[i] = '\0';
  _chunkStream.read (buffer, 20);                // reading size of the buffer
  //cout<<_chunkStream.gcount()<<" char read"<<endl;
  //cout<<"buffer : ."<<buffer<<endl;
  std::istringstream iss (buffer);
  size_t chunk_size;
  iss >> chunk_size;
  _chunkStream.seekg (chunk_size + 40 /* skipping seed*/);
  _chunkStream >> _nextIndex;
//clean_file();

  if (_fileStream.is_open ())
    _fileStream.close ();
  if (std::remove (_chunkName.c_str ()) != 0)
    {
      ret_val = false;
      cerr << "Failed to remove " << _chunkName << endl;
    }
  else
//        cout<<"File removed"<<endl;

    return ret_val;
}

string
SsbChunk::chunkName ()
{

  return _chunkName;
}
string
SsbChunk::nextChunkName ()
{

  return _nextIndex;
}

bool
SsbChunk::good ()
{
  if (0 == _status)
    return true;
  return false;
}

int
SsbChunk::error ()
{          // returns error code. if all good, returns 0.

  return _status;
}

bool
SsbChunk::open_err ()
{                       // returns true if open fails

  if ((_status & SSB_OPEN_ERR) == SSB_OPEN_ERR)
    return true;
  return false;

}
bool
SsbChunk::ioErr ()
{                           // returns true if io fails

  if ((_status & SSB_IO_ERR) == SSB_IO_ERR)
    return true;
  return false;
}
bool
SsbChunk::cryptErr ()
{  // returns true if cryptographic operations fails

  if ((_status & SSB_CRYPT_ERR) == SSB_CRYPT_ERR)
    return true;
  return false;
}
/*
 bool ssb_chunk::split_err(){                                       // returns true if file cannot be split

 if((_status & SSB_SPLIT_ERR) == SSB_SPLIT_ERR)
 return true;
 return false;
 }
 */
bool
SsbChunk::removeErr ()
{         // returns true if file cannot be removed

  if ((_status & SSB_DEL_ERR) == SSB_DEL_ERR)
    return true;
  return false;
}
/*
 bool ssb_chunk::is_split(){                                      // returns true if file is split

 if((_status & SSB_SPLIT_FILE) == SSB_SPLIT_FILE)
 return true;
 return false;
 }
 */
size_t
SsbChunk::size ()
{                  // returns size of the file in bytes

  struct stat st;
  if (stat (_chunkName.c_str (), &st) == 0)
    return st.st_size;
  cerr << "cannot stat file : " << _chunkName << endl;
  return 0;

}

/*
 int ssb_chunk::get_valid_n(const size_t& file_size,const int& N){                            // Returns acceptable value of n

 int n(N);
 if(!valid_n(file_size,n))
 n = cal_valid_n(file_size);
 return n;
 }

 #define MIN_chunkSize 512
 // each chunck resulting with value n, should have at-least 512 bytes
 bool ssb_chunk::valid_n(const size_t& file_size, const int& N ){                               // check if the given N is accpeptable or not

 bool ret_val(false);
 //cout<<"Logic to check validity of N is to be implemented"<<endl;
 if( (file_size / N ) > MIN_chunkSize ){
 cout<<"N is valid"<<endl; 
 ret_val = true;
 }
 return ret_val;
 }

 // It will be called only if user wants to create more than valid number of chunks for the file

 int ssb_chunk::cal_valid_n( const size_t& file_size){   // calulates maximum value of N acceptable for file

 int n(1) ;                 // numbers of chunks to be done
 cout<<"Calculating valid n size"<<endl;
 if( file_size > MIN_chunkSize )
 n = file_size / MIN_chunkSize;
 cout<<"Returning n = "<<n<<endl;
 return n;
 }
 */
}
