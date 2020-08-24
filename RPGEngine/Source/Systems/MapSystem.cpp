#include "MapSystem.h"

#include <random>


#include "../core.hpp"
#include "../Utility/MTRandom.h"
#include "../Utility/Rectangle.h"

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


constexpr int maxRooms = 10;
constexpr int roomMinSize = 4;
constexpr int roomMaxSize = 8;

int RandomRange(int low, int high)
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static std::mt19937 mtengine(static_cast<unsigned int>(seed));
	std::uniform_int_distribution<int> uniform(low, high);
	return uniform(mtengine);
}



void MakeRoom(Map& map, RPGEngine::Rect& room)
{
	for (auto x = room.x + 1; x < room.x + room.w; x++)
	{
		for (auto y = room.y + 1; y < room.y + room.h; y++)
		{
			map.cell[y][x].isWalkable = true;;
			map.cell[y][x].isTransparent = true;;
		}
	}
}

void MakeHorizontalTunnel(Map& map, const int xStart, const int xEnd, int yPosition)
{
	for (auto x = std::min(xStart, xEnd); x <= std::max(xStart, xEnd); x++)
	{
		map.cell[yPosition][x].isWalkable = true;
		map.cell[yPosition][x].isTransparent = true;
	}
}

void MakeVerticalTunnel(Map& map, int yStart, int yEnd, int xPosition)
{
	for (auto y = std::min(yStart, yEnd); y <= std::max(yStart, yEnd); y++)
	{
		map.cell[y][xPosition].isWalkable = true;
		map.cell[y][xPosition].isTransparent = true;
	}
}


void CreateMap(int w, int h)
{

	MTRandom random;
	random.Randomize();
	std::vector<RPGEngine::Rect> rooms;

	const auto id = registry.create();
	auto& mapGrid = registry.emplace<Map>(id, Layer::Floor, w, h);
	registry.emplace<Position>(id);
	registry.emplace<Active>(id);

	auto mapView = registry.view<Map, Position>();

	for (const auto& cell : mapView)
	{
		auto& grid = mapView.get<Map>(cell);

		for (auto r = 0; r < maxRooms; r++)
		{
			const auto roomWidth = RandomRange(roomMinSize, roomMaxSize);
			const auto roomHeight = RandomRange(roomMinSize, roomMaxSize);
			const auto roomXPosition = RandomRange(0, w - roomWidth - 1);
			const auto roomYPosition = RandomRange(0, h - roomHeight - 1);

			RPGEngine::Rect newRoom(roomXPosition, roomYPosition, roomWidth, roomHeight);
			auto newRoomIntersects = false;
			for (auto& room : rooms)
			{
				if (RPGEngine::Intersect(newRoom, room))
				{
					newRoomIntersects = true;
					break;
				}
			}
			
			if (!newRoomIntersects)
			{
				rooms.emplace_back(newRoom);
			}
		}

		for (auto& room : rooms)
		{
			MakeRoom(grid, room);
		}

		for (auto r = 0; r < rooms.size(); r++)
		{
			if (r == 0)
			{
				continue;
			}

			const auto previousRoomCenterX = RPGEngine::GetCenter(rooms[r - 1]).x;
			const auto previousRoomCenterY = RPGEngine::GetCenter(rooms[r - 1]).y;
			const auto currentRoomCenterX = RPGEngine::GetCenter(rooms[r]).x;
			const auto currentRoomCenterY = RPGEngine::GetCenter(rooms[r]).y;

			if (RandomRange(0, 2) == 0)
			{
				MakeHorizontalTunnel(grid, previousRoomCenterX, currentRoomCenterX, previousRoomCenterY);
				MakeVerticalTunnel(grid, previousRoomCenterY, currentRoomCenterY, currentRoomCenterX);
			}
			else
			{
				MakeVerticalTunnel(grid, previousRoomCenterY, currentRoomCenterY, previousRoomCenterX);
				MakeHorizontalTunnel(grid, previousRoomCenterX, currentRoomCenterX, currentRoomCenterY);
			}
		}

	}
}

