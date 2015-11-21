/*
 * Text.cpp
 *
 *  Created on: 18 nov 2010
 *      Author: samuel
 */

#include <UI/Text.h>
#include <iostream>

Text::Text(): Sprite(NULL), font(NULL)
{
}

Text::Text(SDL_Renderer* renderer, std::string input_text, int x, int y, TTF_Font* font_in): Sprite(NULL)
{
	if (input_text == "")
		input_text = " ";

	this->renderer = renderer;
	font = font_in;
	text_color.r = 0x01;
	text_color.g = 0x9F;
	text_color.b = 0xEC;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, input_text.c_str(), text_color);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	textSurface = NULL;

	x_pos = x;
	y_pos = y;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

Text::Text(SDL_Renderer* renderer, std::string input_text, int r, int g, int b, int x, int y, TTF_Font* font_in) : Sprite(NULL) {
	if (input_text == "")
		input_text = " ";

	this->renderer = renderer;
	font = font_in;

	text_color.r = r;
	text_color.g = g;
	text_color.b = b;
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, input_text.c_str(), text_color);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	textSurface = NULL;

	x_pos = x;
	y_pos = y;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Text::update_text(std::string in_text)
{
	SDL_DestroyTexture(texture);
	texture = NULL;

	if (in_text != "")
	{
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, in_text.c_str(), text_color);
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}
}

Text::~Text()
{
}
