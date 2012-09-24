#include "player.h"

Player::Player(){
this->setActorType(ACTOR_PLAYER);
}

void Player::reset(){
    this->setForce(new Point3f(0, 0, 0));
    this->gameState = GameState();
}
