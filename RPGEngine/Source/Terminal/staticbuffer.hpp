#pragma once

#include <memory>
#include "buffer.hpp"

namespace Term
{
	class StaticBuffer : public Buffer
	{
	public:
		StaticBuffer(int width, int height);

		[[nodiscard]] int Width() const override;
		[[nodiscard]] int Height() const override;
		[[nodiscard]] Char Get(int x, int y) const override;

		void Clear() override;
		void ClearChar(Char) override;
		void Put(int x, int y, Char) override;
		void Scroll(int rows, int cols = 0) override;
		void Copy(const Buffer&, int dx, int dy, int sx, int sy, int sw, int sh) override;
		void Copy(const Buffer&) override;
		[[nodiscard]] bool IsDirty() const {return is_dirty;}
		void Clean() override {is_dirty = false;} 
		void Dirty() override {is_dirty = true;}
		
	private:
		bool is_dirty = true;
		typedef std::unique_ptr<Char[]>	BufferData;
		int width, height;
		Char clear_char;
		BufferData buffer;
	};
}
