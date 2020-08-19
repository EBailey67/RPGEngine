#include "string.hpp"

namespace Term
{
	String MakeString(const std::string& str)
	{
		String termstr;
		termstr.reserve(str.length());
		Char ch;
		for (auto c : str)
		{
			ch.Ascii(c);
			termstr.push_back(ch);
		}
		return termstr;
	}

	void PriColor(String& str, Color color)
	{
		for (auto& ch : str)
			ch.BgColor(color);
	}

	void SecColor(String& str, Color color)
	{
		for (auto& ch : str)
			ch.FgColor(color);
	}
}
