#include "tty.hpp"

namespace Term
{
	TTY::TTY(ConsoleBuffer& buf) :
		buffer(&buf),
		curs_x(0), curs_y(0),
		state(0),
		bg_color(255, 255, 255),
		fg_color(0, 0, 0)
	{}


	TTY& TTY::Set(const StateBit b, const bool setTo)
	{
		if (setTo)
			state |= b;
		else
			state ^= b;
		return *this;
	}


	bool TTY::IsSet(const StateBit b) const
	{
		return (b & state) == b;
	}


	TTY& TTY::Place(int x, int y)
	{
		if (IsSet(Wrap) && x >= buffer->Width())
		{
			++y;
			x %= buffer->Width();
		}

		if (IsSet(VScroll) && y >= buffer->Height())
		{
			buffer->Scroll(y - buffer->Height() + 1);
			y = buffer->Height() - 1;
		}
		curs_x = x;
		curs_y = y;
		return *this;
	}


	TTY& TTY::ClearLine()
	{
		for (auto x = 0; x < buffer->Width(); ++x)
			buffer->Put(x, curs_y, Char());

		Place(0, curs_y);
		return *this;
	}


	Char TTY::Peek() const
	{
		return buffer->Get(curs_x, curs_y);
	}


	TTY& TTY::Put(const Char ch)
	{
		if (IsSet(Insert) && Peek().Ascii() != '\0')
		{
			TTY insert(*buffer);
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
			buffer->Put(curs_x, curs_y, ch);
			Place(curs_x + 1, curs_y);
		}
		
		return *this;
	}


	TTY& TTY::Put(const String& str)
	{
		for (auto ch : str)
			Put(ch);

		return *this;
	}



	TTY& TTY::Put(char c)
	{
		Put(Char(c, bg_color, fg_color));
		return *this;
	}


	TTY& TTY::Put(const std::string& str)
	{
		for (auto c : str)
			Put(c);
		return *this;
	}


	TTY& TTY::BgColor(const Color color)
	{
		bg_color = color;
		return *this;
	}


	TTY& TTY::FgColor(const Color color)
	{
		fg_color = color;
		return *this;
	}
}
