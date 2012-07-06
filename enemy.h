#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"

class Enemy : public Character
{
public:
    Enemy(QList<AnimationImage> *animations, int baseAnimationId) : Character(animations, baseAnimationId){
    this->setActorType(ACTOR_ENEMY);
    }

    void onEveryFrame(){
        /*int r = rand() % 100;
        if(r < 10){
            this->gameState.force.setX(-5);
        }

        if(r > 10 && r < 30){
            this->gameState.force.setX(5);
        }
*/
    }

    void onCollisionUp(SceneActor *){
        this->removeFromIndex();
    }
};

#endif // ENEMY_H
