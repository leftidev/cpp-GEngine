#include "StateManager.h"


namespace GEngine {

StateManager::StateManager() {
	// Empty
}

StateManager::~StateManager() {
	// Empty
}

void StateManager::changeState(GameState* gameState) {
	if (!_running) {
		_running = true;
	}

	// cleanup the current state
	if (!_gameStates.empty()) {
		_gameStates.pop_back();
	}

	// store and init the new state
	_gameStates.push_back(gameState);
	_gameStates.back()->init();
}

void StateManager::updateInputManager() {
	_gameStates.back()->updateInputManager();
}

void StateManager::update(float deltaTime) {
	_gameStates.back()->update(deltaTime);
}

void StateManager::updateCamera() {
	_gameStates.back()->updateCamera();
}

void StateManager::processEvents() {
	_gameStates.back()->processEvents();
}

void StateManager::draw() {
	_gameStates.back()->draw();
}

}