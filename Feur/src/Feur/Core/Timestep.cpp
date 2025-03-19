#include "fpch.h"
#include "Timestep.h"

#include <GLFW/glfw3.h>

namespace Feur {
	Timestep Time::m_deltaTime = 0.0f;

	Time::Time()
		: m_lastFrameTime(GetTime()) { }

	const float Time::GetTime()
	{
		return (float)glfwGetTime();
	}
	const void Time::UpdateDeltaTime()
	{
		float currentTime = GetTime();
		m_deltaTime = currentTime - m_lastFrameTime;
		m_lastFrameTime = currentTime;
	}
}