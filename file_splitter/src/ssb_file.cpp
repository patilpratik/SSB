/****************************************************************************************************/
/*                                                                                                  */
/* Application  : This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : SsbFile                                                                           */
/* Description  : This is a file representative                                                     */
/* Input        : File path                                                                         */
/* Output       : Adds file to or retrieve file from the pool of chunk                              */
/* Date         : 26 August 2014                                                                    */
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
#include <memory>
#include <stdlib.h>
#include "ssb_file.h"
#include "ssb_pool.h"
#include "authenticator/authenticator.h"
#include "key_mutator/key_mutator.h"
using namespace std;

namespace SSB
{
#define MIN_CHUNK_SIZE 16 //512 
  /******************************************

   uses following macros to mark object status

   #define SSB_OPEN_ERR    0x00000001
   #define SSB_IO_ERR      0x00000010
   #define SSB_CRYPT_ERR   0x00000100
   #define SSB_SPLIT_ERR   0x00001000
   #define SSB_DEL_ERR     0x00010000
   #define SSB_EMPTY_OBJ   0x00100000

   #define SSB_SPLIT_FILE  0x01000000
   *******************************************/

  SsbFile::SsbFile ()
  {
    //cout<<"Empty obj constructed"<<endl;
    _fileName.clear ();
    _head = -1;
    _status = SSB_EMPTY_OBJ;
  }

