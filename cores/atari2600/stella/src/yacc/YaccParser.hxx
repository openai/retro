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
// $Id: YaccParser.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef PARSER_HXX
#define PARSER_HXX

class Expression;

//#ifdef __cplusplus
//extern "C" {
//#endif

namespace YaccParser {
	int parse(const char *);
	Expression* getResult();
	const string& errorMessage();
}

//#ifdef __cplusplus
//}
//#endif

#endif
