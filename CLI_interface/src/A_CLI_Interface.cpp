/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: A_CLI_Interface                                                                   */
/* Description	: This is abstract class providing basic CLI infrastructure                         */
/* Input    	:                                                                                   */
/* Output   	:                                                                                   */
/* Date	    	: 30 July 2014                                                                      */
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

#include "../include/A_CLI_Interface.h"

using namespace std;
CliInterface::CliInterface ()
{
  // TODO Auto-generated constructor stub
  _validationStatus = true;
  initSupportedOpt ();
  _optList = 0;

}

CliInterface::~CliInterface ()
{
  // TODO Auto-generated destructor stub
}

bool
CliInterface::isValid ()
{
  return _validationStatus;
}

int
CliInterface::checkArg (const string& arg)
{
  //cout<<"arg : \""<<arg<<"\""<<endl;
  short int i = 0;
  int ret_val (0);
  for (map<string, int>::iterator it = _supportedOpt.begin ();
      it != _supportedOpt.end (); it++)
    {
      if ((arg[0] == '-') && (arg[1] == '-'))
	{
	  if ((*it).first.compare (0, arg.size (), arg) == 0)
	    {
	      i++;
	      ret_val = (*it).second;
//                cout<<"Found argument : "<<(*it).first<<endl;
	    }
	}
      else
	{
	  if ((*it).first.compare (1, 2, arg) == 0)
	    {
	      i++;
	      ret_val = (*it).second;
//                cout<<"Found argument : "<<(*it).first<<endl;
	    }
	}
    }
  if (1 != i)
    {
      ret_val = 0;                      // match not found or ambiguous option
    }
  return ret_val;
}

void
CliInterface::initSupportedOpt ()
{
//    cout<<"List of supported opts will be initialised"<<endl;
  _supportedOpt.insert (pair<string, int> (OPT_HELP, HELP));
  _supportedOpt.insert (pair<string, int> (OPT_VER, VERSION));
}

bool
CliInterface::remOptSupport (const string& opt)
{
  map<string, int>::iterator it;
  it = _supportedOpt.find (opt);
  if (_supportedOpt.end () != it)
    {
      _supportedOpt.erase (opt);
      return true;
    }
  return false;                                   // opt is already supported.
}

bool
CliInterface::help ()
{
  if ((HELP & _optList) == HELP)
    return true;
  return false;
}
bool
CliInterface::version ()
{
  if ((VERSION & _optList) == VERSION)
    return true;
  return false;
}

bool
CliInterface::addOptSupport (const string& opt, const int &val)
{
  if ((_supportedOpt.insert (pair<string, int> (opt, val))).second)
    return true;
  return false;                                   // opt is already supported.
}

