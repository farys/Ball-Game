#include "character.h"

Character::Character()
{
    /*stand = (AnimationImage*) &animations->at(baseAnimationId+ANIM_STAND);
    left = (AnimationImage*) &animations->at(baseAnimationId+ANIM_LEFT);
    right = (AnimationImage*) &animations->at(baseAnimationId+ANIM_RIGHT);
    jump = (AnimationImage*) &animations->at(baseAnimationId+ANIM_JUMP);
    dead = (AnimationImage*) &animations->at(baseAnimationId+ANIM_DEAD);
    this->setAnimation(stand);*/
    this->setActorType(ACTOR_CHARACTER);
}

void Character::onChangeGameState(){

    /*if(this->gameState.force.x() > 0)
        this->setAnimation(right);
    else if(this->gameState.force.x() < 0)
        this->setAnimation(left);

    if(this->gameState.zeroForce){
        this->setAnimation(stand);
    }*/
}
