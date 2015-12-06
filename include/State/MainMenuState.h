#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <State/State.h>

class MainMenuState : public State
{
public:
	MainMenuState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	bool handleEvent(const SDL_Event &ev) override;
	void render(SDL_Renderer* const renderer) override;

private:
	ButtonList buttons;
	Sprite* background;
};

#endif