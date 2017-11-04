/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	:                                                                                   */
/* Description	:                                                                                   */
/* Input    	:                                                                                   */
/* Output   	:                                                                                   */
/* Date	    	: 17 August 2014                                                                    */
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
#include <stdlib.h>
#include "ssb_cli_interface.h"
#include <sstream>
using namespace std;

template<class T>
  T
  StrToNum (const std::string str)
  {
    T num;
    std::istringstream buffer (str);
    buffer >> num;
    return num;
  }

ssbCliInterface::ssbCliInterface ()
{

  //cout<<"Options object will be constucted"<<endl;
  initSupportedOpt ();
  _chunks_count = 0;
  _inputFile.empty ();
  _outputFile.empty ();
}
ssbCliInterface::ssbCliInterface (int argc, char** argv)
{

  //cout<<"Options object will be constucted"<<endl;
  initSupportedOpt ();
  _chunks_count = 0;
  _inputFile.empty ();
  _outputFile.empty ();
  if (1 == argc)
    {
      _optList |= LIST;
    }
  else
    {
      parse_argv (argc, argv);
    }
}

void
ssbCliInterface::printHelp ()
{

  cout << "\tSecure Storeage Box Version " << SSB_VERSION << endl;
  cout << "\tCopyright (C) 2017  Pratik Patil" << endl;
  cout
      << "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License Version 3 as published by the Free Software Foundation."
      << endl << endl;
  cout << "Usage: [OPERATION MODE] [file]" << endl;
  cout << "Usage: [OPTION]" << endl;
  cout << "ssb secures file by creating a jigsaw puzzle out of if." << endl
      << endl;
  cout << " Main operation mode:" << endl << endl;
  cout << "    -e, --encypt   : perform encryption" << endl;
  cout << "    -d, --decrypt  : perform decryption" << endl << endl;
  cout << "    -r, --remove   : Remove file from SSB pool" << endl;
  cout << " Following option can be used when running in encryption mode"
      << endl << endl;
  cout << "    -c, --chunk    : take number of chunks to create" << endl
      << endl;
  cout << " Following options are to be used independently" << endl << endl;
  cout << "    -v, --version  : display verison information" << endl;
  cout << "    -l, --list     : list down all the encrypted files <default>"
      << endl << endl;
//    cout<<"  -output   : name of the output file"<<endl;
}

void
ssbCliInterface::printVersion ()
{
  cout << "Secure Storage Box version : " << SSB_CLI_VERSION << endl;
}

void
ssbCliInterface::copyArgValue (char* argVal, std::string& parm)
{
  if ((argVal != NULL) && argVal[0] != '-')
    { // make sure that the next arg is there and it is not command line option
      parm = argVal;
    }
  else
    _validationStatus = false;
}

bool
ssbCliInterface::parse_argv (int argc, char** argv)
{
  bool ret_val (true);
  int curr_param (0);
  for (short int i = 1; (i < argc) && (ret_val == true); i++)
    {
//        cout<<"argv["<<i<<"] : "<<argv[i]<<endl;
      _optList |= (curr_param = checkArg (argv[i]));
      if (ENCRYPT == curr_param)
        {
          copyArgValue (argv[++i], _inputFile);
        }
      else if (DECRYPT == curr_param)
        {
          copyArgValue (argv[++i], _outputFile );
        }
      else if (CHUNKS_COUNT == curr_param)
        {
          string num;
          copyArgValue (argv[++i], num);
          _chunks_count = StrToNum<int> (num);
        }
      else if (REMOVE == curr_param)
        {
          if (_outputFile.empty ())
            copyArgValue (argv[++i], _outputFile );
        }
      else if (0 == curr_param)
        _validationStatus = false;
    }
//    cout<<"_optList : "<<_optList<<endl;
//    cout<<"_input : "<<_inputFile<<endl;
//    cout<<"_output : "<<_outputFile<<endl;
  return true;
}

void
ssbCliInterface::initSupportedOpt ()
{
//    cout<<"List of supported opts will be initialised"<<endl;
  CliInterface::initSupportedOpt ();
  addOptSupport (OPT_ENCR, ENCRYPT);
  addOptSupport (OPT_DECR, DECRYPT);
  addOptSupport (OPT_REM, REMOVE);
  addOptSupport (OPT_CHUNKS, CHUNKS_COUNT);
  addOptSupport (OPT_LIST, LIST);
  addOptSupport (OPT_OUTPUT, OUTPUT);
}

// here will be the functions to determine the options

bool
ssbCliInterface::encrypt ()
{
  if ((ENCRYPT & _optList) == ENCRYPT)
    return true;
  return false;
}
bool
ssbCliInterface::decrypt ()
{
  if ((DECRYPT & _optList) == DECRYPT)
    return true;
  return false;
}
bool
ssbCliInterface::remove ()
{
  if (((REMOVE & _optList) == REMOVE) && (ENCRYPT & _optList) != ENCRYPT) // no effect when remove is used with encrypt
    return true;
  return false;
}
bool
ssbCliInterface::chunksCount ()
{
  if ((CHUNKS_COUNT & _optList) == CHUNKS_COUNT)
    return true;
  return false;
}
bool
ssbCliInterface::list ()
{
  if ((LIST & _optList) == LIST)
    return true;
  return false;
}
bool
ssbCliInterface::output ()
{
  if ((OUTPUT & _optList) == OUTPUT)
    return true;
  return false;
}

// Here are the functions returning commandline paramters

string
ssbCliInterface::inputFileArg ()
{
  return _inputFile;
}
string
ssbCliInterface::outputFileArg ()
{
  return _outputFile;
}
int
ssbCliInterface::count_arg ()
{
  return _chunks_count;
}
