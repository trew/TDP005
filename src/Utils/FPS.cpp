#include <Utils/FPS.h>
#include <SDL.h>

void FPS::update()
{
	if (oldTime + 1000 < SDL_GetTicks())
	{
		oldTime = SDL_GetTicks();
		frames = 0;
	}

	frames++;
}

const int FPS::getFPS() const
{
	const Uint32 ticks = SDL_GetTicks();
	if (ticks == oldTime) // prevent zero division
	{
		return frames;
	}

	return (int)(frames / ((float)(ticks - oldTime) / 1000.f));
}