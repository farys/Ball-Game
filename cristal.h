#ifndef CRISTAL_H
#define CRISTAL_H
#include "sceneactor.h"

class Cristal : public SceneActor
{
public:
    Cristal();

    void onCollision(SceneActor *a){
        if(this->taken) return;

        a->gameState.cristals++;
        this->taken = true;
        delete this;
    }

    void onCollisionUp(SceneActor *a){
        onCollision(a);
    }

    void onCollisionLeft(SceneActor *a){
        onCollision(a);
    }

    void onCollisionRight(SceneActor *a){
        onCollision(a);
    }

    bool taken;
    float rotateZ;
};

#endif // POWERUP_H
