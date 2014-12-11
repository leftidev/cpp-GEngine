#include "StateMachine.h"
#include "GameState.h"


GameState::GameState(StateMachine& stateMachine, Window& window) : 
	_stateMachine(stateMachine),
	_window(window) {
}


GameState::~GameState() {
}

void GameState::changeState(StateMachine& machine, GameState* state) {
	machine.changeState(state);
}
