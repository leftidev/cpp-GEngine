#include "InputManager.h"
#include "StateManager.h"
#include "GameState.h"

namespace GEngine {

GameState::GameState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager) :
	_stateMachine(stateMachine),
	_window(window),
	_inputManager(inputManager) {
}

GameState::~GameState() {
}

void GameState::updateInputManager() {
	_inputManager.update();
}

void GameState::changeState(StateManager& machine, GameState* state) {
	machine.changeState(state);
}

}