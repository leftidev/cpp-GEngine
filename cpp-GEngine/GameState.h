#pragma once

class StateMachine;
class Window;

class GameState {
public:
	GameState(StateMachine& stateMachine, Window& window);
	virtual ~GameState();
	virtual void init() = 0;
	virtual void processEvents() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	void changeState(StateMachine& machine, GameState* state);

protected:
	StateMachine& _stateMachine;
	Window& _window;
};
