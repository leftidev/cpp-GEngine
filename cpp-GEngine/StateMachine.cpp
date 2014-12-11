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

	void StateMachine::inputManagerUpdate() {
		//_inputManager.update();
	}

	void StateMachine::update() {
		_gameStates.back()->update();
	}

	void StateMachine::processEvents() {
		_gameStates.back()->processEvents();
	}

	void StateMachine::draw() {
		_gameStates.back()->draw();
	}
