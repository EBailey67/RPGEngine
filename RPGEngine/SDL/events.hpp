#pragma once

#include <SDL_events.h>

#include "fwd.hpp"

class Events
{
    friend Game;

public:
    static const SDL_Event &Event() noexcept
    {
        return m_event;
    }

    static const Uint8 *KeyboardState() noexcept
    {
        return SDL_GetKeyboardState(NULL);
    }

private:
    static inline SDL_Event m_event{};
};

