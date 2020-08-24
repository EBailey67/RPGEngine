#include "CharCell.hpp"

namespace Term
{
	CharCell::CharCell(const char_t c_, const Color bg, const Color fg) :
		c(c_), bg_color(bg), fg_color(fg)
	{}

	CharCell::CharCell(const char_t c_) :
		c(c_), bg_color(Color::Black), fg_color(Color::White)
	{}

	CharCell::char_t CharCell::Ascii() const
	{
		return c;
	}

	CharCell& CharCell::Ascii(const char_t newC)
	{
		c = newC;
		return *this;
	}

	CharCell& CharCell::BgColor(const Color newCol)
	{
		bg_color = newCol;
		return *this;
	}

	Color CharCell::BgColor() const
	{
		return bg_color;
	}

	CharCell& CharCell::FgColor(const Color newCol)
	{
		fg_color = newCol;
		return *this;
	}

	Color CharCell::FgColor() const
	{
		return fg_color;
	}


	bool operator ==(const CharCell& lhs, const CharCell& rhs)
	{
		return lhs.Ascii() == rhs.Ascii() && lhs.BgColor() == rhs.BgColor() && lhs.FgColor() == rhs.FgColor();
	}

}
