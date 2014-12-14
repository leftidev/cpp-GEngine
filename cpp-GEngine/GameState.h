#pragma once

#include "Camera2D.h"

namespace GEngine {

class StateManager;
class Window;
class InputManager;

}

namespace GEngine {

class GameState {
public:
	GameState(StateManager& stateMachine, Window& window, InputManager& inputManager, int currentLevel);
	virtual ~GameState();
	virtual void init() = 0;
	virtual void processEvents() = 0;
	virtual void update(float deltaTime) = 0;
	void updateInputManager();
	virtual void updateCamera() = 0;
	virtual void draw() = 0;
	void changeState(StateManager& machine, GameState* state);

protected:
	StateManager& m_stateMachine;
	Window& m_window;
	InputManager& m_inputManager;
	GEngine::Camera2D m_camera; // main camera
	int m_currentLevel;
};

}