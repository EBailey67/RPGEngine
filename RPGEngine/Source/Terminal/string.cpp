#include "string.hpp"

namespace Term
{
	String MakeString(const std::string_view& str)
	{
		String termstr;
		termstr.reserve(str.length());
		CharCell ch;
		for (auto c : str)
		{
			ch.Ascii(c);
			termstr.push_back(ch);
		}
		return termstr;
	}

	String MakeString(const std::string_view& str, const Color fgColor, const Color bgColor)
	{
		String termstr;
		termstr.reserve(str.length());
		CharCell ch;
		for (auto c : str)
		{
			ch.Ascii(c).FgColor(fgColor).BgColor(bgColor);
			termstr.push_back(ch);
		}
		return termstr;
	}
	
	void PriColor(String& str, const Color color)
	{
		for (auto& ch : str)
			ch.BgColor(color);
	}

	void SecColor(String& str, const Color color)
	{
		for (auto& ch : str)
			ch.FgColor(color);
	}
}
