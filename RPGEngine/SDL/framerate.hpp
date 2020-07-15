#pragma once

#include "fwd.hpp"
#include "timer.hpp"

class FrameRate
{
    friend Game;

public:
    void OnFrameStart()
    {
        m_deltaTime = static_cast<float>(m_timer.Elapsed() / 1000.f);
        m_timer.Record();

        dt = m_hasFrameLock ? (1.0f / m_targetFrameRate) : m_deltaTime;
    }

private:
    float dt;

    float m_targetFrameRate = 60;
    bool m_hasFrameLock = true;
    float m_deltaTime = 0;

    Timer m_timer;
};
