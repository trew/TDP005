#include <State/InGameMenuState.h>
#include <Core/GameEngine.h>

bool InGameMenuState::init()
{
	inGameMenuScreen = new Sprite(game, "./gfx/menu/ingamemenu.png", 0, 0, WWIDTH, WHEIGHT);

	inGameMenuButtons.push_back(new Button(getRenderer(), 340, 225, 121, 41, "./gfx/button/ingamemenu-continue-121x41.png", "./gfx/button/ingamemenu-continue-over-131x51.png", [this](Button*) -> bool
	{
		game->getEngine()->popState();
		return true;
	}));
	inGameMenuButtons.push_back(new Button(getRenderer(), 317, 275, 166, 40, "./gfx/button/ingamemenu-exittomenu-166x40.png", "./gfx/button/ingamemenu-exittomenu-over-176x50.png", [this](Button*) -> bool
	{
		game->getEngine()->setState((State*)game->mainMenuState);
		return true;
	}));
	inGameMenuButtons.push_back(new Button(getRenderer(), 329, 325, 142, 39, "./gfx/button/ingamemenu-exittoos-142x39.png", "./gfx/button/ingamemenu-exittoos-over-152x49.png", [this](Button*) -> bool
	{
		game->getEngine()->exit();
		return true;
	}));

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
			game->getEngine()->popState();
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
					return (*it)->performAction();
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