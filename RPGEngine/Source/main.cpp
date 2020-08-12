#include <iostream>

#include "game_scene.hpp"

#define PROFILING 1
#include "Utility/Instrumentor.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Instrumentor::Get().BeginSession("Game Engine Profile");

	auto* game = Instances::CreateGame();
    try
    {
        game->InitializeSubsystems();
        Game::AssignWindow(
            SDL_CreateWindow("RPG Playground", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        game->CreateScene<GameScene>();
        game->Run();

        fontCache.reset();
        musicCache.reset();
        scoreTable.Save("score.txt");
        Instances::DestroyGame();

		Instrumentor::Get().EndSession();
	    return 0;
    }
    catch (std::exception& e)
    {
        DebugError("Exception", e.what());

        fontCache.reset();
        Instances::DestroyGame();
		Instrumentor::Get().EndSession();
	    return 1;
    }
}
