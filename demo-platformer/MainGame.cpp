/*
    This is a basic platformer framework.

    - Player can run and jump
    - Level data(tile map) reading from file
    - Collisions with tiles
    - Camera follows the player
    - Gravity pulls down
*/

#include <algorithm>
#include <iostream>

#include <SDL/SDL.h>

#include <cpp-GEngine/GEngine.h>
#include <cpp-GEngine/Timing.h>
#include <cpp-GEngine/Errors.h>

#include "MainGame.h"


MainGame::MainGame() :
    _screenWidth(1024),
    _screenHeight(768),
    _gameState(GameState::PLAY),
    _fps(0),
    _player(nullptr) {
    // Empty
}

MainGame::~MainGame() {
    // Don't forget to delete the levels!
    for (int i = 0; i < _levels.size(); i++) {
        delete _levels[i];
    }
}

void MainGame::run() {

    initSystems();

    initLevel();

    gameLoop();
}

void MainGame::initSystems() {
    // Initialize the game engine
    GEngine::init();

    // Create our window
    _window.create("ARPG Experiment?", _screenWidth, _screenHeight, 0);

    // Black background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set up the shaders
    initShaders();

    // Initialize our sprite batch
    _firstSpriteBatch.init();

    // Set up the camera
    _camera.init(_screenWidth, _screenHeight);

}

void MainGame::initLevel() {
    // Initialize level 1.
    _levels.push_back(new Level("Levels/level1.txt"));
    _currentLevel = 0;

    // Initialize the player.
    _player = new Player();
    _player->init(_levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera);
}

void MainGame::initShaders() {
    // Compile the color shader.
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
    // Some helpful constants.
    const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
    const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
    const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
    const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
    const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

    // Used to cap the FPS
    GEngine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(3000000.0f);

    // Zoom out the camera by 2x
    const float CAMERA_SCALE = 1.0f / 2.0f;
    _camera.setScale(CAMERA_SCALE);

    // Start our previousTicks variable
    float previousTicks = SDL_GetTicks();

    // Main loop
    while (_gameState == GameState::PLAY) {
        fpsLimiter.beginFrame();

        // Calculate the frameTime in milliseconds
        float newTicks = SDL_GetTicks();
        float frameTime = newTicks - previousTicks;
        previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
        // Get the total delta time
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        _inputManager.update();

        processInput();

        int i = 0; // This counter makes sure we don't spiral to death!
        // Loop while we still have steps to process.
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
            // The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
            // Update all physics here and pass in deltaTime
            _player->update(_levels[_currentLevel]->_tiles, deltaTime);
            // Since we just took a step that is length deltaTime, subtract from totalDeltaTime
            totalDeltaTime -= deltaTime;
            // Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
            i++;
        }

        // Make sure the camera is bound to the player position
        _camera.setPosition(_player->getPosition());
        _camera.update();

        drawGame();

        // End the frame, limit the FPS, and get the current FPS.
        _fps = fpsLimiter.endFrame();
        std::cout << _fps << std::endl;
    }
}

void MainGame::processInput() {
    SDL_Event evnt;
    // Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
        case SDL_QUIT:
            _gameState = GameState::EXIT;
            GEngine::fatalError("oops");
        case SDL_MOUSEMOTION:
            _inputManager.setMouseCoords(float(evnt.motion.x), float(evnt.motion.y));
            break;
        case SDL_KEYDOWN:
            _inputManager.pressKey(evnt.key.keysym.sym);
            break;
        case SDL_KEYUP:
            _inputManager.releaseKey(evnt.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            _inputManager.pressKey(evnt.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _inputManager.releaseKey(evnt.button.button);
            break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureProgram.use();

    // Draw code goes here
    glActiveTexture(GL_TEXTURE0);

    // Make sure the shader uses texture 0
    GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    glm::mat4 projectionMatrix = _camera.getCameraMatrix();
    GLint pUniform = _textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    const glm::vec2 tileDimensions(_levels[_currentLevel]->_tiles[0]->width);

    // Begin drawing.
    _firstSpriteBatch.begin();

    // Draw tiles with camera culling.
    for (int i = 0; i < _levels[_currentLevel]->_tiles.size(); i++) {
        if (_camera.isBoxInView(_levels[_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
            _levels[_currentLevel]->_tiles[i]->draw(_firstSpriteBatch);
        }
    }

    // Draw player.
    _player->draw(_firstSpriteBatch);

    // End sprite batch creation
    _firstSpriteBatch.end();

    // Render to the screen
    _firstSpriteBatch.renderBatch();

    // Unbind the program
    _textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}