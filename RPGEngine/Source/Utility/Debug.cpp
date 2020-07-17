// debug.cpp - debugging support routines

#include "Debug.h"

#define _VERBOSE true

// Write debug information out
void DebugTrace(const char* strFile, int dwLine, const std::string& category, const std::string& message)
{
#if defined(_DEBUG)
	std::cout << strFile << "(" << dwLine << ") - " << category << " " << message << std::endl;
#endif
}

long DebugFail(const char* strFile, int dwLine, long hr, const char* strMsg, bool fPopMsgBox)
{
#if defined(_DEBUG)
	std::cout << strFile << "(" << dwLine << ") - " << std::hex << hr << " " << strMsg << std::endl;
	return hr;
#endif
}


// Simple method to output to the debugger console window
void DebugLog(const std::string area, const std::string message)
{
#if defined(_DEBUG) && defined(_VERBOSE)
	std::cout << "(" << area << ") - " << message << std::endl;
#endif
}

void DebugWarning(const std::string area, const std::string message)
{
#if defined(_DEBUG)
	std::cout << "Warning: (" << area << ") - " << message << std::endl;
#endif
}

void DebugError(const std::string area, const std::string message)
{
#if defined(_DEBUG)
	std::cout << "ERROR: (" << area << ") - " << message << std::endl;
#endif
}
