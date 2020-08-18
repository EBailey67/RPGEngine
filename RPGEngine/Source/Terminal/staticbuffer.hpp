#pragma once

#include <memory>
#include "buffer.hpp"

namespace Term
{
	class StaticBuffer : public Buffer
	{
	public:
		StaticBuffer(size_t width, size_t height);

		[[nodiscard]] size_t Width() const override;
		[[nodiscard]] size_t Height() const override;
		[[nodiscard]] Char Get(size_t x, size_t y) const override;

		void Clear() override;
		void ClearChar(Char) override;
		void Put(size_t x, size_t y, Char) override;
		void Scroll(int rows, int cols = 0) override;
		void Copy(const Buffer&, int dx, int dy, int sx, int sy, size_t sw, size_t sh) override;
		void Copy(const Buffer&) override;

	private:
		typedef std::unique_ptr<Char[]>	BufferData;
		size_t      width, height;
		Char        clear_char;
		BufferData  buffer;
	};
}
