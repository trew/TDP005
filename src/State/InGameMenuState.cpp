#include <State/InGameMenuState.h>

bool InGameMenuState::init()
{
	inGameMenuScreen = new Sprite(game, "./gfx/menu/ingamemenu.png", 0, 0, WWIDTH, WHEIGHT);

	inGameMenuButtons.push_back(new Button(game->getRenderer(), BUTTON_RESUMEGAME, 340, 225, 121, 41, false, "./gfx/button/ingamemenu-continue-121x41.png", "./gfx/button/ingamemenu-continue-over-131x51.png"));
	inGameMenuButtons.push_back(new Button(game->getRenderer(), BUTTON_EXITTOMENU, 317, 275, 166, 40, false, "./gfx/button/ingamemenu-exittomenu-166x40.png", "./gfx/button/ingamemenu-exittomenu-over-176x50.png"));
	inGameMenuButtons.push_back(new Button(game->getRenderer(), BUTTON_EXITGAME, 329, 325, 142, 39, false, "./gfx/button/ingamemenu-exittoos-142x39.png", "./gfx/button/ingamemenu-exittoos-over-152x49.png"));

	return true;
}

void InGameMenuState::cleanup()
{
	delete inGameMenuScreen;

	for (ButtonList::iterator it = inGameMenuButtons.begin(); it != inGameMenuButtons.end(); it++)
	{
		delete (*it);
	}
}

bool InGameMenuState::handleEvent(const SDL_Event &ev)
{
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	if (ev.key.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE)
		{
			game->setState(getParentStateValue());
			return true;
		}
	}

	else if (ev.key.type == SDL_MOUSEBUTTONDOWN)
	{
		if (ev.button.button == SDL_BUTTON_LEFT)
		{
			for (ButtonList::iterator it = inGameMenuButtons.begin(); it != inGameMenuButtons.end(); it++)
			{
				if ((*it)->overlaps(m_x, m_y))
				{
					switch ((*it)->get_type())
					{
					case BUTTON_RESUMEGAME:
						game->setState(getParentStateValue());
						return true;
					case BUTTON_EXITTOMENU:
						game->setState(MAINMENU);
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

void InGameMenuState::render(SDL_Renderer* const renderer)
{
	inGameMenuScreen->draw(renderer);

	for (ButtonList::iterator it = inGameMenuButtons.begin(); it != inGameMenuButtons.end(); it++)
	{
		(*it)->draw(renderer);
	}
}