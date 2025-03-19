#pragma once

namespace Feur {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			:m_Time(time) {

		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000; }

	private:
		float m_Time;
	};


	class Time {
	public:
		Time::Time();

	public:
		static const float GetTime();
		static const Timestep GetDeltaTime() { return m_deltaTime; }

		const void UpdateDeltaTime();

	private:
		float m_lastFrameTime;
		static Timestep m_deltaTime;
	};
}
