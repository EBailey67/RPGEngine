#pragma once

#include <chrono>

namespace RPGEngine
{
	class Timer
	{
	public:
		static Timer* Instance();
		static void Release();

		void Reset();
		[[nodiscard]] float DeltaTime() const;
		[[nodiscard]] float TimeScale() const;
		void TimeScale(float t = 1.0f);
		void Tick();

	private:
		static Timer* m_instance;
		std::chrono::system_clock::time_point m_startTime;
		std::chrono::duration<float> m_deltaTime{};
		float m_timeScale;

		Timer();
		~Timer() = default;
	};
}
