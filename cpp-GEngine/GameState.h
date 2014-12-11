#pragma once

class StateMachine;
class Window;
class InputManager;

class GameState {
public:
	GameState(StateMachine& stateMachine, Window& window, InputManager& inputManager);
	virtual ~GameState();
	virtual void init() = 0;
	virtual void processEvents() = 0;
	virtual void update() = 0;
	void inputManagerUpdate();
	virtual void draw() = 0;
	void changeState(StateMachine& machine, GameState* state);

protected:
	StateMachine& _stateMachine;
	Window& _window;
	InputManager& _inputManager;
};
