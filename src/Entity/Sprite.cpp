/*
 * Sprite.cpp
 *
 *  Created on: 17 nov 2010
 *      Author: samuel
 */

#include <Entity/Sprite.h>
#include <Core/Game.h>
#include <iostream>
#include <cmath>

/* Sprite specific functions! */

Sprite::Sprite(Game* game): game(game)
{
	///Default constructor
	texture = NULL;
	x_pos = 0;
	y_pos = 0;
	height = 0;
	width = 0;
	selected = false;
	visible = true;
	type = 0;
}

Sprite::Sprite(Game* _game, std::string File, int x, int y, int w, int h): game(_game)
{
	/**
	 * Loads an image to the sprites surface and sets position, height and width. Visible sprite is default.
	 */
	if ((texture = load_image(game->getRenderer(), File.c_str())) == NULL)
	{
		std::cerr << "Image " << File << " could not be loaded." << std::endl;
		return;
	}
	x_pos = (float)x;
	y_pos = (float)y;
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
	SDL_DestroyTexture(texture);
	texture = NULL;
}

SDL_Texture* Sprite::load_image(SDL_Renderer* renderer, std::string file)
{
	/**
	 * Load an image file and returns an SDL_Surface* . Returns NULL if image cannot be loaded.
	 */
	SDL_Texture* texture = IMG_LoadTexture(renderer, file.c_str());
	if (texture == NULL)
	{
		std::cerr << "Image " << file << " could not be loaded." << std::endl;
		return NULL;
	}

	return texture;
}

float Sprite::get_distance_to(Sprite *target)
{
	/*
	 * Calculates the distance between two sprites center positions
	 */
	float delta_x = get_center_x() - target->get_center_x();
	float delta_y = get_center_y() - target->get_center_y();
	double distance = sqrt( pow(delta_x, 2.f) + pow(delta_y, 2.f) );
	return (float)distance;
}

float Sprite::get_distance_to_edge(Sprite* target)
{
	float x1 = get_center_x();
	float y1 = get_center_y();
	float x2 = target->get_center_x();
	float y2 = target->get_center_y();

	float deltaY = y1 - y2;
	float deltaX = x1 - x2;

	float angleInRadians = atan2(deltaY, deltaX);

	float extraLength = target->get_width() / 2.f ;

	double distance = sqrt(pow(deltaX, 2.f) + pow(deltaY, 2.f));
	return (float)distance - extraLength;
}

bool Sprite::intersects(const Sprite* const him) {
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

bool Sprite::overlaps(const int &x, const int &y) {
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
const int Sprite::get_height() const
{
	///Returns height of Sprite
	return height;
}
const int Sprite::get_width() const
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
void Sprite::draw(SDL_Renderer* renderer)
{
	/*
	 * Virtual function that draws the sprites surface to the screen using its own X and Y coordinates.
	 */
	draw(renderer, (int)x_pos, (int)y_pos);
}

void Sprite::draw(SDL_Renderer* renderer, int X, int Y)
{
	/*
	 * Virtual function that draws the sprites surface to the screen using its own X and Y coordinates.
	 */
	if (!visible)
		return;
	SDL_Rect dest_rect;
	dest_rect.x = X;
	dest_rect.y = Y;
	SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
	SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
}


/*****************************************************************************************/
/* Text specifics */
void Sprite::update_text(std::string){
	///Pure virtual
}

/*****************************************************************************************/
/* Virtuals */
void Sprite::update(const float&)
{
	///Pure virtual
}

Game* Sprite::get_game() {
	return game;
}
