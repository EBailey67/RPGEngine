#pragma once

#include "fwd.hpp"

class Instances
{

public:
    /**
     * @brief Get game instance.
     */
    static Game *GetGameInstance();

    /**
     * @brief Create game instance.
     */
    static Game *CreateGame();

    /**
     * @brief Destroy game instance if it exist.
     */
    static void DestroyGame();

private:
    inline static Game *m_gameInstance = nullptr;
};

