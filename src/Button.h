/*
 * Button.h
 *
 *  Created on: Nov 30, 2010
 *      Author: bjorn
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Define.h"
#include "Sprite.h"

/**
 * \brief The Button class
 *
 * This class handles the buttons in the game.
 */
class Button : public Sprite
{
public:
	Button(int typeIn, int X, int Y, int W, int H, bool can_toggle_in, std::string std_img_in, std::string second_img_in = "UNDEFINED"); ///<Constructor
	virtual ~Button(); ///<Deconstructor

	void draw(SDL_Surface* dest_surf); ///<Method for drawing the button at its position
	void draw(SDL_Surface* dest_surf, int X, int Y); ///<Method for drawing the button at (X, Y)
	void update(); ///<Updates the button's state, toggling on/off

private:
	SDL_Surface* second_surf;
	bool can_toggle;
	bool is_toggled;
};

#endif /* BUTTON_H_ */
