#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "point3f.h"
class GameState
{
public:
    GameState();

    int life;
    Point3f force;
    int cristals;
    bool zeroForce;

    bool operator !=(GameState b);
    GameState operator+(GameState b);
    GameState operator-(GameState b);

};

#endif // GAMESTATE_H
