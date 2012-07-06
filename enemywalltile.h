#ifndef ENEMYWALLTILE_H
#define ENEMYWALLTILE_H
#include "maptile.h"

class EnemyWallTile : public MapTile
{
public:
    EnemyWallTile();

    void onCollisionUp(SceneActor *a){
        if(a->getActorType() == ACTOR_ENEMY){
            MapTile::onCollisionUp(a);
        }
    }

    void onCollisionLeft(SceneActor *a){
        if(a->getActorType() == ACTOR_ENEMY){
            MapTile::onCollisionLeft(a);
        }
    }

    void onCollisionRight(SceneActor *a){
        if(a->getActorType() == ACTOR_ENEMY){
            MapTile::onCollisionRight(a);
        }
    }

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){}
};

#endif // ENEMYWALLTILE_H
