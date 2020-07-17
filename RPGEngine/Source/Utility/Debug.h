#pragma once

#include <string>
#include <iostream>

/*
   Debug helper functions.
*/

// Helper macros until C++20 is available
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


void DebugLog(std::string_view area, std::string_view message);
long DebugFail(std::string_view strFile, int dwLine, long hr, std::string_view strMsg, bool fPopMsgBox);
void DebugWarning(std::string_view area, std::string_view message);
void DebugError(std::string_view area, std::string_view message);
void DebugTrace(std::string_view strFile, int dwLine, std::string_view category, std::string_view message);

