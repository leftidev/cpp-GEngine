#pragma once


namespace GEngine {

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