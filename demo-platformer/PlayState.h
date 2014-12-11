#pragma once

#include <cpp-GEngine/GameState.h>
#include <cpp-GEngine/InputManager.h>

class StateMachine;

class PlayState : public GameState {
public:
	PlayState(StateMachine& stateMachine, Window& window, InputManager& inputManager);
	~PlayState();
	void init() override;
	void initShaders();
	void initLevel();
	void processEvents() override;
	void update() override;
	void draw() override;
};

