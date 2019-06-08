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
// $Id: DebuggerExpressions.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef DEBUGGER_EXPRESSIONS_HXX
#define DEBUGGER_EXPRESSIONS_HXX

#include "bspf.hxx"
#include "CartDebug.hxx"
#include "CpuDebug.hxx"
#include "TIADebug.hxx"
#include "Debugger.hxx"
#include "Expression.hxx"

/**
  All expressions currently supported by the debugger.
  @author  B. Watson and Stephen Anthony
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class BinAndExpression : public Expression
{
  public:
    BinAndExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() & myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class BinNotExpression : public Expression
{
  public:
    BinNotExpression(Expression* left) : Expression(left, 0) {}
    uInt16 evaluate() const
      { return ~(myLHS->evaluate()); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class BinOrExpression : public Expression
{
  public:
    BinOrExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() | myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class BinXorExpression : public Expression
{
  public:
    BinXorExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() ^ myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ByteDerefExpression : public Expression
{
  public:
    ByteDerefExpression(Expression* left): Expression(left, 0) {}
    uInt16 evaluate() const
      { return Debugger::debugger().peek(myLHS->evaluate()); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ByteDerefOffsetExpression : public Expression
{
  public:
    ByteDerefOffsetExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return Debugger::debugger().peek(myLHS->evaluate() + myRHS->evaluate()); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ConstExpression : public Expression
{
  public:
    ConstExpression(const int value) : Expression(0, 0), myValue(value) {}
    uInt16 evaluate() const
      { return myValue; }

  private:
    int myValue;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class CpuMethodExpression : public Expression
{
  public:
    CpuMethodExpression(CPUDEBUG_INT_METHOD method) : Expression(0, 0), myMethod(method) {}
    uInt16 evaluate() const
      { return CALL_CPUDEBUG_METHOD(myMethod); }

  private:
    CPUDEBUG_INT_METHOD myMethod;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class DivExpression : public Expression
{
  public:
    DivExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { int denom = myRHS->evaluate();
        return denom == 0 ? 0 : myLHS->evaluate() / denom; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class EqualsExpression : public Expression
{
  public:
    EqualsExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() == myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class EquateExpression : public Expression
{
  public:
    EquateExpression(const string& label) : Expression(0, 0), myLabel(label) {}
    uInt16 evaluate() const
      { return Debugger::debugger().cartDebug().getAddress(myLabel); }

  private:
    string myLabel;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class FunctionExpression : public Expression
{
  public:
    FunctionExpression(const string& label) : Expression(0, 0), myLabel(label) {}
    uInt16 evaluate() const
    {
      const Expression* exp = Debugger::debugger().getFunction(myLabel);
      if(exp) return exp->evaluate();
      else    return 0;
    }

  private:
    string myLabel;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class GreaterEqualsExpression : public Expression
{
  public:
    GreaterEqualsExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() >= myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class GreaterExpression : public Expression
{
  public:
    GreaterExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() > myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class HiByteExpression : public Expression
{
  public:
    HiByteExpression(Expression* left) : Expression(left, 0) {}
    uInt16 evaluate() const
      { return 0xff & (myLHS->evaluate() >> 8); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class LessEqualsExpression : public Expression
{
  public:
    LessEqualsExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() <= myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class LessExpression : public Expression
{
  public:
    LessExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() < myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class LoByteExpression : public Expression
{
  public:
    LoByteExpression(Expression* left) : Expression(left, 0) {}
    uInt16 evaluate() const
      { return 0xff & myLHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class LogAndExpression : public Expression
{
  public:
    LogAndExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() && myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class LogNotExpression : public Expression
{
  public:
    LogNotExpression(Expression* left) : Expression(left, 0) {}
    uInt16 evaluate() const
      { return !(myLHS->evaluate()); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class LogOrExpression : public Expression
{
  public:
    LogOrExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() || myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class MinusExpression : public Expression
{
  public:
    MinusExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() - myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ModExpression : public Expression
{
  public:
    ModExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { int rhs = myRHS->evaluate();
        return rhs == 0 ? 0 : myLHS->evaluate() % rhs; }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class MultExpression : public Expression
{
  public:
    MultExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() * myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class NotEqualsExpression : public Expression
{
  public:
    NotEqualsExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() != myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class PlusExpression : public Expression
{
  public:
    PlusExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() + myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class CartMethodExpression : public Expression
{
  public:
    CartMethodExpression(CARTDEBUG_INT_METHOD method) : Expression(0, 0), myMethod(method) {}
    uInt16 evaluate() const
      { return CALL_CARTDEBUG_METHOD(myMethod); }

  private:
    CARTDEBUG_INT_METHOD myMethod;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ShiftLeftExpression : public Expression
{
  public:
    ShiftLeftExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() << myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ShiftRightExpression : public Expression
{
  public:
    ShiftRightExpression(Expression* left, Expression* right) : Expression(left, right) {}
    uInt16 evaluate() const
      { return myLHS->evaluate() >> myRHS->evaluate(); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class TiaMethodExpression : public Expression
{
  public:
    TiaMethodExpression(TIADEBUG_INT_METHOD method) : Expression(0, 0), myMethod(method) {}
    uInt16 evaluate() const
      { return CALL_TIADEBUG_METHOD(myMethod); }

  private:
    TIADEBUG_INT_METHOD myMethod;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class UnaryMinusExpression : public Expression
{
  public:
    UnaryMinusExpression(Expression* left) : Expression(left, 0) {}
    uInt16 evaluate() const
      { return -(myLHS->evaluate()); }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class WordDerefExpression : public Expression
{
  public:
    WordDerefExpression(Expression* left) : Expression(left, 0) {}
    uInt16 evaluate() const
      { return Debugger::debugger().dpeek(myLHS->evaluate()); }
};

#endif
