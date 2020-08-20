#pragma once

class Cell
{
public:
	Cell(int x, int y, bool isTransparent, bool isWalkable) :
		x(x), y(y), isTransparent(isTransparent), isWalkable(isWalkable), isExplored(false)
	{}

	int x;
	int y;
	bool isTransparent;
	bool isWalkable;
	bool isExplored;
};