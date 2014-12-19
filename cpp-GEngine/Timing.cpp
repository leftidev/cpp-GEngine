#include <SDL/SDL.h>

#include "Timing.h"


namespace GEngine {
	Timer::Timer() {
		startTicks = 0;
		pausedTicks = 0;
		paused = false;
		started = false;
	}

	Timer::~Timer() { }

	void Timer::start() {
		// Start the timer
		started = true;

		// Unpause the timer
		paused = false;

		// Get the current clock time
		startTicks = SDL_GetTicks();
	}

	void Timer::stop() {
		// Stop the timer
		started = false;

		// Unpause the timer
		paused = false;
	}

	void Timer::pause() {
		// If the timer is running and isn't already paused
		if ((started == true) && (paused == false))
		{
			// Pause the timer
			paused = true;

			// Calculate the paused ticks
			pausedTicks = SDL_GetTicks() - startTicks;
		}
	}

	void Timer::unpause() {
		// If the timer is paused
		if (paused == true)
		{
			// Unpause the timer
			paused = false;

			// Reset the starting ticks
			startTicks = SDL_GetTicks() - pausedTicks;

			// Reset the paused ticks
			pausedTicks = 0;
		}
	}

	int Timer::get_ticks() {
		// If the timer is running
		if (started == true)
		{
			// If the timer is paused
			if (paused == true)
			{
				// Return the number of ticks when the timer was paused
				return pausedTicks;
			}
			else
			{
				// Return the current time minus the start time
				return SDL_GetTicks() - startTicks;
			}
		}

		// If the timer isn't running
		return 0;
	}

	bool Timer::is_started() {
		return started;
	}

	bool Timer::is_paused() {
		return paused;
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