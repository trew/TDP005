#ifndef INGAMEMENUSTATE_H
#define INGAMEMENUSTATE_H

#include <State/State.h>

class InGameMenuState : public State
{
public:
	InGameMenuState(Game* const game) : State(game) {}

	bool init();
	void cleanup();

	bool handleEvent(const SDL_Event &ev);
	void render(SDL_Renderer* const renderer);
	
private:
	ButtonList inGameMenuButtons;
	Sprite* inGameMenuScreen;

};

#endif