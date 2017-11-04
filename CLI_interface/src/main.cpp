/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/*                This file will be used only in development phase to provide a stub to the         */
/*                CLI_interface                                                                     */
/* Module   	: main stub                                                                         */
/* Description	: used as a stub in CLI_interface development                                       */
/* Input    	:                                                                                   */
/* Output   	:                                                                                   */
/* Date	    	: 18 July 2014                                                                      */
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
#include "ssb_cli_interface.h"
using namespace std;
#if 0
int main( int argc, char** argv )
  {

    int ret_val(1);
    cout<<"In progress"<<endl;
    ssb_cli_interface * cli_opt = new ssb_cli_interface(argc,argv);
    cli_opt->parse_argv(argc, argv);
    if(cli_opt->is_valid())
      {
	if(cli_opt->help())
	cli_opt->print_help();
	if(cli_opt->version())
	cli_opt->print_version();

	if(cli_opt->encrypt())
	cout<<"encryption will be done."<<"input : "<<cli_opt->input_file_arg()<<endl<<"output : "<<cli_opt->output_file_arg()<<endl;
	if(cli_opt->decrypt())
	cout<<"decryption will be done."<<"input : "<<cli_opt->input_file_arg()<<endl<<"output : "<<cli_opt->output_file_arg()<<endl;
	if(cli_opt->list())
	cout<<"list of the files in box will be displayed."<<endl;
	if(cli_opt->rem_opt_support("-list"))
	cout<<"Removed list optin"<<endl;
	ret_val = 0;
      }
    else
      {
	cout<<"Invalid paramters"<<endl;
	cli_opt->print_help();
      }
    return ret_val;
  }
#endif
