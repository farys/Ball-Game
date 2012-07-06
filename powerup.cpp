#include "powerup.h"

PowerUp::PowerUp()
{
    this->setActorType(ACTOR_POWERUP);
    this->setBarrier(false);
    this->setCalculatingPsychics(true);
    this->setCollisionDetection(true);
    this->taken = false;
}

void PowerUp::setTakenAnimation(AnimationImage *takenAnimation)
{
    this->takenAnimation = takenAnimation;
}