  SsbFile::SsbFile (const string& file_path)
  {
    //cout<<"Obj with file name "<<endl;
    _fileName = getAbsPath (file_path);
    _head = -1;
    _status = SSB_EMPTY_OBJ;
  }

//Open file stram for _file_name
  bool
  SsbFile::open ()
  {
    bool ret_val (false);
    //cout<<"File will be opened"<<endl;
    _fileStream.open (_fileName, ios::in | std::ios_base::binary);
    if (_fileStream.good ())
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

//
  bool
  SsbFile::open (const string& file)
  {

    _fileName = getAbsPath (file);                  // set _file_name to file
    return open ();                                         // open file.

  }

  bool
  SsbFile::close ()
  {

    _fileStream.close ();
    return true;
  }

  string
  SsbFile::getAbsPath (string file_name)
  {
    size_t pos = file_name.find_last_of ("\/");
    string parent_dir (".");
    if (string::npos != pos)
      parent_dir = file_name.substr (0, pos);
    char * buffer = realpath (parent_dir.c_str (), NULL);
//    if(NULL != buffer)
    string path (buffer);
    free (buffer);
    if (string::npos != pos)
      path += file_name.substr (pos);
    else
      path += "/" + file_name;
    return path;
  }

///
/// This method adjusts the file stream so that the current offset of the stream 
/// points to the start of the chunk indicated by chunk_num.
/// for eg. if chunk_num = 4 then file_stream will seek to start of the 4th chunk from the end. 
///Note that the splitting of the file is started from the end towards the beginning of the file.
///
  void
  SsbFile::adjustFileStream (unsigned int& chunk_num, size_t& chunk_size)
  {

    size_t pos;
    pos = chunk_size * (chunk_num - 1);
    _fileStream.seekg (pos);
  }

///
/// This method is just a wrapper provided by SsbFile class to call lookup_pol method of ssb_pool class
///
  bool
  SsbFile::lookup (const string& file_name)
  {

    bool ret_val;
    SsbPool* pool_obj = SsbPool::getPool ();
    ret_val = pool_obj->lookupPool (file_name);
    pool_obj->freePool ();
    return ret_val;
  }

  string
  SsbFile::readPassword ()
  {
    Authenticator athr;
    string pwd;
    int attempt;
    for (attempt = 0; attempt < MAX_ATTEMPTS; attempt++)
      {
	pwd = athr.readPassword ();
	if (athr.isValidPass (pwd))
	  break;                                     // break out of the loop.
	else
	  {
	    cout << endl << "Password does not satisfy necessary condition. "
		<< endl << "Error Code :" << athr.getErrorCode () << endl
		<< "Try again." << endl;
	    pwd.clear ();
	    _status |= SSB_PWD_ERR;
	  }
      }
    if (MAX_ATTEMPTS <= attempt)
      cerr << "Maximum attempts." << endl;

    return pwd;
  }

///
/// This method splits the current file in N sections. Splitting starts from the last chunk towards the first chunk. I.e that first last chunk will be generated.
///
  bool
  SsbFile::split (const int& chunkCount, string& pwd)
  {

    if (pwd.empty ())
      {
	return false;
      }
    bool ret_val (true);
    int keyLenght = SSB_DEFAULT_KEYLENGTH; //CryptoPP::AES::DEFAULT_KEYLENGTH;
    KEY km_it;
    shared_ptr<KeyMutator> keyMutator = NULL;

    if (false == lookup (_fileName))
      {

	//cout<<"file splitting is to be done"<<endl;
	const size_t total_chunks = correctNumOfChunks (size (), chunkCount);
	cout << "Using N = " << total_chunks << " for splitting" << endl;
	unsigned int counter = total_chunks;
	size_t chunk_size = size () / total_chunks;
	string next_chunk; //  holds the name of the chunk to be linked to the next
	_fileStream.seekg (0, _fileStream.beg);
	if ((true == ret_val) && (counter == total_chunks))
	  {
	    // write last chunk first
	    //            cout<<"Adjust filestream "<<endl;
	    size_t last_chunk_size = chunk_size + size () % total_chunks;
	    adjustFileStream (counter, chunk_size);
	    keyMutator = make_shared < KeyMutator
		> (KeyMutator (pwd, total_chunks + 1, keyLenght)); // 1 extra key is produced which will be used for adding file in pool
	    km_it = keyMutator->next ();
	    SsbChunk chunk_file (_fileStream, last_chunk_size);
	    if ((ret_val = chunk_file.open ()))
	      // open chunk
	      if ((ret_val = chunk_file.create (km_it)))
		// create file from the given stream
		next_chunk = chunk_file.chunkName (); // get the name of the chunk

	    //cout<<"Chunk no : "<<counter<<" Key : "<<km_it.value()<<endl;
	    counter--;
	  }
	while ((counter > 0) && (true == ret_val))
	  {
	    //cout<<"Adjust filestream "<<endl;
	    km_it = keyMutator->next ();
	    adjustFileStream (counter, chunk_size);
	    SsbChunk chunk_file (_fileStream, chunk_size);
	    if ((ret_val = chunk_file.open ()))
	      // open a chunk
	      if ((ret_val = chunk_file.linkNextChunk (next_chunk)))
		// link next node
		try
		  {
		    if ((ret_val = chunk_file.create (km_it)))
		      {
			// create chunk
			next_chunk = chunk_file.chunkName ();
		      }
		  }
		catch (...)
		  {
		    removeChunks (chunk_file);
		  }
	    //cout<<"Chunk no : "<<counter<<" Key : "<<km_it.value()<<endl;
	    counter--;
	  }
	if (!next_chunk.empty ())
	  {
	    _head = next_chunk;
	    // setting the status
	    if (true == ret_val)
	      {
		if (addFileInPool (keyMutator->next ()) == true)
		  _status |= SSB_SPLIT_FILE;
		else
		  _status |= SSB_POOL_ERR;
	      }
	    else
	      _status |= SSB_SPLIT_ERR;
	  }
	else
	  _status |= SSB_SPLIT_ERR;

      }
    else
      {
	cerr << "File alrady present in the pool. cannot overwrite." << endl;
	_status |= _status |= SSB_SPLIT_ERR;
	ret_val = false;
      }

    return ret_val;
  }

///
/// This method splits the current file in N sections. Reads password and calls overload.
///
  bool
  SsbFile::split (const int& N)
  {
    bool ret_val (true);
//    map<int/*random seed*/,KEY /*key*/>::iterator
    string pwd = readPassword ();
    //cout<<"pwd : ."<<pwd<<"."<<endl;
    ret_val = split (N, pwd);
    //delete km;
    return ret_val;
  }

///
/// This method makes an entry of the file in the ssb_pool object.
/// This enables tracking down the list of files currently in pool.
/// \todo checksum of the original file is also to be added in pool entry.
///
  bool
  SsbFile::addFileInPool (const KEY& key)
  {

//    cout<<"add_file_in_pool : Implementation is remaining"<<endl;
    SsbPool * pool_obj = SsbPool::getPool ();
    pool_obj->addFile (_fileName, _head, key);
    pool_obj->freePool ();
    return true;
  }

///
/// This method recreates the file from its chunks. This recreation is from first node
/// to the last. I.e its exactly in opposite order that of the splitting.
///
  bool
  SsbFile::stitch (string& pwd)
  {

    Authenticator athr;
    SsbPool* pool_obj = SsbPool::getPool ();
    string head_node = pool_obj->getHeadNode (_fileName);
    if (head_node.empty ())
      {
	return false;
      }
    KeyMutator *km = NULL;

    //cout<<"pwd : ."<<pwd<<"."<<endl;
    int num = SSB_DEFAULT_KEYLENGTH; //CryptoPP::AES::DEFAULT_KEYLENGTH ;

    km = new KeyMutator (num);
    KEY key;
    key = km->makeKey (pwd);

    int seed;
    if (0 == (seed = athr.authenticate (_fileName, key)))
      {
	// This means that the authentication failed. Hence empty token is returned.
	cerr << "Incorrect password" << endl;
	pool_obj->freePool ();
	return false;
      }
    //int seed = pool_obj->get_seed(_file_name, key) ;                 // get the to produce key for head_node
    key.setSeed (seed);
    //cout<<"seed : "<<seed<<endl;
    key = km->getNextKey (key, seed);         // getting the key for head node
    int counter (1);				// Just for degubbing purpose
    string out_file (_fileName);
    size_t pos = out_file.rfind ("/");
    if (string::npos != pos)
      out_file.replace (pos, 1, "/ssb_");
    else
      {
	out_file = "ssb_";
	out_file += _fileName;
      }
    //    out_file += ".out";
    ofstream out_stream;
    out_stream.open (out_file, ios::out | std::ios_base::binary);
    SsbChunk file_chunk (head_node);
    file_chunk.open ();

    //cout<<"Chunk no : "<<counter<<" Key : "<<key.value()<<endl;

    seed = file_chunk.mergeTo (out_stream, key);// merging this chunk with the output file.
    key.setSeed (seed);             // setting seed for the next key generation
    //cout<<"head node : "<<head_node<<endl;
    string next = file_chunk.nextChunkName ();
    while (!next.empty ())
      {
	counter++;
	SsbChunk file_chunk (next);
	file_chunk.open ();
	key = km->getNextKey (key, seed);
	//cout<<"Chunk no : "<<counter<<" Key : "<<key.value()<<endl;
	seed = file_chunk.mergeTo (out_stream, key);
	key.setSeed (seed);
	next = file_chunk.nextChunkName ();
      }
    pool_obj->freePool ();
    delete km;
    return true;
  }

///
/// Reads password and calls overload
  bool
  SsbFile::stitch ()
  {

//    cout<<"stitch : Implementation remaining"<<endl;
    Authenticator athr;
    string pwd (athr.readPassword ());
    if (pwd.empty ())
      {
	return false;
      }

    return stitch (pwd);
  }

  bool
  SsbFile::removeChunks (SsbChunk& file_chunk)
  {
    bool ret_val (false);
    file_chunk.open ();
    if (file_chunk.remove ())
      {
	ret_val = true;
	string next = file_chunk.nextChunkName ();
	while (!next.empty () && (ret_val == true))
	  {
	    SsbChunk file_chunk (next);
	    file_chunk.open ();
	    if (file_chunk.remove ())
	      {
		next = file_chunk.nextChunkName ();
	      }
	    else
	      ret_val = false;
	  }
      }
    return ret_val;
  }

///
/// Removes the file from the pool. Deletes the entry of the file from the catalog. It reads through all chunks to remove all of them.
///
  bool
  SsbFile::remove (string& pwd)
  {

    bool ret_val (false);
    Authenticator athr;
    KeyMutator *km = NULL;
    int num = SSB_DEFAULT_KEYLENGTH; //CryptoPP::AES::DEFAULT_KEYLENGTH ;

    km = new KeyMutator (num);
    KEY key;
    key = km->makeKey (pwd);
    delete km;

    int seed;

    SsbPool* pool_obj = SsbPool::getPool ();
    if (0 == (seed = athr.authenticate (_fileName, key)))
      {
	// This means that the authentication failed. Hence empty token is returned.
	cerr << "Incorrect password" << endl;
	ret_val = false;
      }
    else
      {
	string head_node = pool_obj->getHeadNode (_fileName);

	if (!head_node.empty ())
	  {
	    SsbChunk file_chunk (head_node);
	    ret_val = removeChunks (file_chunk);
	    if (true == ret_val)
	      pool_obj->removeFile (_fileName);
	  }
      }

    pool_obj->freePool ();
    return ret_val;
  }
  bool
  SsbFile::remove ()
  {

    Authenticator athr;
    string pwd (athr.readPassword ());
    if (pwd.empty ())
      {
	cerr << "Incorrect password" << endl;
	return false;
      };
    return remove (pwd);
  }

  bool
  SsbFile::encrypt ()
  {
    bool ret_val (false);
    cout << "content of the file wil be encrypted" << endl;
    return ret_val;
  }

  bool
  SsbFile::decrypt ()
  {
    bool ret_val (false);
    cout << "content of the file will be encrypted" << endl;
    return ret_val;
  }

  bool
  SsbFile::good ()
  {
    if (0 == _status)
      return true;
    return false;
  }

  int
  SsbFile::error ()
  {           // returns error code. if all good, returns 0.

    return _status;
  }

  bool
  SsbFile::openErr ()
  {                        // returns true if open fails

    if ((_status & SSB_OPEN_ERR) == SSB_OPEN_ERR)
      return true;
    return false;

  }
  bool
  SsbFile::ioErr ()
  {                            // returns true if io fails

    if ((_status & SSB_IO_ERR) == SSB_IO_ERR)
      return true;
    return false;
  }
  bool
  SsbFile::cryptErr ()
  {   // returns true if cryptographic operations fails

    if ((_status & SSB_CRYPT_ERR) == SSB_CRYPT_ERR)
      return true;
    return false;
  }
  bool
  SsbFile::splitErr ()
  {             // returns true if file cannot be split

    if ((_status & SSB_SPLIT_ERR) == SSB_SPLIT_ERR)
      return true;
    return false;
  }
  bool
  SsbFile::removeErr ()
  {          // returns true if file cannot be removed

    if ((_status & SSB_DEL_ERR) == SSB_DEL_ERR)
      return true;
    return false;
  }

  bool
  SsbFile::isSplit ()
  {                     // returns true if file is split

    if ((_status & SSB_SPLIT_FILE) == SSB_SPLIT_FILE)
      return true;
    return false;
  }

///
/// This method returns total number of characters in file.
/// It also checks for the existence of the file. If file is not found, then returns 0
///
  size_t
  SsbFile::size ()
  {                   // returns size of the file in bytes

    struct stat st;
    if (stat (_fileName.c_str (), &st) == 0)
      return st.st_size;
    cerr << "cannot stat file : " << _fileName << endl;
    return 0;

  }

///
/// Returns a valid number of chunks that can be generated. If user specified number is valid, same is returned unchanged.
/// If number specified does not fulfill requirement, then maximum number of chunks that can be generated is returned. 
///

  int
  SsbFile::correctNumOfChunks (const size_t& file_size, const int& N)
  { // Returns acceptable value of n

//cout<<"given N = "<<N<<endl;
    int n (N);
    if (!valildateNumOfChunks (file_size, n))
      n = calMaxNumOfChunks (file_size);
//cout <<"Valid N = "<<N<<endl;
    return n;
  }

// each chunck resulting with value n, should have at-least 512 bytes
  bool
  SsbFile::valildateNumOfChunks (const size_t& file_size, const int& N)
  { // check if the given N is accpeptable or not

    bool ret_val (false);
    //cout<<"Logic to check validity of N is to be implemented"<<endl;
    if (0 != N)
      {
	size_t chunk_size = file_size / N;
	if (chunk_size > MIN_CHUNK_SIZE)
	  {
	    //   if( file_size % chunk_size > 16) {
//            cout<<"N is valid"<<endl; 
	    ret_val = true;
	    // }
	  }
      }
    return ret_val;
  }

// It will be called only if user wants to create more than valid number of chunks for the file

  int
  SsbFile::calMaxNumOfChunks (const size_t& file_size)
  { // calulates maximum value of N acceptable for file

    int n (1);                 // numbers of chunks to be done
//    cout<<"Calculating valid n size"<<endl;
    if (file_size > MIN_CHUNK_SIZE)
      {
	n = file_size / MIN_CHUNK_SIZE;
	if ((file_size % MIN_CHUNK_SIZE) != 0)
	  if (1 != n)
	    n--;
      }
//    cout<<"Returning n = "<<n<<endl;
    return n;
  }

} // end of SSB
