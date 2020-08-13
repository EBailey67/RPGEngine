#include "tilegridsystem.hpp"

#include "../core.hpp"
#include "../Utility/Visibility.h"


constexpr int North = 0;
constexpr int South = 1;
constexpr int East = 2;
constexpr int West = 3;

void ConvertTileMapToPolyMap(TileGrid& tileGrid, int sx, int sy, int w, int h, float fBlockWidth, int pitch, RPGEngine::Cell* world)
{
	PROFILE_FUNCTION();
	
	// Clear our edges array
	tileGrid.vecEdges.clear();

	for (auto x = 0; x < w; x++)
		for (auto y = 0; y < h; y++)
		{
			for (auto j = 0; j < 4; j++)
			{
				world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
				world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
			}
		}

	// Iterate through region from top left to bottom right
	for (auto x = 1; x < w - 1; x++)
		for (auto y = 1; y < h - 1; y++)
		{
			// Create some convenient indices
			auto i = (y + sy) * pitch + (x + sx);		// This
			auto n = (y + sy - 1) * pitch + (x + sx);	// Northern Neighbour
			auto s = (y + sy + 1) * pitch + (x + sx);	// Southern Neighbour
			auto w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
			auto e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

			// If this cell exists, check if it needs edges
			if (world[i].exist)
			{
				// If this cell has no western neighbour, it needs a western edge
				if (!world[w].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (world[n].edge_exist[West])
					{
						// Northern neighbour has a western edge, so grow it downwards
						tileGrid.vecEdges[world[n].edge_id[West]].b.y += fBlockWidth;
						world[i].edge_id[West] = world[n].edge_id[West];
						world[i].edge_exist[West] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						RPGEngine::LineSegment edge;
						edge.a.x = (sx + x) * fBlockWidth; edge.a.y = (sy + y) * fBlockWidth;
						edge.b.x = edge.a.x; edge.b.y = edge.a.y + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = tileGrid.vecEdges.size();
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[West] = edge_id;
						world[i].edge_exist[West] = true;
					}
				}

				// If this cell dont have an eastern neignbour, It needs a eastern edge
				if (!world[e].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (world[n].edge_exist[East])
					{
						// Northern neighbour has one, so grow it downwards
						tileGrid.vecEdges[world[n].edge_id[East]].b.y += fBlockWidth;
						world[i].edge_id[East] = world[n].edge_id[East];
						world[i].edge_exist[East] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						RPGEngine::LineSegment edge;
						edge.a.x = (sx + x + 1) * fBlockWidth; edge.a.y = (sy + y) * fBlockWidth;
						edge.b.x = edge.a.x; edge.b.y = edge.a.y + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = tileGrid.vecEdges.size();
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[East] = edge_id;
						world[i].edge_exist[East] = true;
					}
				}

				// If this cell doesnt have a northern neignbour, It needs a northern edge
				if (!world[n].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (world[w].edge_exist[North])
					{
						// Western neighbour has one, so grow it eastwards
						tileGrid.vecEdges[world[w].edge_id[North]].b.x += fBlockWidth;
						world[i].edge_id[North] = world[w].edge_id[North];
						world[i].edge_exist[North] = true;
					}
					else
					{
						// Western neighbour does not have one, so create one
						RPGEngine::LineSegment edge;
						edge.a.x = (sx + x) * fBlockWidth; edge.a.y = (sy + y) * fBlockWidth;
						edge.b.x = edge.a.x + fBlockWidth; edge.b.y = edge.a.y;

						// Add edge to Polygon Pool
						int edge_id = tileGrid.vecEdges.size();
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[North] = edge_id;
						world[i].edge_exist[North] = true;
					}
				}

				// If this cell doesnt have a southern neignbour, It needs a southern edge
				if (!world[s].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (world[w].edge_exist[South])
					{
						// Western neighbour has one, so grow it eastwards
						tileGrid.vecEdges[world[w].edge_id[South]].b.x += fBlockWidth;
						world[i].edge_id[South] = world[w].edge_id[South];
						world[i].edge_exist[South] = true;
					}
					else
					{
						// Western neighbour does not have one, so I need to create one
						RPGEngine::LineSegment edge;
						edge.a.x = (sx + x) * fBlockWidth; edge.a.y = (sy + y + 1) * fBlockWidth;
						edge.b.x = edge.a.x + fBlockWidth; edge.b.y = edge.a.y;

						// Add edge to Polygon Pool
						int edge_id = tileGrid.vecEdges.size();
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[South] = edge_id;
						world[i].edge_exist[South] = true;
					}
				}

			}

		}

	std::cout << "Edges :" << tileGrid.vecEdges.size() << std::endl;
}

