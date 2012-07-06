#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"

class Player : public Character
{
public:
    Player(){
    this->setActorType(ACTOR_PLAYER);
        //this->setFriction(0.3f);
    }
};

#endif // PLAYER_H
