/*****************************************************************************************************/
/*                                                                                                   */
/* Application        : This application is intended to provide secure box for putting up your stuff.*/
/*                The basic idea behind this app is to split the file in multiple chuncks and        */
/*                encrypt each chunk with a key derived from the key of the previous.                */
/* Module           : ssb_error                                                                      */
/* Description        :                                                                              */
/* Input            : File path                                                                      */
/* Output           :                                                                                */
/* Date                    : 05 April 2015                                                           */
/* Author           : Pratik Patil                                                                   */
/* License      :                                                                                    */
/*               Secure Storeage Box Version 1.0.0                                                   */
/*               Copyright (C) 2017  Pratik Patil                                                    */
/*                                                                                                   */
/*               This program is free software: you can redistribute it and/or modify                */
/*               it under the terms of the GNU General Public License Version 3 as published by      */
/*               the Free Software Foundation.                                                       */
/*                                                                                                   */
/*               This program is distributed in the hope that it will be useful,                     */
/*               but WITHOUT ANY WARRANTY; without even the implied warranty of                      */
/*               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       */
/*               GNU General Public License for more details.                                        */
/*                                                                                                   */
/*               You should have received a copy of the GNU General Public License                   */
/*               along with this program.  If not, see <http://www.gnu.org/licenses/>.               */
/*****************************************************************************************************/

#ifndef SSB_ERROR_H
#define SSB_ERROR_H

namespace SSB
{
#ifndef SSB_SPLIT_FILE
#define SSB_SPLIT_FILE  0x000000000
#endif
#define SSB_OPEN_ERR    0x000000001
#define SSB_IO_ERR      0x000000010
#define SSB_CRYPT_ERR   0x000000100
#define SSB_SPLIT_ERR   0x000001000
#define SSB_DEL_ERR     0x000010000
#define SSB_EMPTY_OBJ   0x000100000
#define SSB_POOL_ERR    0x001000000
#define SSB_PWD_ERR     0x010000000
#define SSB_CLOSE_ERR   0x100000000
}
#endif
