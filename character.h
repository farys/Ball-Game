#ifndef CHARACTER_H
#define CHARACTER_H

#include <QList>
#include "sceneactor.h"

class Character : public SceneActor
{
public:
    Character();

    void onChangeGameState();
};

#endif // CHARACTER_H