void CalculateVisibilityPolygon(TileGrid& tileGrid, const float ox, const float oy, float radius)
{
	// Get rid of existing polygon
	tileGrid.vecVisibilityPolygon.clear();
	tileGrid.vecVisibilityPolygon = RPGEngine::visibility_polygon(Vector2D(ox, oy), tileGrid.vecEdges.begin(), tileGrid.vecEdges.end());
}

void UpdateVisibility()
{
	const auto playerView = registry.view<Player, Hierarchy, Position, Health, Dash>();
	auto &&[player, hierarchy, health, pos, dash] = registry.get<Player, Hierarchy, Health, Position, Dash>(*playerView.begin());

    auto gridView = registry.view<TileGrid, Position>();

	for (const auto& tile : gridView)
    {
        auto& grid = gridView.get<TileGrid>(tile);
		if (grid.layer == Layer::Walls)
		{
			grid.visibilityPos.x = roundf(pos.position.x);
			grid.visibilityPos.y = roundf(pos.position.y);
			CalculateVisibilityPolygon(grid, grid.visibilityPos.x, grid.visibilityPos.y, 1000.0f);
			std::cout << "VisibilityPolygonSize :" << grid.vecVisibilityPolygon.size() << std::endl;
		}
	}
}

void GridCalculateVisibility()
{
    auto gridView = registry.view<TileGrid, Position>();

	for (const auto& tile : gridView)
    {
        auto& grid = gridView.get<TileGrid>(tile);
		auto* const world = new RPGEngine::Cell[grid.mapWidth * grid.mapHeight];

        auto j = grid.cell.size() - 1;
		
        for (const auto &row : grid.cell)
        {
	        auto i = 0;
            for (const auto &id : row)
            {
                if (id && grid.layer == Layer::Walls)
                {
                    // std::cout << "Wall :" << i << "," << j << std::endl;
                	world[j * grid.mapWidth + i].exist = true;
                }
                i++;
            }
            j--;
        }
		ConvertTileMapToPolyMap(grid, 0, 0, grid.mapWidth, grid.mapHeight, 32.0f, grid.mapWidth, world);
		delete[] world;
    }
}


