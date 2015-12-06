#include <Utils/Utils.h>
#include <SDL.h>

namespace Utils
{

std::string itos(int i)
{
	std::stringstream s_stream;
	s_stream << i;
	return s_stream.str();
}

std::string ftos(float f)
{
	std::stringstream ss;
	ss << f;
	return ss.str();
}

const bool isArrowKey(const SDL_Event &ev)
{
	return ev.key.keysym.sym == SDLK_LEFT ||
		ev.key.keysym.sym == SDLK_RIGHT ||
		ev.key.keysym.sym == SDLK_UP ||
		ev.key.keysym.sym == SDLK_DOWN;
}

} // namespace Utils