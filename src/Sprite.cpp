/*
 * Sprite.cpp
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#include "Sprite.h"
#include <iostream>
#include <cmath>

/* Sprite specific functions! */

Sprite::Sprite()
{
	///Default constructor
	sprite_surf = NULL;
	x_pos = 0;
	y_pos = 0;
	height = 0;
	width = 0;
	selected = false;
	visible = true;
}

Sprite::Sprite(Game* game, std::string File, int x, int y, int w, int h): game(game)
{
	/**
	 * Loads an image to the sprites surface and sets position, height and width. Visible sprite is default.
	 */
	if ((sprite_surf = load_image(File.c_str())) == NULL)
	{
		std::cerr << "Image " << File << " could not be loaded." << std::endl;
		return;
	}
	x_pos = x;
	y_pos = y;
	height = h;
	width = w;
	selected = false;
	visible = true;
}

Sprite::~Sprite()
{
	/**
	 * Free sprite surface and set to NULL.
	 */
	if (sprite_surf != NULL)
		SDL_FreeSurface(sprite_surf);
	sprite_surf = NULL;
}

SDL_Surface* Sprite::load_image(std::string file)
{
	/**
	 * Load an image file and returns an SDL_Surface* . Returns NULL if image cannot be loaded.
	 */
	SDL_Surface* temp_surf;
	SDL_Surface* return_surf;
	if ((temp_surf = IMG_Load(file.c_str())) == NULL)
	{
		std::cerr << "Image " << file << " could not be loaded." << std::endl;
		return NULL;
	}
	return_surf = SDL_DisplayFormatAlpha(temp_surf);
	SDL_FreeSurface(temp_surf);
	return return_surf;

}

double Sprite::get_distance_to(Sprite *target)
{
	/*
	 * Calculates the distance between two sprites center positions
	 */
	double delta_x = (double)(x_pos + (width / 2) - (target->get_x_pos() + target->get_width() / 2));
	double delta_y = (double)(y_pos + (height / 2) - (target->get_y_pos() + target->get_height() / 2));
	double distance = sqrt(pow(delta_x, 2.0) + pow(delta_y, 2.0));
	return distance;
}

/* Setters and getters for Sprite */

bool Sprite::is_visible()
{
	///Is sprite visible?
	return visible;
}

void Sprite::hide()
{
	///Hide sprite
	visible = false;
}

void Sprite::show()
{
	///Show sprite
	visible = true;
}

void Sprite::toggle_visibility()
{
	///Toggle visibility
	visible = !visible;
}

void Sprite::set_type(int new_type)
{
	///Set new type to sprite
	type = new_type;
}

void Sprite::set_x_pos(int x)
{
	///Set x position
	x_pos = x;
}
void Sprite::set_y_pos(int y)
{
	///Set y position
	y_pos = y;
}

int Sprite::get_x_pos()
{
	///Returns x position
	return x_pos;
}

int Sprite::get_y_pos()
{
	///Returns y position
	return y_pos;
}
int Sprite::get_height()
{
	///Returns height of Sprite
	return height;
}
int Sprite::get_width()
{
	///Returns width of Sprite
	return width;
}

int Sprite::get_type()
{
	///Returns type
	return type;
}
/* End setters and getters for Sprite */

/* Virtual functions */
void Sprite::draw(SDL_Surface* dest_surf)
{
	/*
	 * Virtual function that draws the sprites surface to the screen using its own X and Y coordinates.
	 */
	draw(dest_surf, x_pos, y_pos);
}

void Sprite::draw(SDL_Surface* dest_surf, int X, int Y)
{
	/*
	 * Virtual function that draws the sprites surface to the screen using its own X and Y coordinates.
	 */
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = X;
	dest_rect.y = Y;
	SDL_BlitSurface(sprite_surf, NULL, dest_surf, &dest_rect);
}


/*****************************************************************************************/
/*
 * Tower specific virtuals
 */
void Sprite::update_boost(Sprite_List &tower_list)
{
	///Pure virtual
}

void Sprite::shoot_if_possible(Sprite_List &tower_list)
{
	///Pure virtual
}

int Sprite::get_level()
{
	///Pure virtual
	return 0;
}
int Sprite::get_damage()
{
	///Pure virtual
	return 0;
}
int Sprite::get_boostmod()
{
	///Pure virtual
	return 0;
}
int Sprite::get_range()
{
	///Pure virtual
	return 0;
}
int Sprite::get_cost_buy()
{
	///Pure virtual
	return 0;
}
int Sprite::get_cost_upgrade()
{
	///Pure virtual
	return 0;
}
int Sprite::get_sell_value() {
	///Pure virtual
	return 0;
}
void Sprite::add_to_sell_value(int cost) {
	///Pure virtual
}
std::string Sprite::get_type_str()
{
	///Pure virtual
	return "";
}
std::string Sprite::get_range_str()
{
	///Pure virtual
	return "";
}
std::string Sprite::get_damage_str()
{
	///Pure virtual
	return "";
}
std::string Sprite::get_level_str()
{
	///Pure virtual
	return "";
}

bool Sprite::has_this_target(Sprite* target) {
	///Pure virtual
	return true;
}
void Sprite::null_current_target() {
	///Pure virtual
}
Sprite_List Sprite::get_infosprites()
{
	///Applies to Tower currently, might apply to Enemy
	return visible_infosprites;
}
bool Sprite::upgrade(int input)
{
	///Pure virtual
	return true;
}
void Sprite::clear_selected()
{
	///Pure virtual
}
void Sprite::set_selected()
{
	///Pure virtual
}
void Sprite::display_range(SDL_Surface* destSurf, int x_pos, int y_pos)
{
	///Pure virtual
}


/*****************************************************************************************/
/*
 * Enemy virtuals
 */
bool Sprite::is_killed()
{
	///Pure virtual
	return false;
}
bool Sprite::has_reached_goal()
{
	///Pure virtual
	return false;
}
void Sprite::update_path() {
	///Pure virtual
}
bool Sprite::try_update_path() {
	///Pure virtual
	return true;
}
void Sprite::take_damage(int)
{
	///Pure virtual
}
int Sprite::get_reward_money()
{
	///Pure virtual.
	return 0;
}

int Sprite::get_reward_score()
{
	///Pure virtual.
	return 0;
}

/*****************************************************************************************/
/* Text specifics */
void Sprite::update_text(std::string){
	///Pure virtual
}

/*****************************************************************************************/
/* Projectile specific */
bool Sprite::is_dead() {
	///Pure virtual
	return false;
}


/*****************************************************************************************/
/* Virtuals */
void Sprite::update()
{
	///Pure virtual
}
void Sprite::update(Sprite_List &object_list)
{
	///Pure virtual
}

Game* Sprite::get_game() {
	return game;
}
