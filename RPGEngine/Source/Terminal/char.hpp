#pragma once

#include <cstdint>

namespace Term
{
	struct Color
	{
		typedef uint8_t component_t;
		component_t r, g, b;

		Color() = default;
		Color(component_t r_, component_t g_, component_t b_) :
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

		Char(char_t, uint8_t pal_i = 0);

		[[nodiscard]] char_t ASCII() const;
		[[nodiscard]] Color PriColor() const;
		[[nodiscard]] Color SecColor() const;

		Char& ASCII(char_t);
		Char& PriColor(Color);
		Char& SecColor(Color);
	private:
		union
		{
			char16_t wc;
			struct
			{
				char_t  c;
				uint8_t pal_i;
			};
		};

		Color priColor = Color::Black;
		Color secColor = Color::White;
	};
}
