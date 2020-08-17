#include <memory>

#include "tilegridsystem.hpp"

#include "../core.hpp"
#include "../Utility/FOVRecurse.h"
#include "../Utility/Visibility.h"


constexpr int North = 0;
constexpr int South = 1;
constexpr int East = 2;
constexpr int West = 3;

std::vector<Vector2D> VisibleTiles;

void ConvertTileMapToPolyMap(TileGrid& tileGrid, const int sx, const int sy, const int width, const int height, const float fBlockWidth, const int pitch, const std::shared_ptr<RPGEngine::Cell[]>& world)
{
	PROFILE_FUNCTION();
	
	// Clear our edges array
	tileGrid.vecEdges.clear();

	for (auto x = 0; x < width; x++)
	{
		for (auto y = 0; y < height; y++)
		{
			for (auto j = 0; j < 4; j++)
			{
				const auto index = (static_cast<size_t>(y) + sy) * pitch + (static_cast<size_t>(x) + sx);
				world[index].edge_exist[j] = false;
				world[index].edge_id[j] = 0;
			}
		}
	}

	// Iterate through region from top left to bottom right
	for (auto x = 0; x < width; x++)
	{
		for (auto y = 0; y < height; y++)
		{
			// Create some convenient indices
			const auto i = (y + sy) * pitch + (x + sx);		// This
			const auto n = (y + sy - 1) * pitch + (x + sx);	// Northern Neighbour
			const auto s = (y + sy + 1) * pitch + (x + sx);	// Southern Neighbour
			const auto w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
			const auto e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

			// If this cell exists, check if it needs edges
			if (world[i].exist)
			{
				// If this cell has no western neighbour, it needs a western edge
				if (x == 0 || !world[w].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (y != 0 && world[n].edge_exist[West])
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
						edge.a.x = static_cast<float>(sx + x) * fBlockWidth;
						edge.a.y = static_cast<float>(sy + y) * fBlockWidth;
						edge.b.x = edge.a.x;
						edge.b.y = edge.a.y + fBlockWidth;

						// Add edge to Polygon Pool
						const auto edge_id = static_cast<int>(tileGrid.vecEdges.size());
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[West] = edge_id;
						world[i].edge_exist[West] = true;
					}
				}

				// If this cell dont have an eastern neignbour, It needs a eastern edge
				if (x == width - 1 || !world[e].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (y != 0 && world[n].edge_exist[East])
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
						edge.a.x = static_cast<float>(sx + x + 1) * fBlockWidth;
						edge.a.y = static_cast<float>(sy + y) * fBlockWidth;
						edge.b.x = edge.a.x;
						edge.b.y = edge.a.y + fBlockWidth;

						// Add edge to Polygon Pool
						const auto edge_id = static_cast<int>(tileGrid.vecEdges.size());
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[East] = edge_id;
						world[i].edge_exist[East] = true;
					}
				}

				// If this cell doesnt have a northern neignbour, It needs a northern edge
				if (y == 0 || !world[n].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (x != 0 && world[w].edge_exist[North])
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
						edge.a.x = static_cast<float>(sx + x) * fBlockWidth;
						edge.a.y = static_cast<float>(sy + y) * fBlockWidth;
						edge.b.x = edge.a.x + fBlockWidth;
						edge.b.y = edge.a.y;

						// Add edge to Polygon Pool
						const auto edge_id = static_cast<int>(tileGrid.vecEdges.size());
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[North] = edge_id;
						world[i].edge_exist[North] = true;
					}
				}

				// If this cell doesnt have a southern neignbour, It needs a southern edge
				if (y == height - 1 || !world[s].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (x != 0 && world[w].edge_exist[South])
					{
						// Western neighbour has one, so grow it eastwards
						tileGrid.vecEdges[world[w].edge_id[South]].b.x += fBlockWidth;
						world[i].edge_id[South] = world[w].edge_id[South];
						world[i].edge_exist[South] = true;
					}
					else
					{
						// Western neighbor does not have one, so I need to create one
						RPGEngine::LineSegment edge;
						edge.a.x = static_cast<float>(sx + x) * fBlockWidth;
						edge.a.y = static_cast<float>(sy + y + 1) * fBlockWidth;
						edge.b.x = edge.a.x + fBlockWidth;
						edge.b.y = edge.a.y;

						// Add edge to Polygon pool
						const auto edge_id = static_cast<int>(tileGrid.vecEdges.size());
						tileGrid.vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[South] = edge_id;
						world[i].edge_exist[South] = true;
					}
				}
			}
		}
	}
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
			//grid.vecVisibilityPolygon.clear();
			//grid.vecVisibilityPolygon = RPGEngine::VisibilityPolygon(grid.visibilityPos,  grid.vecEdges.begin(), grid.vecEdges.end());
			FOVCalculate();
		}
	}
}

