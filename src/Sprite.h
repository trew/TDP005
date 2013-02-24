/*
 * Sprite.h
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include "Define.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <list>
#include <vector>
#include <string>

class Sprite;
class Tower;
class Enemy;
class Projectile;
typedef std::list<Sprite*> Sprite_List;
typedef std::list<Tower*> TowerList;
typedef std::list<Enemy*> EnemyList;
typedef std::list<Projectile*> ProjectileList;
typedef std::pair<int, int> GridPosition;

/** \brief Sprites are the graphic objects of the game
 *
 * The sprite class contains information about position on screen, visibility and also a lot of virtual functions
 * used by various subclasses such as Tower, Enemy, Projectile and Text.
 *
 */

class Game;
class Sprite {
public:
	/* Constructor & destructor */
	Sprite();			///<Default Constructor
	Sprite(Game* game, std::string File, int x, int y, int w, int h);	///<Constructor using file and position
	virtual ~Sprite();	///<Destructor

	Game* get_game();

	/* Sprite specific functions */
	SDL_Surface* load_image(std::string file);	///<<Load image from file
	void set_type(int newType);	///<Set new type
	void set_x_pos(int X);		///<Set new x pos
	void set_y_pos(int Y);		///<Set new y pos
	void hide();				///<Hide sprite
	void show();				///<Show sprite
	void toggle_visibility();	///<Toggle visibility
	bool is_visible();		///<Is sprite visible?

	int get_x_pos();		///<returns x position
	int get_y_pos();		///<Returns y position
	int get_height();		///<Returns height of Sprite
	int get_width();		///<Returns width of Sprite

	/* Virtual functions for all inherited objects */
	virtual void draw(SDL_Surface* dest_surf);	///<Blit sprite to surface
	virtual void draw(SDL_Surface* dest_surf, int x, int y); ///<Blit sprite to surface
	virtual int get_type();		///<Returns type
	double get_distance_to(Sprite *s); ///<Pure virtual

	virtual void update();			///<Pure virtual
	virtual void update(Sprite_List &object_list);	///<Pure virtual

	/* Tower Specific */
	virtual void update_boost(Sprite_List &tower_list); ///<<Pure virtual
	virtual void shoot_if_possible(Sprite_List &object_list);	///<Pure virtual
	virtual int get_level(); ///<Pure virtual
	virtual int get_damage(); ///<Pure virtual
	virtual int get_boostmod(); ///<Pure virtual
	virtual int get_range(); ///<Pure virtual
	virtual int get_cost_buy(); ///<Pure virtual
	virtual int get_cost_upgrade(); ///<Pure virtual
	virtual int get_sell_value(); ///<Pure virtual
	virtual void add_to_sell_value(int cost); ///<Pure virtual
	virtual std::string get_type_str(); ///<Pure virtual
	virtual std::string get_range_str(); ///<Pure virtual
	virtual std::string get_damage_str(); ///<Pure virtual
	virtual std::string get_level_str(); ///<Pure virtual

	virtual bool has_this_target(Sprite* target); ///<Pure virtual
	virtual void null_current_target(); ///<Pure virtual

	virtual Sprite_List get_infosprites(); ///<pure virtual
	virtual bool upgrade(int);		///<Pure virtual
	virtual void clear_selected();	///<Pure virtual
	virtual void set_selected();	///<Pure virtual
	virtual void display_range(SDL_Surface* dest_surf, int x_pos, int y_pos);	///<Pure virtual

	/* Enemy specific */
	virtual void take_damage(int);///<Pure virtual
	virtual bool is_killed();///<Pure virtual
	virtual bool has_reached_goal();///<Pure virtual
	virtual void update_path();///<Pure virtual
	virtual bool try_update_path();///<Pure virtual
	virtual int get_reward_money();	///<Pure virtual
	virtual int get_reward_score();	///<Pure virtual

	/******************/


	/* Text specific */
	virtual void update_text(std::string);		///<Pure virtual

	/* Projectile specific */
	virtual bool is_dead();		///<Pure virtual


protected:
	SDL_Surface* sprite_surf;					///<Surface containing the image
	int type;									///<Current type
	int x_pos;									///<X position
	int y_pos;									///<Y position
	int height;									///<Height
	int width;									///<width
	bool selected;								///<Selected or not
	bool visible;								///<Visible or not
	std::vector<Sprite*> infosprites;			///<List of all Textsprites
	Sprite_List visible_infosprites;			///<List containing those textsprites that are currently visible

private:
	Game* game;
};

#endif /* SPRITE_H_ */
