// debug.cpp - debugging support routines

#include "Debug.h"

#define _VERBOSE true

// Write debug information out
void DebugTrace(std::string_view strFile, int dwLine, std::string_view category, std::string_view message)
{
#if defined(_DEBUG)
	std::cout << strFile << "(" << dwLine << ") - " << category << " " << message << std::endl;
#endif
}

long DebugFail(std::string_view strFile, int dwLine, long hr, std::string_view strMsg, bool fPopMsgBox)
{
#if defined(_DEBUG)
	std::cout << strFile << "(" << dwLine << ") - " << std::hex << hr << " " << strMsg << std::endl;
	return hr;
#endif
}


// Simple method to output to the debugger console window
void DebugLog(std::string_view area, std::string_view message)
{
#if defined(_DEBUG) && defined(_VERBOSE)
	std::cout << "(" << area << ") - " << message << std::endl;
#endif
}

void DebugWarning(std::string_view area, std::string_view message)
{
#if defined(_DEBUG)
	std::cout << "Warning: (" << area << ") - " << message << std::endl;
#endif
}

void DebugError(std::string_view area, std::string_view message)
{
#if defined(_DEBUG)
	std::cout << "ERROR: (" << area << ") - " << message << std::endl;
#endif
}
