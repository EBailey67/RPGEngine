#pragma once
#include "../Utility/Color.h"
#include "../Utility/Vector2D.h"

struct Actor
{
	int Attack = 2;
	int AttackChance = 50;
	int Awareness = 15;
	int Defense = 2;
	int DefenseChance = 40;
	int Gold = 0;
	int Health = 100;
	int MaxHealth = 100;
	int Speed = 10;
};

struct Player
{
    bool isAttacking = true;
    bool isHit = true;
    Vector2Df attack_direction;
    float attack_rate = 0.5;
    float dt = 0;
    int score = 0;
	std::string Name = "Luven Lightfinger";
};


struct Monster
{
	char c = '!';
	Color color = Color::Red;
	std::string name = "FIXME";
};


struct Dash
{
    bool canDashing = true;
    float duration = 0.1f;
    float power = 3.f;
    float cd = 2.f;
    float dt = 0;
};



struct Enemy
{
    inline static int currentSpawn = 0;
    inline static constexpr auto MAX_SPAWNS = 8;
    inline static const Vector2Df spawns[MAX_SPAWNS]
    {
        {250.0f, 250.0f},
        {500.0f, 500.0f},
        {100.0f, 400.09f},
        {974.667847f, 716.000488f},
        {600.0f, 140.0f},
        {1100.667603f, 271.333405f},
        {1135.333862f, 786.667297f},
        {1135.333862f, 200.0f},
    };
    bool isCharging = false;
    float time = 0.5;
    float dt = 0;
};

struct Health
{
    int health = 2;
};
