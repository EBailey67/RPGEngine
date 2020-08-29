#include "actorsystem.h"

#include "../core.hpp"
#include "../Component/transform.hpp"
#include "../Dice/diceparser.h"
#include "../SDL/game.hpp"

void CreateKobold(const int level, const Vector2Di location)
{
	auto& dice = Game::GetInstance()->Dice();

	auto id = registry.create();
	auto& k = registry.emplace<Monster>(id, 'k', Color::Khaki, "Kobold");

	auto& a = registry.emplace<Actor>(id);
	a.Health = dice.Parse("2d5");
	a.Attack = dice.Parse("1d3").ToInt() + level / 3;
	a.AttackChance = dice.Parse("25d3");
	a.Awareness = 5;
	a.Defense = dice.Parse("1d3").ToInt() + level / 3;
	a.DefenseChance = dice.Parse("10d4");
	a.Gold = dice.Parse("5d5");
	a.MaxHealth = a.Health;
	a.Speed = 14;
		
	auto &pos = registry.emplace<Position>(id);
	pos.position.x = static_cast<float>(location.x);
	pos.position.y = static_cast<float>(location.y);

	return;
}


