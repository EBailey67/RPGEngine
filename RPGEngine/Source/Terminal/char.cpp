#include "char.hpp"

namespace Term
{
	Color Color::Black(0, 0, 0);
	Color Color::White(255, 255, 255);


	Char::Char(char_t c_, uint8_t pal, Color pri, Color sec) :
		c(c_),
		pal_i(pal),
		bg_color(pri),
		fg_color(sec)
	{}

	Char::Char(const char_t c_, const uint8_t pal) :
		c(c_),
		pal_i(pal),
		bg_color(Color::Black),
		fg_color(Color::White)
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
