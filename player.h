#ifndef PLAYER_H
#define PLAYER_H
#include "gamestate.h"
#include "sceneactor.h"

class Player : public SceneActor
{
public:
    Player();
    void reset();
};

#endif // PLAYER_H
