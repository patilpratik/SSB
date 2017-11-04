/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: SsbSetting                                                                        */
/* Description	: This is a setting class                                                           */
/* Input    	:                                                                                   */
/* Output   	:                                                                                   */
/* Date	    	: 2 November 2014                                                                   */
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

#include <iostream>
#include <string>
//#include <unordered_map>
//#include "ssb_pool.h"
#include <unistd.h>          //getuid
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>

#include "ssb_setting.h"

using namespace std;
namespace SSB
{
  SsbSetting * SsbSetting::_obj = NULL;
  int SsbSetting::refCount = 0;

  SsbSetting::SsbSetting (std::string& bdir, KEY& adm_key) :
      _baseDir (bdir), _adminKey (adm_key)
  {

    makedir (_baseDir); // Make _base_dir if it does not exist // This should be made while installing
    makedir (poolDir ());                  // make poolDir if it does not exits
  } //////////////////////////////////////////////////////////////////////////////
/// Function    : makedir
/// Description : create directory
/// @param      : const string& dir
/// @return     : bool (true if created)
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbSetting::makedir (const string& dir)
  {

    struct stat buffer;
    if (stat (dir.c_str (), &buffer) != 0)
      mkdir (dir.c_str (), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //\todo check access rights
    if (stat (dir.c_str (), &buffer) == 0)
      return true;
    return false;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : get_setting
/// Description : Gets the setting Object of singleton class.
/// @param      : None
/// @return     : SSB_SETTING* _obj
//////////////////////////////////////////////////////////////////////////////
  SsbSetting *
  SsbSetting::getSetting ()
  {

    if (NULL == _obj)
      {
        string basedir ((getpwuid (getuid ()))->pw_dir);
        basedir.append ("/ssb/");
        KEY admkey;
        _obj = new SsbSetting (basedir, admkey);
      }
    refCount++;
    return _obj;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : base_dir
/// Description : Returns base directory
/// @param      : None
/// @return     : const string _base_dir
//////////////////////////////////////////////////////////////////////////////
  const std::string
  SsbSetting::baseDir () const
  {
    return _baseDir;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : admin_key
/// Description : Returns admin key
/// @param      : None
/// @return     : const KEY _admin_key
//////////////////////////////////////////////////////////////////////////////
  const KEY
  SsbSetting::adminKey () const
  {
    return _adminKey;
  }

//////////////////////////////////////////////////////////////////////////////
/// Function    : poolDir
/// Description : Returns pool directory
/// @param      : None
/// @return     : const string str (path to the pool within _base_dir )
//////////////////////////////////////////////////////////////////////////////
  const string
  SsbSetting::poolDir () const
  {

    string str (_baseDir);
    str += "pool/";
    return str;
  }
//////////////////////////////////////////////////////////////////////////////
/// Function    : free_pool
/// Description : surrender the setting object
/// @param      : None
/// @return     : void
//////////////////////////////////////////////////////////////////////////////
  bool
  SsbSetting::free ()
  {

    bool retVal (false);
    refCount--;
    if (refCount == 0)
      {
        if (_obj != NULL)
          {
            delete _obj;
            _obj = NULL;
            retVal = true;
          }
      }
    return retVal;
  }
}
