#include <iostream>
#include <SDL2/SDL.h>

#include "Base.h"

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	Base::run(glm::uvec2(1024, 768), "Sidescroller");

	return 0;
}
