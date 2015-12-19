#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <SDL.h>

namespace Utils
{

std::string itos(int i);
std::string ftos(float);
const bool isArrowKey(const SDL_Event &ev);
}

#endif