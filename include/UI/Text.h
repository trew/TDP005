/*
 * Text.h
 *
 *  Created on: 18 nov 2010
 *      Author: samuel
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <Entity/Sprite.h>
#include <SDL_ttf.h>
#include <string>

/**	\brief Text objects
 *
 * Creating a new text object will result in an image extracted from TTF_RenderText_Blended.
 * Using text objects is useful because you can just update the text in the object at any time.
 */

class Text: public Sprite {
public:
	Text();				///< Default
	Text(SDL_Renderer* renderer, std::string input_text, int x, int y, TTF_Font* font_in);							///< New text using standard color
	Text(SDL_Renderer* renderer, std::string input_text, int r, int g, int b, int x, int y, TTF_Font* font_in);		///< New text with custom color
	std::string const getSourceText() const { return source; }
	void update_text(std::string in_text) override;		///<Replace text
	virtual ~Text();				///< Destructor

private:
	TTF_Font* font;
	SDL_Color text_color;
	SDL_Renderer* renderer;
	std::string source;
};

#endif /* TEXT_H_ */
