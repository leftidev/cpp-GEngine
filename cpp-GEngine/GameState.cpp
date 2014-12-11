#include "InputManager.h"
#include "StateMachine.h"
#include "GameState.h"


GameState::GameState(StateMachine& stateMachine, Window& window, InputManager& inputManager) :
	_stateMachine(stateMachine),
	_window(window),
	_inputManager(inputManager) {
}

GameState::~GameState() {
}

void GameState::updateInputManager() {
	_inputManager.update();
}

void GameState::changeState(StateMachine& machine, GameState* state) {
	machine.changeState(state);
}
