#include <State/State.h>
#include <Core/Game.h>
#include <Core/GameEngine.h>

State::State(Game* const game) : game(game), renderer(game->getEngine()->getRenderer())
{
}
