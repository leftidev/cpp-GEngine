#include "InputManager.h"
#include "StateManager.h"
#include "GameState.h"

namespace GEngine {

GameState::GameState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel) :
	m_stateMachine(stateMachine),
	m_window(window),
	m_inputManager(inputManager),
	m_currentLevel(currentLevel) {
}

GameState::~GameState() {
}

void GameState::updateInputManager() {
	m_inputManager.update();
}

void GameState::changeState(StateManager& machine, GameState* state) {
	machine.changeState(state);
}

}