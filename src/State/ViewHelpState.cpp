#include <State/ViewHelpState.h>

bool ViewHelpState::init()
{
	escBack = new Text(game->getRenderer(), "Esc (back)", 520, 555, game->standard_font_32);
	introductionScreen = new Sprite(game, "./gfx/intro/introductscreen.png", 0, 0, WWIDTH, WHEIGHT);

	return true;
}

void ViewHelpState::cleanup()
{
	delete escBack;
	delete introductionScreen;
}

bool ViewHelpState::handleEvent(const SDL_Event &ev)
{
	if (ev.key.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
	{
		game->setState(MAINMENU);
		return true;
	}

	return false;
}

void ViewHelpState::render(SDL_Renderer* const renderer)
{
	introductionScreen->draw(renderer);
	escBack->draw(renderer);
}