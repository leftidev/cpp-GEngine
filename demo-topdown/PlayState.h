#pragma once

#include <cpp-GEngine/GameState.h>
#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/SpriteBatch.h>
#include <cpp-GEngine/Shader.h>
#include <cpp-GEngine/ShaderProgram.h>

#include "Player.h"
#include "Tile.h"
#include "Level.h"
#include "Enemy.h"


class StateManager;

class PlayState : public GEngine::GameState {
public:
	PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel);
	~PlayState();
	void init() override;
	void loadShaders();
	void initLevel();
	void processEvents() override;
	void update(float deltaTime) override;
	void updateCamera() override;
	void draw() override;

private:
	GEngine::SpriteBatch m_spriteBatch; // Draws all sprites
	GEngine::ShaderProgram m_shaderProgram; // The shader program
	std::vector<GEngine::Shader> m_shaders;
	std::vector<Level*> m_levels; // vector of all levels

	std::vector<Enemy*> m_enemies; // All enemies

	Player* m_player = nullptr;
	int m_currentLevel = 0;
};

