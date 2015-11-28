#include <Core/Game.h>
#include <State/MainMenuState.h>
#include <State/HighscoreState.h>

bool MainMenuState::init()
{
	SDL_Renderer* const renderer = game->getRenderer();
	background = new Sprite(game, "./gfx/menu/mainmenu.png", 0, 0, WWIDTH, WHEIGHT);
	buttons.push_back(new Button(renderer, BUTTON_STARTGAME, 272, 230, 257, 53, false, "./gfx/button/mainmenu-startgame-257x53.png", "./gfx/button/mainmenu-startgame-over-267x63.png"));
	buttons.push_back(new Button(renderer, BUTTON_HIGHSCORE, 287, 300, 226, 52, false, "./gfx/button/mainmenu-highscore-226x52.png", "./gfx/button/mainmenu-highscore-over-236x62.png"));
	buttons.push_back(new Button(renderer, BUTTON_VIEW_HELP, 343, 370, 115, 50, false, "./gfx/button/mainmenu-help-115x50.png", "./gfx/button/mainmenu-help-over-125x60.png"));
	buttons.push_back(new Button(renderer, BUTTON_EXITGAME, 295, 440, 210, 53, false, "./gfx/button/mainmenu-exitgame-210x53.png", "./gfx/button/mainmenu-exitgame-over-220x63.png"));
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
				switch ((*it)->get_type())
				{
				case BUTTON_STARTGAME:
					game->reset_game();
					game->setState(GAME_PAUSED);
					return true;
				case BUTTON_HIGHSCORE:
					game->highscoreState->updateHighscoreSprites();
					game->setState(HIGHSCORE);
					return true;
				case BUTTON_VIEW_HELP:
					game->setState(VIEW_HELP);
					return true;
				case BUTTON_EXITGAME:
					game->exit();
					return true;
				}
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
