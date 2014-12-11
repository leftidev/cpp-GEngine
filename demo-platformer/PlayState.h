#pragma once

#include <cpp-GEngine/GameState.h>
#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/SpriteBatch.h>
#include <cpp-GEngine/GLSLProgram.h>


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

private:
	GEngine::SpriteBatch _spriteBatch; // Draws all spritebatches
	GEngine::GLSLProgram _textureProgram; // The shader program
};

