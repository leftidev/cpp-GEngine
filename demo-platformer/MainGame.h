#pragma once

#include <cpp-GEngine/Window.h>
#include <cpp-GEngine/GLSLProgram.h>
#include <cpp-GEngine/Camera2D.h>
#include <cpp-GEngine/InputManager.h>
#include <cpp-GEngine/SpriteBatch.h>

#include "Player.h"
#include "Tile.h"
#include "Level.h"

enum class GameState {
    PLAY,
    EXIT
};

class MainGame {
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the level and sets up everything
    void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Member Variables
    GEngine::Window _window; ///< The game window

    GEngine::GLSLProgram _textureProgram; ///< The shader program

    GEngine::InputManager _inputManager; ///< Handles input

    GEngine::Camera2D _camera; ///< Main Camera

    GEngine::SpriteBatch _firstSpriteBatch; ///< Draws all agents

    std::vector<Level*> _levels; ///< vector of all levels

    Player* _player;

    int _screenWidth, _screenHeight;

    float _fps;

    int _currentLevel;

    int _frameCounter = 0;

    GameState _gameState;
};

