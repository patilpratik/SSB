/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: ssb_pool                                                                          */
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

#ifndef SSB_SETTING_H
#define SSB_SETTING_H

#include <unordered_map>
#include "key_mutator/key.h"
#include <list>
#include <utility>
namespace SSB
{
/////////////////////////////////////////////////////////////////////////
/// This is a singelton class. It hold configuration related information
/// about ssb.
/// Todo use config file to persist configuration and allow user to change
///
/// \brief This singleton class holds the setting information of the ssb
////////////////////////////////////////////////////////////////////////
  class SsbSetting
  {

  public:
    ///
    /// \brief This method will return already created Object of the class
    ///
    static SsbSetting *
    getSetting ();
    ///
    /// \brief This is get method for _base_dir menber
    ///
    const std::string
    baseDir () const;
    ///
    /// \brief This is get method for _admin_key.
    ///
    const KEY
    adminKey () const;
    ///
    /// \brief This returns the path of the pool directory
    ///
    const std::string
    poolDir () const;
    ///
    /// \brief This method frees the object
    ///
    bool
    free ();
  protected:

  private:
    ///
    /// \brief Private constructor.
    ///
    SsbSetting (std::string&, KEY&);
    ///
    /// \brief This member holds the base directory in which all the files
    ///
    /// This member will not be updated once initialized. No set method will be provied for the same.
    ///
    const std::string _baseDir;
    ///
    /// \brief This member is a key that will be used for encrypting setting and pool files
    ///
    const KEY _adminKey;
    ///
    /// \brief This member holds the address of the obhect
    ///
    /// This member will not be updated once initialized. No set method will be provied for the same.
    ///
    static SsbSetting * _obj;
    ///
    /// \brief This member tracks reference count
    ///
    static int refCount;
    ///
    /// \brief This method creates dir if it does not exit.
    ///
    bool
    makedir (const std::string&);
  };
}
#endif
