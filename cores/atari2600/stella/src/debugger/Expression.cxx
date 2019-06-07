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
// $Id: Expression.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "Expression.hxx"

#ifdef EXPR_REF_COUNT
#include "bspf.hxx"
    int refCount = 0;
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Expression::Expression(Expression* lhs, Expression* rhs)
  : myLHS(lhs),
    myRHS(rhs)
{
#ifdef EXPR_REF_COUNT
  refCount++;
  cerr << "new Expression::Expression()" << endl;
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Expression::~Expression()
{
#ifdef EXPR_REF_COUNT
  refCount--;
  cerr << "~Expression::Expression()" << endl;
#endif
  delete myLHS;
  delete myRHS;
}
