#include <Core/Game.h>
#include <State/MainMenuState.h>
#include <State/HighscoreState.h>
#include <State/GamePlayState.h>
#include <Core/GameEngine.h>

bool MainMenuState::init()
{
	SDL_Renderer* const renderer = getRenderer();
	background = new Sprite(game, "./gfx/menu/mainmenu.png", 0, 0, WWIDTH, WHEIGHT);
	buttons.push_back(new Button(renderer, 272, 230, 257, 53, "./gfx/button/mainmenu-startgame-257x53.png", "./gfx/button/mainmenu-startgame-over-267x63.png", [this](Button*) -> bool
	{
		game->reset_game();
		game->gamePlayState->setSubState(GAME_PAUSED);
		game->getEngine()->setState((State*)game->gamePlayState);
		return true;
	}));
	buttons.push_back(new Button(renderer, 287, 300, 226, 52, "./gfx/button/mainmenu-highscore-226x52.png", "./gfx/button/mainmenu-highscore-over-236x62.png", [this](Button*) -> bool
	{
		game->highscoreState->updateHighscoreSprites();
		game->getEngine()->setState(game->highscoreState);
		return true;
	}));
	buttons.push_back(new Button(renderer, 343, 370, 115, 50, "./gfx/button/mainmenu-help-115x50.png", "./gfx/button/mainmenu-help-over-125x60.png", [this](Button*) -> bool
	{
		game->getEngine()->setState((State*)game->viewHelpState);
		return true;
	}));
	buttons.push_back(new Button(renderer, 295, 440, 210, 53, "./gfx/button/mainmenu-exitgame-210x53.png", "./gfx/button/mainmenu-exitgame-over-220x63.png", [this](Button*) -> bool
	{
		game->getEngine()->exit();
		return true;
	}));

	return true;
}

void MainMenuState::cleanup()
{
	delete background;

	for (ButtonList::iterator it = buttons.begin(); it != buttons.end(); it++)
	{
		delete (*it);
	}
}

bool MainMenuState::handleEvent(const SDL_Event &ev)
{
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	if (ev.key.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT)
	{
		for (ButtonList::iterator it = buttons.begin(); it != buttons.end(); it++)
		{
			if ((*it)->overlaps(m_x, m_y))
			{
				return (*it)->performAction();
			}
		}
	}

	return false;
}

void MainMenuState::render(SDL_Renderer* const renderer)
{
	background->draw(renderer);
	for (ButtonList::iterator it = buttons.begin(); it != buttons.end(); it++)
	{
		(*it)->draw(renderer);
	}
}
