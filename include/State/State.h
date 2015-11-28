#ifndef STATE_H
#define STATE_H

#include <SDL.h>
#include <Core/Game.h>

class Game;

class State
{
public:
	virtual ~State() {}

	virtual bool init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() {}
	virtual void resume() {}

	virtual bool handleEvent(const SDL_Event &ev) { return false; }
	virtual void update() {}

	virtual void render(SDL_Renderer* const renderer) {}

	Game* const getGame() { return game; }

protected:
	State(Game* const game) : game(game) {}

	Game* const game;
};

#endif