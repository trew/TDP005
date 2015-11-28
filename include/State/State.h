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

	virtual void onEnter() {};
	virtual void onExit() {};

	virtual bool handleEvent(const SDL_Event &ev) { return false; }
	virtual void update(const float &timeStep) {}

	virtual void render(SDL_Renderer* const renderer) {}

	Game* const getGame() { return game; }
	SDL_Renderer* const getRenderer() { return renderer; }

protected:
	State(Game* const game);

	Game* const game;
	SDL_Renderer* const renderer;

	State* parent;
	int parentStateValue;
};

#endif