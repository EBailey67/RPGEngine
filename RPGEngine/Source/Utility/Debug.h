#pragma once

#include <string>
#include <iostream>

/*
   Debug helper functions.
*/

// Helper macros
#if defined(DEBUG) | defined(_DEBUG)
#define TRACE_ERROR(str)	DebugTrace( __FILE__, __LINE__, "ERROR", str)
#define TRACE_FATAL(str)	DebugTrace( __FILE__, __LINE__, "FATAL", str);
#define TRACE_WARNING(str)	DebugTrace( __FILE__, __LINE__, "WARNING", str);
#define TRACE_INFO(str)     DebugTrace( __FILE__, __LINE__, "INFO", str);
#define TRACE_LOG(cat, str) DebugTrace( __FILE__, __LINE__, cat, str);
#define TRACE_ASSERT(expr) \
	do \
	{ \
		if (!(expr)) \
		{ \
			DebugTrace( __FILE__, __LINE__, L"ASSERT", #expr ); \
		} \
	} \
	while (0) \

#else
#define TRACE_ERROR(str)     do { (void)sizeof(str); } while(0)
#define TRACE_FATAL(str)	 do { (void)sizeof(str); } while(0)
#define TRACE_WARNING(str)	 do { (void)sizeof(str); } while(0)
#define TRACE_INFO(str)      do { (void)sizeof(str); } while(0)
#define TRACE_LOG(cat, str)  do { (void)sizeof(cat); (void)sizeof(str); } while(0)
#define TRACE_ASSERT(expr)   do { (void)sizeof(expr); } while(0)
#endif


void DebugLog(const std::string area, const std::string message);
long DebugFail(const char* strFile, int dwLine, long hr, const char* strMsg, bool fPopMsgBox);
void DebugWarning(const std::string area, const std::string message);
void DebugError(const std::string area, const std::string message);
void DebugTrace(const char* strFile, int dwLine, const std::string& category, const std::string& message);

