#include "FramerateCounter.h"


FramerateCounter::FramerateCounter(void) :
	m_frameCounter(0),
	m_frameRate(0.0f),
	m_elapsedTime(0.0f)
{
}

void FramerateCounter::Update(const float deltaTime)
{
	// Update the scene stats once per second
	if (m_elapsedTime > 1.0f)
	{
		m_frameRate = static_cast<float>(m_frameCounter) / (m_elapsedTime);
		m_elapsedTime = 0;
		m_frameCounter = 0;
	}
	else
	{
		m_frameCounter++;
		m_elapsedTime += deltaTime;
	}
}