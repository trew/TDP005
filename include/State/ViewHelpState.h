#ifndef VIEWHELPSTATE_H
#define VIEWHELPSTATE_H

#include <State/State.h>

class ViewHelpState : public State
{
public:
	ViewHelpState(Game* const game) : State(game) {}

	bool init();
	void cleanup();

	bool handleEvent(const SDL_Event &ev);
	void render(SDL_Renderer* const renderer);

private:
	Sprite* introductionScreen;
	Sprite* escBack;
};

#endif