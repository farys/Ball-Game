#include "maptile.h"

MapTile::MapTile()
{
    this->setActorType(ACTOR_MAP_TILE);
    //this->setShapeConditions(new ShapeConditions(SHAPE_CUBE, 0, Point3f(-0.5,0,-0.5), Point3f(0.5,1,0.5)));
    this->setCollisionDetection(true);
}
