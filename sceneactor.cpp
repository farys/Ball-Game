#include "sceneactor.h"
#include <math.h>

SceneActor::SceneActor(){
    this->barrier = false;
    this->psychic = false;
    this->life = 1;
    this->friction = 1;
    this->calcCollision = true;
}

void SceneActor::setFriction(qreal f){
    this->friction = f;
}

qreal SceneActor::getFriction(){
    return this->friction;
}

void SceneActor::setModelId(uint listId){
    this->modelId = listId;
}

uint SceneActor::getModelId(){
    return modelId;
}

void SceneActor::setPosition(Point3f *p){
    this->pos = *p;
}

Point3f* SceneActor::getPosition(){
    return &this->pos;
}

short SceneActor::getActorType(){
    return this->actorType;
}

void SceneActor::setActorType(short t){
    this->actorType = t;
}

Point3f SceneActor::getForce(){
    return this->force;
}

void SceneActor::setForce(Point3f *f){
    this->force = *f;
}

bool SceneActor::calculatePsychics(){
    return this->psychic;
}

void SceneActor::setCalculatingPsychics(bool p){
    this->psychic = p;
}

bool SceneActor::getBarrier(){
    return this->barrier;
}

void SceneActor::setBarrier(bool b){
    this->barrier = b;
}

bool SceneActor::calculateCollisions(){
    return this->calcCollision;
}

void SceneActor::setCollisionDetection(bool c){
    this->calcCollision = c;
}

void SceneActor::update(){

    if(this->gameState != GameState()){
       this->onChangeGameState();
    }
}

void SceneActor::msg(QString txt){
    QMessageBox msg;
    msg.setText(txt);
    msg.exec();
}


