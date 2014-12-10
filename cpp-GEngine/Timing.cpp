#include "Timing.h"

#include <SDL/SDL.h>

namespace GEngine
{

FpsLimiter::FpsLimiter()
{
}

void FpsLimiter::init(float maxFPS)
{
    setMaxFPS(maxFPS);
}

void FpsLimiter::setMaxFPS(float maxFPS)
{
    _maxFPS = maxFPS;
}

void FpsLimiter::beginFrame()
{
    _startTicks = SDL_GetTicks();
}

// Return the current FPS.
float FpsLimiter::endFrame()
{
    calculateFPS();

    float frameTicks = SDL_GetTicks() - _startTicks;
    // Limit the FPS to the max FPS.
    if (1000.0f / _maxFPS > frameTicks)
    {
        SDL_Delay(1000.0f / _maxFPS - frameTicks);
    }

    return _fps;
}

void FpsLimiter::calculateFPS()
{
    // The number of frames to average.
    static const int NUM_SAMPLES = 10;
    // Stores all the frametimes for each frame that we will average.
    static float frameTimes[NUM_SAMPLES];
    // The current frame we are on
    static int currentFrame = 0;
    // The ticks of the previous frame
    static float prevTicks = SDL_GetTicks();

    // Ticks for the current frame.
    float currentTicks = SDL_GetTicks();

    // Calculate the number of ticks (ms) for this frame.
    _frameTime = currentTicks - prevTicks;
    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    // Current ticks is now previous ticks.
    prevTicks = currentTicks;

    // The number of frames to average.
    int count;

    currentFrame++;

    if (currentFrame < NUM_SAMPLES)
    {
        count = currentFrame;
    }
    else
    {
        count = NUM_SAMPLES;
    }

    // Average all the frame times.
    float frameTimeAverage = 0;
    for (int i = 0; i < count; i++)
    {
        frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    // Calculate FPS.
    if (frameTimeAverage > 0)
    {
        _fps = 1000.0f / frameTimeAverage;
    }
    else
    {
        _fps = 60.0f;
    }
}

}