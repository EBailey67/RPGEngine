#include "Console.h"


namespace Term
{
	Console::Console(const int width, const int height) :
		ConsoleBuffer{width, height},
		state(0),
		curs_x(0), curs_y(0),
		bg_color(255, 255, 255),
		fg_color(0, 0, 0)
		
	{
		//buffer = new ConsoleBuffer(width, height);
	}
	
	Console::Console(ConsoleBuffer& buf) :
		ConsoleBuffer(buf),
		state(0),
		curs_x(0), curs_y(0),
		bg_color(255, 255, 255),
		fg_color(0, 0, 0)
		{}

	Console& Console::Set(const StateBit b, const bool setTo)
	{
		if (setTo)
			state |= b;
		else
			state ^= b;
		return *this;
	}

	bool Console::IsSet(const StateBit b) const
	{
		return (b & state) == b;
	}

	Console& Console::Place(int x, int y)
	{
		if (IsSet(Wrap) && x >= Width())
		{
			++y;
			x %= Width();
		}

		if (IsSet(VScroll) && y >= Height())
		{
			Scroll(y - Height() + 1);
			y = Height() - 1;
		}
		curs_x = x;
		curs_y = y;
		return *this;
	}

	Console& Console::ClearLine()
	{
		for (auto x = 0; x < Width(); ++x)
			PutCh(x, curs_y, Char());

		Place(0, curs_y);
		return *this;
	}

	Char Console::Peek() const
	{
		return GetCh(curs_x, curs_y);
	}

	Console& Console::Put(const Char ch)
	{
		if (IsSet(Insert) && Peek().Ascii() != '\0')
		{
			Console insert(Width(), Height());
			insert.Copy(*this);
			insert.Place(curs_x + 1, curs_y);
			auto next = Peek();
			do 
			{
				auto tmp = insert.Peek();
				insert.Put(next);
				next = tmp;
			} while (next.Ascii() != '\0');
		}

		if (ch.Ascii() == '\n')
		{
			Place(0, curs_y + 1);
		}
		else
		{
			PutCh(curs_x, curs_y, ch);
			Place(curs_x + 1, curs_y);
		}
		
		return *this;
	}

	Console& Console::Put(const String& str)
	{
		for (auto ch : str)
			Put(ch);

		return *this;
	}

	Console& Console::Put(const char c)
	{
		Put(Char(c, bg_color, fg_color));
		return *this;
	}

	Console& Console::Put(const std::string& str)
	{
		for (auto c : str)
			Put(c);
		
		return *this;
	}

	Console& Console::BgColor(const Color color)
	{
		bg_color = color;
		return *this;
	}

	Console& Console::FgColor(const Color color)
	{
		fg_color = color;
		return *this;
	}
}
