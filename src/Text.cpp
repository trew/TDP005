/*
 * Text.cpp
 *
 *  Created on: 18 nov 2010
 *      Author: samuel
 */

#include "Text.h"
#include <iostream>

Text::Text()
{
}

Text::Text(std::string input_text, int x, int y, TTF_Font* font_in)
{
	if (input_text == "")
		input_text = " ";

	font = font_in;
	text_color.r = 0x01;
	text_color.g = 0x9F;
	text_color.b = 0xEC;

	sprite_surf = TTF_RenderText_Blended(font, input_text.c_str(), text_color);

	x_pos = x;
	y_pos = y;
	height = sprite_surf->h;
	width = sprite_surf->w;
}

Text::Text(std::string input_text, int r, int g, int b, int x, int y, TTF_Font* font_in) {
	if (input_text == "")
		input_text = " ";

	font = font_in;

	text_color.r = r;
	text_color.g = g;
	text_color.b = b;
	sprite_surf = TTF_RenderText_Blended(font, input_text.c_str(), text_color);
	x_pos = x;
	y_pos = y;
	height = sprite_surf->h;
	width = sprite_surf->w;
}

void Text::update_text(std::string in_text)
{
	SDL_FreeSurface(sprite_surf);

	if (in_text == "") {
		sprite_surf = NULL;
	}

	else
	{
		sprite_surf = TTF_RenderText_Blended(font, in_text.c_str(), text_color);
		height = sprite_surf->h;
		width = sprite_surf->w;
	}
}

Text::~Text()
{
}
