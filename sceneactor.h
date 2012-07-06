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
    Point3f* getPosition(){ return &this->pos;}

    short getActorType(){return this->actorType;}
    void setActorType(short t){this->actorType = t;}

    Point3f getForce(){ return this->force;}
    void setForce(Point3f *f){ this->force = *f;}

    bool calculatePsychics(){ return this->psychic;}
    void setCalculatingPsychics(bool p){ this->psychic = p;}

    bool getBarrier(){ return this->barrier;}
    void setBarrier(bool b){ this->barrier = b;}

    bool calculateCollisions(){return this->calcCollision;}
    void setCollisionDetection(bool c){this->calcCollision = c;}

    QString toString();

    void update(){

        if(this->gameState != GameState()){
           this->onChangeGameState();
        }
    }

    void onCollisionUp(SceneActor *){}
    void onCollisionLeft(SceneActor *){}
    void onCollisionRight(SceneActor *){}
    void onCollisionDown(SceneActor *){}
    void onEndOfAnimation(){}
    void onChangeGameState(){}
    void onEveryFrame(){}

    GameState gameState;

    void msg(QString txt){
        QMessageBox msg;
        msg.setText(txt);
        msg.exec();
    }

    void setFriction(qreal f){this->friction = f;}
    qreal getFriction(){return this->friction;}

    void setModelId(uint listId){this->modelId = listId;}
    uint getModelId(){return modelId;}

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
