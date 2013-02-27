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

Sprite::Sprite(Game* game): game(game)
{
	///Default constructor
	sprite_surf = NULL;
	x_pos = 0;
	y_pos = 0;
	height = 0;
	width = 0;
	selected = false;
	visible = true;
	type = 0;
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

float Sprite::get_distance_to(Sprite *target)
{
	/*
	 * Calculates the distance between two sprites center positions
	 */
	float delta_x = x_pos + (width / 2) - (target->get_x() + target->get_width() / 2);
	float delta_y = y_pos + (height / 2) - (target->get_y() + target->get_height() / 2);
	double distance = sqrt( pow(delta_x, 2) + pow(delta_y, 2) );
	return (float)distance;
}

bool Sprite::intersects(Sprite* him) {
	/**
	 * Perform simple bounding box detection.
	 * Returns true if the other sprite intersects with us
	 */
	if (y_pos + height < him->y_pos) return false;
	if (y_pos > him->y_pos + him->height) return false;

	if (x_pos + width < him->x_pos) return false;
	if (x_pos > him->x_pos + him->width) return false;

	return true;
}

bool Sprite::overlaps(int x, int y) {
	/**
	 * Returns true if the pixel can be found within this sprite
	 */
	if (x < x_pos) return false;
	if (x > x_pos + width) return false;
	if (y < y_pos) return false;
	if (y > y_pos + height) return false;
	return true;
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

void Sprite::set_center_x(float _x) {
	x_pos = _x - width / 2.f;
}

void Sprite::set_center_y(float _y) {
	y_pos = _y - height / 2.f;
}

void Sprite::set_x(float x)
{
	///Set x position
	x_pos = x;
}
void Sprite::set_y(float y)
{
	///Set y position
	y_pos = y;
}

float Sprite::get_center_x() {
	return x_pos + width / 2.f;
}

float Sprite::get_center_y() {
	return y_pos + height / 2.f;
}

float Sprite::get_x()
{
	///Returns x position
	return x_pos;
}

float Sprite::get_y()
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

int Sprite::get_int_type()
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
	draw(dest_surf, (int)x_pos, (int)y_pos);
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
/* Text specifics */
void Sprite::update_text(std::string){
	///Pure virtual
}

/*****************************************************************************************/
/* Virtuals */
void Sprite::update(int delta)
{
	///Pure virtual
}

Game* Sprite::get_game() {
	return game;
}
