#ifndef INGAMEMENUSTATE_H
#define INGAMEMENUSTATE_H

#include <State/State.h>

class InGameMenuState : public State
{
public:
	InGameMenuState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	bool handleEvent(const SDL_Event &ev) override;
	void render(SDL_Renderer* const renderer) override;
	
private:
	ButtonList inGameMenuButtons;
	Sprite* inGameMenuScreen;

};

#endif