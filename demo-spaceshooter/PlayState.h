#pragma once

#include <cpp-GEngine/GameState.h>
#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/SpriteBatch.h>
#include <cpp-GEngine/GLSLProgram.h>

#include "Enemy.h"
#include "Player.h"


class StateManager;

class PlayState : public GEngine::GameState {
public:
	PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel);
	~PlayState();
	void init() override;
	void initShaders();
	void initLevel();
	void processEvents() override;
	void update(float deltaTime) override;
	void updateCamera() override;
	void draw() override;

private:
	GEngine::SpriteBatch m_spriteBatch; // Draws all sprites
	GEngine::GLSLProgram m_textureProgram; // The shader program

	std::vector<Enemy*> m_enemies; // All enemies

	Player* m_player = nullptr;
	int m_currentLevel = 0;
};

