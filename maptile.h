#ifndef MAPTILE_H
#define MAPTILE_H
#include "sceneactor.h"

class MapTile : public SceneActor
{
public:
    MapTile();

    void onCollisionUp(SceneActor *a){
            Point3f f = a->getForce();
            //Point3f *p = a->getPosition();

            f.y = 0;
            a->setForce(&f);
            a->gameState.force.y  = 3;
            //p->y = this->getPosition()->y + 12;
    }

    void onCollisionLeft(SceneActor *a){
        Point3f f = a->getForce();

        a->gameState.force.x = -f.x;
        f.x = 0;
        a->setForce(&f);
    }

    void onCollisionRight(SceneActor *a){
        onCollisionLeft(a);
    }

    void onCollisionDown(SceneActor *a){
            Point3f f = a->getForce();

            a->gameState.force.y  = -f.y;
            f.y = 0;
            a->setForce(&f);
    }




};

#endif // MAPTILE_H
