#include "Game.h"

SDL_Renderer* Game::renderer = nullptr;

// TileMap* map = nullptr;

// Manager manager;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0, 0, 800, 640 };

//std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;
//auto& player(manager.AddEntity());
const char* mapFile = "Assets/Sprites/terrain_ss.png";

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};


//auto& tiles(manager.GetGroup(groupMap));
//auto& players(manager.GetGroup(groupPlayers));
//auto& enemies(manager.GetGroup(groupEnemies));


Game::Game()
{
	isRunning = false;
	window = nullptr;
	renderer = nullptr;
	counter = 0;
}


Game::~Game()
{

}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen)
{
	isRunning = false;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialized..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, fullScreen ? SDL_WINDOW_FULLSCREEN : 0);
		if (window)
		{
			std::cout << "Window created..." << std::endl;
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				std::cout << "Renderer created..." << std::endl;
				isRunning = true;
			}
		}
	}

	// ECS Implementation
	//TileMap::LoadTileMap("assets/levels/map.map", 25, 20);


	//player.AddComponent<TransformComponent>(4);
	//player.AddComponent<SpriteComponent>("Assets/Sprites/player_anims.png", true);
	//player.AddComponent<KeyboardController>();
	//player.AddComponent<ColliderComponent>("player");
	//player.AddGroup(groupPlayers);

	//map = new TileMap();
}

void Game::HandleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::Update()
{
	//manager.Refresh();
	//manager.Update();

	//camera.x = static_cast<int>(player.GetComponent<TransformComponent>().position.x) - 400;
	//camera.y = static_cast<int>(player.GetComponent<TransformComponent>().position.y) - 320;

	//if (camera.x < 0)
	//	camera.x = 0;
	//if (camera.y < 0)
	//	camera.y = 0;
	//if (camera.x > camera.w)
	//	camera.x = camera.w;
	//if (camera.y > camera.h)
	//	camera.y = camera.h;


	//for (auto cc : colliders)
	//{
	//	Collision::AABB(player.GetComponent<ColliderComponent>(), *cc);
	//}
}


void Game::Render()
{
	SDL_RenderClear(renderer);

	//// add stuff to render here
	//for (auto& t : tiles)
	//{
	//	t->Draw();
	//}

	//for (auto& p : players)
	//{
	//	p->Draw();
	//}

	//for (auto& e : enemies)
	//{
	//	e->Draw();
	//}


	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	std::cout << "Game cleaned..." << std::endl;
}

bool Game::IsRunning()
{
	return isRunning;
}

void Game::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	//auto& tile(manager.AddEntity());
	//tile.AddComponent<TileComponent>(srcX, srcY, xPos, yPos, mapFile);
	//tile.AddGroup(groupMap);
}
