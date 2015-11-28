#include <State/IntroState.h>
#include <Core/Define.h>
#include <SDL2_gfxPrimitives.h>

bool IntroState::init()
{
	internalState = DEVSCREEN;

	devScreen = new Sprite(game, "./gfx/intro/devscreen.png", 0, 0, WWIDTH, WHEIGHT);
	introScreen = new Sprite(game, "./gfx/intro/introscreen.png", 0, 0, WWIDTH, WHEIGHT);

	startTime = SDL_GetTicks();
	return true;
}

void IntroState::cleanup()
{
	delete devScreen;
	delete introScreen;
}

bool IntroState::handleEvent(const SDL_Event &ev)
{
	if (ev.type == SDL_QUIT)
	{
		game->exit();
		game->setState(EXIT);
		return true;
	}
	else if (ev.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_SPACE)
		{
			if (internalState == INTROSCREEN)
			{
				game->setState(MAINMENU);
			}
			else
			{
				internalState = INTROSCREEN;
			}
			return true;
		}
	}

	return false;
}

void IntroState::update()
{
	
	if (SDL_GetTicks() > (startTime + 6000) && internalState == DEVSCREEN)
		internalState = INTROSCREEN;
}

void IntroState::render(SDL_Renderer* const renderer)
{
	if (internalState == DEVSCREEN)
	{
		unsigned int initial_delay = 1000;
		unsigned int fadeout_time = 3000;
		int alpha_level = 255 - ((SDL_GetTicks() - initial_delay) / 5);

		if (alpha_level < 0)
			alpha_level = 0;

		if (SDL_GetTicks() > initial_delay && SDL_GetTicks() <= fadeout_time)
		{
			devScreen->draw(renderer);
			boxRGBA(renderer, 0, 0, WWIDTH, WHEIGHT, 0, 0, 0, alpha_level);
		}

		else if (SDL_GetTicks() > initial_delay)
			devScreen->draw(renderer);
	}

	else if (internalState == INTROSCREEN)
	{
		introScreen->draw(renderer);
	}
}

