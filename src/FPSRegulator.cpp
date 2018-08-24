#include "FPSRegulator.h"

#include <SDL2/SDL.h>
#include <iostream>

uint32_t FPSRegulator::m_beginningTicks;
uint32_t FPSRegulator::m_endTicks;

float FPSRegulator::m_delta;
float FPSRegulator::m_fps;

void FPSRegulator::begin()
{
	m_beginningTicks = SDL_GetTicks();
}

void FPSRegulator::regulate(uint32_t maxFPS)
{
	uint32_t skip = 1000 / maxFPS;
	uint32_t difference = SDL_GetTicks() - m_beginningTicks;

	m_fps = 1000.f / difference;
	m_delta = (difference / 1000.f);

	int skipby = (SDL_GetTicks() - m_beginningTicks) + skip;
	
	//printf("FPS: %f, Skipby %u\n", m_fps, skipby);
	
	if (skipby >= 0) {
		SDL_Delay(skipby);
	}
	else {
		printf("Running behind %i ticks\n", -skipby);
	}
}
