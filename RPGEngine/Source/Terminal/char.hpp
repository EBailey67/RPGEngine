#pragma once

#include <cstdint>

namespace Term
{
	struct Color
	{
		typedef uint8_t component_t;
		component_t r, g, b;

		Color() = default;
		Color(const component_t r_, const component_t g_, const component_t b_) :
			r(r_), g(g_), b(b_)
		{}

		static Color Black, White;
	};

	class Char
	{
	public:
		typedef uint8_t char_t;

		Char() = default;

		Char(char_t, uint8_t pal_i, Color, Color);

		explicit Char(char_t, uint8_t pal_i = 0);

		[[nodiscard]] char_t Ascii() const;
		[[nodiscard]] Color BgColor() const;
		[[nodiscard]] Color FgColor() const;

		Char& Ascii(char_t);
		Char& BgColor(Color);
		Char& FgColor(Color);
	private:
		union
		{
			char16_t wc{};
			struct
			{
				char_t  c;
				uint8_t pal_i;
			};
		};

		Color bg_color = Color::Black;
		Color fg_color = Color::White;
	};
}
