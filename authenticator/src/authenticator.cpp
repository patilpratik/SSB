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

#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>

#include "authenticator.h"

using namespace std;
namespace SSB
{
  const int Authenticator::EXCEPTION::EMPTY_PWD = 1;
  const int Authenticator::EXCEPTION::INVALID_PWD_LEN = 2;
  string
  Authenticator::readPassword ()
  {

    termios oldTerminal;
    tcgetattr (STDIN_FILENO, &oldTerminal);

    termios newTerminal = oldTerminal;
    newTerminal.c_lflag &= ~ECHO;
    tcsetattr (STDIN_FILENO, TCSANOW, &newTerminal);

    string pwd;
    cout << "Enter password: ";
    //fflush(stdin);
    cin.clear ();
    cin >> pwd;

    tcsetattr (STDIN_FILENO, TCSANOW, &oldTerminal);
    cout << endl;

    return pwd;

  }

  void
  Authenticator::minLenghtCheck (string& pwd) throw (EXCEPTION)
  {

    if (pwd.empty ())
      {
	EXCEPTION e (EXCEPTION::EMPTY_PWD);
	throw e;
      }
    if (pwd.size () < 6)
      {
	EXCEPTION e (EXCEPTION::INVALID_PWD_LEN);
	throw e;
      }

  }

///
/// Currently only constraint on password is size. Its size should be >= 6
/// 
  bool
  Authenticator::isValidPass (string & pwd)
  {

    bool retVal (false);
    try
      {
	minLenghtCheck (pwd);
	retVal = true;
      }
    catch (EXCEPTION e)
      {
	retVal = false;
	errorId = e.getErrorCode ();
      }
    return retVal;
  }

  int
  Authenticator::authenticate (std::string& file_name, KEY& key)
  {

    SsbPool* pool_obj = SsbPool::getPool ();
    int seed;
    try
      {
	seed = pool_obj->getSeed (file_name, key);
      }
    catch (...)
      {
	return 0;
      }
    pool_obj->freePool ();
    return seed;
  }
} // end of SSB
