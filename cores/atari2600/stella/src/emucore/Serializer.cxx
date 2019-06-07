//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: Serializer.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <fstream>
#include <sstream>

#include "Serializer.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Serializer::Serializer(const string& filename, bool readonly)
  : myStream(NULL),
    myUseFilestream(true)
{
  if(readonly)
  {
    //FilesystemNode node(filename);
    //if(node.isFile() && node.isReadable())
    {
      fstream* str = new fstream(filename.c_str(), ios::in | ios::binary);
      if(str && str->is_open())
      {
        myStream = str;
        myStream->exceptions( ios_base::failbit | ios_base::badbit | ios_base::eofbit );
        reset();
      }
      else
        delete str;
    }
  }
  else
  {
    // When using fstreams, we need to manually create the file first
    // if we want to use it in read/write mode, since it won't be created
    // if it doesn't already exist
    // However, if it *does* exist, we don't want to overwrite it
    // So we open in write and append mode - the write creates the file
    // when necessary, and the append doesn't delete any data if it
    // already exists
    fstream temp(filename.c_str(), ios::out | ios::app);
    temp.close();

    fstream* str = new fstream(filename.c_str(), ios::in | ios::out | ios::binary);
    if(str && str->is_open())
    {
      myStream = str;
      myStream->exceptions( ios_base::failbit | ios_base::badbit | ios_base::eofbit );
      reset();
    }
    else
      delete str;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Serializer::Serializer(void)
  : myStream(NULL),
    myUseFilestream(false)
{
  myStream = new stringstream(ios::in | ios::out | ios::binary);
  
  // For some reason, Windows and possibly OSX needs to store something in
  // the stream before it is used for the first time
  if(myStream)
  {
    myStream->exceptions( ios_base::failbit | ios_base::badbit | ios_base::eofbit );
    putBool(true);
    reset();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Serializer::~Serializer(void)
{
  if(myStream != NULL)
  {
    if(myUseFilestream)
      ((fstream*)myStream)->close();

    delete myStream;
    myStream = NULL;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Serializer::isValid(void)
{
  return myStream != NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::reset(void)
{
  myStream->clear();
  myStream->seekg(ios_base::beg);
  myStream->seekp(ios_base::beg);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 Serializer::getByte(void)
{
  char buf;
  myStream->read(&buf, 1);

  return buf;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::getByteArray(uInt8* array, uInt32 size)
{
  myStream->read((char*)array, size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 Serializer::getShort(void)
{
  uInt16 val = 0;
  myStream->read((char*)&val, sizeof(uInt16));

  return val;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::getShortArray(uInt16* array, uInt32 size)
{
  myStream->read((char*)array, sizeof(uInt16)*size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt32 Serializer::getInt(void)
{
  uInt32 val = 0;
  myStream->read((char*)&val, sizeof(uInt32));

  return val;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::getIntArray(uInt32* array, uInt32 size)
{
  myStream->read((char*)array, sizeof(uInt32)*size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Serializer::getString(void)
{
  int len = getInt();
  string str;
  str.resize(len);
  myStream->read(&str[0], len);

  return str;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Serializer::getBool(void)
{
  return getByte() == TruePattern;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putByte(uInt8 value)
{
  myStream->write((char*)&value, 1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putByteArray(const uInt8* array, uInt32 size)
{
  myStream->write((char*)array, size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putShort(uInt16 value)
{
  myStream->write((char*)&value, sizeof(uInt16));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putShortArray(const uInt16* array, uInt32 size)
{
  myStream->write((char*)array, sizeof(uInt16)*size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putInt(uInt32 value)
{
  myStream->write((char*)&value, sizeof(uInt32));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putIntArray(const uInt32* array, uInt32 size)
{
  myStream->write((char*)array, sizeof(uInt32)*size);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putString(const string& str)
{
  int len = str.length();
  putInt(len);
  myStream->write(str.data(), len);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Serializer::putBool(bool b)
{
  putByte(b ? TruePattern: FalsePattern);
}
