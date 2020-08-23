#pragma once
// Derivitive work based on Kim Simmons Terminate project
// https://github.com/Zoomulator/Terminate


#include <memory>

#include "../Utility/Color.h"
#include "string.hpp"
#include "CharCell.hpp"

namespace Term
{
	class Console
	{
	typedef unsigned StateBit;
		
	public:
		enum : StateBit // State bitflags
		{
			Wrap = 1,
			Insert = 2,
			VScroll = 4,
			AutoVScroll = 8
		};

		Console(int width, int height);
		explicit Console(Console&);

		[[nodiscard]] bool IsSet(StateBit b) const;
		[[nodiscard]] CharCell Peek() const;
		[[nodiscard]] int Width() const;
		[[nodiscard]] int Height() const;
		[[nodiscard]] CharCell GetCh(int x, int y) const;
		[[nodiscard]] CharCell GetClearChar() const { return clear_char; }

		Console& Set(StateBit b, bool setTo = true);
		Console& Place(int x, int y);
		Console& ClearLine();
		Console& Put(CharCell);
		Console& Put(const String&);
		Console& Put(char);
		Console& Put(const std::string&);
		Console& BgColor(Color);
		Console& FgColor(Color);

		void Clear() const;
		void ClearChar(CharCell);
		void PutCh(const int x, const int y, const CharCell);
		void Scroll(int rows, int cols = 0);
		void Copy(const Console&, int dx, int dy, int sx, int sy, int sw, int sh);
		void Copy(const Console&);

		[[nodiscard]] bool IsDirty() const {return is_dirty;}
		void Clean() {is_dirty = false;} 
		void Dirty() {is_dirty = true;}
		
	private:
		StateBit state;
		int curs_x, curs_y;
		Color bg_color, fg_color;
		bool is_dirty = true;
		int width, height;
		CharCell clear_char;
		std::unique_ptr<CharCell[]> buffer;
	};

}
