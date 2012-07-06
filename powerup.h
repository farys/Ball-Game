#ifndef POWERUP_H
#define POWERUP_H
#include "sceneactor.h"
#include "animationimage.h"

class PowerUp : public SceneActor
{
public:
    PowerUp();

    void onCollision(SceneActor *a){
        if(this->taken) return;

        a->gameState.gold++;
        this->setAnimation(takenAnimation);
        this->taken = true;
        this->setCollisionDetection(false);
    }

    void setTakenAnimation(AnimationImage *takenAnimation);

    void onEndOfAnimation(){
        if(taken)
        this->removeFromIndex();
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
    AnimationImage *takenAnimation;
    bool taken;
};

#endif // POWERUP_H
