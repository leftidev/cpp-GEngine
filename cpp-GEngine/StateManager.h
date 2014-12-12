#pragma once

#include <vector>

#include "GameState.h"


namespace GEngine {

class StateManager {
public:
	StateManager();
	~StateManager();
	// Runs the game
	void changeState(GameState* gameState);
	// Input manager update
	void updateInputManager();
	// Main game loop for the program
	void update(float deltaTime);
	// Handles input processing
	void processEvents();
	// Update camera
	void updateCamera();
	// Renders the game
	void draw();
	bool running() { return _running; }
	void quit() { _running = false; }

private:
	// The stack of states
	std::vector<GameState*> _gameStates;
	bool _running;
};

}