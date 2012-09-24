#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"
#include "gamestate.h"

class Player : public Character
{
public:
    Player();
    void reset();
};

#endif // PLAYER_H
