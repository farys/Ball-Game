#include "gamestate.h"

GameState::GameState()
{
    this->life = 0;
    this->cristals = 0;
    this->force = Point3f(0, 0, 0);
    this->zeroForce = false;
}

