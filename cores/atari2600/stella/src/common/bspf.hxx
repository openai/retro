//============================================================================
//
//  BBBBB    SSSS   PPPPP   FFFFFF
//  BB  BB  SS  SS  PP  PP  FF
//  BB  BB  SS      PP  PP  FF
//  BBBBB    SSSS   PPPPP   FFFF    --  "Brad's Simple Portability Framework"
//  BB  BB      SS  PP      FF
//  BB  BB  SS  SS  PP      FF
//  BBBBB    SSSS   PP      FF
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: bspf.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef BSPF_HXX
#define BSPF_HXX

/**
  This file defines various basic data types and preprocessor variables
  that need to be defined for different operating systems.

  @author Bradford W. Mott
  @version $Id: bspf.hxx 2838 2014-01-17 23:34:03Z stephena $
*/

#include <stdint.h>

#if defined(_MSC_VER)
  // Types for 8-bit signed and unsigned integers
  typedef signed char Int8;
  typedef unsigned char uInt8;
  // Types for 16-bit signed and unsigned integers
  typedef signed short Int16;
  typedef unsigned short uInt16;
  // Types for 32-bit signed and unsigned integers
  typedef signed int Int32;
  typedef unsigned int uInt32;
  // Types for 64-bit signed and unsigned integers
  typedef __int64 Int64;
  typedef unsigned __int64 uInt64;
#else
  // Types for 8-bit signed and unsigned integers
  typedef int8_t Int8;
  typedef uint8_t uInt8;
  // Types for 16-bit signed and unsigned integers
  typedef int16_t Int16;
  typedef uint16_t uInt16;
  // Types for 32-bit signed and unsigned integers
  typedef int32_t Int32;
  typedef uint32_t uInt32;
  // Types for 64-bit signed and unsigned integers
  typedef int64_t Int64;
  typedef uint64_t uInt64;
#endif


// The following code should provide access to the standard C++ objects and
// types: cout, cerr, string, ostream, istream, etc.
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstring>
#include <cctype>
#include <cstdio>
using namespace std;

// Defines to help with path handling
#ifdef _MSC_VER
  #define BSPF_PATH_SEPARATOR  "\\"
#else
  #define BSPF_PATH_SEPARATOR  "/"
#endif

// CPU architecture type
// This isn't complete yet, but takes care of all the major platforms
#if defined(__i386__) || defined(_M_IX86)
  #define BSPF_ARCH "i386"
#elif defined(__x86_64__) || defined(_WIN64)
  #define BSPF_ARCH "x86_64"
#elif defined(__powerpc__) || defined(__ppc__)
  #define BSPF_ARCH "ppc"
#else
  #define BSPF_ARCH "NOARCH"
#endif

// I wish Windows had a complete POSIX layer
#ifdef _MSC_VER
  #define BSPF_snprintf _snprintf
  #define BSPF_vsnprintf _vsnprintf
#else
  #define BSPF_snprintf snprintf
  #define BSPF_vsnprintf vsnprintf
#endif

static const string EmptyString("");

//////////////////////////////////////////////////////////////////////
// Some convenience functions

template<typename T> inline void BSPF_swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
template<typename T> inline T BSPF_abs (T x) { return (x>=0) ? x : -x; }
template<typename T> inline T BSPF_clamp (T a, T l, T u) { return (a<l) ? l : (a>u) ? u : a; }

// Test whether two characters are equal (case insensitive)
static bool BSPF_equalsIgnoreCaseChar(char ch1, char ch2)
{
  return toupper((unsigned char)ch1) == toupper((unsigned char)ch2);
}

// Compare two strings, ignoring case
inline int BSPF_compareIgnoreCase(const string& s1, const string& s2)
{
#ifdef _MSC_VER
  return _stricmp(s1.c_str(), s2.c_str());
#else
  return strcasecmp(s1.c_str(), s2.c_str());
#endif
}
inline int BSPF_compareIgnoreCase(const char* s1, const char* s2)
{
#ifdef _MSC_VER
  return _stricmp(s1, s2);
#else
  return strcasecmp(s1, s2);
#endif
}

// Test whether the first string starts with the second one (case insensitive)
inline bool BSPF_startsWithIgnoreCase(const string& s1, const string& s2)
{
#ifdef _MSC_VER
  return _strnicmp(s1.c_str(), s2.c_str(), s2.length()) == 0;
#else
  return strncasecmp(s1.c_str(), s2.c_str(), s2.length()) == 0;
#endif
}
inline bool BSPF_startsWithIgnoreCase(const char* s1, const char* s2)
{
#ifdef _MSC_VER
  return _strnicmp(s1, s2, strlen(s2)) == 0;
#else
  return strncasecmp(s1, s2, strlen(s2)) == 0;
#endif
}

// Test whether two strings are equal (case insensitive)
inline bool BSPF_equalsIgnoreCase(const string& s1, const string& s2)
{
  return BSPF_compareIgnoreCase(s1, s2) == 0;
}

// Find location (if any) of the second string within the first,
// starting from 'startpos' in the first string
inline size_t BSPF_findIgnoreCase(const string& s1, const string& s2, int startpos = 0)
{
  string::const_iterator pos = std::search(s1.begin()+startpos, s1.end(),
    s2.begin(), s2.end(), BSPF_equalsIgnoreCaseChar);
  return pos == s1.end() ? string::npos : pos - (s1.begin()+startpos);
}

// Test whether the first string ends with the second one (case insensitive)
inline bool BSPF_endsWithIgnoreCase(const string& s1, const string& s2)
{
  if(s1.length() >= s2.length())
  {
    const char* end = s1.c_str() + s1.length() - s2.length();
    return BSPF_compareIgnoreCase(end, s2.c_str()) == 0;
  }
  return false;
}

// Test whether the first string contains the second one (case insensitive)
inline bool BSPF_containsIgnoreCase(const string& s1, const string& s2)
{
  return BSPF_findIgnoreCase(s1, s2) != string::npos;
}

#endif
