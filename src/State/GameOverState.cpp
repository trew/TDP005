#include <State/GameOverState.h>
#include <Utils/Utils.h>

bool GameOverState::init()
{
	gameOverScreen = new Sprite(game, "./gfx/menu/gameover.png", 0, 0, WWIDTH, WHEIGHT);
	gameOverScoreText = new Text(game->getRenderer(), "", 167, 203, 237, 0, 0, game->standard_font_42);

	return true;
}

void GameOverState::cleanup()
{
	delete gameOverScreen;
	delete gameOverScoreText;
}


bool GameOverState::handleEvent(const SDL_Event &ev)
{
	if (ev.key.type == SDL_KEYDOWN)
	{
		game->setState(MAINMENU);
		return true;
	}

	return false;
}

void GameOverState::render(SDL_Renderer* const renderer)
{
	gameOverScreen->draw(renderer);
	gameOverScoreText->draw(renderer, ((WWIDTH / 2) - (gameOverScoreText->get_width() / 2)), 260);
}

void GameOverState::setScore(const int score)
{
	std::string str = "Your score: ";
	str.append(Utils::itos(score));
	gameOverScoreText->update_text(str);
}