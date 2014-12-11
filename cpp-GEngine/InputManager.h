#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

class InputManager {
public:
	InputManager();
	~InputManager();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	void setMouseCoords(float x, float y);

	// Returns true if the key is held down.
	bool isKeyDown(unsigned int keyID);

	// Returns true if the key was held down.
	bool wasKeyDown(unsigned int keyID);

	// Returns true if the key was just pressed.
	bool isKeyPressed(unsigned int keyID);

	// Getters.
	glm::vec2 getMouseCoords() const { return _mouseCoords; }

private:
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords;
};