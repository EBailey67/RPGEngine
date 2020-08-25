
#include <memory>

#include "playersystem.h"

#include "../core.hpp"
#include "../Utility/FOVRecurse.h"
#include "../Utility/Visibility.h"

void UpdatePlayerFOV()
{
	PROFILE_FUNCTION();
	
	const auto playerView = registry.view<Player, Hierarchy, Position, Health, Dash>();
	auto &&[player, hierarchy, health, pos, dash] = registry.get<Player, Hierarchy, Health, Position, Dash>(*playerView.begin());

    auto mapView = registry.view<Map, Position>();
	
	for (const auto& currentMap : mapView)
    {
        auto& map = mapView.get<Map>(currentMap);
		if (map.mapLayer == Layer::Floor)
		{
	        auto fovRecurse = std::make_shared<RPGEngine::FOVRecurse>(static_cast<int>(pos.position.x), static_cast<int>(pos.position.y), 7, &map);
			
			fovRecurse->CalculateFOV();
			map.fovCells.clear();

			std::copy(fovRecurse->VisiblePoints.begin(), fovRecurse->VisiblePoints.end(), std::back_inserter(map.fovCells));
		}
    }
}
