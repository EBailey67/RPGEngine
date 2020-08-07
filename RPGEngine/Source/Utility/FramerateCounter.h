#pragma once

class FramerateCounter
{
public:
	FramerateCounter();

	void Update(const float deltaTime);

	[[nodiscard]] float GetFrameRate() const
	{
		return m_frameRate;
	}
	
protected:
	int m_frameCounter;
	float m_frameRate;
	float m_elapsedTime;
};
