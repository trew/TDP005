/*
 * Game header
 *
 *
 */

#ifndef GAME_H_
#define GAME_H_

//Constants
#include "Define.h"

//Graphics and Sound
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

//Standard Library dependecies
#include <list>
#include <map>
#include <vector>
#include <sstream>

//Image related
#include "Sprite.h"
#include "Tower.h"
#include "Projectile.h"
#include "Text.h"
#include "Button.h"
#include "Enemy.h"

#include "Sound.h"

//Pathfinding and Difficulty Control
#include "Grid.h"
#include "Dijkstra.h"
#include "Level.h"

class Level;
typedef std::vector<std::pair<int, std::string>* > HighscoreList;
typedef std::pair<int, int> GridPosition;
typedef std::map<GridPosition, Sprite*> GridMap;

/** \brief The Game class.
 *
 * The game class controls the flow of the game. It connects different parts of the game.
 *
 * */
class Game {
public:
	Game(); ///Constructor
	virtual ~Game(); ///Destructor
	int on_execute();	///Defined in Game.cpp
	static TTF_Font* standard_font_48;	///Global font
	static TTF_Font* standard_font_46;	///Global font
	static TTF_Font* standard_font_42;	///Global font
	static TTF_Font* standard_font_32;	///Global font
	static TTF_Font* standard_font_20;	///Global font
	static TTF_Font* standard_font_18;	///Global font
	static TTF_Font* standard_font_16;	///Global font
	static TTF_Font* standard_font_12;	///Global font

	Grid* get_grid();
private:

/** *** Game Variables *** */
	/** Game Values */
	int game_state;
	int old_game_state;
	bool game_running;
	bool game_started;
	int FPS_MAX;
	int current_fps;
	Uint32 fps_timer;
	bool fullscreen;
	bool need_boost_update;	///< Indicates whether we need to update the boost connections

	bool snap_to_grid;
	bool option_box_visible;
	bool update_enemy_path;		///Sets true if player build or sell a tower

	bool grid_visible;
	bool sound_enabled;
	bool music_enabled;

	bool building_flag;			///True if player has selected a tower from the building menu.
	Tower* hovered_build_item;

	/** Player values */
	std::string playername;		///Entered when achieving place in highscore
	int lives;
	int score;
	int money;
	Grid* grid;

private:
	Level* level_control;		//Controls difficulty and enemy waves
	SDL_Surface* screen;		//Main screen, everything visible is blitted to this screen
	Tile* tile_selection;	//Pointer to a tower, don't delete on cleanup.
	Tile* last_selected_tile;
	Tower* buildmenu_selection; //The selected tower in the build menu

	/** Sprites */
	Sprite* dev_screen;
	Sprite* intro_screen;
	Sprite* introduction_screen;
	Sprite* main_menu_screen;
	Sprite* highscore_screen;
	Sprite* ingame_menu_screen;		//When pausing the game (pressing F10 in game)
	Sprite* gameover_screen;

	Sprite* map;
	Sprite* map_grid;
	Sprite* map_entrance;
	Sprite* map_exit;
	Sprite* map_wall;

	Sprite* menu_background;
	Sprite* menu_money_score;
	Sprite* menu_build;
	Sprite* menu_upgrade;
	Sprite* menu_info;
	Sprite* menu_lives;
	Sprite* selection_sprite;		//Sprite indicating which object is selected

	Sprite* press_enter_to_start;
	Sprite* esc_back;

	Sprite* input_text;				///Test displayed when typing in name at highscore
	Sprite* error_loading_highscore;
	Sprite* gameover_score_text;

	Sprite* lives_text;
	Sprite* score_text;
	Sprite* money_text;
	Sprite* level_text;
	int timer;						//Strongly connected to timer_text
	int old_timer;
	Sprite* timer_text;
	Sprite* fps_text;
	Sprite* split_money_score;		///A line separating money and score

	Sprite* free_spot;			///When trying to build a tower...
	Sprite* not_free_spot;


	/** Different sizes on box depending on amount of buttons in it */
	Sprite* option_box_BGx1;
	Sprite* option_box_BGx2;
	Sprite* option_box_BGx3;
	Sprite* option_box_BGx4;
	Sprite* option_box_BGx5;
	Sprite* option_box_BGx6;

	/** Sounds */
	Sound* music;
	Sound* SFX_cant_build;
	Sound* SFX_build;
	Sound* SFX_upgrade;
	Sound* SFX_sell;
	Sound* SFX_life_lost;
	Sound* SFX_new_highscore;
	Sound* SFX_game_over;

private:
/** *** Game Functions *** */
/** Definition in Game.cpp */
	GridPosition get_grid_pixel_position(GridPosition pos);		///Convert X and Y coordinates to appropriate place in Gridcontrol
	void set_boost_update(bool);
	void snap_XY_to_grid(int &X, int &Y);		///Snap X and Y to top left corner of a grid spot.

