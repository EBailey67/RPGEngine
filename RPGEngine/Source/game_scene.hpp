#pragma once

#include <filesystem>
#include <iostream>
#include <random>

#include "core.hpp"
#include "sdlgui/entypo.h"
#include "Systems/systems.hpp"
#include "sdlgui/button.h"
#include "sdlgui/label.h"
#include "sdlgui/screen.h"
#include "sdlgui/screen.h"
#include "sdlgui/window.h"
// #include "xml/pugixml.hpp"
#include "GameSceneUI.h"

constexpr auto tileid = "tileset";
constexpr auto spriteid = "spritesheet";

class GameScene : public BasicScene
{
public:
	GUI::GameSceneUI* gameUI;

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

		//fontCache.load(GetUIFontName(UIFont::Debug), ResourceLoader::Font("resources/fonts/consola.ttf", 18));
		////	fontCache.load(GetUIFontName(UIFont::Default), ResourceLoader::Font("resources/fonts/consola.ttf", 18));
		//fontCache.load(GetUIFontName(UIFont::Default), ResourceLoader::Font("resources/fonts/kongtext.ttf", 12));

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

		int ww;
		int wh;
		SDL_GetWindowSize(Graphics::Window(), &ww, &wh);
		gameUI = new GUI::GameSceneUI(Graphics::Window(), ww, wh);
	}

	void FixedUpdate() override
	{
		UpdateView();
		CameraFollow();
		CollisionDetection();
		UpdateVisibility();
		HealthUpdate();
	}

	void Update(const float dt) override
	{
		CollisionTileDetection(dt);
		AnimationUpdate(dt);
		MovementUpdate(dt);
		EnemyWalking(dt);
		PlayerMovement(dt);
		PlayerAttack(dt);
		ParticleUpdate(dt);
	}

	void InputUpdate() override
	{
		if (gameUI->onEvent(Events::Event()))
			return;
		CameraUpdateDebug();
		OpenGame();
		DebugMode();
	}

	void Render() override
	{
		GridRender();
		SpriteRender();
		PositionDebug();
		RectDebug();
	}

	virtual void RenderUI() override
	{
		gameUI->drawAll();
	}

	virtual void ResizeEvent() override
	{
		gameUI->resizeCallbackEvent(0, 0);
	}
private:
};


