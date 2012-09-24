#ifndef MAPTILE_H
#define MAPTILE_H
#include "sceneactor.h"

class MapTile : public SceneActor
{
public:
    MapTile();

    void onCollisionUp(SceneActor *a, bool jump);
    void onCollisionLeft(SceneActor *a);
    void onCollisionRight(SceneActor *a);
    void onCollisionDown(SceneActor *a);
};

#endif // MAPTILE_H
