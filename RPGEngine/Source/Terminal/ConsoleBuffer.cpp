#include "ConsoleBuffer.hpp"
#include <stdexcept>
#include <algorithm>

using std::max;
using std::min;

namespace Term
{
	ConsoleBuffer::ConsoleBuffer(const int width, const int height) :
		width(width), height(height),
		clear_char('\0', Color::Black, Color::White),
		buffer(new Char[width * height])
	{}

	ConsoleBuffer::ConsoleBuffer(ConsoleBuffer& cb) :
		width(cb.width), height(cb.height),
		clear_char('\0', Color::Black, Color::White),
		buffer(new Char[cb.width * cb.height])
	
	{
		Copy(cb);
	}
	
	int ConsoleBuffer::Width() const
	{
		return width;
	}

	int ConsoleBuffer::Height() const
	{
		return height;
	}

	void ConsoleBuffer::Clear() const
	{
		int size = width * height;
		for (size_t i = 0; i < size; ++i)
			buffer[i] = clear_char;
	}

	void ConsoleBuffer::ClearChar(const Char ch)
	{
		clear_char = ch;
	}

	void ConsoleBuffer::PutCh(const int x, const int y, const Char c)
	{
		if (x >= width || y >= height)
			return;
		
		Dirty();
		
		buffer[x + y * width] = c;
	}

	Char ConsoleBuffer::GetCh(const int x, const int y) const
	{
		if (x < width && y < height)
			return buffer[x + y * width];

		return Char();
	}

	void ConsoleBuffer::Scroll(const int rows, const int cols)
	{
		ConsoleBuffer tmpBuf(Width(), Height());
		tmpBuf.ClearChar(clear_char);
		tmpBuf.Clear();
		tmpBuf.Copy(*this, -cols, -rows, 0, 0, Width(), Height());
		Copy(tmpBuf); // [REVIEW:EBailey] - Used this instead of std::move
		// *this = std::move(tmpBuf);
	}

	void ConsoleBuffer::Copy(const ConsoleBuffer& other, const int dx, const int dy, 
			const int sx, const int sy, int sw, int sh)
	{
		sh = min(sh, other.Height() - sy);
		sw = min(sw, other.Width() - sx);

		for (int y = max(-sy, 0); y < sh; ++y)
			for (int x = max(-sx, 0); x < sw; ++x)
				PutCh(dx + x, dy + y, other.GetCh(sx + x, sy + y));
	}

	void ConsoleBuffer::Copy(const ConsoleBuffer& other)
	{
		Copy(other,	0, 0, 0, 0,
			min(Width(), other.Width()), min(Height(), other.Height()));
	}
}
