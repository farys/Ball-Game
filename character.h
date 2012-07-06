#ifndef CHARACTER_H
#define CHARACTER_H

#include <QList>
#include "sceneactor.h"

#define ANIM_STAND 0
#define ANIM_LEFT 1
#define ANIM_RIGHT 2
#define ANIM_JUMP 3
#define ANIM_DEAD 4

class Character : public SceneActor
{
public:
    Character();

    void onChangeGameState();

private:
    /*AnimationImage *stand;
    AnimationImage *left;
    AnimationImage *right;
    AnimationImage *jump;
    AnimationImage *dead;*/
};

#endif // CHARACTER_H
