#pragma once

class StateMachine;

	class GameState {
	public:
		GameState(StateMachine& stateMachine);
		virtual ~GameState();
		virtual void init() = 0;
		virtual void processEvents() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		void changeState(StateMachine& machine, GameState* state);

		StateMachine& _stateMachine;
	};
