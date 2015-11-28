#ifndef VIEWHELPSTATE_H
#define VIEWHELPSTATE_H

#include <State/State.h>

class ViewHelpState : public State
{
public:
	ViewHelpState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	bool handleEvent(const SDL_Event &ev) override;
	void render(SDL_Renderer* const renderer) override;

private:
	Sprite* introductionScreen;
	Sprite* escBack;
};

#endif