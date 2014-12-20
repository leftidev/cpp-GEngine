#include <SDL/SDL.h>

#include "Timing.h"


namespace GEngine {
	Timer::Timer() {
		m_startTicks = 0;
		m_pausedTicks = 0;
		m_paused = false;
		m_started = false;
	}

	Timer::~Timer() { }

	void Timer::start() {
		// Start the timer
		m_started = true;

		// Unpause the timer
		m_paused = false;

		// Get the current clock time
		m_startTicks = SDL_GetTicks();
	}

	void Timer::stop() {
		// Stop the timer
		m_started = false;

		// Unpause the timer
		m_paused = false;
	}

	void Timer::pause() {
		// If the timer is running and isn't already paused
		if ((m_started == true) && (m_paused == false))
		{
			// Pause the timer
			m_paused = true;

			// Calculate the paused ticks
			m_pausedTicks = SDL_GetTicks() - m_startTicks;
		}
	}

	void Timer::unpause() {
		// If the timer is paused
		if (m_paused == true)
		{
			// Unpause the timer
			m_paused = false;

			// Reset the starting ticks
			m_startTicks = SDL_GetTicks() - m_pausedTicks;

			// Reset the paused ticks
			m_pausedTicks = 0;
		}
	}

	int Timer::getTicks() {
		// If the timer is running
		if (m_started == true)
		{
			// If the timer is paused
			if (m_paused == true)
			{
				// Return the number of ticks when the timer was paused
				return m_pausedTicks;
			}
			else
			{
				// Return the current time minus the start time
				return SDL_GetTicks() - m_startTicks;
			}
		}

		// If the timer isn't running
		return 0;
	}

	bool Timer::isStarted() {
		return m_started;
	}

	bool Timer::isPaused() {
		return m_paused;
	}


	FpsLimiter::FpsLimiter() {
	}

	void FpsLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}

	void FpsLimiter::setMaxFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}

	void FpsLimiter::beginFrame() {
		_startTicks = SDL_GetTicks();
	}

	// Return the current FPS
	float FpsLimiter::endFrame() {
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		// Limit the FPS to the max FPS
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}

		return _fps;
	}

	void FpsLimiter::calculateFPS() {
		// The number of frames to average
		static const int NUM_SAMPLES = 10;
		// Stores all the frametimes for each frame that we will average
		static float frameTimes[NUM_SAMPLES];
		// The current frame we are on
		static int currentFrame = 0;
		// The ticks of the previous frame
		static float prevTicks = SDL_GetTicks();

		// Ticks for the current frame.
		float currentTicks = SDL_GetTicks();

		// Calculate the number of ticks (ms) for this frame
		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		// Current ticks is now previous ticks
		prevTicks = currentTicks;

		// The number of frames to average
		int count;

		currentFrame++;

		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		// Average all the frame times
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		// Calculate FPS
		if (frameTimeAverage > 0) {
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}
	}
}