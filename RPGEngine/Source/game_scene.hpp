#pragma once

#include <filesystem>
#include <iostream>
#include <random>

#include "core.hpp"
#include "systems/systems.hpp"
#include "UI/UISystem.h"
#include "xml/pugixml.hpp"

constexpr const auto tileid = "tileset";
constexpr const auto spriteid = "spritesheet";

class GameScene : public BasicScene
{
public:
    ~GameScene() {}
    
    GameScene()
    {
        auto &signal = registry.set<collision_signal>();
        auto &sink = registry.set<collision_sink>(signal);
        sink.connect<&NullVelocity>();
        sink.connect<&EnemyCharging>();
        sink.connect<&OnHit>();

        if (Mix_OpenAudio(44110, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            SDL_THROW();
        }

        auto music = musicCache.load("main", ResourceLoader::Music("resources/mix/main.mp3"));
        Mix_PlayMusic(music, -1);

        // (FIXME) - Quiet this down for now.  This should be a user preference in a config file
        auto vol = Mix_VolumeMusic(3);
        std::cout << "Old Volume :" << vol << std::endl;


        textureCache.load(tileid, ResourceLoader::Sprite("resources/tiled_files/GameTiles.png"));
        textureCache.load(spriteid, ResourceLoader::Sprite("resources/sprites/spritesheet.png"));

        fontCache.load(GetUIFontName(UIFont::Debug), ResourceLoader::Font("resources/fonts/consola.ttf", 20));
        fontCache.load(GetUIFontName(UIFont::Default), ResourceLoader::Font("resources/fonts/segoeui.ttf", 22));

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

        std::shared_ptr<UIPanel> panel = std::make_shared<UIPanel>(50, 50, 200, 200);
        panel->SetActive(true);
        panel->color.a = 128;
        m_uiSystem->AddComponent("Test", panel);

        std::shared_ptr<UILabel> label = std::make_shared<UILabel>(4, 4, "Hello World");
        label->SetActive(true);
        panel->AddChild("HW", label);

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
        //UIPanelsRender();
        //UILabelsRender();
        //LabelsRender();
    }

private:
    std::unique_ptr<UISystem> m_uiSystem = std::make_unique<UISystem>();
};


