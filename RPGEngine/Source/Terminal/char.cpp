#include "char.hpp"

namespace Term
{
	Char::Char(char_t c_, Color bg, Color fg) :
		c(c_), bg_color(bg), fg_color(fg)
	{}

	Char::Char(const char_t c_, const uint8_t pal) :
		c(c_), bg_color(Color::Black), fg_color(Color::White)
	{}

	Char::char_t Char::Ascii() const
	{
		return c;
	}

	Char& Char::Ascii(const char_t newC)
	{
		c = newC;
		return *this;
	}

	Char& Char::BgColor(const Color newCol)
	{
		bg_color = newCol;
		return *this;
	}

	Color Char::BgColor() const
	{
		return bg_color;
	}

	Char& Char::FgColor(const Color newCol)
	{
		fg_color = newCol;
		return *this;
	}

	Color Char::FgColor() const
	{
		return fg_color;
	}
}
