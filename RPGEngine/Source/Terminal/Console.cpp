#include "Console.h"


namespace Term
{
	Console::Console(const int width, const int height) :
		width(width), height(height),
		clear_char('\0', Color::Black, Color::White),
		buffer(new CharCell[width * height]),
		state(0),
		curs_x(0), curs_y(0),
		bg_color(255, 255, 255),
		fg_color(0, 0, 0)
		
	{
		//buffer = new ConsoleBuffer(width, height);
	}
	
	Console::Console(Console& buf) :
		width(buf.width), height(buf.height),
		clear_char('\0', Color::Black, Color::White),
		buffer(new CharCell[buf.width * buf.height]),
	
		state(0),
		curs_x(0), curs_y(0),
		bg_color(255, 255, 255),
		fg_color(0, 0, 0)
	{
		Copy(buf);
	}

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
		if (IsSet(Wrap) && x >= width)
		{
			++y;
			x %= width;
		}

		if (IsSet(VScroll) && y >= height)
		{
			Scroll(y - height + 1);
			y = height - 1;
		}
		curs_x = x;
		curs_y = y;
		return *this;
	}

	Console& Console::ClearLine()
	{
		for (auto x = 0; x < width; ++x)
			PutCh(x, curs_y, CharCell());

		Place(0, curs_y);
		return *this;
	}

	CharCell Console::Peek() const
	{
		return GetCh(curs_x, curs_y);
	}

	Console& Console::Put(const CharCell ch)
	{
		if (IsSet(Insert) && Peek().Ascii() != '\0')
		{
			Console insert(Width(), Height());
			insert.Copy(*this);
			insert.Place(curs_x + 1, curs_y);
			auto next = Peek();
			do 
			{
				const auto tmp = insert.Peek();
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
		Put(CharCell(c, bg_color, fg_color));
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

	int Console::Width() const
	{
		return width;
	}

	int Console::Height() const
	{
		return height;
	}

	void Console::Clear() const
	{
		int size = width * height;
		for (size_t i = 0; i < size; ++i)
			buffer[i] = clear_char;
	}

	void Console::ClearChar(const CharCell ch)
	{
		clear_char = ch;
	}

	void Console::PutCh(const int x, const int y, const CharCell c)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		
		Dirty();
		
		buffer[x + y * width] = c;
	}

	CharCell Console::GetCh(const int x, const int y) const
	{
		if (x>= 0 && x < width && y >= 0 && y < height)
			return buffer[x + y * width];

		return CharCell();
	}

	void Console::Scroll(const int rows, const int cols)
	{
		Console tmpBuf(Width(), Height());
		tmpBuf.ClearChar(clear_char);
		tmpBuf.Clear();
		tmpBuf.Copy(*this, -cols, -rows, 0, 0, Width(), Height());
		Copy(tmpBuf);
	}

	void Console::Copy(const Console& other, const int dx, const int dy, 
			const int sx, const int sy, int sw, int sh)
	{
		sh = std::min(sh, other.Height() - sy);
		sw = std::min(sw, other.Width() - sx);

		for (auto y = std::max(-sy, 0); y < sh; ++y)
			for (auto x = std::max(-sx, 0); x < sw; ++x)
				PutCh(dx + x, dy + y, other.GetCh(sx + x, sy + y));
	}

	void Console::Copy(const Console& other)
	{
		Copy(other,	0, 0, 0, 0,
			std::min(Width(), other.Width()), std::min(Height(), other.Height()));
	}
}
