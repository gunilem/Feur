#pragma once

#include <chrono>

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

	template<typename Fn>
	class Timer {
	public:
		Timer(const char* name, Fn&& callback)
			: m_Name(name), m_Callback(callback), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer() {
			if (!m_Stopped)
				Stop();
		}

		void Stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			auto duration = (end - start) * 0.001f;

			m_Callback({ m_Name, duration });
		}

	private:
		const char* m_Name;
		Fn m_Callback;

		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}
