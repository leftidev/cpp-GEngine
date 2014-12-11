#pragma once

#include <cpp-GEngine/GameState.h>

class StateMachine;

class PlayState : public GameState {
public:
	PlayState(StateMachine& stateMachine);
	~PlayState();
	void init() override;
	void initShaders();
	void initLevel();
	void processEvents() override;
	void update() override;
	void draw() override;

};

