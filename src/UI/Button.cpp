/*
 * Button.cpp
 *
 *  Created on: Nov 30, 2010
 *      Author: bjorn
 */

#include <UI/Button.h>
#include <string>
#include <iostream>
using namespace std;

Button::Button(int type_in, int x, int y, int w, int h, bool can_toggle_in, std::string std_img_in, std::string second_img_in): Sprite(NULL)
{

	if ((sprite_surf = load_image(std_img_in.c_str())) == NULL)
	{
		std::cerr << "Image " << std_img_in << " could not be loaded." << std::endl;
		return;
	}

	if (second_img_in == "UNDEFINED")
		second_surf = NULL;
	else
	{
		if ((second_surf = load_image(second_img_in.c_str())) == NULL)
		{
			std::cerr << "Image " << second_img_in << " could not be loaded." << std::endl;
			return;
		}
	}

	x_pos = x;
	y_pos = y;
	width = w;
	height = h;
	is_toggled = false;
	can_toggle = can_toggle_in;
	type = type_in;
}

Button::~Button()
{
	if (second_surf != NULL)
		SDL_FreeSurface(second_surf);

}

void Button::draw(SDL_Surface* dest_surf)
{
	if (!visible)
		return;

	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	SDL_Rect dest_rect;

	dest_rect.x = (Sint16)x_pos;
	dest_rect.y = (Sint16)y_pos;

	if(can_toggle) {
		if(is_toggled)
			SDL_BlitSurface(second_surf, NULL, dest_surf, &dest_rect);
		else
			SDL_BlitSurface(sprite_surf, NULL, dest_surf, &dest_rect);
	}

	else { // cannot toggle this button
		//If mouse is over button
		if (m_x > x_pos && m_y > y_pos && m_x < x_pos + width && m_y < y_pos + height && second_surf != NULL)
		{
			dest_rect.x = (Sint16)(x_pos - ((second_surf->w - sprite_surf->w) / 2.0));
			dest_rect.y = (Sint16)(y_pos - ((second_surf->h - sprite_surf->h) / 2.0));
			SDL_BlitSurface(second_surf, NULL, dest_surf, &dest_rect);
		}

		else
		{
			SDL_BlitSurface(sprite_surf, NULL, dest_surf, &dest_rect);
		}
	}
}

void Button::draw(SDL_Surface* dest_surf, int x, int y)
{
	if (!visible)
		return;

	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	SDL_Rect dest_rect;

	//If mouse is over button
	if (m_x > x_pos && m_y > y_pos && m_x < x_pos + width && m_y < y_pos + height && second_surf != NULL)
	{
		dest_rect.x = (Sint16)(x - ((second_surf->w - sprite_surf->w)/2.0));
		dest_rect.y = (Sint16)(y - ((second_surf->h - sprite_surf->h)/2.0));
		SDL_BlitSurface(second_surf, NULL, dest_surf, &dest_rect);
	}

	else
	{
		dest_rect.x = x;
		dest_rect.y = y;
		SDL_BlitSurface(sprite_surf, NULL, dest_surf, &dest_rect);
	}
}

void Button::update() {
	if(can_toggle)
		is_toggled = !is_toggled;
}
