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
// $Id: YaccParser.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

//#include "YaccParser.hxx"

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "Base.hxx"
#include "Expression.hxx"
#include "CartDebug.hxx"
#include "CpuDebug.hxx"
#include "TIADebug.hxx"

#include "DebuggerExpressions.hxx"

namespace YaccParser {
#include <stdio.h>
#include <ctype.h>

#include "y.tab.h"
yystype result;
string errMsg;
#include "y.tab.c"

const string& errorMessage() 
{
  return errMsg;
}

Expression* getResult()
{
  lastExp = 0;
  return result.exp;
}

const char *input, *c;

enum {
  ST_DEFAULT,
  ST_IDENTIFIER,
  ST_OPERATOR,
  ST_SPACE
};

int state = ST_DEFAULT;

//extern int yylval; // bison provides this

void setInput(const char *in)
{
  input = c = in;
  state = ST_DEFAULT;
}

int parse(const char *in)
{
  lastExp = 0;
  errMsg = "(no error)";
  setInput(in);
  return yyparse();
}

/* hand-rolled lexer. Hopefully faster than flex... */
inline bool is_base_prefix(char x)
{
  return ( (x=='\\' || x=='$' || x=='#') );
}

inline bool is_identifier(char x)
{
  return ( (x>='0' && x<='9') || 
           (x>='a' && x<='z') ||
           (x>='A' && x<='Z') ||
            x=='.' || x=='_'  );
}

inline bool is_operator(char x)
{
  return ( (x=='+' || x=='-' || x=='*' ||
            x=='/' || x=='<' || x=='>' ||
            x=='|' || x=='&' || x=='^' ||
            x=='!' || x=='~' || x=='(' ||
            x==')' || x=='=' || x=='%' ||
            x=='[' || x==']' ) );
}

// const_to_int converts a string into a number, in either the
// current base, or (if there's a base override) the selected base.
// Returns -1 on error, since negative numbers are the parser's
// responsibility, not the lexer's
int const_to_int(char *c) {
  // what base is the input in?
  Common::Base::Format format = Common::Base::format();

  switch(*c) {
    case '\\':
      format = Common::Base::F_2;
      c++;
      break;

    case '#':
      format = Common::Base::F_10;
      c++;
      break;

    case '$':
      format = Common::Base::F_16;
      c++;
      break;

    default: // not a base_prefix, use default base
      break;
  }

  int ret = 0;
  switch(format) {
    case Common::Base::F_2:
      while(*c) {
        if(*c != '0' && *c != '1')
          return -1;
        ret *= 2;
        ret += (*c - '0');
        c++;
      }
      return ret;

    case Common::Base::F_10:
      while(*c) {
        if(!isdigit(*c))
          return -1;
        ret *= 10;
        ret += (*c - '0');
        c++;
      }
      return ret;

    case Common::Base::F_16:
      while(*c) { // FIXME: error check!
        if(!isxdigit(*c))
          return -1;
        int dig = (*c - '0');
        if(dig > 9) dig = tolower(*c) - 'a' + 10;
        ret *= 16;
        ret += dig;
        c++;
      }
      return ret;

    default:
      fprintf(stderr, "INVALID BASE in lexer!");
      return 0;
  }
}

// special methods that get e.g. CPU registers
CPUDEBUG_INT_METHOD getCpuSpecial(char *c)
{
  if(BSPF_equalsIgnoreCase(c, "a"))
    return &CpuDebug::a;
  else if(BSPF_equalsIgnoreCase(c, "x"))
    return &CpuDebug::x;
  else if(BSPF_equalsIgnoreCase(c, "y"))
    return &CpuDebug::y;
  else if(BSPF_equalsIgnoreCase(c, "pc"))
    return &CpuDebug::pc;
  else if(BSPF_equalsIgnoreCase(c, "sp"))
    return &CpuDebug::sp;
  else if(BSPF_equalsIgnoreCase(c, "c"))
    return &CpuDebug::c;
  else if(BSPF_equalsIgnoreCase(c, "z"))
    return &CpuDebug::z;
  else if(BSPF_equalsIgnoreCase(c, "n"))
    return &CpuDebug::n;
  else if(BSPF_equalsIgnoreCase(c, "v"))
    return &CpuDebug::v;
  else if(BSPF_equalsIgnoreCase(c, "d"))
    return &CpuDebug::d;
  else if(BSPF_equalsIgnoreCase(c, "i"))
    return &CpuDebug::i;
  else if(BSPF_equalsIgnoreCase(c, "b"))
    return &CpuDebug::b;
  else
    return 0;
}

// special methods that get Cart RAM/ROM internal state
CARTDEBUG_INT_METHOD getCartSpecial(char *c)
{
  if(BSPF_equalsIgnoreCase(c, "_bank"))
    return &CartDebug::getBank;
  else if(BSPF_equalsIgnoreCase(c, "_rwport"))
    return &CartDebug::readFromWritePort;
  else
    return 0;
}

// special methods that get TIA internal state
TIADEBUG_INT_METHOD getTiaSpecial(char *c)
{
  if(BSPF_equalsIgnoreCase(c, "_scan"))
    return &TIADebug::scanlines;
  else if(BSPF_equalsIgnoreCase(c, "_fcount"))
    return &TIADebug::frameCount;
  else if(BSPF_equalsIgnoreCase(c, "_cclocks"))
    return &TIADebug::clocksThisLine;
  else if(BSPF_equalsIgnoreCase(c, "_vsync"))
    return &TIADebug::vsyncAsInt;
  else if(BSPF_equalsIgnoreCase(c, "_vblank"))
    return &TIADebug::vblankAsInt;
  else
    return 0;
}

int yylex() {
  static char idbuf[255];
  char o, p;
  yylval.val = 0;
  while(*c != '\0') {
    //fprintf(stderr, "looking at %c, state %d\n", *c, state);
    switch(state) {
      case ST_SPACE:
        yylval.val = 0;
        if(isspace(*c)) {
          c++;
        } else if(is_identifier(*c) || is_base_prefix(*c)) {
          state = ST_IDENTIFIER;
        } else if(is_operator(*c)) {
          state = ST_OPERATOR;
        } else {
          state = ST_DEFAULT;
        }

        break;

      case ST_IDENTIFIER:
        {
          CARTDEBUG_INT_METHOD cartMeth;
          CPUDEBUG_INT_METHOD  cpuMeth;
          TIADEBUG_INT_METHOD  tiaMeth;

          char *bufp = idbuf;
          *bufp++ = *c++; // might be a base prefix
          while(is_identifier(*c)) { // may NOT be base prefixes
            *bufp++ = *c++;
            //fprintf(stderr, "yylval==%d, *c==%c\n", yylval, *c);
          }
          *bufp = '\0';
          state = ST_DEFAULT;

          // Note: specials (like "a" for accumulator) have priority over
          // numbers. So "a" always means accumulator, not hex 0xa. User
          // is welcome to use a base prefix ("$a"), or a capital "A",
          // to mean 0xa.

          // Also, labels have priority over specials, so Bad Things will
          // happen if the user defines a label that matches one of
          // the specials. Who would do that, though?

          if(Debugger::debugger().cartDebug().getAddress(idbuf) > -1) {
            yylval.equate = idbuf;
            return EQUATE;
          } else if( (cpuMeth = getCpuSpecial(idbuf)) ) {
            yylval.cpuMethod = cpuMeth;
            return CPU_METHOD;
          } else if( (cartMeth = getCartSpecial(idbuf)) ) {
            yylval.cartMethod = cartMeth;
            return CART_METHOD;
          } else if( (tiaMeth = getTiaSpecial(idbuf)) ) {
            yylval.tiaMethod = tiaMeth;
            return TIA_METHOD;
          } else if( Debugger::debugger().getFunction(idbuf) != 0) {
            yylval.function = idbuf;
            return FUNCTION;
          } else {
            yylval.val = const_to_int(idbuf);
            if(yylval.val >= 0)
              return NUMBER;
            else
              return ERR;
          }
        }

      case ST_OPERATOR:
        o = *c++;
        if(!*c) return o;
        if(isspace(*c)) {
          state = ST_SPACE;
          return o;
        } else if(is_identifier(*c) || is_base_prefix(*c)) {
          state = ST_IDENTIFIER;
          return o;
        } else {
          state = ST_DEFAULT;
          p = *c++;
          //fprintf(stderr, "o==%c, p==%c\n", o, p);
          if(o == '>' && p == '=')
            return GTE;
          else if(o == '<' && p == '=')
            return LTE;
          else if(o == '!' && p == '=')
            return NE;
          else if(o == '=' && p == '=')
            return EQ;
          else if(o == '|' && p == '|')
            return LOG_OR;
          else if(o == '&' && p == '&')
            return LOG_AND;
          else if(o == '<' && p == '<')
            return SHL;
          else if(o == '>' && p == '>')
            return SHR;
          else {
            c--;
            return o;
          }
        }

        break;

      case ST_DEFAULT:
      default:
        yylval.val = 0;
        if(isspace(*c)) {
          state = ST_SPACE;
        } else if(is_identifier(*c) || is_base_prefix(*c)) {
          state = ST_IDENTIFIER;
        } else if(is_operator(*c)) {
          state = ST_OPERATOR;
        } else {
          yylval.val = *c++;
          return yylval.val;
        }
        break;
    }
  }

  //fprintf(stderr, "end of input\n");
  return 0; // hit NUL, end of input.
}


#if 0
int main(int argc, char **argv) {
  int l;

  set_input(argv[1]);
  while( (l = yylex()) != 0 )
    printf("ret %d, %d\n", l, yylval);

  printf("%d\n", yylval);
}
#endif
}

//#ifdef __cplusplus
//}
//#endif
