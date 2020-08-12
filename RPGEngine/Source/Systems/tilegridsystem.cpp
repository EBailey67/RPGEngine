#include "tilegridsystem.hpp"

#include "../core.hpp"

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
                        	bool fDraw = true;
                        	
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
}
