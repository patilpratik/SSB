/****************************************************************************************************/
/*                                                                                                  */
/* Application    : This application is intended to provide secure box for putting up your stuff.   */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : SsbPool                                                                          */
/* Description    : This is a collection of all chunks. Better be daemon providing service          */
/* Input        :                                                                                   */
/* Output       :                                                                                   */
/* Date        	: 29 August 2014                                                                    */
/* Author       : Pratik Patil                                                                      */
//* License      :                                                                                   */
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
#include <map>
#include "ssb_pool.h"
#include <unistd.h>
#include <sys/types.h>
#include "cryptopp_wrapper/cryptopp_wrapper.h"
//#include <pwd.h>
//#include <sys/stat.h>

namespace SSB
{

#define SEPARATOR "::"
#define LINE_END "::::::"

  int SsbPool::_userCount = 0;
  SsbPool* SsbPool::_poolObj = NULL;

  using namespace std;
///
/// Default constructor for SsbPool
///
  SsbPool::SsbPool ()
  {

    _userCount = 0;
    _checksum.clear ();
    _catalog.clear ();

    SsbSetting * ssb_setting;
    ssb_setting = SsbSetting::getSetting ();

    _poolFile = ssb_setting->baseDir () + "/pool_catalog";
    ssb_setting->free ();
  }
  SsbPool::~SsbPool ()
  {

    writeCatalog ();
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : getPool
/// Description : Gets the Pool Object of singleton class.
/// @param      : None
/// @return     : SsbPool* _pool_obj
//////////////////////////////////////////////////////////////////////////////
  SsbPool*
  SsbPool::getPool ()
  {

    if (NULL == _poolObj)
      { // First time the get_pool is used. create instance
	_poolObj = new SsbPool;
	_poolObj->readCatalog ();
      }
    _userCount++;                                   // increment the user count
    return _poolObj;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : freePool
/// Description : surrender the pool object
/// @param      : None
/// @return     : void
//////////////////////////////////////////////////////////////////////////////
  void
  SsbPool::freePool ()
  {

    _userCount--;
    if (0 == _userCount)
      {
	if (NULL != _poolObj)
	  {
	    delete _poolObj;
	    _poolObj = NULL;
	  }
      }
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : readLine
/// Description : reads a single line from is and returns
/// @param      : ifstream& is (input stream from where to read)
/// @param      : string& str (output where read line is returned)
/// @return     : bool neof (returns true if anything is read)
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbPool::readLine (ifstream& is, string& str)
  {

    bool neof (false);
    string line;
    str.clear ();
    while (getline (is, line))
      {
	neof = true;
	str.append (line);
	line.clear ();
	if (str.find (LINE_END) != string::npos)
	  break;
      }
    return neof;                          /// return true if something was read.
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : readCatalog
/// Description : Reads _pool_file which has data about secured files and load
///               it in _catalog
/// @param      : None
/// @return     : bool retVal
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbPool::readCatalog ()
  {

    bool retVal (false);
    ifstream pool (_poolFile, ios::binary);
    string line;
    if (pool.is_open ())
      {

	while (readLine (pool, line))
	  {

	    int pos_s = 0, pos_e = 0;
	    pos_e = line.find (SEPARATOR, pos_s);
	    string file;
	    head_info hi;
	    file = line.substr (pos_s, pos_e); // pos_s is 0 at this moment
	    pos_s = pos_e + 2;
	    pos_e = line.find (SEPARATOR, pos_s);
	    hi._name = line.substr (pos_s, (pos_e - pos_s));
	    pos_s = pos_e + 2;
	    pos_e = line.find (LINE_END, pos_s);
	    hi._seed = line.substr (pos_s, (pos_e - pos_s));
	    _catalog[file] = hi;
	    retVal = true;
	  }
	pool.close ();
      }
    return retVal;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : writeCatalog
/// Description : writes content of _catalog into _pool_file
/// @param      : None
/// @return     : bool retVal
//////////////////////////////////////////////////////////////////////////////

  bool
  SsbPool::writeCatalog ()
  {

    bool retVal (false);
    ofstream pool (_poolFile, ios::binary | ios::out);
    for (auto it = _catalog.begin (); it != _catalog.end (); it++)
      {

	//pool<<"File name:"<<it->first<<":head name:"<<it->second._name<<":checksum :"<<it->second._checksum<<endl;
	pool << it->first << SEPARATOR << it->second._name << SEPARATOR
	    << it->second._seed << LINE_END << endl;
      }
    pool.close ();
    retVal = true;
    return retVal;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : add_file
/// Description : Adds file entry into the _catalog map
/// @param      : string& file_path (absolute path of the file)
/// @param      : string& head (head node/chunk of the encrypted file)
/// @param      : KEY& key (key to decrypt head)
/// @return     :
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbPool::addFile (string& file_path, string& head, const KEY& key)
  {

    head_info hi;
    hi._name = head;
    hi._seed = encrypt (head, key);
    _catalog[file_path] = hi;
    return true;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : encrypt
/// Description : Encrypts head
/// @param      : string& head (head chunck)
/// @param      : const KEY& key (key to encrypt)
/// @return     : string seed (seed for the next key)
//////////////////////////////////////////////////////////////////////////////
  string
  SsbPool::encrypt (string& head, const KEY& key)
  { //Todo check why head is passed

    string seed;
    CryptoPPWrapper crypt_obj;
    byte aes_iv[key.size () + 1];
    memset (aes_iv, 0x00, key.size ());
    seed = crypt_obj.aesEncrypt (NumToStr (key.seed ()), key, aes_iv);
    //cout<<"Size of seed : "<<seed.size()<<endl;
    return seed;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : getHeadNode
/// Description : returns head node of the specified file
/// @param      : const string& file_name
/// @return     : string head_node
//////////////////////////////////////////////////////////////////////////////
  string
  SsbPool::getHeadNode (const string& file_name)
  {

    string head_node ("");
    map<string, head_info>::const_iterator file_node = _catalog.find (
	file_name);
    if (_catalog.end () == file_node)
      cerr << file_name << " not found in pool" << endl;
    else
      head_node = file_node->second._name;
    return head_node;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : getSeed
/// Description : returns seed
/// @param      : const string& file_name
/// @param      : KEY& key
/// @return     : int seed (seed to germinate in current key and produce next key)
//////////////////////////////////////////////////////////////////////////////
  int
  SsbPool::getSeed (const string& file_name, KEY& key)
  {
    int seed (0);
    map<string, head_info>::const_iterator file_node = _catalog.find (
	file_name);
    if (_catalog.end () == file_node)
      cerr << file_name << " not found in pool" << endl;
    else
      {
	CryptoPPWrapper crypt_obj;
	byte aes_iv[key.size () + 1];
	memset (aes_iv, 0x00, key.size ());
	//cout<<"seed from file : "<<file_node->second._seed<<endl;
	seed = StrToNum<int> (
	    crypt_obj.aesDecrypt (file_node->second._seed, key, aes_iv));
      }
    return seed;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : lookupPool
/// Description : Checks if file is present in the catalog
/// @param      : const string& file_name
/// @return     : bool retVal (true if present)
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbPool::lookupPool (const std::string & file_name)
  {

    bool retVal (true);
    //map <std::string , head_info>
    if (_catalog.end () == _catalog.find (file_name))
      retVal = false;
    return retVal;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : list
/// Description : returns list of the files present in catalog
/// @param      : None
/// @return     : vector<string> list
//////////////////////////////////////////////////////////////////////////////
  vector<string>
  SsbPool::list ()
  {

    vector<string> list;
    for (auto it = _catalog.begin (); it != _catalog.end (); it++)
      {
	list.push_back (it->first);
      }
    return list;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : removeFile
/// Description : removes file from catalog.
/// @param      : string& file_path
/// @return     : bool (status of the remove: true of removed)
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbPool::removeFile (std::string& file_path)
  {

    if (!file_path.empty ())
      if (0 != _catalog.erase (file_path))
	return true;
      else
	return false;
    return true;                    // specified file is not present in the pool
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : printFiles
/// Description : prints file from catalog.
/// @param      : None
/// @return     : void
//////////////////////////////////////////////////////////////////////////////

  void
  SsbPool::printFiles ()
  {

    cout
	<< "*******************SSB has secured following files**************************"
	<< endl << endl;
//	for (int i = 0; i < file_list.size(); i++)
//		cout << "\t" << i + 1 << " : " << file_list[i] << endl;
    int i = 0;
    for (auto it = _catalog.begin (); it != _catalog.end (); ++it, ++i)
      {
	cout << "\t" << i + 1 << " : " << it->first << endl;
      }
    cout << endl
	<< "****************************************************************************"
	<< endl;
  }

} // end of SSB
