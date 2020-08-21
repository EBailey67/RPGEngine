#pragma once

#include <cstdint>
#include "../Utility/Color.h"

namespace Term
{
	class Char
	{
	public:
		typedef uint8_t char_t;

		Char() = default;
		Char(char_t, Color, Color);

		Char(char_t);

		[[nodiscard]] char_t Ascii() const;
		[[nodiscard]] Color BgColor() const;
		[[nodiscard]] Color FgColor() const;

		Char& Ascii(char_t);
		Char& BgColor(Color);
		Char& FgColor(Color);
	private:
		char_t  c{};
		Color bg_color = Color::Black;
		Color fg_color = Color::White;
	};
}
