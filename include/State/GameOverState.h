#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <State/State.h>

class GameOverState : public State
{
public:
	GameOverState(Game* const game) : State(game) {}

	bool init() override;
	void cleanup() override;

	bool handleEvent(const SDL_Event &ev) override;
	void render(SDL_Renderer* const renderer) override;

	void setScore(const int score);
private:
	Sprite* gameOverScreen;
	Sprite* gameOverScoreText;
};

#endif