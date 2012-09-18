#include "cristal.h"
#include "math.h"

Cristal::Cristal()
{
    this->rotateZ = rand() % 360;
    this->setActorType(ACTOR_POWERUP);
    this->setBarrier(false);
    this->setCalculatingPsychics(true);
    this->setCollisionDetection(true);
    this->taken = false;
}
