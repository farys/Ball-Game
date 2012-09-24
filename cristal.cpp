#include "cristal.h"
#include "math.h"

Cristal::Cristal(){
    this->rotateZ = rand() % 360;
    this->setActorType(ACTOR_POWERUP);
    this->setBarrier(false);
    this->setCalculatingPsychics(true);
    this->setCollisionDetection(true);
    this->taken = false;
}

void Cristal::onCollision(SceneActor *a){
    if(this->taken) return;

    a->gameState.cristals++;
    this->taken = true;
    delete this;
}

void Cristal::onCollisionUp(SceneActor *a){
    onCollision(a);
}

void Cristal::onCollisionLeft(SceneActor *a){
    onCollision(a);
}

void Cristal::onCollisionRight(SceneActor *a){
    onCollision(a);
}
