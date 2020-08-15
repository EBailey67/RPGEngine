#include "Timer.h"


namespace RPGEngine
{
	Timer* Timer::m_instance = nullptr;
	
	Timer* Timer::GetInstance()
	{
		if (m_instance == nullptr)
			m_instance = new Timer();

		return m_instance;
	}

	void Timer::Release()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	void Timer::Reset()
	{
		m_startTime = std::chrono::system_clock::now();
	}

	float Timer::DeltaTime() const
	{
		return m_deltaTime.count();
	}

	void Timer::TimeScale(const float t)
	{
		m_timeScale = t;
	}

	float Timer::TimeScale() const
	{
		return m_timeScale;
	}

	void Timer::Tick()
	{
		m_deltaTime = std::chrono::system_clock::now() - m_startTime;
	}

	Timer::Timer()
	{
		Reset();
		m_timeScale = 1.0f;
		m_deltaTime = std::chrono::duration<float>(0.0f);
	}
}
