#ifndef FPSREGULATOR_H
#define FPSREGULATOR_H

#include <cstdint>

class FPSRegulator
{
public:

	static void begin();

	static void regulate(uint32_t maxFPS);

	static float getFPS() { return m_fps; }
	static float getDelta() { return m_delta; }

private:
	static uint32_t m_beginningTicks;
	static uint32_t m_endTicks;

	static float m_delta;
	static float m_fps;
};

#endif