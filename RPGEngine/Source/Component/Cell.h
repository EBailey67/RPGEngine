#pragma once

class Cell
{
public:
	Cell() :
		x(0), y(0), isTransparent(false), isWalkable(false), isExplored(false), isInFOV(false), isDirty(true)
	{
		
	}
	
	Cell(const int x, const int y, const bool isTransparent, const bool isWalkable) :
		x(x), y(y), isTransparent(isTransparent), isWalkable(isWalkable), isExplored(false), isInFOV(false), isDirty(true)
	{}

	int x;
	int y;
	bool isTransparent;
	bool isWalkable;
	bool isExplored;
	bool isInFOV;
	bool isDirty;
};