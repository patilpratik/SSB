/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: key                                                                               */
/* Description	: This module manipulates the key in irriversible way.                              */
/*                i.e. given mutated key and seed, original key could not be derived                */
/* Input    	:   1) key                                                                          */
/*                  2) seed : number that controls the mutation.                                    */
/*                  3) num : total number of keys to make                                           */
/* Output   	: mutated key                                                                       */
/* Date	    	: 6 October 2014                                                                    */
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
#include "key.h"
#include <chrono>
#include <random>

using namespace std;
namespace SSB
{
  int
  rand ()
  {

    unsigned seed =
        std::chrono::system_clock::now ().time_since_epoch ().count ();
    std::mt19937_64 generator (seed); // mt19937 is a standard mersenne_twister_engine
    unsigned long num = generator ();

    //cout<<"Random : "<<num<<"\tIndex : ";
    return num;
  }

  KEY::KEY ()
  {

    _keySize = 0;
    _key = NULL;
// try
    _seed = 0;
//\\ try
  }

  KEY::KEY (string val, size_t key_size)
  {

    _keySize = key_size;
    _key = new byte[key_size + 1];
// try
    _seed = 0;
//\\ try
    memcpy (_key, val.c_str (), key_size + 1);
  }
  KEY::KEY (byte* val, size_t key_size)
  {

    _keySize = key_size;
    _key = new byte[key_size + 1];
// try
    _seed = 0;
//\\ try
    memcpy (_key, val, key_size + 1);
  }

  KEY::KEY (const KEY& obj)
  {

    _keySize = obj.size ();
    _key = new byte[_keySize + 1];
// try
    _seed = obj.seed ();
//\\ try
    memset (_key, '\0', _keySize + 1);
    memcpy (_key, obj.value ().c_str (), _keySize);

  }

  KEY::~KEY ()
  {

    if (_key != NULL)
      {
        delete[] _key;
        _key = NULL;
      }
  }
  string
  KEY::value () const
  {

    string val (reinterpret_cast<char const*> (_key), _keySize);
    return val;
  }

  const byte*
  KEY::bvalue () const
  {
    return _key;
  }
  size_t
  KEY::size () const
  {

    return _keySize;
  }
  const int
  KEY::seed () const
  {
    return _seed;
  }
  bool
  KEY::setSeed (int & seed)
  {

    _seed = seed;
  }
  bool
  KEY::clear ()
  {

    if (_keySize != 0)
      {
        delete _key;
        _key = NULL;
        _keySize = 0;
      }
    return true;
  }

  void
  KEY::operator= (const KEY& obj)
  {

    _keySize = obj.size ();
    _key = new byte[_keySize];
    memcpy (_key, obj.value ().c_str (), _keySize);
    _seed = obj.seed ();
  }
}
