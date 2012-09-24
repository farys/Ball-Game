#ifndef CRISTAL_H
#define CRISTAL_H
#include "sceneactor.h"

class Cristal : public SceneActor
{
public:
    Cristal();

    void onCollision(SceneActor *a);

    void onCollisionUp(SceneActor *a);
    void onCollisionLeft(SceneActor *a);
    void onCollisionRight(SceneActor *a);

    bool taken;
    float rotateZ;
};

#endif // POWERUP_H
