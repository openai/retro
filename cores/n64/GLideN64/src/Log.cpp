#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include "Log.h"
#include "PluginAPI.h"
#include "wst.h"
#include <mutex>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

std::mutex g_logMutex;
std::wofstream fileOutput;

std::wstring getFormattedTime()
{
	using namespace std::chrono;

	// get current time
	auto now = system_clock::now();

	// get number of milliseconds for the current second
	// (remainder after division into seconds)
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

	// convert to std::time_t in order to convert to std::tm (broken time)
	auto timer = system_clock::to_time_t(now);

	// convert to broken time
	std::tm bt = *std::localtime(&timer);

	std::wstringstream oss;

	oss << std::put_time(&bt, L"%Y/%m/%d,%H:%M:%S"); // HH:MM:SS
	oss << L'.' << std::setfill(L'0') << std::setw(3) << ms.count();

	return oss.str();
}

void LogDebug(const char* _fileName, int _line, u16 _type, const char* _format, ...) {

	static const char* logLevelText[] = {
			"NONE",
			"ERROR",
			"MINIMAL",
			"WARNING",
			"VERBOSE",
			"APIFUNC",
	};

	if (_type > LOG_LEVEL)
		return;

	std::wstring formattedTimeOfLog = getFormattedTime();

	std::unique_lock<std::mutex> lock(g_logMutex);

	if (!fileOutput.is_open()) {
		class SetLocale
		{
		public:
			SetLocale() : m_locale(setlocale(LC_CTYPE, NULL)) { setlocale(LC_CTYPE, ""); }
			~SetLocale() { setlocale(LC_CTYPE, m_locale.c_str()); }
		private:
			std::string m_locale;
		} setLocale;

		wchar_t logPath[PLUGIN_PATH_SIZE + 16];
		api().GetUserDataPath(logPath);

		// Convert wchar string to multibyte string
		// Use large enough buffer to hold multibyte conversion of wchar string
		char logPathChar[PLUGIN_PATH_SIZE * 4];
		std::wcstombs(logPathChar, logPath, sizeof(logPathChar));

		std::stringstream logPathStream;
		logPathStream << logPathChar << "/" << "gliden64.log";
		fileOutput.open(logPathStream.str().c_str(), std::wofstream::out | std::wofstream::app);
	}

	if (!fileOutput.is_open())
		return;

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, _format);

	// reliably acquire the size from a copy of
	// the variable argument array
	// and a functionally reliable call
	// to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf(NULL, 0, _format, vaCopy);
	va_end(vaCopy);

	// return a formatted string without
	// risking memory mismanagement
	// and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc(iLen + 1);
	std::vsnprintf(zc.data(), zc.size(), _format, vaArgs);
	va_end(vaArgs);

	std::time_t t = std::time(nullptr);
	std::wstringstream lcFormatString;
	lcFormatString << formattedTimeOfLog << "," << _fileName << ":" << _line << "," << logLevelText[_type] << ", \"" << zc.data() << "\"" << std::endl;
	fileOutput << lcFormatString.str();
	fileOutput.flush();
}

#if defined(OS_WINDOWS) && !defined(MINGW)
#include "windows/GLideN64_windows.h"
void debugPrint(const char * format, ...) {
	char text[256];
	wchar_t wtext[256];
	va_list va;
	va_start(va, format);
	vsprintf(text, format, va);
	mbstowcs(wtext, text, 256);
	OutputDebugString(wtext);
	va_end(va);
}
#endif
