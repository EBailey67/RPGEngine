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
			return a.ASCII() == b.ASCII();
		}

		static bool lt(Char a, Char b)
		{
			return a.ASCII() < b.ASCII();
		}

		static int compare(const Char* a, const Char* b, size_t n)
		{
			while (n-- != 0)
			{
				if (a[n].ASCII() < b[n].ASCII()) return -1;
				if (a[n].ASCII() > b[n].ASCII()) return 1;
				++a; ++b;
			}
			return 0;
		}
	};

	typedef std::basic_string<Char, TermCharTraits> String;
	String MakeString(const std::string&);
	void PriColor(String&, Color);
	void SecColor(String&, Color);
}
