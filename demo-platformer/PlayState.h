#pragma once

#include <cpp-GEngine/GameState.h>
#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/SpriteBatch.h>
#include <cpp-GEngine/GLSLProgram.h>

#include "Player.h"
#include "Tile.h"
#include "Level.h"


class StateManager;

class PlayState : public GEngine::GameState {
public:
	PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager);
	~PlayState();
	void init() override;
	void initShaders();
	void initLevel();
	void processEvents() override;
	void update(float deltaTime) override;
	void updateCamera() override;
	void draw() override;

private:
	GEngine::SpriteBatch _spriteBatch; // Draws all sprites
	GEngine::GLSLProgram _textureProgram; // The shader program
	std::vector<Level*> _levels; // vector of all levels

	Player* _player;
	GLenum result;
	int _currentLevel;
};

