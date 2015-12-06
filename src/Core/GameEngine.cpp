#include <Core/GameEngine.h>
#include <State/State.h>
#include <State/IntroState.h>
#include <State/GameOverState.h>
#include <State/GamePlayState.h>
#include <State/HighscoreState.h>
#include <State/InGameMenuState.h>
#include <State/MainMenuState.h>
#include <State/ViewHelpState.h>
#include <Utils/Log.h>
#include <SDL2_framerate.h>
#include <list>

GameEngine::~GameEngine()
{
	LOG_INFO << "Destroying GameEngine";
	game->cleanup();

	SDL_DestroyRenderer(renderer);
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();

	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool GameEngine::init()
{
	LOG_INFO << "Initializing GameEngine";

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG_ERROR << "Error initializing SDL: " << SDL_GetError();
		return false;
	}

	window = SDL_CreateWindow(settings.WindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		LOG_ERROR << "Error creating window: " << SDL_GetError();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		LOG_ERROR << "Error creating renderer: " << SDL_GetError();
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	if (TTF_Init() < 0)
	{
		LOG_ERROR << "Error initializing SDL_TTF: " << TTF_GetError();
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		LOG_ERROR << "Error initializing SDL_Image: " << IMG_GetError();
		return false;
	}

	SDL_Surface* const icon = IMG_Load("./gfx/icon/icon64.png");
	if (icon != nullptr)
	{
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}
	else
	{
		LOG_WARN << "Couldn't load icon for window: " << "./gfx/icon/icon64.png";
	}

	if (Mix_Init(MIX_INIT_OGG) == 0)
	{
		LOG_ERROR << "Error initializing SDL_Mixer: " << Mix_GetError();
		return false;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		LOG_ERROR << "Error opening audio channels: " << Mix_GetError();
		return false;
	}

	LOG_INFO << "Successfully initialized GameEngine";
	return true;
}

State* const GameEngine::popState()
{
	if (states.empty())
	{
		return nullptr;
	}

	State* const poppedState = states.back();
	states.pop_back();

	pendingOnExitStates.push_back(poppedState);
	if (!states.empty())
	{
		pendingOnEnterStates.push_back(states.back());
	}

	return poppedState;
}

void GameEngine::pushState(State* const state)
{
	if (state == states.back())
	{
		return;
	}

	pendingOnEnterStates.push_back(state);

	states.push_back(state);
}

void GameEngine::setState(State* const state)
{
	pendingOnExitStates.insert(pendingOnExitStates.end(), states.begin(), states.end());
	pendingOnEnterStates.push_back(state);

	states.clear();
	states.push_back(state);
}

bool GameEngine::handleEvent(const SDL_Event &ev)
{
	if (ev.type == SDL_QUIT)
	{
		exit();
		return true;
	}

	return false;
}

int GameEngine::run(Game* const game)
{
	if (isRunning)
	{
		return -1;
	}

	FPSmanager manager;
	if (settings.MaxFPS > 0)
	{
		SDL_initFramerate(&manager);
		SDL_setFramerate(&manager, settings.MaxFPS);
	}

	isRunning = true;
	this->game = game;
	
	if (!init())
	{
		return -1;
	}

	game->engine = this;
	if (!game->init())
	{
		return -1;
	}

	if (states.empty())
	{
		// TODO log error that init() of game didn't set an initial state
		return -1;
	}

	for (State*& state : states)
	{
		state->onEnter();
	}

	float accumulator = 0;
	SDL_Event ev;
	long lastTime = SDL_GetTicks();

	Uint32 TICKS_PER_FRAME = (Uint32)(1000.f / settings.MaxFPS);
	
	while (isRunning)
	{
		{
			float deltaTime = (SDL_GetTicks() - lastTime) / 1000.f;

			lastTime = SDL_GetTicks();
			accumulator += deltaTime * getGameSpeed();
		}

		if (states.empty())
		{
			exit();
			break;
		}

		for(State*& state : pendingOnExitStates)
		{
			state->onEnter();
		}
		pendingOnExitStates.clear();

		for(State*& state : pendingOnEnterStates)
		{
			state->onEnter();
		}
		pendingOnEnterStates.clear();

		State* const currentState = states.back();
		while (SDL_PollEvent(&ev))
		{
			if (handleEvent(ev))
			{
				continue;
			}

			if (currentState->handleEvent(ev))
			{
				continue;
			}
		}

		// If any events caused the game to exit
		if (!isRunning)
		{
			break;
		}

		int loops = 0;
		while (accumulator >= settings.TimeStep && loops < settings.MaxFrameSkip)
		{
			currentState->update(settings.TimeStep);
			accumulator -= settings.TimeStep;
			loops++;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		for (State*& state : states)
		{
			state->render(renderer);
		}

		SDL_RenderPresent(renderer);

		fps.update();

		if (settings.MaxFPS > 0)
		{
			SDL_framerateDelay(&manager);
		}
	}

	return 0;
}

void GameEngine::exit()
{
	isRunning = false;
}