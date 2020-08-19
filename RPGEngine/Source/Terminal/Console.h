#pragma once
#include <cstdint>
#include "../Utility/Color.h"

namespace Console
{
	class Char
	{
	public:
		typedef uint8_t char_t;

		Char() = default;
		Char(char_t, Color, Color);

		explicit Char(char_t, uint8_t pal_i = 0);

		[[nodiscard]] char_t Ascii() const;
		[[nodiscard]] Color BgColor() const;
		[[nodiscard]] Color FgColor() const;

		Char& Ascii(char_t);
		Char& BgColor(Color);
		Char& FgColor(Color);
	private:
		char_t  c;
		Color bg_color = Color::Black;
		Color fg_color = Color::White;
	};


	class Console
	{
	public:

	};

}
