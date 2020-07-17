#pragma once

#include "fwd.hpp"

class Instances
{
public:
    static Game *GetGameInstance();
    static Game *CreateGame();
    static void DestroyGame();

private:
    inline static Game *m_gameInstance = nullptr;
};

