/*
 * Button.h
 *
 *  Created on: Nov 30, 2010
 *      Author: bjorn
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Core/Define.h>
#include <Entity/Sprite.h>
#include <functional>

/**
 * \brief The Button class
 *
 * This class handles the buttons in the game.
 */
class Button : public Sprite
{
private:
	Button(SDL_Renderer* renderer, int typeIn, int X, int Y, int W, int H, bool can_toggle_in, std::string std_img_in, std::string second_img_in = "UNDEFINED"); ///<Constructor
public:
	Button(SDL_Renderer* const renderer, const int X, const int Y, const int W, const int H, const std::string std_img_in); ///<Constructor
	Button(SDL_Renderer* const renderer, const int X, const int Y, const int W, const int H, const std::string std_img_in, const std::function<bool(Button* const)> action); ///<Constructor
	Button(SDL_Renderer* const renderer, const int X, const int Y, const int W, const int H, const std::string std_img_in, const std::string secondImg, std::function<bool(Button* const)> action); ///<Constructor
	Button(SDL_Renderer* const renderer, const int X, const int Y, const int W, const int H, const std::string std_img_in, const std::string secondImg, bool toggleable, std::function<bool(Button* const)> action); ///<Constructor
	virtual ~Button(); ///<Deconstructor

	void draw(SDL_Renderer* renderer); ///<Method for drawing the button at its position

	void setToggle(const bool toggled) { is_toggled = toggled; }
	const bool isToggle() const { return is_toggled; }

	bool performAction();
	bool hasAction() const { return action.operator bool(); }

	const bool isToggleable() const { return can_toggle; }
	void setToggleable(const bool toggle) { can_toggle = toggle; }

private:
	std::function<bool(Button* const)> action;
	int type;
	SDL_Texture* second_texture;
	bool can_toggle;
	bool is_toggled;
};

#endif /* BUTTON_H_ */
