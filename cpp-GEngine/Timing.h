#pragma once


namespace GEngine {
	class Timer {
	public:
		Timer();
		~Timer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		// Gets the timer's time
		int getTicks();

		// Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		// The clock time when the timer started
		int m_startTicks;

		// The ticks stored when the timer was paused
		int m_pausedTicks;

		// The timer status
		bool m_paused;
		bool m_started;
	};

	class FpsLimiter {
	public:
		FpsLimiter();

		void init(float maxFPS);
		void setMaxFPS(float maxFPS);
		void beginFrame();

		// Return the current FPS
		float endFrame();

	private:
		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;

		void calculateFPS();
};
}