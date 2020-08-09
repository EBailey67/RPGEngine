#pragma once

#include <filesystem>
#include <iostream>
#include <random>

#include "core.hpp"
#include "Systems/systems.hpp"
#include "UI/UISystem.h"
// #include "xml/pugixml.hpp"

constexpr auto tileid = "tileset";
constexpr auto spriteid = "spritesheet";

class GameScene : public BasicScene
{
public:
	std::shared_ptr<UILabel> label_score;
	std::shared_ptr<UILabel> label_health;
	std::shared_ptr<UILabel> label_FPS;

	GameScene(const GameScene&) = delete;
	GameScene& operator=(const GameScene&) = delete;
	~GameScene() = default;

	GameScene()
	{
		auto& signal = registry.set<collision_signal>();
		auto& sink = registry.set<collision_sink>(signal);
		sink.connect<&NullVelocity>();
		sink.connect<&EnemyCharging>();
		sink.connect<&OnHit>();

		if (Mix_OpenAudio(44110, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			SDL_THROW();
		}

		auto* const music = musicCache.load("main", ResourceLoader::Music("resources/mix/main.mp3"));
		Mix_PlayMusic(music, -1);

		// (FIXME) - Quiet this down for now.  This should be a user preference in a config file
		const auto vol = Mix_VolumeMusic(1);
		std::cout << "Old Volume :" << vol << std::endl;

		textureCache.load(tileid, ResourceLoader::Sprite("resources/tiled_files/GameTiles.png"));
		textureCache.load(spriteid, ResourceLoader::Sprite("resources/sprites/spritesheet.png"));

		fontCache.load(GetUIFontName(UIFont::Debug), ResourceLoader::Font("resources/fonts/consola.ttf", 18));
	//	fontCache.load(GetUIFontName(UIFont::Default), ResourceLoader::Font("resources/fonts/consola.ttf", 18));
		fontCache.load(GetUIFontName(UIFont::Default), ResourceLoader::Font("resources/fonts/kongtext.ttf", 12));

		scoreTable.Open("resources/score.txt");

		spriteSheet.Load("resources/tiled_files/sprite.tsx", textureCache.resource(spriteid));
		tileset.Load("resources/tiled_files/RPG Test.tsx", textureCache.resource(tileid));

		CameraCreate();
		GridCreate();
		PlayerCreate();

		EnemyCreate(Enemy::spawns[0]);
		EnemyCreate(Enemy::spawns[1]);
		EnemyCreate(Enemy::spawns[2]);
		EnemyCreate(Enemy::spawns[3]);
		EnemyCreate(Enemy::spawns[4]);
		EnemyCreate(Enemy::spawns[5]);
		Enemy::currentSpawn = 6;

		auto panel = std::make_shared<UIPanel>(50, 50, 200, 200);
		panel->SetActive(true);
		panel->color.a = 128;
		m_uiSystem->AddComponent("Test", panel);

		auto label = std::make_shared<UILabel>(4, 4, "Hello World\nGoodbye World");
		label->SetActive(true);
		panel->AddChild("HW", label);

		label_score = std::make_shared<UILabel>(4, 46, "Score : 0");
		label_score->SetActive(true);
		panel->AddChild("Score", label_score);

		label_health = std::make_shared<UILabel>(4, 66  , "Health : 0");
		label_health->SetActive(true);
		panel->AddChild("Health", label_health);

		label_FPS = std::make_shared<UILabel>(4, 86, "FPS : 0");
		label_FPS->SetActive(true);
		panel->AddChild("FPS", label_FPS);

		// CreateLabels();
	}

	void FixedUpdate() override
	{
		UpdateView();
		CameraFollow();
		CollisionDetection();
		HealthUpdate();
		m_uiSystem->OnFixedUpdate();
	}

	void Update(const float dt) override
	{
		auto* game = Instances::GetGameInstance();
		auto* gs = dynamic_cast<GameScene*>(game->Scene());
		gs->label_FPS->SetText("FPS :" + std::to_string(game->fps_counter.GetFrameRate()));

		CollisionTileDetection(dt);
		AnimationUpdate(dt);
		MovementUpdate(dt);
		EnemyWalking(dt);
		PlayerMovement(dt);
		PlayerAttack(dt);
		ParticleUpdate(dt);
		m_uiSystem->OnUpdate(dt);
	}

	void InputUpdate() override
	{
		CameraUpdateDebug();
		OpenGame();
		DebugMode();
		m_uiSystem->OnInputUpdate();
	}

	void Render() override
	{
		GridRender();
		SpriteRender();
		PositionDebug();
		RectDebug();
		m_uiSystem->OnRender();
	}

	
private:
	std::unique_ptr<UISystem> m_uiSystem = std::make_unique<UISystem>();
};


