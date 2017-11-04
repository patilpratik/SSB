/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: file splitter                                                                     */
/* Description	: This module will devide given file in N number of files of equal size             */
/*                  if N is passed as 0, file will not be split.                                    */
/*                  if size of each sub file is less than limit, each sub file will be padded with  */
/*                      (01010101)                                                                  */
/* Input    	:   1) Path to the file/ inode to the file                                          */
/*                  2) N : number of sub files to be made                                           */
/* Output   	: A list inodes of subfiles.                                                        */
/* Date	    	: 21 August 2014                                                                    */
/* Author   	: Pratik Patil                                                                      */
//* License      :                                                                                   */
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
#include "ssb_file_splitter.h"

using namespace std;

