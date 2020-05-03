#ifndef __LOG_H__
#define __LOG_H__

#define LOG_NONE	0
#define LOG_ERROR   1
#define LOG_MINIMAL	2
#define LOG_WARNING 3
#define LOG_VERBOSE 4
#define LOG_APIFUNC 5

#define LOG_LEVEL LOG_WARNING

#if LOG_LEVEL > 0

#include "Types.h"
#include <cstdio>
#include <string>

// for strrchr
#include <string.h>

#ifdef OS_WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif //OS_WINDOWS

#define LOG(...) LogDebug(__FILENAME__, __LINE__, __VA_ARGS__)

void LogDebug(const char* _fileName, int _line, u16 _type, const char* _format, ...);

#else

#define LOG(A, ...)

#endif

#if defined(OS_WINDOWS) && !defined(MINGW)
void debugPrint(const char * format, ...);
#else
#define debugPrint(A, ...)
#endif

#endif
