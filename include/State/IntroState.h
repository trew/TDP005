#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <State/State.h>
#include <Entity/Sprite.h>

class IntroState : public State
{
public:
	IntroState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	bool handleEvent(const SDL_Event &ev) override;
	void update(const float &timeStep) override;
	void render(SDL_Renderer* const renderer) override;

private:
	int internalState;
	Sprite* devScreen;
	Sprite* introScreen;
	unsigned int startTime;
};

#endif