void GridCalculateVisibility()
{
    auto gridView = registry.view<TileGrid, Position>();

	for (const auto& tile : gridView)
    {
        auto& grid = gridView.get<TileGrid>(tile);
		auto const world = std::make_shared<RPGEngine::Cell[]>(static_cast<size_t>(grid.mapWidth) * static_cast<size_t>(grid.mapHeight));

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
    }
}


void GridRender()
{
	PROFILE_FUNCTION();
    auto gridView = registry.view<TileGrid, Position>();
    const auto cameraView = registry.view<Camera>();
    auto &activeCamera = cameraView.get(*cameraView.begin());
    activeCamera.UpdateWindowSize(Graphics::Window());

	for (const auto& tile : gridView)
    {
        const auto& grid = gridView.get<TileGrid>(tile);
        const auto& position = gridView.get<Position>(tile);
        SDL_FRect world_tile{position.position.x, position.position.y, static_cast<float>(grid.tileSet->TileWidth()) * grid.scale.x,
                             static_cast<float>(grid.tileSet->TileHeight()) * grid.scale.y};
        auto screenRect = activeCamera.FromWorldToScreenRect(world_tile);
        const Vector2D screenPosition = {screenRect.x, screenRect.y};

        auto j = static_cast<int>(grid.cell.size()) - 1;
        for (const auto &row : grid.cell)
        {
	        auto i = 0;
            for (const auto &id : row)
            {
                if (id)
                {
					SDL_FRect testTile{world_tile.x + i * world_tile.w, world_tile.y + j * world_tile.h, world_tile.w, world_tile.h};
                   	auto testRect = activeCamera.FromWorldToScreenRect(testTile);
                    if (activeCamera.Contains(testRect))
                    {
                    	if(std::find(std::begin(VisibleTiles), std::end(VisibleTiles), Vector2D(i, j)) == std::end(VisibleTiles))
	                    	SDL_SetTextureColorMod(grid.tileSet->Texture(), 128, 128, 128);
                        Graphics::RenderToLayer(grid.layer, grid.tileSet->Texture(), &(*grid.tileSet)[id - 1], &testRect);
                    	SDL_SetTextureColorMod(grid.tileSet->Texture(), 255, 255, 255);
                    	
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
	                            Graphics::DrawRectToLayer(Layer::Debug, &testRect);
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

void LightsRender()
{
	PROFILE_FUNCTION();
	
	const auto playerView = registry.view<Player, Hierarchy, Position, Health, Dash>();
	auto &&[player, hierarchy, health, pos, dash] = registry.get<Player, Hierarchy, Health, Position, Dash>(*playerView.begin());

    auto gridView = registry.view<TileGrid, Position>();
    const auto cameraView = registry.view<Camera>();
    auto &activeCamera = cameraView.get(*cameraView.begin());

	for (const auto& tile : gridView)
    {
        const auto& grid = gridView.get<TileGrid>(tile);
		
		if (grid.layer == Layer::Walls)
		{
			if (!grid.vecEdges.empty() && TileGrid::hasDebugDraw)
			{
				for (const auto& e : grid.vecEdges)
				{
					const auto p1 = activeCamera.FromWorldToScreenView(e.a);
					const auto p2 = activeCamera.FromWorldToScreenView(e.b);

					Graphics::SetDrawColor(0, 255, 0, 255);
					Graphics::DrawLineToLayer(Layer::Debug, static_cast<int>(roundf(p1.x)), static_cast<int>(roundf(p1.y)), static_cast<int>(roundf(p2.x)), static_cast<int>(roundf(p2.y)));
					Graphics::SetDrawColor(255, 0, 0);
					Graphics::DrawFillCircleToLayer(Layer::Debug, static_cast<int>(p1.x), static_cast<int>(p1.y), 4);
					Graphics::DrawFillCircleToLayer(Layer::Debug, static_cast<int>(p2.x), static_cast<int>(p2.y), 4);
					Graphics::ResetDrawColor();
				}
			}

			// Clear the background to our 'dark' alpha
			constexpr  SDL_Color background{0, 0, 0, 96};
			Graphics::LayerClear(Layer::Lights, background );

			std::vector<Vector2D> screenPolygon(grid.vecVisibilityPolygon.size());
			for (auto i = 0; i < static_cast<int>(grid.vecVisibilityPolygon.size()); i++)
			{
				screenPolygon[i] = activeCamera.FromWorldToScreenView(grid.vecVisibilityPolygon[i]);
			}
			SDL_SetRenderDrawBlendMode(Graphics::Renderer(), SDL_BLENDMODE_NONE);
			Graphics::SetDrawColor(255, 255, 255, 0);
			Graphics::DrawFillPolygonToLayer(Layer::Lights, screenPolygon);
			SDL_SetRenderDrawBlendMode(Graphics::Renderer(), SDL_BLENDMODE_BLEND);

			// ** EXPERIMENT **
#ifdef LIGHT_EXPERIMENT			
			{
				int width;
				int height;
				SDL_QueryTexture(Graphics::GetLayerTexture(Layer::Lights), nullptr, nullptr, &width, &height);
				auto* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
				memset(surface->pixels, 0x00000000, width * height * sizeof(uint32_t));

				const auto center = activeCamera.FromWorldToScreenView(pos.position);
				for (auto i = 0; i < grid.vecVisibilityPolygon.size() - 1; i++)
				{
					Graphics::DrawFillTriangleToSurface(surface, center.x, center.y, 
						screenPolygon[i].x, screenPolygon[i].y,
						screenPolygon[i + 1].x, screenPolygon[i + 1].y, 0xFF00FFFF);
				}

				auto xcenter = static_cast<int>(center.x) + 16;
				auto ycenter = static_cast<int>(center.y) - 16;
				Vector2D m(xcenter, ycenter);

				constexpr const auto radius = 256;

				auto xmin = std::max(0, xcenter - radius);
				auto xmax = std::min(width, static_cast<int>(xcenter + radius));
				auto ymin = std::max(0, ycenter - radius);
				auto ymax = std::min(height, static_cast<int>(ycenter + radius));


				// https://docs.blender.org/manual/fr/2.79/render/blender_render/lighting/lights/attenuation.html
				// http://learnwebgl.brown37.net/09_lights/lights_attenuation.html
				constexpr auto lightEnergy = 100.0f;
				constexpr auto constant = 1.0f;
				constexpr auto linear = 0.05f / lightEnergy;
				constexpr auto quadratic = 0.0f / lightEnergy;

				SDL_Renderer *renderer = Graphics::Renderer();
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
				for (auto x = xmin; x < xmax; x++)
					for (auto y = ymin; y < ymax; y++)
					{
						const auto blr = static_cast<uint32_t *>(surface->pixels)[y * surface->w + x];
						if (blr > 256)
						{
							Vector2D pt(x, y);

							const auto d = Vector2D::DistanceSquared(m, pt);
							if (d < radius * radius)
							{
								const auto luminosity = 1.0f / (constant + linear * d + quadratic * (d * d));

								SDL_Color c;
								c.r = static_cast<int>(255.0f * luminosity);
								c.g = static_cast<int>(192.0f * luminosity);
								c.b = static_cast<int>(96.0f * luminosity);
								c.a = static_cast<int>(128 - 127 * luminosity);
								SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
								SDL_RenderDrawPoint(renderer, x, y);
							}
						}
						else
						{
							int a = 10;
						}
					}

				//SDL_UpdateTexture(Graphics::GetLayerTexture(Layer::Lights), nullptr, surface->pixels, surface->pitch);
				SDL_FreeSurface(surface);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				
			}
#endif			
			// ** END EXPERIMENT **


			
			Graphics::ResetDrawColor();
			
			if (TileGrid::hasDebugDraw)
			{
				Graphics::SetDrawColor(0, 255, 255, 255);
				Graphics::DrawPolygonToLayer(Layer::Lights, screenPolygon);
			}
			Graphics::ResetDrawColor();
		}
    }
	FOVCalculate();
}

void FOVCalculate()
{
	PROFILE_FUNCTION();
	static RPGEngine::FOVRecurse fovRecurse;
	
	const auto playerView = registry.view<Player, Hierarchy, Position, Health, Dash>();
	auto &&[player, hierarchy, health, pos, dash] = registry.get<Player, Hierarchy, Health, Position, Dash>(*playerView.begin());

    auto gridView = registry.view<TileGrid, Position>();
    const auto cameraView = registry.view<Camera>();
    auto &activeCamera = cameraView.get(*cameraView.begin());

	fovRecurse.ClearMap();
	
	for (const auto& tile : gridView)
    {
        auto& grid = gridView.get<TileGrid>(tile);
		const auto& tilePos = gridView.get<Position>(tile);
		
        auto j = static_cast<int>(grid.cell.size()) - 1;
        for (const auto &row : grid.cell)
        {
	        auto i = 0;
            for (const auto &id : row)
            {
                if (id)
                {
					if (grid.layer == Layer::Walls)
					{
						fovRecurse.PointSet(i, j, 1);
					}
				}
            i++;
            }
        j--;
        }
		
		if (grid.layer == Layer::Walls)
		{
			const auto playerX = static_cast<int>(pos.position.x) / 32;
			const auto playerY = static_cast<int>(pos.position.y) / 32;

			fovRecurse.MovePlayer(playerX, playerY);
			grid.visibleTiles.clear();
			std::copy(fovRecurse.VisiblePoints.begin(), fovRecurse.VisiblePoints.end(), std::back_inserter(grid.visibleTiles));
			VisibleTiles.clear();
			std::copy(fovRecurse.VisiblePoints.begin(), fovRecurse.VisiblePoints.end(), std::back_inserter(VisibleTiles));
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
        const auto id = registry.create();
        auto &tilegrid = registry.emplace<TileGrid>(id, map, &tileset, 1);
        registry.emplace<Position>(id);
        registry.emplace<TileGridCollider>(id);
        registry.emplace<CollisionLayer>(id, LayersID::FLOOR);
        registry.emplace<Active>(id);
        tilegrid.scale = {1, 1};
        tilegrid.layer = Layer::Floor;
    }

    {
        const auto id = registry.create();
        auto &tilegrid = registry.emplace<TileGrid>(id, map, &tileset, 2);
        registry.emplace<Position>(id);
        registry.emplace<TileGridCollider>(id);
        registry.emplace<CollisionLayer>(id, LayersID::WALLS);
        registry.emplace<Active>(id);
        tilegrid.scale = {1, 1};
        tilegrid.layer = Layer::Walls;
    }
    {
        const auto id = registry.create();
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