	bool read_highscores_from_file();
	int  get_highscore_pos();
	void insert_new_highscore(int new_score, int position, std::string name);
	void write_highscore_to_file();
	void update_highscore_sprites();

	void show_intro(SDL_Event* event);
	void update_fps();

/** Definition in Game_Cleanup.cpp */
	void reset_game();							///Resets containers of enemies, towers and variables that change during gameplay.
	void cleanup();								///Cleanup when exiting game.

/** Definition in Game_Init.cpp    */
	bool init();

/** Definition in Game_HandleEvent.cpp */
	bool is_arrow_key(SDL_Event* event);
	void create_new_tower(towers::TowerType tower_type, GridPosition);
	void upgrade_tower(towers::TowerType tower_type);
	void send_new_wave();

	bool optbox_do_selection(Sprite* curr_opt_sel, GridPosition position); ///Passes on to optbox_do_selection(int, int, int)
	bool optbox_do_selection(int type, GridPosition position);
	void arrowkey_bflag_not_set(SDL_Event* event);
	void buildingflag_not_set(SDL_Event* event);
	void buildingflag_set(SDL_Event* event);
	void left_mousebutton(int m_x, int m_y);

	/** Handle event depending on gamestate */
	void state_gameplay_running(SDL_Event* event);
	void state_mainmenu(SDL_Event* event);
	void state_introduction(SDL_Event* event);
	void state_gameover(SDL_Event* event);
	void state_highscore(SDL_Event* event);
	void state_view_help(SDL_Event* event);
	void state_set_highscore(SDL_Event* event);
	void state_ingame_menu(SDL_Event* event);

	/** Main function for handling events */
	/** This function passes event on to above subroutines */
	void handle_event(SDL_Event* event);

/** Definition in Game_HandleSelection.cpp */
	bool sell(Tile*);
	void cancel_selection();
	void select(Tile*);
	void select_from_buildmenu(Tower*);

/** Definition in Game_OptionBox.cpp */
	void compose_box_pos(int &optbox_pos_x, int &optbox_pos_y, Sprite* option_box);
	void toggle_option_box();
	void update_option_box();
	void show_option_box();
	void hide_option_box();
	bool optionbox_visible();

/** Definition in Game_Render.cpp */
	void toggle_fullscreen();
	void draw_dev_screen();
	void draw_introscreen();
	void draw_view_help();
	void draw_mainmenu();
	void draw_highscore();
	void draw_gameover();
	void draw_ingame_menu(); ///Game paused
	void draw_optionbox();
	void draw_selection();
	void draw_enemies();
	void draw_boost_connections();
	void draw_towers();
	void draw_projectiles();
	void draw_menu_towers();
	void draw_build_item();
	void draw_ingame_buttons();
	void draw_ingame_menu_buttons();
	void draw_mainmenu_buttons();
	void draw_money_score();

	void render_gameplay();
	void render(); ///Executes subroutines depending on game_state

/** Definition in Game_UpdateState.cpp */

	/**Update text sprites */
	std::string conv_int_to_str(int i);
	std::string get_lives_str();
	std::string get_money_str();
	std::string get_score_str();
	std::string get_level_str();
	std::string get_timer_str();
	void update_lives();
	void update_money();
	void update_score();
	void update_level();
	void update_timer();
	void update_boost();

	void get_rewards(Enemy* enemy);
	void update_state();

private:
/** Sprite containers */

	std::map<int, Sprite*> optionbox_buttonstorage;
	std::map<int, Sprite*>::iterator iter_map;

	/** Buttons stored in 'optionbox' are pointers to Sprites in 'optionbox_buttonstorage'. */
	/** Don't run delete on these objects! */
	/** On cleanup, run delete on 'optionbox_buttonstorage' instead of 'optionbox' */
	Sprite_List optionbox;
	Sprite_List::iterator iter_op_box;

	TowerList tower_list;
	TowerList::iterator iter_tower;

	TowerList build_list;
	TowerList::iterator iter_build_obj;

	EnemyList enemy_list;
	EnemyList::iterator iter_enemy;

	ProjectileList projectile_list;
	ProjectileList::iterator iter_projectile;

	ButtonList ingame_menu_buttons;
	ButtonList::iterator iter_ingame_menu_button;

	ButtonList mainmenu_buttons;
	ButtonList::iterator iter_mainmenu_button;

	ButtonList ingame_buttons;
	ButtonList::iterator iter_ingame_button;

	Sprite_List::iterator iter_sel;

	HighscoreList highscores;
	HighscoreList::iterator iter_highscore;

	std::vector<Sprite*> highscore_name_sprites;
	std::vector<Sprite*>::iterator iter_highscore_name;

	std::vector<Sprite*> highscore_score_sprites;
	std::vector<Sprite*>::iterator iter_highscore_score;

};

#endif /* GAME_H_ */
