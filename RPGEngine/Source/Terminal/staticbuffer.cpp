#include "staticbuffer.hpp"
#include <stdexcept>
#include <algorithm>

using std::max;
using std::min;

namespace Term
{
	StaticBuffer::StaticBuffer(int w, int h) :
		width(w), height(h),
		clear_char('\0', 0, Color::Black, Color::White),
		buffer(new Char[w * h])
	{}

	int StaticBuffer::Width() const
	{
		return width;
	}

	int StaticBuffer::Height() const
	{
		return height;
	}

	void StaticBuffer::Clear()
	{
		int size = width * height;
		for (size_t i = 0; i < size; ++i)
			buffer[i] = clear_char;
	}

	void StaticBuffer::ClearChar(const Char ch)
	{
		clear_char = ch;
	}

	void StaticBuffer::Put(const int x, const int y, const Char c)
	{
		if (x >= width || y >= height)
			return;
		
		Dirty();
		
		buffer[x + y * width] = c;
	}

	Char StaticBuffer::Get(const int x, const int y) const
	{
		if (x < width && y < height)
			return buffer[x + y * width];

		return Char();
	}

	void StaticBuffer::Scroll(const int rows, const int cols)
	{
		StaticBuffer tmpBuf(Width(), Height());
		tmpBuf.ClearChar(clear_char);
		tmpBuf.Clear();
		tmpBuf.Copy(*this, -cols, -rows, 0, 0, Width(), Height());
		*this = std::move(tmpBuf);
	}

	void StaticBuffer::Copy(const Buffer& other, const int dx, const int dy, 
			const int sx, const int sy, int sw, int sh)
	{
		sh = min(sh, other.Height() - sy);
		sw = min(sw, other.Width() - sx);

		for (int y = max(-sy, 0); y < sh; ++y)
			for (int x = max(-sx, 0); x < sw; ++x)
				Put(dx + x, dy + y, other.Get(sx + x, sy + y));
	}

	void StaticBuffer::Copy(const Buffer& other)
	{
		Copy(other,	0, 0, 0, 0,
			min(Width(), other.Width()), min(Height(), other.Height()));
	}
}