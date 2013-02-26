/*
 * Game_Init.cpp
 *
 *  Created on: 29 nov 2010
 *      Author: samuel
 */
#include "Game.h"

bool Game::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if (fullscreen)
	{
		if ((screen = SDL_SetVideoMode(WWIDTH, WHEIGHT, WBPP, SDL_HWSURFACE || SDL_FULLSCREEN)) == NULL)
			return false;
	}
	else
	{
		if ((screen = SDL_SetVideoMode(WWIDTH, WHEIGHT, WBPP, SDL_HWSURFACE || SDL_DOUBLEBUF)) == NULL)
			return false;
	}

	if (TTF_Init() < 0)
		return false;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		return false;

	SDL_WM_SetCaption("Tower Defence Pro 1.1 (Build 1.10.0000.000) © 2010 A15 Entertainment", NULL);
	SDL_EnableKeyRepeat(400, 70);
	SDL_EnableUNICODE(SDL_ENABLE);

	std::string standard_font = "./fonts/Graffiare.ttf";

	standard_font_48 = TTF_OpenFont(standard_font.c_str(), 48);
	standard_font_46 = TTF_OpenFont(standard_font.c_str(), 46);
	standard_font_42 = TTF_OpenFont(standard_font.c_str(), 42);
	standard_font_32 = TTF_OpenFont(standard_font.c_str(), 32);
	standard_font_20 = TTF_OpenFont(standard_font.c_str(), 20);
	standard_font_18 = TTF_OpenFont(standard_font.c_str(), 18);
	standard_font_16 = TTF_OpenFont(standard_font.c_str(), 16);
	standard_font_12 = TTF_OpenFont(standard_font.c_str(), 12);

	input_text = new Text("Enter your name", 167, 203, 237, 0, 0, standard_font_46);
	gameover_score_text = new Text("", 167, 203, 237, 0, 0, standard_font_42);
	error_loading_highscore = new Text("Error! Cannot load highscores!", 200, 200,standard_font_42);
	error_loading_highscore->hide();

	press_enter_to_start = new Text("Deploy towers, then press enter to start", 70, 550, standard_font_32);
	esc_back = new Text("Esc (back)", 520, 555, standard_font_32);

	grid = new Grid;
	grid->create_grid(15, 15);
	grid->set_portal_tile(grid->get_tile(GridPosition(7, 14)));
	grid->set_start_tile(grid->get_tile(GridPosition(7, 0)));

	map = new Sprite(this, 				"./gfx/map/map.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	map_grid = new Sprite(this, 			"./gfx/map/map-grid.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	map_wall = new Sprite(this, 			"./gfx/map/map-wall.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	map_entrance = new Sprite(this, 		"./gfx/map/map-entrance.png", 0, 280, TILESIZE, TILESIZE);
	map_exit = new Sprite(this, 			"./gfx/map/map-portal.png", 520, 239, TILESIZE, TILESIZE);

	dev_screen = new Sprite(this, 		"./gfx/intro/devscreen.png", 0, 0, WWIDTH, WHEIGHT);
	intro_screen = new Sprite(this, 		"./gfx/intro/introscreen.png", 0, 0, WWIDTH, WHEIGHT);
	introduction_screen = new Sprite(this, "./gfx/intro/introductscreen.png", 0, 0, WWIDTH, WHEIGHT);
	main_menu_screen = new Sprite(this, 	"./gfx/menu/mainmenu.png", 0, 0, WWIDTH, WHEIGHT);
	highscore_screen = new Sprite(this, 	"./gfx/menu/highscore.png", 0, 0, WWIDTH, WHEIGHT);
	ingame_menu_screen = new Sprite(this, "./gfx/menu/ingamemenu.png", 0, 0, WWIDTH, WHEIGHT);
	gameover_screen = new Sprite(this, 	"./gfx/menu/gameover.png", 0, 0, WWIDTH, WHEIGHT);

	menu_background = new Sprite(this, 	"./gfx/menu/menu-bg.png", 580, 0, WWIDTH - MENUWIDTH, WHEIGHT);
	menu_money_score = new Sprite(this, 	"./gfx/menu/menu-money-score-200x90.png", GRIDWIDTH, 50, 200, 90);
	menu_build = new Sprite(this, 		"./gfx/menu/menu-build-200x90.png", GRIDWIDTH, 140, 200, 90);
	menu_upgrade = new Sprite(this, 		"./gfx/menu/menu-upgrade-sell-200x90.png", GRIDWIDTH, 250, 200, 90);
	menu_info = new Sprite(this, 			"./gfx/menu/menu-information-200x240.png", GRIDWIDTH, 360, 200, 240);
	menu_lives = new Sprite(this, 		"./gfx/menu/heart_big-25x25.png", 624, 104, 18, 18);
	selection_sprite = new Sprite(this, 	"./gfx/misc/marker44x44.png", -5, -5, 44, 44);
	selection_sprite->hide();

	score_text = new Text(get_score_str(), 0, (int)menu_money_score->get_y() + 30, standard_font_16);
	update_score();
	split_money_score = new Text("-------------------------------------", (int)menu_money_score->get_x() + 30, (int)menu_money_score->get_y() + 45, standard_font_12);
	money_text = new Text(get_money_str(), 0, (int)menu_money_score->get_y() + 55, standard_font_16);
	update_money();
	lives_text = new Text(get_lives_str(), 0, (int)menu_money_score->get_y() + 55, standard_font_16);
	update_lives();
	level_text = new Text("Wave: 1", (int)menu_money_score->get_x() + 25, (int)menu_money_score->get_y() + 30, standard_font_16);
	timer_text = new Text("",255,255,255, 0, 5, standard_font_12);
	fps_text = new Text("", 255, 255, 255, 5, 5, standard_font_12); fps_text->hide();

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
	music =			 	new Sound("./snd/Ultrasyd_-_Lonesome_Robot.mp3", true, -1);

	SFX_cant_build = 	new Sound("./snd/SFX_1.wav", false, 0);
	SFX_build = 		new Sound("./snd/SFX_4.wav", false, 0);
	SFX_sell = 			new Sound("./snd/SFX_5.wav", false, 0);
	SFX_upgrade =		new Sound("./snd/SFX_2.wav", false, 0);
	SFX_life_lost = 	new Sound("./snd/SFX_3.wav", false, 0);
	SFX_new_highscore = new Sound("./snd/new_highscore.wav", false, 0);
	SFX_game_over = 	new Sound("./snd/game_over.wav", false, 0);

	//Available towers in menu
	build_list.push_back(new Tower(towers::SIMPLE, NULL));
	build_list.back()->set_x(630);
	build_list.back()->set_y(175);
	build_list.push_back(new Tower(towers::BOOST, NULL));
	build_list.back()->set_x(680);
	build_list.back()->set_y(175);
	build_list.push_back(new Tower(towers::WALL, NULL));
	build_list.back()->set_x(730);
	build_list.back()->set_y(175);

	//Ingame buttons
	ingame_buttons.push_back(new Button(BUTTON_MENU, 		600,   0, 112, 51, false, "./gfx/button/ingame-menuf10-112x51.png"));
	ingame_buttons.push_back(new Button(BUTTON_TOGGLESOUND, 712,   0, 44,  42,  true, "./gfx/button/ingame-soundon-44x51.png", "./gfx/button/ingame-soundoff-44x51.png"));
	ingame_buttons.push_back(new Button(BUTTON_TOGGLEGRID, 	756,   0, 44,  42,  true, "./gfx/button/ingame-gridon-44x51.png", "./gfx/button/ingame-gridoff-44x51.png"));
	ingame_buttons.push_back(new Button(BUTTON_UPGR, 		647, 280, 48,  48, false, "./gfx/button/menu-button-upgrade-48x48.png"));
	ingame_buttons.push_back(new Button(BUTTON_SELL, 		705, 279, 48,  48, false, "./gfx/button/menu-button-sell-48x48.png"));

	//Ingame menu buttons
	ingame_menu_buttons.push_back(new Button(BUTTON_RESUMEGAME, 340, 225, 121, 41, false, "./gfx/button/ingamemenu-continue-121x41.png", "./gfx/button/ingamemenu-continue-over-131x51.png"));
	ingame_menu_buttons.push_back(new Button(BUTTON_EXITTOMENU, 317, 275, 166, 40, false, "./gfx/button/ingamemenu-exittomenu-166x40.png", "./gfx/button/ingamemenu-exittomenu-over-176x50.png"));
	ingame_menu_buttons.push_back(new Button(BUTTON_EXITGAME,   329, 325, 142, 39, false, "./gfx/button/ingamemenu-exittoos-142x39.png", "./gfx/button/ingamemenu-exittoos-over-152x49.png"));

	//Main menu buttons
	mainmenu_buttons.push_back(new Button(BUTTON_STARTGAME, 272, 230, 257, 53, false, "./gfx/button/mainmenu-startgame-257x53.png", "./gfx/button/mainmenu-startgame-over-267x63.png"));
	mainmenu_buttons.push_back(new Button(BUTTON_HIGHSCORE, 287, 300, 226, 52, false, "./gfx/button/mainmenu-highscore-226x52.png", "./gfx/button/mainmenu-highscore-over-236x62.png"));
	mainmenu_buttons.push_back(new Button(BUTTON_VIEW_HELP, 343, 370, 115, 50, false, "./gfx/button/mainmenu-help-115x50.png", "./gfx/button/mainmenu-help-over-125x60.png"));
	mainmenu_buttons.push_back(new Button(BUTTON_EXITGAME,  295, 440, 210, 53, false, "./gfx/button/mainmenu-exitgame-210x53.png", "./gfx/button/mainmenu-exitgame-over-220x63.png"));

	//Green and red rects
	free_spot = new Sprite(this, "./gfx/misc/spot-free-4.png", 0, 0, 40, 40);
	not_free_spot = new Sprite(this, "./gfx/misc/spot-taken-4.png", 0, 0, 40, 40);

	if (music_enabled)
	{
		Mix_VolumeMusic(MUSIC_VOLUME);
		music->play();
	}

	return true;
}
