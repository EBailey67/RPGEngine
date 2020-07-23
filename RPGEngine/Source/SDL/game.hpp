#pragma once

#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "config.hpp"
#include "framerate.hpp"
#include "scene.hpp"
#include "timer.hpp"

#include "events.hpp"
#include "graphics.hpp"

#include "../UI/UISystem.h"

class Game
{
    friend Instances;

private:
    Game() {}
    ~Game() {}

    void InputUpdate()
    {
        SDL_PollEvent(&Events::m_event);

        // Check for terminate.
        if (Events::Event().type == SDL_QUIT)
        {
            Quit();
        }

        if (Events::Event().window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            Graphics::DestroyLayers();
            Graphics::CreateLayers();
        }

        m_scene->InputUpdate();
    }

    void Update(const float dt)
    {
        m_scene->Update(dt);
    }

    void FixedUpdate()
    {
        m_scene->FixedUpdate();
    }

    void Render()
    {
        Graphics::OnRenderStart();

        m_scene->Render();

        Graphics::DrawLayers();
        Graphics::RenderPresent();
    }

    /* Destroy window and renderer.
     * Close all subsystems. */
    void Destroy()
    {
        Graphics::DestroyData();

        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

public:

    // Initialization SDL's subsystems. 
    void InitializeSubsystems()
    {
        if (m_hadInitialization)
        {
            return;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || TTF_Init() == -1 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0 ||
            Mix_Init(MIX_INIT_MP3) == 0)
        {
            SDL_THROW();
        }

        m_hadInitialization = true;
    }
    
    bool HadInitialization() const noexcept
    {
        return m_hadInitialization;
    }
    
    // Assign the window to graphic engine and create render from it.
    // Also create all the render layers
    void AssignWindow(SDL_Window *window, const Uint32 rendererFlags = 0)
    {
        Graphics::AssignWindow(window, rendererFlags);
    }

    
    void Run()
    {
        m_isRunning = true;

        while (m_isRunning)
        {

            m_frameRate.OnFrameStart();

            FixedUpdate();

            // Variable-timed update:
            Update(m_frameRate.dt);

            InputUpdate();

            Render();
        }
    }

    void Quit() noexcept
    {
        m_isRunning = false;
    }

    bool IsRunning() const noexcept
    {
        return m_isRunning;
    }

    const auto Scene() const noexcept
    {
        return m_scene.get();
    }

    
    template <typename TScene, typename... Args>
    auto CreateScene(Args... args)
    {
        m_scene = std::make_unique<TScene>(std::forward<Args>(args)...);
    }

private:
    bool m_hadInitialization = false;
    bool m_isRunning = false;
    FrameRate m_frameRate;
    std::unique_ptr<BasicScene> m_scene = std::make_unique<BasicScene>();
};

