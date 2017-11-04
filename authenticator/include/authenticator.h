/****************************************************************************************************/
/*                                                                                                  */
/* Application  : This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : Authenticator                                                                     */
/* Description  : This module provides the mechanism to authenticate user and read passwords        */
/* Input        :   1)                                                                              */
/*                  2)                                                                              */
/* Output       : Authentication status                                                             */
/* Date         : 05 November 2014                                                                  */
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

#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "key_mutator/key.h"
#include "file_splitter/ssb_pool.h"

namespace SSB
{
#define MAX_ATTEMPTS 3

///
/// \brief This class will authenticate the user by validating provided password.
///
/// Utility will not proceed if the authentication fails.
/// \todo Implement authenicator class and integrate with the utility
  class Authenticator
  {

  private:

    ///
    /// \breif This class defines exceptions that handle error situation
    ///
    class EXCEPTION
    {
    public:
      EXCEPTION (const int err) :
	  errorId (err)
      {
      }
      static const int INVALID_PWD_LEN;
      static const int EMPTY_PWD;
      int
      getErrorCode ()
      {
	return errorId;
      }
    private:
      int errorId;
    };

    ///
    /// \bried This is the error ID
    ///
    int errorId;
  public:
    ///
    /// \brief This method will read password from console
    ///
    std::string
    readPassword ();
    ///
    /// \brief This method will authenticate the user and will produce a seed for file decryption if successful authentication
    ///
    int
    authenticate (std::string &, KEY&);
    ///
    /// \brief This method returns true if the password given satisfies requirements
    ///
    bool
    isValidPass (std::string&);
    ///
    /// \breif This method will put lenght constain on password
    ///
    void
    minLenghtCheck (std::string&) throw (EXCEPTION);
    ///
    /// \brief This method returns error code
    ///
    int
    getErrorCode ()
    {
      return errorId;
    }
  protected:

  };
} // end of SSB
#endif
