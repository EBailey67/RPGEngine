#include "MapSystem.h"

#include "../core.hpp"

void MapCreate(int w, int h)
{
	PROFILE_FUNCTION();

    const auto id = registry.create();
    auto &mapGrid = registry.emplace<Map>(id, Layer::Floor, w, h);
    registry.emplace<Position>(id);
    registry.emplace<Active>(id);
	
    auto mapView = registry.view<Map, Position>();

	for (const auto& cell : mapView)
    {
        auto& grid = mapView.get<Map>(cell);
        const auto& position = mapView.get<Position>(cell);

        auto j = static_cast<int>(grid.cell.size()) - 1;
        for (auto &row : grid.cell)
        {
	        auto i = 0;
            for (auto &id : row)
            {
            	if (j == grid.mapHeight - 1 || j == 0 || i == 0 || i == grid.mapWidth - 1)
                    id.isWalkable = false;
                else
                    id.isWalkable = true;
                i++;
            }
            j--;
        }
    }

}

void MapRender(Term::SDL::Context& context)
{
	PROFILE_FUNCTION();
    auto mapView = registry.view<Map, Position>();

	auto& console = context.GetConsole();
	
	for (const auto& cell : mapView)
    {
        auto& grid = mapView.get<Map>(cell);
        const auto& position = mapView.get<Position>(cell);

        auto j = static_cast<int>(grid.cell.size()) - 1;
        for (auto &row : grid.cell)
        {
	        auto i = 0;
            for (auto &id : row)
            {
            	if(id.isDirty)
            	{
               		console.Place(i, j);
	                if (id.isWalkable)
	                {
                		console.FgColor(Color::DarkGray).BgColor(Color::Black).Put('.');
	                }
	                else
	                {
                		console.FgColor(Color::Black).BgColor(Color::White).Put('#');
	                }
            		id.isDirty = false;
                }
                i++;
            }
            j--;
        }
    }
	
}
