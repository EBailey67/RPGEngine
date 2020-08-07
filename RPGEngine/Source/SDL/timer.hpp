#pragma once

#include <SDL.h>

/**
 * \brief Timer buildon SDL_GetTicks()
 */
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

    [[nodiscard]] Uint32 Started() const
    {
        return m_previous;
    }

    // Return time since last call elapsed.
    [[nodiscard]] Uint32 Elapsed() const
    {
        return SDL_GetTicks() - m_previous;
    }

    // Start recording
    void Record()
    {
        m_previous = SDL_GetTicks();
    }
};
