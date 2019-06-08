#ifndef __MDFN_STREAM_H
#define __MDFN_STREAM_H

// TODO/WIP

// TODO?: BufferedStream, no virtual functions, yes inline functions, constructor takes a Stream* argument.

#include "mednafen.h"
#include <errno.h>

#include <stdio.h>	// For SEEK_* defines, which we will use in Stream out of FORCE OF HABIT.

#define MODE_READ       0
#define MODE_WRITE      1
#define MODE_WRITE_SAFE 2

class Stream
{
 public:

 Stream();
 virtual ~Stream();

 enum
 {
  ATTRIBUTE_READABLE = 0,
  ATTRIBUTE_WRITEABLE,
  ATTRIBUTE_SEEKABLE
 };
 virtual uint64 attributes(void) = 0;

 virtual uint64 read(void *data, uint64 count, bool error_on_eos = true) = 0;
 virtual void write(const void *data, uint64 count) = 0;

 virtual void seek(int64 offset, int whence) = 0;
 virtual int64 tell(void) = 0;
 virtual int64 size(void) = 0;
 virtual void close(void) = 0;	// Flushes(in the case of writeable streams) and closes the stream.
				// Necessary since this operation can fail(running out of disk space, for instance),
				// and throw an exception in the destructor would be a Bad Idea(TM).
				//
				// Manually calling this function isn't strictly necessary, but recommended when the
				// stream is writeable; it will be called automatically from the destructor, with any
				// exceptions thrown caught and logged.

 //
 // Utility functions(TODO):
 //
 INLINE uint8 get_u8(void)
 {
  uint8 ret;

  read(&ret, sizeof(ret));

  return ret;
 }

 INLINE void put_u8(uint8 c)
 {
  write(&c, sizeof(c));
 }


 template<typename T>
 INLINE T get_NE(void)
 {
  T ret;

  read(&ret, sizeof(ret));

  return ret;
 }

 template<typename T>
 INLINE void put_NE(T c)
 {
  write(&c, sizeof(c));
 }


 template<typename T>
 INLINE T get_RE(void)
 {
  uint8 tmp[sizeof(T)];
  T ret = 0;

  read(tmp, sizeof(tmp));

  for(unsigned i = 0; i < sizeof(T); i++)
   ret |= (T)tmp[i] << (i * 8);

  return ret;
 }

 template<typename T>
 INLINE void put_RE(T c)
 {
  uint8 tmp[sizeof(T)];

  for(unsigned i = 0; i < sizeof(T); i++)
   tmp[i] = ((uint8 *)&c)[sizeof(T) - 1 - i];

  write(tmp, sizeof(tmp));
 }

 template<typename T>
 INLINE T get_LE(void)
 {
#ifdef MSB_FIRST
    return get_RE<T>();
#else
    return get_NE<T>();
#endif
 }

 template<typename T>
 INLINE void put_LE(T c)
 {
#ifdef MSB_FIRST
    return put_RE<T>(c);
#else
    return put_NE<T>(c);
#endif
 }

 template<typename T>
 INLINE T get_BE(void)
 {
#ifdef MSB_FIRST
    return get_NE<T>();
#else
    return get_RE<T>();
#endif
 }

 template<typename T>
 INLINE void put_BE(T c)
 {
#ifdef MSB_FIRST
    return put_NE<T>(c);
#else
    return put_RE<T>(c);
#endif
 }

 // Reads a line into "str", overwriting its contents; returns the line-end char('\n' or '\r' or '\0'), or -1 on EOF.
 // The line-end char won't be added to "str".
 // It's up to the caller to handle extraneous empty lines caused by DOS-format text lines(\r\n).
 // ("str" is passed by reference for the possibility of improved performance by reusing alloced memory for the std::string, though part
 //  of it would be up to the STL implementation).
 // Implemented as virtual so that a higher-performance version can be implemented if possible(IE with MemoryStream)
 virtual int get_line(std::string &str);
};
#endif
