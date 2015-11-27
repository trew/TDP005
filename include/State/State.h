#ifndef STATE_H
#define STATE_H

#include <SDL.h>
#include <Core/Game.h>

class State
{
public:
	virtual ~State() {}

	virtual bool init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() {}
	virtual void resume() {}

	virtual bool handleEvent(SDL_Event &ev) = 0;
	virtual void update() = 0;

	virtual void render(SDL_Renderer* const renderer) = 0;

	Game* const getGame() { return game; }

protected:
	State(Game* const game) : game(game) {}

	Game* const game;
};

#endif