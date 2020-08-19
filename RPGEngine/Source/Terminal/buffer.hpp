#pragma once

#include "char.hpp"

namespace Term
{
	class Buffer
	{
	public:
		virtual ~Buffer() = default;
		[[nodiscard]] virtual int Width() const = 0;
		[[nodiscard]] virtual int Height() const = 0;
		[[nodiscard]] virtual Char Get(int x, int y) const = 0;
		virtual void Clear() = 0;
		virtual void ClearChar(Char) = 0;
		virtual void Put(int x, int y, Char) = 0;
		virtual void Scroll(int rows, int cols = 0) = 0;
		virtual void Copy(const Buffer&, int dx, int dy, int sx, int sy, int sw, int sh) = 0;
		virtual void Copy(const Buffer&) = 0;
		[[nodiscard]] virtual bool IsDirty() const = 0;
		virtual void Clean() = 0;
		virtual void Dirty() = 0;
	};
}


