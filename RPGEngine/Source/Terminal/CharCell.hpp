#pragma once

#include <cstdint>
#include "../Utility/Color.h"

namespace Term
{
	class CharCell
	{
	public:
		typedef uint8_t char_t;

		CharCell() = default;
		CharCell(char_t, Color, Color);

		CharCell(char_t);

		[[nodiscard]] char_t Ascii() const;
		[[nodiscard]] Color BgColor() const;
		[[nodiscard]] Color FgColor() const;

		CharCell& Ascii(char_t);
		CharCell& BgColor(Color);
		CharCell& FgColor(Color);

		bool operator ==(const CharCell& rhs) const
		{
			return c == rhs.c && bg_color == rhs.bg_color && fg_color == rhs.fg_color;
		}

		bool isDirty{true};

private:
		char_t  c{};
		Color bg_color = Color::Black;
		Color fg_color = Color::White;
	};

	// bool operator ==(const CharCell& lhs, const CharCell& rhs);
	
	class CharCellHashFunction
	{ 
	public: 
		// ch * (fg + bg)
	    size_t operator()(const CharCell& cell) const
	    {
		    const auto bg = cell.BgColor();
		    const auto fg = cell.FgColor();
		    const int ch = cell.Ascii();
		    const auto a = ((fg.a + bg.a) % 255) << 24;
		    const auto r = ((fg.r + bg.r) % 255) << 16;
		    const auto g = ((fg.g + bg.g) % 255) << 8;
		    const auto b = ((fg.b + bg.b) % 255);
	        return ch * (a + r + g + b);
	    } 
	}; 

}
