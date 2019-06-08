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
// $Id: Rect.hxx 2838 2014-01-17 23:34:03Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef RECT_HXX
#define RECT_HXX

#include <cassert>

#include "bspf.hxx"

namespace GUI {

/*
  This small class is an helper for position and size values.
*/
struct Point
{
  int x;  //!< The horizontal part of the point
  int y;  //!< The vertical part of the point

  Point() : x(0), y(0) {};
  Point(const Point & p) : x(p.x), y(p.y) {};
  explicit Point(int x1, int y1) : x(x1), y(y1) {};
  Point(const string& p) {
    char c = '\0';
    x = y = -1;
    istringstream buf(p);
    buf >> x >> c >> y;
    if(c != 'x')
      x = y = 0;
  }
  Point & operator=(const Point & p) { x = p.x; y = p.y; return *this; };
  bool operator==(const Point & p) const { return x == p.x && y == p.y; };
  bool operator!=(const Point & p) const { return x != p.x || y != p.y; };

  friend ostream& operator<<(ostream& os, const Point& p) {
    os << p.x << "x" << p.y;
    return os;
  }
};

struct Size
{
  int w;  //!< The width part of the size
  int h;  //!< The height part of the size

  Size() : w(0), h(0) {};
  Size(const Size & s) : w(s.w), h(s.h) {};
  explicit Size(int w1, int h1) : w(w1), h(h1) {};
  Size(const string& s) {
    char c = '\0';
    w = h = -1;
    istringstream buf(s);
    buf >> w >> c >> h;
    if(c != 'x')
      w = h = -1;
  }
  Size & operator=(const Size & s) { w = s.w; h = s.h; return *this; };
  bool operator==(const Size & s) const { return w == s.w && h == s.h; };
  bool operator!=(const Size & s) const { return w != s.w || h != s.h; };

  friend ostream& operator<<(ostream& os, const Size& s) {
    os << s.w << "x" << s.h;
    return os;
  }
};

/*
  This small class is an helper for rectangles.
  Note: This implementation is built around the assumption that (top,left) is
  part of the rectangle, but (bottom,right) is not! This is reflected in 
  various methods, including contains(), intersects() and others.

  Another very wide spread approach to rectangle classes treats (bottom,right)
  also as a part of the rectangle.

  Coneptually, both are sound, but the approach we use saves many intermediate
  computations (like computing the height in our case is done by doing this:
    height = bottom - top;
  while in the alternate system, it would be
    height = bottom - top + 1;

  When writing code using our Rect class, always keep this principle in mind!
*/
struct Rect
{
  int top, left;        //!< The point at the top left of the rectangle (part of the rect).
  int bottom, right;    //!< The point at the bottom right of the rectangle (not part of the rect).

  Rect() : top(0), left(0), bottom(0), right(0) {}
  Rect(int w, int h) : top(0), left(0), bottom(h), right(w) {}
  Rect(int x1, int y1, int x2, int y2) : top(y1), left(x1), bottom(y2), right(x2)
  {
    assert(isValidRect());
  }

  int x() const { return left; }
  int y() const { return top; }
  Point point() const { return Point(x(), y()); }

  int width() const  { return right - left; }
  int height() const { return bottom - top; }
  Size size() const  { return Size(width(), height()); }

  void setWidth(int aWidth)   { right = left + aWidth; }
  void setHeight(int aHeight) { bottom = top + aHeight; }
  void setSize(const Size& size) { setWidth(size.w); setHeight(size.h); }

  /*
    @param x the horizontal position to check
    @param y the vertical position to check	

    @return true if the given position is inside this rectangle, false otherwise
  */
  bool contains(int x, int y) const {
    return (left <= x) && (x < right) && (top <= y) && (y < bottom);
  }

  /*
    @param p the point to check

    @return true if the given point is inside this rectangle, false otherwise
  */
  bool contains(const Point & p) const { return contains(p.x, p.y); }

  /*
    @param r the rectangle to check

    @return true if the given rectangle is inside the rectangle, false otherwise
  */
  bool intersects(const Rect & r) const {
    return (left < r.right) && (r.left < right) && (top < r.bottom) && (r.top < bottom);
  }

  /*
    @param r the rectangle to extend by
  */
  void extend(const Rect & r) {
    left   = MIN(left, r.left);
    right  = MAX(right, r.right);
    top    = MIN(top, r.top);
    bottom = MAX(bottom, r.bottom);
  }

  /*
    Extend this rectangle in all four directions by the given number of pixels

    @param offset the size to grow by
  */
  void grow(int offset) {
    top -= offset;
    left -= offset;
    bottom += offset;
    right += offset;
  }

  void clip(const Rect & r) {
    assert(isValidRect());
    assert(r.isValidRect());

    if (top < r.top) top = r.top;
    else if (top > r.bottom) top = r.bottom;

    if (left < r.left) left = r.left;
    else if (left > r.right) left = r.right;

    if (bottom > r.bottom) bottom = r.bottom;
    else if (bottom < r.top) bottom = r.top;

    if (right > r.right) right = r.right;
    else if (right < r.left) right = r.left;
  }

  void clip(int maxw, int maxh) {
    clip(Rect(0, 0, maxw, maxh));
  }

  bool isValidRect() const {
    return (left <= right && top <= bottom);
  }

  void moveTo(int x, int y) {
    bottom += y - top;
    right += x - left;
    top = y;
    left = x;
  }

  void moveTo(const Point & p) {
    moveTo(p.x, p.y);
  }

  friend ostream& operator<<(ostream& os, const Rect& r) {
    os << "Point: " << r.point() << ", Size: " << r.size();
    return os;
  }
};

}  // End of namespace GUI

#endif
