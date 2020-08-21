#pragma once

#include <string>
#include "CharCell.hpp"

namespace Term
{
	struct TermCharTraits : std::char_traits<CharCell>
	{
		typedef CharCell char_type;

		static bool eq(CharCell a, CharCell b)
		{
			return a.Ascii() == b.Ascii();
		}

		static bool lt(CharCell a, CharCell b)
		{
			return a.Ascii() < b.Ascii();
		}

		static int compare(const CharCell* a, const CharCell* b, size_t n)
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

	typedef std::basic_string<CharCell, TermCharTraits> String;
	String MakeString(const std::string&);
	void BgColor(String&, Color);
	void FgColor(String&, Color);
}
