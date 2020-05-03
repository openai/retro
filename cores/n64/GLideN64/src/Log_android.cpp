#include "Log.h"
#include <string.h>
#include <android/log.h>
#include <vector>
#include <sstream>

void LogDebug(const char* _fileName, int _line, u16 _type, const char* _format, ...) {
	static android_LogPriority androidLogTranslate[] = {
			ANDROID_LOG_SILENT,
			ANDROID_LOG_ERROR,
			ANDROID_LOG_INFO,
			ANDROID_LOG_WARN,
			ANDROID_LOG_DEBUG,
			ANDROID_LOG_VERBOSE,
	};

	if (_type > LOG_LEVEL)
		return;

	va_list va;
	va_start(va, _format);
	__android_log_vprint(androidLogTranslate[_type], "GLideN64", _format, va);
	va_end(va);
}
