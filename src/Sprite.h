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
	void set_x_pos(float X);		///<Set new x pos
	void set_y_pos(float Y);		///<Set new y pos
	void hide();				///<Hide sprite
	void show();				///<Show sprite
	void toggle_visibility();	///<Toggle visibility
	bool is_visible();		///<Is sprite visible?

	float get_x_pos();		///<returns x position
	float get_y_pos();		///<Returns y position
	int get_height();		///<Returns height of Sprite
	int get_width();		///<Returns width of Sprite

	/* Virtual functions for all inherited objects */
	virtual void draw(SDL_Surface* dest_surf);	///<Blit sprite to surface
	virtual void draw(SDL_Surface* dest_surf, int x, int y); ///<Blit sprite to surface
	virtual int get_type();		///<Returns type
	float get_distance_to(Sprite *s); ///<Pure virtual

	virtual void update();			///<Pure virtual

	virtual bool intersects(Sprite*);
	virtual bool overlaps(int x, int y);

	/* Text specific */
	virtual void update_text(std::string);		///<Pure virtual

protected:
	SDL_Surface* sprite_surf;					///<Surface containing the image
	int type;									///<Current type
	float x_pos;									///<X position
	float y_pos;									///<Y position
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
