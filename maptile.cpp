#include "maptile.h"

MapTile::MapTile()
{
    this->setActorType(ACTOR_MAP_TILE);
    //this->setShapeConditions(new ShapeConditions(SHAPE_CUBE, 0, Point3f(-0.5,0,-0.5), Point3f(0.5,1,0.5)));
    this->setCollisionDetection(true);
}

void MapTile::onCollisionUp(SceneActor *a, bool jump){
        Point3f f = a->getForce();

        f.y = 0;
        a->setForce(&f);
        a->gameState.force.y = jump? 4 : 3;
}

void MapTile::onCollisionLeft(SceneActor *a){
    Point3f f = a->getForce();

    a->gameState.force.x = -f.x;
    f.x = 0;
    a->setForce(&f);
}

void MapTile::onCollisionRight(SceneActor *a){
    onCollisionLeft(a);
}

void MapTile::onCollisionDown(SceneActor *a){
        Point3f f = a->getForce();

        a->gameState.force.y  = -f.y;
        f.y = 0;
        a->setForce(&f);
}
