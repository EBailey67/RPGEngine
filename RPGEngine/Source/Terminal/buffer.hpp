#pragma once

#include "char.hpp"

namespace Term
{
	class Buffer
	{
	public:
		virtual ~Buffer() = default;
		[[nodiscard]] virtual size_t Width() const = 0;
		[[nodiscard]] virtual size_t Height() const = 0;
		[[nodiscard]] virtual Char Get(size_t x, size_t y) const = 0;
		virtual void Clear() = 0;
		virtual void ClearChar(Char) = 0;
		virtual void Put(size_t x, size_t y, Char) = 0;
		virtual void Scroll(int rows, int cols = 0) = 0;
		virtual void Copy(const Buffer&, int dx, int dy, int sx, int sy, size_t sw, size_t sh) = 0;
		virtual void Copy(const Buffer&) = 0;
	};
}


