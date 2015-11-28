#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <State/State.h>
#include <Entity/Sprite.h>

class IntroState : public State
{
public:
	IntroState(Game* const game) : State(game) {}

	bool init();
	void cleanup();

	bool handleEvent(const SDL_Event &ev);
	void update();
	void render(SDL_Renderer* const renderer);

private:
	int internalState;
	Sprite* devScreen;
	Sprite* introScreen;
	unsigned int startTime;
};

#endif