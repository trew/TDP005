#include <State/MainMenuState.h>

bool MainMenuState::init()
{
	SDL_Renderer* const renderer = game->getRenderer();
	main_menu_screen = new Sprite(game, "./gfx/menu/mainmenu.png", 0, 0, WWIDTH, WHEIGHT);
	mainmenu_buttons.push_back(new Button(renderer, BUTTON_STARTGAME, 272, 230, 257, 53, false, "./gfx/button/mainmenu-startgame-257x53.png", "./gfx/button/mainmenu-startgame-over-267x63.png"));
	mainmenu_buttons.push_back(new Button(renderer, BUTTON_HIGHSCORE, 287, 300, 226, 52, false, "./gfx/button/mainmenu-highscore-226x52.png", "./gfx/button/mainmenu-highscore-over-236x62.png"));
	mainmenu_buttons.push_back(new Button(renderer, BUTTON_VIEW_HELP, 343, 370, 115, 50, false, "./gfx/button/mainmenu-help-115x50.png", "./gfx/button/mainmenu-help-over-125x60.png"));
	mainmenu_buttons.push_back(new Button(renderer, BUTTON_EXITGAME, 295, 440, 210, 53, false, "./gfx/button/mainmenu-exitgame-210x53.png", "./gfx/button/mainmenu-exitgame-over-220x63.png"));
	return true;
}

void MainMenuState::cleanup()
{
	delete main_menu_screen;

	for (ButtonList::iterator it = mainmenu_buttons.begin(); it != mainmenu_buttons.end(); it++)
	{
		delete (*it);
	}
}

bool MainMenuState::handleEvent(SDL_Event &ev)
{
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	if (ev.key.type == SDL_MOUSEBUTTONDOWN)
	{
		if (ev.button.button == SDL_BUTTON_LEFT)
		{
			for (ButtonList::iterator it = mainmenu_buttons.begin(); it != mainmenu_buttons.end(); it++)
			{
				if ((m_x > (*it)->get_x()) && (m_x < (*it)->get_x() + (*it)->get_width()) &&
					  (m_y > (*it)->get_y()) && (m_y < (*it)->get_y() + (*it)->get_height()))
				{
					switch ((*it)->get_type())
					{
					case BUTTON_STARTGAME:
						game->reset_game();
						game->setState(GAME_PAUSED);
						return true;
					case BUTTON_HIGHSCORE:
						game->update_highscore_sprites();
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
	}

	return false;
}

void MainMenuState::update()
{
}

void MainMenuState::render(SDL_Renderer* const renderer)
{
	main_menu_screen->draw(renderer);
	for (ButtonList::iterator it = mainmenu_buttons.begin(); it != mainmenu_buttons.end(); it++)
	{
		(*it)->draw(renderer);
	}
}
