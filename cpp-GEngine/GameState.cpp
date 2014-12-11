#include "GameState.h"

#include "StateMachine.h"


	GameState::GameState(StateMachine& stateMachine) : _stateMachine(stateMachine) {
	}


	GameState::~GameState() {
	}

	void GameState::changeState(StateMachine& machine, GameState* state) {
		machine.changeState(state);
	}
