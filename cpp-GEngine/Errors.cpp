#include <SDL/SDL.h>

#include <cstdlib>
#include <iostream>

#include "Errors.h"


namespace GEngine {

// Prints out an error message and exits the game
void fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(69);
}

}