void GridRender()
{
    auto gridView = registry.view<TileGrid, Position>();
    const auto cameraView = registry.view<Camera>();
    auto &activeCamera = cameraView.get(*cameraView.begin());
    activeCamera.UpdateWindowSize(Graphics::Window());

	for (const auto& tile : gridView)
    {
        const auto& grid = gridView.get<TileGrid>(tile);
        const auto& position = gridView.get<Position>(tile);
        SDL_FRect world_tile{position.position.x, position.position.y, grid.tileSet->TileWidth() * grid.scale.x,
                             grid.tileSet->TileHeight() * grid.scale.y};
        auto screenRect = activeCamera.FromWorldToScreenRect(world_tile);
        const SDL_Rect screenPosition = {screenRect.x, screenRect.y};

        auto j = grid.cell.size() - 1;
        for (const auto &row : grid.cell)
        {
	        auto i = 0;
            screenRect.y = static_cast<int>(screenPosition.y - screenRect.h * j);
            for (const auto &id : row)
            {
                if (id)
                {
                    screenRect.x = static_cast<int>(screenPosition.x + screenRect.w * i);
                    if (activeCamera.Contains(screenRect))
                    {
                        Graphics::RenderToLayer(grid.layer, grid.tileSet->Texture(), &(*grid.tileSet)[id - 1],
                                                     &screenRect);
                        if (TileGrid::hasDebugDraw)
                        {
	                        auto fDraw = true;
                        	
                        	switch(grid.layer)
                        	{
                            case Layer::Walls:
	                            Graphics::SetDrawColor(255, 0, 0, 255);
                        		break;
                            case Layer::Floor:
                        		fDraw = false;
								Graphics::SetDrawColor(0, 0, 255, 255);
                        		break;
                            case Layer::Objects:
	                            Graphics::SetDrawColor(0, 255, 0, 255);
                        		break;
                            default:
	                            Graphics::SetDrawColor(255, 255, 255, 255);
								break;                        		
                        	}
                        	if (fDraw)
                        	{
	                            Graphics::DrawRectToLayer(Layer::Debug, &screenRect);
	                            Graphics::ResetDrawColor();
                            }
                        }
                    }
                }
                i++;
            }
            j--;
        }
		if (grid.layer == Layer::Walls)
		{
			if (!grid.vecEdges.empty())
			{
				for (auto& e : grid.vecEdges)
				{
					auto p1 = activeCamera.FromWorldToScreenView(e.a);
					auto p2 = activeCamera.FromWorldToScreenView(e.b);
					Graphics::SetDrawColor(0, 255, 0, 255);
					Graphics::DrawLineToLayer(Layer::Debug, p1.x, p1.y, p2.x, p2.y);
					Graphics::SetDrawColor(255, 0, 0);
					Graphics::DrawFillCircleToLayer(Layer::Debug, p1.x, p1.y, 4);
					Graphics::DrawFillCircleToLayer(Layer::Debug, p2.x, p2.y, 4);
					Graphics::ResetDrawColor();
				}
			}

			Graphics::SetDrawColor(0, 255, 0, 64);

			std::vector<Vector2D> screenPolygon(grid.vecVisibilityPolygon.size());
			for (auto i = 0; i < grid.vecVisibilityPolygon.size(); i++)
			{
				screenPolygon[i] = activeCamera.FromWorldToScreenView(grid.vecVisibilityPolygon[i]);
			}

			Graphics::DrawFillPolygonToLayer(Layer::Debug, screenPolygon);
			Graphics::SetDrawColor(0, 255, 255, 255);
			Graphics::DrawPolygonToLayer(Layer::Debug, screenPolygon);
			
			//for (auto i = 0; i < grid.vecVisibilityPolygon.size() - 1; i++)
			//{
			//	auto p1 = activeCamera.FromWorldToScreenView(grid.visibilityPos);
			//	auto p2 = activeCamera.FromWorldToScreenView(grid.vecVisibilityPolygon[i]);
			//	auto p3 = activeCamera.FromWorldToScreenView(grid.vecVisibilityPolygon[i + 1]);

			//	Graphics::DrawFillTriangleToLayer(Layer::Debug, p1, p2, p3);
			//}

			//auto p1 = activeCamera.FromWorldToScreenView(grid.visibilityPos);
			//auto p2 = activeCamera.FromWorldToScreenView(grid.vecVisibilityPolygon[grid.vecVisibilityPolygon.size() - 1]);
			//auto p3 = activeCamera.FromWorldToScreenView(grid.vecVisibilityPolygon[0]);

			//// Fan will have one open edge, so draw last point of fan to first
			//Graphics::DrawFillTriangleToLayer(Layer::Debug, p1, p2, p3);
			Graphics::ResetDrawColor();
		}
		
    }
}

void GridCreate()
{
    pugi::xml_document map;
    const auto result = map.load_file("resources/tiled_files/RPGTest.tmx");
    if (!result)
    {
        FAST_THROW(result.description());
    }
    
    {
        auto id = registry.create();
        auto &tilegrid = registry.emplace<TileGrid>(id, map, &tileset, 1);
        registry.emplace<Position>(id);
        registry.emplace<TileGridCollider>(id);
        registry.emplace<CollisionLayer>(id, LayersID::FLOOR);
        registry.emplace<Active>(id);
        tilegrid.scale = {1, 1};
        tilegrid.layer = Layer::Floor;
    }

    {
        auto id = registry.create();
        auto &tilegrid = registry.emplace<TileGrid>(id, map, &tileset, 2);
        registry.emplace<Position>(id);
        registry.emplace<TileGridCollider>(id);
        registry.emplace<CollisionLayer>(id, LayersID::WALLS);
        registry.emplace<Active>(id);
        tilegrid.scale = {1, 1};
        tilegrid.layer = Layer::Walls;
    }
    {
        auto id = registry.create();
        auto &tilegrid = registry.emplace<TileGrid>(id, map, &tileset, 3);
        registry.emplace<Position>(id);
        registry.emplace<TileGridCollider>(id);
        registry.emplace<CollisionLayer>(id, LayersID::WALLS);
        registry.emplace<Active>(id);

        tilegrid.scale = {1, 1};
        tilegrid.layer = Layer::Objects;
    }
	GridCalculateVisibility();
}
