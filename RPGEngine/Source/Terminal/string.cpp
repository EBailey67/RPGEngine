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
			ch.ASCII(c);
			termstr.push_back(ch);
		}
		return termstr;
	}

	void PriColor(String& str, Color color)
	{
		for (auto& ch : str)
			ch.PriColor(color);
	}

	void SecColor(String& str, Color color)
	{
		for (auto& ch : str)
			ch.SecColor(color);
	}
}
