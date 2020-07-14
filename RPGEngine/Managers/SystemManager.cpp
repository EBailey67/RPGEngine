#include "SystemManager.h"
#include <iostream>

SDL_Renderer* SystemManager::renderer = nullptr;
SDL_Event SystemManager::event;

void SystemManager::Activate()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        if (TTF_Init() == 0)
        {
            std::cout << "Subsystems Initialized..." << std::endl;
            window = SDL_CreateWindow("RPG Playground", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false /* fullScreen */);
            if (window)
            {
                std::cout << "Window created..." << std::endl;
                renderer = SDL_CreateRenderer(window, -1, 0);
                if (renderer)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    std::cout << "Renderer created..." << std::endl;

                    uiFont = TTF_OpenFont("Fonts/arial.ttf", 25);
                    if (!uiFont)
                    {
                        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
                    }
                    SDL_Color color = { 255, 0, 0 };
                    SDL_Surface* uiSurface = TTF_RenderText_Blended(uiFont, "Welcome to the RPG Engine!", color);
                    uiTexture = SDL_CreateTextureFromSurface(renderer, uiSurface);
                    SDL_FreeSurface(uiSurface);
                    isRunning = true;
                }
            }
        }
    }

    Update(window);
}


void SystemManager::Update(SDL_Window* window)
{
    Uint32 frameStart = SDL_GetTicks();
    float deltaTime = 0.0f;

    while (isRunning)
    {
        // deltaTime is referring to the elapsed seconds between two frames (typically < 1.0)
        deltaTime = static_cast<float>(SDL_GetTicks() - frameStart) / 1000.0f;
        frameStart = SDL_GetTicks();

        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
        }
        
        // The window is cleared.
        SDL_RenderClear(renderer);

        SDL_Rect destRect = { 0, 0, 0, 0 };
        SDL_QueryTexture(uiTexture, nullptr, nullptr, &destRect.w, &destRect.h);
        SDL_RenderCopy(renderer, uiTexture, NULL, &destRect);

        // The systems will then be updated.
        //InputSystem.Update(Registry, deltaTime);
        //PhysicsSystem.Update(Registry, deltaTime);
        //RenderSystem.Update(Registry);

        // Then they will be drawn.
//        RenderSystem.Draw(Registry, Window);
        //Grid.Draw(Window);
        //TopBox.Draw(Window);
        //BottomBox.Draw(Window);

        // Then they will display. 
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(uiTexture);
    TTF_CloseFont(uiFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
