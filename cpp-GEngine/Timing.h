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
		int get_ticks();

		// Checks the status of the timer
		bool is_started();
		bool is_paused();

	private:
		// The clock time when the timer started
		int startTicks;

		// The ticks stored when the timer was paused
		int pausedTicks;

		// The timer status
		bool paused;
		bool started;
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