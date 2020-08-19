#pragma once

#include "ConsoleBuffer.hpp"
#include "string.hpp"

namespace Term
{
	typedef unsigned StateBit;

	class TTY
	{
	public:
		enum : StateBit // State bitflags
		{
			Wrap = 1,
			Insert = 2,
			VScroll = 4,
			AutoVScroll = 8
		};

		explicit TTY(ConsoleBuffer&);

		[[nodiscard]] bool IsSet(StateBit b) const;
		[[nodiscard]] Char Peek() const;
		TTY& Set(StateBit b, bool setTo = true);
		TTY& Place(int x, int y);
		TTY& ClearLine();
		TTY& Put(Char);
		TTY& Put(const String&);
		TTY& Put(char);
		TTY& Put(const std::string&);
		TTY& BgColor(Color);
		TTY& FgColor(Color);

	private:
		StateBit state;
		int curs_x, curs_y;
		Color bg_color, fg_color;
		ConsoleBuffer* buffer;
	};
}

