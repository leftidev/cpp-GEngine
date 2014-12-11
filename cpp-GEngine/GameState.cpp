#include "InputManager.h"
#include "StateMachine.h"
#include "GameState.h"

namespace GEngine {

GameState::GameState(GEngine::StateMachine& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager) :
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

}