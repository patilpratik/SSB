/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: SsbPool                                                                           */
/* Description	: This is a collection of all chunks. Better be daemon providing service            */
/* Input    	:                                                                                   */
/* Output   	:                                                                                   */
/* Date	    	: 29 August 2014                                                                    */
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

#ifndef SSB_POOL_H
#define SSB_POOL_H

#include <map>
#include "ssb_setting.h"
#include "ssb_chunk.h"
#include <list>
#include <utility>
namespace SSB {
#if 0
///
/// \brief This structure holds the information about head_node of the file.
///
typedef struct {
	///
	/// \brief name of the chunk in file system
	///
	std::string _name;
	///
	/// \brief checksum of the head node calculated while encrypting the file
	///
	std::string _checksum;
}head_info;

/// 
/// \brief This singleton class holds the setting information of the ssb
///
class SSB_SETTING {

public:
	///
	/// \brief This method will return already created Object of the class
	///
	static SSB_SETTING * get_setting();
	///
	/// \brief This is get method for _base_dir menber
	///
	const std::string base_dir() const;
	///
	/// \brief This is get method for _admin_key.
	///
	const KEY admin_key() const;
protected:

private:
	///
	/// \brief Private constructor.
	///
	SSB_SETTING( std::string, KEY);
	/// 
	/// \brief This member holds the base directory in which all the files 
	/// 
	/// This member will not be updated once initialized. No set method will be provied for the same.
	///
	std::string _base_dir;
	///
	/// \brief This member is a key that will be used for encrypting setting and pool files
	///
	KEY _admin_key;
	///
	/// \brief This member holds the address of the obhect
	///
	/// This member will not be updated once initialized. No set method will be provied for the same.
	///
	static SSB_SETTING * _obj;
};

extern SSB_SETTING ssb_setting;

#endif

///
/// \brief This structure holds the information about head_node of the file.
///
typedef struct {
	///
	/// \brief name of the chunk in file system
	///
	std::string _name;
	///
	/// \brief seed of the head node calculated while encrypting the file
	///
	std::string _seed;
} head_info;

///////////////////////////////////////////////////////////////////////////////
/// This is a singleton class.
/// This class has the map of the file and its head node.
/// \brief It serves as the index for the files added in pool.
///
/// The list of the files with corrosponding head node will be stored by encrypting
/// it with the admin password.
/// \todo checksum of the head node is yet to be added
/// \todo _catalog needs to be made persistant. Encryption has to be used.
///////////////////////////////////////////////////////////////////////////////
class SsbPool {

public:
	///
	/// \brief returns the pointer to the pool
	///
	static SsbPool * getPool();
	///
	/// \brief renounce the pool
	///
	void freePool();
	/// 
	/// \brief adds the file to pool and returns its index
	///
	bool addFile(std::string& /*file_path*/, std::string& /*head_chunk*/,
			const KEY&);
	/// 
	/// \brief opens the file header listed in pool
	///
	bool openFile(std::string& /*file_path*/);
	/// 
	/// \brief adds the chunk name to the file's list
	///
	bool addChunk(std::string& /*file_path*/, std::string /*chunk_name*/);
	/// 
	/// \brief removes file from the pool. 
	///
	///This method is to be called when all chunks of the file are removed. If this is not observed, zombie chunk files will remain in the pool
	///
	bool removeFile(std::string& /*file_path*/);
	/// 
	/// returns head chunk name of the file
	///
	std::string getHeadNode(const std::string& /*file_path*/);
	/// 
	/// \brief returns the list of files
	///
	std::list<std::string> listFiles();
	///
	/// \brief This method will check if the file is alrealdy in the pool
	///
	bool lookupPool(const std::string &);
	/// 
	/// \brief This method will return the seed of the file
	///
	int getSeed(const std::string&, KEY&);
	/// 
	/// \brief This method will return the list of the files added in SSB
	///
	std::vector<std::string> list();
	///
	/// \brief This method will print the list of the files added in SSB
	///
	void printFiles();

protected:

private:
	/// 
	/// \brief pointer to hold the object
	///
	static SsbPool * _poolObj;
	SsbPool();
	///
	///private constructor
	///	
	~SsbPool();
	///
	/// \brief This read single record from the catalog file
	///
	bool readLine(std::ifstream& is, std::string& str);
	/// 
	/// \brief reads the file to populate _catalog
	///
	bool readCatalog();
	///
	///writes the updated catalog to catalog file
	///
	bool writeCatalog();
	///
	/// \brief accepts a head file
	///
	std::string encrypt(std::string& /*head_file*/, const KEY&);
	///
	/// \brief maintains the catalog of all files added in pool
	///
	std::map<std::string /*file_path*/, head_info> _catalog;
	///
	/// \brief to maintain the count of user
	///
	static int _userCount;
	///
	/// \brief checksum of complete catalog
	///
	std::string _checksum;
	///
	/// \brief This member holds the name of the pool file
	///
	std::string _poolFile;
};
}	// end of SSB
#endif //end of #ifndef SSB_POOL_H
