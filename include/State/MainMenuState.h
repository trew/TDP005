#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <State/State.h>

class MainMenuState : public State
{
public:
	MainMenuState(Game* const game) : State(game) {}

	bool init();
	void cleanup();

	bool handleEvent(const SDL_Event &ev);
	void render(SDL_Renderer* const renderer);

private:
	ButtonList buttons;
	Sprite* background;
};

#endif