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
// $Id: Expression.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef EXPRESSION_HXX
#define EXPRESSION_HXX

#include "bspf.hxx"

// define this to count Expression instances. Only useful for debugging
// Stella itself.
//#define EXPR_REF_COUNT

/**
  This class provides an implementation of an expression node, which
  is a construct that is given two other expressions and evaluates and
  returns the result.  When placed in a tree, a collection of such nodes
  can represent complex expression statements.

  @author  Stephen Anthony
  @version $Id: Expression.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class Expression
{
  public:
    Expression(Expression* lhs, Expression* rhs);
    virtual ~Expression();

    virtual uInt16 evaluate() const = 0;

  protected:
    Expression* myLHS;
    Expression* myRHS;
};

#endif
