#ifndef SCENEACTOR_H
#define SCENEACTOR_H

#include "config.h"
#include "gamestate.h"
#include <QMessageBox>
#include "point3f.h"

#define ACTOR_MAP_TILE 0
#define ACTOR_CHARACTER 1
#define ACTOR_ENEMY 2
#define ACTOR_PLAYER 3
#define ACTOR_RESPAWN 4
#define ACTOR_POWERUP 5
#define ACTOR_WALLTILE 6

#define SHAPE_CUBE 0
#define SHAPE_SPHERE 1

class SceneActor
{

public:
    SceneActor();

    void setPosition(Point3f *pos);
    Point3f* getPosition();

    short getActorType();
    void setActorType(short t);

    Point3f getForce();
    void setForce(Point3f *f);

    bool calculatePsychics();
    void setCalculatingPsychics(bool p);

    bool getBarrier();
    void setBarrier(bool b);

    void setFriction(qreal f);
    qreal getFriction();

    void setModelId(uint listId);
    uint getModelId();

    bool calculateCollisions();
    void setCollisionDetection(bool c);

    QString toString();

    void update();

    void onCollisionUp(SceneActor *){}
    void onCollisionLeft(SceneActor *){}
    void onCollisionRight(SceneActor *){}
    void onCollisionDown(SceneActor *){}
    void onEndOfAnimation(){}
    void onChangeGameState(){}
    void onEveryFrame(){}

    void msg(QString txt);

    GameState gameState;
private:

    Point3f pos;
    Point3f force;
    short actorType;
    uint modelId;
    qreal friction;
    int life;
    bool psychic;
    bool barrier;
    bool calcCollision;
};

#endif // SCENEACTOR_H
