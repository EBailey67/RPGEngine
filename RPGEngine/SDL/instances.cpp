#include "instances.hpp"

#include "game.hpp"

Game *Instances::GetGameInstance()
{
    return m_gameInstance;
}

Game *Instances::CreateGame()
{
    if (!m_gameInstance)
    {
        m_gameInstance = new Game();
    }
    return m_gameInstance;
}

void Instances::DestroyGame()
{

    if (m_gameInstance)
    {
        m_gameInstance->Destroy();
    }
}
