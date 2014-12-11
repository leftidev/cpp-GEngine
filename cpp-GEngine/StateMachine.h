#pragma once

#include <vector>

#include "GameState.h"


class StateMachine
{
public:
	StateMachine();
	~StateMachine();
	// Runs the game
	void changeState(GameState* gameState);
	// Input manager update
	void inputManagerUpdate();
	// Main game loop for the program
	void update();
	// Handles input processing
	void processEvents();
	// Renders the game
	void draw();
	bool running() { return _running; }
	void quit() { _running = false; }
private:
	// The stack of states
	std::vector<GameState*> _gameStates;
	bool _running;


	//GEngine::InputManager _inputManager; // Handles input
};
