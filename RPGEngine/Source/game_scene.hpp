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
#include "Systems/MapSystem.h"
#include "Terminal/sdl_context.hpp"
#include "Utility/Swatch.h"

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
		const auto vol = Mix_VolumeMusic(0);
		std::cout << "Old Volume :" << vol << std::endl;

		textureCache.load(tileid, ResourceLoader::Sprite("resources/tiled_files/GameTiles.png"));
		textureCache.load(spriteid, ResourceLoader::Sprite("resources/sprites/spritesheet.png"));

		scoreTable.Open("resources/score.txt");

		spriteSheet.Load("resources/tiled_files/sprite.tsx", textureCache.resource(spriteid));
		tileset.Load("resources/tiled_files/RPG Test.tsx", textureCache.resource(tileid));

		CameraCreate();
		GridCreate();
		MapCreate(80, 48);
		PlayerCreate();

		//EnemyCreate(Enemy::spawns[0]);
		//EnemyCreate(Enemy::spawns[1]);
		//EnemyCreate(Enemy::spawns[2]);
		//EnemyCreate(Enemy::spawns[3]);
		//EnemyCreate(Enemy::spawns[4]);
		//EnemyCreate(Enemy::spawns[5]);
		//Enemy::currentSpawn = 6;

		int ww;
		int wh;
		SDL_GetWindowSize(Graphics::Window(), &ww, &wh);
		gameUI = new GUI::GameSceneUI(Graphics::Window(), ww, wh);

		Term::CharCell clearChar('\0', Color::Black, Color::White);

		// ==== MAP CONSOLE ====
		{
		auto& console = map_console.GetConsole();
		console.FgColor(Color::White).BgColor(Color::Black);
		console.ClearChar(clearChar);
		console.Clear();
		console.Place(0, 0).Put("Map");
		}

		// ==== MESSAGE CONSOLE ====
		{
		auto& console = message_console.GetConsole();
		console.Set(Term::Console::VScroll, true);
		clearChar.BgColor(Swatch::DbDeepWater);
		clearChar.FgColor(Color::White);
		console.FgColor(Color::White);
		console.BgColor(Swatch::DbDeepWater);
		console.ClearChar(clearChar);
		console.Clear();
		console.Place(0, 0).Put("Message");
		}

		// ==== STATS CONSOLE ====
		{
		auto& console = stat_console.GetConsole();
		clearChar.FgColor(Color::White);
		clearChar.BgColor(Swatch::DbOldStone);
		console.FgColor(Color::White).BgColor(Swatch::DbOldStone);
		console.ClearChar(clearChar);
		console.Clear();
		console.Place(0, 0).Put("Stats");
		}

		// ==== INVENTORY CONSOLE ==== 
		{
		auto& console = inventory_console.GetConsole();
		clearChar.FgColor(Color::White);
		clearChar.BgColor(Swatch::DbWood);
		console.FgColor(Color::White).BgColor(Swatch::DbWood);
		console.ClearChar(clearChar);
		console.Clear();
		console.Place(0, 0).Put("Inventory");
		}
	}

	void FixedUpdate() override
	{
		UpdateView();
		CameraFollow();
		CollisionDetection();
		UpdateVisibility();
		HealthUpdate();

		const auto frameRate = static_cast<int>(floorf(Game::GetInstance()->fps_counter.GetFrameRate() * 100 + 0.5f) / 100.0f);
		message_console.GetConsole().FgColor(Color::White).Place(0, 1).Put("Framerate: " + std::to_string(frameRate));

		map_console.Print();
		message_console.Print();
		stat_console.Print();
		inventory_console.Print();
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
		PROFILE_SCOPE("Render Pass");
		{
		GridRender();
		MapRender(map_console);
		SpriteRender();
		// LightsRender();
		PositionDebug();
		RectDebug();
		RenderUI();
		
		}
	}

	void RenderUI() override
	{
		inventory_console.Render(0, 0);
		map_console.Render(0, inventory_console.TileHeight() * inventory_height);
		message_console.Render(0, inventory_console.TileHeight() * inventory_height + map_console.TileHeight() * map_height);
		stat_console.Render(inventory_console.TileWidth() * inventory_width, 0);
		gameUI->drawAll();
	}

	virtual void ResizeEvent() override
	{
		gameUI->resizeCallbackEvent(0, 0);
		inventory_console.GetConsole().Dirty();
		map_console.GetConsole().Dirty();
		message_console.GetConsole().Dirty();
		stat_console.GetConsole().Dirty();
	}
private:
	const int map_width = 80;
	const int map_height = 48;
	Term::SDL::Context map_console = Term::SDL::Context( map_width, map_height );

	const int message_width = 80;
	const int message_height = 11;
	Term::SDL::Context message_console = Term::SDL::Context( message_width, message_height );

	const int stat_width = 20;
	const int stat_height = 70;
	Term::SDL::Context stat_console = Term::SDL::Context( stat_width, stat_height );
	
	const int inventory_width = 80;
	const int inventory_height = 11;
	Term::SDL::Context inventory_console = Term::SDL::Context( inventory_width, inventory_height );
};
