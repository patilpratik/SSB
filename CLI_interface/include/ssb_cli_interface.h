/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: ssb_cli_interface                                                                 */
/* Description	: This module provides the basic command line options handling.                     */
/*                The opts are                                                                      */
/*                  1) help     : print help                                                        */
/*                  2) version  : display verison information                                       */
/*                  3) encypt   : perform encryption                                                */
/*                  4) decrypt  : perform decryption                                                */
/*                  5) file     : take input from given file                                        */
/*                  6) console  : take input from console                                           */
/*                  7) list     : list down all the encrypted files                                 */
/*                  8) output   : name of the output file                                           */
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

#include "A_CLI_Interface.h"

namespace SSB
{
#ifndef SSB_CLI_INTERFACE_H
#define SSB_CLI_INTERFACE_H


#define SSB_VERSION "1.0.0"
///
/// Options signaling encryption
///
#define ENCRYPT 0x00000100
#define OPT_ENCR "--encrypt"

///
/// Option signaling decryption
///
#define DECRYPT 0x00001000
#define OPT_DECR "--decrypt"

///
/// Take input from the file. the file will be replace with the encrypted or decrypted file
///
#define REMOVE 0x00010000
#define OPT_REM    "--remove"

///
/// Take number of chunks to be made
///
#define CHUNKS_COUNT 0x00100000
#define OPT_CHUNKS "--chunks"

///
/// List down all the files available with secure box
///
#define LIST 0x01000000
#define OPT_LIST "--list"

///
/// Name of the output file to produce. if used with -file option, input file will remain untouched. 
///
#define OUTPUT 0x10000000
#define OPT_OUTPUT "--output"

#define SSB_CLI_VERSION "0.0.0-0"

///
/// \brief This class provides the basic command line options
///

/// Options can be added or removed by inheriting this class and using provided methods
/// Inputs :
///         1) argc : number of total paramters
///         2) argv : memory address of the parameter list
///
///                The opts are
///                  1) help     : print help                
///                  2) version  : display verison information
///                  3) encypt   : perform encryption
///                  4) decrypt  : perform decryption
///                  5) file     : take input from given file
///                  6) chunk    : take number of chunks to create
///                  7) list     : list down all the encrypted files
///                  8) output   : name of the output file                 
  class ssbCliInterface : public CliInterface
  {
  public:
    ssbCliInterface ();
    ///
    /// Construction of this object will do all checking of the options
    ///
    ssbCliInterface (int argc, char** argv);
    /// Prints a help menu. Child can override this function
    /// \todo help menu yet to be printed
    virtual void
    printHelp ();
    ///
    /// Prints version information
    ///
    virtual void
    printVersion ();
    ///
    /// Returns true if -e used
    ///
    bool
    encrypt ();
    ///
    /// Returns true if -d used
    ///
    bool
    decrypt ();
    ///
    /// true if -r used
    ///
    bool
    remove ();
    ///
    /// true if -c used
    ///
    bool
    chunksCount ();
    ///
    /// true if -l used
    ///
    bool
    list ();
    ///
    /// true if -o used
    ///
    bool
    output ();
    ///
    /// returns arg given after -f option : input file name
    ///
    std::string
    inputFileArg ();
    ///
    /// returns arg given after -o option : output file name
    ///
    std::string
    outputFileArg ();
    ///
    /// returns argument passed to -chunks option
    ///
    int
    count_arg ();
    ///
    ///Parses the commandline parameters
    ///
    bool
    parse_argv (int, char**);
  protected:

    ///
    /// stores the input file name
    ///
    std::string _inputFile;
    ///
    /// Stores the count of chunks to be created
    ///
    int _chunks_count;
    ///
    /// stores the output file name
    ///
    std::string _outputFile;
    ///
    /// Populates _supported_opt with default values
    ///
    virtual void
    initSupportedOpt ();

  private:
    void
    copyArgValue (char* argVal, std::string& parm);
  };
}
#endif
