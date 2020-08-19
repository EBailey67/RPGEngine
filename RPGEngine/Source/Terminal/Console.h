#pragma once
#include "../Utility/Color.h"
#include "ConsoleBuffer.hpp"
#include "string.hpp"
#include "char.hpp"

namespace Term
{
	class Console : public ConsoleBuffer
	{
	typedef unsigned StateBit;
		
	public:
		enum : StateBit // State bitflags
		{
			Wrap = 1,
			Insert = 2,
			VScroll = 4,
			AutoVScroll = 8
		};

		Console(int width, int height);
		explicit Console(ConsoleBuffer&);

		[[nodiscard]] bool IsSet(StateBit b) const;
		[[nodiscard]] Char Peek() const;
		Console& Set(StateBit b, bool setTo = true);
		Console& Place(int x, int y);
		Console& ClearLine();
		Console& Put(Char);
		Console& Put(const String&);
		Console& Put(char);
		Console& Put(const std::string&);
		Console& BgColor(Color);
		Console& FgColor(Color);

	private:
		StateBit state;
		int curs_x, curs_y;
		Color bg_color, fg_color;
	};

}
