#pragma once

#include "buffer.hpp"

namespace Term
{

	typedef int TileCount;
	typedef int TileSize;
	typedef int PixDim;

	/* Abstract base class for system specific functionality.
	*/
	class Context
	{
	public:
		virtual ~Context() = default;
		virtual void Print() const = 0;
		virtual void Print(Char ch, size_t x, size_t y) const = 0;
		virtual Buffer& Framebuffer() = 0;
	};
}
