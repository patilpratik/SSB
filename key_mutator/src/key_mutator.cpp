/****************************************************************************************************/
/*                                                                                                  */
/* Application	: This application is intended to provide secure box for putting up your stuff.     */
/*                The basic idea behind this app is to split the file in multiple chuncks and       */
/*                encrypt each chunk with a key derived from the key of the previous.               */
/* Module   	: key mutator                                                                       */
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
#include "key_mutator.h"
#include <sstream>

using namespace std;
namespace SSB
{
  KeyMutator::KeyMutator (size_t key_size)
  {

    //cout<<"default constructor"<<endl;
    clearContainer ();
    _size = 0;
    _counter = 0;
    _keySize = key_size;
    // _next_key.clear();
  }

  KeyMutator::KeyMutator (const string& password, int size, size_t key_size)
  {

    //cout<<"parameterized constructor"<<endl;
    clearContainer ();
    _size = size;
    _counter = 0;
    _keySize = key_size;
    //_next_key = _keyContainer.end();
    initKeyContainer (password);
  }
  /*
   KeyMutator::KeyMutator( const KeyMutator &obj){

   cout<<"Copy constructor"<<endl;
   _size = obj.size();
   _counter = 0;
   for (map<int,KEY>::iterator it = (obj.getKeySet()).begin(); it!= (obj.getKeySet()).end(); it++)
   _keyContainer[it->first] = it->second;
   }
   */
  /*map<int,KEY>*/std::stack<KEY>
  KeyMutator::getKeySet () const
  {

    return _keyContainer;
  }

  int
  KeyMutator::size () const
  {

    return _size;
  }

  bool
  KeyMutator::clearContainer ()
  {

    //_keyContainer.clear();
    for (int i = _keyContainer.size (); i > 0; i--)
      _keyContainer.pop ();
    return (_keyContainer.empty ());        // confirm if the container is empty
  }

///
/// This method generates a KEY from given password. The most crud way of doing it is to take a Hash of the password.
/// For now, this method will be used.
///
  KEY
  KeyMutator::makeKey (const string& pwd)
  {

    //byte *temp_key = new byte[_keySize];
    //cout<<"Key will be made here by the use of password."<<endl;
    //cout<<"Here"<<endl;
    CryptoPPWrapper crypt_obj;
#if 0
    //memset( &key, 0x00,SSB_DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/ );
    memset( temp_key, 0x00, sizeof(byte)*_keySize );
    //memcpy( temp_key,"Firstkey" , sizeof(byte)*_keySize );
    KEY temp(temp_key,_keySize);
#else 
    string hash = crypt_obj.sha (pwd, _keySize);
    KEY temp (hash, _keySize);
#endif
    //cout<<"returning key : "<<temp.value()<<endl;
    return temp;
  }
  void
  KeyMutator::initKeyContainer (const string& pwd)
  {

    if (pwd.empty ())
      return;
    //cout<<"Initialization of the key container is done here"<<endl;
    KEY new_key;
    new_key = makeKey (pwd);                  // Initializeing key from password
    //cout<<"got the key"<<endl;
    KEY prev_key;
    int seed;
    for (int i = 0; i < _size;)
      {

        if (isValidKey (new_key))
          {                     // Confirming that key does not starts with null
            seed = rand ();                      // generating seed for mutation
            new_key.setSeed (seed);
//try
//            _keyContainer[seed] = new_key;                     // pairing seed and key for mutation
            _keyContainer.push (new_key);
//\\try
            prev_key = new_key;
            i++;
          }
//        else
        //          delete new_key;                                     // Key is not valid
        //cout<<"KEY "<<i<<": "<<new_key.value()<<"seed : "<<new_key.seed()<<endl;
        new_key = getNextKey (prev_key, seed);          // obtaining mutated key
      }       // here the last key will also be associated with garbage seed.
    //cout<<"Key container is initialised."<<endl;
    // try _next_key = _keyContainer.end();
  }
  void
  KeyMutator::initKeyContainer (const string& pwd, int size)
  {

    clearContainer ();
    _size = size;
    initKeyContainer (pwd);
  }

///
/// This method will return a mutated key. The most crud way of doing it is to XOR the key data with the given seed.
/// For now this method will be used.
/// 
  KEY
  KeyMutator::getNextKey (KEY& key, int seed)
  {

    CryptoPPWrapper c_obj;
    stringstream seed_str;
    seed_str << seed;
#if 0
    KEY new_key("",DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/);
    //memset( &new_key, 0x00,SSB_DEFAULT_KEYLENGTH /*CryptoPP::AES::DEFAULT_KEYLENGTH*/ );
    //memcpy( new_key,&seed , sizeof(byte)*_keySize );
#else
    KEY new_key (c_obj.sha (key.value () + seed_str.str (), key.size ()),
                 key.size ());
#endif 
    //cout<<"Key mutation will be done here in getNextKey() method"<<endl;
    return new_key;
  }
#if 0
// try
  const std::map<int/*random seed*/,KEY /*key*/>::iterator KeyMutator::next()
    {

      if(_counter >= _size)
      cerr<<"no more keys to provide"<<endl;
      else
        {
          if( _keyContainer.end() == _next_key)
          _next_key = _keyContainer.begin();            // retrive the first key
          else
          _next_key ++;// increment the iterator to point to the next key-seed pair
          _counter ++; // increment the counter.

          cout<<"_counter : "<<_counter<<endl;
          return _next_key;
        }
      return _keyContainer.end();
    }
#endif 

  KEY
  KeyMutator::next ()
  {
    KEY key = _keyContainer.top ();
    _keyContainer.pop ();
    return key;
  }
//\\try
  bool
  KeyMutator::isValidKey (KEY& key)
  {

    //cout<<"Do the validation processing"<<endl;
    return true;
  }
}
