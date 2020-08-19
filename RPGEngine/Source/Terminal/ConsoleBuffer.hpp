#pragma once

#include <memory>
#include "char.hpp"

namespace Term
{
	class ConsoleBuffer
	{
	public:
		ConsoleBuffer(int width, int height);

		[[nodiscard]] int Width() const;
		[[nodiscard]] int Height() const;
		[[nodiscard]] Char Get(int x, int y) const;

		void Clear() const;
		void ClearChar(Char);
		void Put(int x, int y, Char);
		void Scroll(int rows, int cols = 0);
		void Copy(const ConsoleBuffer&, int dx, int dy, int sx, int sy, int sw, int sh);
		void Copy(const ConsoleBuffer&);

		[[nodiscard]] bool IsDirty() const {return is_dirty;}
		void Clean() {is_dirty = false;} 
		void Dirty() {is_dirty = true;}
		
	private:
		bool is_dirty = true;
		int width, height;
		Char clear_char;
		std::unique_ptr<Char[]> buffer;
	};
}
