#pragma once

#include <string>
#include "char.hpp"

namespace Term
{
	struct TermCharTraits : std::char_traits<Char>
	{
		typedef Char char_type;

		static bool eq(Char a, Char b)
		{
			return a.Ascii() == b.Ascii();
		}

		static bool lt(Char a, Char b)
		{
			return a.Ascii() < b.Ascii();
		}

		static int compare(const Char* a, const Char* b, size_t n)
		{
			while (n-- != 0)
			{
				if (a[n].Ascii() < b[n].Ascii()) return -1;
				if (a[n].Ascii() > b[n].Ascii()) return 1;
				++a; ++b;
			}
			return 0;
		}
	};

	typedef std::basic_string<Char, TermCharTraits> String;
	String MakeString(const std::string&);
	void BgColor(String&, Color);
	void FgColor(String&, Color);
}
