#pragma once

#include <vector>

#include "Cell.h"
#include "../core.hpp"

class Map
{

public:
    Map(const Layer layer, const int width, const int height) :
		mapLayer(layer), mapWidth (width), mapHeight(height)
    {
        cell.resize(height);
        for (auto &row : cell)
        {
            row.resize(width);
        }

    	auto y = 0;
        for (auto &row : cell)
        {
        	auto x = 0;
            for (auto &col : row)
            {
                col.x = x;
            	col.y = y;
            	col.isTransparent = false;
            	col.isWalkable = false;
            	col.isExplored = false;
            	col.isDirty = true;
            	
            	++x;
            }
        	++y;
        }
    }

	Layer mapLayer;
    int mapWidth;
	int mapHeight;
    std::vector<std::vector<Cell>> cell;
    inline static bool hasDebugDraw = false;
};