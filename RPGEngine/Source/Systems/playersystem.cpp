
#include <memory>

#include "playersystem.h"

#include "../core.hpp"
#include "../Terminal/Console.h"
#include "../Utility/FOVRecurse.h"
#include "../Utility/Swatch.h"
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
			auto fovRecurse = std::make_shared<RPGEngine::FOVRecurse>(static_cast<int>(pos.position.x), static_cast<int>(pos.position.y), player.Awareness, &map);
			fovRecurse->CalculateFOV();
			for (auto& row: map.cell)
				for (auto& col : row)
				{
					if (col.isInFOV)
					{
						col.isInFOV = false;
						col.isDirty = true;
						col.isExplored = true;
					}
				}

			for (auto& pt_fov: fovRecurse->VisiblePoints)
			{
				map.cell[pt_fov.y][pt_fov.x].isInFOV = true;
				map.cell[pt_fov.y][pt_fov.x].isDirty = true;
			}
		}
    }
}


void UpdatePlayerStats(Term::Console& console)
{
	const auto playerView = registry.view<Player>();
	auto &&player =  registry.get<Player>(*playerView.begin());

	console.Place(1, 2).FgColor(Swatch::Text).Put("Name:    " + player.Name);
	console.Place(1, 4).FgColor(Swatch::Text).Put("Health:  " + std::to_string(player.Health) + "/" + std::to_string(player.MaxHealth));
	console.Place(1, 6).FgColor(Swatch::Text).Put("Attack:  " + std::to_string(player.Attack) + " (" + std::to_string(player.AttackChance) + "%)");
	console.Place(1, 8).FgColor(Swatch::Text).Put("Defense: " + std::to_string(player.Defense) + " (" + std::to_string(player.DefenseChance) + "%)");
	console.Place(1, 10).FgColor(Color::Gold).Put("Gold:     " + std::to_string(player.Gold));
}
