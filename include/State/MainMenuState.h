#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <State/State.h>

class MainMenuState : public State
{
public:
	MainMenuState(Game* const game) : State(game) {}

	bool init();
	void cleanup();

	bool handleEvent(SDL_Event &ev);
	void update();
	void render(SDL_Renderer* const renderer);

private:
	ButtonList mainmenu_buttons;
	Sprite* main_menu_screen;
};

#endif