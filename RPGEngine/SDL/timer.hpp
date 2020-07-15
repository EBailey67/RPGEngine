#pragma once

#include <SDL.h>

// Timer built on SDL_GetTicks()
class Timer
{
private:
    Uint32 m_previous{};

public:
    // Return ticks from init SDL subsystems
    static Uint32 Ticks()
    {
        return SDL_GetTicks();
    }

    Uint32 Started()
    {
        return m_previous;
    }

    // Return time since last call elapsed.
    Uint32 Elapsed()
    {
        return SDL_GetTicks() - m_previous;
    }

    // Start recording
    void Record()
    {
        m_previous = SDL_GetTicks();
    }
};
