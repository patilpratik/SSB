/****************************************************************************************************/
/*                                                                                                  */
/* Application  : This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module       : file_splitter                                                                     */
/* Description  : this file is a stub for development                                               */
/* Input        :                                                                                   */
/* Output       :                                                                                   */
/* Date         : 21 August 2014                                                                    */
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
#include <vector>
#include "file_splitter/ssb_file_splitter.h"
#include "file_splitter/ssb_file.h"
#include "file_splitter/ssb_pool.h"
#include "CLI_interface/ssb_cli_interface.h"

/*!	\mainpage This document describes the detailed low level design of Secure Storage Box 1.0.0
 <dl>
 <dt> This application is intended to provide secure box for putting up your stuff.
 <dt>The basic idea behind this app is to split the file in multiple chuncks and encrypt each chunk with a key derived from the key of the previous. 
 </dl>
 <dl>
 <dt> The key mutation implemented is one way. It implies that key will not lead to the previous key.
 </dl>
 <dl>
 <dt>How to use this Utility

 <dt> Usage: [OPERATION MODE] [file]
 <dt> Usage: [OPTION]
 </dl>

 <dl>
 <dt> Main operation mode
 <dt>     -e, --encypt   : perform encryption
 <dt>     -d, --decrypt  : perform decryption
 <dt>     -r, --remove   : remove file from SSB pool
 </dl>
 <dl>
 <dt> Following option can be used when running in encryption mode
 <dt>     -c, --chunk    : take number of chunks to create
 </dl>
 <dl>
 <dt> Following options are to be used independently
 <dt>     -v, --version  : display verison information
 <dt>     -l, --list     : list down all the encrypted files
 </dl>
 */

using namespace std;
using namespace SSB;
bool
is_dir (string& path)
{

  bool status (false);
  struct stat buffer;
  if (stat (path.c_str (), &buffer) == 0)
    if (S_ISDIR(buffer.st_mode))
      status = true;

  return status;

}

string
getTarFileName (string & dir)
{

  string dest (dir);
  dest.append (".tar");
  cout << dest << " will be created here " << endl;
  return dest;
}
string
createTar (string& dir)
{

  string dest (getTarFileName (dir));
  string command ("tar -czvf ");
  command += dest + " " + dir;
  system (command.c_str ());
  return dest;
}
//SSB_SETTING ssb_setting;
#if 1

void
getConfirmation (string& choice)
{
  cout << "The file is not found in your filesystem." << endl;
  cout << "Continue only if you have restored the file from SSB" << endl;
  cout << "To continue, press \"y\", else press \"n\" : ";
  cin.clear ();
  cin >> choice;
  cin.clear ();
}

int
main (int argc, char** argv)
{

  int ret_val (1);

  ///
  /// Todo : Make sure that single instance is running
  ///
  ssbCliInterface cmd_opt (argc, argv);
//    cmd_opt.rem_opt_support(OPT_LIST);
  string file_name;
  if (cmd_opt.isValid () == true)
    {

      SsbPool* pool_obj = SsbPool::getPool (); // get the pool to make sure it is not freed in between

      bool removeTar (false);
      if (cmd_opt.help ())
        {
          cmd_opt.printHelp ();
        }
      if (cmd_opt.version () == true)
        {
          cout << "Secure Storage Box Version : " << SSB_VERSION << endl;
        }
      if (cmd_opt.encrypt () == true)
        {
          file_name.clear ();
          file_name = cmd_opt.inputFileArg ();
///
/// \todo support dir.
/// 1) check if input file is dir,
/// 2) if yes, then create tar file of that dir in a /tmp/.ssb folder
/// 3) set file_name to the tar file.
///
          if (is_dir (file_name))
            {
              file_name = createTar (file_name);
              removeTar = true;
            }

          SsbFile file1 (file_name);
          file1.open ();
          if (!file1.good ())
            {
              cerr << "file not ready for use" << endl;
            }
          if (file1.openErr ())
            {
              cerr << "file1 is not opened" << endl;
            }
          else
            {
              if (file1.split (cmd_opt.count_arg ()))
                cout << "file is split" << endl;
              else
                cerr << "Error while splitting the file." << endl;
              file1.close ();
            }
          if (removeTar)
            {
              remove (file_name.c_str ());
            }
        }
      else if (cmd_opt.decrypt () == true)
        {
          file_name.clear ();
          file_name = cmd_opt.outputFileArg ();
          //cout<<"file "<<file_name<<"will be decrypted"<<endl;
///
/// \todo support dir.
/// 1) check if input file is dir,
/// 2) if yes, then create tar file of that dir in a /tmp/.ssb folder
/// 3) set file_name to the tar file.
///
          if (is_dir (file_name))
            {
              file_name = getTarFileName (file_name);
            }
          SsbFile file2 (file_name);
          /*file2.open(); // No need to open the file while decrypting as the actual file is supposed to be removed by user.
           if(!file2.good()){
           cerr<<"file not ready for use"<<endl;
           if(file2.open_err())
           cerr<<"file2 is not opened"<<endl;
           }else{*/
          if (file2.stitch ())
            cout << file_name << " is stitched again" << endl;
          else
            cerr << "Error while stitching file." << endl;
          /*}*/

        }
      else if (cmd_opt.list () == true)
        {
          pool_obj->printFiles ();
        }
      else if (cmd_opt.remove () == true)
        {
          file_name.clear ();
          file_name = cmd_opt.outputFileArg ();
          cout << file_name << " will be removed" << endl;
          SsbFile file (file_name);
          string choice ("y");
          file.open ();
          if (!file.good ())
            {
              // file is not present in the file system. Indicates it is not restored from SSB.
              getConfirmation (choice); // gets confirmation from user about deleting the file from ssb
            }
          if (choice.compare ("y") == 0)
            {
              if (file.remove ())
                cout << file_name << " is removed" << endl;
              else
                cerr << "Error while removing file." << endl;
              file.close ();
            }
        }

      /*
       ssb_pool* pool_obj = ssb_pool::get_pool();            // get the pool to make sure it is not freed in between
       SsbFile file1("temp/dmesg.txt");
       file1.open();
       if(!file1.good()){
       cerr<<"file not ready for use"<<endl;
       if(file1.open_err())
       cerr<<"file1 is not opened"<<endl;
       }else{
       file1.split(200000000);
       cout<<"file is split"<<endl;
       file1.close();
       }
       SsbFile file2("temp/dmesg.txt");
       file2.open();
       if(!file2.good()){
       cerr<<"file not ready for use"<<endl;
       if(file2.open_err())
       cerr<<"file2 is not opened"<<endl;
       }else{
       file2.stitch();
       cout<<"File2 is stitched again"<<endl;
       }
       */
      pool_obj->freePool ();
    }
  else
    {
      cout << "Invalid paramter" << endl;
      cmd_opt.printHelp ();
    }

  return ret_val;

}
#endif
