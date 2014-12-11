#include "StateMachine.h"


StateMachine::StateMachine() {
	// Empty
}

StateMachine::~StateMachine() {
	// Empty
}

void StateMachine::changeState(GameState* gameState) {
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

void StateMachine::updateInputManager() {
	_gameStates.back()->updateInputManager();
}

void StateMachine::update(float deltaTime) {
	_gameStates.back()->update(deltaTime);
}

void StateMachine::updateCamera() {
	_gameStates.back()->updateCamera();
}

void StateMachine::processEvents() {
	_gameStates.back()->processEvents();
}

void StateMachine::draw() {
	_gameStates.back()->draw();
}
