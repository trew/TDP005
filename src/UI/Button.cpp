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

Button::Button(SDL_Renderer* renderer, int type_in, int x, int y, int w, int h, bool can_toggle_in, std::string std_img_in, std::string second_img_in): Sprite(NULL)
{

	if ((texture = load_image(renderer, std_img_in.c_str())) == NULL)
	{
		std::cerr << "Image " << std_img_in << " could not be loaded." << std::endl;
		return;
	}

	if (second_img_in == "UNDEFINED")
		second_texture = NULL;
	else
	{
		if ((second_texture = load_image(renderer, second_img_in.c_str())) == NULL)
		{
			std::cerr << "Image " << second_img_in << " could not be loaded." << std::endl;
			return;
		}
	}

	x_pos = (float)x;
	y_pos = (float)y;
	width = w;
	height = h;
	is_toggled = false;
	can_toggle = can_toggle_in;
	type = type_in;
}

Button::~Button()
{
	if (second_texture != NULL)
	{
		SDL_DestroyTexture(second_texture);
	}
}

void Button::draw(SDL_Renderer* renderer)
{
	if (!visible)
		return;

	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	SDL_Rect dest_rect;

	dest_rect.x = (Sint16)x_pos;
	dest_rect.y = (Sint16)y_pos;

	if(can_toggle) {
		if (is_toggled)
		{
			SDL_QueryTexture(second_texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
			SDL_RenderCopy(renderer, second_texture, NULL, &dest_rect);
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
			SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
		}
	}

	else { // cannot toggle this button
		//If mouse is over button
		if (m_x > x_pos && m_y > y_pos && m_x < x_pos + width && m_y < y_pos + height && second_texture != NULL)
		{
			int textureW, textureH, secondTextureW, secondTextureH;
			SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);
			SDL_QueryTexture(second_texture, NULL, NULL, &secondTextureW, &secondTextureH);

			dest_rect.x = (Sint16)(x_pos - ((secondTextureW - textureW) / 2.0));
			dest_rect.y = (Sint16)(y_pos - ((secondTextureH - textureH) / 2.0));
			dest_rect.w = secondTextureW;
			dest_rect.h = secondTextureH;
			SDL_RenderCopy(renderer, second_texture, NULL, &dest_rect);
		}

		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
			SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
		}
	}
}

void Button::draw(SDL_Renderer* renderer, int x, int y)
{
	if (!visible)
		return;

	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	SDL_Rect dest_rect;

	//If mouse is over button
	if (m_x > x_pos && m_y > y_pos && m_x < x_pos + width && m_y < y_pos + height && second_texture != NULL)
	{
		int textureW, textureH, secondTextureW, secondTextureH;
		SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);
		SDL_QueryTexture(second_texture, NULL, NULL, &secondTextureW, &secondTextureH);

		dest_rect.x = (Sint16)(x - ((secondTextureW - textureW) / 2.0));
		dest_rect.y = (Sint16)(y - ((secondTextureH - textureH) / 2.0));
		dest_rect.w = secondTextureW;
		dest_rect.h = secondTextureH;
		SDL_RenderCopy(renderer, second_texture, NULL, &dest_rect);
	}

	else
	{
		dest_rect.x = x;
		dest_rect.y = y;
		SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h);
		SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
	}
}

void Button::update() {
	if(can_toggle)
		is_toggled = !is_toggled;
}
