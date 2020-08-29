
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
	
	const auto playerView = registry.view<Player, Actor, Hierarchy, Position, Health, Dash>();
	auto &&[player, actor, hierarchy, health, pos, dash] = registry.get<Player, Actor, Hierarchy, Health, Position, Dash>(*playerView.begin());

    auto mapView = registry.view<Map, Position>();
	
	for (const auto& currentMap : mapView)
    {
        auto& map = mapView.get<Map>(currentMap);
		if (map.mapLayer == Layer::Floor)
		{
			auto fovRecurse = std::make_shared<RPGEngine::FOVRecurse>(static_cast<int>(pos.position.x), static_cast<int>(pos.position.y), actor.Awareness, &map);
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
	const auto playerView = registry.view<Player, Actor>();
	auto &&[player, actor] =  registry.get<Player, Actor>(*playerView.begin());

	console.Place(1, 2).FgColor(Swatch::Text).Put("Name:    " + player.Name);
	console.Place(1, 4).FgColor(Swatch::Text).Put("Health:  " + std::to_string(actor.Health) + "/" + std::to_string(actor.MaxHealth));
	console.Place(1, 6).FgColor(Swatch::Text).Put("Attack:  " + std::to_string(actor.Attack) + " (" + std::to_string(actor.AttackChance) + "%)");
	console.Place(1, 8).FgColor(Swatch::Text).Put("Defense: " + std::to_string(actor.Defense) + " (" + std::to_string(actor.DefenseChance) + "%)");
	console.Place(1, 10).FgColor(Color::Gold).Put("Gold:     " + std::to_string(actor.Gold));
}

void UpdateMonsterStats(Term::Console& console)
{
    auto mapView = registry.view<Map, Position>();
    auto& map = mapView.get<Map>(*mapView.begin());

	const auto enemyView = registry.view<Monster, Actor, Position>();

	for (auto y = 14; y < 30; y++)
		console.Place(0, y).ClearLine();
	
    auto yPosOffset = 0;
	
	for (auto enemy : enemyView)
	{
		auto &&[kobold, actor, pos] = registry.get<Monster, Actor, Position>(enemy);

		const auto yPosition = 14 + (yPosOffset * 2);

		if (map.cell[static_cast<int>(pos.position.y)][static_cast<int>(pos.position.x)].isInFOV)
		{
			console.Place(1, yPosition).FgColor(kobold.color).Put(kobold.c);
			const auto width = static_cast<int>((static_cast<double>(actor.Health) / static_cast<double>(actor.MaxHealth)) * 16);
			const auto remainingWidth = 16 - width;
			std::string h1(width, ' ');
			std::string h2(remainingWidth, ' ');
			console.Place(3, yPosition).BgColor(Swatch::Primary).Put(h1);
			console.BgColor(Swatch::PrimaryDarkest).Put(h2);
			console.Place(2, yPosition).FgColor(Swatch::DbLight).PutFg(": " + kobold.name);
			
			yPosOffset++;
		}
	}
}