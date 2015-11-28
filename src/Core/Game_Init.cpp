/*
 * Game_Init.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */
#include <Core/Game.h>
#include <State/MainMenuState.h>
#include <State/IntroState.h>
#include <State/ViewHelpState.h>
#include <State/HighscoreState.h>
#include <State/GameOverState.h>

void Game::parse_config() {
	config = new ConfigFile("settings.cfg");
	fullscreen = config->get_value<bool>("fullscreen", false);
	sound_volume = config->get_value<int>("sound_volume", 35);
	grid_visible = config->get_value<bool>("grid", true);
}

bool Game::init()
{
	parse_config();

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}

	const std::string windowTitle = "Tower Defence Pro " + std::string(VERSION) + " © 2010 - 2016 A15 Entertainment";

	if (fullscreen)
	{
		if (SDL_CreateWindowAndRenderer(WWIDTH, WHEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer) == -1)
		{
			std::cerr << SDL_GetError() << std::endl;
			return false;
		}
	}
	else
	{
		window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (window == NULL || renderer == NULL)
		{
			std::cerr << SDL_GetError() << std::endl;
			return false;
		}
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	SDL_Surface* const icon = IMG_Load("./gfx/icon/icon64.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	if (TTF_Init() < 0) {
		std::cerr << TTF_GetError() << std::endl;
		return false;
	}

	if (Mix_Init(MIX_INIT_OGG) == 0) {
		std::cerr << Mix_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		return false;

	SDL_SetWindowTitle(window, windowTitle.c_str());

	std::string standard_font = "./fonts/Graffiare.ttf";

	standard_font_48 = TTF_OpenFont(standard_font.c_str(), 48);
	standard_font_46 = TTF_OpenFont(standard_font.c_str(), 46);
	standard_font_42 = TTF_OpenFont(standard_font.c_str(), 42);
	standard_font_32 = TTF_OpenFont(standard_font.c_str(), 32);
	standard_font_20 = TTF_OpenFont(standard_font.c_str(), 20);
	standard_font_18 = TTF_OpenFont(standard_font.c_str(), 18);
	standard_font_16 = TTF_OpenFont(standard_font.c_str(), 16);
	standard_font_12 = TTF_OpenFont(standard_font.c_str(), 12);

	error_loading_highscore = new Text(renderer, "Error! Cannot load highscores!", 200, 200, standard_font_42);
	error_loading_highscore->hide();


	press_enter_to_start = new Text(renderer, "Deploy towers, then press enter to start", 70, 550, standard_font_32);

	grid = new Grid;
	grid->create_grid(15, 15);
	grid->set_portal_tile(grid->get_tile(GridPosition(7, 14)));
	grid->set_start_tile(grid->get_tile(GridPosition(7, 0)));

	map = new Sprite(this, 				"./gfx/map/map.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	map_grid = new Sprite(this, 			"./gfx/map/map-grid.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	map_wall = new Sprite(this, 			"./gfx/map/map-wall.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	map_entrance = new Sprite(this, 		"./gfx/map/map-entrance.png", 0, 280, TILESIZE, TILESIZE);
	map_exit = new Sprite(this, 			"./gfx/map/map-portal.png", 520, 239, TILESIZE, TILESIZE);

	ingame_menu_screen = new Sprite(this, "./gfx/menu/ingamemenu.png", 0, 0, WWIDTH, WHEIGHT);

	menu_background = new Sprite(this, 	"./gfx/menu/menu-bg.png", 580, 0, WWIDTH - MENUWIDTH, WHEIGHT);
	menu_money_score = new Sprite(this, 	"./gfx/menu/menu-money-score-200x90.png", GRIDWIDTH, 50, 200, 90);
	menu_build = new Sprite(this, 		"./gfx/menu/menu-build-200x90.png", GRIDWIDTH, 140, 200, 90);
	menu_upgrade = new Sprite(this, 		"./gfx/menu/menu-upgrade-sell-200x90.png", GRIDWIDTH, 250, 200, 90);
	menu_info = new Sprite(this, 			"./gfx/menu/menu-information-200x240.png", GRIDWIDTH, 360, 200, 240);
	menu_lives = new Sprite(this, 		"./gfx/menu/heart_big-25x25.png", 624, 104, 18, 18);
	selection_sprite = new Sprite(this, 	"./gfx/misc/marker44x44.png", -5, -5, 44, 44);
	selection_sprite->hide();

	score_text = new Text(renderer, get_score_str(), 0, (int)menu_money_score->get_y() + 30, standard_font_16);
	update_score();
	split_money_score = new Text(renderer, "-------------------------------------", (int)menu_money_score->get_x() + 30, (int)menu_money_score->get_y() + 45, standard_font_12);
	money_text = new Text(renderer, get_money_str(), 0, (int)menu_money_score->get_y() + 55, standard_font_16);
	update_money();
	lives_text = new Text(renderer, get_lives_str(), 0, (int)menu_money_score->get_y() + 55, standard_font_16);
	update_lives();
	level_text = new Text(renderer, "Wave: 1", (int)menu_money_score->get_x() + 25, (int)menu_money_score->get_y() + 30, standard_font_16);
	timer_text = new Text(renderer, "", 255, 255, 255, 0, 5, standard_font_12);
	fps_text = new Text(renderer, "", 255, 255, 255, 5, 5, standard_font_12); fps_text->hide();
	speed_text = new Text(renderer, "", 255, 255, 255, 570, 580, standard_font_12); speed_text->hide();

	//Optionbox buttons and background
	option_box_BGx1 = new Sprite(this, "./gfx/menu/popup-menu-1x-40x43.png", 0, 0, 40, 43);
	option_box_BGx2 = new Sprite(this, "./gfx/menu/popup-menu-2x-67x43.png", 0, 0, 67, 43);
	option_box_BGx3 = new Sprite(this, "./gfx/menu/popup-menu-3x-94x43.png", 0, 0, 94, 43);
	option_box_BGx4 = new Sprite(this, "./gfx/menu/popup-menu-4x-122x43.png", 0, 0, 122, 43);
	option_box_BGx5 = new Sprite(this, "./gfx/menu/popup-menu-5x-149x43.png", 0, 0, 149, 43);
	option_box_BGx6 = new Sprite(this, "./gfx/menu/popup-menu-6x-177x43.png", 0, 0, 177, 43);

	optionbox_buttonstorage[BUTTON_BASE] = (new Sprite(this, "./gfx/button/menu-button-base-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_BASE]->set_type(BUTTON_BASE);
	optionbox_buttonstorage[BUTTON_BASIC] = (new Sprite(this, "./gfx/button/menu-button-basic-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_BASIC]->set_type(BUTTON_BASIC);
	optionbox_buttonstorage[BUTTON_BOMB] = (new Sprite(this, "./gfx/button/menu-button-bomb-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_BOMB]->set_type(BUTTON_BOMB);
	optionbox_buttonstorage[BUTTON_BOOST] = (new Sprite(this, "./gfx/button/menu-button-boost-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_BOOST]->set_type(BUTTON_BOOST);
	optionbox_buttonstorage[BUTTON_RANGE] = (new Sprite(this, "./gfx/button/menu-button-range-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_RANGE]->set_type(BUTTON_RANGE);
	optionbox_buttonstorage[BUTTON_SELL] = (new Sprite(this, "./gfx/button/menu-button-sell-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_SELL]->set_type(BUTTON_SELL);
	optionbox_buttonstorage[BUTTON_SPEED] = (new Sprite(this, "./gfx/button/menu-button-speed-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_SPEED]->set_type(BUTTON_SPEED);
	optionbox_buttonstorage[BUTTON_UPGRADE] = (new Sprite(this, "./gfx/button/menu-button-upgrade-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_UPGRADE]->set_type(BUTTON_UPGRADE);
	optionbox_buttonstorage[BUTTON_NOUPGRADE] = (new Sprite(this, "./gfx/button/menu-button-noupgrade-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_NOUPGRADE]->set_type(BUTTON_NOUPGRADE);
	optionbox_buttonstorage[BUTTON_WALL] = (new Sprite(this, "./gfx/button/menu-button-wall-30x30.png", 0, 0, 30, 30));
	optionbox_buttonstorage[BUTTON_WALL]->set_type(BUTTON_WALL);

	//Load sounds
	music =			 	new Sound("./snd/Ultrasyd_Lonesome_Robot.ogg", true, -1);

	SFX_cant_build = 	new Sound("./snd/SFX_1.wav", false, 0);
	SFX_build = 		new Sound("./snd/SFX_4.wav", false, 0);
	SFX_sell = 			new Sound("./snd/SFX_5.wav", false, 0);
	SFX_upgrade =		new Sound("./snd/SFX_2.wav", false, 0);
	SFX_life_lost = 	new Sound("./snd/SFX_3.wav", false, 0);
	SFX_new_highscore = new Sound("./snd/new_highscore.wav", false, 0);
	SFX_game_over = 	new Sound("./snd/game_over.wav", false, 0);

	//Available towers in menu
	build_list.push_back(new BaseTower(this, towers::SIMPLE, NULL));
	build_list.back()->set_x(630);
	build_list.back()->set_y(175);
	build_list.push_back(new BaseTower(this, towers::BOOST, NULL));
	build_list.back()->set_x(680);
	build_list.back()->set_y(175);
	build_list.push_back(new BaseTower(this, towers::WALL, NULL));
	build_list.back()->set_x(730);
	build_list.back()->set_y(175);

	//Ingame buttons
	ingame_buttons.push_back(new Button(renderer, BUTTON_MENU, 		600,   0, 112, 51, false, "./gfx/button/ingame-menuf10-112x51.png"));
	sound_button = new Button(renderer, BUTTON_TOGGLESOUND, 712, 0, 44, 42, false, "./gfx/button/ingame-sound-44x51.png");
	ingame_buttons.push_back(sound_button);
	ingame_buttons.push_back(new Button(renderer, BUTTON_TOGGLEGRID, 756, 0, 44, 42, true, "./gfx/button/ingame-gridon-44x51.png", "./gfx/button/ingame-gridoff-44x51.png"));
	ingame_buttons.push_back(new Button(renderer, BUTTON_UPGR, 647, 280, 48, 48, false, "./gfx/button/menu-button-upgrade-48x48.png"));
	ingame_buttons.push_back(new Button(renderer, BUTTON_SELL, 705, 279, 48, 48, false, "./gfx/button/menu-button-sell-48x48.png"));

	//Ingame menu buttons
	ingame_menu_buttons.push_back(new Button(renderer, BUTTON_RESUMEGAME, 340, 225, 121, 41, false, "./gfx/button/ingamemenu-continue-121x41.png", "./gfx/button/ingamemenu-continue-over-131x51.png"));
	ingame_menu_buttons.push_back(new Button(renderer, BUTTON_EXITTOMENU, 317, 275, 166, 40, false, "./gfx/button/ingamemenu-exittomenu-166x40.png", "./gfx/button/ingamemenu-exittomenu-over-176x50.png"));
	ingame_menu_buttons.push_back(new Button(renderer, BUTTON_EXITGAME, 329, 325, 142, 39, false, "./gfx/button/ingamemenu-exittoos-142x39.png", "./gfx/button/ingamemenu-exittoos-over-152x49.png"));

	//Main menu buttons
	mainMenuState = new MainMenuState(this);
	mainMenuState->init();
	introState = new IntroState(this);
	introState->init();
	viewHelpState = new ViewHelpState(this);
	viewHelpState->init();
	highscoreState = new HighscoreState(this);
	highscoreState->init();
	gameOverState = new GameOverState(this);
	gameOverState->init();

	//Green and red rects
	free_spot = new Sprite(this, "./gfx/misc/spot-free-4.png", 0, 0, 40, 40);
	not_free_spot = new Sprite(this, "./gfx/misc/spot-taken-4.png", 0, 0, 40, 40);

	Sound::set_volume(sound_volume);
	if (music_enabled)
	{
		music->play();
	}

	delta_timer = new Timer();
	fps_timer = new Timer();
	return true;
}
